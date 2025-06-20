// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::hex;
use tw_evm::ffi::barz::{
    tw_barz_get_authorization_hash, tw_barz_get_counterfactual_address,
    tw_barz_get_diamond_cut_code, tw_barz_get_encoded_hash, tw_barz_get_formatted_signature,
    tw_barz_get_init_code, tw_barz_get_prefixed_msg_hash, tw_barz_get_signed_hash,
    tw_barz_sign_authorization,
};
use tw_keypair::{test_utils::tw_public_key_helper::TWPublicKeyHelper, tw::PublicKeyType};
use tw_memory::test_utils::{tw_data_helper::TWDataHelper, tw_string_helper::TWStringHelper};
use tw_number::U256;
use tw_proto::{
    serialize,
    Barz::Proto::{ContractAddressInput, DiamondCutInput, FacetCut, FacetCutAction},
};

#[test]
fn test_get_counterfactual_address_non_zero_salt_ffi() {
    let input = ContractAddressInput {
        account_facet: "0xF6F5e5fC74905e65e3FF53c6BacEba8535dd14d1".into(),
        verification_facet: "0xaB84813cbf26Fd951CB3d7E33Dccb8995027e490".into(),
        entry_point: "0x5FF137D4b0FDCD49DcA30c7CF57E578a026d2789".into(),
        facet_registry: "0x9a95d201BB8F559771784D12c01F8084278c65E5".into(),
        default_fallback: "0x522cDc7558b5f798dF5D61AB09B6D95Ebd342EF9".into(),
        public_key: "0xB5547FBdC56DCE45e1B8ef75569916D438e09c46".into(),
        bytecode: "0x60806040526040516104c83803806104c883398101604081905261002291610163565b6000858585858560405160240161003d959493929190610264565b60408051601f198184030181529181526020820180516001600160e01b0316634a93641760e01b1790525190915060009081906001600160a01b038a16906100869085906102c3565b600060405180830381855af49150503d80600081146100c1576040519150601f19603f3d011682016040523d82523d6000602084013e6100c6565b606091505b50915091508115806100e157506100dc816102df565b600114155b156100ff57604051636ff35f8960e01b815260040160405180910390fd5b505050505050505050610306565b80516001600160a01b038116811461012457600080fd5b919050565b634e487b7160e01b600052604160045260246000fd5b60005b8381101561015a578181015183820152602001610142565b50506000910152565b60008060008060008060c0878903121561017c57600080fd5b6101858761010d565b95506101936020880161010d565b94506101a16040880161010d565b93506101af6060880161010d565b92506101bd6080880161010d565b60a08801519092506001600160401b03808211156101da57600080fd5b818901915089601f8301126101ee57600080fd5b81518181111561020057610200610129565b604051601f8201601f19908116603f0116810190838211818310171561022857610228610129565b816040528281528c602084870101111561024157600080fd5b61025283602083016020880161013f565b80955050505050509295509295509295565b600060018060a01b0380881683528087166020840152808616604084015280851660608401525060a0608083015282518060a08401526102ab8160c085016020870161013f565b601f01601f19169190910160c0019695505050505050565b600082516102d581846020870161013f565b9190910192915050565b80516020808301519190811015610300576000198160200360031b1b821691505b50919050565b6101b3806103156000396000f3fe60806040523661000b57005b600080356001600160e01b03191681527f183cde5d4f6bb7b445b8fc2f7f15d0fd1d162275aded24183babbffee7cd491f6020819052604090912054819060601c806100cf576004838101546040516366ffd66360e11b81526000356001600160e01b031916928101929092526001600160a01b03169063cdffacc690602401602060405180830381865afa1580156100a8573d6000803e3d6000fd5b505050506040513d601f19601f820116820180604052508101906100cc919061014d565b90505b6001600160a01b0381166101295760405162461bcd60e51b815260206004820152601d60248201527f4261727a3a2046756e6374696f6e20646f6573206e6f74206578697374000000604482015260640160405180910390fd5b3660008037600080366000845af43d6000803e808015610148573d6000f35b3d6000fd5b60006020828403121561015f57600080fd5b81516001600160a01b038116811461017657600080fd5b939250505056fea2646970667358221220d35db061bb6ecdb7688c3674af669ce44d527cae4ded59214d06722d73da62be64736f6c63430008120033".into(),
        salt: 123456,
        factory: "0x96C489979E39F877BDb8637b75A25C1a5B2DE14C".into(),
    };
    let input = serialize(&input).unwrap();
    let input = TWDataHelper::create(input);

    let address = TWStringHelper::wrap(unsafe { tw_barz_get_counterfactual_address(input.ptr()) });
    assert_eq!(
        address.to_string(),
        Some("0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1".to_string())
    );
}

