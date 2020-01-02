// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.cardano

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexBytes
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.CardanoSigner
import wallet.core.jni.proto.Cardano

class TestCardanoSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSignTransaction() {
        val signingInput = Cardano.SigningInput.newBuilder()
        signingInput.amount = 1000000
        signingInput.toAddress = "Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp"
        signingInput.changeAddress = "Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW"
        val outPoint = Cardano.OutPoint.newBuilder()
            .setTxid(ByteString.copyFrom("59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14".toHexBytes()))
            .setIndex(6)
            .build()
        val utxo1 = Cardano.UnspentTransaction.newBuilder()
            .setOutPoint(outPoint)
            .setAmount(15000000)
            .setAddress("Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx")
            .build()
        signingInput.addUtxo(utxo1)
        val privateKey = "b8c31abcc41d931ae881be11da9e4d9242b1f01cae4e69fa29d5ba1f89f9c1549ec844c6b39c70fa6d3a254fe57c1efee1a75eb9755e0b751e96dd288deabc881ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f".toHexBytes()
        signingInput.addPrivateKey(ByteString.copyFrom(privateKey))

        val plan: Cardano.TransactionPlan = CardanoSigner.planTransaction(signingInput.build())
        assertEquals(plan.error, "")
        assertEquals(plan.fee, 167994)

        val sign: Cardano.SigningOutput = CardanoSigner.sign(signingInput.build(), plan)
        assertEquals(sign.encoded.toByteArray().toHex(), "0x82839f8200d818582482582059991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac1406ff9f8282d818582183581c4cf4eba3500c1127ab6ce9e011167bc817d6bc5634f2a1e5f6752c3ea0001a6d87265f1a000f42408282d818582183581ceba562709cf7b5e88fe9d9bdcec4f01382aded9b03d31f16da0efdb0a0001acb24046d1a00d30f46ffa0818200d8185885825840835610e371e632d829eb63bf44ec39d0487c19d4d98b8dce9dd88d88414ce5bb1ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f58403646fec783197bdf3d83858d8303f9d72e09a5df882202716c7737e55148c1c2106c550890c0ab001f7a27cb75fabe827f81132486bec41e17861aba1a069508")
        assertEquals(sign.transactionId, "4374315285f5af17c5fd37d78bcaa94534968e925c06e293e10d2348d9420fac")
        assertEquals(sign.error, "")
    }
}
