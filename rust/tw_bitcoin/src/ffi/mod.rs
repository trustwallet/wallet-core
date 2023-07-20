#![allow(clippy::missing_safety_doc)]

use crate::{
    calculate_fee, Error, Result, TXOutputP2TRScriptPath, TaprootScript, TxInputP2TRScriptPath,
};
use bitcoin::{
    consensus::Decodable,
    taproot::{NodeInfo, TapNodeHash, TaprootSpendInfo},
    PublicKey, ScriptBuf, Transaction, Txid,
};
use secp256k1::hashes::Hash;
use secp256k1::KeyPair;
use std::borrow::Cow;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::CUInt64Result;
use tw_memory::ffi::c_result::ErrorCode;
use tw_misc::try_or_else;
use tw_proto::Bitcoin::Proto::{
    OutPoint, SigningInput, SigningOutput, Transaction as ProtoTransaction, TransactionInput,
    TransactionOutput, TransactionVariant as TrVariant,
};

pub mod address;
pub mod scripts;

// Re-exports
pub use address::*;
pub use scripts::*;

use crate::{
    Recipient, TransactionBuilder, TxInput, TxInputP2PKH, TxInputP2TRKeyPath, TxInputP2WPKH,
    TxOutput, TxOutputP2PKH, TxOutputP2TRKeyPath, TxOutputP2WPKH,
};

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
    let (_weight, fee) = calculate_fee(&tx, sat_vb);

    CUInt64Result::ok(fee)
}

