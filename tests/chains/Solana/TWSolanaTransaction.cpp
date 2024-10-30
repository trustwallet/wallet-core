// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TrustWalletCore/TWSolanaTransaction.h"
#include "TrustWalletCore/TWTransactionDecoder.h"
#include "TrustWalletCore/TWAnySigner.h"
#include "proto/Solana.pb.h"
#include "TestUtilities.h"
#include "Base64.h"
#include "Base58.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
namespace TW::Solana::tests {

TEST(TWSolanaTransaction, UpdateBlockhashAndSign) {
    // base64 encoded
    // https://explorer.solana.com/tx/3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej?cluster=devnet
    auto encodedTx = STRING("AnQTYwZpkm3fs4SdLxnV6gQj3hSLsyacpxDdLMALYWObm722f79IfYFTbZeFK9xHtMumiDOWAM2hHQP4r/GtbARpncaXgOVFv7OgbRLMbuCEJHO1qwcdCbtH72VzyzU8yw9sqqHIAaCUE8xaQTgT6Z5IyZfeyMe2QGJIfOjz65UPAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8Aqe6sdLXiXSDILEtzckCjkjchiSf6zVGpMYiAE5BE2IqHAQUEAgQDAQoMoA8AAAAAAAAG");
    // base58 encoded
    auto newBlockhash = STRING("CyPYVsYWrsJNfVpi8aazu7WsrswNFuDd385z6GNoBGUg");

    auto myPrivateKey = DATA("7f0932159226ddec9e1a4b0b8fe7cdc135049f9e549a867d722aa720dd64f32e");
    auto feePayerPrivateKey = DATA("4b9d6f57d28b06cbfa1d4cc710953e62d653caf853415c56ffd9d150acdeb7f7");
    auto privateKeysVec = WRAP(TWDataVector, TWDataVectorCreateWithData(myPrivateKey.get()));
    TWDataVectorAdd(privateKeysVec.get(), feePayerPrivateKey.get());

    auto outputData = WRAPD(TWSolanaTransactionUpdateBlockhashAndSign(encodedTx.get(), newBlockhash.get(), privateKeysVec.get()));

    Proto::SigningOutput output;
    output.ParseFromArray(TWDataBytes(outputData.get()), static_cast<int>(TWDataSize(outputData.get())));

    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);
    EXPECT_EQ(output.encoded(), "Ajzc/Tke0CG8Cew5qFa6xZI/7Ya3DN0M8Ige6tKPsGzhg8Bw9DqL18KUrEZZ1F4YqZBo4Rv+FsDT8A7Nss7p4A6BNVZzzGprCJqYQeNg0EVIbmPc6mDitNniHXGeKgPZ6QZbM4FElw9O7IOFTpOBPvQFeqy0vZf/aayncL8EK/UEAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8AqbHiki6ThNH3auuyZPQpJntnN0mA//56nMpK/6HIuu8xAQUEAgQDAQoMoA8AAAAAAAAG");
}

TEST(TWSolanaTransaction, DecodeUpdateBlockhashAndSign) {
    // base64 encoded
    // https://explorer.solana.com/tx/3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej?cluster=devnet
    auto encodedTx = Base64::decode("AnQTYwZpkm3fs4SdLxnV6gQj3hSLsyacpxDdLMALYWObm722f79IfYFTbZeFK9xHtMumiDOWAM2hHQP4r/GtbARpncaXgOVFv7OgbRLMbuCEJHO1qwcdCbtH72VzyzU8yw9sqqHIAaCUE8xaQTgT6Z5IyZfeyMe2QGJIfOjz65UPAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8Aqe6sdLXiXSDILEtzckCjkjchiSf6zVGpMYiAE5BE2IqHAQUEAgQDAQoMoA8AAAAAAAAG");
    auto newBlockhash = "CyPYVsYWrsJNfVpi8aazu7WsrswNFuDd385z6GNoBGUg";
    auto senderPrivateKey = Base58::decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
    auto feePayerPrivateKey = Base58::decode("66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c");

    // Step 1: Decode the transaction.

    auto encodedTxPtr = WRAPD(TWDataCreateWithBytes(encodedTx.data(), encodedTx.size()));
    auto decodeOutputPtr = WRAPD(TWTransactionDecoderDecode(TWCoinTypeSolana, encodedTxPtr.get()));

    Proto::DecodingTransactionOutput decodeOutput;
    decodeOutput.ParseFromArray(
        TWDataBytes(decodeOutputPtr.get()),
        static_cast<int>(TWDataSize(decodeOutputPtr.get()))
    );

    EXPECT_EQ(decodeOutput.error(), Common::Proto::SigningError::OK);
    ASSERT_TRUE(decodeOutput.transaction().has_legacy());
    // Previous fee payer signature.
    EXPECT_EQ(
        decodeOutput.transaction().signatures(0).signature(),
        "3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej"
    );
    // Previous sender signature.
    EXPECT_EQ(
        decodeOutput.transaction().signatures(1).signature(),
        "37UT8U6DdqaWqV6yQuHcRN3JpMefDgVna8LJJPmmDNKYMwmefEgvcreSg9AqSsT7cU2rMBNyDktEtDU19ZqqZvML"
    );

    // Step 2. Prepare a signing input and update the recent-blockhash.

    Proto::SigningInput signingInput;
    *signingInput.mutable_raw_message() = decodeOutput.transaction();
    signingInput.mutable_raw_message()->mutable_legacy()->set_recent_blockhash(newBlockhash);
    signingInput.set_private_key(senderPrivateKey.data(), senderPrivateKey.size());
    signingInput.set_fee_payer_private_key(feePayerPrivateKey.data(), feePayerPrivateKey.size());
    signingInput.set_tx_encoding(Proto::Encoding::Base64);

    // Step 3. Re-sign the updated transaction.

    Proto::SigningOutput output;
    ANY_SIGN(signingInput, TWCoinTypeSolana);

    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);
    EXPECT_EQ(output.encoded(), "Ajzc/Tke0CG8Cew5qFa6xZI/7Ya3DN0M8Ige6tKPsGzhg8Bw9DqL18KUrEZZ1F4YqZBo4Rv+FsDT8A7Nss7p4A6BNVZzzGprCJqYQeNg0EVIbmPc6mDitNniHXGeKgPZ6QZbM4FElw9O7IOFTpOBPvQFeqy0vZf/aayncL8EK/UEAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8AqbHiki6ThNH3auuyZPQpJntnN0mA//56nMpK/6HIuu8xAQUEAgQDAQoMoA8AAAAAAAAG");
}

