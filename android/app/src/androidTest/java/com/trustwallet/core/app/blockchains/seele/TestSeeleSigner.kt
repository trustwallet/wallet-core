package com.trustwallet.core.app.blockchains.seele

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.jni.SeeleSigner
import wallet.core.jni.proto.Seele

class TestSeeleSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSeeleSigning() {
        val signingInput = Seele.SigningInput.newBuilder()
        val transaction = Seele.SignTransaction.newBuilder()

        transaction.apply {
            type = 0
            from = "0xe95d99fec90954eb8f6f899c188aef5caa20d501"
            to = "0x0a57a2714e193b7ac50475ce625f2dcfb483d741"
            amount = 0
            accountNonce = 0
            gasPrice = 1
            gasLimit = 3000000
            timestamp = 0
            payload = (ByteString.copyFrom("".toHexByteArray()))

        }

        signingInput.apply{
            privateKey = ByteString.copyFrom(PrivateKey("0xa417551e1522d88d8b2c1592f9e273f7f8bf68517195418b4b21d40e17cdaa1f".toHexByteArray()).data())
            signTransaction = transaction.build()
        }


        val actualResult = "{\"result\":{\"data\":{\"type\":0,\"from\":\"0xe95d99fec90954eb8f6f899c188aef5caa20d501\",\"to\":\"0x0a57a2714e193b7ac50475ce625f2dcfb483d741\",\"amount\":0,\"accountNonce\":0,\"gasPrice\":1,\"gasLimit\":3000000,\"timestamp\":0,\"payload\":\"\"},\"signature\":{\"sig\":\"H8VP/eTJVUohMa08OL5N3tk+qceOnmGTrN3xWbA3HuBmgus1z7mLJg4Gqgv1a8kB6eSIwtAkXX8yRGgK42WY5gE=\"},\"hash\":\"0xd4acdd7bda7229a66ab6b871a3128629b5569ad6fd783fb3be3d53adab64cf32\"}}"

        val json = SeeleSigner.sign(signingInput.build()).json

        assertEquals(json, actualResult)

    }
}