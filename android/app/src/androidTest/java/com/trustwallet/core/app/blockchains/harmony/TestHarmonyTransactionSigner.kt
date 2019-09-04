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
            privateKey = ByteString.copyFrom(PrivateKey("0xe2f88b4974ae763ca1c2db49218802c2e441293a09eaa9ab681779e05d1b7b94".toHexByteArray()).data())
            toAddress = "0x587c66b4b973a7b231d02ebbc7e7d9f6c5a49ef2"
            chainId = ByteString.copyFrom("0x190".toHexByteArray())
            nonce = ByteString.copyFrom("0x9".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x5208".toHexByteArray())
            fromShardId = ByteString.copyFrom("0x3".toHexByteArray())
            toShardId = ByteString.copyFrom("0x2".toHexByteArray())
            amount = ByteString.copyFrom("0x168d28e3f00280000".toHexByteArray())
        }

        val sign: Harmony.SigningOutput = HarmonySigner.sign(signingInput.build())

        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x343")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0xf4757c9ffad127996f788fb388be3e3e03440f6980b36dc6cee7230e390f0c13")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x42f0ff332bd552e8ad7a1cf6a0af4ebebfb1f8aae413c54d3464b9babba5f28d")
    }
}
