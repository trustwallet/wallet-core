package com.trustwallet.core.app.blockchains.solana

import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestNEARAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    private val expectedAddressString = "917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d"

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
