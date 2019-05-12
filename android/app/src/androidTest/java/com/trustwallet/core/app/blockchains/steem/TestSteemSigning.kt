package com.trustwallet.core.app.blockchains.steem

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


class TestSteemSigning {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testValidAddresses() {
        assertTrue(CoinType.STEEM.validate("STM52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa"))
        assertTrue(CoinType.STEEM.validate("TST6EFA9Ge5KQaCS2jGZVf7xHZ6hzcH7uvikf5oR7YnYKemkucxB4"))
    }

    @Test
    fun testInvalidAddresses() {
        var addr = listOf(
            "52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa",
            "TST52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRd",
            "TST52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaaaa",
            "STEEM52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa",
            "TEST52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa"
        )
        addr.forEach {
            assertFalse(CoinType.STEEM.validate(it))
        }
    }

    // ensure valid input is signed
    @Test
    fun bravoTransactionSigning() {

        val bravoSigner: Bravo.SigningInput.Builder = Bravo.SigningInput.newBuilder()

        bravoSigner.apply {
            chainId = ByteString.copyFrom(ByteArray(32))
            sender = "alice"
            recipient = "bob"
            amount = 90.0
            testnet = false
            memo = "Eva"
            referenceBlockId = ByteString.copyFrom("000000c4819e7d5c54c23cca3f438a1f62aa2464".toHexByteArray())
            referenceBlockTime = 1553596224
            privateKey = ByteString.copyFrom(Hash.sha256("A".toByteArray()))
        }

        val result = SteemSigner.sign(bravoSigner.build())
        assertTrue("Error signing: " + result.error, result.success)
        assertEquals(1, result.objectsCount)

        val signingOutput: Bravo.SigningOutput = result.getObjects(0).unpack(Bravo.SigningOutput::class.java)
        val jsonObj = JSONObject(signingOutput.jsonEncoded)
        assertNotNull(jsonObj)

        val signatures = jsonObj.getJSONArray("signatures")
        assertNotNull("Error parsing JSON result", signatures)

        val signatureValue: String = signatures.get(0) as String;
        assertNotNull("Error parsing JSON result", signatureValue)
        assertEquals(
            "20e8d87c007becf88802ab0e074c001f070cd6522e8d9966feb344b3cd761d43286127787d25fa0aa4cb76969f1facb34e76fc460f7feb029c650bef697080e55e",
            signatureValue
        )

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
            referenceBlockId = ByteString.copyFrom("000000c4819e7d5c54c23cca3f438a1f62aa2464".toHexByteArray())
            referenceBlockTime = 1553596224
            privateKey = ByteString.copyFrom(Hash.sha256("A".toByteArray()))
        }


        var badinput = goodInput;
        badinput.amount = -90.00;
        var result = SteemSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)


        badinput = goodInput;
        badinput.sender = "A".repeat(17)
        result = SteemSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)

        badinput = goodInput;
        badinput.memo = "A".repeat(2049)
        result = SteemSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)

        badinput = goodInput;
        badinput.referenceBlockId = ByteString.copyFrom("0000086bf9e7704509aa41311a66fa0a1b479c".toHexByteArray())
        result = SteemSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)
    }


}