package com.trustwallet.core.app.blockchains.solana

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.Base64
import wallet.core.jni.CoinType.SOLANA
import wallet.core.jni.SolanaTransaction
import wallet.core.jni.DataVector
import wallet.core.jni.TransactionDecoder
import wallet.core.jni.proto.Common.SigningError
import wallet.core.jni.proto.Solana
import wallet.core.jni.proto.Solana.DecodingTransactionOutput
import wallet.core.jni.proto.Solana.SigningInput
import wallet.core.jni.proto.Solana.SigningOutput
import wallet.core.jni.proto.Solana.Encoding

class TestSolanaTransaction {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testUpdateBlockhashAndSign() {
        val encodedTx = "AnQTYwZpkm3fs4SdLxnV6gQj3hSLsyacpxDdLMALYWObm722f79IfYFTbZeFK9xHtMumiDOWAM2hHQP4r/GtbARpncaXgOVFv7OgbRLMbuCEJHO1qwcdCbtH72VzyzU8yw9sqqHIAaCUE8xaQTgT6Z5IyZfeyMe2QGJIfOjz65UPAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8Aqe6sdLXiXSDILEtzckCjkjchiSf6zVGpMYiAE5BE2IqHAQUEAgQDAQoMoA8AAAAAAAAG"
        val newBlockhash = "CyPYVsYWrsJNfVpi8aazu7WsrswNFuDd385z6GNoBGUg"

        val myPrivateKey = "7f0932159226ddec9e1a4b0b8fe7cdc135049f9e549a867d722aa720dd64f32e".toHexByteArray()
        val feePayerPrivateKey = "4b9d6f57d28b06cbfa1d4cc710953e62d653caf853415c56ffd9d150acdeb7f7".toHexByteArray()

        val privateKeys = DataVector()
        privateKeys.add(myPrivateKey)
        privateKeys.add(feePayerPrivateKey)

        val outputData = SolanaTransaction.updateBlockhashAndSign(encodedTx, newBlockhash, privateKeys)
        val output = SigningOutput.parseFrom(outputData)

        assertEquals(output.error, SigningError.OK)
        val expectedString = "Ajzc/Tke0CG8Cew5qFa6xZI/7Ya3DN0M8Ige6tKPsGzhg8Bw9DqL18KUrEZZ1F4YqZBo4Rv+FsDT8A7Nss7p4A6BNVZzzGprCJqYQeNg0EVIbmPc6mDitNniHXGeKgPZ6QZbM4FElw9O7IOFTpOBPvQFeqy0vZf/aayncL8EK/UEAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8AqbHiki6ThNH3auuyZPQpJntnN0mA//56nMpK/6HIuu8xAQUEAgQDAQoMoA8AAAAAAAAG"
        assertEquals(output.encoded, expectedString)
    }

    @Test
    fun testDecodeUpdateBlockhashAndSign() {
        // https://explorer.solana.com/tx/3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej?cluster=devnet
        val encodedTx = Base64.decode("AnQTYwZpkm3fs4SdLxnV6gQj3hSLsyacpxDdLMALYWObm722f79IfYFTbZeFK9xHtMumiDOWAM2hHQP4r/GtbARpncaXgOVFv7OgbRLMbuCEJHO1qwcdCbtH72VzyzU8yw9sqqHIAaCUE8xaQTgT6Z5IyZfeyMe2QGJIfOjz65UPAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8Aqe6sdLXiXSDILEtzckCjkjchiSf6zVGpMYiAE5BE2IqHAQUEAgQDAQoMoA8AAAAAAAAG")
        val newBlockhash = "CyPYVsYWrsJNfVpi8aazu7WsrswNFuDd385z6GNoBGUg"

        val senderPrivateKeyData = "7f0932159226ddec9e1a4b0b8fe7cdc135049f9e549a867d722aa720dd64f32e".toHexByteArray()
        val feePayerPrivateKeyData = "4b9d6f57d28b06cbfa1d4cc710953e62d653caf853415c56ffd9d150acdeb7f7".toHexByteArray()

        // Step 1: Decode the transaction.

        val decodedData = TransactionDecoder.decode(SOLANA, encodedTx)
        val decodedOutput = DecodingTransactionOutput.parseFrom(decodedData)

        assertEquals(decodedOutput.error, SigningError.OK)
        assert(decodedOutput.transaction.hasLegacy())

        // Step 2: Update recent blockhash.

        val rawTx = decodedOutput.transaction.toBuilder().apply {
            legacy = decodedOutput.transaction.legacy.toBuilder().setRecentBlockhash(newBlockhash).build()
        }.build()

        // Step 3: Re-sign the updated transaction.

        val signingInput = SigningInput.newBuilder().apply {
            rawMessage = rawTx
            privateKey = ByteString.copyFrom(senderPrivateKeyData)
            feePayerPrivateKey = ByteString.copyFrom(feePayerPrivateKeyData)
            txEncoding = Solana.Encoding.Base64
        }.build()

        val output = AnySigner.sign(signingInput, SOLANA, SigningOutput.parser())
        val expectedString = "Ajzc/Tke0CG8Cew5qFa6xZI/7Ya3DN0M8Ige6tKPsGzhg8Bw9DqL18KUrEZZ1F4YqZBo4Rv+FsDT8A7Nss7p4A6BNVZzzGprCJqYQeNg0EVIbmPc6mDitNniHXGeKgPZ6QZbM4FElw9O7IOFTpOBPvQFeqy0vZf/aayncL8EK/UEAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8AqbHiki6ThNH3auuyZPQpJntnN0mA//56nMpK/6HIuu8xAQUEAgQDAQoMoA8AAAAAAAAG"
        assertEquals(output.encoded, expectedString)
    }

