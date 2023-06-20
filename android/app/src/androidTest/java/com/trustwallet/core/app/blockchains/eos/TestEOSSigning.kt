package com.trustwallet.core.app.blockchains.eos

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.*
import org.junit.Test
import org.json.JSONObject
import wallet.core.jni.*
import wallet.core.jni.proto.EOS
import wallet.core.jni.proto.EOS.SigningOutput
import wallet.core.jni.proto.Common.SigningError
import wallet.core.java.AnySigner


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

        val output = AnySigner.sign(signingInput.build(), CoinType.EOS, SigningOutput.parser())
        assertEquals(SigningError.OK, output.error)

        val jsonObj = JSONObject(output.jsonEncoded)
        assertNotNull(jsonObj)

        val signatures = jsonObj.getJSONArray("signatures")
        assertNotNull("Error parsing JSON result", signatures)

        val signatureValue: String = signatures.get(0) as String;
        assertNotNull("Error parsing JSON result", signatureValue)
        assertEquals(
            "SIG_K1_K9RdLC7DEDWjTfR64GU8BtDHcAjzR1ntcT651JMcfHNTpdsvDrUwfyzF1FkvL9fxEi2UCtGJZ9zYoNbJoMF1fbU64cRiJ7",
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
        var result = AnySigner.sign(badinput.build(), CoinType.EOS, SigningOutput.parser())
        assertEquals("Expected error but signing succeeded!", SigningError.Error_internal, result.error)


        badinput = goodInput;
        badinput.asset = getAssetBuilder(300000, 4, "xyz").build()
        result = AnySigner.sign(badinput.build(), CoinType.EOS, SigningOutput.parser())
        assertEquals("Expected error but signing succeeded!", SigningError.Error_internal, result.error)


        badinput = goodInput;
        badinput.recipient = "A".repeat(15)
        result = AnySigner.sign(badinput.build(), CoinType.EOS, SigningOutput.parser())
        assertEquals("Expected error but signing succeeded!", SigningError.Error_internal, result.error)


        badinput = goodInput;
        badinput.referenceBlockId = ByteString.copyFrom("0000086bf9e7704509aa41311a66fa0a1b479c".toHexByteArray())
        result = AnySigner.sign(badinput.build(), CoinType.EOS, SigningOutput.parser())
        assertEquals("Expected error but signing succeeded!", SigningError.Error_internal, result.error)
    }
}
