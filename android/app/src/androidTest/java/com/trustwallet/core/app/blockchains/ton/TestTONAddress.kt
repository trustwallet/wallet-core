// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.ton

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestTONAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val pubkey = PublicKey("F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3".toHexByteArray(), PublicKeyType.ED25519)
        val address = AnyAddress(pubkey, CoinType.TON)
        val expected = AnyAddress("EQAkAJCrZkWb9uYePf1D97nB8efUvYHTsqSscyPMGpcHUx3Y", CoinType.TON)

        assertEquals(address.description(), expected.description())
    }
}
