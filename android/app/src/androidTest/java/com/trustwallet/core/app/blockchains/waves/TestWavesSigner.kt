package com.trustwallet.core.app.blockchains.waves

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.CoinType.WAVES
import wallet.core.java.AnySigner
import wallet.core.jni.proto.Waves
import wallet.core.jni.proto.Waves.SigningOutput

class TestWavesTransactionSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testWavesTransactionSigning() {
        val signingInput = Waves.SigningInput.newBuilder()
        val transferMsg = Waves.TransferMessage.newBuilder().apply {
            amount = 100_000_000
            asset = "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq"
            fee = 100_000
            feeAsset = "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq"
            to = "3PPCZQkvdMJpmx7Zrz1cnYsPe9Bt1XT2Ckx"
            attachment = ByteString.copyFrom("68656c6c6f".toHexByteArray())
        }.build()
        signingInput.apply {
            timestamp = 1559146613
            privateKey = ByteString.copyFrom("68b7a9adb4a655b205f43dac413803785921e22cd7c4d05857b203a62621075f".toHexByteArray())
            transferMessage = transferMsg
        }

        val sign = AnySigner.sign(signingInput.build(), WAVES, SigningOutput.parser())
        val signBytes = sign.signature
        assertEquals(signBytes.toByteArray().toHex(), "0x5d6a77b1fd9b53d9735cd2543ba94215664f2b07d6c7befb081221fcd49f5b6ad6b9ac108582e8d3e74943bdf35fd80d985edf4b4de1fb1c5c427e84d0879f8f")
    }
}
