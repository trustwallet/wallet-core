package com.trustwallet.core.app.blockchains.binance

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexBytes
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.BinanceSigner
import wallet.core.jni.HRP
import wallet.core.jni.PrivateKeySecp256k1 as PrivateKey
import wallet.core.jni.TendermintAddress
import wallet.core.jni.proto.Binance

class TestBinanceTransactionSigning {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSignBinanceTransaction() {
        val privateKey = PrivateKey("95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832".toHexBytes())
        val publicKey = privateKey.getPublicKey(true)

        val signingInput = Binance.SigningInput.newBuilder()
        signingInput.chainId = "Binance-Chain-Nile"
        signingInput.accountNumber = 0
        signingInput.sequence = 0
        signingInput.testNet = true

        signingInput.privateKey = ByteString.copyFrom(privateKey.data())

        val token = Binance.SendOrder.Token.newBuilder()
        token.denom = "BNB"
        token.amount = 1

        val input = Binance.SendOrder.Input.newBuilder()
        input.address = ByteString.copyFrom(TendermintAddress(HRP.BINANCETEST, publicKey).keyHash())
        input.addAllCoins(listOf(token.build()))

        val output =  Binance.SendOrder.Output.newBuilder()
        output.address = ByteString.copyFrom(TendermintAddress("tbnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx3f309d9").keyHash())
        output.addAllCoins(listOf(token.build()))

        val sendOrder = Binance.SendOrder.newBuilder()
        sendOrder.addAllInputs(listOf(input.build()))
        sendOrder.addAllOutputs(listOf(output.build()))

        signingInput.sendOrder = sendOrder.build()

        val data = BinanceSigner(signingInput.build()).build()
        assertEquals(data.toHex(), "0xb801f0625dee0a462a2c87fa0a1f0a1440c2979694bbc961023d1d27be6fc4d21a9febe612070a03424e421001121f0a14bffe47abfaede50419c577f1074fee6dd1535cd112070a03424e421001126a0a26eb5ae98721026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e5021240376f64070fdb621a4cf24e24da350476260efaf4bae799cacca19b31bc0d1ce4054f09fcffe9cfabef6a8d6a24bc1814bd444720a35f035f3c26409ffad2a9e2")
    }
}