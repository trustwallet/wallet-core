// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
        assertEquals(address.description(), "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV")
    }

    @Test
    fun testAddressFromPublicKey() {
        val publicKey = PublicKey("f42c77f931bea20ec5d0150731276bbb2e2860947661245b2319ef8133ee8d41".toHexByteArray(), PublicKeyType.ED25519)
        val address = AnyAddress(publicKey, CoinType.TON)
        assertEquals(address.description(), "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV")
    }

    @Test
    fun testAddressFromRawString() {
        val addressString = "0:66fbe3c5c03bf5c82792f904c9f8bf28894a6aa3d213d41c20569b654aadedb3"
        val address = AnyAddress(addressString, CoinType.TON)
        assertEquals(address.description(), "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV")
    }

    @Test
    fun testAddressFromBounceableString() {
        val addressString = "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q"
        val address = AnyAddress(addressString, CoinType.TON)
        assertEquals(address.description(), "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV")
    }

    @Test
    fun testAddressFromUserFriendlyString() {
        val addressString = "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV"
        val address = AnyAddress(addressString, CoinType.TON)
        assertEquals(address.description(), "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV")
    }

    @Test
    fun testAddressToBounceable() {
        val addressString = "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV"
        val bounceable = true
        val testnet = false
        val address = TONAddressConverter.toUserFriendly(addressString, bounceable, testnet)
        assertEquals(address, "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q")
    }

    @Test
    fun testGenerateJettonAddress() {
        val mainAddress = "UQBjKqthWBE6GEcqb_epTRFrQ1niS6Z1Z1MHMwR-mnAYRoYr"
        val mainAddressBoc = TONAddressConverter.toBoc(mainAddress)
        assertEquals(mainAddressBoc, "te6cckEBAQEAJAAAQ4AMZVVsKwInQwjlTf71KaItaGs8SXTOrOpg5mCP004DCNAptHQU")

        // curl --location 'https://toncenter.com/api/v2/runGetMethod' --header 'Content-Type: application/json' --data \
        // '{"address":"EQAvlWFDxGF2lXm67y4yzC17wYKD9A0guwPkMs1gOsM__NOT","method":"get_wallet_address","method":"get_wallet_address","stack":[["tvm.Slice","te6ccgICAAEAAQAAACQAAABDgAxlVWwrAidDCOVN/vUpoi1oazxJdM6s6mDmYI/TTgMI0A=="]]}'

        // Parse the `get_wallet_address` RPC response.
        val jettonAddressBocEncoded = "te6cckEBAQEAJAAAQ4AFvT5rqwxcbKfITqnkwL+go4Zi9bulRHAtLt4cjjFdK7B8L+Cq"
        val jettonAddress = TONAddressConverter.fromBoc(jettonAddressBocEncoded)
        assertEquals(jettonAddress, "UQAt6fNdWGLjZT5CdU8mBf0FHDMXrd0qI4FpdvDkcYrpXV5H")
    }
}
