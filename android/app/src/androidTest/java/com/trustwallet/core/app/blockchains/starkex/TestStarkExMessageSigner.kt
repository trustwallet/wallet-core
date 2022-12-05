package com.trustwallet.core.app.blockchains.starkex

import com.trustwallet.core.app.utils.Numeric
import org.junit.Assert.assertEquals
import org.junit.Assert.assertTrue
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.jni.PublicKeyType
import wallet.core.jni.StarkExMessageSigner

class TestStarkExMessageSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testStarkExSignAndVerifyMessage() {
        val data = Numeric.hexStringToByteArray("04be51a04e718c202e4dca60c2b72958252024cfc1070c090dd0f170298249de")
        val privateKey = PrivateKey(data)
        val publicKey = privateKey.getPublicKeyByType(PublicKeyType.STARKEX)
        val msg = "463a2240432264a3aa71a5713f2a4e4c1b9e12bbb56083cd56af6d878217cf"
        val signature = StarkExMessageSigner.signMessage(privateKey, msg)
        assertEquals(signature, "04cf5f21333dd189ada3c0f2a51430d733501a9b1d5e07905273c1938cfb261e05b6013d74adde403e8953743a338c8d414bb96bf69d2ca1a91a85ed2700a528");
        assertTrue(StarkExMessageSigner.verifyMessage(publicKey, msg, signature))
    }
}
