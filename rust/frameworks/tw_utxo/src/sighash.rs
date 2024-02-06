// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::UtxoResult;

const ANYONE_CAN_PAY_FLAG: u32 = 0x80;
const FORK_ID_FLAG: u32 = 0x40;
const BASE_FLAG: u32 = 0x1f;

#[derive(Clone, Copy, Debug, PartialEq)]
#[repr(u32)]
pub enum SighashBase {
    /// Use default (All) - 0x00
    UseDefault = 0,
    /// Sign all inputs and outputs (default) - 0x01
    All = 1,
    /// Sign all inputs but no outputs, anyone can choose the destination - 0x02
    None = 2,
    /// Sign the output whose index matches this inputs index - 0x03
    Single = 3,
}

/// Signature hash type.
/// https://en.bitcoin.it/wiki/OP_CHECKSIG#Procedure_for_Hashtype_SIGHASH_SINGLE
#[derive(Debug, PartialEq, Clone, Copy)]
pub struct Sighash {
    /// Original raw sighash type.
    raw_sighash: u32,
    /// Sighash base type.
    base: SighashBase,
}

impl Sighash {
    /// Creates Sighash from any u32.
    pub fn from_u32(u: u32) -> UtxoResult<Sighash> {
        let base = match u & BASE_FLAG {
            0 => SighashBase::UseDefault,
            2 => SighashBase::None,
            3 => SighashBase::Single,
            _ => SighashBase::All,
        };
        Ok(Sighash {
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

impl Default for Sighash {
    fn default() -> Self {
        Sighash {
            raw_sighash: SighashBase::UseDefault as u32,
            base: SighashBase::UseDefault,
        }
    }
}
