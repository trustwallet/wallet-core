// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.newchain

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.NewChain

class TestNewChainSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun NewChainTransactionSigning() {
        val signingInput = NewChain.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("298b9bee0a0431e8f1a81323df6810b72467db21f9c252cb6d134d149005a386".toHexByteArray()).data())
            toAddress = "0x524e6cc2bfa3b2d3f35818ab47a3fcd5621a4ff9"
            chainId = ByteString.copyFrom("0x1".toHexByteArray())
            nonce = ByteString.copyFrom("0x1".toHexByteArray())
            // txMode not set, Legacy is the default
            gasPrice = ByteString.copyFrom("0x1".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x1".toHexByteArray())
            transaction = NewChain.Transaction.newBuilder().apply {
                transfer = NewChain.Transaction.Transfer.newBuilder().apply {
                    amount = ByteString.copyFrom("0x1".toHexByteArray())
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), CoinType.NEWCHAIN, NewChain.SigningOutput.parser())
        assertEquals(Numeric.toHexString(output.encoded.toByteArray()), "0xf85d01010194524e6cc2bfa3b2d3f35818ab47a3fcd5621a4ff9018025a00740b87399b6ef8ce07fbf492613fd3ca91ec630b24ae92a8a006b1cc42c6f72a04730ce1edf6fa0a2ae59caf2683a9632d9266d2bee05a88955958ce46205c0f6")
    }
}
