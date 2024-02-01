// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.smartbitcoincash

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestSmartBitcoinCashAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("155cbd57319f3d938977b4c18000473eb3c432c4e31b667b63e88559c497d82d".toHexByteArray())
        val pubkey = key.getPublicKeySecp256k1(false)
        val address = AnyAddress(pubkey, CoinType.SMARTBITCOINCASH)
        val expected = AnyAddress("0x8bFC9477684987dcAf0970b9bce5E3D9267C99C0", CoinType.SMARTBITCOINCASH)

        assertEquals(pubkey.data().toHex(), "0x046439f94100c802691c53ef18523be2c24d301f0e2bd3b425e832378a5405eff4331d5e57303785969073321fc76a8504a3854bdb21e6ab7b268a1737882a29c0")
        assertEquals(address.description(), expected.description())
    }
}
