package com.trustwallet.core.app.blockchains.solana

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.Base58
import wallet.core.java.AnySigner
import wallet.core.jni.Base64
import wallet.core.jni.CoinType
import wallet.core.jni.CoinType.SOLANA
import wallet.core.jni.SolanaTransaction
import wallet.core.jni.DataVector
import wallet.core.jni.TransactionDecoder
import wallet.core.jni.proto.Common.SigningError
import wallet.core.jni.proto.Solana
import wallet.core.jni.proto.Solana.DecodingTransactionOutput
import wallet.core.jni.proto.Solana.SigningInput
import wallet.core.jni.proto.Solana.SigningOutput

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
}