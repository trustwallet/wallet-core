// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.avalanche

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestAvalancheAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("0x98cb077f972feb0481f1d894f272c6a1e3c15e272a1658ff716444f465200070".toHexByteArray())
        val pubkey = key.publicKeyEd25519
        val address = AnyAddress(pubkey, CoinType.AVALANCHEXCHAIN)
        val expected = AnyAddress("X-avax1apmh7wxg3js48fhacfv5y9md9065jxuft30vup", CoinType.AVALANCHEXCHAIN)

        assertEquals(pubkey.data().toHex(), "0x57c66cf849b2186cb52398caf8cb9cd390aac3e217e9b66bab9602faff715098")
        assertEquals(address.description(), expected.description())
    }
}
