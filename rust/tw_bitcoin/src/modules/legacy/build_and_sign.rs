use crate::entry::{ProtoInputRecipient, ProtoOutputRecipient};
use crate::Result;
use bitcoin::absolute::LockTime;
use bitcoin::consensus::Decodable;
use bitcoin::key::TapTweak;
use bitcoin::taproot::{LeafVersion, NodeInfo, TapNodeHash, TaprootSpendInfo};
use bitcoin::{PublicKey, ScriptBuf, Witness};
use secp256k1::XOnlyPublicKey;
use std::borrow::Cow;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_encoding::hex;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_misc::traits::ToBytesVec;
use tw_misc::try_or_else;
use tw_proto::Bitcoin::Proto as LegacyProto;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Common::Proto as CommonProto;
use tw_proto::Utxo::Proto as UtxoProto;

#[no_mangle]
pub unsafe extern "C" fn tw_taproot_build_and_sign_transaction(
    input: *const u8,
    input_len: usize,
) -> CByteArray {
    let data = CByteArrayRef::new(input, input_len)
        .to_vec()
        .unwrap_or_default();

    let proto: LegacyProto::SigningInput =
        try_or_else!(tw_proto::deserialize(&data), CByteArray::null);
    let signing: LegacyProto::SigningOutput =
        try_or_else!(taproot_build_and_sign_transaction(proto), CByteArray::null);

    let _serialized =
        tw_proto::serialize(&signing).expect("failed to serialize signed transaction");

    todo!()
}

pub fn taproot_build_and_sign_transaction(
    legacy: LegacyProto::SigningInput,
) -> Result<LegacyProto::SigningOutput> {
    // Convert the appropriate lock time.
    let native_lock_time = LockTime::from_consensus(legacy.lock_time);
    let lock_time = match native_lock_time {
        LockTime::Blocks(blocks) => UtxoProto::LockTime {
            variant: UtxoProto::mod_LockTime::OneOfvariant::blocks(blocks.to_consensus_u32()),
        },
        LockTime::Seconds(seconds) => UtxoProto::LockTime {
            variant: UtxoProto::mod_LockTime::OneOfvariant::seconds(seconds.to_consensus_u32()),
        },
    };

    // Prepare the inputs.
    let mut inputs = vec![];

    // If a plan exists, we will use the provided one and interpret it as
    // `InputSelector::UseAll`.
    let input_selector = if let Some(plan) = legacy.plan {
        for utxo in plan.utxos {
            inputs.push(input_from_legacy_utxo(utxo))
        }

        UtxoProto::InputSelector::UseAll
    }
    // If there is no plan, we will construct it for the user and therefore
    // interpret it as `InputSelector::SelectAscending`.
    else {
        for utxo in legacy.utxo {
            inputs.push(input_from_legacy_utxo(utxo))
        }

        UtxoProto::InputSelector::SelectAscending
    };

    // The primary payload.
    let signing_input = Proto::SigningInput {
        version: 2,
        // TODO: each input should have an individual field for this.
        private_key: legacy.private_key[0].to_vec().into(),
        lock_time: Some(lock_time),
        inputs,
        outputs: vec![
            // The target output (main recipient).
            Proto::Output {
                amount: legacy.amount as u64,
                to_recipient: ProtoOutputRecipient::from_address(legacy.to_address),
            },
        ],
        // The input selector, as dictated by the `TransactionPlan` of the
        // legacy protobuf structure.
        input_selector,
        sat_vb: legacy.byte_fee as u64,
        change_output: Some(
            // The change output (return to myself).
            Proto::Output {
                amount: legacy.amount as u64,
                to_recipient: ProtoOutputRecipient::from_address(legacy.change_address),
            },
        ),
        disable_change_output: false,
    };

    // NOTE, unhandled legacy fields:
    // * hash_type
    // * scripts (for P2SH)
    // * use_max_amount
    // * coin_type
    // * output_op_return
    // * extra_outputs
    // * use_max_utxo
    // * disable_dust_filter
    // * time (for XVG)
    // * is_it_brc_operation

    let signed = crate::entry::BitcoinEntry.sign(&crate::entry::PlaceHolder, signing_input);
    let transaction = signed
        .transaction
        .expect("transactio not returned from signer");

    let legacy_transaction = LegacyProto::Transaction {
        version: 2,
        lockTime: native_lock_time.to_consensus_u32(),
        inputs: transaction
            .inputs
            .iter()
            .map(|input| LegacyProto::TransactionInput {
                previousOutput: Some(LegacyProto::OutPoint {
                    hash: input.txid.clone(),
                    index: input.vout,
                    sequence: input.sequence,
                    // Unused for Bitcoin
                    tree: Default::default(),
                }),
                // TODO: Why does this exist twice?
                sequence: input.sequence,
                script: input.script_sig.clone(),
            })
            .collect(),
        outputs: transaction
            .outputs
            .iter()
            .map(|output| LegacyProto::TransactionOutput {
                value: output.amount as i64,
                script: output.script_pubkey.clone(),
                spendingScript: output.taproot_payload.clone(),
            })
            .collect(),
    };

    let error = if signed.error == Proto::Error::OK {
        CommonProto::SigningError::OK
    } else {
        CommonProto::SigningError::Error_internal
    };

    let transaction_id_hex = hex::encode(signed.transaction_id.as_ref(), false);

    let legacy_output = LegacyProto::SigningOutput {
        transaction: Some(legacy_transaction),
        encoded: signed.encoded,
        transaction_id: transaction_id_hex.into(),
        error,
        error_message: Default::default(),
    };

    Ok(legacy_output)
}

