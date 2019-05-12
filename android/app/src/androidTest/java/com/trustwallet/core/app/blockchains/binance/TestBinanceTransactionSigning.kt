package com.trustwallet.core.app.blockchains.binance

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexBytes
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.BinanceSigner
import wallet.core.jni.HRP
import wallet.core.jni.PrivateKey
import wallet.core.jni.CosmosAddress
import wallet.core.jni.proto.Binance
import com.trustwallet.core.app.utils.toHex

class TestBinanceTransactionSigning {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSignBinanceTransaction() {
        val privateKey = PrivateKey("95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832".toHexBytes())
        val publicKey = privateKey.getPublicKeySecp256k1(true)

        val signingInput = Binance.SigningInput.newBuilder()
        signingInput.chainId = "Binance-Chain-Nile"
        signingInput.accountNumber = 0
        signingInput.sequence = 0

        signingInput.privateKey = ByteString.copyFrom(privateKey.data())

        val token = Binance.SendOrder.Token.newBuilder()
        token.denom = "BNB"
        token.amount = 1

        val input = Binance.SendOrder.Input.newBuilder()
        input.address = ByteString.copyFrom(CosmosAddress(HRP.BINANCE, publicKey).keyHash())
        input.addAllCoins(listOf(token.build()))

        val output =  Binance.SendOrder.Output.newBuilder()
        output.address = ByteString.copyFrom(CosmosAddress("bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5").keyHash())
        output.addAllCoins(listOf(token.build()))

        val sendOrder = Binance.SendOrder.newBuilder()
        sendOrder.addAllInputs(listOf(input.build()))
        sendOrder.addAllOutputs(listOf(output.build()))

        signingInput.sendOrder = sendOrder.build()

        val sign: Binance.SigningOutput = BinanceSigner.sign(signingInput.build())
        val signBytes = sign.encoded.toByteArray()
        assertEquals(signBytes.toHex(), "0xb801f0625dee0a462a2c87fa0a1f0a1440c2979694bbc961023d1d27be6fc4d21a9febe612070a03424e421001121f0a14bffe47abfaede50419c577f1074fee6dd1535cd112070a03424e421001126a0a26eb5ae98721026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e50212401b1181faec30b60a2ddaa2804c253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a504f9e8310679")
    }
}