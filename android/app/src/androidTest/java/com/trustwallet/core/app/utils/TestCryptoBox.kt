package com.trustwallet.core.app.utils

import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHex
import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.*

class TestCryptoBox {
    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testEncryptDecryptEasy() {
        val mySecret = CryptoBoxSecretKey()
        val myPubkey = mySecret.publicKey

        val otherSecret = CryptoBoxSecretKey()
        val otherPubkey = otherSecret.publicKey

        val message = "Well done is better than well said. -Benjamin Franklin"
        val encrypted = CryptoBox.encryptEasy(mySecret, otherPubkey, message.toByteArray())

        // Step 2. Make sure the Box can be decrypted by the other side.
        val decrypted = CryptoBox.decryptEasy(otherSecret, myPubkey, encrypted)
        assertEquals(decrypted.toString(Charsets.UTF_8), message)
    }

    @Test
    fun testSecretKeyFromToBytes() {
        val secretBytesHex = "0xdd87000d4805d6fbd89ae1352f5e4445648b79d5e901c92aebcb610e9be468e4"
        val secretBytes = secretBytesHex.toHexByteArray()
        assert(CryptoBoxSecretKey.isValid(secretBytes))
        val secret = CryptoBoxSecretKey(secretBytes)
        assertEquals(secret.data().toHex(), secretBytesHex)
    }

    @Test
    fun testPublicKeyFromToBytes() {
        val publicBytesHex = "0xafccabc5b28a8a1fd1cd880516f9c854ae2498d0d1b978b53a59f38e4ae55747"
        val publicBytes = publicBytesHex.toHexByteArray()
        assert(CryptoBoxPublicKey.isValid(publicBytes))
        val pubkey = CryptoBoxPublicKey(publicBytes)
        assertEquals(pubkey.data().toHex(), publicBytesHex)
    }
}