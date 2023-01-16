package com.trustwallet.core.app.blockchains.ethereum

import com.trustwallet.core.app.utils.Numeric
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.AnyAddress
import wallet.core.jni.CoinType
import org.junit.Assert.assertFalse
import wallet.core.jni.Ethereum
import wallet.core.jni.Hash

class TestEthereumAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testEthereumCreate2Addresses() {
        val from = "0x0000000000000000000000000000000000000000"
        val salt = Numeric.hexStringToByteArray("0x0000000000000000000000000000000000000000000000000000000000000000")
        val initCodeHash = Hash.keccak256(Numeric.hexStringToByteArray("0x0"))
        val result = Ethereum.eip1014AddressCreate2(from, salt, initCodeHash)
        assertEquals(result, "0x4D1A2e2bB4F88F0250f26Ffff098B0b30B26BF38")
    }

    @Test
    fun testEthereumAddresses() {
        val any = AnyAddress("0x7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1", CoinType.ETHEREUM)
        assertEquals(any.coin(), CoinType.ETHEREUM)
        assertEquals(any.description(), "0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1")

        assertFalse(AnyAddress.isValid("0xMQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4", CoinType.ETHEREUM))
    }
}
