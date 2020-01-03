package com.trustwalval.core.app.blockchains.terra

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

class TestTerraTransactions {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSigningTransaction() {
        val key =
            PrivateKey("1037f828ca313f4c9e120316e8e9ff25e17f07fe66ba557d5bc5e2eeb7cba8f6".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = CosmosAddress(HRP.TERRA, publicKey).description()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = 1000000
            denom = "uluna"
        }.build()

        val sendCoinsMsg = Cosmos.Message.Send.newBuilder().apply {
            fromAddress = from
            toAddress = "terra1hdp298kaz0eezpgl6scsykxljrje3667d233ms"
            addAllAmounts(listOf(txAmount))
            typePrefix = "bank/MsgSend"
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            sendCoinsMessage = sendCoinsMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = 3000
            denom = "uluna"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            accountNumber = 158
            chainId = "soju-0013"
            memo = ""
            sequence = 0
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val jsonPayload = CosmosSigner.sign(signingInput).json

        val expectedJsonPayload = """{"mode":"block","tx":{"fee":{"amount":[{"amount":"3000","denom":"uluna"}],"gas":"200000"},"memo":"","msg":[{"type":"bank/MsgSend","value":{"amount":[{"amount":"1000000","denom":"uluna"}],"from_address":"terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe","to_address":"terra1hdp298kaz0eezpgl6scsykxljrje3667d233ms"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"},"signature":"KPdiVsKpY12JG/VKEJVa/FpMKclxlS0qNNG6VOAypj10R5vY5UX5IgRJET1zNYnH0wvcXxfNXV+s8jtwN2UXiQ=="}]}}"""
        assertEquals(expectedJsonPayload, jsonPayload)

    }
}
