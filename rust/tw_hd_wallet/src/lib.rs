// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

extern crate core;

pub mod bip39;
pub mod ton;

pub type WalletResult<T> = Result<T, WalletError>;

#[derive(Debug, Eq, PartialEq)]
pub enum WalletError {
    InvalidMnemonicWordCount,
    InvalidMnemonicUnknownWord,
    InvalidMnemonicEntropy,
    InvalidChecksum,
}
