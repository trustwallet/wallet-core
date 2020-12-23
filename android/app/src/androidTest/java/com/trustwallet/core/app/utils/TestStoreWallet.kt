// Copyright © 2019-2020 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.utils

import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.CoinType
import wallet.core.jni.CoinType.BITCOIN
import wallet.core.jni.PrivateKey
import wallet.core.jni.StoredKey
import wallet.core.jni.StoreWallet
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHex

class TestStoreWallet {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    val password = "password"
    val walletPath = "testwallet"

    fun createWallet(): StoreWallet {
        val keyStore = StoredKey("name", password.toByteArray(), "team engine square letter hero song dizzy scrub tornado fabric divert saddle")
        val path = "./" + walletPath
        return StoreWallet(path, keyStore)
    }

    @Test
    fun testCreate() {
        val wallet = createWallet()
        assertEquals(wallet.indentifier(), walletPath)
    }

    @Test
    fun testGetAccount() {
        val wallet = createWallet()
        val coin = CoinType.BITCOIN
        val account = wallet.getAccount(password, coin)
        assertEquals(account.coin(), coin)
        assertEquals(account.address(), "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny")
    }

    @Test
    fun testGetPrivateKey() {
        val wallet = createWallet()
        val coin = CoinType.BITCOIN
        val privateKey = wallet.privateKey(password, coin)
        assertEquals(privateKey.data().toHex(), "0xd2568511baea8dc347f14c4e0479eb8ebe29eb5f664ed796e755896250ffd11f")
    }
}
