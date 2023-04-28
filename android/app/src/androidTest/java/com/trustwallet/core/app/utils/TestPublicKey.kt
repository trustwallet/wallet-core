package com.trustwallet.core.app.utils

import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHex
import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.*


class TestPublicKey {
    private val validPrivateKeyData = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5".toHexBytes()

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testPublicKeyCompressed() {
        var privateKey: PrivateKey? = null
        try {
            privateKey = PrivateKey(validPrivateKeyData)
        } catch (ex: Exception) {

        }
        val publicKey = privateKey?.getPublicKeySecp256k1(true)
        assertEquals("0x0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1", publicKey?.data()?.toHex())
    }
    
    @Test
    fun testVerifyStarkey() {
        val data = Numeric.hexStringToByteArray("02c5dbad71c92a45cc4b40573ae661f8147869a91d57b8d9b8f48c8af7f83159")
        val publicKey = PublicKey(data, PublicKeyType.STARKEX)
        var signature = Numeric.hexStringToByteArray("061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9a")
        val hash = Numeric.hexStringToByteArray("06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76")
        assertTrue(publicKey.verify(signature, hash))
        signature = Numeric.hexStringToByteArray("061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9b")
        assertFalse(publicKey.verify(signature, hash))
    }

    @Test
    fun testSign() {
        val validSign = "0x8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de901"
        val data = Hash.keccak256("hello".toByteArray())
        val sign = PrivateKey(validPrivateKeyData).sign(data, Curve.SECP256K1)
        assertEquals(sign.toHex(), validSign)
    }

    @Test
    fun testVerify() {
        val privateKey = PrivateKey(validPrivateKeyData)

        val message = Hash.sha256("hello".toByteArray())
        val sig1 = privateKey.sign(message, Curve.ED25519)
        val result1 = privateKey.getPublicKeyEd25519().verify(sig1, message)
        assert(result1)

        val sig2 = privateKey.sign(message, Curve.ED25519BLAKE2BNANO)
        val result2 = privateKey.getPublicKeyEd25519Blake2b().verify(sig2, message)
        assert(result2)

        val sig3 = privateKey.sign(message, Curve.SECP256K1)
        val result3 = privateKey.getPublicKeySecp256k1(true).verify(sig3, message)
        assert(result3)
    }
}
