package com.trustwallet.core.app.blockchains.polkadot

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.*
import org.junit.Test
import wallet.core.jni.PolkadotAddress
import wallet.core.jni.PrivateKey
import wallet.core.jni.PublicKey
import wallet.core.jni.PublicKeyType

class TestPolkadotAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testInvalidAddress() {
        assertFalse(PolkadotAddress.isValidString("aaa-aaa"))
        assertFalse(PolkadotAddress.isValidString("FM5N1JNemHSJAnW8V99ijj7ncHSQ5X9fDs38pqkZZfW62Pr"))
    }

    @Test
    fun testValidAddress() {
        assertTrue(PolkadotAddress.isValidString("DMZFfVn8yxNXroTXzuoRoVKnsitJJx4jnS3DoaficA4woPL"))
        assertTrue(PolkadotAddress.isValidString("FM4N1JNemHSJAnW8V99ijj7ncHSQ5X9fDs38pqkZZfW62Pr"))
    }

    @Test
    fun testAddressFromString() {
        val addrString = "DtbB3Uy8zX9khHRf7SsSVk7esY5AngKvC9EJVhpjSzza2MU"
        val address = PolkadotAddress(addrString)
        assertEquals(addrString, address.description())
    }

    @Test
    fun testAddressFromPublicKey() {
        val publicKey = PublicKey("5cc8df138a881319a75ab3a4f9621cf4fe614948f45a87118093edfab1679c64".toHexByteArray(), PublicKeyType.ED25519)
        val address = PolkadotAddress(publicKey)
        assertEquals("136f5StL4ANS6oAdjCWZzEhdCXPcd5P3ZB4LtHnFBbWYv27r", address.description())
    }

    @Test
    fun testAddressFromPrivateKey() {
        val privateKey = PrivateKey("ae92f15b17ad7a306f9e3e24d293d3988e625990c716ca099b623611695f02e0".toHexByteArray())
        val publicKey = privateKey.publicKeyEd25519
        val address = PolkadotAddress(publicKey)
        assertEquals("136f5StL4ANS6oAdjCWZzEhdCXPcd5P3ZB4LtHnFBbWYv27r", address.description())
    }
}
