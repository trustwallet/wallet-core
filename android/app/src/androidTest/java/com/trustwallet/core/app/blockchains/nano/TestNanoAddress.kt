package com.trustwallet.core.app.blockchains.nano

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PublicKey
import wallet.core.jni.PublicKeyType
import wallet.core.jni.NanoAddress

class TestNanoAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressFromPublicKey() {
        val pubkey = PublicKey(("5114aad86a390897d2a91b33b931b3a59a7df9e63eb3694f9430122f5622ae50").toHexByteArray(), PublicKeyType.ED25519BLAKE2B)
        val address = NanoAddress(pubkey)

        assertEquals(address.description(), "nano_1nanode8ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg")
    }
}
