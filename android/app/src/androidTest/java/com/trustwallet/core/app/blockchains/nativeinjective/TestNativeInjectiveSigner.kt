// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
        assertEquals(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"Co8BCowBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmwKKmluajEzdTZnN3ZxZ3cwNzRtZ21mMnplMmNhZHp2a3o5c25sd2NydHE4YRIqaW5qMXhtcGtteHI0YXMwMGVtMjN0YzJ6Z211eXkyZ3I0aDN3Z2NsNnZkGhIKA2luahILMTAwMDAwMDAwMDASfgpeClQKLS9pbmplY3RpdmUuY3J5cHRvLnYxYmV0YTEuZXRoc2VjcDI1NmsxLlB1YktleRIjCiEDWgxrg7i9mCflBycMrbSZt+OpCVJG9qIhMoH3g9h3yYsSBAoCCAEYARIcChYKA2luahIPMTAwMDAwMDAwMDAwMDAwELDbBhpArNDBBEwHVKwuSDozLIwvOOhDQ/i7bXC6Av5ZefSQf7RS2ejrapX/JKXPsYrtMWadhKCedomODhujvWzzGwmXVA==\"}")
    }
}
