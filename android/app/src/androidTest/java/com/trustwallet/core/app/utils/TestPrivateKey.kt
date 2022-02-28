package com.trustwallet.core.app.utils

import com.trustwallet.core.app.utils.toHexBytes
import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.Curve
import wallet.core.jni.Hash
import wallet.core.jni.PrivateKey
import wallet.core.jni.PublicKey
import wallet.core.jni.PublicKeyType

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

    @Test
    fun testGetSharedKey() {
        val privateKeyData = "9cd3b16e10bd574fed3743d8e0de0b7b4e6c69f3245ab5a168ef010d22bfefa0".toHexBytes()
        val privateKey = PrivateKey(privateKeyData)
        
        val publicKeyData = "02a18a98316b5f52596e75bfa5ca9fa9912edd0c989b86b73d41bb64c9c6adb992".toHexBytes()
        val publicKey = PublicKey(publicKeyData, PublicKeyType.SECP256K1)

        val derivedData = privateKey.getSharedKey(publicKey, Curve.SECP256K1)
        assertNotNull(derivedData)

        assertEquals(derivedData?.toHex(), "0xef2cf705af8714b35c0855030f358f2bee356ff3579cea2607b2025d80133c3a")
    }

    @Test
    fun testGetSharedKeyWycherproof() {
        val privateKeyData = "f4b7ff7cccc98813a69fae3df222bfe3f4e28f764bf91b4a10d8096ce446b254".toHexBytes()
        val privateKey = PrivateKey(privateKeyData)
        
        val publicKeyData = "02d8096af8a11e0b80037e1ee68246b5dcbb0aeb1cf1244fd767db80f3fa27da2b".toHexBytes()
        val publicKey = PublicKey(publicKeyData, PublicKeyType.SECP256K1)

        val derivedData = privateKey.getSharedKey(publicKey, Curve.SECP256K1)
        assertNotNull(derivedData)

        assertEquals(derivedData?.toHex(), "0x81165066322732362ca5d3f0991d7f1f7d0aad7ea533276496785d369e35159a")
    }

    @Test
    fun testGetSharedKeyBidirectional() {
        val privateKeyData1 = "9cd3b16e10bd574fed3743d8e0de0b7b4e6c69f3245ab5a168ef010d22bfefa0".toHexBytes()
        val privateKey1 = PrivateKey(privateKeyData1)
        val publicKey1 = privateKey1.getPublicKeySecp256k1(true)
        
        val privateKeyData2 = "ef2cf705af8714b35c0855030f358f2bee356ff3579cea2607b2025d80133c3a".toHexBytes()
        val privateKey2 = PrivateKey(privateKeyData2)
        val publicKey2 = privateKey2.getPublicKeySecp256k1(true)

        val derivedData1 = privateKey1.getSharedKey(publicKey2, Curve.SECP256K1)
        assertNotNull(derivedData1)

        val derivedData2 = privateKey2.getSharedKey(publicKey1, Curve.SECP256K1)
        assertNotNull(derivedData2)

        assertEquals(derivedData1?.toHex(), derivedData2?.toHex())
    }

    @Test
    fun testGetSharedKeyError() {
        val privateKeyData = "9cd3b16e10bd574fed3743d8e0de0b7b4e6c69f3245ab5a168ef010d22bfefa0".toHexBytes()
        val privateKey = PrivateKey(privateKeyData)
        
        val publicKeyData = "02a18a98316b5f52596e75bfa5ca9fa9912edd0c989b86b73d41bb64c9c6adb992".toHexBytes()
        val publicKey = PublicKey(publicKeyData, PublicKeyType.SECP256K1)

        val derivedData = privateKey.getSharedKey(publicKey, Curve.ED25519)
        assertNull(derivedData)
    }    
}