// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.nativezetachain

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestNativeZetaChainAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("8d2a3bd62d300a148c89dc8635f87b7a24a951bd1c4e78675fe40e1a640d46ed".toHexByteArray())
        val pubKey = key.getPublicKeySecp256k1(false)
        val address = AnyAddress(pubKey, CoinType.NATIVEZETACHAIN)
        val expected = AnyAddress("zeta14py36sx57ud82t9yrks9z6hdsrpn5x6kmxs0ne", CoinType.NATIVEZETACHAIN)

        assertEquals(address.description(), expected.description())
    }
}
