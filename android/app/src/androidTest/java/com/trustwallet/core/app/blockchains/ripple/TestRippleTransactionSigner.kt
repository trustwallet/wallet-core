package com.trustwallet.core.app.blockchains.ripple

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.XRP
import wallet.core.jni.proto.Ripple
import wallet.core.jni.proto.Ripple.SigningOutput

class TestRippleTransactionSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testRippleTransactionSigning() {
        val operation = Ripple.OperationPayment.newBuilder()
        operation.apply {
            amount = 10
            destination = "rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF"
        }
        val signingInput = Ripple.SigningInput.newBuilder()
        signingInput.apply {
            account = "rfxdLwsZnoespnTDDb1Xhvbc8EFNdztaoq"
            fee = 10
            sequence = 32268248
            lastLedgerSequence = 32268269
            privateKey = ByteString.copyFrom(PrivateKey("a5576c0f63da10e584568c8d134569ff44017b0a249eb70657127ae04f38cc77".toHexByteArray()).data())
            opPayment = operation.build()
        }

        val sign = AnySigner.sign(signingInput.build(), XRP, SigningOutput.parser())
        val signBytes = sign.encoded.toByteArray()

        assertEquals(signBytes.toHex(), "0x12000022000000002401ec5fd8201b01ec5fed61400000000000000a68400000000000000a732103d13e1152965a51a4a9fd9a8b4ea3dd82a4eba6b25fcad5f460a2342bb650333f74463044022037d32835c9394f39b2cfd4eaf5b0a80e0db397ace06630fa2b099ff73e425dbc02205288f780330b7a88a1980fa83c647b5908502ad7de9a44500c08f0750b0d9e8481144c55f5a78067206507580be7bb2686c8460adff983148132e4e20aecf29090ac428a9c43f230a829220d")
    }

}
