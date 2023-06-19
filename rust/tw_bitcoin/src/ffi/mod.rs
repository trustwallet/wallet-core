#![allow(clippy::missing_safety_doc)]

use super::try_or_else;
use crate::{Error, Result, TXOutputP2TRScriptPath, TaprootScript, TxInputP2TRScriptPath};
use bitcoin::{
    taproot::{NodeInfo, TapNodeHash, TaprootSpendInfo},
    PublicKey, ScriptBuf, Txid,
};
use secp256k1::hashes::Hash;
use secp256k1::KeyPair;
use std::borrow::Cow;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::ErrorCode;
use tw_proto::Bitcoin::Proto::{
    OutPoint, SigningInput, SigningOutput, Transaction, TransactionInput, TransactionOutput,
    TransactionVariant as TrVariant,
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
pub unsafe extern "C" fn tw_taproot_build_and_sign_transaction(
    input: *const u8,
    input_len: usize,
) -> CByteArray {
    let data = CByteArrayRef::new(input, input_len)
        .to_vec()
        .unwrap_or_default();

    let proto: SigningInput = try_or_else!(tw_proto::deserialize(&data), CByteArray::null);
    let transaction = try_or_else!(taproot_build_and_sign_transaction(proto), CByteArray::null);

    let serialized =
        tw_proto::serialize(&transaction).expect("failed to serialize signed transaction");

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
        let vout = out_point.index as u32;
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
            TrVariant::BRC20TRANSFER => {
                let spending_script = ScriptBuf::from_bytes(input.spendingScript.to_vec());
                let merkle_root = TapNodeHash::from_script(
                    spending_script.as_script(),
                    bitcoin::taproot::LeafVersion::TapScript,
                );

                // Convert to tapscript recipient.
                let recipient =
                    Recipient::<TaprootScript>::from_pubkey_recipient(my_pubkey, merkle_root);

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

        let tx: TxOutput = match output.variant {
            TrVariant::P2PKH => TxOutputP2PKH::new_with_script(satoshis, script_buf).into(),
            TrVariant::P2WPKH => TxOutputP2WPKH::new_with_script(satoshis, script_buf).into(),
            TrVariant::P2TRKEYPATH => {
                TxOutputP2TRKeyPath::new_with_script(satoshis, script_buf).into()
            },
            TrVariant::BRC20TRANSFER => {
                TXOutputP2TRScriptPath::new_with_script(satoshis, script_buf).into()
            },
        };

        builder = builder.add_output(tx);
    }

    // Copy those values before `builder` gets consumed.
    let version = builder.version;
    let lock_time = builder.lock_time.to_consensus_u32();

    // Sign transaction and create protobuf structures.
    let tx = builder.sign_inputs(keypair)?;

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
                tree: 0,
            }),
            sequence: input.sequence.to_consensus_u32(),
            script: {
                // If `scriptSig` is empty, then the Witness is being used.
                if input.script_sig.is_empty() {
                    // TODO: `to_vec` returns a `Vec<Vec<u8>>` representing
                    // individual items. Is it appropriate to simply merge
                    // everything here?
                    let witness: Vec<u8> = input.witness.to_vec().into_iter().flatten().collect();
                    Cow::from(witness)
                } else {
                    Cow::from(input.script_sig.to_bytes())
                }
            },
        });
    }

    let mut proto_outputs = vec![];
    for output in &tx.inner.output {
        proto_outputs.push(TransactionOutput {
            value: output.value as i64,
            script: Cow::from(output.script_pubkey.to_bytes()),
            spendingScript: Cow::default(),
        })
    }

    let mut transaction = SigningOutput {
        transaction: Some(Transaction {
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

    // Sign transaction and return array.
    let signed = tx.serialize()?;
    transaction.encoded = Cow::from(signed);

    Ok(transaction)
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

// The code below is reimplemented from the `dev` branch.

#[macro_export]
macro_rules! try_or_else {
    ($exp:expr, $or_else:expr) => {{
        match $crate::ffi::IntoOption::into_option($exp) {
            Some(res) => res,
            None => return $or_else(),
        }
    }};
}

pub trait IntoOption<T> {
    fn into_option(self) -> Option<T>;
}

impl<T, E> IntoOption<T> for std::result::Result<T, E> {
    fn into_option(self) -> Option<T> {
        self.ok()
    }
}

impl<T> IntoOption<T> for Option<T> {
    fn into_option(self) -> Option<T> {
        self
    }
}

pub trait RawPtrTrait: Sized {
    fn into_ptr(self) -> *mut Self {
        Box::into_raw(Box::new(self))
    }

    unsafe fn from_ptr(raw: *mut Self) -> Option<Self> {
        if raw.is_null() {
            return None;
        }
        Some(*Box::from_raw(raw))
    }

    unsafe fn from_ptr_as_ref(raw: *mut Self) -> Option<&'static Self> {
        if raw.is_null() {
            return None;
        }
        Some(&*raw)
    }

    unsafe fn from_ptr_as_box(raw: *mut Self) -> Option<Box<Self>> {
        if raw.is_null() {
            return None;
        }
        Some(Box::from_raw(raw))
    }
}
