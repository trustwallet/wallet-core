package com.trustwallet.core.app.utils

import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.StoredKey

class TestKeyStore {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testDecryptMnemonic() {
        val keyStore = StoredKey("Test Wallet", "password")
        val result = keyStore.decryptMnemonic("wrong")
        val result2 = keyStore.decryptMnemonic("password")

        assertNull(result)
        assertNotNull(result2)
    }
}
