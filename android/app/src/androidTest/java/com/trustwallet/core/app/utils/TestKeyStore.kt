package com.trustwallet.core.app.utils

import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.StoredKey
import wallet.core.jni.CoinType

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

    @Test
    fun testRemoveCoins() {
        val password = "password"
        val keyStore = StoredKey("Test Wallet", password)
        val wallet = keyStore.wallet(password)

        assertNotNull(keyStore.accountForCoin(CoinType.BITCOIN, wallet))
        assertNotNull(keyStore.accountForCoin(CoinType.ETHEREUM, wallet))
        assertEquals(keyStore.accountCount(), 2)

        keyStore.removeAccountForCoin(CoinType.BITCOIN)

        assertEquals(keyStore.accountCount(), 1)
        assertEquals(keyStore.account(0).coin(), CoinType.ETHEREUM)
    }
}
