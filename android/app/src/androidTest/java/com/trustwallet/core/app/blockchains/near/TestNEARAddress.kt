package com.trustwallet.core.app.blockchains.solana

import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

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
        val address = AnyAddress(pubkey, CoinType.NEAR)

        assertEquals(address.description(), expectedAddressString)
    }

    @Test
    fun testAddressFromString() {
        val addressString = expectedAddressString
        val address = AnyAddress(addressString, CoinType.NEAR)
        assertEquals(address.description(), addressString)
    }
}
