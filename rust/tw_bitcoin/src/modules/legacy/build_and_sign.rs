use crate::aliases::*;
use crate::{Error, Result};
use bitcoin::absolute::LockTime;
use bitcoin::consensus::Decodable;
use bitcoin::taproot::{LeafVersion, NodeInfo, TaprootSpendInfo};
use bitcoin::{Network, PrivateKey, PublicKey, ScriptBuf, Transaction};
use secp256k1::XOnlyPublicKey;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::test_utils::empty_context::EmptyCoinContext;
use tw_encoding::hex;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::CUInt64Result;
use tw_misc::traits::ToBytesVec;
use tw_misc::try_or_else;
use tw_proto::Bitcoin::Proto as LegacyProto;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Common::Proto as CommonProto;
use tw_proto::Utxo::Proto as UtxoProto;

#[deprecated]
#[no_mangle]
pub unsafe extern "C" fn tw_bitcoin_calculate_transaction_fee(
    input: *const u8,
    input_len: usize,
    sat_vb: u64,
) -> CUInt64Result {
    let Some(mut encoded) = CByteArrayRef::new(input, input_len).as_slice() else {
        return CUInt64Result::error(1);
    };

    // Decode transaction.
    let Ok(tx) = Transaction::consensus_decode(&mut encoded) else {
        return CUInt64Result::error(1);
    };

    // Calculate fee.
    let weight = tx.weight();
    let fee = weight.to_vbytes_ceil() * sat_vb;

    CUInt64Result::ok(fee)
}

#[no_mangle]
#[deprecated]
pub unsafe extern "C" fn tw_taproot_build_and_sign_transaction(
    input: *const u8,
    input_len: usize,
) -> CByteArray {
    let data = CByteArrayRef::new(input, input_len)
        .to_vec()
        .unwrap_or_default();

    let proto: LegacyProto::SigningInput =
        try_or_else!(tw_proto::deserialize(&data), CByteArray::null);

    let Ok(signing) = taproot_build_and_sign_transaction(proto) else {
        // Convert the `BitcoinV2.proto` error type inot the `Common.proto`
        // errot type and return.
        let error = LegacyProto::SigningOutput {
            error: CommonProto::SigningError::Error_general,
            ..Default::default()
        };

        let serialized = tw_proto::serialize(&error).expect("failed to serialize error message");
        return CByteArray::from(serialized)
    };

    // Serialize SigningOutput and return.
    let serialized = tw_proto::serialize(&signing).expect("failed to serialize signed transaction");
    CByteArray::from(serialized)
}

fn taproot_build_and_sign_transaction(
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
    for (index, utxo) in legacy.utxo.iter().enumerate() {
        // We try to fetch the private key in the `private_key` fields by the
        // corresponding index. If there is none, we default to the first
        // provided key (implying that one single private key is used for all
        // inputs).
        let private_key = if let Some(private_key) = legacy.private_key.get(index) {
            private_key
        } else {
            legacy
                .private_key
                .get(0)
                .ok_or_else(|| Error::from(Proto::Error::Error_legacy_no_private_key))?
        };

        let private_key = PrivateKey::from_slice(private_key, Network::Bitcoin)?;

        let my_pubkey = private_key.public_key(&secp256k1::Secp256k1::new());

        inputs.push(input_from_legacy_utxo(my_pubkey, utxo, legacy.hash_type)?)
    }

    // We skip any sort of builders and use the provided scripts directly.
    let mut outputs = vec![];
    for output in legacy
        .plan
        .ok_or_else(|| Error::from(Proto::Error::Error_legacy_no_plan_provided))?
        .utxos
    {
        outputs.push(Proto::Output {
            value: output.amount as u64,
            to_recipient: ProtoOutputRecipient::custom_script_pubkey(output.script),
        })
    }

    // We only select enough inputs to cover the output balance. However, since
    // some transaction types require precise input ordering (such as BRC20), we
    // do not sort the inputs and use the ordering as provided by the caller.
    let input_selector = UtxoProto::InputSelector::SelectInOrder;

    // The primary payload.
    let signing_input = Proto::SigningInput {
        version: 2,
        // TODO: each input should have an individual field for this.
        private_key: legacy.private_key[0].to_vec().into(),
        lock_time: Some(lock_time),
        inputs,
        outputs,
        input_selector,
        fee_per_vb: legacy.byte_fee as u64,
        change_output: None,
        disable_change_output: true,
        dangerous_use_fixed_schnorr_rng: false,
    };

    // Build and sign the Bitcoin transaction.
    let signed = crate::entry::BitcoinEntry.sign(&EmptyCoinContext, signing_input);

    // Check for error.
    if signed.error != Proto::Error::OK {
        return Err(Error::from(signed.error));
    }

    let transaction = signed
        .transaction
        .expect("transaction not returned from signer");

    // Convert the returned transaction data into the (legacy) `Transaction`
    // protobuf from `Bitcoin.proto`.
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
                // Notr: Not sure why this exists twice?
                sequence: input.sequence,
                script: input.script_sig.clone(),
            })
            .collect(),
        outputs: transaction
            .outputs
            .iter()
            .map(|output| LegacyProto::TransactionOutput {
                value: output.value as i64,
                script: output.script_pubkey.clone(),
                spendingScript: output.taproot_payload.clone(),
            })
            .collect(),
    };

    let txid_hex = hex::encode(signed.txid.as_ref(), false);

    // Put the `Transaction` into the `SigningOutput`, return.
    let legacy_output = LegacyProto::SigningOutput {
        transaction: Some(legacy_transaction),
        encoded: signed.encoded,
        transaction_id: txid_hex.into(),
        error: CommonProto::SigningError::OK,
        error_message: Default::default(),
    };

    Ok(legacy_output)
}

