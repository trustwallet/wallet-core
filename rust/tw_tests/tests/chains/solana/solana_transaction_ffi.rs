// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::sign_utils::{AnySignerHelper, CompilerHelper, PreImageHelper};
use tw_any_coin::test_utils::transaction_decode_utils::TransactionDecoderHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::base64::STANDARD;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_encoding::{base58, base64};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_data_vector_helper::TWDataVectorHelper;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Solana::Proto::{self};
use tw_solana::SOLANA_ALPHABET;
use wallet_core_rs::ffi::solana::transaction::{
    tw_solana_transaction_get_compute_unit_limit, tw_solana_transaction_get_compute_unit_price,
    tw_solana_transaction_set_compute_unit_limit, tw_solana_transaction_set_compute_unit_price,
    tw_solana_transaction_set_fee_payer, tw_solana_transaction_update_blockhash_and_sign,
};

#[test]
fn test_solana_transaction_update_blockhash_and_sign_token_transfer_with_external_fee_payer() {
    // base64 encoded
    // https://explorer.solana.com/tx/3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej?cluster=devnet
    let encoded_tx = "AnQTYwZpkm3fs4SdLxnV6gQj3hSLsyacpxDdLMALYWObm722f79IfYFTbZeFK9xHtMumiDOWAM2hHQP4r/GtbARpncaXgOVFv7OgbRLMbuCEJHO1qwcdCbtH72VzyzU8yw9sqqHIAaCUE8xaQTgT6Z5IyZfeyMe2QGJIfOjz65UPAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8Aqe6sdLXiXSDILEtzckCjkjchiSf6zVGpMYiAE5BE2IqHAQUEAgQDAQoMoA8AAAAAAAAG";
    let encoded_tx = TWStringHelper::create(encoded_tx);

    // base58 encoded
    let new_blockhash = "CyPYVsYWrsJNfVpi8aazu7WsrswNFuDd385z6GNoBGUg";
    let new_blockhash = TWStringHelper::create(new_blockhash);

    let my_private_key = base58::decode(
        "9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5",
        SOLANA_ALPHABET,
    )
    .unwrap();
    let fee_payer_private_key = base58::decode(
        "66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c",
        SOLANA_ALPHABET,
    )
    .unwrap();
    let private_keys = TWDataVectorHelper::create([fee_payer_private_key, my_private_key]);

    let output_data = unsafe {
        TWDataHelper::wrap(tw_solana_transaction_update_blockhash_and_sign(
            encoded_tx.ptr(),
            new_blockhash.ptr(),
            private_keys.ptr(),
        ))
        .to_vec()
        .expect("Expected a non-null output data")
    };
    let output: Proto::SigningOutput = tw_proto::deserialize(&output_data).unwrap();
    assert_eq!(output.error, SigningError::OK);

    let expected = "Ajzc/Tke0CG8Cew5qFa6xZI/7Ya3DN0M8Ige6tKPsGzhg8Bw9DqL18KUrEZZ1F4YqZBo4Rv+FsDT8A7Nss7p4A6BNVZzzGprCJqYQeNg0EVIbmPc6mDitNniHXGeKgPZ6QZbM4FElw9O7IOFTpOBPvQFeqy0vZf/aayncL8EK/UEAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8AqbHiki6ThNH3auuyZPQpJntnN0mA//56nMpK/6HIuu8xAQUEAgQDAQoMoA8AAAAAAAAG";
    assert_eq!(output.encoded, expected);
}

