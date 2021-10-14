// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.newchain

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestNewChainAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("ea2a7f292949079272bf082b1301ce31cf49015622d48f0036777848705f2650".toHexByteArray())
        val pubkey = key.publicKeyEd25519
        val address = AnyAddress(pubkey, CoinType.NEWCHAIN)
        val expected = AnyAddress("0x9DE95e3234410B89A363d3429d5Ca24D20Fa91Bc", CoinType.NEWCHAIN)
        assertEquals(address.description(), expected.description())
    }
}
