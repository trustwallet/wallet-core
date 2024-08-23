// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ton::mnemonic::TonMnemonic;
use crate::{WalletError, WalletResult};
use tw_hash::hmac::hmac_sha512;
use tw_hash::pbkdf2::pbkdf2_hmac_sha512;
use tw_hash::{H256, H512};
use tw_keypair::ed25519::sha512::KeyPair;
use zeroize::ZeroizeOnDrop;

pub const TON_WALLET_SALT: &[u8] = b"TON default seed";
pub const TON_WALLET_PBKDF_ITERATIONS: u32 = 100_000;

/// Used to verify a pair of mnemonic and passphrase only.
const TON_BASIC_SEED_SALT: &[u8] = b"TON seed version";
/// Equals to `(TON_WALLET_PBKDF_ITERATIONS as f64 / 256.0).floor() as u32`.
const TON_BASIC_SEED_ROUNDS: u32 = 390;

/// Used to verify a pair of mnemonic and passphrase only.
const TON_PASSPHRASE_SEED_SALT: &[u8] = b"TON fast seed version";
const TON_PASSPHRASE_SEED_ROUNDS: u32 = 1;

pub mod mnemonic;

#[derive(ZeroizeOnDrop)]
pub struct TonWallet {
    mnemonic: TonMnemonic,
    passphrase: Option<String>,
    entropy: H512,
    seed: H512,
}

impl TonWallet {
    pub const MNEMONIC_WORDS: usize = 24;

    /// Creates `TonWallet` while validating if there should or shouldn't be a passphrase.
    /// https://github.com/toncenter/tonweb-mnemonic/blob/a338a00d4ca0ed833431e0e49e4cfad766ac713c/src/functions/validate-mnemonic.ts#L20-L28
    pub fn new(mnemonic: TonMnemonic, passphrase: Option<String>) -> WalletResult<TonWallet> {
        let entropy = Self::ton_mnemonic_to_entropy(&mnemonic, passphrase.as_deref());

        match passphrase {
            Some(ref passphrase) if !passphrase.is_empty() => {
                // Check whether the passphrase is really needed.
                if !Self::is_password_needed(&mnemonic) {
                    return Err(WalletError::InvalidMnemonicEntropy);
                }
            },
            _ => (),
        };

        // The pair of `[mnemonic, Option<passphrase>]` should give a `basic` seed.
        // Otherwise, `passphrase` is either not set or invalid.
        if !is_basic_seed(&entropy) {
            return Err(WalletError::InvalidMnemonicEntropy);
        }

        let seed = ton_seed(&entropy);
        Ok(TonWallet {
            mnemonic,
            passphrase,
            entropy,
            seed,
        })
    }

    pub fn to_key_pair(&self) -> KeyPair {
        let (secret, _): (H256, H256) = self.seed.split();
        KeyPair::from(secret)
    }

    /// Whether the mnemonic can be used with a passphrase only.
    /// https://github.com/toncenter/tonweb-mnemonic/blob/a338a00d4ca0ed833431e0e49e4cfad766ac713c/src/functions/is-password-needed.ts#L5-L11
    fn is_password_needed(mnemonic: &TonMnemonic) -> bool {
        // Password mnemonic (without password) should be password seed, but not basic seed.
        let entropy = Self::ton_mnemonic_to_entropy(mnemonic, None);
        is_password_seed(&entropy) && !is_basic_seed(&entropy)
    }

    /// https://github.com/toncenter/tonweb-mnemonic/blob/a338a00d4ca0ed833431e0e49e4cfad766ac713c/src/functions/common.ts#L20-L23
    fn ton_mnemonic_to_entropy(mnemonic: &TonMnemonic, passphrase: Option<&str>) -> H512 {
        let passphrase_bytes = passphrase.map(str::as_bytes).unwrap_or(&[]);
        let entropy = hmac_sha512(mnemonic.as_str().as_bytes(), passphrase_bytes);
        H512::try_from(entropy.as_slice()).expect("hmac_sha512 must return 64 bytes")
    }
}

/// https://github.com/toncenter/tonweb-mnemonic/blob/a338a00d4ca0ed833431e0e49e4cfad766ac713c/src/functions/common.ts#L8-L11
fn is_basic_seed(entropy: &H512) -> bool {
    basic_seed(entropy)[0] == 0
}

