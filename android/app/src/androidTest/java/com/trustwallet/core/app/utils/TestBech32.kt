package com.trustwallet.core.app.utils

import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.Bech32

class TestBech32 {
    init {
        System.loadLibrary("TrustWalletCore");
    }

    @Test
    fun testEncode() {
        val data = Numeric.hexStringToByteArray("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f")
        assertEquals(Bech32.encode("abcdef", data, "abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw")
    }

    @Test
    fun testDecode() {
        val decoded = Bech32.decode("abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw")
        assertEquals(Numeric.toHexString(decoded), "0x000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f")
    }
}
