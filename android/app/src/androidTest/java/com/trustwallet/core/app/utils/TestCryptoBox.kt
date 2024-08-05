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
}