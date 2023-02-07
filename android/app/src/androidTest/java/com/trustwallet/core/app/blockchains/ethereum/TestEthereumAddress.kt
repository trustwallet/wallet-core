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
    fun testEthereumAddresses() {
        val any = AnyAddress("0x7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1", CoinType.ETHEREUM)
        assertEquals(any.coin(), CoinType.ETHEREUM)
        assertEquals(any.description(), "0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1")

        assertFalse(AnyAddress.isValid("0xMQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4", CoinType.ETHEREUM))
    }

    @Test
    fun testEthereumEIP4337DeploymentAddress() {
        val factoryAddress = "0xd9145CCE52D386f254917e481eB44e9943F39138"
        val logicAddress = "0x5C9eb5D6a6C2c1B3EFc52255C0b356f116f6f66D"
        val ownerAddress = "0xA5a1dddEF094095AfB7b6e322dE72961DF2e1988"
        val result = Ethereum.eip4337GetDeploymentAddress(factoryAddress, logicAddress, ownerAddress)
        assertEquals(result, "0xbEaA87cEEaC906C21aaacd258FbFB87CfA3c90a8")
    }
}
