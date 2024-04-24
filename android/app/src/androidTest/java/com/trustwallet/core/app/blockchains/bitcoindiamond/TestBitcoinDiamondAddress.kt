// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.bitcoindiamond

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestBitcoinDiamondAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("d2b9f2846d3adcead910ee0124a3ba7ae29e8a4729787d27f9bea1f532928eee".toHexByteArray())
        val pubkey = key.getPublicKeySecp256k1(true);
        val address = AnyAddress(pubkey, CoinType.BITCOINDIAMOND)
        val expected = AnyAddress("1G15VvshDxwFTnahZZECJfFwEkq9fP79o8", CoinType.BITCOINDIAMOND)

        assertEquals(pubkey.data().toHex(), "0x02485a209514cc896f8ed736e205bc4c35bd5299ef3f9e84054475336b964c02a3")
        assertEquals(address.description(), expected.description())
    }
}
