package com.trustwallet.core.app.utils

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import com.trustwallet.core.app.utils.Numeric
import wallet.core.jni.*

class TestWebAuthn {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testGetPublicKey() {
        val attestationObject = Numeric.hexStringToByteArray("0xa363666d74646e6f6e656761747453746d74a068617574684461746158a4f95bc73828ee210f9fd3bbe72d97908013b0a3759e9aea3d0ae318766cd2e1ad4500000000adce000235bcc60a648b0b25f1f055030020c720eb493e167ce93183dd91f5661e1004ed8cc1be23d3340d92381da5c0c80ca5010203262001215820a620a8cfc88fd062b11eab31663e56cad95278bef612959be214d98779f645b82258204e7b905b42917570148b0432f99ba21f2e7eebe018cbf837247e38150a89f771")
        val result = WebAuthn.getPublicKey(attestationObject).data()
        assertEquals(Numeric.toHexString(result), "0x04a620a8cfc88fd062b11eab31663e56cad95278bef612959be214d98779f645b84e7b905b42917570148b0432f99ba21f2e7eebe018cbf837247e38150a89f771")
    }

    @Test
    fun testGetRSValues() {
        val signature = Numeric.hexStringToByteArray("0x30440220766589b461a838748708cdf88444b21b1fa52b57d70671b4f9bf60ad14b372ec022020cc439c9c20661bfa39bbea24a900ec1484b2395eb065ead8ef4e273144a57d")
        val result = WebAuthn.getRSValues(signature)
        assertEquals(Numeric.toHexString(result), "0x766589b461a838748708cdf88444b21b1fa52b57d70671b4f9bf60ad14b372ec20cc439c9c20661bfa39bbea24a900ec1484b2395eb065ead8ef4e273144a57d")
    }

    @Test
    fun testReconstructOriginalMessage() {
        val authenticatorData = Numeric.hexStringToByteArray("0x1a70842af8c1feb7133b81e6a160a6a2be45ee057f0eb6d3f7f5126daa202e071d00000000")
        val clientDataJSON = Numeric.hexStringToByteArray("0x7b2274797065223a22776562617574686e2e676574222c226368616c6c656e6765223a224e5549794f5545774d6b45744e554535517930304d6b5a424c546847516a4174517a52474f4441794d3045304f546b30222c226f726967696e223a2268747470733a2f2f747275737477616c6c65742e636f6d227d")
        val result = WebAuthn.reconstructOriginalMessage(authenticatorData, clientDataJSON)
        assertEquals(Numeric.toHexString(result), "0x3254cdbd677e6e31e75d2135bad0cf56440d7c6b108c141a3509d76ce45c6731")
    }
}