// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_any_coin::test_utils::sign_utils::{CompilerHelper, PreImageHelper};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::ed25519::{sha512::PublicKey, Signature};
use tw_keypair::traits::VerifyingKeyTrait;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Polkadot::Proto::{self, SigningInput};
use tw_proto::TxCompiler::Proto::{self as CompilerProto, PreSigningOutput};

mod polkadot_address;
mod polkadot_compile;
mod polkadot_sign;
mod polkadot_transaction_util;

const GENESIS_HASH: &str = "91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3";
const PRIVATE_KEY: &str = "abf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115";
const PRIVATE_KEY_IOS: &str = "37932b086586a6675e66e562fe68bd3eeea4177d066619c602fe3efc290ada62";
const PRIVATE_KEY_2: &str = "70a794d4f1019c3ce002f33062f45029c4f930a56b3d20ec477f7668c6bbc37f";
const PRIVATE_KEY_POLKADOT: &str =
    "298fcced2b497ed48367261d8340f647b3fca2d9415d57c2e3c5ef90482a2266";
const ACCOUNT_2: &str = "14Ztd3KJDaB9xyJtRkREtSZDdhLSbm7UUKt8Z7AwSv7q85G2";

pub fn helper_sign(coin: CoinType, input: SigningInput<'_>) -> String {
    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let signed_output = signer.sign(coin, input);
    assert_eq!(signed_output.error, SigningError::OK);

    signed_output.encoded.to_hex()
}

pub fn helper_encode(coin: CoinType, input: &SigningInput<'_>) -> String {
    let mut pre_imager = PreImageHelper::<PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(coin, input);

    assert_eq!(preimage_output.error, SigningError::OK);
    preimage_output.data.to_hex()
}

pub fn helper_encode_and_maybe_sign(
    coin: CoinType,
    input: SigningInput<'_>,
) -> (String, Option<String>) {
    // Step 1: Obtain preimage hash
    let preimage = helper_encode(coin, &input);
    if input.private_key.is_empty() {
        return (preimage, None);
    }

    // Step 2: If we have the private key try signing.
    let signed = helper_sign(coin, input);

    (preimage, Some(signed))
}

pub fn helper_encode_and_compile(
    coin: CoinType,
    input: Proto::SigningInput,
    signature: &str,
    public_key: &str,
    ed25519: bool,
) -> (String, String) {
    // Step 1: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(coin, &input);

    assert_eq!(
        preimage_output.error,
        SigningError::OK,
        "{}",
        preimage_output.error_message
    );
    let preimage = preimage_output.data.to_hex();

    // Step 2: Compile transaction info

    // Simulate signature, normally obtained from signature server
    let signature_bytes = signature.decode_hex().unwrap();
    let public_key = public_key.decode_hex().unwrap();

    // Verify signature (pubkey & hash & signature)
    if !ed25519 {
        let signature = Signature::try_from(signature_bytes.as_slice()).unwrap();
        let public = PublicKey::try_from(public_key.as_slice()).unwrap();
        assert!(public.verify(signature, preimage_output.data.into()));
    }

    // Compile transaction info
    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(coin, &input, vec![signature_bytes], vec![public_key]);
    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    let signed = output.encoded.to_hex();

    (preimage, signed)
}

fn balance_call(
    call: Proto::mod_Balance::OneOfmessage_oneof,
) -> Proto::mod_SigningInput::OneOfmessage_oneof {
    Proto::mod_SigningInput::OneOfmessage_oneof::balance_call(Proto::Balance {
        message_oneof: call,
    })
}

pub fn staking_call(
    call: Proto::mod_Staking::OneOfmessage_oneof,
) -> Proto::mod_SigningInput::OneOfmessage_oneof {
    Proto::mod_SigningInput::OneOfmessage_oneof::staking_call(Proto::Staking {
        message_oneof: call,
    })
}
