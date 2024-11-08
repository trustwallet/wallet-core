// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::polkadot::{balance_call, ACCOUNT_2, GENESIS_HASH, PRIVATE_KEY_2};
use std::borrow::Cow;
use tw_any_coin::any_address::AnyAddress;
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::ed25519::sha512::PrivateKey;
use tw_keypair::traits::VerifyingKeyTrait;
use tw_number::U256;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Polkadot::Proto;
use tw_proto::Polkadot::Proto::mod_Balance::Transfer;

#[test]
fn test_polkadot_sign_transfer() {
    // Step 1: Prepare input.
    let to_address = "13ZLCqJNPsRZYEbwjtZZFpWt9GyFzg5WahXCVWKpWdUJqrQ5";
    {
        let private_key = PrivateKey::try_from(PRIVATE_KEY_2).unwrap();
        let public_key = private_key.public();
        let address = AnyAddress::with_string(CoinType::Polkadot, ACCOUNT_2, None)
            .unwrap()
            .get_data()
            .unwrap();
        assert_eq!(public_key.as_slice().to_vec(), address);
    }
    let private_key = PRIVATE_KEY_2.decode_hex().unwrap();
    let block_hash = "5d2143bb808626d63ad7e1cda70fa8697059d670a992e82cd440fbb95ea40351"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();
    let value = 2000000000u64;

    let input = Proto::SigningInput {
        network: 0,
        private_key: private_key.into(),
        nonce: 3,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 26,
        transaction_version: 5,
        era: Some(Proto::Era {
            block_number: 3541050,
            period: 64,
        }),
        message_oneof: balance_call(Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
            to_address: to_address.into(),
            value: Cow::Owned(U256::from(value).to_big_endian().to_vec()),
            ..Default::default()
        })),
        ..Default::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Polkadot, input);

    assert_eq!(output.error, SigningError::OK);
    // https://polkadot.subscan.io/extrinsic/0x9fd06208a6023e489147d8d93f0182b0cb7e45a40165247319b87278e08362d8
    assert_eq!(
        output.encoded.to_hex(),
        "3502849dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f7830073e59cef381aedf56d7af076bafff9857ffc1e3bd7d1d7484176ff5b58b73f1211a518e1ed1fd2ea201bd31869c0798bba4ffe753998c409d098b65d25dff801a5030c0005007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0300943577"
    );
}
