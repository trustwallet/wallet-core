// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.secret

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestSecretAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("87201512d132ef7a1e57f9e24905fbc24300bd73f676b5716182be5f3e39dada".toHexByteArray())
        val pubkey = key.getPublicKeySecp256k1(true)
        val address = AnyAddress(pubkey, CoinType.SECRET)
        val expected = AnyAddress("secret18mdrja40gfuftt5yx6tgj0fn5lurplezyp894y", CoinType.SECRET)

        assertEquals(pubkey.data().toHex(), "0x02466ac5d28cb4fab6c349060c6c1619e8d301e7741fb6b33cc1edac25f45d8646")
        assertEquals(address.description(), expected.description())
    }
}
