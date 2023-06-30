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
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.CRYPTOORG
import wallet.core.jni.proto.Cosmos
import wallet.core.jni.proto.Cosmos.SigningOutput
import wallet.core.jni.proto.Cosmos.SigningMode
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
            amount = "50000000"
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
            amount = "5000"
            denom = "basecro"
        }.build()

        val cosmosFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            signingMode = SigningMode.Protobuf
            accountNumber = 125798
            chainId = "crypto-org-chain-mainnet-1"
            memo = ""
            sequence = 2
            fee = cosmosFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, CRYPTOORG, SigningOutput.parser())

        // https://crypto.org/explorer/tx/BCB213B0A121F0CF11BECCF52475F1C8328D6070F3CFDA9E14C42E6DB30E847E
        assertEquals(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CpABCo0BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm0KKmNybzFjdHd0Y3dwZ2tza3k5ODhkaHRoNmpzbHh2ZXVtZ3UwZDQ1emdmMBIqY3JvMXhwYWh5NmM3d2xkeGFjdjZsZDk5aDQzNW1odmZuc3VwMjR2Y3VzGhMKB2Jhc2Vjcm8SCDUwMDAwMDAwEmkKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQOIMbBhYj5+i+WdiIxxCEpFyNCJMHy/XsVdxiwde9Vr4xIECgIIARgCEhUKDwoHYmFzZWNybxIENTAwMBDAmgwaQAcxK9xk6r69gmz+1UWaCnYxNuXPXZdp59YcqKPJE5d6fp+IICTBOwd2rs8MiApcf8kNSrbZ6oECxcGQAdxF0SI=\"}")
        assertEquals(output.errorMessage, "")
    }
}
