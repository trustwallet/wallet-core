package com.trustwallet.core.app.utils

import org.json.JSONObject
import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.StoredKey
import wallet.core.jni.CoinType
import wallet.core.jni.StoredKeyEncryption
import java.io.File

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
    fun testDecryptMnemonicAes192Ctr() {
        val keyStore = StoredKey("Test Wallet", "password".toByteArray(), StoredKeyEncryption.AES192CTR)
        val result = keyStore.decryptMnemonic("wrong".toByteArray())
        val result2 = keyStore.decryptMnemonic("password".toByteArray())

        assertNull(result)
        assertNotNull(result2)
    }

    @Test
    fun testDecryptMnemonicAes256Ctr() {
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

    @Test
    fun testImportKeyEncodedEthereum() {
        val privateKeyHex = "9cdb5cab19aec3bd0fcd614c5f185e7a1d97634d4225730eba22497dc89a716c"
        val password = "password".toByteArray()
        val key = StoredKey.importPrivateKeyEncoded(privateKeyHex, "name", password, CoinType.ETHEREUM)
        val json = key.exportJSON()

        val keyStore = StoredKey.importJSON(json)
        val storedEncoded = keyStore.decryptPrivateKeyEncoded(password)

        assertTrue(keyStore.hasPrivateKeyEncoded())
        assertNotNull(keyStore)
        assertNotNull(storedEncoded)
        assertEquals(privateKeyHex, storedEncoded)
    }

    @Test
    fun testFixScryptWithEmptySalt() {
        val gMnemonic = "team engine square letter hero song dizzy scrub tornado fabric divert saddle"
        val password = "password".toByteArray()
        val walletID = "8e334366-020b-493f-81ab-a946432f536d"

        val json = """
            {
              "activeAccounts": [
                {
                  "address": "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny",
                  "coin": 0,
                  "derivationPath": "m/84'/0'/0'/0/0",
                  "extendedPublicKey": "zpub6qbsWdbcKW9sC6shTKK4VEhfWvDCoWpfLnnVfYKHLHt31wKYUwH3aFDz4WLjZvjHZ5W4qVEyk37cRwzTbfrrT1Gnu8SgXawASnkdQ994atn",
                  "publicKey": "02df6fc590ab3101bbe0bb5765cbaeab9b5dcfe09ac9315d707047cbd13bc7e006"
                }
              ],
              "crypto": {
                "cipher": "aes-128-ctr",
                "cipherparams": { "iv": "f375d3903fa00839c43109b1d26436b7" },
                "ciphertext": "9768cdec22c3b0d5d7eced4e5387c138045367b9481d5cf017d262e645accd478f0f197f6dcb97e2ce9105fee0e7074d891a9826df3bc8f4dca17f5cdfb9992b86e40f26028c5a392cb2de17",
                "kdf": "scrypt",
                "kdfparams": { "dklen": 32, "n": 16384, "p": 4, "r": 8, "salt": "" },
                "mac": "bbdba986c713d91828a7a2f031d3535414967fce81c6c826b50b0cfab8783dfc"
              },
              "id": "8e334366-020b-493f-81ab-a946432f536d",
              "name": "name",
              "type": "mnemonic",
              "version": 3
            }
        """.trimIndent()

        val key = StoredKey.importJSON(json.toByteArray())
        assertEquals(walletID, key.identifier())

        val cryptoBefore = JSONObject(String(key.exportJSON())).getJSONObject("crypto")
        val saltBefore = cryptoBefore.getJSONObject("kdfparams").getString("salt")
        val ciphertextBefore = cryptoBefore.getString("ciphertext")
        assertEquals("", saltBefore)

        assertTrue(key.fixEncryption(password))

        val tmpFile = File.createTempFile("scrypt-empty-salt", ".json")
        try {
            assertTrue(key.store(tmpFile.absolutePath))

            val reloadedKey = StoredKey.load(tmpFile.absolutePath)
            assertNotNull(reloadedKey)

            assertEquals(walletID, reloadedKey.identifier())

            val cryptoAfter = JSONObject(String(reloadedKey.exportJSON())).getJSONObject("crypto")
            val saltAfter = cryptoAfter.getJSONObject("kdfparams").getString("salt")
            val ciphertextAfter = cryptoAfter.getString("ciphertext")

            assertTrue(saltAfter.isNotEmpty())
            assertNotEquals(ciphertextBefore, ciphertextAfter)

            assertEquals(gMnemonic, reloadedKey.decryptMnemonic(password))
        } finally {
            tmpFile.delete()
        }
    }

    @Test
    fun testImportKeyEncodedSolana() {
        val privateKeyBase58 = "A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr"
        val password = "password".toByteArray()
        val key = StoredKey.importPrivateKeyEncoded(privateKeyBase58, "name", password, CoinType.SOLANA)
        val json = key.exportJSON()

        val keyStore = StoredKey.importJSON(json)
        val storedEncoded = keyStore.decryptPrivateKeyEncoded(password)

        assertTrue(keyStore.hasPrivateKeyEncoded())
        assertNotNull(keyStore)
        assertNotNull(storedEncoded)
        assertEquals(privateKeyBase58, storedEncoded)
    }
}
