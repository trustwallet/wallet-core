// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::{blake, blake2, groestl, hmac, ripemd, sha1, sha2, sha3};
use tw_memory::ffi::c_byte_array::CByteArray;

/// Computes the Blake-256 hash of the `input` byte array.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn blake_256(input: *const u8, input_len: usize) -> CByteArray {
    let input = std::slice::from_raw_parts(input, input_len);
    blake::blake_256(input).into()
}

/// Computes the BLAKE2B hash of the `input` byte array.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \param hash_size the size of the output hash.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn blake2_b(
    input: *const u8,
    input_len: usize,
    hash_size: usize,
) -> CByteArray {
    let input = std::slice::from_raw_parts(input, input_len);
    blake2::blake2_b(input, hash_size).into()
}

/// Computes the personalized BLAKE2B hash of the `input` byte array.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \param hash_size the size of the output hash.
/// \param personal_input *non-null* byte array.
/// \param personal_len the length of the `personal_input` array.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn blake2_b_personal(
    input: *const u8,
    input_len: usize,
    hash_size: usize,
    personal_input: *const u8,
    personal_len: usize,
) -> CByteArray {
    let input = std::slice::from_raw_parts(input, input_len);
    let personal = std::slice::from_raw_parts(personal_input, personal_len);
    blake2::blake2_b_personal(input, hash_size, personal).into()
}

/// Computes the Groestl-512 hash of the `input` byte array.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn groestl_512(input: *const u8, input_len: usize) -> CByteArray {
    let input = std::slice::from_raw_parts(input, input_len);
    groestl::groestl_512(input).into()
}

/// Hash-based Message Authentication Code (HMAC) using the SHA-256 hash function.
/// \param key *non-null* byte array.
/// \param key_len the length of the `key` array.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn hmac__sha256(
    key: *const u8,
    key_len: usize,
    input: *const u8,
    input_len: usize,
) -> CByteArray {
    let key = std::slice::from_raw_parts(key, key_len);
    let input = std::slice::from_raw_parts(input, input_len);
    hmac::hmac_sha256(key, input).into()
}

/// Computes the RIPEMD-160 hash of the `input` byte array.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn ripemd_160(input: *const u8, input_len: usize) -> CByteArray {
    let input = std::slice::from_raw_parts(input, input_len);
    ripemd::ripemd_160(input).into()
}

/// Computes the SHA-1 hash of the `input` byte array.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn sha1(input: *const u8, input_len: usize) -> CByteArray {
    let input = std::slice::from_raw_parts(input, input_len);
    sha1::sha1(input).into()
}

/// Computes the SHA-256 hash of the `input` byte array.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn sha256(input: *const u8, input_len: usize) -> CByteArray {
    let input = std::slice::from_raw_parts(input, input_len);
    sha2::sha256(input).into()
}

/// Computes the SHA-512 hash of the `input` byte array.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn sha512(input: *const u8, input_len: usize) -> CByteArray {
    let input = std::slice::from_raw_parts(input, input_len);
    sha2::sha512(input).into()
}

/// Computes the SHA-512/256 hash of the `input` byte array.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn sha512_256(input: *const u8, input_len: usize) -> CByteArray {
    let input = std::slice::from_raw_parts(input, input_len);
    sha2::sha512_256(input).into()
}

/// Computes the Keccak-256 hash of the `input` byte array.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn keccak256(input: *const u8, input_len: usize) -> CByteArray {
    let input = std::slice::from_raw_parts(input, input_len);
    sha3::keccak256(input).into()
}

/// Computes the Keccak-512 hash of the `input` byte array.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn keccak512(input: *const u8, input_len: usize) -> CByteArray {
    let input = std::slice::from_raw_parts(input, input_len);
    sha3::keccak512(input).into()
}

/// Computes the SHA-3-256 hash of the `input` byte array.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn sha3__256(input: *const u8, input_len: usize) -> CByteArray {
    let input = std::slice::from_raw_parts(input, input_len);
    sha3::sha3_256(input).into()
}

/// Computes the SHA-3-512 hash of the `input` byte array.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \return C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn sha3__512(input: *const u8, input_len: usize) -> CByteArray {
    let input = std::slice::from_raw_parts(input, input_len);
    sha3::sha3_512(input).into()
}
