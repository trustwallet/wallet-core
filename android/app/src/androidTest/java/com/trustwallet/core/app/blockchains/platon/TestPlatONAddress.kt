// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.platon

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestPlatONAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("03a4130e4abb887a296eb38c15bbd83253ab09492a505b10a54b008b7dcc1668".toHexByteArray())
        val pubkey = key.getPublicKeySecp256k1(false)
        val address = AnyAddress(pubkey, CoinType.PLATON)
        val expected = AnyAddress("0xC115CEADF9e5923330e5f42903Fe7f926DDA65D2", CoinType.PLATON)

        assertEquals(pubkey.data().toHex(), "0x04013f756ad4762f5109dba9e652ae43905730c882a12dedc17edca788f51dcb3b8a74ce86d8b6674fa167798d30f1149d9ab86dffcc976278966b5584450107a6")
        assertEquals(address.description(), expected.description())
    }
}
