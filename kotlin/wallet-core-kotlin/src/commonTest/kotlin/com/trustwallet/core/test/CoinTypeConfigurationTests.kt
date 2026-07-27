package com.trustwallet.core.test

import com.trustwallet.core.CoinType
import com.trustwallet.core.CoinTypeConfiguration
import com.trustwallet.core.LibLoader
import kotlin.test.Test
import kotlin.test.assertEquals
import kotlin.test.assertNotEquals

class CoinTypeConfigurationTests {

    init {
        LibLoader.loadLibrary()
    }

    @Test
    fun testNativeTokenNameDiffersFromNameForSmartChain() {
        val name = CoinTypeConfiguration.getName(CoinType.SmartChain)
        val nativeTokenName = CoinTypeConfiguration.getNativeTokenName(CoinType.SmartChain)

        assertEquals("BNB Smart Chain", name)
        assertEquals("BNB", nativeTokenName)
        assertNotEquals(name, nativeTokenName)
    }

    @Test
    fun testNativeTokenNameMatchesNameForBitcoin() {
        val name = CoinTypeConfiguration.getName(CoinType.Bitcoin)
        val nativeTokenName = CoinTypeConfiguration.getNativeTokenName(CoinType.Bitcoin)

        assertEquals(name, nativeTokenName)
    }
}
