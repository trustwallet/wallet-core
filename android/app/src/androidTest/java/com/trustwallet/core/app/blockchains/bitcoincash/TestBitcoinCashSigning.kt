package com.trustwallet.core.app.blockchains.bitcoincash

import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.BitcoinSigHashType
import wallet.core.jni.CoinType
import wallet.core.jni.proto.Bitcoin
import wallet.core.jni.proto.BitcoinV2
import wallet.core.jni.proto.Common.SigningError

class TestBitcoinCashSigning {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSignV2P2PKH() {
        val privateKey = "7fdafb9db5bc501f2096e7d13d331dc7a75d9594af3d251313ba8b6200f4e384".toHexBytesInByteString()

        val utxo1 = BitcoinV2.Input.newBuilder().apply {
            outPoint = BitcoinV2.OutPoint.newBuilder().apply {
                hash = "e28c2b955293159898e34c6840d99bf4d390e2ee1c6f606939f18ee1e2000d05".toHexBytesInByteString()
                vout = 2
            }.build()
            value = 5151
            sighashType = BitcoinSigHashType.ALL.value() + BitcoinSigHashType.FORK.value()
            receiverAddress = "bitcoincash:qzhlrcrcne07x94h99thved2pgzdtv8ccujjy73xya"
        }

        val out1 = BitcoinV2.Output.newBuilder().apply {
            value = 600
            // Legacy address
            toAddress = "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx"
        }
        val changeOutputExplicit = BitcoinV2.Output.newBuilder().apply {
            value = 4325
            // Legacy address
            toAddress = "qz0q3xmg38sr94rw8wg45vujah7kzma3cskxymnw06"
        }

        val builder = BitcoinV2.TransactionBuilder.newBuilder()
            .setVersion(BitcoinV2.TransactionVersion.V1)
            .addInputs(utxo1)
            .addOutputs(out1)
            .addOutputs(changeOutputExplicit)
            .setInputSelector(BitcoinV2.InputSelector.UseAll)
            .setFixedDustThreshold(546)
            .build()

        val input = BitcoinV2.SigningInput.newBuilder()
            .addPrivateKeys(privateKey)
            .setBuilder(builder)
            .setChainInfo(BitcoinV2.ChainInfo.newBuilder().apply {
                p2PkhPrefix = 0
                p2ShPrefix = 5
                hrp = "bitcoincash"
            })
            .build()

        val legacyInput = Bitcoin.SigningInput.newBuilder()
            .setSigningV2(input)
            // `CoinType` must be set to be handled correctly.
            .setCoinType(CoinType.BITCOINCASH.value())
            .build()

        val outputLegacy = AnySigner.sign(legacyInput, CoinType.BITCOINCASH, Bitcoin.SigningOutput.parser())
        assertEquals(outputLegacy.error, SigningError.OK)
        assert(outputLegacy.hasSigningResultV2())

        val output = outputLegacy.signingResultV2
        assertEquals(output.error, SigningError.OK)
        assertEquals(
            Numeric.toHexString(output.encoded.toByteArray()),
            "0x0100000001e28c2b955293159898e34c6840d99bf4d390e2ee1c6f606939f18ee1e2000d05020000006b483045022100b70d158b43cbcded60e6977e93f9a84966bc0cec6f2dfd1463d1223a90563f0d02207548d081069de570a494d0967ba388ff02641d91cadb060587ead95a98d4e3534121038eab72ec78e639d02758e7860cdec018b49498c307791f785aa3019622f4ea5bffffffff0258020000000000001976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ace5100000000000001976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac00000000"
        )
    }
}
