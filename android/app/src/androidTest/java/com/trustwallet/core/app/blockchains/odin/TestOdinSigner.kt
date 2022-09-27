// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.odin

import android.util.Log
import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import com.trustwallet.core.app.utils.toHex
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*
import wallet.core.jni.CoinType.ODIN
import wallet.core.jni.proto.Cosmos
import wallet.core.jni.proto.Cosmos.SigningOutput
import wallet.core.jni.proto.Cosmos.SigningMode
import wallet.core.java.AnySigner

class TestOdinSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun OdinTransactionSigning() {
        val key = PrivateKey("a39b28abca9b410e6f53d87a42229150cb176211996391c989336fbbcb9606cb".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, ODIN).description()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = "99800"
            denom = "loki"
        }.build()

        val sendCoinsMsg = Cosmos.Message.Send.newBuilder().apply {
            fromAddress = from
            toAddress = "odin13d7rfa8v8jmmsh63jetd9euqftaqfc3hre5ake"
            addAllAmounts(listOf(txAmount))
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            sendCoinsMessage = sendCoinsMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "200"
            denom = "loki"
        }.build()

        val odinFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            signingMode = SigningMode.Protobuf
            accountNumber = 124703
            chainId = "odin-mainnet-freya"
            memo = ""
            sequence = 0
            fee = odinFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, ODIN, SigningOutput.parser())

        assertEquals(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKK29kaW4xZnpqbnN1d3p4em5xZDNxbGt0NW5qZDR2dnBjanFjYTZ6Y2tmdXgSK29kaW4xM2Q3cmZhOHY4am1tc2g2M2pldGQ5ZXVxZnRhcWZjM2hyZTVha2UaDQoEbG9raRIFOTk4MDASYwpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAzdXdUgXyztVVpl5pFOdmj8kUEWei9xLNcZQihXXmt5IEgQKAggBEhEKCwoEbG9raRIDMjAwEMCaDBpAfXlsIu1Ke+kalRDx2vkE8anvcsajlru8RETlqBqEQWYD2JENyS0fmxTNsEmNu40Unjth3c0O8aUFYnj4D38P3A==\"}")
        assertEquals(output.error, "")
    }
}
