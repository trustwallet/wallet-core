// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.iost

import com.trustwallet.core.app.utils.toHex
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestIOSTAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val privateKeyBytes = Base58.decodeNoCheck("2Gk8MxhWJQhsLXfUH87iQAPdVWsZ8aUXHcJAst4tzGYCGBaqE2VMABcCQYd6WbFm5PWDDxLNAr2yaGkHRCJnsSfo").sliceArray(0..31)
        val key = PrivateKey(privateKeyBytes)
        val pubkey = key.getPublicKeyEd25519()
        val address = AnyAddress(pubkey, CoinType.IOST)
        val expected = AnyAddress("2QNt4yU73VfwLyGdKj5yirtRJn5Zy7pB3KAGyUiTSfmP", CoinType.IOST)

        assertEquals(pubkey.data().toHex(), "0x14d8c92fceefdc967f1f6cbace65a9f0740bfe45742a478cc0c04cca341cfb4e")
        assertEquals(address.description(), expected.description())
    }

    @Test
    fun testAddressFromString() {
        val addressString = "2QNt4yU73VfwLyGdKj5yirtRJn5Zy7pB3KAGyUiTSfmP"
        val address = AnyAddress(addressString, CoinType.IOST)
        assertEquals(address.description(), addressString)
    }
}