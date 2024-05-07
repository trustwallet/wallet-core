// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::sighash::SighashType;
use tw_coin_entry::error::prelude::*;
use tw_keypair::ecdsa::der;
use tw_keypair::schnorr;
use tw_memory::Data;
use tw_misc::traits::ToBytesVec;

/// A Bitcoin ECDSA signature with a sighash type, which must be serialzed
/// occordingly in the scriptSig/Witness data to spend an output.
pub struct BitcoinEcdsaSignature {
    sig: der::Signature,
    sighash_ty: SighashType,
}

impl BitcoinEcdsaSignature {
    // The max size of the serialized signature including sighash type.
    const SER_SIZE: usize = 73;

    pub fn new(sig: der::Signature, sighash_ty: SighashType) -> Self {
        BitcoinEcdsaSignature { sig, sighash_ty }
    }

    pub fn serialize(&self) -> Data {
        let mut ser = Vec::with_capacity(Self::SER_SIZE);
        ser.extend(self.sig.der_bytes());
        ser.push(self.sighash_ty.raw_sighash() as u8);
        debug_assert!(ser.len() <= Self::SER_SIZE);
        ser
    }
}

pub struct BitcoinSchnorrSignature {
    sig: schnorr::Signature,
    sighash_ty: u8,
}

impl BitcoinSchnorrSignature {
    pub fn new(
        sig: schnorr::Signature,
        sighash_ty: SighashType,
    ) -> SigningResult<BitcoinSchnorrSignature> {
        Ok(BitcoinSchnorrSignature {
            sig,
            sighash_ty: sighash_ty.serialize_as_taproot()?,
        })
    }

    pub fn serialize(&self) -> Vec<u8> {
        let mut ser = Vec::with_capacity(65);
        ser.extend(self.sig.to_vec());
        if self.sighash_ty != 0 {
            ser.push(self.sighash_ty)
        }
        ser
    }
}
