// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.mayachain

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*
import wallet.core.jni.CoinType.MAYACHAIN

class TestMAYAChainAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressValidation() {
        var addr = listOf(
            "maya1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r",
            "maya1c8jd7ad9pcw4k3wkuqlkz4auv95mldr2kyhc65"
        )
        addr.forEach {
            assert(CoinType.MAYACHAIN.validate(it))
            assertEquals(it, AnyAddress(it, CoinType.MAYACHAIN).description())
        }
    }
}
