// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::bip39::bip39_english::BIP39_WORDS_MAP;
use crate::{WalletError, WalletResult};

pub const WORDS_LEN: usize = 24;

/// Validates the given `mnemonic` string if it consists of 24 known words (see BIP39 words list).
///
/// Please note there this function doesn't validate the mnemonic but it words only.
/// See [`TonWallet::new`].
pub fn validate_mnemonic_words(mnemonic: &str) -> WalletResult<()> {
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
    if words_count != WORDS_LEN {
        return Err(WalletError::InvalidMnemonicWordCount);
    }

    Ok(())
}
