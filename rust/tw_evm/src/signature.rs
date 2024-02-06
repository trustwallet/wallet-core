// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::ops::BitXor;
use tw_number::{NumberResult, U256};

/// EIP155 Eth encoding of V, of the form 27+v, or 35+chainID*2+v.
/// cbindgin:ignore
pub const ETHEREUM_SIGNATURE_V_OFFSET: u8 = 27;

/// Embeds `chain_id` in `v` param, for replay protection, legacy or EIP155.
#[inline]
pub fn replay_protection(chain_id: U256, v: u8) -> NumberResult<U256> {
    if chain_id.is_zero() {
        legacy_replay_protection(v)
    } else {
        eip155_replay_protection(chain_id, v)
    }
}

/// Embeds `chain_id` in `v` param, for replay protection, legacy.
#[inline]
pub fn legacy_replay_protection(v: u8) -> NumberResult<U256> {
    U256::from(v).checked_add(ETHEREUM_SIGNATURE_V_OFFSET)
}

/// Embeds `chain_id` in `v` param, for replay protection, EIP155.
#[inline]
pub fn eip155_replay_protection(chain_id: U256, v: u8) -> NumberResult<U256> {
    // chain_id + chain_id + 35u8 + v
    chain_id
        .checked_add(chain_id)?
        .checked_add(35_u64)?
        .checked_add(v)
}

/// Removes EIP155 or legacy replay protection.
#[inline]
pub fn remove_replay_protection(v: u8) -> u8 {
    const BIT_MASK: u8 = 0x01;
    if v >= ETHEREUM_SIGNATURE_V_OFFSET {
        return (v & BIT_MASK).bitxor(BIT_MASK);
    }
    v
}
