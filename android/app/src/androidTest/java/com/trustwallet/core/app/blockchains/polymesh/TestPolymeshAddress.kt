// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.polymesh

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestPolymeshAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {

        val key = PrivateKey("0x790a0a01ec2e7c7db4abcaffc92ce70a960ef9ad3021dbe3bf327c1c6343aee4".toHexByteArray())
        val pubkey = key.publicKeyEd25519
        val address = AnyAddress(pubkey, CoinType.POLYMESH)
        val expected = AnyAddress("2EANwBfNsFu9KV8JsW5sbhF6ft8bzvw5EW1LCrgHhrqtK6Ys", CoinType.POLYMESH)

        assertEquals(pubkey.data().toHex(), "0x4bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c")
        assertEquals(address.description(), expected.description())
    }
}
