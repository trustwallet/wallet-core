// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.kcc

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestKuCoinCommunityChainAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }
    
    @Test
    fun testAddress() {
        val key = PrivateKey("33b85056aabab539bcb68540735ecf054e38bc58b29b751530e2b54ecb4ca564".toHexByteArray())
        val pubkey = key.getPublicKeySecp256k1(false)
        val address = AnyAddress(pubkey, CoinType.KUCOINCOMMUNITYCHAIN)
        val expected = AnyAddress("0xE5cA667d795685E9915E5F4b4254ca832eEB398B", CoinType.KUCOINCOMMUNITYCHAIN)

        assertEquals(pubkey.data().toHex(), "0x0413bde18e3329af54d51a24f424fe09a8d7d42c324c07e10e53a6e139cbee80e6288142dec2ed46f7b81dccbb28d6168cdc7b208928730cbeeb911f8db6a707bb")
        assertEquals(address.description(), expected.description())
    }
}
