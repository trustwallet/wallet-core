package com.trustwallet.core.app.blockchains.bravo

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.BravoSigner
import wallet.core.jni.Hash
import wallet.core.jni.proto.Bravo
import org.json.JSONObject


class TestBravoSigning {

    init {
        System.loadLibrary("TrustWalletCore")
    }


    // ensure valid input is signed
    @Test
    fun bravoTransactionSigning() {

        val bravoSigner: Bravo.SigningInput.Builder = Bravo.SigningInput.newBuilder()

        bravoSigner.apply {
            chainId = ByteString.copyFrom("00000000000000000000000000000000".toHexByteArray())
            sender = "alice"
            recipient = "bob"
            amount = 90.0
            testnet = false
            memo = "Eva"
            referenceBlockId = ByteString.copyFrom("0000086bf9e7704509aa41311a66fa0a1b479c6b".toHexByteArray())
            referenceBlockTime = 1552464180
            privateKey = ByteString.copyFrom(Hash.sha256("A".toHexByteArray()))
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
        assertEquals("1f3176fdb81ebe2dd7584607df148898ed90a000c6559611a6c29f05af586f365804a862e3a2bbba64f19160eb38e402c12eb6eeb81d880606ef86bb8ccfa59b76",signatureValue)

    }

    // ensure invalid inputs are not signed
    @Test
    fun testFailures() {

        val bravoSigner: Bravo.SigningInput.Builder = Bravo.SigningInput.newBuilder()

        val goodInput = bravoSigner.apply {
            chainId = ByteString.copyFrom("00000000000000000000000000000000".toHexByteArray())
            sender = "alice"
            recipient = "bob"
            amount = 90.0
            testnet = false
            memo = "Eva"
            referenceBlockId = ByteString.copyFrom("0000086bf9e7704509aa41311a66fa0a1b479c6b".toHexByteArray())
            referenceBlockTime = 1552464180
            privateKey = ByteString.copyFrom(Hash.sha256("A".toHexByteArray()))
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