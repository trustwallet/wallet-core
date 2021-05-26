// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.crypto_org

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.AnyAddress
import wallet.core.jni.CoinType.CRYPTO_ORG
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
            PrivateKey("d49a797db63b61c148d1767e616ff15c3dd9c83b63cba61c218ea172f238622a".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, CRYPTO_ORG).description()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = 1
            denom = "basecro"
        }.build()

        val sendCoinsMsg = Cosmos.Message.Send.newBuilder().apply {
            fromAddress = from
            toAddress = "cro1u9q8mfpzhyv2s43js7l5qseapx5kt3g2rf7ppf"
            addAllAmounts(listOf(txAmount))
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            sendCoinsMessage = sendCoinsMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = 200
            denom = "basecro"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            accountNumber = 1037
            chainId = "crypto-org-chain-mainnet-1"
            memo = ""
            sequence = 8
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, CRYPTO_ORG, SigningOutput.parser())
        val jsonPayload = output.json

        val expectedJsonPayload = """{"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"basecro"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"basecro"}],"from_address":"cro1u9q8mfpzhyv2s43js7l5qseapx5kt3g2rf7ppf","to_address":"cro1u9q8mfpzhyv2s43js7l5qseapx5kt3g2rf7ppf"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"B1ZG7pUWW2mPxE7WzBt8SafZMtEtFWgUJePJ+Dj/q7cHxj4scGmopQUG4+AZcJbRQgjrMGM11Yhm3vXYQYtSDA=="}]}}"""
        assertEquals(expectedJsonPayload, jsonPayload)

    }

    @Test
    fun testSigningJSON() {
        val json = """
        {
            "accountNumber": "8733",
            "chainId": "crypto-org-chain-mainnet-1",
            "fee": {
                "amounts": [{
                    "denom": "basecro",
                    "amount": "5000"
                }],
                "gas": "200000"
            },
            "memo": "Testing",
            "messages": [{
                "sendCoinsMessage": {
                    "fromAddress": "cro1u9q8mfpzhyv2s43js7l5qseapx5kt3g2rf7ppf",
                    "toAddress": "cro1u9q8mfpzhyv2s43js7l5qseapx5kt3g2rf7ppf",
                    "amounts": [{
                        "denom": "basecro",
                        "amount": "995000"
                    }]
                }
            }]
        }
        """
        val key = "c9b0a273831931aa4a5f8d1a570d5021dda91d3319bd3819becdaabfb7b44e3b".toHexByteArray()
        val result = AnySigner.signJSON(json, key, CRYPTO_ORG.value())

        assertEquals(result, """{"mode":"block","tx":{"fee":{"amount":[{"amount":"5000","denom":"basecro"}],"gas":"200000"},"memo":"Testing","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"995000","denom":"basecro"}],"from_address":"cro1u9q8mfpzhyv2s43js7l5qseapx5kt3g2rf7ppf","to_address":"cro1u9q8mfpzhyv2s43js7l5qseapx5kt3g2rf7ppf"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6EsukEXB53GhohQVeDpxtkeH8KQIayd/Co/ApYRYkTm"},"signature":"sw2YPxjQ5DiKjd2o70sQb44OSzMH2Pm4V+Z8ld1uYiNbMXWQBK8SH2tcKUIU3SwYZ1qvi2DbmxqHyONksJ0Rmg=="}]}}""")
    }
}
