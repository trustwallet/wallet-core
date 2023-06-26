// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.stargaze

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.AnyAddress
import wallet.core.jni.CoinType
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Cosmos

class TestStargazeSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun stargazeTransactionCW721Signing() {
        val key =
            PrivateKey("a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433".toHexByteArray())

        val txMsg = Cosmos.Message.WasmExecuteContractGeneric.newBuilder().apply {
            senderAddress = "stars1mry47pkga5tdswtluy0m8teslpalkdq02a8nhy"
            contractAddress = "stars14gmjlyfz5mpv5d8zrksn0tjhz2wwvdc4yk06754alfasq9qen7fsknry42"
            executeMsg = """{"transfer_nft": {"recipient": "stars1kd5q7qejlqz94kpmd9pvr4v2gzgnca3lvt6xnp","token_id": "1209"}}"""
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            wasmExecuteContractGeneric = txMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "1000"
            denom = "ustars"
        }.build()

        val stargazeFee = Cosmos.Fee.newBuilder().apply {
            gas = 666666
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            signingMode = Cosmos.SigningMode.Protobuf
            accountNumber = 188393
            chainId = "stargaze-1"
            memo = ""
            sequence = 5
            fee = stargazeFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, CoinType.STARGAZE, Cosmos.SigningOutput.parser())
        // Successfully broadcasted: https://www.mintscan.io/stargaze/txs/300836A5BF9002CF38EE34A8C56E8E7E6854FA64F1DEB3AE108F381A48150F7C
        val expected = """{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CoACCv0BCiQvY29zbXdhc20ud2FzbS52MS5Nc2dFeGVjdXRlQ29udHJhY3QS1AEKLHN0YXJzMW1yeTQ3cGtnYTV0ZHN3dGx1eTBtOHRlc2xwYWxrZHEwMmE4bmh5EkBzdGFyczE0Z21qbHlmejVtcHY1ZDh6cmtzbjB0amh6Mnd3dmRjNHlrMDY3NTRhbGZhc3E5cWVuN2Zza25yeTQyGmJ7InRyYW5zZmVyX25mdCI6IHsicmVjaXBpZW50IjogInN0YXJzMWtkNXE3cWVqbHF6OTRrcG1kOXB2cjR2Mmd6Z25jYTNsdnQ2eG5wIiwidG9rZW5faWQiOiAiMTIwOSJ9fRJoClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiECy/215HKJMyIpTmDPCIPUPfQx4QidKey0R6nm1VBFquUSBAoCCAEYBRIUCg4KBnVzdGFycxIEMTAwMBCq2CgaQMx+l2sdM5DAPbDyY1p173MLnjGyNWIcRmaFiVNphLuTV3tjhwPbsXEA0hyRxyWS3vN0/xUF/JEsO9wRspj2aJ4="}""".trimIndent()
        assertEquals(
            output.serialized,
            expected
        )
        assertEquals(output.errorMessage, "")
    }

    @Test
    fun stargazeTransactionSigning() {
        val key =
            PrivateKey("a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, CoinType.STARGAZE).description()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = "10000"
            denom = "ustars"
        }.build()

        val sendCoinsMsg = Cosmos.Message.Send.newBuilder().apply {
            fromAddress = from
            toAddress = "stars1mry47pkga5tdswtluy0m8teslpalkdq02a8nhy"
            addAllAmounts(listOf(txAmount))
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            sendCoinsMessage = sendCoinsMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "1000"
            denom = "ustars"
        }.build()

        val stargazeFee = Cosmos.Fee.newBuilder().apply {
            gas = 80000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            signingMode = Cosmos.SigningMode.Protobuf
            accountNumber = 188393
            chainId = "stargaze-1"
            memo = ""
            sequence = 0
            fee = stargazeFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, CoinType.STARGAZE, Cosmos.SigningOutput.parser())
        // Successfully broadcasted: https://www.mintscan.io/stargaze/txs/98D5E36CA7080DDB286FE924A5A9976ABD4EBE49C92A09D322F29AD30DE4BE4D
        val expected = """{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CpABCo0BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm0KLHN0YXJzMW1yeTQ3cGtnYTV0ZHN3dGx1eTBtOHRlc2xwYWxrZHEwMmE4bmh5EixzdGFyczFtcnk0N3BrZ2E1dGRzd3RsdXkwbTh0ZXNscGFsa2RxMDJhOG5oeRoPCgZ1c3RhcnMSBTEwMDAwEmYKTgpGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQLL/bXkcokzIilOYM8Ig9Q99DHhCJ0p7LRHqebVUEWq5RIECgIIARIUCg4KBnVzdGFycxIEMTAwMBCA8QQaQHAkntxzC1oH7Yde4+KEmnB+K3XbJIYw0q6MqMPEY65YAwBDNDOdaTu/rpehus/20MvBfbAEZiw9+whzXLpkQ5A="}""".trimIndent()
        assertEquals(
            output.serialized,
            expected
        )
        assertEquals(output.errorMessage, "")
    }
}
