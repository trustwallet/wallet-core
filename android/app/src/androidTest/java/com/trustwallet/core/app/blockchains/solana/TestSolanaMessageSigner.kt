package com.trustwallet.core.app.blockchains.solana

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.Base58
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.SOLANA
import wallet.core.jni.MessageSigner
import wallet.core.jni.proto.Common.SigningError
import wallet.core.jni.proto.Solana

class TestSolanaMessageSigner {
    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testMessageSign() {
        val signingInput = Solana.MessageSigningInput.newBuilder().apply {
            privateKey = ByteString.copyFrom("44f480ca27711895586074a14c552e58cc52e66a58edb6c58cf9b9b7295d4a2d".toHexByteArray())
            message = "Hello world"
        }.build()

        val outputData = MessageSigner.sign(SOLANA, signingInput.toByteArray())
        val output = Solana.MessageSigningOutput.parseFrom(outputData)

        assertEquals(output.error, SigningError.OK)
        assertEquals(output.signature, "2iBZ6zrQRKHcbD8NWmm552gU5vGvh1dk3XV4jxnyEdRKm8up8AeQk1GFr9pJokSmchw7i9gMtNyFBdDt8tBxM1cG")
    }

    @Test
    fun testMessageVerify() {
        val verifyingInput = Solana.MessageVerifyingInput.newBuilder().apply {
            publicKey = ByteString.copyFrom("ee6d61a89fc8f9909585a996bb0d2b2ac69ae23b5acf39a19f32631239ba06f9".toHexByteArray())
            signature = "2iBZ6zrQRKHcbD8NWmm552gU5vGvh1dk3XV4jxnyEdRKm8up8AeQk1GFr9pJokSmchw7i9gMtNyFBdDt8tBxM1cG"
            message = "Hello world"
        }.build()

        assert(MessageSigner.verify(SOLANA, verifyingInput.toByteArray()))
    }
}