#[test]
fn test_solana_transaction_update_blockhash_and_sign_no_matching_pubkey() {
    // base64 encoded
    let encoded_tx = "AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAQAHEIoR5xuWyrvjIW4xU7CWlPOfyFAiy8B295hGo6tNjBmRCgUkQaFYTleMcAX2p74eBXQZd1dwDyQZAPJfSv2KGc5kcFLJj5qd2BVMaSNGVPfVBm74GbLwUq5/U1Ccdqc2gokZQxRDpMq7aeToP3nRaWIP4RXMxN+LJetccXMPq/QumgOqt7kkqk07cyPCKgYoQ4fQtOqqZn5sEqjWHYj3CDS5ha48uggePWu090s1ff4yoCjAvULeZ+cqYFn+Adk5Teyfw71W3u/F6VTnLQEPW96gJr5Kcm3bGi08n224JyF++PTko52VL0CIM2xtl0WkvNslD6Wawxr7yd9HYllN4Lz8lFwXilWGgyJdOq1qqBuZbE49glHeCO/sJHNnIHC0BgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwZGb+UhFzL/7K26csOb57yM5bvF9xJrLEObOkAAAAAEedVb8jHAbu50xW7OaBUH/bGy3qP0jlECsc2iVrwTjwbd9uHXZaGT2cvhRs7reawctIXtX1s3kTqM9YV+/wCpjJclj04kifG7PRApFI4NgwtaE5na/xCEBI572Nvp+Fm0P/on9df2SnTAmx8pWHneSwmrNt/J3VFLMhqns4zl6OL4d+g9rsaIj0Orta57MRu3jDSWCJf85ae4LBbiD/GXvOojZjsHekJrpRUuPggLJr943hDVD5UareeEucjCvaoHCgAFAsBcFQAKAAkDBBcBAAAAAAANBgAGACMJDAEBCQIABgwCAAAAAMqaOwAAAAAMAQYBEQs1DA8ABgEFAiMhCwsOCx0MDxoBGQcYBAgDJBscDB4PBwUQEhEfFR8UFwcFISITHw8MDCAfFgstwSCbM0HWnIEAAwAAABEBZAABCh0BAyZHAQMAypo7AAAAAJaWFAYAAAAAMgAADAMGAAABCQPZoILFk7gfE2y5bt3AC+g/4OwNzdiHKBhIbdeYvYFEjQPKyMkExMUkx0R25UNa/g5KsG0vfUwdUJ8e8HecK/Jkd3qm9XefBOB0BaD1+J+dBJz09vfyGuRYZH09HfdE/kL8v6Ql+H03+tO+9lMmmVg8O1c6gAN6eX0Cbn4=";
    let encoded_tx = TWStringHelper::create(encoded_tx);

    // base58 encoded
    let new_blockhash = "CyPYVsYWrsJNfVpi8aazu7WsrswNFuDd385z6GNoBGUg";
    let new_blockhash = TWStringHelper::create(new_blockhash);

    // there is no matching pubkey in the transaction account keys.
    let private_key = base58::decode(
        "A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr",
        SOLANA_ALPHABET,
    )
    .unwrap();
    let private_keys = TWDataVectorHelper::create([private_key]);

    let output_data = unsafe {
        TWDataHelper::wrap(tw_solana_transaction_update_blockhash_and_sign(
            encoded_tx.ptr(),
            new_blockhash.ptr(),
            private_keys.ptr(),
        ))
        .to_vec()
        .expect("Expected a non-null output data")
    };
    let output: Proto::SigningOutput = tw_proto::deserialize(&output_data).unwrap();
    assert_eq!(output.error, SigningError::Error_missing_private_key);
}

#[test]
fn test_solana_transaction_update_blockhash_and_sign_empty_private_keys() {
    // base64 encoded
    // https://explorer.solana.com/tx/3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej?cluster=devnet
    let encoded_tx = "AnQTYwZpkm3fs4SdLxnV6gQj3hSLsyacpxDdLMALYWObm722f79IfYFTbZeFK9xHtMumiDOWAM2hHQP4r/GtbARpncaXgOVFv7OgbRLMbuCEJHO1qwcdCbtH72VzyzU8yw9sqqHIAaCUE8xaQTgT6Z5IyZfeyMe2QGJIfOjz65UPAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8Aqe6sdLXiXSDILEtzckCjkjchiSf6zVGpMYiAE5BE2IqHAQUEAgQDAQoMoA8AAAAAAAAG";
    let encoded_tx = TWStringHelper::create(encoded_tx);

    // base58 encoded
    let new_blockhash = "CyPYVsYWrsJNfVpi8aazu7WsrswNFuDd385z6GNoBGUg";
    let new_blockhash = TWStringHelper::create(new_blockhash);

    // empty private keys
    let private_keys = TWDataVectorHelper::default();

    let output_data = unsafe {
        TWDataHelper::wrap(tw_solana_transaction_update_blockhash_and_sign(
            encoded_tx.ptr(),
            new_blockhash.ptr(),
            private_keys.ptr(),
        ))
        .to_vec()
        .expect("Expected a non-null output data")
    };
    let output: Proto::SigningOutput = tw_proto::deserialize(&output_data).unwrap();
    assert_eq!(output.error, SigningError::OK);

    let expected = "AgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8AqbHiki6ThNH3auuyZPQpJntnN0mA//56nMpK/6HIuu8xAQUEAgQDAQoMoA8AAAAAAAAG";
    assert_eq!(output.encoded, expected);

    let expected_message = "AgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8AqbHiki6ThNH3auuyZPQpJntnN0mA//56nMpK/6HIuu8xAQUEAgQDAQoMoA8AAAAAAAAG";
    assert_eq!(output.unsigned_tx, expected_message);
}

