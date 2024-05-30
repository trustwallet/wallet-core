// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::sighash::SighashType;
use tw_coin_entry::error::prelude::*;
use tw_hash::H512;
use tw_keypair::ecdsa::der;
use tw_keypair::schnorr;
use tw_memory::Data;

/// Use the max value of the u8 type to avoid potential serialization optimizations (like ASN.1 DER).
const ESTIMATION_DEFAULT_BYTE: u8 = u8::MAX;

/// A Bitcoin ECDSA signature with a sighash type, which must be serialzed
/// occordingly in the scriptSig/Witness data to spend an output.
pub struct BitcoinEcdsaSignature {
    /// ECDSA signature serialized as DER. See [`der::Signature`].
    der_sig: Data,
    sighash_ty: u8,
}

impl BitcoinEcdsaSignature {
    // The max size of the serialized signature including sighash type.
    const SER_SIZE: usize = 73;

    pub fn new(sig: der::Signature, sighash_ty: SighashType) -> SigningResult<Self> {
        let der_sig = sig.der_bytes();
        let sighash_ty = sighash_ty.serialize()?;
        Ok(BitcoinEcdsaSignature {
            der_sig,
            sighash_ty,
        })
    }

    pub fn estimated() -> Self {
        BitcoinEcdsaSignature {
            der_sig: vec![ESTIMATION_DEFAULT_BYTE; Self::SER_SIZE - 1],
            sighash_ty: ESTIMATION_DEFAULT_BYTE,
        }
    }

    pub fn serialize(&self) -> Data {
        let mut ser = Vec::with_capacity(Self::SER_SIZE);
        ser.extend_from_slice(self.der_sig.as_slice());
        ser.push(self.sighash_ty);
        debug_assert!(ser.len() <= Self::SER_SIZE);
        ser
    }
}

pub struct BitcoinSchnorrSignature {
    sig: H512,
    sighash_ty: u8,
}

impl BitcoinSchnorrSignature {
    pub fn new(
        sig: schnorr::Signature,
        sighash_ty: SighashType,
    ) -> SigningResult<BitcoinSchnorrSignature> {
        Ok(BitcoinSchnorrSignature {
            sig: sig.bytes(),
            sighash_ty: sighash_ty.serialize_as_taproot()?,
        })
    }

    pub fn estimated() -> BitcoinSchnorrSignature {
        BitcoinSchnorrSignature {
            sig: H512::from([ESTIMATION_DEFAULT_BYTE; H512::LEN]),
            sighash_ty: ESTIMATION_DEFAULT_BYTE,
        }
    }

    pub fn serialize(&self) -> Vec<u8> {
        let mut ser = Vec::with_capacity(65);
        ser.extend_from_slice(self.sig.as_slice());
        if self.sighash_ty != 0 {
            ser.push(self.sighash_ty)
        }
        ser
    }
}
