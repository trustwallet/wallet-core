package com.trustwallet.core.app.blockchains.tron

import com.trustwallet.core.app.utils.Numeric
import org.junit.Assert.assertEquals
import org.junit.Assert.assertTrue
import org.junit.Test
import wallet.core.jni.CoinType
import wallet.core.jni.TronMessageSigner
import wallet.core.jni.PrivateKey

class TestTronMessageSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testMessageSignerSignAndVerify() {
        val data = Numeric.hexStringToByteArray("75065f100e38d3f3b4c5c4235834ba8216de62272a4f03532c44b31a5734360a")
        val privateKey = PrivateKey(data)
        val msg = "Hello World"
        val signature = TronMessageSigner.signMessage(privateKey, msg)
        assertEquals("9bb6d11ec8a6a3fb686a8f55b123e7ec4e9746a26157f6f9e854dd72f5683b450397a7b0a9653865658de8f9243f877539882891bad30c7286c3bf5622b900471b", signature)
        val pubKey = privateKey.getPublicKey(CoinType.TRON)
        assertTrue(TronMessageSigner.verifyMessage(pubKey, msg, signature))
    }
}
