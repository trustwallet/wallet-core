// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.bluzelle

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestBluzelleAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressPublicKey() {

        val key = PrivateKey("1037f828ca313f4c9e120316e8e9ff25e17f07fe66ba557d5bc5e2eeb7cba8f6".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val expectedAddress = "bluzelle1jf9aaj9myrzsnmpdr7twecnaftzmku2myvn4dg"
        val actualAddress = AnyAddress(publicKey, CoinType.BLUZELLE).description()

        val expectedPublicKeyData = "0x035df185566521d6a7802319ee06e1a28e97b7772dfb5fdd13ca6f0575518968e4"
        val actualPublicKeyData = publicKey.data().toHex()

        assertEquals(expectedAddress, actualAddress)
        assertEquals(expectedPublicKeyData, actualPublicKeyData)
    }

    @Test
    fun testAddressValidation() {
        val addr = listOf(
            "bluzelle1yhtq5zm293m2r3sp2guj9m5pg5e273n6r0szul",
            "bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"
        )
        addr.forEach {
            assert(CoinType.BLUZELLE.validate(it))
            assertEquals(it, AnyAddress(it, CoinType.BLUZELLE).description())
        }
    }
}