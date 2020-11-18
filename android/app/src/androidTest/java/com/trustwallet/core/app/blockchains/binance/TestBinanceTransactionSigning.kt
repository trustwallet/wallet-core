package com.trustwallet.core.app.blockchains.binance

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexBytes
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.proto.Binance
import wallet.core.jni.proto.Binance.SigningOutput
import wallet.core.jni.*
import com.trustwallet.core.app.utils.toHex
import wallet.core.jni.CoinType.BINANCE
import wallet.core.java.AnySigner

class TestBinanceTransactionSigning {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    val testKey = PrivateKey("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d".toHexBytes())

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
        input.address = ByteString.copyFrom(AnyAddress(publicKey, BINANCE).data())
        input.addAllCoins(listOf(token.build()))

        val output =  Binance.SendOrder.Output.newBuilder()
        output.address = ByteString.copyFrom(AnyAddress("bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5", BINANCE).data())
        output.addAllCoins(listOf(token.build()))

        val sendOrder = Binance.SendOrder.newBuilder()
        sendOrder.addAllInputs(listOf(input.build()))
        sendOrder.addAllOutputs(listOf(output.build()))

        signingInput.sendOrder = sendOrder.build()

        val sign: Binance.SigningOutput = AnySigner.sign(signingInput.build(), BINANCE, SigningOutput.parser())
        val signBytes = sign.encoded.toByteArray()
        assertEquals(signBytes.toHex(), "0xb801f0625dee0a462a2c87fa0a1f0a1440c2979694bbc961023d1d27be6fc4d21a9febe612070a03424e421001121f0a14bffe47abfaede50419c577f1074fee6dd1535cd112070a03424e421001126a0a26eb5ae98721026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e50212401b1181faec30b60a2ddaa2804c253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a504f9e8310679")
    }

    @Test
    fun testSignTransferOutOrder() {
        val key = testKey
        val pubkey = key.getPublicKeySecp256k1(true)
        val input = Binance.SigningInput.newBuilder()
        input.chainId = "test-chain"
        input.accountNumber = 15
        input.sequence = 1
        input.privateKey = ByteString.copyFrom(key.data())

        val amount = Binance.SendOrder.Token.newBuilder()
        amount.denom = "BNB"
        amount.amount = 100000000

        val order = Binance.TransferOut.newBuilder()
        order.from = ByteString.copyFrom(AnyAddress(pubkey, CoinType.BINANCE).data())
        order.to = ByteString.copyFrom(AnyAddress("0x35552c16704d214347f29Fa77f77DA6d75d7C752", CoinType.ETHEREUM).data())
        order.amount = amount.build()
        order.expireTime = 12345678

        input.transferOutOrder = order.build()

        val output: Binance.SigningOutput = AnySigner.sign(input.build(), CoinType.BINANCE, SigningOutput.parser())

        assertEquals("0xb701f0625dee0a41800819c00a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e1121435552c16704d214347f29fa77f77da6d75d7c7521a0a0a03424e421080c2d72f20cec2f105126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc712407eda148e1167b1be1271a788ccf4e3eade1c7e1773e9d2093982d7f802f8f85f35ef550049011728206e4eda1a272f9e96fd95ef3983cad85a29cd14262c22e0180f2001",
            output.encoded.toByteArray().toHex())
    }
}