#[no_mangle]
pub unsafe extern "C" fn tw_taproot_build_and_sign_transaction(
    input: *const u8,
    input_len: usize,
) -> CByteArray {
    let data = CByteArrayRef::new(input, input_len)
        .to_vec()
        .unwrap_or_default();

    let proto: SigningInput = try_or_else!(tw_proto::deserialize(&data), CByteArray::null);
    let signing = try_or_else!(taproot_build_and_sign_transaction(proto), CByteArray::null);

    let serialized = tw_proto::serialize(&signing).expect("failed to serialize signed transaction");

    CByteArray::from(serialized)
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
pub(crate) fn taproot_build_and_sign_transaction(proto: SigningInput) -> Result<SigningOutput> {
    let privkey = proto.private_key.get(0).ok_or(Error::Todo)?;

    // Prepare keypair and derive corresponding public key.
    let keypair = KeyPair::from_seckey_slice(&secp256k1::Secp256k1::new(), privkey.as_ref())
        .map_err(|_| crate::Error::Todo)?;

    let my_pubkey = Recipient::<PublicKey>::from(keypair);

    let mut builder = TransactionBuilder::new();

    // Process inputs.
    for input in proto.utxo {
        let my_pubkey = my_pubkey.clone();

        let out_point = input.out_point.ok_or(Error::Todo)?;
        let txid = Txid::from_slice(&out_point.hash).map_err(|_| crate::Error::Todo)?;
        let vout = out_point.index;
        let satoshis = input.amount as u64;

        let script_buf = ScriptBuf::from_bytes(input.script.to_vec());

        let tx: TxInput = match input.variant {
            TrVariant::P2PKH => {
                TxInputP2PKH::new_with_script(txid, vout, my_pubkey.into(), satoshis, script_buf)
                    .into()
            },
            TrVariant::P2WPKH => TxInputP2WPKH::new_with_script(
                txid,
                vout,
                my_pubkey.try_into()?,
                satoshis,
                script_buf,
            )
            .into(),
            TrVariant::P2TRKEYPATH => TxInputP2TRKeyPath::new_with_script(
                txid,
                vout,
                my_pubkey.into(),
                satoshis,
                script_buf,
            )
            .into(),
            TrVariant::BRC20TRANSFER | TrVariant::NFTINSCRIPTION => {
                // We construct the merkle root for the given spending script.
                let spending_script = ScriptBuf::from_bytes(input.spendingScript.to_vec());
                let merkle_root = TapNodeHash::from_script(
                    spending_script.as_script(),
                    bitcoin::taproot::LeafVersion::TapScript,
                );

                // Convert to tapscript recipient with the given merkle root.
                let recipient =
                    Recipient::<TaprootScript>::from_pubkey_recipient(my_pubkey, merkle_root);

                // Derive the spending information for the taproot recipient.
                let spend_info = TaprootSpendInfo::from_node_info(
                    &secp256k1::Secp256k1::new(),
                    recipient.untweaked_pubkey(),
                    NodeInfo::new_leaf_with_ver(
                        spending_script.clone(),
                        bitcoin::taproot::LeafVersion::TapScript,
                    ),
                );

                TxInputP2TRScriptPath::new_with_script(
                    txid,
                    vout,
                    recipient,
                    satoshis,
                    script_buf,
                    spending_script,
                    spend_info,
                )
                .into()
            },
        };

        builder = builder.add_input(tx);
    }

    // Process outputs.
    for output in proto.plan.ok_or(Error::Todo)?.utxos {
        let script_buf = ScriptBuf::from_bytes(output.script.to_vec());
        let satoshis = output.amount as u64;

        #[rustfmt::skip]
        let tx: TxOutput = match output.variant {
            TrVariant::P2PKH => {
                TxOutputP2PKH::new_with_script(satoshis, script_buf).into()
            },
            TrVariant::P2WPKH => {
                TxOutputP2WPKH::new_with_script(satoshis, script_buf).into()
            },
            TrVariant::P2TRKEYPATH => {
                TxOutputP2TRKeyPath::new_with_script(satoshis, script_buf).into()
            },
            // We're keeping those two variants separate for now, we're planning
            // on writing a new interface as part of a larger task anyway.
            TrVariant::BRC20TRANSFER => {
                TXOutputP2TRScriptPath::new_with_script(satoshis, script_buf).into()
            },
            TrVariant::NFTINSCRIPTION => {
                TXOutputP2TRScriptPath::new_with_script(satoshis, script_buf).into()
            }
        };

        builder = builder.add_output(tx);
    }

    // Copy those values before `builder` gets consumed.
    let version = builder.version;
    let lock_time = builder.lock_time.to_consensus_u32();

    // Sign transaction and create protobuf structures.
    let tx = builder.sign_inputs(keypair)?;

    // Create Protobuf structures of inputs.
    let mut proto_inputs = vec![];
    for input in &tx.inner.input {
        let txid: Vec<u8> = input
            .previous_output
            .txid
            .as_byte_array()
            .iter()
            .cloned()
            .rev()
            .collect();

        proto_inputs.push(TransactionInput {
            previousOutput: Some(OutPoint {
                hash: Cow::from(txid),
                index: input.previous_output.vout,
                sequence: input.sequence.to_consensus_u32(),
                // Unused.
                tree: 0,
            }),
            sequence: input.sequence.to_consensus_u32(),
            script: {
                // If `scriptSig` is empty, then the Witness is being used.
                if input.script_sig.is_empty() {
                    let witness: Vec<u8> = input.witness.to_vec().into_iter().flatten().collect();
                    Cow::from(witness)
                } else {
                    Cow::from(input.script_sig.to_bytes())
                }
            },
        });
    }

    // Create Protobuf structures of outputs.
    let mut proto_outputs = vec![];
    for output in &tx.inner.output {
        proto_outputs.push(TransactionOutput {
            value: output.value as i64,
            script: Cow::from(output.script_pubkey.to_bytes()),
            spendingScript: Cow::default(),
        })
    }

    // Create Protobuf structure of the full transaction.
    let mut signing = SigningOutput {
        transaction: Some(ProtoTransaction {
            version,
            lockTime: lock_time,
            inputs: proto_inputs,
            outputs: proto_outputs,
        }),
        encoded: Cow::default(),
        transaction_id: Cow::from(tx.inner.txid().to_string()),
        error: tw_proto::Common::Proto::SigningError::OK,
        error_message: Cow::default(),
    };

    // Sign transaction and update Protobuf structure.
    let signed = tx.serialize()?;
    signing.encoded = Cow::from(signed);

    Ok(signing)
}

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
