package com.trustwallet.core.app.blockchains.aeternity

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestAeternityAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressFromPublicKey() {
        val key = PublicKey("ee93a4f66f8d16b819bb9beb9ffccdfcdc1412e87fee6a324c2a99a1e0e67148".toHexByteArray(), PublicKeyType.ED25519)
        val address = AnyAddress(key, CoinType.AETERNITY)

        assertEquals(address.description(), "ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw")
    }

    @Test
    fun testAddressFromString() {
        val addressString = "ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw"
        val address = AnyAddress(addressString, CoinType.AETERNITY)
        assertEquals(address.description(), addressString)
    }
}

