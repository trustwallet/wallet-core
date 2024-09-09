// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

pub mod bip39_english;

pub fn normalize_mnemonic(mnemonic: &str) -> String {
    mnemonic.trim().to_string()
}
