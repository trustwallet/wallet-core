package com.trustwallet.core.app.blockchains.eos

import android.util.Log
import com.google.common.base.Utf8
import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.proto.Bravo
import org.json.JSONObject
import wallet.core.jni.*
import wallet.core.jni.proto.EOS
import java.util.*


class TestEOSSigning {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testValidAddresses() {
        assertTrue(CoinType.EOS.validate("EOS6Vm7RWMS1KKAM9kDXgggpu4sJkFMEpARhmsWA84tk4P22m29AV"))
        assertTrue(CoinType.EOS.validate("PUB_R1_6pQRUVU5vdneRnmjSiZPsvu3zBqcptvg6iK2Vz4vKo4ugnzow3"))
    }

    @Test
    fun testInvalidAddresses() {
        var addr = listOf(
            "abc",
            "65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjTF",
            "EOS65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjT",
            "PUB_5hieQEFWh68h6bjaYAY25Ptd2bmqLCaFsunaneh9gZsmSgUBUe",
            "PUB_K1_5hieQEFWh68h6bjaYAY25Ptd2bmqLCaFsunaneh9gZsmSgUBUe",
            "PUB_K1_65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjTF"
        )
        addr.forEach {
            assertFalse(CoinType.EOS.validate(it))
        }
    }

    fun getAssetBuilder(amt: Long, decimal: Int, sym: String): EOS.Asset.Builder {
        var asset: EOS.Asset.Builder = EOS.Asset.newBuilder();
        asset.apply {
            amount = amt
            decimals = decimal
            symbol = sym
        }
        return asset;
    }


    // ensure valid input is signed
    @Test
    fun eosTransactionSigning() {
        val signingInput: EOS.SigningInput.Builder = EOS.SigningInput.newBuilder()

        signingInput.apply {
            chainId =
                ByteString.copyFrom("cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f".toHexByteArray())
            referenceBlockId =
                ByteString.copyFrom("000067d6f6a7e7799a1f3d487439a679f8cf95f1c986f35c0d2fa320f51a7144".toHexByteArray())
            referenceBlockTime = 1554209118
            currency = "token"
            sender = "token"
            recipient = "eosio"
            memo = "my second transfer"
            asset = getAssetBuilder(300000, 4, "TKN").build()
            privateKey = ByteString.copyFrom(Hash.sha256("A".toByteArray()))
            privateKeyType = EOS.KeyType.MODERNK1
        }

        val result = EOSSigner.sign(signingInput.build())
        assertTrue("Error signing: " + result.error, result.success)
        assertEquals(1, result.objectsCount)

        val signingOutput: EOS.SigningOutput = result.getObjects(0).unpack(EOS.SigningOutput::class.java)
        val jsonObj = JSONObject(signingOutput.jsonEncoded)
        assertNotNull(jsonObj)

        val signatures = jsonObj.getJSONArray("signatures")
        assertNotNull("Error parsing JSON result", signatures)

        val signatureValue: String = signatures.get(0) as String;
        assertNotNull("Error parsing JSON result", signatureValue)
        assertEquals(
            "SIG_K1_KfCdjsrTnx5cBpbA5cUdHZAsRYsnC9uKzuS1shFeqfMCfdZwX4PBm9pfHwGRT6ffz3eavhtkyNci5GoFozQAx8P8PBnDmj",
            signatureValue
        )

    }

    // ensure invalid inputs are not signed
    @Test
    fun testFailures() {
        val signingInput: EOS.SigningInput.Builder = EOS.SigningInput.newBuilder()
        val goodInput = signingInput.apply {
            chainId =ByteString.copyFrom("cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f".toHexByteArray())
            referenceBlockId =ByteString.copyFrom("000067d6f6a7e7799a1f3d487439a679f8cf95f1c986f35c0d2fa320f51a7144".toHexByteArray())
            referenceBlockTime = 1554209118
            currency = "token"
            sender = "token"
            recipient = "eosio"
            memo = "my second transfer"
            asset = getAssetBuilder(300000, 4, "TKN").build()
            privateKey = ByteString.copyFrom(Hash.sha256("A".toByteArray()))
            privateKeyType = EOS.KeyType.MODERNK1
        }


        var badinput = goodInput;
        badinput.asset = getAssetBuilder(300000, 19, "TKN").build();
        var result = EOSSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)


        badinput = goodInput;
        badinput.asset = getAssetBuilder(300000, 4, "xyz").build()
        result = EOSSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)


        badinput = goodInput;
        badinput.recipient = "A".repeat(15)
        result = EOSSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)



        badinput = goodInput;
        badinput.referenceBlockId = ByteString.copyFrom("0000086bf9e7704509aa41311a66fa0a1b479c".toHexByteArray())
        result = EOSSigner.sign(badinput.build())
        assertFalse("Expected error but signing suceeded!", result.success)
    }


}
