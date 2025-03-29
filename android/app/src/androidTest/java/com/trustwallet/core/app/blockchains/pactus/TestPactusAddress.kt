// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.pactus

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestPactusAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testMainnetAddress() {
        val key = PrivateKey("4e51f1f3721f644ac7a193be7f5e7b8c2abaa3467871daf4eacb5d3af080e5d6".toHexByteArray())
        val pubkey = key.publicKeyEd25519
        val address = AnyAddress(pubkey, CoinType.PACTUS)
        val expected = AnyAddress("pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr", CoinType.PACTUS)

        assertEquals(pubkey.data().toHex(), "0x95794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa")
        assertEquals(address.description(), expected.description())
    }

    @Test
    fun testTestnetAddress() {
        val key = PrivateKey("4e51f1f3721f644ac7a193be7f5e7b8c2abaa3467871daf4eacb5d3af080e5d6".toHexByteArray())
        val pubkey = key.publicKeyEd25519
        val address = AnyAddress(pubkey, CoinType.PACTUS, Derivation.PACTUSTESTNET)
        val expected = AnyAddress("tpc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymzqkcrg", CoinType.PACTUS)

        assertEquals(pubkey.data().toHex(), "0x95794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa")
        assertEquals(address.description(), expected.description())
    }
}
