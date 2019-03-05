package com.trustwallet.core.app.blockchains.ripple

import android.util.Log
import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.jni.RippleSigner
import wallet.core.jni.proto.Ripple

class TestRippleTransactionSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testRippleTransactionSigning() {
        val signingInput = Ripple.SigningInput.newBuilder()
        signingInput.apply {
            account = "rDpysuumkweqeC7XdNgYNtzL5GxbdsmrtF"
            amount = 29_000_000
            destination = "rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF"
            fee = 200_000
            sequence = 1
            privateKey = ByteString.copyFrom(PrivateKey("ba005cd605d8a02e3d5dfd04234cef3a3ee4f76bfbad2722d1fb5af8e12e6764".toHexByteArray()).data())
        }

        val sign: Ripple.SigningOutput = RippleSigner.sign(signingInput.build())
        val signBytes = sign.encoded.toByteArray()

        assertEquals(signBytes.toHex(), "0x12000022800000002400000001614000000001ba8140684000000000030d407321026cc34b92cefb3a4537b3edb0b6044c04af27c01583c577823ecc69a9a21119b6744630440220067f20b3eebfc7107dd0bcc72337a236ac3be042c0469f2341d76694a17d4bb9022048393d7ee7dcb729783b33f5038939ddce1bb8337e66d752974626854556bbb681148400b6b6d08d5d495653d73eda6804c249a5148883148132e4e20aecf29090ac428a9c43f230a829220d")
    }

}