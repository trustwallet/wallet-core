// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::bip39::bip39_english::BIP39_WORDS_MAP;
use crate::{WalletError, WalletResult};
use zeroize::ZeroizeOnDrop;

#[derive(Debug, ZeroizeOnDrop)]
pub struct TonMnemonic(String);

impl TonMnemonic {
    pub const WORDS_LEN: usize = 24;

    /// Creates `TonMnemonic` from a 24-words string.
    ///
    /// Please note there can be a passphrase required to be used to derive a key-pair.
    /// See [`TonWallet::new`].
    pub fn new(mnemonic: &str) -> WalletResult<TonMnemonic> {
        let normalized_mnemonic = mnemonic.trim().to_string();

        let mut invalid = false;
        let mut words_count = 0;
        for word in normalized_mnemonic.split(" ") {
            words_count += 1;

            // Although this operation is not security-critical, we aim for constant-time operation here as well
            // (i.e., no early exit on match)
            //
            // We expect words in lowercase only.
            // It's a responsibility of the WalletCore user to transform the mnemonic if needed.
            if !BIP39_WORDS_MAP.contains_key(word) {
                invalid = true;
            }
        }

        if invalid {
            return Err(WalletError::InvalidMnemonicUnknownWord);
        }
        if words_count != TonMnemonic::WORDS_LEN {
            return Err(WalletError::InvalidMnemonicWordCount);
        }

        Ok(TonMnemonic(normalized_mnemonic))
    }

    pub fn as_str(&self) -> &str {
        &self.0
    }
}
