// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.polygon

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestPolygonAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        // TODO: Check and finalize implementation

        val key = PrivateKey("__PRIVATE_KEY_DATA__".toHexByteArray())
        val pubkey = key.publicKeyEd25519
        val address = AnyAddress(pubkey, CoinType.POLYGON)
        val expected = AnyAddress("__EXPECTED_RESULT_ADDRESS__", CoinType.POLYGON)

        assertEquals(pubkey.data().toHex(), "0x__EXPECTED_PUBKEY_DATA__")
        assertEquals(address.description(), expected.description())
    }
}
