// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.zen

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestZenAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("3a8e0a528f62f4ca2c77744c8a571def2845079b50105a9f7ef6b1b823def67a".toHexByteArray())
        val pubkey = key.getPublicKeySecp256k1(true)
        val address = AnyAddress(pubkey, CoinType.ZEN)
        val expected = AnyAddress("znk19H1wcARcCa7TM6zgmJUbWoWWtZ8k5cg", CoinType.ZEN)

        assertEquals(pubkey.data().toHex(), "0x02b4ac9056d20c52ac11b0d7e83715dd3eac851cfc9cb64b8546d9ea0d4bb3bdfe")
        assertEquals(address.description(), expected.description())
    }
}
