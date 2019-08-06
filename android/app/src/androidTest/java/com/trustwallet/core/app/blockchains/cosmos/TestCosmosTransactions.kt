package com.trustwalval.core.app.blockchains.cosmos

import android.util.Log
import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.CosmosAddress
import wallet.core.jni.CosmosSigner
import wallet.core.jni.HRP
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Cosmos

class TestCosmosTransactions {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSigningTransaction() {
        val privateKey =
            PrivateKey("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005".toHexByteArray())
        val publicKey = privateKey.getPublicKeySecp256k1(true)
        val from = CosmosAddress(HRP.COSMOS, publicKey).description()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = 1
            denom = "muon"
        }.build()

        val sendCoinsMsg = Cosmos.SendCoinsMessage.newBuilder().apply {
            fromAddress = from
            toAddress = "cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"
            addAllAmounts(listOf(txAmount))
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = 200
            denom = "muon"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            accountNumber = 1037
            chainId = "gaia-13003"
            memo = ""
            sequence = 8
            sendCoinsMessage = sendCoinsMsg
            fee = cosmosFee
            this.privateKey = ByteString.copyFrom(privateKey.data())
        }.build()

        val jsonPayload = CosmosSigner.sign(signingInput).json

        val expectedJsonPayload = """{"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"muon"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"muon"}],"from_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","to_address":"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg=="}],"type":"cosmos-sdk/MsgSend"}}"""
        assertEquals(expectedJsonPayload, jsonPayload)

    }
}