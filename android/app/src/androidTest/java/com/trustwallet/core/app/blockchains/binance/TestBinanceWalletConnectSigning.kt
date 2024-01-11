package com.trustwallet.core.app.blockchains.binance

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexBytes
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.proto.Binance.SigningOutput
import wallet.core.jni.proto.WalletConnect
import wallet.core.jni.*
import wallet.core.jni.CoinType.BINANCE
import wallet.core.java.AnySigner
import wallet.core.jni.proto.Common

class TestBinanceWalletConnectSigning {
    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSignBinanceTransactionFromWalletConnectRequest() {
        // Step 1: Parse a signing request received through WalletConnect.

        val parsingInput = WalletConnect.ParseRequestInput.newBuilder().apply {
            method = WalletConnect.Method.CosmosSignAmino
            payload = "{\"signerAddress\":\"bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2\",\"signDoc\":{\"account_number\":\"19\",\"chain_id\":\"chain-bnb\",\"memo\":\"\",\"data\":null,\"msgs\":[{\"inputs\":[{\"address\":\"bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2\",\"coins\":[{\"amount\":1001000000,\"denom\":\"BNB\"}]}],\"outputs\":[{\"address\":\"bnb13zeh6hs97d5eu2s5qerguhv8ewwue6u4ywa6yf\",\"coins\":[{\"amount\":1001000000,\"denom\":\"BNB\"}]}]}],\"sequence\":\"23\",\"source\":\"1\"}}"
        }.build()

        val parsingOutputBytes = WalletConnectRequest.parse(BINANCE, parsingInput.toByteArray())
        val parsingOutput = WalletConnect.ParseRequestOutput.parseFrom(parsingOutputBytes)

        assertEquals(parsingOutput.error, Common.SigningError.OK)

        // Step 2: Set missing fields.

        val signingInput = parsingOutput.binance.toBuilder().apply {
            privateKey = ByteString.copyFrom("95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832".toHexBytes())
        }.build()

        // Step 3: Sign the transaction.

        val output = AnySigner.sign(signingInput, BINANCE, SigningOutput.parser())

        assertEquals(output.error, Common.SigningError.OK)
        assertEquals(output.signatureJson, "{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"Amo1kgCI2Yw4iMpoxT38k/RWRgJgbLuH8P5e5TPbOOUC\"},\"signature\":\"PCTHhMa7+Z1U/6uxU+3LbTxKd0k231xypdMolyVvjgYvMg+0dTMC+wqW8IxHWXTSDt/Ronu+7ac1h/WN3JWJdQ==\"}")
    }
}