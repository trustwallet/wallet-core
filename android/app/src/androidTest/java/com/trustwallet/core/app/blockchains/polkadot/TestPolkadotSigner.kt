// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.polkadot

import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.POLKADOT
import wallet.core.jni.proto.Polkadot
import wallet.core.jni.proto.Polkadot.SigningOutput

class TestPolkadotSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun PolkadotTransactionSigning() {
        val key = "0x37932b086586a6675e66e562fe68bd3eeea4177d066619c602fe3efc290ada62".toHexBytesInByteString()
        val hash = "0x91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3".toHexBytesInByteString()

        val call = Polkadot.Staking.Bond.newBuilder().apply {
            controller = "14CeRumfZBNBVux9GgaiR5qw9E8RndNsiFWvhcHs76HEPjbP"
            value = "0x02540be400".toHexBytesInByteString()
            rewardDestination = Polkadot.RewardDestination.STAKED
        }

        val input = Polkadot.SigningInput.newBuilder().apply {
            genesisHash = hash
            blockHash = hash
            nonce = 0
            specVersion = 17
            network = Polkadot.Network.POLKADOT
            transactionVersion = 3
            privateKey = key
            stakingCall = Polkadot.Staking.newBuilder().apply {
                bond = call.build()
            }.build()
        }

        val output = AnySigner.sign(input.build(), POLKADOT, SigningOutput.parser())
        val encoded = Numeric.toHexString(output.encoded.toByteArray())

        val expected = "0x3902848d96660f14babe708b5e61853c9f5929bc90dd9874485bf4d6dc32d3e6f22eaa00a559867d1304cc95bac7cfe5d1b2fd49aed9f43c25c7d29b9b01c1238fa1f6ffef34b9650e42325de41e20fd502af7b074c67a9ec858bd9a1ba6d4212e3e0d0f00000007008d96660f14babe708b5e61853c9f5929bc90dd9874485bf4d6dc32d3e6f22eaa0700e40b540200"
        assertEquals(encoded, expected)
    }
}
