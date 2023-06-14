#![allow(clippy::missing_safety_doc)]

use super::try_or_else;
use crate::{Error, Result};
use bitcoin::{PublicKey, ScriptBuf, Txid};
use secp256k1::hashes::Hash;
use secp256k1::KeyPair;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::ErrorCode;
use tw_proto::Bitcoin::Proto::{SigningInput, TransactionVariant as TrVariant};

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
    let bytes = try_or_else!(taproot_build_and_sign_transaction(proto), CByteArray::null);

    CByteArray::from(bytes)
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
pub(crate) fn taproot_build_and_sign_transaction(proto: SigningInput) -> Result<Vec<u8>> {
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
        };

        builder = builder.add_output(tx);
    }

    // Sign transaction and return array.
    builder.sign_inputs(keypair)?.serialize()
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
