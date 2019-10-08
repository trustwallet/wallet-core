package com.trustwallet.core.app.blockchains.harmony

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.HarmonyStakingSigner
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Harmony

class TestHarmonyStakingDelegateSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testHarmonyStakingTransactionDelegateSigning() {
        val signingInput = Harmony.StakingTransactionInput.newBuilder()
        val delegate = Harmony.DirectiveDelegate.newBuilder()
        delegate.apply {
            delegatorAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            validatorAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            amount = ByteString.copyFrom("0xa".toHexByteArray())
        }
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0xb578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x02".toHexByteArray())
            delegateMessage = delegate.build()
            nonce = ByteString.copyFrom("0xa".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x64".toHexByteArray())
        }
        val sign: Harmony.StakingTransactionOutput = HarmonyStakingSigner.sign(signingInput.build())

        val e1 = "0xf87302eb94ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a9c"
        val e2 = "580a02806428a0ada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80a05c28dbc417"
        val e3 = "63dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54"

        // assertEquals(Numeric.toHexString(sign.encoded.toByteArray()), e1 + e2 + e3)
        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x28")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0x2e43463bf0bfd1563d3702f301f92c15d167892e337d9831d4c2d3ba1507efbc")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x14e5f9d9618699316c69e720af3b373bc9d3c3d97962decd13dc3b48d2a4ac75")
    }
}
