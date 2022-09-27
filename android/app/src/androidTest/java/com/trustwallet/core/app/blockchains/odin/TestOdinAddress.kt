// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.odin

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestOdinAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("83e68457212d6026ec2fe18cf1ecb1ed2cb2aa8ecdec38d884c89c40dfb68f62".toHexByteArray())
        val pubkey = key.getPublicKeySecp256k1(true)
        val address = AnyAddress(pubkey, CoinType.ODIN)
        val expected = AnyAddress("odin1d224epchv57ftr6qwq98nzllfw9mmtzpfg4zla", CoinType.ODIN)

        assertEquals(pubkey.data().toHex(), "0x028edd997cd83ecc9f82c70483d116de363efa9ee5ba6888e8483b8c7c6679fc29")
        assertEquals(address.description(), expected.description())
    }
}