#[test]
fn test_get_init_code_ffi() {
    let public_key = TWPublicKeyHelper::with_hex("04e6f4e0351e2f556fd7284a9a033832bae046ac31fd529ad02ab6220870624b79eb760e718fdaed7a037dd1d77a561759cee9f2706eb55a729dc953e0d5719b02", PublicKeyType::Nist256p1Extended);

    let factory_address = TWStringHelper::create("0x3fC708630d85A3B5ec217E53100eC2b735d4f800");
    let verification_facet = TWStringHelper::create("0x6BF22ff186CC97D88ECfbA47d1473a234CEBEFDf");
    let salt = 0;

    let init_code = TWDataHelper::wrap(unsafe {
        tw_barz_get_init_code(
            factory_address.ptr(),
            public_key.ptr(),
            verification_facet.ptr(),
            salt,
        )
    });
    assert_eq!(hex::encode(init_code.to_vec().unwrap(), true), "0x3fc708630d85a3b5ec217e53100ec2b735d4f800296601cd0000000000000000000000006bf22ff186cc97d88ecfba47d1473a234cebefdf00000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004104e6f4e0351e2f556fd7284a9a033832bae046ac31fd529ad02ab6220870624b79eb760e718fdaed7a037dd1d77a561759cee9f2706eb55a729dc953e0d5719b0200000000000000000000000000000000000000000000000000000000000000");

    let salt = 1;
    let init_code = TWDataHelper::wrap(unsafe {
        tw_barz_get_init_code(
            factory_address.ptr(),
            public_key.ptr(),
            verification_facet.ptr(),
            salt,
        )
    });
    assert_eq!(hex::encode(init_code.to_vec().unwrap(), true), "0x3fc708630d85a3b5ec217e53100ec2b735d4f800296601cd0000000000000000000000006bf22ff186cc97d88ecfba47d1473a234cebefdf00000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000004104e6f4e0351e2f556fd7284a9a033832bae046ac31fd529ad02ab6220870624b79eb760e718fdaed7a037dd1d77a561759cee9f2706eb55a729dc953e0d5719b0200000000000000000000000000000000000000000000000000000000000000");
}

#[test]
fn test_get_formatted_signature_ffi() {
    let signature = TWDataHelper::create(hex::decode("3044022012d89e3b41e253dc9e90bd34dc1750d059b76d0b1d16af2059aa26e90b8960bf0220256d8a05572c654906ce422464693e280e243e6d9dbc5f96a681dba846bca276").unwrap());
    let challenge = TWDataHelper::create(
        hex::decode("cf267a78c5adaf96f341a696eb576824284c572f3e61be619694d539db1925f9").unwrap(),
    );
    let authenticator_data = TWDataHelper::create(
        hex::decode("1a70842af8c1feb7133b81e6a160a6a2be45ee057f0eb6d3f7f5126daa202e071d00000000")
            .unwrap(),
    );
    let client_data_json = TWStringHelper::create("{\"type\":\"webauthn.get\",\"challenge\":\"zyZ6eMWtr5bzQaaW61doJChMVy8-Yb5hlpTVOdsZJfk\",\"origin\":\"https://trustwallet.com\"}");

    let formatted_signature = TWDataHelper::wrap(unsafe {
        tw_barz_get_formatted_signature(
            signature.ptr(),
            challenge.ptr(),
            authenticator_data.ptr(),
            client_data_json.ptr(),
        )
    });

    assert_eq!(hex::encode(formatted_signature.to_vec().unwrap(), true), "0x12d89e3b41e253dc9e90bd34dc1750d059b76d0b1d16af2059aa26e90b8960bf256d8a05572c654906ce422464693e280e243e6d9dbc5f96a681dba846bca27600000000000000000000000000000000000000000000000000000000000000a00000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000016000000000000000000000000000000000000000000000000000000000000000251a70842af8c1feb7133b81e6a160a6a2be45ee057f0eb6d3f7f5126daa202e071d0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000247b2274797065223a22776562617574686e2e676574222c226368616c6c656e6765223a22000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000025222c226f726967696e223a2268747470733a2f2f747275737477616c6c65742e636f6d227d000000000000000000000000000000000000000000000000000000");
}

