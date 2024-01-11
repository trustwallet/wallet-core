// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![no_main]

use libfuzzer_sys::{arbitrary, fuzz_target};

#[derive(arbitrary::Arbitrary, Debug)]
struct HashInput<'a> {
    data: &'a [u8],
    additional_data: &'a [u8],
    hash_size: usize,
}

fuzz_target!(|input: HashInput<'_>| {
    tw_hash::blake::blake_256(input.data);
    tw_hash::blake2::blake2_b(input.data, input.hash_size).ok();
    tw_hash::blake2::blake2_b_personal(input.data, input.hash_size, input.additional_data).ok();
    tw_hash::crc32::crc32(input.data);
    tw_hash::groestl::groestl_512(input.data);
    tw_hash::hmac::hmac_sha256(input.additional_data, input.data);
    tw_hash::ripemd::ripemd_160(input.data);
    tw_hash::sha1::sha1(input.data);
    tw_hash::sha2::sha256(input.data);
    tw_hash::sha2::sha512(input.data);
    tw_hash::sha2::sha512_256(input.data);
    tw_hash::sha3::keccak256(input.data);
    tw_hash::sha3::keccak512(input.data);
    tw_hash::sha3::sha3_256(input.data);
    tw_hash::sha3::sha3_512(input.data);
});
