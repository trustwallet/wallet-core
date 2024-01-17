// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.bandchain

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.AnyAddress
import wallet.core.jni.CoinType.BANDCHAIN
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Cosmos
import wallet.core.jni.proto.Cosmos.SigningOutput

class TestBandChainSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSigningTransaction() {
        val key =
            PrivateKey("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, BANDCHAIN).description()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = "1"
            denom = "uband"
        }.build()

        val sendCoinsMsg = Cosmos.Message.Send.newBuilder().apply {
            fromAddress = from
            toAddress = "band1jf9aaj9myrzsnmpdr7twecnaftzmku2mgms4n3"
            addAllAmounts(listOf(txAmount))
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            sendCoinsMessage = sendCoinsMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "200"
            denom = "uband"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            accountNumber = 1037
            chainId = "band-wenchang-testnet2"
            memo = ""
            sequence = 8
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, BANDCHAIN, SigningOutput.parser())
        val jsonPayload = output.json

        val expectedJsonPayload = """{"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"uband"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"uband"}],"from_address":"band1hsk6jryyqjfhp5dhc55tc9jtckygx0epw4d0hz","to_address":"band1jf9aaj9myrzsnmpdr7twecnaftzmku2mgms4n3"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"B1ZG7pUWW2mPxE7WzBt8SafZMtEtFWgUJePJ+Dj/q7cHxj4scGmopQUG4+AZcJbRQgjrMGM11Yhm3vXYQYtSDA=="}]}}"""
        assertEquals(expectedJsonPayload, jsonPayload)

    }

    @Test
    fun testSigningJSON() {
        val json = """
        {
            "accountNumber": "8733",
            "chainId": "band-wenchang-testnet2",
            "fee": {
                "amounts": [{
                    "denom": "uband",
                    "amount": "5000"
                }],
                "gas": "200000"
            },
            "memo": "Testing",
            "messages": [{
                "sendCoinsMessage": {
                    "fromAddress": "band1jf9aaj9myrzsnmpdr7twecnaftzmku2mgms4n3",
                    "toAddress": "band1pnndgfwsrff86263xzpc5cd3t6yfvgjyqc87jh",
                    "amounts": [{
                        "denom": "uband",
                        "amount": "995000"
                    }]
                }
            }]
        }
        """
        val key = "c9b0a273831931aa4a5f8d1a570d5021dda91d3319bd3819becdaabfb7b44e3b".toHexByteArray()
        val result = AnySigner.signJSON(json, key, BANDCHAIN.value())

        assertEquals(result, """{"mode":"block","tx":{"fee":{"amount":[{"amount":"5000","denom":"uband"}],"gas":"200000"},"memo":"Testing","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"995000","denom":"uband"}],"from_address":"band1jf9aaj9myrzsnmpdr7twecnaftzmku2mgms4n3","to_address":"band1pnndgfwsrff86263xzpc5cd3t6yfvgjyqc87jh"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6EsukEXB53GhohQVeDpxtkeH8KQIayd/Co/ApYRYkTm"},"signature":"sw2YPxjQ5DiKjd2o70sQb44OSzMH2Pm4V+Z8ld1uYiNbMXWQBK8SH2tcKUIU3SwYZ1qvi2DbmxqHyONksJ0Rmg=="}]}}""")
    }
}