/// Convenience function.
fn input_from_legacy_utxo(
    my_pubkey: PublicKey,
    utxo: &LegacyProto::UnspentTransaction,
    hash_type: u32,
) -> Result<Proto::Input<'static>> {
    // We identify the provided `Variant` and invoke the builder function. We
    // explicitly skip/ignore any provided script in the input.
    let input_builder = match utxo.variant {
        LegacyProto::TransactionVariant::P2PKH => Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2pkh(my_pubkey.to_bytes().into()),
        },
        LegacyProto::TransactionVariant::P2WPKH => Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2wpkh(my_pubkey.to_bytes().into()),
        },
        LegacyProto::TransactionVariant::P2TRKEYPATH => Proto::mod_Input::InputBuilder {
            variant: ProtoInputBuilder::p2tr_key_path(Proto::mod_Input::InputTaprootKeyPath {
                one_prevout: false,
                public_key: my_pubkey.to_bytes().into(),
            }),
        },
        LegacyProto::TransactionVariant::BRC20TRANSFER
        | LegacyProto::TransactionVariant::NFTINSCRIPTION => {
            // The spending script must to be empty.
            if utxo.spendingScript.is_empty() {
                return Err(Error::from(
                    Proto::Error::Error_legacy_no_spending_script_provided,
                ));
            }

            let spending_script = ScriptBuf::from_bytes(utxo.spendingScript.to_vec());

            let xonly = XOnlyPublicKey::from(my_pubkey.inner);
            let spend_info = TaprootSpendInfo::from_node_info(
                &secp256k1::Secp256k1::new(),
                xonly,
                NodeInfo::new_leaf_with_ver(spending_script.clone(), LeafVersion::TapScript),
            );

            let control_block = spend_info
                .control_block(&(spending_script, LeafVersion::TapScript))
                .expect("failed to construct control block");

            Proto::mod_Input::InputBuilder {
                variant: ProtoInputBuilder::p2tr_script_path(
                    Proto::mod_Input::InputTaprootScriptPath {
                        one_prevout: false,
                        payload: utxo.spendingScript.to_vec().into(),
                        control_block: control_block.serialize().into(),
                    },
                ),
            }
        },
    };

    // Convert the integer indicating the sighash type into the corresponding
    // Utxo variant.
    let sighash_type = match hash_type {
        0 => UtxoProto::SighashType::UseDefault,
        1 => UtxoProto::SighashType::All,
        2 => UtxoProto::SighashType::None_pb,
        3 => UtxoProto::SighashType::Single,
        129 => UtxoProto::SighashType::AllPlusAnyoneCanPay,
        130 => UtxoProto::SighashType::NonePlusAnyoneCanPay,
        131 => UtxoProto::SighashType::SinglePlusAnyoneCanPay,
        _ => return Err(Error::from(Proto::Error::Error_utxo_invalid_sighash_type)),
    };

    // Construct Input and return.
    let out_point = utxo
        .out_point
        .as_ref()
        .ok_or_else(|| Error::from(Proto::Error::Error_legacy_outpoint_not_set))?;

    // We explicitly disable zero-valued sequences for legacy and default to
    // `0xFFFFFFFF`'
    let sequence = if out_point.sequence == 0 {
        u32::MAX
    } else {
        out_point.sequence
    };

    Ok(Proto::Input {
        private_key: Default::default(),
        txid: out_point.hash.to_vec().into(),
        vout: out_point.index,
        value: utxo.amount as u64,
        sequence,
        sequence_enable_zero: false,
        sighash_type,
        to_recipient: ProtoInputRecipient::builder(input_builder),
    })
}
