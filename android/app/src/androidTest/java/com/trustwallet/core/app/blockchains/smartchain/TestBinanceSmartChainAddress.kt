// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.binancesmartchain

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestBinanceSmartChainAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("727f677b390c151caf9c206fd77f77918f56904b5504243db9b21e51182c4c06".toHexByteArray())
        val pubkey = key.getPublicKeySecp256k1(false)
        val address = AnyAddress(pubkey, CoinType.SMARTCHAIN)
        val expected = AnyAddress("0xf3d468DBb386aaD46E92FF222adDdf872C8CC064", CoinType.SMARTCHAIN)

        assertEquals(address.description(), expected.description())
    }
}
