// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.internetcomputer

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestInternetComputerAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("ee42eaada903e20ef6e5069f0428d552475c1ea7ed940842da6448f6ef9d48e7".toHexByteArray())
        val pubkey = key.getPublicKeySecp256k1(false);
        val address = AnyAddress(pubkey, CoinType.INTERNETCOMPUTER)
        val expected = AnyAddress("2f25874478d06cf68b9833524a6390d0ba69c566b02f46626979a3d6a4153211", CoinType.INTERNETCOMPUTER)

        assertEquals(pubkey.data().toHex(), "0x048542e6fb4b17d6dfcac3948fe412c00d626728815ee7cc70509603f1bc92128a6e7548f3432d6248bc49ff44a1e50f6389238468d17f7d7024de5be9b181dbc8")
        assertEquals(address.description(), expected.description())
    }
}