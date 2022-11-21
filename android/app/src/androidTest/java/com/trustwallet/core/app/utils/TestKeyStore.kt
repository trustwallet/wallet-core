package com.trustwallet.core.app.utils

import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.StoredKey
import wallet.core.jni.CoinType
import wallet.core.jni.StoredKeyEncryption

class TestKeyStore {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testDecryptMnemonic() {
        val keyStore = StoredKey("Test Wallet", "password".toByteArray())
        val result = keyStore.decryptMnemonic("wrong".toByteArray())
        val result2 = keyStore.decryptMnemonic("password".toByteArray())

        assertNull(result)
        assertNotNull(result2)
    }

    @Test
    fun testDecryptMnemonicAes256() {
        val keyStore = StoredKey("Test Wallet", "password".toByteArray(), StoredKeyEncryption.AES256CTR)
        val result = keyStore.decryptMnemonic("wrong".toByteArray())
        val result2 = keyStore.decryptMnemonic("password".toByteArray())

        assertNull(result)
        assertNotNull(result2)
    }

    @Test
    fun testRemoveCoins() {
        val password = "password".toByteArray()
        val keyStore = StoredKey("Test Wallet", password)
        val wallet = keyStore.wallet(password)

        assertNotNull(keyStore.accountForCoin(CoinType.BITCOIN, wallet))
        assertNotNull(keyStore.accountForCoin(CoinType.ETHEREUM, wallet))
        assertEquals(keyStore.accountCount(), 2)

        keyStore.removeAccountForCoin(CoinType.BITCOIN)

        assertEquals(keyStore.accountCount(), 1)
        assertEquals(keyStore.account(0).coin(), CoinType.ETHEREUM)
    }

    @Test
    fun testLongHexPassword() {
        val json = """
            {
            "address": "34bae2218c254ed190c0f5b1dd4323aee8e7da09",
            "id": "86066d8c-8dba-4d81-afd4-934e2a2b72a2",
            "version": 3,
            "crypto": {
                "cipher": "aes-128-ctr",
                "cipherparams": {
                    "iv": "a4976ad73057007ad788d1f792db851d"
                },
                "ciphertext": "5e4458d69964172c492616b751d6589b4ad7da4217dcfccecc3f4e515a934bb8",
                "kdf": "scrypt",
                "kdfparams": {
                    "dklen": 32,
                    "n": 4096,
                    "p": 6,
                    "r": 8,
                    "salt": "24c72d92bf88a4f7c7b3f5e3cb3620714d71fceabbb0bc6099f50c6d5d898e7c"
                },
                "mac": "c15e3035ddcaca766dfc56648978d33e94d3c57d4a5e13fcf8b5f8dbb0902900"
            }
        }
        """.trimIndent()
        val password = "2d6eefbfbd4622efbfbdefbfbd516718efbfbdefbfbdefbfbdefbfbd59efbfbd30efbfbdefbfbd3a4348efbfbd2aefbfbdefbfbd49efbfbd27efbfbd0638efbfbdefbfbdefbfbd4cefbfbd6befbfbdefbfbd6defbfbdefbfbd63efbfbd5aefbfbd61262b70efbfbdefbfbdefbfbdefbfbdefbfbdc7aa373163417cefbfbdefbfbdefbfbd44efbfbdefbfbd1d10efbfbdefbfbdefbfbd61dc9e5b124befbfbd11efbfbdefbfbd2fefbfbdefbfbd3d7c574868efbfbdefbfbdefbfbd37043b7b5c1a436471592f02efbfbd18efbfbdefbfbd2befbfbdefbfbd7218efbfbd6a68efbfbdcb8e5f3328773ec48174efbfbd67efbfbdefbfbdefbfbdefbfbdefbfbd2a31efbfbd7f60efbfbdd884efbfbd57efbfbd25efbfbd590459efbfbd37efbfbd2bdca20fefbfbdefbfbdefbfbdefbfbd39450113efbfbdefbfbdefbfbd454671efbfbdefbfbdd49fefbfbd47efbfbdefbfbdefbfbdefbfbd00efbfbdefbfbdefbfbdefbfbd05203f4c17712defbfbd7bd1bbdc967902efbfbdc98a77efbfbd707a36efbfbd12efbfbdefbfbd57c78cefbfbdefbfbdefbfbd10efbfbdefbfbdefbfbde1a1bb08efbfbdefbfbd26efbfbdefbfbd58efbfbdefbfbdc4b1efbfbd295fefbfbd0eefbfbdefbfbdefbfbd0e6eefbfbd"
        val pass = password.toHexByteArray()
        val keyStore = StoredKey.importJSON(json.toByteArray())
        val privateKey = keyStore.decryptPrivateKey(pass)
        assertEquals(privateKey.toHex(), "0x043c5429c7872502531708ec0d821c711691402caf37ef7ba78a8c506f10653b")
    }

    @Test
    fun testExportJSON() {
        val password = "password".toByteArray()
        val keyStore = StoredKey("Test Wallet", password)
        val json = keyStore.exportJSON()
        assertNotNull(json)

        val newKeyStore = StoredKey.importJSON(json)
        val privateKey = newKeyStore.decryptPrivateKey("".toByteArray())
        assertNull(privateKey)
    }
}
