// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub mod crypto_aes_cbc;
pub mod crypto_aes_ctr;
pub mod crypto_hd_node;
pub mod crypto_mnemonic;
pub mod crypto_pbkdf2;
pub mod crypto_scrypt;
pub mod ffi;

pub const KEY_SIZE_AES_128: usize = 16;
pub const KEY_SIZE_AES_192: usize = 24;
pub const KEY_SIZE_AES_256: usize = 32;
