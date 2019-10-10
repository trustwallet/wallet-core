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
            privateKey = ByteString.copyFrom(PrivateKey("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x02".toHexByteArray())
            delegateMessage = delegate.build()
            nonce = ByteString.copyFrom("0x2".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x64".toHexByteArray())
        }
        val sign: Harmony.StakingTransactionOutput = HarmonyStakingSigner.sign(signingInput.build())

        val e1 = "0xf87302eb94ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a9c"
        val e2 = "580a02806428a0ada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80a05c28dbc417"
        val e3 = "63dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54"

        assertEquals(Numeric.toHexString(sign.encoded.toByteArray()), e1 + e2 + e3)
        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x28")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0xada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x5c28dbc41763dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54")
    }
}

