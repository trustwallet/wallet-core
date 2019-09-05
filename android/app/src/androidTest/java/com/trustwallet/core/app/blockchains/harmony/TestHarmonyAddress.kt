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

    val targetAddress = "one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe"

    @Test
    fun testAddressFromPrivateKey() {
         val key = PrivateKey(Base58.decodeNoCheck("GGzxJ4QmKCXH2juK89RVAmvFAfdUfUARCvxEsBM356vX"))
         val pubkey = key.getPublicKeySecp256k1(false)
         val address = HarmonyAddress(pubkey)
         assertEquals(address.description(), targetAddress)
    }

    @Test
    fun testAddressFromPublicKey() {
         val pubkey = PublicKey(
             Base58.decodeNoCheck("RKjfnr3wMojEruvXZuvNDmL7UfLUiyU3vsBGoZ4k2qY8YzoEJDHLmXDWid9K6YDuGJ2u1fZ8E8JXDjk3KUuDXtwz"),
             PublicKeyType.SECP256K1EXTENDED
         )
         val address = HarmonyAddress(pubkey)
    	 assertEquals(address.description(), targetAddress)
    }

    @Test
    fun testAddressFromString() {
         val address = HarmonyAddress(targetAddress)
         assertEquals(address.description(), targetAddress)
    }
}
