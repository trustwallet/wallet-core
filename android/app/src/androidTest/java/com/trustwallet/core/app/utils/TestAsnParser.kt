// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.utils

import wallet.core.jni.AsnParser
import org.junit.Assert.assertEquals
import org.junit.Test

class TestAsnParser {
    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testEcdsaSignatureFromDer() {
        val encoded = "3046022100db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495da022100ff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1"
        val expected = "0xdb421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495daff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1"
        val actual = AsnParser.ecdsaSignatureFromDer(encoded.toHexBytes())
        assertEquals(actual.toHex(), expected)
    }
}