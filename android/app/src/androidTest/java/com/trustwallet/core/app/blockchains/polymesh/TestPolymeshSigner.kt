// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.polymesh

import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.POLYMESH
import wallet.core.jni.proto.Polkadot
import wallet.core.jni.proto.Polymesh
import wallet.core.jni.proto.Polymesh.SigningOutput

class TestPolymeshSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    val genesisHashStr = "0x6fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063".toHexBytesInByteString()
    // Private key for testing.  DO NOT USE, since this is public.
    val TestKey1 = "0x790a0a01ec2e7c7db4abcaffc92ce70a960ef9ad3021dbe3bf327c1c6343aee4".toHexBytesInByteString()

    @Test
    fun PolymeshTransactionSigning() {
        // https://polymesh.subscan.io/extrinsic/0x98cb5e33d8ff3dd5838c384e2ef9e291314ed8db13f5d4f42cdd70bad54a5e04

        // Step 1: Prepare input.
        val blockHashStr = "77d32517dcc7b74501096afdcff3af72008a2c489e17083f56629d195e5c6a1d".toHexBytesInByteString()

        var call = Polymesh.Balance.Transfer.newBuilder().apply {
            toAddress = "2CpqFh8VnwJAjenw4xSUWCaaJ2QwGdhnCikoSEczMhjgqyj7"
            value = "0x0F4240".toHexBytesInByteString()
        }

        val input = Polymesh.SigningInput.newBuilder().apply {
            genesisHash = genesisHashStr
            blockHash = blockHashStr
            era = Polkadot.Era.newBuilder().apply {
                blockNumber = 16_102_106
                period = 64
            }.build()
            network = POLYMESH.ss58Prefix()
            nonce = 1
            specVersion = 7000005
            transactionVersion = 7
            privateKey = TestKey1
            runtimeCall = Polymesh.RuntimeCall.newBuilder().apply {
                balanceCall = Polymesh.Balance.newBuilder().apply {
                    transfer = call.build()
                }.build()
            }.build()
        }

        val output = AnySigner.sign(input.build(), POLYMESH, SigningOutput.parser())
        val encoded = Numeric.toHexString(output.encoded.toByteArray())

        val expected = "0x390284004bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c00e9b4742a2b66931e0cf29f6811e4d44545b4f278a667b9eb1217c4b2de8763c8037e4501dd4a21179b737beb33415f458788f2d1093b527cae8bee8b2d55210ba501040005000010b713ceeb165c1ac7c450f5b138a6da0eba50bb18849f5b8e83985daa45a87e02093d00"
        assertEquals(encoded, expected)
    }
}
