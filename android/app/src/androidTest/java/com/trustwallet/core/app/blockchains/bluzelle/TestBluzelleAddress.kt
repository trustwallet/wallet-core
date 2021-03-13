// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.bluzelle

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestBluzelleAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {

        val key = PrivateKey("1037f828ca313f4c9e120316e8e9ff25e17f07fe66ba557d5bc5e2eeb7cba8f6".toHexByteArray())
        val pubkey = key.publicKeyEd25519
        val address = AnyAddress(pubkey, CoinType.BLUZELLE)
        val expected = AnyAddress("bluzelle1yhtq5zm293m2r3sp2guj9m5pg5e273n6r0szul", CoinType.BLUZELLE)

        assertEquals(pubkey.data().toHex(), "0x381207e7f7fa7c44564534b0f47dfef3fc942e42defc10b2bf614e4dfa773e5a")
        assertEquals(address.description(), expected.description())
    }
}
