#![allow(clippy::missing_safety_doc)]

use crate::entry::{ProtoInputRecipient, ProtoOutputRecipient};
use crate::Result;
use bitcoin::absolute::LockTime;
use bitcoin::consensus::Decodable;
use bitcoin::Witness;
use std::borrow::Cow;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::ErrorCode;
use tw_misc::try_or_else;
use tw_proto::Bitcoin::Proto as LegacyProto;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

// TODO: Should be deprecated.
#[repr(C)]
pub enum CTaprootError {
    Ok = 0,
    InvalidSlice = 1,
    InvalidPubkey = 2,
    InvalidSegwitPukey = 3,
}

impl From<CTaprootError> for ErrorCode {
    fn from(error: CTaprootError) -> Self {
        error as ErrorCode
    }
}

pub mod address;
pub mod scripts;

// Re-exports
pub use address::*;
pub use scripts::*;

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

/// Note: many of the fields used in the `SigningInput` are currently unused. We
/// can later easily replicate the funcationlity and behavior of the C++
/// implemenation.
///
/// Additionally, the `SigningInput` supports two ways of operating (which
/// should probably be separated anyway): one way where the `TransactionPlan` is
/// skipped (and hence automatically constructed) and the other way where the
/// `TransactionPlan` is created manually. As of now, it's expected that the
/// `TransactionPlan` is created manually, meaning that the caller must careful
/// construct the outputs, which must include the return/change transaction and
/// how much goes to the miner as fee (<total-satoshi-inputs> minus
/// <total-satoshi-outputs>).
pub(crate) fn taproot_build_and_sign_transaction(
    legacy: LegacyProto::SigningInput,
) -> Result<LegacyProto::SigningOutput> {
    // Convert the appropriate lock time.
    let lock_time = match LockTime::from_consensus(legacy.lock_time) {
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

    let proto = Proto::SigningInput {
        version: 2,
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

    todo!()
}

/// Convenience function.
fn input_from_legacy_utxo(utxo: LegacyProto::UnspentTransaction) -> Proto::Input {
    let out_point = utxo.out_point.as_ref().unwrap();

    let witness = Witness::consensus_decode(&mut utxo.spendingScript.as_ref()).unwrap();

    Proto::Input {
        txid: out_point.hash.clone(),
        vout: out_point.index,
        amount: utxo.amount as u64,
        // TODO: `utxo.variant` important here?
        // TODO:
        sighash_type: UtxoProto::SighashType::All,
        to_recipient: ProtoInputRecipient::custom(Proto::mod_Input::ScriptWitness {
            script_sig: utxo.script,
            witness_items: witness
                .to_vec()
                .into_iter()
                .map(Cow::Owned)
                .collect::<Vec<Cow<_>>>(),
        }),
    }
}