struct SetPriorityTestInput {
    /// Hex-encoded private key.
    private_key: &'static str,
    /// Base64-encoded original transaction.
    original_tx: &'static str,
    /// Priority fee limit if present in the original transaction.
    original_limit: Option<u32>,
    /// Priority fee price if present in the original transaction.
    original_price: Option<u64>,
    /// Base64-encoded transaction after the priority fee and limit being changed.
    updated_tx: &'static str,
    /// Priority fee limit to be set to.
    limit: u32,
    /// Priority fee price to be set to.
    price: u64,
    /// Base64-encoded [`SetPriorityTestInput::updated_tx`] transaction that has been signed.
    signed_tx: &'static str,
    /// Hex-encoded signature of the [`SetPriorityTestInput::signed_tx`] transaction.
    signature: &'static str,
}

fn test_solana_transaction_set_priority_fee(input: SetPriorityTestInput) {
    let original_tx = TWStringHelper::create(input.original_tx);

    // Step 1 - Check if there is are no price and limit instructions.

    let original_price = TWStringHelper::wrap(unsafe {
        tw_solana_transaction_get_compute_unit_price(original_tx.ptr())
    });
    let original_limit = TWStringHelper::wrap(unsafe {
        tw_solana_transaction_get_compute_unit_limit(original_tx.ptr())
    });
    assert_eq!(
        original_price.to_string(),
        input.original_price.map(|v| v.to_string()),
        "Invalid original price"
    );
    assert_eq!(
        original_limit.to_string(),
        input.original_limit.map(|v| v.to_string()),
        "Invalid original limit"
    );

    // Step 2 - Set price and limit instructions.

    let updated_tx = TWStringHelper::wrap(unsafe {
        tw_solana_transaction_set_compute_unit_limit(
            original_tx.ptr(),
            TWStringHelper::create(&input.limit.to_string()).ptr(),
        )
    });
    let updated_tx = TWStringHelper::wrap(unsafe {
        tw_solana_transaction_set_compute_unit_price(
            updated_tx.ptr(),
            TWStringHelper::create(&input.price.to_string()).ptr(),
        )
    });

    // Step 3 - Check if price and limit instructions are set successfully.

    let actual_limit = TWStringHelper::wrap(unsafe {
        tw_solana_transaction_get_compute_unit_limit(updated_tx.ptr())
    });
    let actual_price = TWStringHelper::wrap(unsafe {
        tw_solana_transaction_get_compute_unit_price(updated_tx.ptr())
    });
    assert_eq!(
        actual_limit.to_string(),
        Some(input.limit.to_string()),
        "Invalid updated limit"
    );
    assert_eq!(
        actual_price.to_string(),
        Some(input.price.to_string()),
        "Invalid updated price"
    );

    let actual_updated_tx = updated_tx.to_string().unwrap();
    assert_eq!(actual_updated_tx, input.updated_tx);

    // Step 4 - Decode transaction into a `RawMessage` Protobuf.

    let tx_data = base64::decode(&actual_updated_tx, STANDARD).unwrap();
    let mut decoder = TransactionDecoderHelper::<Proto::DecodingTransactionOutput>::default();
    let decode_output = decoder.decode(CoinType::Solana, tx_data);

    // Step 5 - Sign the decoded `RawMessage` transaction.

    let signing_input = Proto::SigningInput {
        private_key: input.private_key.decode_hex().unwrap().into(),
        raw_message: decode_output.transaction,
        tx_encoding: Proto::Encoding::Base64,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Solana, signing_input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    assert_eq!(
        output.encoded, input.signed_tx,
        "Invalid signed transaction"
    );
    assert_eq!(
        output.signatures.first().unwrap().signature,
        input.signature,
        "Invalid signature"
    );
}

#[test]
fn test_solana_transaction_set_priority_fee_transfer() {
    // Successfully broadcasted tx:
    // https://explorer.solana.com/tx/2ho7wZUXbDNz12xGfsXg2kcNMqkBAQjv7YNXNcVcuCmbC4p9FZe9ELeM2gMjq9MKQPpmE3nBW5pbdgwVCfNLr1h8
    test_solana_transaction_set_priority_fee(SetPriorityTestInput {
        private_key: "baf2b2dbbbad7ca96c1fa199c686f3d8fbd2c7b352f307e37e04f33df6741f18",
        original_tx: "AX43+Ir2EDqf2zLEvgzFrCZKRjdr3wCdp8CnvYh6N0G/s86IueX9BbiNUl16iLRGvwREDfi2Srb0hmLNBFw1BwABAAEDODI+iWe7g68B9iwCy8bFkJKvsIEj350oSOpcv4gNnv/st+6qmqipl9lwMK6toB9TiL7LrJVfij+pKwr+pUKxfwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAG6GdPcA92ORzVJe2jfG8KQqqMHr9YTLu30oM4i7MFEoBAgIAAQwCAAAA6AMAAAAAAAA=",
        original_limit: None,
        original_price: None,
        updated_tx: "AX43+Ir2EDqf2zLEvgzFrCZKRjdr3wCdp8CnvYh6N0G/s86IueX9BbiNUl16iLRGvwREDfi2Srb0hmLNBFw1BwABAAIEODI+iWe7g68B9iwCy8bFkJKvsIEj350oSOpcv4gNnv/st+6qmqipl9lwMK6toB9TiL7LrJVfij+pKwr+pUKxfwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwZGb+UhFzL/7K26csOb57yM5bvF9xJrLEObOkAAAAAboZ09wD3Y5HNUl7aN8bwpCqowev1hMu7fSgziLswUSgMDAAUCECcAAAICAAEMAgAAAOgDAAAAAAAAAwAJA+gDAAAAAAAA",
        limit: 10_000,
        price: 1_000,
        signed_tx: "AVUye82Mv+/aWeU2G+B6Nes365mUU2m8iqcGZn/8kFJvw4wY6AgKGG+vJHaknHlCDwE1yi1SIMVUUtNCOm3kHg8BAAIEODI+iWe7g68B9iwCy8bFkJKvsIEj350oSOpcv4gNnv/st+6qmqipl9lwMK6toB9TiL7LrJVfij+pKwr+pUKxfwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwZGb+UhFzL/7K26csOb57yM5bvF9xJrLEObOkAAAAAboZ09wD3Y5HNUl7aN8bwpCqowev1hMu7fSgziLswUSgMDAAUCECcAAAICAAEMAgAAAOgDAAAAAAAAAwAJA+gDAAAAAAAA",
        signature: "2ho7wZUXbDNz12xGfsXg2kcNMqkBAQjv7YNXNcVcuCmbC4p9FZe9ELeM2gMjq9MKQPpmE3nBW5pbdgwVCfNLr1h8",
    });
}

#[test]
fn test_solana_transaction_set_priority_fee_transfer_override() {
    // Successfully broadcasted tx:
    // https://explorer.solana.com/tx/3dxcZP8Q8WTj2JapeDLymQZvn1wGxPpism59RcaWA28igXo8Lyh1xRbAjipv7hrgo6JKyoAxbZ6ADayZLWqNJZg1
    test_solana_transaction_set_priority_fee(SetPriorityTestInput {
        private_key: "baf2b2dbbbad7ca96c1fa199c686f3d8fbd2c7b352f307e37e04f33df6741f18",
        original_tx: "Ab8KqzeQeInEQVRNDD1akh2B0EEeb5XJFfyVjqDrocz49NcZ6x5cp76zC9mjZKspsVCWVYIYdvNRYJYVB2OK3g4BAAIEODI+iWe7g68B9iwCy8bFkJKvsIEj350oSOpcv4gNnv/st+6qmqipl9lwMK6toB9TiL7LrJVfij+pKwr+pUKxfwMGRm/lIRcy/+ytunLDm+e8jOW7xfcSayxDmzpAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABwXONiS/AgQzMMUxsafHXszeF2QJdufrIFBZHMZlu8/gMCAAkDLAEAAAAAAAACAAUCiBMAAAMCAAEMAgAAAOgDAAAAAAAA",
        original_limit: Some(5_000),
        original_price: Some(300),
        updated_tx: "Ab8KqzeQeInEQVRNDD1akh2B0EEeb5XJFfyVjqDrocz49NcZ6x5cp76zC9mjZKspsVCWVYIYdvNRYJYVB2OK3g4BAAIEODI+iWe7g68B9iwCy8bFkJKvsIEj350oSOpcv4gNnv/st+6qmqipl9lwMK6toB9TiL7LrJVfij+pKwr+pUKxfwMGRm/lIRcy/+ytunLDm+e8jOW7xfcSayxDmzpAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABwXONiS/AgQzMMUxsafHXszeF2QJdufrIFBZHMZlu8/gMCAAkD6AMAAAAAAAACAAUCECcAAAMCAAEMAgAAAOgDAAAAAAAA",
        limit: 10_000,
        price: 1_000,
        signed_tx: "AYPn6T3Qqfqg/IW+2FL1suZyUDrhFVlEiKBaCAWAtINfuLgWkDw7Q+QaDvGGRa7eMgEnj4hfCA92tssQByeCCw4BAAIEODI+iWe7g68B9iwCy8bFkJKvsIEj350oSOpcv4gNnv/st+6qmqipl9lwMK6toB9TiL7LrJVfij+pKwr+pUKxfwMGRm/lIRcy/+ytunLDm+e8jOW7xfcSayxDmzpAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABwXONiS/AgQzMMUxsafHXszeF2QJdufrIFBZHMZlu8/gMCAAkD6AMAAAAAAAACAAUCECcAAAMCAAEMAgAAAOgDAAAAAAAA",
        signature: "3dxcZP8Q8WTj2JapeDLymQZvn1wGxPpism59RcaWA28igXo8Lyh1xRbAjipv7hrgo6JKyoAxbZ6ADayZLWqNJZg1",
    });
}

#[test]
fn test_solana_transaction_set_priority_fee_transfer_with_address_lookup() {
    // Successfully broadcasted tx:
    // https://explorer.solana.com/tx/4vkDYvXnAyauDwgQUT9pjhvArCm1jZZFp6xFiT6SYKDHwabPNyNskzzd8YJZR4UJVXakBtRAFku3axVQoA7Apido
    test_solana_transaction_set_priority_fee(SetPriorityTestInput {
        private_key: "a73613f265ae572772e28bd921a7bcd0f020e188aae4d1ebd07b9b487fde3573",
        original_tx: "Acjjw79E1WGC531Oq6lsyg3X+D65Fo37i6A4nKaqDjxWVrF/djOxbyShfg0HwMuCMp2Mj2JtsiGMkJbt/iD8rQ2AAQABAgEFuF07bbuXk4EuSECBjRkLhDeGEZ4Jm4QbrRF/TN0CAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAUE/bdT0AlTN8m+IZkrbzaTUxHcnoKW3WZucqu7msImQEBAgACDAIAAAD0AQAAAAAAAAGDq5uNdcMMtPgcMghpRBOza1eEzcNhvr0zid3hRZoV0wEAAA==",
        original_limit: None,
        original_price: None,
        updated_tx: "Acjjw79E1WGC531Oq6lsyg3X+D65Fo37i6A4nKaqDjxWVrF/djOxbyShfg0HwMuCMp2Mj2JtsiGMkJbt/iD8rQ2AAQACAwEFuF07bbuXk4EuSECBjRkLhDeGEZ4Jm4QbrRF/TN0CAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADBkZv5SEXMv/srbpyw5vnvIzlu8X3EmssQ5s6QAAAABQT9t1PQCVM3yb4hmStvNpNTEdyegpbdZm5yq7uawiZAwIABQIQJwAAAQIAAwwCAAAA9AEAAAAAAAACAAkD6AMAAAAAAAABg6ubjXXDDLT4HDIIaUQTs2tXhM3DYb69M4nd4UWaFdMBAAA=",
        limit: 10_000,
        price: 1_000,
        signed_tx: "AcRmE7GRYYh3XKjfYpBRvTdXjYMLtowRUxaStETGPTm0qxa7sm11yqGXKiO3SgOdsRL2Y9IQRBMfqwkBZyrK9gKAAQACAwEFuF07bbuXk4EuSECBjRkLhDeGEZ4Jm4QbrRF/TN0CAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADBkZv5SEXMv/srbpyw5vnvIzlu8X3EmssQ5s6QAAAABQT9t1PQCVM3yb4hmStvNpNTEdyegpbdZm5yq7uawiZAwIABQIQJwAAAQIAAwwCAAAA9AEAAAAAAAACAAkD6AMAAAAAAAABg6ubjXXDDLT4HDIIaUQTs2tXhM3DYb69M4nd4UWaFdMBAAA=",
        signature: "4vkDYvXnAyauDwgQUT9pjhvArCm1jZZFp6xFiT6SYKDHwabPNyNskzzd8YJZR4UJVXakBtRAFku3axVQoA7Apido",
    });
}

#[test]
fn test_solana_transaction_set_fee_payer() {
    let encoded_tx_str = "AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAQABA2uEKrOPvZNBtdUtSFXcg8+kj4O/Z1Ht/hwvnaqq5s6mTXd3KtwUyJFfRs2PBfeQW8xCEZvNr/5J/Tx8ltbn0pwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACo+QRbvXWNKoOfaOL4cSpfYrmn/2TV+dBmct+HsmmwdAQICAAEMAgAAAACcnwYAAAAAAA==";
    let encoded_tx = TWStringHelper::create(encoded_tx_str);

    let fee_payer_str = "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ";
    let fee_payer = TWStringHelper::create(fee_payer_str);

    // Step 1 - Add fee payer to the transaction.
    let updated_tx = TWStringHelper::wrap(unsafe {
        tw_solana_transaction_set_fee_payer(encoded_tx.ptr(), fee_payer.ptr())
    });
    let updated_tx = updated_tx.to_string().unwrap();
    assert_eq!(updated_tx, "AgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgAIAAQTLKvCJtWpVdze8Fxjgy/Iyz1sC4U7gqnxmdSM/X2+bV2uEKrOPvZNBtdUtSFXcg8+kj4O/Z1Ht/hwvnaqq5s6mTXd3KtwUyJFfRs2PBfeQW8xCEZvNr/5J/Tx8ltbn0pwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACo+QRbvXWNKoOfaOL4cSpfYrmn/2TV+dBmct+HsmmwdAQMCAQIMAgAAAACcnwYAAAAAAA==");

    // Step 2 - Decode transaction into a `RawMessage` Protobuf.
    let tx_data = base64::decode(&updated_tx, STANDARD).unwrap();
    let mut decoder = TransactionDecoderHelper::<Proto::DecodingTransactionOutput>::default();
    let output = decoder.decode(CoinType::Solana, tx_data);

    assert_eq!(output.error, SigningError::OK);
    let decoded_tx = output.transaction.unwrap();

    let signing_input = Proto::SigningInput {
        raw_message: Some(decoded_tx),
        tx_encoding: Proto::Encoding::Base64,
        ..Proto::SigningInput::default()
    };

    // Step 3 - Obtain preimage hash.
    let mut pre_imager = PreImageHelper::<Proto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Solana, &signing_input);

    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(
        preimage_output.data.to_hex(),
        "8002000104cb2af089b56a557737bc1718e0cbf232cf5b02e14ee0aa7c6675233f5f6f9b576b842ab38fbd9341b5d52d4855dc83cfa48f83bf6751edfe1c2f9daaaae6cea64d77772adc14c8915f46cd8f05f7905bcc42119bcdaffe49fd3c7c96d6e7d29c00000000000000000000000000000000000000000000000000000000000000002a3e4116ef5d634aa0e7da38be1c4a97d8ae69ffd9357e74199cb7e1ec9a6c1d01030201020c02000000009c9f060000000000"
    );

    // Step 4 - Compile transaction info.
    // Simulate signature, normally obtained from signature server.
    let fee_payer_signature = "feb9f15cc345fa156450676100033860edbe80a6f61dab8199e94fdc47678ecfdb95e3bc10ec0a7f863ab8ef5c38edae72db7e5d72855db225fd935fd59b700a".decode_hex().unwrap();
    let fee_payer_public_key = base58::decode(fee_payer_str, SOLANA_ALPHABET).unwrap();

    let sol_sender_signature = "936cd6d176e701d1f748031925b2f029f6f1ab4b99aec76e24ccf05649ec269569a08ec0bd80f5fee1cb8d13ecd420bf50c5f64ae74c7afa267458cabb4e5804".decode_hex().unwrap();
    let sol_sender_public_key = "6b842ab38fbd9341b5d52d4855dc83cfa48f83bf6751edfe1c2f9daaaae6cea6"
        .decode_hex()
        .unwrap();

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(
        CoinType::Solana,
        &signing_input,
        vec![fee_payer_signature, sol_sender_signature],
        vec![fee_payer_public_key, sol_sender_public_key],
    );

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted tx:
    // https://explorer.solana.com/tx/66PAVjxFVGP4ctrkXmyNRhp6BdFT7gDe1k356DZzCRaBDTmJZF1ewGsbujWRjDTrt5utnz8oHZw3mg8qBNyct41w?cluster=devnet
    assert_eq!(output.encoded, "Av658VzDRfoVZFBnYQADOGDtvoCm9h2rgZnpT9xHZ47P25XjvBDsCn+GOrjvXDjtrnLbfl1yhV2yJf2TX9WbcAqTbNbRducB0fdIAxklsvAp9vGrS5mux24kzPBWSewmlWmgjsC9gPX+4cuNE+zUIL9QxfZK50x6+iZ0WMq7TlgEgAIAAQTLKvCJtWpVdze8Fxjgy/Iyz1sC4U7gqnxmdSM/X2+bV2uEKrOPvZNBtdUtSFXcg8+kj4O/Z1Ht/hwvnaqq5s6mTXd3KtwUyJFfRs2PBfeQW8xCEZvNr/5J/Tx8ltbn0pwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACo+QRbvXWNKoOfaOL4cSpfYrmn/2TV+dBmct+HsmmwdAQMCAQIMAgAAAACcnwYAAAAAAA==");
    assert_eq!(output.unsigned_tx, "gAIAAQTLKvCJtWpVdze8Fxjgy/Iyz1sC4U7gqnxmdSM/X2+bV2uEKrOPvZNBtdUtSFXcg8+kj4O/Z1Ht/hwvnaqq5s6mTXd3KtwUyJFfRs2PBfeQW8xCEZvNr/5J/Tx8ltbn0pwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACo+QRbvXWNKoOfaOL4cSpfYrmn/2TV+dBmct+HsmmwdAQMCAQIMAgAAAACcnwYAAAAAAA==");
}

#[test]
fn test_solana_transaction_set_fee_payer_already_exists() {
    let encoded_tx_str = "AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAQABA2uEKrOPvZNBtdUtSFXcg8+kj4O/Z1Ht/hwvnaqq5s6mTXd3KtwUyJFfRs2PBfeQW8xCEZvNr/5J/Tx8ltbn0pwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACo+QRbvXWNKoOfaOL4cSpfYrmn/2TV+dBmct+HsmmwdAQICAAEMAgAAAACcnwYAAAAAAA==";
    let encoded_tx = TWStringHelper::create(encoded_tx_str);

    let fee_payer_str = "8EhWjZGEt58UKzeiburZVx6QQF3rbayScpDjPNqCx62q";
    let fee_payer = TWStringHelper::create(fee_payer_str);

    let updated_tx = TWStringHelper::wrap(unsafe {
        tw_solana_transaction_set_fee_payer(encoded_tx.ptr(), fee_payer.ptr())
    });

    // The fee payer is already in the transaction.
    // We expect tw_solana_transaction_set_fee_payer to return null.
    assert_eq!(updated_tx.to_string(), None);
}
