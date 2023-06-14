#![allow(clippy::missing_safety_doc)]

use super::try_or_else;
use crate::{Error, Result};
use bitcoin::{PublicKey, Txid};
use secp256k1::hashes::Hash;
use secp256k1::KeyPair;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::ErrorCode;
use tw_proto::Bitcoin::Proto::{SigningInput, TransactionVariant as TrVariant};

pub mod address;

// Re-exports
pub use address::*;

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

pub(crate) fn taproot_build_and_sign_transaction(proto: SigningInput) -> Result<Vec<u8>> {
    let privkey = proto.private_key.get(0).ok_or(Error::Todo)?;

    // Prepare keypair and correspodning public key.
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

        let tx: TxInput = match input.variant {
            TrVariant::P2PKH => TxInputP2PKH::new(txid, vout, my_pubkey.into(), satoshis).into(),
            TrVariant::P2WPKH => {
                TxInputP2WPKH::new(txid, vout, my_pubkey.try_into()?, satoshis).into()
            },
            TrVariant::P2TRKEYSPEND => {
                TxInputP2TRKeyPath::new(txid, vout, my_pubkey, satoshis).into()
            },
        };

        builder = builder.add_input(tx);
    }

    // Process outputs.
    for output in proto.plan.ok_or(Error::Todo)?.utxos {
        let tx: TxOutput = match output.variant {
            TrVariant::P2PKH => {
                TxOutputP2PKH::from_bytes(output.script.to_vec(), output.amount as u64).into()
            },
            TrVariant::P2WPKH => {
                TxOutputP2WPKH::from_bytes(output.script.to_vec(), output.amount as u64).into()
            },
            TrVariant::P2TRKEYSPEND => {
                TxOutputP2TRKeyPath::from_bytes(output.script.to_vec(), output.amount as u64).into()
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
