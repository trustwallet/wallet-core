// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.crypto_org

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestCrypto_orgAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        // TODO: Check and finalize implementation

        val key = PrivateKey("d49a797db63b61c148d1767e616ff15c3dd9c83b63cba61c218ea172f238622a".toHexByteArray())
        val pubkey = key.publicKeyEd25519
        val address = AnyAddress(pubkey, CoinType.CRYPTO_ORG)
        val expected = AnyAddress("cro1u9q8mfpzhyv2s43js7l5qseapx5kt3g2rf7ppf", CoinType.CRYPTO_ORG)

        assertEquals(pubkey.data().toHex(), "027b4bf94c4cc89f4d67d0c62e9dafedaddd952c6235731a22dc8e5d2470f22622")
        assertEquals(address.description(), expected.description())
    }
}
