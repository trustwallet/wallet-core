package com.trustwallet.core.app.utils

import com.trustwallet.core.app.utils.toHexBytes
import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.Curve
import wallet.core.jni.Hash
import wallet.core.jni.PrivateKey


class TestPrivateKey {
    private val validPrivateKeyData = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5".toHexBytes()

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testCreate() {
        var privateKey = PrivateKey()
        var data = privateKey.data()
        assertTrue(data.size == 32);
    }

    @Test
    fun testInvalid() {
        val bytes = Numeric.hexStringToByteArray("deadbeaf")
        var privateKey: PrivateKey? = null
        try {
            privateKey = PrivateKey(bytes)
        } catch (ex: Exception) {
        }
        assertNull(privateKey)
    }

    @Test
    fun isValidForInvalidData() {
        val bytes = Numeric.hexStringToByteArray("deadbeaf")
        assertFalse(PrivateKey.isValid(bytes, Curve.SECP256K1))
        assertFalse(PrivateKey.isValid(bytes, Curve.ED25519))

        val bytes2 = Numeric.hexStringToByteArray("0000000000000000000000000000000000000000000000000000000000000000")
        assertFalse(PrivateKey.isValid(bytes2, Curve.SECP256K1))
        assertFalse(PrivateKey.isValid(bytes2, Curve.ED25519))

        val bytes3 = Numeric.hexStringToByteArray("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141")
        assertFalse(PrivateKey.isValid(bytes3, Curve.SECP256K1))
    }

    @Test
    fun isValidForValidData() {
        assertTrue(PrivateKey.isValid(validPrivateKeyData, Curve.SECP256K1))
        assertTrue(PrivateKey.isValid(validPrivateKeyData, Curve.ED25519))
    }

    @Test
    fun testValid() {
        assertTrue(PrivateKey.isValid(validPrivateKeyData, Curve.SECP256K1))
        var privateKey: PrivateKey? = null
        try {
            privateKey = PrivateKey(validPrivateKeyData)
        } catch (ex: Exception) {

        }
        assertNotNull(privateKey)
    }
}