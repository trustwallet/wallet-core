package com.trustwallet.core.app.blockchains.stellar

import android.util.Log
import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.jni.StellarSigner
import wallet.core.jni.proto.Stellar

class TestStellarTransactionSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testStellarTransactionSigning() {
        val signingInput = Stellar.SigningInput.newBuilder()
        signingInput.apply {
            account = "GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI"
            amount = 10_000_000
            destination = "GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52"
            fee = 1000
            sequence = 2
            privateKey = ByteString.copyFrom(PrivateKey("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722".toHexByteArray()).data())
        }

        val sign: Stellar.SigningOutput = StellarSigner.sign(signingInput.build())
        val signBytes = sign.signature

        assertEquals(signBytes, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAAAAAABAAAAAAAAAAEAAAAAxYC2MXoOs5v3/NT6PBn9q0uJu6u/YQle5FBa9uzteq4AAAAAAAAAAACYloAAAAAAAAAAARnfXKIAAABAocQZwTnVvGMQlpdGacWvgenxN5ku8YB8yhEGrDfEV48yDqcj6QaePAitDj/N2gxfYD9Q2pJ+ZpkQMsZZG4ACAg==")
    }

}