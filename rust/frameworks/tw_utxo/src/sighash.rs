// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_keypair::{ecdsa::der, schnorr};
use tw_misc::traits::ToBytesVec;

use crate::error::{UtxoError, UtxoErrorKind, UtxoResult};

const ANYONE_CAN_PAY_FLAG: u32 = 0x80;
const FORK_ID_FLAG: u32 = 0x40;
const BASE_FLAG: u32 = 0x1f;

/// A Bitcoin ECDSA signature with a sighash type, which must be serialzed
/// occordingly in the scriptSig/Witness data to spend an output.
pub struct BitcoinEcdsaSignature {
    sig: der::Signature,
    sighash_ty: SighashType,
}

impl BitcoinEcdsaSignature {
    // The max size of the serialized signature including sighash type.
    const SER_SIZE: usize = 73;

    pub fn new(sig: der::Signature, sighash_ty: SighashType) -> UtxoResult<Self> {
        Ok(BitcoinEcdsaSignature { sig, sighash_ty })
    }
    pub fn serialize(&self) -> Vec<u8> {
        let mut ser = Vec::with_capacity(Self::SER_SIZE);
        ser.extend(self.sig.der_bytes());
        ser.push(self.sighash_ty.raw_sighash() as u8);
        debug_assert!(ser.len() <= Self::SER_SIZE);
        ser
    }
}

// TODO: Do we even need this?
pub struct BitcoinSchnorrSignature {
    sig: schnorr::Signature,
    sighash_ty: SighashType,
}

impl BitcoinSchnorrSignature {
    // The size of the serialized signature including sighash type.
    const SER_SIZE: usize = 65;

    pub fn new(sig: schnorr::Signature, sighash_ty: SighashType) -> UtxoResult<Self> {
        Ok(BitcoinSchnorrSignature { sig, sighash_ty })
    }

    pub fn serialize(&self) -> Vec<u8> {
        let mut ser = Vec::with_capacity(Self::SER_SIZE);
        ser.extend(self.sig.to_vec());
        // TODO: Looks like this is not expected(?)
        //ser.push(self.sighash_ty.raw_sighash() as u8);
        //debug_assert_eq!(ser.len(), Self::SER_SIZE);
        ser
    }
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
#[repr(u32)]
pub enum SighashBase {
    /// Sign all inputs and outputs (default) - 0x01
    All = 1,
    /// Sign all inputs but no outputs, anyone can choose the destination - 0x02
    None = 2,
    /// Sign the output whose index matches this inputs index - 0x03
    Single = 3,
    // TODO:
    // AllPlusAnyoneCanPay = 129,
    // NonePlusAnyoneCanPay = 130,
    // SinglePlusAnyoneCanPay = 131,
}

/// Signature hash type.
/// https://en.bitcoin.it/wiki/OP_CHECKSIG#Procedure_for_Hashtype_SIGHASH_SINGLE
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct SighashType {
    /// Original raw sighash type.
    raw_sighash: u32,
    /// Sighash base type.
    base: SighashBase,
}

impl SighashType {
    pub fn new(base: SighashBase) -> Self {
        SighashType {
            raw_sighash: base as u32,
            base,
        }
    }
    /// Creates Sighash from any u32.
    pub fn from_u32(u: u32) -> UtxoResult<Self> {
        let base = match u & BASE_FLAG {
            1 => SighashBase::All,
            2 => SighashBase::None,
            3 => SighashBase::Single,
            // TODO: Set appropriate error variant
            _ => return Err(UtxoError(UtxoErrorKind::Error_internal)),
        };
        Ok(SighashType {
            raw_sighash: u,
            base,
        })
    }

    /// Returns a raw sighash type.
    pub fn raw_sighash(&self) -> u32 {
        self.raw_sighash
    }

    pub fn base_type(&self) -> SighashBase {
        self.base
    }

    /// Whether anyone can pay.
    pub fn anyone_can_pay(&self) -> bool {
        (self.raw_sighash & ANYONE_CAN_PAY_FLAG) == ANYONE_CAN_PAY_FLAG
    }

    /// Whether a fork chain.
    pub fn fork_id(&self) -> bool {
        (self.raw_sighash & FORK_ID_FLAG) == FORK_ID_FLAG
    }
}

impl Default for SighashType {
    fn default() -> Self {
        SighashType {
            raw_sighash: SighashBase::All as u32,
            base: SighashBase::All,
        }
    }
}
