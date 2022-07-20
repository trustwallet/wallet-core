// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.energywebtoken

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestEnergyWebTokenAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("d429aff03fce610ca3530f08158a96c77f684a1eae39eab5e78b28c46fe4d92c".toHexByteArray())
        val pubkey = key.getPublicKeySecp256k1(false)
        val address = AnyAddress(pubkey, CoinType.ENERGYWEBTOKEN)
        val expected = AnyAddress("0x0C6365602b334158C32b1fa6EEE7A210304960EA", CoinType.ENERGYWEBTOKEN)

        assertEquals(address.description(), expected.description())
    }
}
