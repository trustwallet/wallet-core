package com.trustwallet.core.app.blockchains.ontology

import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.ONTOLOGY
import wallet.core.jni.proto.Ontology
import wallet.core.jni.proto.Ontology.SigningOutput

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
            nonce = 3486522027.toInt()
        }.build()

        val output = AnySigner.sign(input, ONTOLOGY, SigningOutput.parser())
        val result = output.encoded.toByteArray()
        val hex = Numeric.toHexString(result, 0, result.size, false)

        assertEquals(
            "00d1ab1ad0cf0000000000000000000000000000000000000000000000000000000000000000000000004d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f661400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b650000",
            hex
        )
    }

    @Test
    fun testSignOntBalanceOf() {
        val signerBuilder = Ontology.SigningInput.newBuilder()
        val input = signerBuilder.apply {
            contract = "ONT"
            method = "balanceOf"
            queryAddress = "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD"
            nonce = 3959576200.toInt()
        }.build()

        val output = AnySigner.sign(input, ONTOLOGY, SigningOutput.parser())
        val result = output.encoded.toByteArray()
        val hex = Numeric.toHexString(result, 0, result.size, false)

        assertEquals(
            "00d1885602ec0000000000000000000000000000000000000000000000000000000000000000000000004d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f661400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b650000",
            hex
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
            nonce = 2338116610.toInt()
        }.build()

        val output = AnySigner.sign(input, ONTOLOGY, SigningOutput.parser())
        val result = output.encoded.toByteArray()
        val hex = Numeric.toHexString(result, 0, result.size, false)

        assertEquals(
               "00d102d45c8bf401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df67100c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94abac41aaf3ead76586a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b6500024140301766d925382a6ebb2ebeb18d3741954c9370dcf6d9c45b34ce7b18bc42dcdb8300d7215080efb87dd3f35de5f3b6d98aacd6161fbc0845b82d0d8be4b8b6d52321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac414038466b25ac49a22ba8c301328ef049a61711b257987e85e25d63e0444a14e860305a4cd3bb6ea2fe80fd293abb3c592e679c42c546cbf3baa051a07b28b374a6232103d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac",
                hex)
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
            nonce = 2827104669.toInt()
        }.build()

        val output = AnySigner.sign(input, ONTOLOGY, SigningOutput.parser())
        val result = output.encoded.toByteArray()
        val hex = Numeric.toHexString(result, 0, result.size, false)

        assertEquals(
                "00d19d3182a8f401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df67100c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94abac41aaf3ead76586a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b6500024140e27e935b87855efad62bb76b21c7b591f445f867eff86f888ca6ee1870ecd80f8c4754e565b28a85b384612b93b00730143800049b97e83c95844a8eb7d66adc2321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac4140450047b2efb384129a16ec4c707790e9379b978cc7085170071d8d7c5c037d74c4f8742a1de44bc0b3fe7d5cd11fad9edac2a5cdabe2c3b824743cc70df5f276232103d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac",
                hex)
    }

}
