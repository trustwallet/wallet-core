// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde_json::json;
use std::borrow::Cow;
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_any_coin::test_utils::wallet_connect_utils::WalletConnectRequestHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::base58;
use tw_encoding::base58::Alphabet;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Solana::Proto;
use tw_proto::WalletConnect::Proto as WCProto;

fn b58(s: &str) -> Cow<'static, [u8]> {
    base58::decode(s, Alphabet::BITCOIN).unwrap().into()
}

fn pubkey_signature(pubkey: &str, signature: &str) -> Proto::PubkeySignature<'static> {
    Proto::PubkeySignature {
        pubkey: pubkey.to_string().into(),
        signature: signature.to_string().into(),
    }
}

#[test]
fn test_solana_sign_wallet_connect_case_1() {
    let tx_encoded_to_sign = "AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAAEDZsL1CMnFVcrMn7JtiOiN1U4hC7WovOVof2DX51xM0H/GizyJTHgrBanCf8bGbrFNTn0x3pCGq30hKbywSTr6AgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAgIAAQwCAAAAKgAAAAAAAAA=";
    let tx_encoded_signed = "AQPWaOi7dMdmQpXi8HyQQKwiqIftrg1igGQxGtZeT50ksn4wAnyH4DtDrkkuE0fqgx80LTp4LwNN9a440SrmoA8BAAEDZsL1CMnFVcrMn7JtiOiN1U4hC7WovOVof2DX51xM0H/GizyJTHgrBanCf8bGbrFNTn0x3pCGq30hKbywSTr6AgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAgIAAQwCAAAAKgAAAAAAAAA=";

    let request_params = json!({
        "transaction": tx_encoded_to_sign
    });
    let input = WCProto::ParseRequestInput {
        protocol: WCProto::Protocol::V2,
        method: WCProto::Method::SolanaSignTransaction,
        payload: request_params.to_string().into(),
    };

    let mut parser = WalletConnectRequestHelper::default();
    let parsing_output = parser.parse(CoinType::Solana, &input);

    let mut signing_input = match parsing_output.signing_input_oneof {
        WCProto::mod_ParseRequestOutput::OneOfsigning_input_oneof::solana(input) => input,
        _ => unreachable!(),
    };

    // Set missing private key.
    signing_input.private_key = b58("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr");
    signing_input.tx_encoding = Proto::Encoding::Base64;

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let signing_output = signer.sign(CoinType::Solana, signing_input);

    assert_eq!(signing_output.error, SigningError::OK);
    assert_eq!(signing_output.encoded, tx_encoded_signed);
    let expected_signatures = vec![pubkey_signature(
        "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q",
        "5T6uZBHnHFd8uWErDBTFRVkbKuhbcm94K5MJ2beTYDruzqv4FjS7EMKvC94ZfxNAiWUXZ6bZxS3WXUbhJwYNPWn",
    )];
    assert_eq!(signing_output.signatures, expected_signatures);
}

#[test]
fn test_solana_sign_wallet_connect_with_external_fee_payer() {
    let tx_encoded_to_sign = "AgVnzLgdHFLYfmSR0mp/npPzRqaabwMYqy8dj7ohDQsDQ0NzDH7V06lqN+LrTpfLgT7smx+rgrU4+c991xODzQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgAGCssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFqUGMlXapwAxr2PwiP0cVc/cXJIjeEKqE2/Y8U6ILrnF0haJP+0BwRhu21/HIt1jGstyQAp1VG1/U6s2C1l4wIgwjHcAvSCmA99mRXB7PUzdAkdOMBgtJSH+cXZMuB37XY7RCyzkSFX8TqTPQE0KC0DK1/+zQGi2/G3eQYI3wAupwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABt324ddloZPZy+FGzut5rBy0he1fWzeROoz1hX7/AKkGp9UXGSxcUSGMyUw9SvF/WNruCJuh/UTj29mKAAAAAIyXJY9OJInxuz0QKRSODYMLWhOZ2v8QhASOe9jb6fhZzipDMbzjZw5uqL7f9ZCMbZH4M6Maf96sFpeMJhoYAdUCCQcAAgQFBgcIAAcEAwUCAQoMoA8AAAAAAAAG";
    // https://explorer.solana.com/tx/7GZGFT2VA4dpJBBwjiMqj1o8yChDvoCsqnQ7xz4GxY513W3efxRqbB8y7g4tH2GEGQRx4vCkKipG1sMaDEen1A2?cluster=devnet
    let tx_encoded_signed = "AgVnzLgdHFLYfmSR0mp/npPzRqaabwMYqy8dj7ohDQsDQ0NzDH7V06lqN+LrTpfLgT7smx+rgrU4+c991xODzQOK74uU+ESOWD5RtUxJNbGRG7mOvDBTpGhZhlRRWEsR6D/hpQc0r8kzuXMUK3OptIklCXmLKjptzYUcCAlwTUsOAgAGCssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFqUGMlXapwAxr2PwiP0cVc/cXJIjeEKqE2/Y8U6ILrnF0haJP+0BwRhu21/HIt1jGstyQAp1VG1/U6s2C1l4wIgwjHcAvSCmA99mRXB7PUzdAkdOMBgtJSH+cXZMuB37XY7RCyzkSFX8TqTPQE0KC0DK1/+zQGi2/G3eQYI3wAupwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABt324ddloZPZy+FGzut5rBy0he1fWzeROoz1hX7/AKkGp9UXGSxcUSGMyUw9SvF/WNruCJuh/UTj29mKAAAAAIyXJY9OJInxuz0QKRSODYMLWhOZ2v8QhASOe9jb6fhZzipDMbzjZw5uqL7f9ZCMbZH4M6Maf96sFpeMJhoYAdUCCQcAAgQFBgcIAAcEAwUCAQoMoA8AAAAAAAAG";

    let request_params = json!({
        "transaction": tx_encoded_to_sign
    });
    let input = WCProto::ParseRequestInput {
        protocol: WCProto::Protocol::V2,
        method: WCProto::Method::SolanaSignTransaction,
        payload: request_params.to_string().into(),
    };

    let mut parser = WalletConnectRequestHelper::default();
    let parsing_output = parser.parse(CoinType::Solana, &input);

    let mut signing_input = match parsing_output.signing_input_oneof {
        WCProto::mod_ParseRequestOutput::OneOfsigning_input_oneof::solana(input) => input,
        _ => unreachable!(),
    };

    // Set missing private key.
    signing_input.private_key = b58("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
    signing_input.tx_encoding = Proto::Encoding::Base64;

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let signing_output = signer.sign(CoinType::Solana, signing_input);

    assert_eq!(signing_output.error, SigningError::OK);
    assert_eq!(signing_output.encoded, tx_encoded_signed);

    let expected_signatures = vec![
        pubkey_signature(
            "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ",
            "7GZGFT2VA4dpJBBwjiMqj1o8yChDvoCsqnQ7xz4GxY513W3efxRqbB8y7g4tH2GEGQRx4vCkKipG1sMaDEen1A2",
        ),
        pubkey_signature(
            "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V",
            "3n7RHTCBAtnFVuDn5eRbyQB24h6AqajJi5nGMPrfnUVFUDh2Cb8AoaJ7mVtjnv73V4HaJCzSwCLAj3zcGEaFftWZ"
        )
    ];
    assert_eq!(signing_output.signatures, expected_signatures);
}
