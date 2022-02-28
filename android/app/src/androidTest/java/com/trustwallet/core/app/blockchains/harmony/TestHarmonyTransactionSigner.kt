package com.trustwallet.core.app.blockchains.harmony

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.java.AnySigner
import wallet.core.jni.proto.Harmony
import wallet.core.jni.proto.Harmony.SigningOutput
import com.trustwallet.core.app.utils.Numeric
import wallet.core.jni.CoinType.HARMONY

class TestHarmonyTransactionSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testHarmonyTransactionSigning() {
        val transaction = Harmony.TransactionMessage.newBuilder()
        transaction.apply {
            nonce = ByteString.copyFrom("0x9".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x5208".toHexByteArray())
            toAddress = "one1d2rngmem4x2c6zxsjjz29dlah0jzkr0k2n88wc"
            fromShardId = ByteString.copyFrom("0x1".toHexByteArray())
            toShardId = ByteString.copyFrom("0x0".toHexByteArray())
            amount = ByteString.copyFrom("0x4c53ecdc18a60000".toHexByteArray())
        }
        val signingInput = Harmony.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0xb578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e".toHexByteArray()).data())
            chainId = ByteString.copyFrom("0x02".toHexByteArray())
            transactionMessage = transaction.build()
        }
        val sign = AnySigner.sign(signingInput.build(), HARMONY, SigningOutput.parser())
        val expected = "0xf86909808252080180946a87346f3ba9958d08d09484a2b7fdbbe42b0df6884c53ecdc18a600008028a0325aed6caa01a5235b7a508c8ab67f0c43946b05a1ea6a3e0628de4033fe372da06c19085d3376c30f6dc47cec795991cd37d6d0ebddfa633b0a8f494bc19cd01b"

        assertEquals(Numeric.toHexString(sign.encoded.toByteArray()), expected)
        assertEquals(Numeric.toHexString(sign.v.toByteArray()), "0x28")
        assertEquals(Numeric.toHexString(sign.r.toByteArray()), "0x325aed6caa01a5235b7a508c8ab67f0c43946b05a1ea6a3e0628de4033fe372d")
        assertEquals(Numeric.toHexString(sign.s.toByteArray()), "0x6c19085d3376c30f6dc47cec795991cd37d6d0ebddfa633b0a8f494bc19cd01b")
    }

    @Test
    fun testSignJSON() {
        val json = """
            {"chainId":"Ag==","transactionMessage":{"nonce":"AQ==","gasPrice":"AA==","gasLimit":"Ugg=","toAddress":"one129r9pj3sk0re76f7zs3qz92rggmdgjhtwge62k","amount":"Br/I2l7oIgAA","fromShardId":"AQ==","toShardId":"AA=="}}
        """
        val key = "4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48".toHexByteArray()
        val result = AnySigner.signJSON(json, key, HARMONY.value())

        assertEquals("f86a0180825208018094514650ca30b3c79f693e14220115434236d44aeb8906bfc8da5ee82200008028a084cc200aab11f5e1b2f7ece0d56ec67385ac50cefb6e3dc2a2f3e036ed575a5ca0643f18005b790cac8d8e7dc90e6147df0b83874b52db198864694ea28a79e6fc", result)
    }
}
