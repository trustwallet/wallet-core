// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.thorchain

import android.util.Log
import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.THORCHAIN
import wallet.core.jni.proto.Cosmos
import wallet.core.jni.proto.Cosmos.SigningOutput
import wallet.core.jni.proto.Cosmos.SigningMode
import wallet.core.jni.*

class TestTHORChainSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun THORChainTransactionSigning() {
        val key =
            PrivateKey("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, THORCHAIN).data()
        val to = AnyAddress("thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn", THORCHAIN).data()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = "38000000"
            denom = "rune"
        }.build()

        val sendCoinsMsg = Cosmos.Message.THORChainSend.newBuilder().apply {
            fromAddress = ByteString.copyFrom(from)
            toAddress = ByteString.copyFrom(to)
            addAllAmounts(listOf(txAmount))
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            thorchainSendMessage = sendCoinsMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "200"
            denom = "rune"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 2500000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            signingMode = SigningMode.Protobuf
            chainId = "thorchain-mainnet-v1"
            accountNumber = 593
            sequence = 21
            memo = ""
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, THORCHAIN, SigningOutput.parser())

        assertEquals(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"ClIKUAoOL3R5cGVzLk1zZ1NlbmQSPgoUFSLnZ9tusZcIsAOAKb+9YHvJvQ4SFMqGRZ+wBVHH30JUDF54aRksgzrbGhAKBHJ1bmUSCDM4MDAwMDAwEmYKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQPtmX45bPQpL1/OWkK7pBWZzNXZbjExVKfJ6nBJ3jF8dxIECgIIARgVEhIKCwoEcnVuZRIDMjAwEKDLmAEaQKZtS3ATa26OOGvqdKm14ZbHeNfkPtIajXi5MkZ5XaX2SWOeX+YnCPZ9TxF9Jj5cVIo71m55xq4hVL3yDbRe89g=\"}")
        assertEquals(output.errorMessage, "")
        assertEquals(output.json, "")
    }
}
