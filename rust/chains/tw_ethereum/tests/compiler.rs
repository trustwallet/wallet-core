// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_coin_entry::coin_entry_ext::CoinEntryExt;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_encoding::hex;
use tw_ethereum::entry::EthereumEntry;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::tw;
use tw_number::U256;
use tw_proto::Ethereum::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;
use tw_proto::{deserialize, serialize};

#[test]
fn test_external_signature_sign() {
    let coin = TestCoinContext::default();

    let transfer = Proto::mod_Transaction::Transfer {
        amount: U256::encode_be_compact(1_000_000_000_000_000_000),
        data: Cow::default(),
    };
    let input = Proto::SigningInput {
        nonce: U256::encode_be_compact(11),
        chain_id: U256::encode_be_compact(1),
        gas_price: U256::encode_be_compact(20_000_000_000),
        gas_limit: U256::encode_be_compact(21_000),
        to_address: "0x3535353535353535353535353535353535353535".into(),
        transaction: Some(Proto::Transaction {
            transaction_oneof: Proto::mod_Transaction::OneOftransaction_oneof::transfer(transfer),
        }),
        ..Proto::SigningInput::default()
    };

    // Step 1: Obtain preimage hash
    let input_data = serialize(&input).unwrap();
    let preimage_data = EthereumEntry
        .preimage_hashes(&coin, &input_data)
        .expect("!preimage_hashes");
    let preimage: CompilerProto::PreSigningOutput =
        deserialize(&preimage_data).expect("Coin entry returned an invalid output");

    assert_eq!(preimage.error, SigningErrorType::OK);
    assert!(preimage.error_message.is_empty());
    assert_eq!(
        hex::encode(&preimage.data_hash, false),
        "15e180a6274b2f6a572b9b51823fce25ef39576d10188ecdcd7de44526c47217"
    );

    // Simulate signature, normally obtained from signature server
    let public_key = secp256k1::PublicKey::try_from("044bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382ce28cab79ad7119ee1ad3ebcdb98a16805211530ecc6cfefa1b88e6dff99232a").unwrap();
    let public_key = tw::PublicKey::Secp256k1Extended(public_key);
    let signature = hex::decode("360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07b53bd9d1376e15a191d844db458893b928f3efbfee90c9febf51ab84c9796677900").unwrap();

    // Verify signature (pubkey & hash & signature)
    assert!(public_key.verify(&signature, &preimage.data_hash));

    // Step 2: Compile transaction info
    let input_data = serialize(&input).unwrap();
    let output_data = EthereumEntry
        .compile(
            &coin,
            &input_data,
            vec![signature],
            vec![public_key.to_bytes()],
        )
        .expect("!compile");
    let output: Proto::SigningOutput =
        deserialize(&output_data).expect("Coin entry returned an invalid output");

    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());
    let expected_encoded = "f86c0b8504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a0360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07ba053bd9d1376e15a191d844db458893b928f3efbfee90c9febf51ab84c97966779";
    assert_eq!(hex::encode(output.encoded, false), expected_encoded);

    // Double check: check if simple signature process gives the same result. Note that private
    // keys were not used anywhere up to this point.
    let input = Proto::SigningInput {
        private_key: Cow::from(
            hex::decode("4646464646464646464646464646464646464646464646464646464646464646")
                .unwrap(),
        ),
        ..input
    };

    let input_data = serialize(&input).unwrap();
    let output_data = EthereumEntry
        .sign(&coin, &input_data)
        .expect("!output_data");
    let output: Proto::SigningOutput =
        deserialize(&output_data).expect("Coin entry returned an invalid output");
    assert_eq!(hex::encode(output.encoded, false), expected_encoded);
}
