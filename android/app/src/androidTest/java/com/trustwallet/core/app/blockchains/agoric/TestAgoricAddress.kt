// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.agoric

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestAgoricAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {

        val key = PrivateKey("037048190544fa57651452f477c096de4f3073e7835cf3845b04602563a73f73".toHexByteArray())
        val pubkey = key.getPublicKeySecp256k1(true)
        val address = AnyAddress(pubkey, CoinType.AGORIC)
        val expected = AnyAddress("agoric18zvvgk6j3eq5wd7mqxccgt20gz2w94cy88aek5", CoinType.AGORIC)

        assertEquals(pubkey.data().toHex(), "0x03df9a5e4089f89d45913fb2b856de984c7e8bf1344cc6444cc9705899a48c939d")
        assertEquals(address.description(), expected.description())
    }
}
