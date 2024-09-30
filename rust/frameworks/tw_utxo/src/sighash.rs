// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;

const ANYONE_CAN_PAY_FLAG: u32 = 0x80;
const FORK_ID_FLAG: u32 = 0x40;
const BASE_FLAG: u32 = 0x1f;
const DEFAULT_TAPROOT_SIGHASH_TYPE: u8 = 0;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
#[repr(u32)]
pub enum SighashBase {
    /// Sign all inputs and outputs (default) - 0x01
    All = 1,
    /// Sign all inputs but no outputs, anyone can choose the destination - 0x02
    None = 2,
    /// Sign the output whose index matches this inputs index - 0x03
    Single = 3,
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
    /// Creates Sighash from any u32.
    pub fn from_u32(u: u32) -> SigningResult<Self> {
        let base = match u & BASE_FLAG {
            0 | 1 => SighashBase::All,
            2 => SighashBase::None,
            3 => SighashBase::Single,
            _ => {
                return SigningError::err(SigningErrorType::Error_invalid_params)
                    .context("Invalid sighash uint32 representation")
            },
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

    /// Returns a raw sighash type as u8 if possible.
    pub fn serialize(&self) -> SigningResult<u8> {
        self.raw_sighash
            .try_into()
            .tw_err(|_| SigningErrorType::Error_invalid_params)
            .context("sighashType must fit uint8")
    }

    /// Returns a serialized raw sighash type, where `All` sighash is serialized as 0x00 (`Default`).
    /// The 0x00 variant is only supported in Taproot transactions,
    /// not in Legacy or Segwit transactions.
    pub fn serialize_as_taproot(&self) -> SigningResult<u8> {
        if self.is_default_taproot_sighash() {
            Ok(DEFAULT_TAPROOT_SIGHASH_TYPE)
        } else {
            self.raw_sighash
                .try_into()
                .tw_err(|_| SigningErrorType::Error_invalid_params)
                .context("Taproot sighashType must fit uint8")
        }
    }

    /// Whether the sighash type is `Default` Taproot, i.e will be serialized as 0x00.
    /// The 0x00 variant is only supported in Taproot transactions,
    /// not in Legacy or Segwit transactions.
    pub fn is_default_taproot_sighash(&self) -> bool {
        self.raw_sighash == DEFAULT_TAPROOT_SIGHASH_TYPE as u32
            || self.raw_sighash == SighashBase::All as u32
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
