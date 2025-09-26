// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

pub fn is_uint(ty: &str) -> bool {
    matches!(ty, "u8" | "u16" | "u32" | "u64" | "u128" | "usize")
}
