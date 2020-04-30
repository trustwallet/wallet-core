// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.fio

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Assert.assertNull
import org.junit.Test
import wallet.core.jni.*

class TestFIOAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val addressFromString = AnyAddress("FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o", CoinType.FIO)
        assertEquals(addressFromString.description(), "FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o")

        val key = PrivateKey("ea8eb60b7e5868e218f248e032769020b4fea5dcfd02f2992861eaf4fb534854".toHexByteArray())
        val pubkey = key.getPublicKeySecp256k1(false)
        val addressFromKey = AnyAddress(pubkey, CoinType.FIO)
        assertEquals(addressFromKey.description(), "FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o")
    }

    @Test
    fun testAccount() {
        assertEquals(FIOAccount("FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE").description(), "hhq2g4qgycfb")
        assertEquals(FIOAccount("hhq2g4qgycfb").description(), "hhq2g4qgycfb")
        assertEquals(FIOAccount("rewards@wallet").description(), "rewards@wallet")

        var account: FIOAccount? = null
        var account2: FIOAccount? = null
        try {
            account = FIOAccount("asdf19s")
            account2 = FIOAccount("0x320196ef1b137786be51aa391e78e0a2c756f46b")
        } catch (ex: Exception) {
            print(ex)
        }

        assertNull(account)
        assertNull(account2)
    }
}
