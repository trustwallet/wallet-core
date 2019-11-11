package com.trustwallet.core.app.blockchains.samplecoin

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.SampleCoinAddress
import wallet.core.jni.PrivateKey
import wallet.core.jni.PublicKey
import wallet.core.jni.PublicKeyType

class TestSampleCoinAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        // TODO: Check and finalize implementation

        val key = PrivateKey("__PRIVATE_KEY_DATA__".toHexByteArray())
        val pubkey = key.publicKeyEd25519
        val address = SampleCoinAddress(pubkey)
        val expected = SampleCoinAddress("__EXPECTED_RESULT_ADDRESS__")

        assertEquals(pubkey.data().toHex(), "0x__EXPECTED_PUBKEY_DATA__")
        assertEquals(address.description(), expected.description())
    }
}
