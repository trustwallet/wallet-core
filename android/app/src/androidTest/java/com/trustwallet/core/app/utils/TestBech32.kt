package com.trustwallet.core.app.utils

import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.Bech32

class TestBech32 {
    init {
        System.loadLibrary("TrustWalletCore");
    }

    @Test
    fun testEncode() {
        val data = Numeric.hexStringToByteArray("00443214c74254b635cf84653a56d7c675be77df")
        assertEquals(Bech32.encode("abcdef", data), "abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw")
    }

    @Test
    fun testDecode() {
        val decoded = Bech32.decode("abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw")
        assertEquals(Numeric.toHexString(decoded), "0x00443214c74254b635cf84653a56d7c675be77df")
    }

    @Test
    fun testDecodeWrongChecksumVariant() {
        // This is a Bech32m variant, not Bech32 variant. So it should fail using Bech32 decoder.
        val decoded = Bech32.decode("abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx")
        assertNull(decoded)
    }

    @Test
    fun testEncodeM() {
        val data = Numeric.hexStringToByteArray("ffbbcdeb38bdab49ca307b9ac5a928398a418820")
        assertEquals(Bech32.encodeM("abcdef", data), "abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx")
    }

    @Test
    fun testDecodeM() {
        val decoded = Bech32.decodeM("abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx")
        assertEquals(Numeric.toHexString(decoded), "0xffbbcdeb38bdab49ca307b9ac5a928398a418820")
    }

    @Test
    fun testDecodeMWrongChecksumVariant() {
        // This is a Bech32 variant, not Bech32m variant. So it should fail using Bech32M decoder.
        val decoded = Bech32.decodeM("abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw")
        assertNull(decoded)
    }
}
