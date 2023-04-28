// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.agoric

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.AnyAddress
import wallet.core.jni.CoinType
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Cosmos

class TestAgoricSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun AgoricTransactionSigning() {
        val key = PrivateKey("037048190544fa57651452f477c096de4f3073e7835cf3845b04602563a73f73".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, CoinType.AGORIC).description()

        val transferAmount = Cosmos.Amount.newBuilder().apply {
            amount = "1"
            denom = "ubld"
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            sendCoinsMessage = Cosmos.Message.Send.newBuilder().apply {
                fromAddress = from
                toAddress = "agoric1cqvwa8jr6pmt45jndanc8lqmdsxjkhw0yertc0"
                addAllAmounts(listOf(transferAmount))
            }.build()
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "2000"
            denom = "ubld"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 100000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            signingMode = Cosmos.SigningMode.Protobuf
            accountNumber = 62972
            chainId = "agoric-3"
            sequence = 1
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, CoinType.AGORIC, Cosmos.SigningOutput.parser())

        assertEquals(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKLWFnb3JpYzE4enZ2Z2s2ajNlcTV3ZDdtcXhjY2d0MjBnejJ3OTRjeTg4YWVrNRItYWdvcmljMWNxdndhOGpyNnBtdDQ1am5kYW5jOGxxbWRzeGpraHcweWVydGMwGgkKBHVibGQSATESZgpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA9+aXkCJ+J1FkT+yuFbemEx+i/E0TMZETMlwWJmkjJOdEgQKAggBGAESEgoMCgR1YmxkEgQyMDAwEKCNBhpAenbGO4UBK610dwSY6l5pl58qwHW1OujQ/9vF9unQdrA1SE0b/2mZxnevy5y3u6pJfBffWUfCx68PcVEu7D3EYQ==\"}")
    }
}
