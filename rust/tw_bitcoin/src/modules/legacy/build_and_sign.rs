use crate::entry::{ProtoInputRecipient, ProtoOutputRecipient};
use crate::Result;
use bitcoin::absolute::LockTime;
use bitcoin::consensus::Decodable;
use bitcoin::{ScriptBuf, Witness};
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
                to_recipient: ProtoOutputRecipient::from_address(
                    legacy.to_address.as_bytes().into(),
                ),
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
                to_recipient: ProtoOutputRecipient::from_address(
                    legacy.change_address.as_bytes().into(),
                ),
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

    // TODO:
    let legacy_transaction = LegacyProto::Transaction {
        version: 2,
        lockTime: native_lock_time.to_consensus_u32(),
        inputs: Default::default(),
        outputs: transaction
            .outputs
            .iter()
            .map(|_output| {
                //TODO:
                LegacyProto::TransactionOutput {
                    value: Default::default(),
                    script: Default::default(),
                    spendingScript: Default::default(),
                }
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
fn input_from_legacy_utxo(utxo: LegacyProto::UnspentTransaction) -> Proto::Input {
    let out_point = utxo.out_point.as_ref().unwrap();

    let witness = Witness::consensus_decode(&mut utxo.spendingScript.as_ref()).unwrap();

    // TODO:
    let script_pubkey = ScriptBuf::new();
    let signing_method = match utxo.variant {
        LegacyProto::TransactionVariant::P2PKH => UtxoProto::SigningMethod::Legacy,
        LegacyProto::TransactionVariant::P2WPKH => UtxoProto::SigningMethod::Segwit,
        LegacyProto::TransactionVariant::P2TRKEYPATH
        | LegacyProto::TransactionVariant::BRC20TRANSFER
        | LegacyProto::TransactionVariant::NFTINSCRIPTION => UtxoProto::SigningMethod::TaprootAll,
    };

    Proto::Input {
        txid: out_point.hash.clone(),
        vout: out_point.index,
        amount: utxo.amount as u64,
        sequence: out_point.sequence,
        sequence_enable_zero: true,
        // TODO: pass `sighash_type` as parameter.
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::custom(Proto::mod_Input::ScriptWitness {
            script_pubkey: script_pubkey.to_vec().into(),
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
