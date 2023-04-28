// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.nativeinjective

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestNativeInjectiveAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("9ee18daf8e463877aaf497282abc216852420101430482a28e246c179e2c5ef1".toHexByteArray())
        val pubKey = key.getPublicKeySecp256k1(false)
        val address = AnyAddress(pubKey, CoinType.NATIVEINJECTIVE)
        val expected = AnyAddress("inj13u6g7vqgw074mgmf2ze2cadzvkz9snlwcrtq8a", CoinType.NATIVEINJECTIVE)

        assertEquals(address.description(), expected.description())
    }
}
