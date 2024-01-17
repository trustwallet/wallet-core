// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.thorchain

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*
import wallet.core.jni.CoinType.THORCHAIN

class TestTHORChainAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressValidation() {
        var addr = listOf(
            "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r",
            "thor1c8jd7ad9pcw4k3wkuqlkz4auv95mldr2kyhc65"
        )
        addr.forEach {
            assert(CoinType.THORCHAIN.validate(it))
            assertEquals(it, AnyAddress(it, CoinType.THORCHAIN).description())
        }
    }
}
