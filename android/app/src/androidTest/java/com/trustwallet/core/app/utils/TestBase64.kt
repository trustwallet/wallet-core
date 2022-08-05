package com.trustwallet.core.app.utils

import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.Base64

class TestBase64 {
    init {
        System.loadLibrary("TrustWalletCore");
    }

    @Test
    fun testEncode() {
        assertEquals(Base64.encode("HelloWorld".toByteArray()), "SGVsbG9Xb3JsZA==")
    }

    @Test
    fun testDecode() {
        val decoded = Base64.decode("SGVsbG9Xb3JsZA==")
        assertEquals(String(decoded, Charsets.UTF_8), "HelloWorld")
    }

    @Test
    fun testEncodeUrl() {
        assertEquals(Base64.encodeUrl("+\\?ab".toByteArray()), "K1w_YWI=")
    }

    @Test
    fun testDecodeUrl() {
        val decoded = Base64.decodeUrl("K1w_YWI=")
        assertEquals(String(decoded, Charsets.UTF_8), "+\\?ab")
    }
}

