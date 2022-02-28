package com.trustwallet.core.app.utils

import wallet.core.jni.Mnemonic
import org.junit.Assert.assertTrue
import org.junit.Assert.assertFalse
import org.junit.Assert.assertEquals
import org.junit.Test


class TestMnemonic {
    init {
        System.loadLibrary("TrustWalletCore");
    }

    @Test
    fun testIsValid() {
        assertTrue(Mnemonic.isValid("credit expect life fade cover suit response wash pear what skull force"));
        assertFalse(Mnemonic.isValid("ripple scissors hisc mammal hire column oak again sun offer wealth tomorrow"));
    }

    @Test
    fun testIsWordValid() {
        assertTrue(Mnemonic.isValidWord("credit"));

        assertFalse(Mnemonic.isValidWord("di"));
        assertFalse(Mnemonic.isValidWord("cr"));
        assertFalse(Mnemonic.isValidWord("hybridous"));
    }

    @Test
    fun testSuggest() {
        assertEquals(Mnemonic.suggest("air"), "air airport")
        assertEquals(Mnemonic.suggest("rob"), "robot robust")
    }
}
