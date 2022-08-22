package com.trustwallet.core.app.utils

import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.Base32

class TestBase32 {
    init {
        System.loadLibrary("TrustWalletCore");
    }

    @Test
    fun testEncode() {
        assertEquals(Base32.encode("HelloWorld".toByteArray()), "JBSWY3DPK5XXE3DE")
    }

    @Test
    fun testEncodeWithAlphabet() {
        assertEquals(Base32.encodeWithAlphabet("7uoq6tp427uzv7fztkbsnn64iwotfrristwpryy".toByteArray(), "abcdefghijklmnopqrstuvwxyz234567"), "g52w64jworydimrxov5hmn3gpj2gwyttnzxdmndjo5xxiztsojuxg5dxobzhs6i")
    }

    @Test
    fun testDecode() {
        var decoded = Base32.decode("JBSWY3DPK5XXE3DE")

        assertEquals(String(decoded, Charsets.UTF_8), "HelloWorld")
    }

    @Test
    fun testDecodeWithAlphabet() {
        var decoded = Base32.decodeWithAlphabet("g52w64jworydimrxov5hmn3gpj2gwyttnzxdmndjo5xxiztsojuxg5dxobzhs6i", "abcdefghijklmnopqrstuvwxyz234567")

        assertEquals(String(decoded, Charsets.UTF_8), "7uoq6tp427uzv7fztkbsnn64iwotfrristwpryy")
    }
}