#[test]
fn test_get_prefixed_msg_hash_ffi() {
    let msg_hash =
        hex::decode("a6ebe22d8c1ec7edbd7f5776e49a161f67ab97161d7b8c648d80abf365765cf2").unwrap();
    let barz_address = "0x913233BfC283ffe89a5E70ADC39c0926d240bbD9";
    let chain_id = 3604;

    let prefixed_msg_hash = TWDataHelper::wrap(unsafe {
        tw_barz_get_prefixed_msg_hash(
            TWDataHelper::create(msg_hash).ptr(),
            TWStringHelper::create(barz_address).ptr(),
            chain_id,
        )
    });
    assert_eq!(
        hex::encode(prefixed_msg_hash.to_vec().unwrap(), true),
        "0x0488fb3e4fdaa890bf55532fc9840fb9edef9c38244f431c9430a78a86d89157"
    );
}

#[test]
fn test_get_prefixed_msg_hash_with_zero_chain_id_ffi() {
    let msg_hash =
        hex::decode("cf267a78c5adaf96f341a696eb576824284c572f3e61be619694d539db1925f9").unwrap();
    let barz_address = "0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1";
    let chain_id = 0;

    let prefixed_msg_hash = TWDataHelper::wrap(unsafe {
        tw_barz_get_prefixed_msg_hash(
            TWDataHelper::create(msg_hash).ptr(),
            TWStringHelper::create(barz_address).ptr(),
            chain_id,
        )
    });
    assert_eq!(
        hex::encode(prefixed_msg_hash.to_vec().unwrap(), true),
        "0xc74e78634261222af51530703048f98a1b7b995a606a624f0a008e7aaba7a21b"
    );
}

#[test]
fn test_get_diamond_cut_code_ffi() {
    let input = DiamondCutInput {
        init_address: "0x0000000000000000000000000000000000000000".into(),
        init_data: hex::decode("0x00").unwrap().into(),
        facet_cuts: vec![FacetCut {
            facet_address: "0x2279B7A0a67DB372996a5FaB50D91eAA73d2eBe6".into(),
            action: FacetCutAction::ADD,
            function_selectors: vec![hex::decode("fdd8a83c").unwrap().into()],
        }],
    };
    let input = serialize(&input).unwrap();
    let input = TWDataHelper::create(input);

    let diamond_cut_code = TWDataHelper::wrap(unsafe { tw_barz_get_diamond_cut_code(input.ptr()) });
    assert_eq!(hex::encode(diamond_cut_code.to_vec().unwrap(), true), "0x1f931c1c000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000140000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000200000000000000000000000002279b7a0a67db372996a5fab50d91eaa73d2ebe6000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000001fdd8a83c0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000000");
}

#[test]
fn test_get_diamond_cut_code_with_multiple_cut_ffi() {
    let input = DiamondCutInput {
        init_address: "0x2279B7A0a67DB372996a5FaB50D91eAA73d2eBe6".into(),
        init_data: hex::decode("12341234").unwrap().into(),
        facet_cuts: vec![
            FacetCut {
                facet_address: "0x2279B7A0a67DB372996a5FaB50D91eAA73d2eBe6".into(),
                action: FacetCutAction::ADD,
                function_selectors: vec![
                    hex::decode("fdd8a83c").unwrap().into(),
                    hex::decode("fdd8a83c").unwrap().into(),
                    hex::decode("fdd8a83c").unwrap().into(),
                ],
            },
            FacetCut {
                facet_address: "0x6e3c94d74af6227aEeF75b54a679e969189a6aEC".into(),
                action: FacetCutAction::ADD,
                function_selectors: vec![hex::decode("12345678").unwrap().into()],
            },
        ],
    };
    let input = serialize(&input).unwrap();
    let input = TWDataHelper::create(input);

    let diamond_cut_code = TWDataHelper::wrap(unsafe { tw_barz_get_diamond_cut_code(input.ptr()) });
    assert_eq!(hex::encode(diamond_cut_code.to_vec().unwrap(), true), "0x1f931c1c00000000000000000000000000000000000000000000000000000000000000600000000000000000000000002279b7a0a67db372996a5fab50d91eaa73d2ebe600000000000000000000000000000000000000000000000000000000000002400000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000001200000000000000000000000002279b7a0a67db372996a5fab50d91eaa73d2ebe6000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000003fdd8a83c00000000000000000000000000000000000000000000000000000000fdd8a83c00000000000000000000000000000000000000000000000000000000fdd8a83c000000000000000000000000000000000000000000000000000000000000000000000000000000006e3c94d74af6227aeef75b54a679e969189a6aec000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000001123456780000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000041234123400000000000000000000000000000000000000000000000000000000");
}

