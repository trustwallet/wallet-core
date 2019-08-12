package com.trustwallet.core.app.blockchains.solana

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertArrayEquals
import org.junit.Test
import wallet.core.jni.Base58
import wallet.core.jni.PrivateKey
import wallet.core.jni.SolanaSigner
import wallet.core.jni.proto.Solana

class TestSolanaSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSign() {
        val signingInput = Solana.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr"))
            recipient = "EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd"
            lamports = 42
            recentBlockhash = "11111111111111111111111111111111"
        }

        val output: Solana.SigningOutput = SolanaSigner.sign(signingInput.build())
        val ints: List<Int> = listOf(1,253,161,200,173,136,114,217,79,126,171,82,249,195,141,199,126,16,97,244,137,126,61,226,184,70,158,176,153,34,105,246,250,31,23,62,147,219,178,218,115,138,180,232,149,159,250,80,205,8,124,223,168,137,243,161,184,172,221,98,85,47,124,29,7,1,0,2,3,102,194,245,8,201,197,85,202,204,159,178,109,136,232,141,213,78,33,11,181,168,188,229,104,127,96,215,231,92,76,208,127,198,139,60,137,76,120,43,5,169,194,127,198,198,110,177,77,78,125,49,222,144,134,171,125,33,41,188,176,73,58,250,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,0,1,12,2,0,0,0,42,0,0,0,0,0,0,0)
        val expectedTransactionBytes = ints.map(Int::toByte).toByteArray()
        assertArrayEquals(output.encoded.toByteArray(), expectedTransactionBytes)
    }
}