    @Test
    fun testSetPriorityFee() {
        val privateKey = ByteString.copyFrom("baf2b2dbbbad7ca96c1fa199c686f3d8fbd2c7b352f307e37e04f33df6741f18".toHexByteArray())
        val originalTx = "AX43+Ir2EDqf2zLEvgzFrCZKRjdr3wCdp8CnvYh6N0G/s86IueX9BbiNUl16iLRGvwREDfi2Srb0hmLNBFw1BwABAAEDODI+iWe7g68B9iwCy8bFkJKvsIEj350oSOpcv4gNnv/st+6qmqipl9lwMK6toB9TiL7LrJVfij+pKwr+pUKxfwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAG6GdPcA92ORzVJe2jfG8KQqqMHr9YTLu30oM4i7MFEoBAgIAAQwCAAAA6AMAAAAAAAA="

        // Step 1 - Check if there are no price and limit instructions in the original transaction.
        assertEquals(SolanaTransaction.getComputeUnitPrice(originalTx), null)
        assertEquals(SolanaTransaction.getComputeUnitLimit(originalTx), null)

        // Step 2 - Set price and limit instructions.
        val txWithPrice = SolanaTransaction.setComputeUnitPrice(originalTx, "1000")
        val updatedTx = SolanaTransaction.setComputeUnitLimit(txWithPrice, "10000")

        assertEquals(updatedTx, "AX43+Ir2EDqf2zLEvgzFrCZKRjdr3wCdp8CnvYh6N0G/s86IueX9BbiNUl16iLRGvwREDfi2Srb0hmLNBFw1BwABAAIEODI+iWe7g68B9iwCy8bFkJKvsIEj350oSOpcv4gNnv/st+6qmqipl9lwMK6toB9TiL7LrJVfij+pKwr+pUKxfwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwZGb+UhFzL/7K26csOb57yM5bvF9xJrLEObOkAAAAAboZ09wD3Y5HNUl7aN8bwpCqowev1hMu7fSgziLswUSgMDAAUCECcAAAICAAEMAgAAAOgDAAAAAAAAAwAJA+gDAAAAAAAA")

        // Step 3 - Check if price and limit instructions are set successfully.
        assertEquals(SolanaTransaction.getComputeUnitPrice(updatedTx), "1000")
        assertEquals(SolanaTransaction.getComputeUnitLimit(updatedTx), "10000")

        // Step 4 - Decode transaction into a `RawMessage` Protobuf.
        val updatedTxData = Base64.decode(updatedTx)
        val decodedData = TransactionDecoder.decode(SOLANA, updatedTxData)
        val decodedOutput = DecodingTransactionOutput.parseFrom(decodedData)

        assertEquals(decodedOutput.error, SigningError.OK)

        // Step 5 - Sign the decoded `RawMessage` transaction.
        val signingInput = SigningInput.newBuilder()
            .setPrivateKey(privateKey)
            .setRawMessage(decodedOutput.transaction)
            .setTxEncoding(Encoding.Base64)
            .build()
        val output = AnySigner.sign(signingInput, SOLANA, SigningOutput.parser())
        // Successfully broadcasted tx:
        // https://explorer.solana.com/tx/2ho7wZUXbDNz12xGfsXg2kcNMqkBAQjv7YNXNcVcuCmbC4p9FZe9ELeM2gMjq9MKQPpmE3nBW5pbdgwVCfNLr1h8
        val expectedString = "AVUye82Mv+/aWeU2G+B6Nes365mUU2m8iqcGZn/8kFJvw4wY6AgKGG+vJHaknHlCDwE1yi1SIMVUUtNCOm3kHg8BAAIEODI+iWe7g68B9iwCy8bFkJKvsIEj350oSOpcv4gNnv/st+6qmqipl9lwMK6toB9TiL7LrJVfij+pKwr+pUKxfwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwZGb+UhFzL/7K26csOb57yM5bvF9xJrLEObOkAAAAAboZ09wD3Y5HNUl7aN8bwpCqowev1hMu7fSgziLswUSgMDAAUCECcAAAICAAEMAgAAAOgDAAAAAAAAAwAJA+gDAAAAAAAA"
        assertEquals(output.encoded, expectedString)
    }

    @Test
    fun testSetFeePayer() {
        val originalTx = "AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAQABA2uEKrOPvZNBtdUtSFXcg8+kj4O/Z1Ht/hwvnaqq5s6mTXd3KtwUyJFfRs2PBfeQW8xCEZvNr/5J/Tx8ltbn0pwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACo+QRbvXWNKoOfaOL4cSpfYrmn/2TV+dBmct+HsmmwdAQICAAEMAgAAAACcnwYAAAAAAA=="

        // Step 1 - Add fee payer to the transaction.
        val updatedTx = SolanaTransaction.setFeePayer(originalTx, "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ")
        assertEquals(updatedTx, "AgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgAIAAQTLKvCJtWpVdze8Fxjgy/Iyz1sC4U7gqnxmdSM/X2+bV2uEKrOPvZNBtdUtSFXcg8+kj4O/Z1Ht/hwvnaqq5s6mTXd3KtwUyJFfRs2PBfeQW8xCEZvNr/5J/Tx8ltbn0pwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACo+QRbvXWNKoOfaOL4cSpfYrmn/2TV+dBmct+HsmmwdAQMCAQIMAgAAAACcnwYAAAAAAA==")

        // This case originates from a test case in C++. Here, only the most critical function is verified for correctness,
        // while the remaining steps have been omitted.
        // Step 2 - Decode transaction into a `RawMessage` Protobuf.
        // Step 3 - Obtain preimage hash.
        // Step 4 - Compile transaction info.
    }
}