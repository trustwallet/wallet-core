package com.trustwallet.core.app.blockchains.nebulas

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestNebulasAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressFromString() {
        var addrStr = "n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY"
        var addrStr2 = "n1zUNqeBPvsyrw5zxp9mKcDdLTjuaEL7s39"
        
        val address = AnyAddress(addrStr, CoinType.NEBULAS)
        assertEquals(address.description(), addrStr)

        val addressOfATP = AnyAddress(addrStr2, CoinType.NEBULAS)
        assertEquals(addressOfATP.description(), addrStr2)
    }

    @Test
    fun testAddressFromPublicKey() {
        var priKey = PrivateKey(("d2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b").toHexByteArray())
        val pubkey = priKey.getPublicKeySecp256k1(false)
        val address = AnyAddress(pubkey, CoinType.NEBULAS)

        assertEquals(address.description(), "n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY")
    }
}
