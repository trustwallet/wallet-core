// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::hex;
use tw_evm::modules::biz::{get_encoded_hash, sign_user_op_hash};
use tw_number::U256;

#[test]
fn test_get_encoded_hash() {
    let chain_id = U256::from(31337u64).to_big_endian();
    let code_address = "0x2e234DAe75C793f67A35089C9d99245E1C58470b";
    let code_name = "Biz";
    let code_version = "v1.0.0";
    let type_hash = "0x4f51e7a567f083a31264743067875fc6a7ae45c32c5bd71f6a998c4625b13867";
    let domain_separator_hash =
        "0xd87cd6ef79d4e2b95e15ce8abf732db51ec771f1ca2edccf22a46c729ac56472";
    let sender = "0x174a240e5147D02dE4d7724D5D3E1c1bF11cE029";
    let user_op_hash = "0xf177858c1c500e51f38ffe937bed7e4d3a8678725900be4682d3ce04d97071eb";

    let encoded_hash = get_encoded_hash(
        &chain_id[..],
        code_address,
        code_name,
        code_version,
        type_hash,
        domain_separator_hash,
        sender,
        user_op_hash,
    )
    .unwrap();
    assert_eq!(
        hex::encode(encoded_hash, true),
        "0xc63891abc38f7a991f89ad7cb6d7e53543627b0536c3f5e545b736756c971635"
    );
}

#[test]
fn test_sign_user_op_hash() {
    let hash = "0xc63891abc38f7a991f89ad7cb6d7e53543627b0536c3f5e545b736756c971635";
    let private_key = "0x947dd69af402e7f48da1b845dfc1df6be593d01a0d8274bd03ec56712e7164e8";
    let signed_hash = sign_user_op_hash(hash, private_key).unwrap();
    assert_eq!(hex::encode(signed_hash, true), "0xa29e460720e4b539f593d1a407827d9608cccc2c18b7af7b3689094dca8a016755bca072ffe39bc62285b65aff8f271f20798a421acf18bb2a7be8dbe0eb05f81c");
}
