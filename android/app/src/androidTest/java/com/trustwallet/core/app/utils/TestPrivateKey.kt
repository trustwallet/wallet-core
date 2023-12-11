package com.trustwallet.core.app.utils

import com.trustwallet.core.app.utils.toHexBytes
import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.*

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
    fun testCreateStarkKey() {
        val data = Numeric.hexStringToByteArray("06cf0a8bf113352eb863157a45c5e5567abb34f8d32cddafd2c22aa803f4892c")
        assertTrue(PrivateKey.isValid(data, Curve.STARKEX))
        var privateKey = PrivateKey(data)
        var pubKey = privateKey.getPublicKeyByType(PublicKeyType.STARKEX)
        assertEquals(pubKey.data().toHex(), "0x02d2bbdc1adaf887b0027cdde2113cfd81c60493aa6dc15d7887ddf1a82bc831")
    }

    @Test
    fun testCreateStarkKeySigning() {
        val data = Numeric.hexStringToByteArray("0139fe4d6f02e666e86a6f58e65060f115cd3c185bd9e98bd829636931458f79")
        var privateKey = PrivateKey(data)
        val digest = Numeric.hexStringToByteArray("06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76")
        val signature = privateKey.sign(digest, Curve.STARKEX)
        assertEquals(signature.toHex(), "0x061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9a")
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
    fun testGetPublicKeyCoinType() {
        val privateKeyData = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5".toHexBytes()
        val privateKey = PrivateKey(privateKeyData)
        assertEquals(privateKey.getPublicKey(CoinType.ETHEREUM).data().toHex(), "0x0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91");
    }
}