package com.trustwallet.core.app.blockchains.bitshares

import android.util.Log
import androidx.annotation.IntegerRes
import com.google.common.base.Utf8
import com.google.gson.Gson
import com.google.gson.GsonBuilder
import com.google.gson.JsonObject
import com.google.gson.reflect.TypeToken
import com.google.protobuf.ByteString
import com.google.protobuf.UInt64Value
import com.trustwallet.core.app.utils.*
import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.proto.Bravo
import org.json.JSONObject
import wallet.core.jni.*
import wallet.core.jni.proto.Bitshares
import wallet.core.jni.proto.EOS
import java.math.BigInteger
import java.util.*


class TestBitShares {

    init {
        System.loadLibrary("TrustWalletCore")
    }


    @Test
    fun testAddresses() {
        assertTrue(CoinType.BITSHARES.validate("BTS52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa"))
        var addr = listOf(
            "52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa",
            "BTS52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRd",
            "BTS52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaaaa",
            "BITS52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa"
        )
        addr.forEach {
            assertFalse(CoinType.STEEM.validate(it))
        }
    }

    fun getAssetBuilder(amt: Long, assetID: Long): Bitshares.Asset.Builder {
        val asset: Bitshares.Asset.Builder = Bitshares.Asset.newBuilder();
        asset.apply {
            amount = amt
            assetId = assetID
        }
        return asset;
    }

    fun getFeeBuilder(amt: Long, assetID: Long): Bitshares.Asset.Builder {
        val asset: Bitshares.Asset.Builder = Bitshares.Asset.newBuilder();
        asset.apply {
            amount = amt
            assetId = assetID
        }
        return asset;
    }

    fun signingInput(): Bitshares.SigningInput.Builder {
        val signingInput: Bitshares.SigningInput.Builder = Bitshares.SigningInput.newBuilder()

        signingInput.apply {
            chainId = ByteString.copyFrom(ByteArray(32))
            senderId = 12
            recipientId = 16
            asset = getAssetBuilder(511, 0).build()
            fee = getFeeBuilder(2, 0).build()
            referenceBlockId = ByteString.copyFrom("0000086bf9e7704509aa41311a66fa0a1b479c6b".toHexByteArray())
            referenceBlockTime = 1552464180
            privateKey = ByteString.copyFrom(Hash.sha256("A".toByteArray()))
        }
        return signingInput;
    }


    // ensure valid input is signed
    @Test
    fun testSigning() {

        val result = BitsharesSigner.sign(signingInput().build())
        assertTrue("Error signing: " + result.error, result.success)
        assertEquals(1, result.objectsCount)

        val signingOutput: Bitshares.SigningOutput = result.getObjects(0).unpack(Bitshares.SigningOutput::class.java)
        val jsonObj = JSONObject(signingOutput.jsonEncoded)
        assertNotNull(jsonObj)

        val signatures = jsonObj.getJSONArray("signatures")
        assertNotNull("Error parsing JSON result", signatures)

        val signatureValue: String = signatures.get(0) as String;
        assertNotNull("Error parsing JSON result", signatureValue)
        assertEquals(
            "1f0ee91e5f9cdd04629d4db71a6d5f0d75c282669bbaf84c184b6c18f04fe75dfb560969a9d8360f31bdd93b90c40dfe5fed601094433962205b4c49e925b51b24",
            signatureValue
        )

    }

    // ensure invalid inputs are not signed
    @Test
    fun testFailures() {
        var badinput = signingInput()
        badinput.asset = getAssetBuilder(0, 0).build();
        var result = BitsharesSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)


        badinput = signingInput()
        badinput.fee = getFeeBuilder(-1, 0).build()
        result = BitsharesSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)


        badinput = signingInput()
        badinput.senderId = badinput.recipientId
        result = BitsharesSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)

        badinput = signingInput()
        badinput.referenceBlockId = ByteString.copyFrom("0000086bf9e7704509aa41311a66fa0a1b479c".toHexByteArray())
        result = BitsharesSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)
    }


    @Test
    fun testMemo() {
        val message = "Hello, world!"
        var inputWithMemo = signingInput() //signingInput
        val senderPublicKey = PrivateKey(Hash.sha256("A".toByteArray())).getPublicKeySecp256k1(true)
        val recipientPublicKey = PrivateKey(Hash.sha256("B".toByteArray())).getPublicKeySecp256k1(true)

        inputWithMemo.memo = message
        inputWithMemo.recipientPublicKey = ByteString.copyFrom(recipientPublicKey.data())

        val result = BitsharesSigner.sign(inputWithMemo.build());
        assertTrue("Error signing: " + result.error, result.success)


        assertEquals(1, result.objectsCount)

        val signingOutput: Bitshares.SigningOutput = result.getObjects(0).unpack(Bitshares.SigningOutput::class.java)

        val memoJSON = Gson().fromJson(signingOutput.jsonEncoded, JsonObject::class.java)
                                .getAsJsonArray("operations")
                                    .get(0).getAsJsonArray().get(1).getAsJsonObject().get("memo")
            .getAsJsonObject()
        val nonce = memoJSON.get("nonce").toString().toULong()
        val encryptedData = memoJSON.get("message").asString.toHexByteArray()

        assertEquals(memoJSON.get("to").asString, BitsharesAddress(recipientPublicKey).description())
        assertEquals(memoJSON.get("from").asString, BitsharesAddress(senderPublicKey).description())

        if (nonce > Long.MAX_VALUE.toULong()) {
            Log.e("Bitshares", "nonce bigger than long max.")
        }

        if (encryptedData == null || nonce == null) {
            fail("Failed to extract encrypted message")
            return
        }

        val sharedSecretHex =
            "05c160e88379eabeaac0ac98afd22486a1e1ec7982cebad6073d68647f8cdea8e61cb714f314a2ad2d9d6eb74cde6f1e7150823b361032c8b598dcd68957f474"
        val keyPlusIV = Hash.sha512(("" + nonce + sharedSecretHex).toByteArray())
        val decrypted = AES.cbcdecrypt(keyPlusIV.copyOfRange(0, 32), encryptedData, keyPlusIV.copyOfRange(32, 48))
        assertNotNull(decrypted)
        val messageData = message.toByteArray()

        assertEquals(
            Numeric.toHexString(messageData),
            Numeric.toHexString(decrypted.copyOfRange(4, messageData.size + 4))
        )
    }
}