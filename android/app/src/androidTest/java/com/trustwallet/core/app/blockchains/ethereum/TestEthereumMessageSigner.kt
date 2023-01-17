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
    fun testEthereumSignAndVerifyMessageImmutableX() {
        val data = Numeric.hexStringToByteArray("3b0a61f46fdae924007146eacb6db6642de7a5603ad843ec58e10331d89d4b84")
        val privateKey = PrivateKey(data)
        val publicKey = privateKey.getPublicKey(CoinType.ETHEREUM)
        val msg = "Only sign this request if you’ve initiated an action with Immutable X.\n\nFor internal use:\nbd717ba31dca6e0f3f136f7c4197babce5f09a9f25176044c0b3112b1b6017a3"
        val signature = EthereumMessageSigner.signMessageImmutableX(privateKey, msg)
        assertEquals(signature, "32cd5a58f3419fc5db672e3d57f76199b853eda0856d491b38f557b629b0a0814ace689412bf354a1af81126d2749207dffae8ae8845160f33948a6b787e17ee01");
        assertTrue(EthereumMessageSigner.verifyMessage(publicKey, msg, signature))
    }

    @Test
    fun testEthereumSignAndVerifyMessageLegacy() {
        val data = Numeric.hexStringToByteArray("03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d")
        val privateKey = PrivateKey(data)
        val publicKey = privateKey.getPublicKey(CoinType.ETHEREUM)
        val msg = "Foo"
        val signature = EthereumMessageSigner.signMessage(privateKey, msg)
        assertEquals(signature, "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be71101b");
        assertTrue(EthereumMessageSigner.verifyMessage(publicKey, msg, signature))
    }

    @Test
    fun testEthereumSignAndVerifyMessageEip155() {
        val data = Numeric.hexStringToByteArray("03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d")
        val privateKey = PrivateKey(data)
        val publicKey = privateKey.getPublicKey(CoinType.ETHEREUM)
        val msg = "Foo"
        val signature = EthereumMessageSigner.signMessageEip155(privateKey, msg, 0)
        assertEquals(signature, "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be711023");
        assertTrue(EthereumMessageSigner.verifyMessage(publicKey, msg, signature))
    }
}
