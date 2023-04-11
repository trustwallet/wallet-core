package com.trustwallet.core.app.utils

import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHex
import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.Curve
import wallet.core.jni.Hash
import wallet.core.jni.PrivateKey
import wallet.core.jni.PublicKey


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
