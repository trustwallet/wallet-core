// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
        val from = AnyAddress(publicKey, THORCHAIN).description()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = 2000000
            denom = "rune"
        }.build()

        val sendCoinsMsg = Cosmos.Message.Send.newBuilder().apply {
            fromAddress = from
            toAddress = "thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"
            addAllAmounts(listOf(txAmount))
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            sendCoinsMessage = sendCoinsMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = 2000000
            denom = "rune"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            accountNumber = 593
            chainId = "thorchain"
            memo = ""
            sequence = 2
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, THORCHAIN, SigningOutput.parser())
        val jsonPayload = output.json

        val expectedJsonPayload = """{"mode":"block","tx":{"fee":{"amount":[{"amount":"2000000","denom":"rune"}],"gas":"200000"},"memo":"","msg":[{"type":"thorchain/MsgSend","value":{"amount":[{"amount":"2000000","denom":"rune"}],"from_address":"thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","to_address":"thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"},"signature":"SsagpldYbikqjq0sw4IM8oo3l2FE4iPi1kPxJgQ6PhdT/RRmg4BEAq3weB+hTOp4SfFXI/r+wms7tKkZci6SbA=="}]}}"""
        assertEquals(expectedJsonPayload, jsonPayload)
    }
}
