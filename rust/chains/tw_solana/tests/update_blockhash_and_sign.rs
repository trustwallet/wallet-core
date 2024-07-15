// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;
use tw_encoding::base58;
use tw_solana::modules::utils::SolanaTransaction;
use tw_solana::SOLANA_ALPHABET;

#[test]
fn test_update_recent_blockhash_and_sign() {
    // base64 encoded
    let encoded_tx = "AQPWaOi7dMdmQpXi8HyQQKwiqIftrg1igGQxGtZeT50ksn4wAnyH4DtDrkkuE0fqgx80LTp4LwNN9a440SrmoA8BAAEDZsL1CMnFVcrMn7JtiOiN1U4hC7WovOVof2DX51xM0H/GizyJTHgrBanCf8bGbrFNTn0x3pCGq30hKbywSTr6AgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAgIAAQwCAAAAKgAAAAAAAAA=";
    // base58 encoded
    let new_blockhash = "CyPYVsYWrsJNfVpi8aazu7WsrswNFuDd385z6GNoBGUg";
    let private_key = base58::decode(
        "A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr",
        SOLANA_ALPHABET,
    )
    .unwrap();

    let output =
        SolanaTransaction::update_blockhash_and_sign(encoded_tx, new_blockhash, &[private_key]);
    assert_eq!(output.error, SigningErrorType::OK);
    let expected = "AdQl49kO1FxfkAnAuK9KSQEGLzxHNYLqBrYGFN711q7aT/qyrzYMn/7/IdFBy6yMhjOA1CkwZsgmqmbu+XKvVAUBAAEDZsL1CMnFVcrMn7JtiOiN1U4hC7WovOVof2DX51xM0H/GizyJTHgrBanCf8bGbrFNTn0x3pCGq30hKbywSTr6AgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAseKSLpOE0fdq67Jk9Ckme2c3SYD//nqcykr/oci67zEBAgIAAQwCAAAAKgAAAAAAAAA=";
    assert_eq!(output.encoded, expected);
}
