// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.utils

import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import wallet.core.jni.*
import java.security.InvalidParameterException
import org.junit.Assert.assertEquals
import org.junit.Assert.assertFalse
import org.junit.Assert.assertTrue
import org.junit.Assert.fail
import org.junit.Test

class TestAnyAddress {
    init {
        System.loadLibrary("TrustWalletCore");
    }

    val any_address_test_address = "bc1qcj2vfjec3c3luf9fx9vddnglhh9gawmncmgxhz"
    val any_address_test_pubkey = "02753f5c275e1847ba4d2fd3df36ad00af2e165650b35fe3991e9c9c46f68b12bc"

    @Test
    fun testCreateWithString() {
        val coin = CoinType.BITCOIN
        val address = AnyAddress(any_address_test_address, coin)
        assertEquals(address.coin(), coin)
        assertEquals(address.description(), any_address_test_address)
    }

    @Test
    fun testCreateWithStringBech32() {
        val coin = CoinType.BITCOIN
        val address1 = AnyAddress(any_address_test_address, coin, "bc")
        assertEquals(address1.description(), any_address_test_address)

        val address2 = AnyAddress("tb1qcj2vfjec3c3luf9fx9vddnglhh9gawmnjan4v3", coin, "tb")
        assertEquals(address2.description(), "tb1qcj2vfjec3c3luf9fx9vddnglhh9gawmnjan4v3")
    }

    @Test
    fun testCreateWithPublicKey() {
        val coin = CoinType.BITCOIN
        val pubkey = PublicKey(any_address_test_pubkey.toHexByteArray(), PublicKeyType.SECP256K1)
        val address = AnyAddress(pubkey, coin)
        assertEquals(address.description(), any_address_test_address)
    }

    @Test
    fun testCreateWithPublicKeyDerivation() {
        val coin = CoinType.BITCOIN
        val pubkey = PublicKey(any_address_test_pubkey.toHexByteArray(), PublicKeyType.SECP256K1)
        val address1 = AnyAddress(pubkey, coin, Derivation.BITCOINSEGWIT)
        assertEquals(address1.description(), any_address_test_address)

        val address2 = AnyAddress(pubkey, coin, Derivation.BITCOINLEGACY)
        assertEquals(address2.description(), "1JvRfEQFv5q5qy9uTSAezH7kVQf4hqnHXx")
    }

    @Test
    fun testCreateBech32WithPublicKey() {
        val coin = CoinType.BITCOIN
        val pubkey = PublicKey(any_address_test_pubkey.toHexByteArray(), PublicKeyType.SECP256K1)
        val address1 = AnyAddress(pubkey, coin, "bc")
        assertEquals(address1.description(), any_address_test_address)

        val address2 = AnyAddress(pubkey, coin, "tb")
        assertEquals(address2.description(), "tb1qcj2vfjec3c3luf9fx9vddnglhh9gawmnjan4v3")
    }

    @Test
    fun testIsValid() {
        val coin = CoinType.BITCOIN
        assertTrue(AnyAddress.isValid(any_address_test_address, coin));
        assertFalse(AnyAddress.isValid(any_address_test_address, CoinType.ETHEREUM));
        assertFalse(AnyAddress.isValid("__INVALID_ADDRESS__", CoinType.ETHEREUM));
    }

    @Test
    fun testIsValidBech32() {
        val coin = CoinType.BITCOIN
        assertTrue(AnyAddress.isValidBech32(any_address_test_address, coin, "bc"));
        assertFalse(AnyAddress.isValidBech32(any_address_test_address, coin, "tb"));
        assertTrue(AnyAddress.isValidBech32("tb1qcj2vfjec3c3luf9fx9vddnglhh9gawmnjan4v3", coin, "tb"));
        assertFalse(AnyAddress.isValidBech32("tb1qcj2vfjec3c3luf9fx9vddnglhh9gawmnjan4v3", coin, "bc"));
    }
}