#[test]
fn test_get_diamond_cut_code_with_zero_selector_ffi() {
    let input = DiamondCutInput {
        init_address: "0x0000000000000000000000000000000000000000".into(),
        init_data: hex::decode("00").unwrap().into(),
        facet_cuts: vec![FacetCut {
            facet_address: "0x2279B7A0a67DB372996a5FaB50D91eAA73d2eBe6".into(),
            action: FacetCutAction::ADD,
            function_selectors: vec![],
        }],
    };
    let input = serialize(&input).unwrap();
    let input = TWDataHelper::create(input);

    let diamond_cut_code = TWDataHelper::wrap(unsafe { tw_barz_get_diamond_cut_code(input.ptr()) });
    assert_eq!(hex::encode(diamond_cut_code.to_vec().unwrap(), true), "0x1f931c1c000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000120000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000200000000000000000000000002279b7a0a67db372996a5fab50d91eaa73d2ebe600000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000000");
}

#[test]
fn test_get_diamond_cut_code_with_long_init_data_ffi() {
    let input = DiamondCutInput {
        init_address: "0x0000000000000000000000000000000000000000".into(),
        init_data: hex::decode("b61d27f6000000000000000000000000c2ce171d25837cd43e496719f5355a847edc679b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000024a526d83b00000000000000000000000090f79bf6eb2c4f870365e785982e1f101e93b90600000000000000000000000000000000000000000000000000000000").unwrap().into(),
        facet_cuts: vec![
            FacetCut {
                facet_address: "0x2279B7A0a67DB372996a5FaB50D91eAA73d2eBe6".into(),
                action: FacetCutAction::ADD,
                function_selectors: vec![],
            },
        ],
    };
    let input = serialize(&input).unwrap();
    let input = TWDataHelper::create(input);

    let diamond_cut_code = TWDataHelper::wrap(unsafe { tw_barz_get_diamond_cut_code(input.ptr()) });
    assert_eq!(hex::encode(diamond_cut_code.to_vec().unwrap(), true), "0x1f931c1c000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000120000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000200000000000000000000000002279b7a0a67db372996a5fab50d91eaa73d2ebe600000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000c4b61d27f6000000000000000000000000c2ce171d25837cd43e496719f5355a847edc679b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000024a526d83b00000000000000000000000090f79bf6eb2c4f870365e785982e1f101e93b9060000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
}

#[test]
fn test_get_authorization_hash_ffi() {
    let chain_id = U256::from(1u64).to_big_endian();
    let chain_id = TWDataHelper::create(chain_id.to_vec());

    let contract_address = TWStringHelper::create("0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1");

    let nonce = U256::from(1u64).to_big_endian();
    let nonce = TWDataHelper::create(nonce.to_vec());

    let authorization_hash = TWDataHelper::wrap(unsafe {
        tw_barz_get_authorization_hash(chain_id.ptr(), contract_address.ptr(), nonce.ptr())
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
        tw_barz_sign_authorization(
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
        tw_barz_get_encoded_hash(
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
        TWDataHelper::wrap(unsafe { tw_barz_get_signed_hash(hash.ptr(), private_key.ptr()) });
    assert_eq!(hex::encode(signed_hash.to_vec().unwrap(), true), "0xa29e460720e4b539f593d1a407827d9608cccc2c18b7af7b3689094dca8a016755bca072ffe39bc62285b65aff8f271f20798a421acf18bb2a7be8dbe0eb05f81c");
}
