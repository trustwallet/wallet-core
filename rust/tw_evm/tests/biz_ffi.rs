// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;
use tw_encoding::hex;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_evm::abi::prebuild::erc20::Erc20;
use tw_evm::address::Address;
use tw_evm::ffi::biz::{
    tw_biz_get_encoded_hash, tw_biz_get_signed_hash, tw_biz_sign_execute_with_signature_call,
};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;
use tw_number::U256;
use tw_proto::serialize;
use tw_proto::Biz::Proto::{EncodingHashParams, ExecuteWithSignatureInput, Execution};

#[test]
fn test_get_encoded_hash_ffi() {
    let chain_id = U256::from(31337u64).to_big_endian();
    let chain_id = TWDataHelper::create(chain_id.to_vec());

    let code_address = TWStringHelper::create("0x2e234DAe75C793f67A35089C9d99245E1C58470b");
    let code_name = TWStringHelper::create("Biz");
    let code_version = TWStringHelper::create("v1.0.0");
    let type_hash = TWStringHelper::create(
        "0x4f51e7a567f083a31264743067875fc6a7ae45c32c5bd71f6a998c4625b13867",
    );
    let domain_separator_hash = TWStringHelper::create(
        "0xd87cd6ef79d4e2b95e15ce8abf732db51ec771f1ca2edccf22a46c729ac56472",
    );
    let sender = TWStringHelper::create("0x174a240e5147D02dE4d7724D5D3E1c1bF11cE029");
    let user_op_hash = TWStringHelper::create(
        "0xf177858c1c500e51f38ffe937bed7e4d3a8678725900be4682d3ce04d97071eb",
    );

    let encoded_hash = TWDataHelper::wrap(unsafe {
        tw_biz_get_encoded_hash(
            chain_id.ptr(),
            code_address.ptr(),
            code_name.ptr(),
            code_version.ptr(),
            type_hash.ptr(),
            domain_separator_hash.ptr(),
            sender.ptr(),
            user_op_hash.ptr(),
        )
    });
    assert_eq!(
        hex::encode(encoded_hash.to_vec().unwrap(), true),
        "0xc63891abc38f7a991f89ad7cb6d7e53543627b0536c3f5e545b736756c971635"
    );
}

#[test]
fn test_get_signed_hash_ffi() {
    let hash = TWStringHelper::create(
        "0xc63891abc38f7a991f89ad7cb6d7e53543627b0536c3f5e545b736756c971635",
    );
    let private_key = TWStringHelper::create(
        "0x947dd69af402e7f48da1b845dfc1df6be593d01a0d8274bd03ec56712e7164e8",
    );
    let signed_hash =
        TWDataHelper::wrap(unsafe { tw_biz_get_signed_hash(hash.ptr(), private_key.ptr()) });
    assert_eq!(hex::encode(signed_hash.to_vec().unwrap(), true), "0xa29e460720e4b539f593d1a407827d9608cccc2c18b7af7b3689094dca8a016755bca072ffe39bc62285b65aff8f271f20798a421acf18bb2a7be8dbe0eb05f81c");
}

#[test]
fn test_execute_with_signature_ffi() {
    let spender = Address::from_str("0xBC472b43BC237f733c78a581078F58A6a89c46Ec").unwrap();
    let allowance = U256::from(1000_u64);
    // 0xeAff4df45C35c6d80aDc2E353F8Fc5b93613ef2b
    let private_key = "0xefec50f00ef0c09d967f3e363ee96502ce18a1881f6ac22321aa58071d43c66f";
    let nonce = U256::encode_be_compact(0);

    let chain_id = U256::encode_be_compact(56);
    let biz_address = "0xba083F0EeAF806603d31582D4e7667fB5A4A1B30";
    let biz_code_name = "Biz";
    let biz_code_version = "v1.0.0";
    // keccak("Biz(bytes32 execHash)")
    let type_hash = "0xec429430bbd6d0e373848272230d6fe2bac6319d903762e089c5cae97af53df0";
    // keccak("EIP712Domain(string name,string version,uint256 chainId,address verifyingContract,bytes32 salt)")
    let biz_domain_separator_hash =
        "0xd87cd6ef79d4e2b95e15ce8abf732db51ec771f1ca2edccf22a46c729ac56472";

    let input = ExecuteWithSignatureInput {
        executions: vec![
            // IERC20(TWT).approve()
            Execution {
                address: "0x4B0F1812e5Df2A09796481Ff14017e6005508003".into(),
                amount: U256::encode_be_compact(0),
                payload: Erc20::approve(spender, allowance).unwrap().into(),
            },
        ],
        private_key: private_key.decode_hex().unwrap().into(),
        nonce,
        encoding_hash_params: Some(EncodingHashParams {
            chain_id,
            code_address: biz_address.into(),
            code_name: biz_code_name.into(),
            code_version: biz_code_version.into(),
            type_hash: type_hash.into(),
            domain_separator_hash: biz_domain_separator_hash.into(),
        }),
    };
    let input = TWDataHelper::create(serialize(&input).unwrap());

    let encoded =
        TWDataHelper::wrap(unsafe { tw_biz_sign_execute_with_signature_call(input.ptr()) });

    // Successfully broadcasted tx:
    // https://bscscan.com/tx/0xaeef330e78c73fe1725323e1be66bf59e731a351788de88e238fe137ba5c38d1
    let expected = "0x1d92e4b600000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000160000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000200000000000000000000000004b0f1812e5df2a09796481ff14017e6005508003000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044095ea7b3000000000000000000000000bc472b43bc237f733c78a581078f58a6a89c46ec00000000000000000000000000000000000000000000000000000000000003e8000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000041db18e3a0150ddef964810e480b25592942a22d0b583f7d5cbb33ef6fb4baa66e753af78e967ee374070e16cf963a6cd7a3adb713e50d553aefbc361c48366a101b00000000000000000000000000000000000000000000000000000000000000";
    assert_eq!(encoded.to_vec().unwrap().to_hex_prefixed(), expected);
}
