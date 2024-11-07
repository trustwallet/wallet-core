package com.trustwallet.core.app.utils

import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.Bech32m

class TestBech32m {
    init {
        System.loadLibrary("TrustWalletCore");
    }

    @Test
    fun testEncode() {
        val data = Numeric.hexStringToByteArray("1f1e1d1c1b1a191817161514131211100f0e0d0c0b0a09080706050403020100")
        assertEquals(Bech32m.encode("abcdef", data, "abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx")
    }

    @Test
    fun testDecode() {
        val decoded = Bech32m.decode("abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx")
        assertEquals(Numeric.toHexString(decoded), "0x1f1e1d1c1b1a191817161514131211100f0e0d0c0b0a09080706050403020100")
    }
}
