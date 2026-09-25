package com.trustwallet.core.app.blockchains.ethereum

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import com.trustwallet.core.app.utils.Numeric
import wallet.core.jni.Eip7702

class TestEip7702 {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAuthorizationHash() {
        val chainId = "0x01".toHexByteArray()
        val contractAddress = "0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1"
        val nonce = "0x01".toHexByteArray()
        
        val authorizationHash = Eip7702.getAuthorizationHash(chainId, contractAddress, nonce)
        assertEquals(Numeric.toHexString(authorizationHash), "0x3ae543b2fa103a39a6985d964a67caed05f6b9bb2430ad6d498cda743fe911d9") // Verified with viem
    }

    @Test
    fun testSignAuthorization() {
        val chainId = "0x01".toHexByteArray()
        val contractAddress = "0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1"
        val nonce = "0x01".toHexByteArray()
        val privateKey = "0x947dd69af402e7f48da1b845dfc1df6be593d01a0d8274bd03ec56712e7164e8"
        
        val signedAuthorization = Eip7702.signAuthorization(chainId, contractAddress, nonce, privateKey)
        val json = org.json.JSONObject(signedAuthorization)
        
        // Verified with viem
        assertEquals(Numeric.toHexString(chainId), json.getString("chainId"))
        assertEquals(contractAddress, json.getString("address"))
        assertEquals(Numeric.toHexString(nonce), json.getString("nonce"))
        assertEquals("0x01", json.getString("yParity"))
        assertEquals("0x2c39f2f64441dd38c364ee175dc6b9a87f34ca330bce968f6c8e22811e3bb710", json.getString("r"))
        assertEquals("0x5f1bcde93dee4b214e60bc0e63babcc13e4fecb8a23c4098fd89844762aa012c", json.getString("s"))
    }
}
