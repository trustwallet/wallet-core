package com.trustwallet.core.app.blockchains.solana

import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.Base58
import wallet.core.jni.PrivateKey
import wallet.core.jni.PublicKey
import wallet.core.jni.PublicKeyType
import wallet.core.jni.SolanaAddress

class TestSolanaAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    private val expectedAddressString = "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q"

    @Test
    fun testAddressFromPrivateKey() {
        val key = PrivateKey(Base58.decodeNoCheck("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr"))
        val pubkey = key.getPublicKeyEd25519()
        val address = SolanaAddress(pubkey)

        assertEquals(address.description(), expectedAddressString)
    }

    @Test
    fun testAddressFromPublicKey() {
        val pubkey = PublicKey(Base58.decodeNoCheck(expectedAddressString), PublicKeyType.ED25519)
        val address = SolanaAddress(pubkey)

        assertEquals(address.description(), expectedAddressString)
    }

    @Test
    fun testAddressFromString() {
        val addressString = expectedAddressString
        val address = SolanaAddress(addressString)
        assertEquals(address.description(), addressString)
    }
}
