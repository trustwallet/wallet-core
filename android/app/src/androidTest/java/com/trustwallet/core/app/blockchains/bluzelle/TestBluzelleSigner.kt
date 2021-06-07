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
        val key =
            PrivateKey("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, BLUZELLE).description()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = 1
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
            amount = 200
            denom = "ubnt"
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

        val output = AnySigner.sign(signingInput, BLUZELLE, SigningOutput.parser())
        val jsonPayload = output.json

        val expectedJsonPayload = """{"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"ubnt"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"ubnt"}],"from_address":"bluzelle1hsk6jryyqjfhp5dhc55tc9jtckygx0epzzw0fm","to_address":"bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"B8s3xp3hkkKMqJSGp9sPvUtSjEJ2rKmDzERP/FD/cz09Ot//ERMhUEnIr3jsWVuwj+jv5QYQ15B43Uz3CWx9wg=="}]}}"""
        assertEquals(expectedJsonPayload, jsonPayload)

    }

    @Test
    fun testSigningJSON() {

        val myPrivateKey = "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005"
        val myAddress = "bluzelle1hsk6jryyqjfhp5dhc55tc9jtckygx0epzzw0fm"

        val toAddress = "bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"
        val json = """
        {
            "accountNumber": "8733",
            "chainId": "bluzelle",
            "fee": {
                "amounts": [{
                    "denom": "ubnt",
                    "amount": "5000"
                }],
                "gas": "200000"
            },
            "memo": "Testing",
            "messages": [{
                "sendCoinsMessage": {
                    "fromAddress": "$myAddress",
                    "toAddress": "$toAddress",
                    "amounts": [{
                        "denom": "ubnt",
                        "amount": "995000"
                    }]
                }
            }]
        }
        """
        val key = myPrivateKey.toHexByteArray()
        val actualResult  = AnySigner.signJSON(json, key, BLUZELLE.value())

        val expectedResult = """{"mode":"block","tx":{"fee":{"amount":[{"amount":"5000","denom":"ubnt"}],"gas":"200000"},"memo":"Testing","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"995000","denom":"ubnt"}],"from_address":"$myAddress","to_address":"$toAddress"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"gKWKk5lku9H/wosvnsD6auWUt7AGNjLEudWS+Fdv4TQr16HUBVpkq9A6gZYqZIawFukq2I/qrtyw9MKR4pVkhg=="}]}}"""
        assertEquals(expectedResult, actualResult)
    }
}