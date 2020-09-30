// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.iov

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.AnyAddress
import wallet.core.jni.CoinType.IOV
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Cosmos
import wallet.core.jni.proto.Cosmos.SigningOutput

class TestIOVSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSigningTransaction() {
        val key =
                PrivateKey("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, IOV).description()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = 1
            denom = "uiov"
        }.build()

        val sendCoinsMsg = Cosmos.Message.Send.newBuilder().apply {
            fromAddress = from
            toAddress = "star1gfpfk3s8xnupdce7pgt3z7mf79clvkw6px89ne"
            addAllAmounts(listOf(txAmount))
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            sendCoinsMessage = sendCoinsMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = 200
            denom = "uiov"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            accountNumber = 1037
            chainId = "iov-mainnet-2"
            memo = ""
            sequence = 8
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, IOV, SigningOutput.parser())
        val jsonPayload = output.json

        val expectedJsonPayload = """{"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"uiov"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"uiov"}],"from_address":"star1hsk6jryyqjfhp5dhc55tc9jtckygx0epkqrycx","to_address":"star1gfpfk3s8xnupdce7pgt3z7mf79clvkw6px89ne"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"sRDovdQyk2TZE7iO0mXRfedBgzeAU+kyPXoaz4wYpcomzoxOfaHKC1gI/ZJi3RxOqN0H5C2F52VcaBHw4w9ySQ=="}]}}"""
        assertEquals(expectedJsonPayload, jsonPayload)

    }

    @Test
    fun testSigningJSON() {
        val json = """
        {
            "accountNumber": "8733",
            "chainId": "iov-mainnet-2",
            "fee": {
                "amounts": [{
                    "denom": "uiov",
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
                        "denom": "uiov",
                        "amount": "995000"
                    }]
                }
            }]
        }
        """
        val key = "c9b0a273831931aa4a5f8d1a570d5021dda91d3319bd3819becdaabfb7b44e3b".toHexByteArray()
        val result = AnySigner.signJSON(json, key, IOV.value())

        assertEquals(result, """{"mode":"block","tx":{"fee":{"amount":[{"amount":"5000","denom":"uiov"}],"gas":"200000"},"memo":"Testing","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"995000","denom":"uiov"}],"from_address":"band1jf9aaj9myrzsnmpdr7twecnaftzmku2mgms4n3","to_address":"band1pnndgfwsrff86263xzpc5cd3t6yfvgjyqc87jh"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6EsukEXB53GhohQVeDpxtkeH8KQIayd/Co/ApYRYkTm"},"signature":"tnDMD2JKRXHIlYTtp9BndnjA4lYBvnQzWxuGf0672Y9+s1wV+OSxDO51sx4TyWj3VOHVdJCVSlz9BR21S0OKhQ=="}]}}""")
    }
}
