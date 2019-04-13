package com.trustwallet.core.app.utils

import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.Curve
import wallet.core.jni.Hash
import wallet.core.jni.PrivateKey


class TestPrivateKey {
    private val validPrivateKeyData = intArrayOf(0xaf, 0xee, 0xfc, 0xa7, 0x4d, 0x9a, 0x32, 0x5c, 0xf1, 0xd6, 0xb6, 0x91, 0x1d, 0x61, 0xa6, 0x5c, 0x32, 0xaf, 0xa8, 0xe0, 0x2b, 0xd5, 0xe7, 0x8e, 0x2e, 0x4a, 0xc2, 0x91, 0x0b, 0xab, 0x45, 0xf5)
            .map { it.toByte() }
            .toByteArray()

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
        val bytes = byteArrayOf(0xde.toByte(), 0xad.toByte(), 0xbe.toByte(), 0xef.toByte())
        var privateKey: PrivateKey? = null
        try {
            privateKey = PrivateKey(bytes)
        } catch (ex: Exception) {
        }
        assertNull(privateKey)
    }

    @Test
    fun isValidForInvalidData() {
        val bytes = byteArrayOf(0xde.toByte(), 0xad.toByte(), 0xbe.toByte(), 0xef.toByte())
        assertFalse(PrivateKey.isValid(bytes))
    }

    @Test
    fun isValidForValidData() {
        assertTrue(PrivateKey.isValid(validPrivateKeyData))
    }

    @Test
    fun testValid() {
        assertTrue(PrivateKey.isValid(validPrivateKeyData))
        var privateKey: PrivateKey? = null
        try {
            privateKey = PrivateKey(validPrivateKeyData)
        } catch (ex: Exception) {

        }
        assertNotNull(privateKey)
    }

    @Test
    fun testPublicKeyCompressed() {
        var privateKey: PrivateKey? = null
        try {
            privateKey = PrivateKey(validPrivateKeyData)
        } catch (ex: Exception) {

        }
        val publicKey = privateKey?.getPublicKeySecp256k1(true)
        val valid = arrayOf(0x03, 0x99, 0xc6, 0xf5, 0x1a, 0xd6, 0xf9, 0x8c, 0x9c, 0x58, 0x3f, 0x8e, 0x92, 0xbb, 0x77, 0x58, 0xab, 0x2c, 0xa9, 0xa0, 0x41, 0x10, 0xc0, 0xa1, 0x12, 0x6e, 0xc4, 0x3e, 0x54, 0x53, 0xd1, 0x96, 0xc1)
                .map { it.toByte() }
                .toByteArray()
        assertEquals(Numeric.toHexString(valid), Numeric.toHexString(publicKey?.data()))
    }

    @Test
    fun testSign() {
        val validSign = arrayOf(0x87, 0x20, 0xa4, 0x6b, 0x5b, 0x39, 0x63, 0x79, 0x0d, 0x94, 0xbc, 0xc6, 0x1a, 0xd5, 0x7c, 0xa0, 0x2f, 0xd1, 0x53, 0x58, 0x43, 0x15, 0xbf, 0xa1, 0x61, 0xed, 0x34, 0x55, 0xe3, 0x36, 0xba, 0x62, 0x4d, 0x68, 0xdf, 0x01, 0x0e, 0xd9, 0x34, 0xb8, 0x79, 0x2c, 0x5b, 0x6a, 0x57, 0xba, 0x86, 0xc3, 0xda, 0x31, 0xd0, 0x39, 0xf9, 0x61, 0x2b, 0x44, 0xd1, 0xbf, 0x05, 0x41, 0x32, 0x25, 0x4d, 0xe9, 0x01)
                .map { it.toByte() }
                .toByteArray()
        val data = Hash.keccak256("hello".toByteArray())
        val sign = PrivateKey(validPrivateKeyData).sign(data, Curve.SECP256K1)
        assertEquals(Numeric.toHexString(sign), Numeric.toHexString(validSign))
    }
}