// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.bluzelle

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.AnyAddress
import wallet.core.jni.CoinType.BLUZELLE
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Cosmos
import wallet.core.jni.proto.Cosmos.SigningOutput

class TestBluzelleSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSigningTransaction() {

        // Submitted Realworld tx for the following test : https://bigdipper.net.bluzelle.com/transactions/B3A7F30539CCDF72D210BC995FAF65B43F9BE04FA9F8AFAE0EC969660744002F
        val key =
            PrivateKey("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, BLUZELLE).description()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = "1"
            denom = "ubnt"
        }.build()

        val sendCoinsMsg = Cosmos.Message.Send.newBuilder().apply {
            fromAddress = from
            toAddress = "bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"
            addAllAmounts(listOf(txAmount))
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            sendCoinsMessage = sendCoinsMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "1000"
            denom = "ubnt"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 500000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            accountNumber = 590
            chainId = "net-6"
            memo = ""
            sequence = 2
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, BLUZELLE, SigningOutput.parser())
        val jsonPayload = output.json

        val expectedJsonPayload = """{"mode":"block","tx":{"fee":{"amount":[{"amount":"1000","denom":"ubnt"}],"gas":"500000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"ubnt"}],"from_address":"bluzelle1hsk6jryyqjfhp5dhc55tc9jtckygx0epzzw0fm","to_address":"bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"5e3e13x1F+Y4+cPNvE1jQ/Mrz0J2RQCY69re3g4xuTY3Gw7MNGQ+8E34d9DgvcNLPM05nehdOv/0SvekY/ihIQ=="}]}}"""
        assertEquals(expectedJsonPayload, jsonPayload)

    }

    @Test
    fun testSigningJSON() {

        // Submitted realworld tx for the following test : https://bigdipper.net.bluzelle.com/transactions/DEF394BE0DD1075CDC8B8618A7858AAA4A03A43D04476381224E316E06FD3A5B
        val myPrivateKey = "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005"
        val myAddress = "bluzelle1hsk6jryyqjfhp5dhc55tc9jtckygx0epzzw0fm"

        val toAddress = "bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"
        val json = """
        {
            "accountNumber": "590",
            "chainId": "net-6",
            "sequence": "3",
            "fee": {
                "amounts": [{
                    "denom": "ubnt",
                    "amount": "1000"
                }],
                "gas": "500000"
            },
            "memo": "Testing",
            "messages": [{
                "sendCoinsMessage": {
                    "fromAddress": "$myAddress",
                    "toAddress": "$toAddress",
                    "amounts": [{
                        "denom": "ubnt",
                        "amount": "2"
                    }]
                }
            }]
        }
        """
        val key = myPrivateKey.toHexByteArray()
        val actualResult  = AnySigner.signJSON(json, key, BLUZELLE.value())

        val expectedResult = """{"mode":"block","tx":{"fee":{"amount":[{"amount":"1000","denom":"ubnt"}],"gas":"500000"},"memo":"Testing","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"2","denom":"ubnt"}],"from_address":"bluzelle1hsk6jryyqjfhp5dhc55tc9jtckygx0epzzw0fm","to_address":"bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"2JcfVwq9N3UAk5Lfki7+CngqcefgjfH1q/8chtJMJvEHRe6PvuYKMv5pjeN0Z5Vk2BArJT3V3EHxbpbiY2eLWw=="}]}}"""
        assertEquals(expectedResult, actualResult)
    }
}
