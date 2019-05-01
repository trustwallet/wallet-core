package com.trustwallet.core.app.blockchains.bravo

import android.util.Log
import com.google.common.base.Utf8
import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.proto.Bravo
import org.json.JSONObject
import wallet.core.jni.*
import java.util.*


class TestBravoSigning {

    init {
        System.loadLibrary("TrustWalletCore")
    }


    // ensure valid input is signed
    @Test
    fun bravoTransactionSigning() {

        val bravoSigner: Bravo.SigningInput.Builder = Bravo.SigningInput.newBuilder()

        bravoSigner.apply {
            chainId= ByteString.copyFrom(ByteArray(32))
            sender = "alice"
            recipient = "bob"
            amount = 90.0
            testnet = false
            memo = "Eva"
            referenceBlockId = ByteString.copyFrom("0000086bf9e7704509aa41311a66fa0a1b479c6b".toHexByteArray())
            referenceBlockTime = 1552464180
            privateKey = ByteString.copyFrom(Hash.sha256("A".toByteArray()))
        }

        val result = BravoSigner.sign(bravoSigner.build())
        assertTrue("Error signing: " + result.error, result.success)
        assertEquals(1, result.objectsCount)

        val signingOutput: Bravo.SigningOutput = result.getObjects(0).unpack(Bravo.SigningOutput::class.java)
        val jsonObj = JSONObject(signingOutput.jsonEncoded)
        assertNotNull(jsonObj)

        val signatures = jsonObj.getJSONArray("signatures")
        assertNotNull("Error parsing JSON result", signatures)

        val signatureValue: String = signatures.get(0) as String;
        assertNotNull("Error parsing JSON result", signatureValue)
        assertEquals("1f05b3dddfcbca5748b02562645fe7c3f01044b78983ce673f84c230b2dc97beed19b2e8462f1e45f2ac7600c2900f9f90510efe0891141ad2c6b1ae33b21bcace",signatureValue)

    }

    // ensure invalid inputs are not signed
    @Test
    fun testFailures() {

        val bravoSigner: Bravo.SigningInput.Builder = Bravo.SigningInput.newBuilder()

        val goodInput = bravoSigner.apply {
            chainId = ByteString.copyFrom(ByteArray(32))
            sender = "alice"
            recipient = "bob"
            amount = 90.0
            testnet = false
            memo = "Eva"
            referenceBlockId = ByteString.copyFrom("0000086bf9e7704509aa41311a66fa0a1b479c6b".toHexByteArray())
            referenceBlockTime = 1552464180
            privateKey = ByteString.copyFrom(Hash.sha256("A".toByteArray()))
        }


        var badinput = goodInput;
        badinput.amount = -90.00;
        var result = BravoSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)


        badinput = goodInput;
        badinput.sender = "A".repeat(17)
        result = BravoSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)

        badinput = goodInput;
        badinput.memo = "A".repeat(2049)
        result = BravoSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)

        badinput = goodInput;
        badinput.referenceBlockId = ByteString.copyFrom("0000086bf9e7704509aa41311a66fa0a1b479c".toHexByteArray())
        result = BravoSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)
    }


}