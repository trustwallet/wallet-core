// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.stargaze

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestStargazeAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433".toHexByteArray())
        val pubkey = key.getPublicKeySecp256k1(true)
        val address = AnyAddress(pubkey, CoinType.STARGAZE)
        val expected = AnyAddress("stars1mry47pkga5tdswtluy0m8teslpalkdq02a8nhy", CoinType.STARGAZE)

        assertEquals(pubkey.data().toHex(), "0x02cbfdb5e472893322294e60cf0883d43df431e1089d29ecb447a9e6d55045aae5")
        assertEquals(address.description(), expected.description())
    }
}