/// Convenience function.
fn input_from_legacy_utxo(
    my_pubkey: PublicKey,
    redeem_script: &[u8],
    utxo: LegacyProto::UnspentTransaction,
) -> Proto::Input {
    use LegacyProto::TransactionVariant as Ty;

    // Split the spending script into individual items, accordingly.
    let witness = Witness::consensus_decode(&mut utxo.spendingScript.as_ref()).unwrap();

    let script_pubkey = ScriptBuf::from_bytes(utxo.script.to_vec());
    let (input_builder, signing_method) = if script_pubkey.is_p2sh() {
        (
            Proto::mod_Input::Builder {
                variant: Proto::mod_Input::mod_Builder::OneOfvariant::p2sh(
                    redeem_script.to_vec().into(),
                ),
            },
            UtxoProto::SigningMethod::Legacy,
        )
    } else if script_pubkey.is_p2pkh() {
        (
            Proto::mod_Input::Builder {
                variant: Proto::mod_Input::mod_Builder::OneOfvariant::p2pkh(
                    my_pubkey.to_bytes().into(),
                ),
            },
            UtxoProto::SigningMethod::Legacy,
        )
    } else if script_pubkey.is_v0_p2wpkh() {
        (
            Proto::mod_Input::Builder {
                variant: Proto::mod_Input::mod_Builder::OneOfvariant::p2wpkh(
                    my_pubkey.to_bytes().into(),
                ),
            },
            UtxoProto::SigningMethod::Segwit,
        )
    } else if script_pubkey.is_v1_p2tr() {
        let builder = match utxo.variant {
            Ty::P2TRKEYPATH => Proto::mod_Input::Builder {
                variant: Proto::mod_Input::mod_Builder::OneOfvariant::p2tr_key_path(
                    Proto::mod_Input::TaprootKeyPath {
                        one_prevout: false,
                        public_key: my_pubkey.to_bytes().into(),
                    },
                ),
            },
            Ty::BRC20TRANSFER | Ty::NFTINSCRIPTION => {
                let secp = secp256k1::Secp256k1::new();

                let spending_script = ScriptBuf::from_bytes(utxo.spendingScript.to_vec());
                let merkle_root =
                    TapNodeHash::from_script(&spending_script, LeafVersion::TapScript);

                let xonly = XOnlyPublicKey::from(my_pubkey.inner);
                let spend_info = TaprootSpendInfo::from_node_info(
                    &secp,
                    xonly,
                    NodeInfo::new_leaf_with_ver(spending_script.clone(), LeafVersion::TapScript),
                );

                let control_block = spend_info
                    .control_block(&(spending_script, LeafVersion::TapScript))
                    .unwrap();

                Proto::mod_Input::Builder {
                    variant: Proto::mod_Input::mod_Builder::OneOfvariant::p2tr_script_path(
                        Proto::mod_Input::TaprootScriptPath {
                            one_prevout: false,
                            payload: utxo.spendingScript.to_vec().into(),
                            control_block: control_block.serialize().to_vec().into(),
                        },
                    ),
                }
            },
            _ => panic!(),
        };

        (builder, UtxoProto::SigningMethod::TaprootAll)
    } else {
        todo!()
    };

    let out_point = utxo.out_point.as_ref().unwrap();

    Proto::Input {
        txid: out_point.hash.clone(),
        vout: out_point.index,
        amount: utxo.amount as u64,
        // TODO: Or is it `utxo.sequence`?
        sequence: out_point.sequence,
        // TODO: Is this okay?
        sequence_enable_zero: true,
        // TODO: pass `sighash_type` as parameter.
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::custom(Proto::mod_Input::ScriptWitness {
            script_pubkey: utxo.script,
            script_sig: utxo.script,
            witness_items: witness
                .to_vec()
                .into_iter()
                .map(Cow::Owned)
                .collect::<Vec<Cow<_>>>(),
            signing_method,
        }),
    }
}
