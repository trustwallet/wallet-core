// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.neo

import com.trustwallet.core.app.utils.Numeric
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*
import wallet.core.jni.AnyAddress

class TestNEOAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressFromPublicKey() {
        val pubkey = PublicKey(Numeric.hexStringToByteArray("0222b2277d039d67f4197a638dd5a1d99c290b17aa8c4a16ccee5165fe612de66a"), PublicKeyType.NIST256P1)
        val address = AnyAddress(pubkey, CoinType.NEO)
        val expectedAddressString = "AKmrAHRD9ZDUnu4m3vWWonpsojo4vgSuqp"
        assertEquals(address.description(), expectedAddressString)
    }

    @Test
    fun testAddressFromString() {
        val addressString = "AXkgwcMJTy9wTAXHsbyhauxh7t2Tt31MmC"
        val address = AnyAddress(addressString, CoinType.NEO)
        assertEquals(address.description(), addressString)
    }

    @Test
    fun testAddressFromPrivateKey() {
        val key = PrivateKey(Numeric.hexStringToByteArray("2A9EAB0FEC93CD94FA0A209AC5604602C1F0105FB02EAB398E17B4517C2FFBAB"))
        val pubkey = key.publicKeyNist256p1
        val address = AnyAddress(pubkey, CoinType.NEO)
        val expectedAddressString = "AQCSMB3oSDA1dHPn6GXN6KB4NHmdo1fX41"

        assertEquals(address.description(), expectedAddressString)
    }
}