/// https://github.com/toncenter/tonweb-mnemonic/blob/a338a00d4ca0ed833431e0e49e4cfad766ac713c/src/functions/mnemonic-to-seed.ts#L5-L17
fn ton_seed(entropy: &H512) -> H512 {
    pbkdf2_hmac_sha512(
        entropy.as_slice(),
        TON_WALLET_SALT,
        TON_WALLET_PBKDF_ITERATIONS,
    )
}

/// https://github.com/toncenter/tonweb-mnemonic/blob/a338a00d4ca0ed833431e0e49e4cfad766ac713c/src/functions/common.ts#L9
fn basic_seed(entropy: &H512) -> H512 {
    pbkdf2_hmac_sha512(
        entropy.as_slice(),
        TON_BASIC_SEED_SALT,
        TON_BASIC_SEED_ROUNDS,
    )
}

/// https://github.com/toncenter/tonweb-mnemonic/blob/a338a00d4ca0ed833431e0e49e4cfad766ac713c/src/functions/common.ts#L15
fn password_seed(entropy: &H512) -> H512 {
    pbkdf2_hmac_sha512(
        entropy.as_slice(),
        TON_PASSPHRASE_SEED_SALT,
        TON_PASSPHRASE_SEED_ROUNDS,
    )
}

/// https://github.com/toncenter/tonweb-mnemonic/blob/a338a00d4ca0ed833431e0e49e4cfad766ac713c/src/functions/common.ts#L14-L17
fn is_password_seed(entropy: &H512) -> bool {
    password_seed(entropy)[0] == 1
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_is_basic_seed() {
        let entropy = H512::from("db315e4b9a05d29d4921f3f99c754b52c1035bd20a6c38ae4c39068a844d2d4dccd1b16ab639494155bd635737c9120eab0b0382c6f27d941993ad2a98ee037b");
        let actual = basic_seed(&entropy);
        let expected = H512::from("008303d550147ad20420875810a81496510e32e2ec7b1c4129c8fe55c0886ad2b6c8b6ad88427d3614a27997ec3760e4a6aaae45c8a1c70684aad5206e8559ce");
        assert_eq!(actual, expected);
        assert!(is_basic_seed(&entropy));
    }

    #[test]
    fn test_is_not_basic_seed() {
        let entropy = H512::from("5ad5da67282f932eb9e0b66246af357e1e99f73b066ba1095fedf324629f67048c82e7c6d4cf09f204e2b2fcd002ab9bae25da67f99ecb918861d11ec6553a78");
        let actual = basic_seed(&entropy);
        let expected = H512::from("fd1ab5001f7fec237ad7b90dbd3a8a6d716409688d23517cc80314b79f36f93a21aac798c39778684688b4763bf0294874c067ef3d28d854101c4e5616839dfd");
        assert_eq!(actual, expected);
        assert!(!is_basic_seed(&entropy));
    }

    #[test]
    fn test_is_password_seed() {
        let entropy = H512::from("f0fc0e9610e3a215db47df1fc8dc2142ec4e8559ee1ec384fc3e51234d63c34087f4a6bdb2ad2c5a46a4504e30c9ab4ef7d92dc1836c2854ecef1f7988e60100");
        let actual = password_seed(&entropy);
        let expected = H512::from("014944aac60ad889acab074b850df15b745b2c6ca5367c3ba02f4ae22e5a8953ac33413c0cd7fdb9108935bd6ed82acc73d4ac94202d83933a5480642c371eed");
        assert_eq!(actual, expected);
        assert!(is_password_seed(&entropy));
    }

    #[test]
    fn test_is_not_password_seed() {
        let entropy = H512::from("85092586b7d675688d52b5870966546c7fb0144a2d94badd7c3960d6e9de3094016cea3aa155f5a9b3ce61d5b4ad8393984ed153dc0866304c911ba2edd2ea9e");
        let actual = password_seed(&entropy);
        let expected = H512::from("31f4dd02b333be5635dc46245d67792b85e9b74d788a749caf3d45bfb1cd094a039626ba1a8bff77fa2436a6a228568d29884c2b274e09c0fe722f07980aab8e");
        assert_eq!(actual, expected);
        assert!(!is_password_seed(&entropy));
    }
}
