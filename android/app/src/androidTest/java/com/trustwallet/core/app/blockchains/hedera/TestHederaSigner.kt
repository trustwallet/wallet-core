// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.hedera

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.*
import wallet.core.jni.proto.Hedera

class TestHederaSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun HederaTransactionSimpleTransferSigning() {
        // Successfully broadcasted: https://hashscan.io/testnet/transaction/0.0.48694347-1667222879-749068449?t=1667222891.440398729&p=1
        val key =
            "e87a5584c0173263e138db689fdb2a7389025aaae7cb1a18a1017d76012130e8".toHexBytesInByteString()

        val transfer = Hedera.TransferMessage
            .newBuilder()
            .setAmount(100000000)
            .setFrom("0.0.48694347")
            .setTo("0.0.48462050")
            .build()

        val timestamp = Hedera.Timestamp
            .newBuilder()
            .setSeconds(1667222879)
            .setNanos(749068449)
            .build()

        val transactionID = Hedera.TransactionID
            .newBuilder()
            .setTransactionValidStart(timestamp)
            .setAccountID("0.0.48694347")
            .build()

        val body = Hedera.TransactionBody
            .newBuilder()
            .setMemo("")
            .setTransfer(transfer)
            .setTransactionID(transactionID)
            .setNodeAccountID("0.0.9")
            .setTransactionFee(100000000)
            .setTransactionValidDuration(120)
            .build()

        val signingInput = Hedera.SigningInput
            .newBuilder()
            .setPrivateKey(key)
            .setBody(body).build()

        val result = AnySigner.sign(signingInput, CoinType.HEDERA, Hedera.SigningOutput.parser())
        assertEquals(
            Numeric.cleanHexPrefix(Numeric.toHexString(result.encoded.toByteArray())),
            "0a440a150a0c08df9aff9a0610a1c197e502120518cb889c17120218091880c2d72f22020878721e0a1c0a0c0a0518e2f18d17108084af5f0a0c0a0518cb889c1710ff83af5f12660a640a205d3a70d08b2beafb72c7a68986b3ff819a306078b8c359d739e4966e82e6d40e1a40612589c3b15f1e3ed6084b5a3a5b1b81751578cac8d6c922f31731b3982a5bac80a22558b2197276f5bae49b62503a4d39448ceddbc5ef3ba9bee4c0f302f70c"
        )
    }
}
