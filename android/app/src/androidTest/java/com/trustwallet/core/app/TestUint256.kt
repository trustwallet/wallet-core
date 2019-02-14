package com.trustwallet.core.app

import com.trustwallet.core.app.utils.Numeric
import com.wallet.crypto.trustapp.jni.UInt256
import org.junit.Assert.*
import org.junit.Test

class TestUint256 {
    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testStaticZero() {
        val zero = UInt256.zero()
        assertEquals(zero.uint32Value(), 0)
        assertEquals(zero.uint64Value(), 0)
        assertTrue(zero.isZero)
        assertEquals(Numeric.toHexString(zero.data()), "0x0000000000000000000000000000000000000000000000000000000000000000")
        assertEquals(zero.format(1), "0.0")
    }

    @Test
    fun testInstanceZero() {
        val zero = UInt256(0)
        assertEquals(zero.uint32Value(), 0)
        assertEquals(zero.uint64Value(), 0)
        assertTrue(zero.isZero)
        assertEquals(zero.format(1), "0.0")
        assertEquals(Numeric.toHexString(zero.data()), "0x0000000000000000000000000000000000000000000000000000000000000000")
    }

    @Test
    fun testInstanceTen() {
        val ten = UInt256(10)
        assertEquals(ten.uint32Value(), 10)
        assertEquals(ten.uint64Value(), 10)
        assertFalse(ten.isZero)
        assertEquals(ten.format(2), "0.1")
        assertEquals(ten.format(1), "1.0")
        assertEquals(Numeric.toHexString(ten.data()), "0x000000000000000000000000000000000000000000000000000000000000000a")
    }

    @Test
    fun testRound() {
        val num = UInt256(98756)
        assertEquals(num.uint32Value(), 98756)
        assertEquals(num.uint64Value(), 98756)
        assertFalse(num.isZero)
        assertEquals(num.format(5), "0.98756")
    }

    @Test
    fun testCompare() {
        var num1 = UInt256(98756)
        var num2 = UInt256(100)
        assertFalse(UInt256.equals(num1, num2))
        num2 = UInt256(98756)
        assertTrue(UInt256.equals(num1, num2))
        num2 = UInt256(100000)
        assertTrue(UInt256.less(num1, num2))
    }
}