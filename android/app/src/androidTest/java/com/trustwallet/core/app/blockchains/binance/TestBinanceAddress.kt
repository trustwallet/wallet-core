package com.trustwallet.core.app.blockchains.binance

import com.trustwallet.core.app.utils.toHexBytes
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*
import com.trustwallet.core.app.utils.toHex
import wallet.core.jni.CoinType.BINANCE
import wallet.core.java.AnySigner

class TestBinanceAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val publicKey = PublicKey("026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502".toHexBytes(), PublicKeyType.SECP256K1)
        val address = AnyAddress(publicKey, CoinType.BINANCE)

        assertEquals("bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2", address.description())
    }

    @Test
    fun testBinanceMainnet() {
        val wallet = HDWallet("rabbit tilt arm protect banner ill produce vendor april bike much identify pond upset front easily glass gallery address hair priority focus forest angle", "")
        val key = wallet.getKeyForCoin(CoinType.BINANCE)
        val address = CoinType.BINANCE.deriveAddress(key)

        assertEquals("0x727f677b390c151caf9c206fd77f77918f56904b5504243db9b21e51182c4c06", key.data().toHex())
        assertEquals("bnb1devga6q804tx9fqrnx0vtu5r36kxgp9tmk4xkm", address)
    }
}
