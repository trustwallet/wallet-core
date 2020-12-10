package com.trustwallet.core.app.blockchains.aion

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.AnyAddress
import wallet.core.jni.CoinType
import wallet.core.jni.CoinType.AION
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Aion
import wallet.core.jni.proto.Aion.SigningOutput

class TestAionAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressFromPublicKey() {
        val privateKey = PrivateKey("db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9".toHexByteArray())
        val publicKey = privateKey.getPublicKeyEd25519()
        val address = AnyAddress(publicKey, CoinType.AION)

        assertEquals(address.description(), "0xa0d2312facea71b740679c926d040c9056a65a4bfa2ddd18ec160064f82909e7")
    }

    @Test
    fun testAddressFromString() {
        val addressString = "0xa0d2312facea71b740679c926d040c9056a65a4bfa2ddd18ec160064f82909e7"
        val address = AnyAddress(addressString, CoinType.AION)
        assertEquals(address.description(), addressString)
    }

    @Test
    fun aionTransactionTransactionSign() {
        val signingInput = Aion.SigningInput.newBuilder()
            .setNonce("09".toHexBytesInByteString())
            .setGasPrice("04a817c800".toHexBytesInByteString())
            .setGasLimit("5208".toHexBytesInByteString())
            .setToAddress("0xa082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e")
            .setTimestamp(155157377101)
            .setPrivateKey("db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9".toHexBytesInByteString())
            .setPayload(
                Aion.Payload.newBuilder()
                    .setPayloadTransfer(
                        Aion.Payload.Transfer.newBuilder()
                            .setAmount("2710".toHexBytesInByteString())
                    )
            )
            .build()

        val output = AnySigner.sign(signingInput, AION, SigningOutput.parser())

        assertEquals(
            "0xf89b09a0a082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e8227108085242019b04d8252088800000004a817c80001b860a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a7d3d3386742c2716031b79950cef5fcb49c079a5cab095c8b08915e126b9741389924ba2d5c00036a3b39c2a8562fa0800f1a13a566ce6e027274ce63a41dec07",
            output.encoded.toByteArray().toHex()
        )
        assertEquals(
            "0xa775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a7d3d3386742c2716031b79950cef5fcb49c079a5cab095c8b08915e126b9741389924ba2d5c00036a3b39c2a8562fa0800f1a13a566ce6e027274ce63a41dec07",
            output.signature.toByteArray().toHex()
        )        
    }

    @Test
    fun aionContractTransactionSign() {
        val signingInput = Aion.SigningInput.newBuilder()
            .setNonce("09".toHexBytesInByteString())
            .setGasPrice("04a817c800".toHexBytesInByteString())
            .setGasLimit("5208".toHexBytesInByteString())
            .setToAddress("0xa082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e")
            .setTimestamp(155157377101)
            .setPrivateKey("db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9".toHexBytesInByteString())
            .setPayload(
                Aion.Payload.newBuilder()
                    .setPayloadContractGeneric(
                        Aion.Payload.ContractGeneric.newBuilder()
                            .setPayload("0123".toHexBytesInByteString())
                    )
            )
            .build()

        val output = AnySigner.sign(signingInput, AION, SigningOutput.parser())

        assertEquals(
            "0xf89b09a0a082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e8082012385242019b04d8252088800000004a817c80001b860a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a73ff066d5cdd642f0c132a1afc484c7463773373aaff7ed5d9c6034b95713e91ecde3f8e9074f4d8e974528c45086e1e2df945cc0df5e22ddbf8a9d56888ac103",
            output.encoded.toByteArray().toHex()
        )
        assertEquals(
            "0xa775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a73ff066d5cdd642f0c132a1afc484c7463773373aaff7ed5d9c6034b95713e91ecde3f8e9074f4d8e974528c45086e1e2df945cc0df5e22ddbf8a9d56888ac103",
            output.signature.toByteArray().toHex()
        )
    }
}
