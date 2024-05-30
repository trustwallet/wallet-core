// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::base58;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_data_vector_helper::TWDataVectorHelper;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Solana::Proto;
use tw_solana::SOLANA_ALPHABET;
use wallet_core_rs::ffi::solana::transaction::tw_solana_transaction_update_blockhash_and_sign;

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
