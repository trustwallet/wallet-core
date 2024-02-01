// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.nativeinjective

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.AnyAddress
import wallet.core.jni.CoinType
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Cosmos

class TestNativeInjectiveSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun NativeInjectiveTransactionSigning() {
        val key = PrivateKey("9ee18daf8e463877aaf497282abc216852420101430482a28e246c179e2c5ef1".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(false)
        val from = AnyAddress(publicKey, CoinType.NATIVEINJECTIVE).description()

        val transferAmount = Cosmos.Amount.newBuilder().apply {
            amount = "10000000000"
            denom = "inj"
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            sendCoinsMessage = Cosmos.Message.Send.newBuilder().apply {
                fromAddress = from
                toAddress = "inj1xmpkmxr4as00em23tc2zgmuyy2gr4h3wgcl6vd"
                addAllAmounts(listOf(transferAmount))
            }.build()
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "100000000000000"
            denom = "inj"
        }.build()

        val transferFee = Cosmos.Fee.newBuilder().apply {
            gas = 110000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            signingMode = Cosmos.SigningMode.Protobuf
            accountNumber = 17396
            chainId = "injective-1"
            sequence = 1
            fee = transferFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, CoinType.NATIVEINJECTIVE, Cosmos.SigningOutput.parser())

        // https://www.mintscan.io/injective/txs/135DD2C4A1910E4334A9C0F15125DA992E724EBF23FEB9638FCB71218BB064A5
        assertEquals(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"Co8BCowBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmwKKmluajEzdTZnN3ZxZ3cwNzRtZ21mMnplMmNhZHp2a3o5c25sd2NydHE4YRIqaW5qMXhtcGtteHI0YXMwMGVtMjN0YzJ6Z211eXkyZ3I0aDN3Z2NsNnZkGhIKA2luahILMTAwMDAwMDAwMDASngEKfgp0Ci0vaW5qZWN0aXZlLmNyeXB0by52MWJldGExLmV0aHNlY3AyNTZrMS5QdWJLZXkSQwpBBFoMa4O4vZgn5QcnDK20mbfjqQlSRvaiITKB94PYd8mLJWdCdBsGOfMXdo/k9MJ2JmDCESKDp2hdgVUH3uMikXMSBAoCCAEYARIcChYKA2luahIPMTAwMDAwMDAwMDAwMDAwELDbBhpAx2vkplmzeK7n3puCFGPWhLd0l/ZC/CYkGl+stH+3S3hiCvIe7uwwMpUlNaSwvT8HwF1kNUp+Sx2m0Uo1x5xcFw==\"}")
    }
}
