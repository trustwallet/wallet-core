// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.osmosis

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestOsmosisAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af".toHexByteArray())
        val pubkey = key.getPublicKeySecp256k1(true)
        val address = AnyAddress(pubkey, CoinType.OSMOSIS)
        val expected = AnyAddress("osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f5", CoinType.OSMOSIS)

        assertEquals(pubkey.data().toHex(), "0x02ecef5ce437a302c67f95468de4b31f36e911f467d7e6a52b41c1e13e1d563649")
        assertEquals(address.description(), expected.description())
    }
}
