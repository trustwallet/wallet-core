package com.trustwallet.core.app.blockchains.waves

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressFromPrivateKey() {
        val key = PrivateKey("9864a747e1b97f131fabb6b447296c9b6f0201e79fb3c5356e6c77e89b6a806a".toHexByteArray())
        val pubkey = key.publicKeyCurve25519
        val address = AnyAddress(pubkey, CoinType.WAVES)

        assertEquals(pubkey.data().toHex(), "0x559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d".lowercase())
        assertEquals(address.description(), "3P2uzAzX9XTu1t32GkWw68YFFLwtapWvDds")
    }

    @Test
    fun testAddressFromPublicKey() {
        val pubkey = PublicKey("559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d".toHexByteArray(), PublicKeyType.CURVE25519)
        val address = AnyAddress(pubkey, CoinType.WAVES)

        assertEquals(address.description(), "3P2uzAzX9XTu1t32GkWw68YFFLwtapWvDds")
    }
}
