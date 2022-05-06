package com.trustwallet.core.app.blockchains.cosmos

import android.util.Log
import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*
import wallet.core.jni.CoinType.KAVA
import wallet.core.jni.proto.Cosmos
import wallet.core.jni.proto.Cosmos.SigningOutput
import wallet.core.java.AnySigner

class TestKavaTransactions {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSigningTransaction() {
        val key = PrivateKey("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, KAVA).description()

        val rawJSON = Cosmos.Message.RawJSON.newBuilder().apply {
            type = "withdraw_cdp"
            value = """
                {
                  "collateral": 
                    {
                      "amount": "1000",
                      "denom": "bnb"
                    },
                  "collateral_type": "bnb-a",
                  "depositor": "$from",
                  "owner": "$from"
                }
                """.trimIndent()
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "100"
            denom = "ukava"
        }.build()

        val kavaFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            rawJsonMessage = rawJSON
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            accountNumber = 204
            chainId = "kava-2"
            sequence = 4
            fee = kavaFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, KAVA, SigningOutput.parser())
        println(output.json)

        val expectedJsonPayload = """{"mode":"block","tx":{"fee":{"amount":[{"amount":"100","denom":"ukava"}],"gas":"200000"},"memo":"","msg":[{"type":"withdraw_cdp","value":{"collateral":{"amount":"1000","denom":"bnb"},"collateral_type":"bnb-a","depositor":"kava1hsk6jryyqjfhp5dhc55tc9jtckygx0ept0esed","owner":"kava1hsk6jryyqjfhp5dhc55tc9jtckygx0ept0esed"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"TF31bcLSUDiQFa2Ul6AOuMsmQnPq0OfsSto+6AXhCIUETEZQRIXoK1BqyGeTjIKnJrkg/Jo4SOONJQKx2rDFIg=="}]}}"""
        assertEquals(expectedJsonPayload, output.json)

    }
}
