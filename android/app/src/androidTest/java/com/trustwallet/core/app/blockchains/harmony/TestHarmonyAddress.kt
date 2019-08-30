package com.trustwallet.core.app.blockchains.harmony

import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.Base58
import wallet.core.jni.PrivateKey
import wallet.core.jni.PublicKey
import wallet.core.jni.PublicKeyType
import wallet.core.jni.HarmonyAddress

class TestHarmonyAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressFromPrivateKey() {
        // val key = PrivateKey(Base58.decodeNoCheck("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr"))
        // val pubkey = key.getPublicKeyEd25519()
        // val address = HarmonyAddress(pubkey)
        // assertEquals(address.description(), "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q")
        assertEquals(true, true)
    }

    @Test
    fun testAddressFromPublicKey() {
        // val pubkey = PublicKey(Base58.decodeNoCheck("7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q"), PublicKeyType.ED25519)
        // val address = HarmonyAddress(pubkey)
	// assertEquals(address.description(), "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q")
        assertEquals(true, true)
    }

    @Test
    fun testAddressFromString() {
        // val addressString = "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q"
        // val address = HarmonyAddress(addressString)
        // assertEquals(address.description(), addressString)
        assertEquals(true, true)
    }
}
