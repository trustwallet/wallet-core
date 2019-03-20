package com.trustwallet.core.app.blockchains

import wallet.core.jni.CoinType
import wallet.core.jni.Curve
import wallet.core.jni.Purpose
import org.junit.Assert.assertEquals
import org.junit.Assert.assertFalse
import org.junit.Test


class TestCoinType {
    init {
        System.loadLibrary("TrustWalletCore");
    }

    @Test
    fun testCoinPurpose() {
        assertEquals(Purpose.BIP84, CoinType.BITCOIN.purpose())
    }

    @Test
    fun testCoinCurve() {
        assertEquals(Curve.SECP256K1, CoinType.BITCOIN.curve())
    }
}
