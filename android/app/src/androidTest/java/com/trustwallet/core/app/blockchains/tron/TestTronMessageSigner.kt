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
        assertEquals("bc0753c070cc55693097df11bc11e1a7c4bd5e1a40b9dc94c75568e59bcc9d6b50a7873ef25b469e494490a54de37327b4bc7fc825c81a377b555e34fb7261ba1c", signature)
        val pubKey = privateKey.getPublicKey(CoinType.TRON)
        assertTrue(TronMessageSigner.verifyMessage(pubKey, msg, signature))
    }
}
