// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.theopennetwork

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestTheOpenNetworkAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressFromPrivateKey() {
        val privateKey = PrivateKey("63474e5fe9511f1526a50567ce142befc343e71a49b865ac3908f58667319cb8".toHexByteArray())
        val publicKey = privateKey.getPublicKeyEd25519()
        val address = AnyAddress(publicKey, CoinType.TON)
        assertEquals(publicKey.data().toHex(), "0xf42c77f931bea20ec5d0150731276bbb2e2860947661245b2319ef8133ee8d41")
        assertEquals(address.description(), "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q")
    }

    @Test
    fun testAddressFromPublicKey() {
        val publicKey = PublicKey("f42c77f931bea20ec5d0150731276bbb2e2860947661245b2319ef8133ee8d41".toHexByteArray(), PublicKeyType.ED25519)
        val address = AnyAddress(publicKey, CoinType.TON)
        assertEquals(address.description(), "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q")
    }

    @Test
    fun testAddressFromRawString() {
        val addressString = "0:66fbe3c5c03bf5c82792f904c9f8bf28894a6aa3d213d41c20569b654aadedb3"
        val address = AnyAddress(addressString, CoinType.TON)
        assertEquals(address.description(), "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q")
    }

    @Test
    fun testAddressFromUserFriendlyString() {
        val addressString = "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q"
        val address = AnyAddress(addressString, CoinType.TON)
        assertEquals(address.description(), "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q")
    }
}
