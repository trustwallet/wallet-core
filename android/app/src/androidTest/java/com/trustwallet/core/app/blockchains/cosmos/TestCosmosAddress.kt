package com.trustwallet.core.app.blockchains.cosmos

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexBytes
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*
import com.trustwallet.core.app.utils.toHex
import wallet.core.jni.CoinType.COSMOS
import wallet.core.java.AnySigner

class TestCosmosAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressValidation() {
        var addr = listOf(
            "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02",
            "cosmospub1addwnpepqftjsmkr7d7nx4tmhw4qqze8w39vjq364xt8etn45xqarlu3l2wu2n7pgrq",
            "cosmosvaloper1sxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08",
            "cosmosvalconspub1zcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa"
        )
        addr.forEach {
            assert(CoinType.COSMOS.validate(it))
            assertEquals(it, AnyAddress(it, CoinType.COSMOS).description())
        }
    }
}
