package com.trustwallet.core.app.utils

import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.BitcoinScript


class TestBitcoinScript {

    @Test
    fun testBuildPayToPublicKeyHash() {
        val data = Numeric.hexStringToByteArray("cf5007e19af3641199f21f3fa54dff2fa2627471")
        val script = BitcoinScript.buildPayToPublicKeyHash(data)
        assertEquals(Numeric.toHexString(script.data()), "0x76a914cf5007e19af3641199f21f3fa54dff2fa262747188ac")
    }

    @Test
    fun testConstructorBuild() {
        val pk = Numeric.hexStringToByteArray("cf5007e19af3641199f21f3fa54dff2fa2627471")
        val script = BitcoinScript.buildPayToPublicKeyHash(pk)
        val scriptHash = script.data()
        assertEquals(Numeric.toHexString(scriptHash), "0x76a914cf5007e19af3641199f21f3fa54dff2fa262747188ac")
        assertEquals(Numeric.toHexString(script.scriptHash()), "0xc470d22e69a2a967f2cec0cd5a5aebb955cdd395")
    }

    @Test
    fun testRedeemScript() {
        val data = Numeric.hexStringToByteArray("cf5007e19af3641199f21f3fa54dff2fa2627471")
        val script = BitcoinScript.buildPayToPublicKeyHash(data)
        val scriptHash = script.scriptHash()
        val scriptScript = BitcoinScript.buildPayToScriptHash(scriptHash)
        assertEquals(Numeric.toHexString(scriptScript.data()), "0xa914c470d22e69a2a967f2cec0cd5a5aebb955cdd39587")
    }
}