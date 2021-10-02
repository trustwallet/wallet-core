// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.cryptoorg

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.CRYPTOORG
import wallet.core.jni.proto.Cosmos
import wallet.core.jni.proto.Cosmos.SigningOutput
import wallet.core.jni.*

class TestCryptoorgSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun CryptoorgTransactionSigning() {
        val key = PrivateKey("200e439e39cf1aad465ee3de6166247f914cbc0f823fc2dd48bf16dcd556f39d".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, CRYPTOORG).description()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = 100000000
            denom = "basecro"
        }.build()

        val sendCoinsMsg = Cosmos.Message.Send.newBuilder().apply {
            fromAddress = from
            toAddress = "cro1xpahy6c7wldxacv6ld99h435mhvfnsup24vcus"
            addAllAmounts(listOf(txAmount))
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            sendCoinsMessage = sendCoinsMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = 5000
            denom = "basecro"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            accountNumber = 125798
            chainId = "crypto-org-chain-mainnet-1"
            memo = ""
            sequence = 0
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, CRYPTOORG, SigningOutput.parser())
        val jsonPayload = output.json

        val expectedJsonPayload = """{"mode":"block","tx":{"fee":{"amount":[{"amount":"5000","denom":"basecro"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"100000000","denom":"basecro"}],"from_address":"cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0","to_address":"cro1xpahy6c7wldxacv6ld99h435mhvfnsup24vcus"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A4gxsGFiPn6L5Z2IjHEISkXI0IkwfL9exV3GLB171Wvj"},"signature":"5+5rSFFg0FE9cTklQWQHNktBDJsz7UCnMSgF0t0+gYcrIhEWUyTtibXaHZQbKAAaciJ1BkHXYREjU55VswByVg=="}]}}"""
        assertEquals(expectedJsonPayload, jsonPayload)
    }
}
