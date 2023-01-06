// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.multiversx

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestMultiversXAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    private var aliceBech32 = "erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th"
    private var alicePubKeyHex = "0x0139472eff6886771a982f3083da5d421f24c29181e63888228dc81ca60d69e1"
    private var aliceSeedHex = "0x413f42575f7f26fad3317a778771212fdb80245850981e48b58a4f25e344e8f9"

    @Test
    fun testAddressFromPrivateKey() {
        val key = PrivateKey(aliceSeedHex.toHexByteArray())
        val pubKey = key.publicKeyEd25519
        val address = AnyAddress(pubKey, CoinType.MULTIVERSX)

        assertEquals(alicePubKeyHex, pubKey.data().toHex())
        assertEquals(aliceBech32, address.description())
    }

    @Test
    fun testAddressFromPublicKey() {
        val pubKey = PublicKey(alicePubKeyHex.toHexByteArray(), PublicKeyType.ED25519)
        val address = AnyAddress(pubKey, CoinType.MULTIVERSX)

        assertEquals(aliceBech32, address.description())
    }

    @Test
    fun testAddressFromString() {
        val address = AnyAddress(aliceBech32, CoinType.MULTIVERSX)
        
        assertEquals(aliceBech32, address.description())
    }
}
