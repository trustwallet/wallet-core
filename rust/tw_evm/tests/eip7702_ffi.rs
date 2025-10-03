// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::hex;
use tw_evm::ffi::eip7702::{tw_eip7702_get_authorization_hash, tw_eip7702_sign_authorization};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;
use tw_number::U256;

#[test]
fn test_get_authorization_hash_ffi() {
    let chain_id = U256::from(1u64).to_big_endian();
    let chain_id = TWDataHelper::create(chain_id.to_vec());

    let contract_address = TWStringHelper::create("0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1");

    let nonce = U256::from(1u64).to_big_endian();
    let nonce = TWDataHelper::create(nonce.to_vec());

    let authorization_hash = TWDataHelper::wrap(unsafe {
        tw_eip7702_get_authorization_hash(chain_id.ptr(), contract_address.ptr(), nonce.ptr())
    });
    assert_eq!(
        hex::encode(authorization_hash.to_vec().unwrap(), true),
        "0x3ae543b2fa103a39a6985d964a67caed05f6b9bb2430ad6d498cda743fe911d9"
    ); // Verified with viem
}

#[test]
fn test_sign_authorization_ffi() {
    let chain_id = U256::from(1u64).to_big_endian();
    let chain_id = TWDataHelper::create(chain_id.to_vec());

    let contract_address = TWStringHelper::create("0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1");

    let nonce = U256::from(1u64).to_big_endian();
    let nonce = TWDataHelper::create(nonce.to_vec());

    let private_key = TWStringHelper::create(
        "0x947dd69af402e7f48da1b845dfc1df6be593d01a0d8274bd03ec56712e7164e8",
    );

    let signed_authorization = TWStringHelper::wrap(unsafe {
        tw_eip7702_sign_authorization(
            chain_id.ptr(),
            contract_address.ptr(),
            nonce.ptr(),
            private_key.ptr(),
        )
    });
    let json: serde_json::Value =
        serde_json::from_str(&signed_authorization.to_string().unwrap()).unwrap();

    // Verified with viem
    assert_eq!(
        json["chainId"].as_str().unwrap(),
        hex::encode(U256::from(1u64).to_big_endian_compact(), true)
    );
    assert_eq!(
        json["address"].as_str().unwrap(),
        "0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1"
    );
    assert_eq!(
        json["nonce"].as_str().unwrap(),
        hex::encode(U256::from(1u64).to_big_endian_compact(), true)
    );
    assert_eq!(json["yParity"].as_str().unwrap(), hex::encode(&[1u8], true));
    assert_eq!(
        json["r"].as_str().unwrap(),
        "0x2c39f2f64441dd38c364ee175dc6b9a87f34ca330bce968f6c8e22811e3bb710"
    );
    assert_eq!(
        json["s"].as_str().unwrap(),
        "0x5f1bcde93dee4b214e60bc0e63babcc13e4fecb8a23c4098fd89844762aa012c"
    );
}
