package com.trustwallet.core.app.blockchains.harmony

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.jni.HarmonySigner
import wallet.core.jni.proto.Harmony
import com.trustwallet.core.app.utils.Numeric

class TestHarmonyTransactionSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testHarmonyTransactionSigning() {
        val signingInput = Harmony.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0xb578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e".toHexByteArray()).data())
            toAddress = "one1d2rngmem4x2c6zxsjjz29dlah0jzkr0k2n88wc"
            chainId = ByteString.copyFrom("0x02".toHexByteArray())
            nonce = ByteString.copyFrom("0x9".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x5208".toHexByteArray())
            fromShardId = ByteString.copyFrom("0x1".toHexByteArray())
            toShardId = ByteString.copyFrom("0x0".toHexByteArray())
            amount = ByteString.copyFrom("0x4c53ecdc18a60000".toHexByteArray())
        }
        val sign: Harmony.SigningOutput = HarmonySigner.sign(signingInput.build())
        val e1 = "0xf86909808252080180946a87346f3ba9958d08d09484a2b7fdbbe42b0df6884c53ecdc18a600008028a0325aed6caa01a5235b"
        val e2 = "7a508c8ab67f0c43946b05a1ea6a3e0628de4033fe372da06c19085d3376c30f6dc47cec795991cd37d6d0ebddfa633b0a8f494bc19cd01b"

        assertEquals(Numeric.toHexString(sign.encoded.toByteArray()), e1 + e2)
        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x28")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0x325aed6caa01a5235b7a508c8ab67f0c43946b05a1ea6a3e0628de4033fe372d")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x6c19085d3376c30f6dc47cec795991cd37d6d0ebddfa633b0a8f494bc19cd01b")
    }
}
