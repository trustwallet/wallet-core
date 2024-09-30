// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.thetafuel

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestThetaFuelAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("4646464646464646464646464646464646464646464646464646464646464646".toHexByteArray())
        val pubkey = key.getPublicKeySecp256k1(false)
        val address = AnyAddress(pubkey, CoinType.THETAFUEL)
        val expected = AnyAddress("0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F", CoinType.THETAFUEL)

        assertEquals(address.description(), expected.description())
    }
}
