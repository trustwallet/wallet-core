// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.everscale

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestEverscaleAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f".toHexByteArray())
        val pubkey = key.publicKeyEd25519
        val address = AnyAddress(pubkey, CoinType.EVERSCALE)
        val expected = AnyAddress("0:269fee242eb410786abe1777a14785c8bbeb1e34100c7570e17698b36ad66fb0", CoinType.EVERSCALE)
    }
}
