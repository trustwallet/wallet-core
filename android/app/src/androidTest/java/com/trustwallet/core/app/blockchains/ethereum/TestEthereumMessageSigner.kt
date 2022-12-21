package com.trustwallet.core.app.blockchains.ethereum

import com.trustwallet.core.app.utils.Numeric
import org.junit.Assert.assertEquals
import org.junit.Assert.assertTrue
import org.junit.Test
import wallet.core.jni.CoinType
import wallet.core.jni.EthereumMessageSigner
import wallet.core.jni.PrivateKey

class TestEthereumMessageSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testEthereumSignAndVerifyMessage() {
        val data = Numeric.hexStringToByteArray("3b0a61f46fdae924007146eacb6db6642de7a5603ad843ec58e10331d89d4b84")
        val privateKey = PrivateKey(data)
        val publicKey = privateKey.getPublicKey(CoinType.ETHEREUM)
        val msg = "Only sign this request if you’ve initiated an action with Immutable X.\n\nFor internal use:\nbd717ba31dca6e0f3f136f7c4197babce5f09a9f25176044c0b3112b1b6017a3"
        val signature = EthereumMessageSigner.signMessage(privateKey, msg)
        assertEquals(signature, "32cd5a58f3419fc5db672e3d57f76199b853eda0856d491b38f557b629b0a0814ace689412bf354a1af81126d2749207dffae8ae8845160f33948a6b787e17ee01");
        assertTrue(EthereumMessageSigner.verifyMessage(publicKey, msg, signature))
    }
}
