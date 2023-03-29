package com.trustwallet.core.app.blockchains.filecoin

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.*
import wallet.core.jni.proto.Filecoin
import wallet.core.jni.proto.Filecoin.SigningOutput

class TestFilecoin {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testCreateAddress() {
        val privateKey = PrivateKey("1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe".toHexByteArray())
        val publicKey = privateKey.getPublicKeySecp256k1(false)
        val address = AnyAddress(publicKey, CoinType.FILECOIN)
        assertEquals("f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq", address.description())
    }

    @Test
    fun testCreateDelegatedAddress() {
        val privateKey = PrivateKey("825d2bb32965764a98338139412c7591ed54c951dd65504cd8ddaeaa0fea7b2a".toHexByteArray())
        val publicKey = privateKey.getPublicKeySecp256k1(false)
        val address = AnyAddress(publicKey, FilecoinAddressType.DELEGATED)
        assertEquals("f410fvak24cyg3saddajborn6idt7rrtfj2ptauk5pbq", address.description())
    }

    @Test
    fun testAddressConverter() {
        val ethereumAddress = FilecoinAddressConverter.convertToEthereum("f410frw6wy7w6sbsguyn3yzeygg34fgf72n5ao5sxyky")
        assertEquals(ethereumAddress, "0x8dbD6c7Ede90646a61Bbc649831b7c298BFd37A0")
        assert(AnyAddress.isValid(ethereumAddress, CoinType.ETHEREUM))

        val filecoinAddress = FilecoinAddressConverter.convertFromEthereum("0x8dbD6c7Ede90646a61Bbc649831b7c298BFd37A0")
        assertEquals(filecoinAddress, "f410frw6wy7w6sbsguyn3yzeygg34fgf72n5ao5sxyky")
        assert(AnyAddress.isValid(filecoinAddress, CoinType.FILECOIN))
    }

    @Test
    fun testSigner() {
        val input = Filecoin.SigningInput.newBuilder()
            .setPrivateKey("1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe".toHexBytesInByteString())
            .setTo("f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq")
            .setNonce(2)
            // 600 FIL
            .setValue(ByteString.copyFrom("2086ac351052600000".toHexByteArray()))
            .setGasLimit(1000)
            .setGasFeeCap(ByteString.copyFrom("25f273933db5700000".toHexByteArray()))
            .setGasPremium(ByteString.copyFrom("2b5e3af16b18800000".toHexByteArray()))
            .build()

        val output = AnySigner.sign(input, CoinType.FILECOIN, SigningOutput.parser())
        val expected = """{"Message":{"From":"f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq","GasFeeCap":"700000000000000000000","GasLimit":1000,"GasPremium":"800000000000000000000","Method":0,"Nonce":2,"To":"f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq","Value":"600000000000000000000"},"Signature":{"Data":"jMRu+OZ/lfppgmqSfGsntFrRLWZnUg3ZYmJTTRLsVt4V1310vR3VKGJpaE6S4sNvDOE6sEgmN9YmfTkPVK2qMgE=","Type":1}}"""
        assertEquals(expected, output.json)
    }
}
