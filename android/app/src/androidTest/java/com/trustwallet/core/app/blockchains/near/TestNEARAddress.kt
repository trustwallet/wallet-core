package com.trustwallet.core.app.blockchains.solana

import com.google.protobuf.ByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.Base58
import wallet.core.jni.PrivateKey
import wallet.core.jni.PublicKey
import wallet.core.jni.PublicKeyType
import wallet.core.jni.NEARAddress

class TestNEARAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    private val expectedAddressString = "NEAR2758Nk7CMUcxTwXdjVdSxNEidiZQWMZN3USJzj76q5ia3v2v2v"

    @Test
    fun testAddressFromPrivateKey() {
        val privateKeyBytes = Base58.decodeNoCheck("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv").sliceArray(0..31)
        val key = PrivateKey(privateKeyBytes)
        val pubkey = key.getPublicKeyEd25519()
        val address = NEARAddress(pubkey)

        assertEquals(address.description(), expectedAddressString)
    }

    @Test
    fun testAddressFromString() {
        val addressString = expectedAddressString
        val address = NEARAddress(addressString)
        assertEquals(address.description(), addressString)
    }
}
