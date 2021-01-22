// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.oasis

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestOasisAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("4f8b5676990b00e23d9904a92deb8d8f428ff289c8939926358f1d20537c21a0".toHexByteArray())
        val pubkey = key.publicKeyEd25519
        val address = AnyAddress(pubkey, CoinType.OASIS)
        val expected = AnyAddress("oasis1qzawzy5kaa2xgphenf3r0f5enpr3mx5dps559yxm", CoinType.OASIS)

        assertEquals("0x93d8f8a455f50527976a8aa87ebde38d5606efa86cb985d3fb466aff37000e3b", pubkey.data().toHex())
        assertEquals(expected.description(), address.description())
    }
}
