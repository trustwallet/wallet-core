package com.trustwallet.core.app.blockchains.ontology

import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.OntologySigner
import wallet.core.jni.proto.Ontology

class TestOntologySigning {
    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSignOngBalanceOf() {
        val signerBuilder = Ontology.SigningInput.newBuilder()
        val input = signerBuilder.apply {
            contract = "ONG"
            method = "balanceOf"
            queryAddress = "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD"
        }.build()

        val result = OntologySigner.sign(input).encoded.toByteArray()
        val hex = Numeric.toHexString(result, 0, result.size, false)

        assertEquals(
            86,
            hex.indexOf("1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f661400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b65")
        )
    }

    @Test
    fun testSignOntBalanceOf() {
        val signerBuilder = Ontology.SigningInput.newBuilder()
        val input = signerBuilder.apply {
            contract = "ONT"
            method = "balanceOf"
            queryAddress = "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD"
        }.build()

        val result = OntologySigner.sign(input).encoded.toByteArray()
        val hex = Numeric.toHexString(result, 0, result.size, false)

        assertEquals(0, hex.indexOf("00d1"))
        assertEquals(
            86,
            hex.indexOf("1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f66140000000000000000000000000000000000000001")
        )
    }

    @Test
    fun testSignOntTransfer() {
        val signerBuilder = Ontology.SigningInput.newBuilder()
        val input = signerBuilder.apply {
            contract = "ONT"
            method = "transfer"
            ownerPrivateKey = "4646464646464646464646464646464646464646464646464646464646464646".toHexBytesInByteString()
            toAddress = "Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn"
            amount = 1
            gasPrice = 500
            gasLimit = 20000
            payerPrivateKey = "4646464646464646464646464646464646464646464646464646464646464652".toHexBytesInByteString()
        }.build()

        val result = OntologySigner.sign(input).encoded.toByteArray()
        val hex = Numeric.toHexString(result, 0, result.size, false)

        val verifyPosition1 = hex.indexOf("00d1")
        val verifyPosition2 = hex.indexOf("401000000000000204e000000000000")
        val verifyPosition3 = hex.indexOf("00c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94abac41aaf3ead76586a7cc8516a7cc86c51c1087472616e73666572")
        val verifyPosition4 = hex.indexOf("1400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b")
        val verifyPosition5 = hex.indexOf("031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486")
        val verifyPosition6 = hex.indexOf("03d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1ee")


        assertEquals(724, hex.length)
        assertEquals(0, verifyPosition1)
        assertEquals(13, verifyPosition2)
        assertEquals(86, verifyPosition3)
        assertEquals(220, verifyPosition4)
        assertEquals(452, verifyPosition5)
        assertEquals(656, verifyPosition6)
    }

    @Test
    fun testSignOngTransfer() {
        val signerBuilder = Ontology.SigningInput.newBuilder()
        val input = signerBuilder.apply {
            contract = "ONG"
            method = "transfer"
            ownerPrivateKey = "4646464646464646464646464646464646464646464646464646464646464646".toHexBytesInByteString()
            toAddress = "Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn"
            amount = 1
            gasPrice = 500
            gasLimit = 20000
            payerPrivateKey = "4646464646464646464646464646464646464646464646464646464646464652".toHexBytesInByteString()
        }.build()

        val result = OntologySigner.sign(input).encoded.toByteArray()
        val hex = Numeric.toHexString(result, 0, result.size, false)

        val verifyPosition1 = hex.indexOf("00d1")
        val verifyPosition2 = hex.indexOf("401000000000000204e000000000000")
        val verifyPosition3 = hex.indexOf("00c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94abac41aaf3ead76586a7cc8516a7cc86c51c1087472616e73666572")
        val verifyPosition4 = hex.indexOf("1400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b")
        val verifyPosition5 = hex.indexOf("031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486")
        val verifyPosition6 = hex.indexOf("03d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1ee")


        assertEquals(724, hex.length)
        assertEquals(0, verifyPosition1)
        assertEquals(13, verifyPosition2)
        assertEquals(86, verifyPosition3)
        assertEquals(220, verifyPosition4)
        assertEquals(452, verifyPosition5)
        assertEquals(656, verifyPosition6)
    }

}