TEST(TWSolanaTransaction, SetPriorityFee) {
    // base64 encoded
    const auto privateKey = parse_hex("baf2b2dbbbad7ca96c1fa199c686f3d8fbd2c7b352f307e37e04f33df6741f18");
    const auto originalTx = STRING("AX43+Ir2EDqf2zLEvgzFrCZKRjdr3wCdp8CnvYh6N0G/s86IueX9BbiNUl16iLRGvwREDfi2Srb0hmLNBFw1BwABAAEDODI+iWe7g68B9iwCy8bFkJKvsIEj350oSOpcv4gNnv/st+6qmqipl9lwMK6toB9TiL7LrJVfij+pKwr+pUKxfwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAG6GdPcA92ORzVJe2jfG8KQqqMHr9YTLu30oM4i7MFEoBAgIAAQwCAAAA6AMAAAAAAAA=");

    // Step 1 - Check if there are no price and limit instructions in the original transaction.

    EXPECT_EQ(TWSolanaTransactionGetComputeUnitPrice(originalTx.get()), nullptr);
    EXPECT_EQ(TWSolanaTransactionGetComputeUnitLimit(originalTx.get()), nullptr);

    // Step 2 - Set price and limit instructions.

    const auto txWithPrice = WRAPS(TWSolanaTransactionSetComputeUnitPrice(originalTx.get(), STRING("1000").get()));
    const auto updatedTx = WRAPS(TWSolanaTransactionSetComputeUnitLimit(txWithPrice.get(), STRING("10000").get()));

    assertStringsEqual(updatedTx, "AX43+Ir2EDqf2zLEvgzFrCZKRjdr3wCdp8CnvYh6N0G/s86IueX9BbiNUl16iLRGvwREDfi2Srb0hmLNBFw1BwABAAIEODI+iWe7g68B9iwCy8bFkJKvsIEj350oSOpcv4gNnv/st+6qmqipl9lwMK6toB9TiL7LrJVfij+pKwr+pUKxfwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwZGb+UhFzL/7K26csOb57yM5bvF9xJrLEObOkAAAAAboZ09wD3Y5HNUl7aN8bwpCqowev1hMu7fSgziLswUSgMDAAUCECcAAAICAAEMAgAAAOgDAAAAAAAAAwAJA+gDAAAAAAAA");

    // Step 3 - Check if price and limit instructions are set successfully.

    assertStringsEqual(WRAPS(TWSolanaTransactionGetComputeUnitPrice(updatedTx.get())), "1000");
    assertStringsEqual(WRAPS(TWSolanaTransactionGetComputeUnitLimit(updatedTx.get())), "10000");

    // Step 4 - Decode transaction into a `RawMessage` Protobuf.

    const std::string updateTxDataB64 {TWStringUTF8Bytes(updatedTx.get()) };
    const auto updatedTxData = Base64::decode(updateTxDataB64);
    const auto updatedTxRef = WRAPD(TWDataCreateWithBytes(updatedTxData.data(), updatedTxData.size()));

    const auto decodeOutputData = WRAPD(TWTransactionDecoderDecode(TWCoinTypeSolana, updatedTxRef.get()));
    Proto::DecodingTransactionOutput decodeOutput;
    decodeOutput.ParseFromArray(TWDataBytes(decodeOutputData.get()), static_cast<int>(TWDataSize(decodeOutputData.get())));
    EXPECT_EQ(decodeOutput.error(), Common::Proto::SigningError::OK);

    // Step 5 - Sign the decoded `RawMessage` transaction.

    Proto::SigningInput input;
    input.set_private_key(privateKey.data(), privateKey.size());
    *input.mutable_raw_message() = decodeOutput.transaction();
    input.set_tx_encoding(Proto::Encoding::Base64);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);

    // Successfully broadcasted tx:
    // https://explorer.solana.com/tx/2ho7wZUXbDNz12xGfsXg2kcNMqkBAQjv7YNXNcVcuCmbC4p9FZe9ELeM2gMjq9MKQPpmE3nBW5pbdgwVCfNLr1h8
    EXPECT_EQ(output.encoded(), "AVUye82Mv+/aWeU2G+B6Nes365mUU2m8iqcGZn/8kFJvw4wY6AgKGG+vJHaknHlCDwE1yi1SIMVUUtNCOm3kHg8BAAIEODI+iWe7g68B9iwCy8bFkJKvsIEj350oSOpcv4gNnv/st+6qmqipl9lwMK6toB9TiL7LrJVfij+pKwr+pUKxfwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwZGb+UhFzL/7K26csOb57yM5bvF9xJrLEObOkAAAAAboZ09wD3Y5HNUl7aN8bwpCqowev1hMu7fSgziLswUSgMDAAUCECcAAAICAAEMAgAAAOgDAAAAAAAAAwAJA+gDAAAAAAAA");
}

} // TW::Solana::tests
