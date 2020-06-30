// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.xinfin

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestXinFinAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {

        val key = PrivateKey("861932c871f8a5181cfaa54ba6dbbd9f7fb9d41925021246c10c0d28f3b661c0".toHexByteArray())
        val pubkey = key.publicKeyEd25519
        val address = AnyAddress(pubkey, CoinType.XINFIN)
        val expected = AnyAddress("xdccf808d69db8ddc8d5fc90bf2586074f328058082", CoinType.XINFIN)

        assertEquals(pubkey.data().toHex(), "xdc__EXPECTED_PUBKEY_DATA__")
        assertEquals(address.description(), expected.description())
    }
}
