// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.pactus

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PactusSigner
import wallet.core.jni.proto.Pactus

class TestPactusSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun PactusTransactionSigning() {
        val privateKey =
            "4e51f1f3721f644ac7a193be7f5e7b8c2abaa3467871daf4eacb5d3af080e5d6".toHexBytesInByteString()

        val transfer = Pactus.TransferPayload.newBuilder()
            .setSender("pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr")
            .setReceiver("pc1r0g22ufzn8qtw0742dmfglnw73e260hep0k3yra")
            .setAmount(20000)
            .build()

        val transaction = Pactus.TransactionMessage.newBuilder()
            .setLockTime(0x00030201)
            .setFee(1000)
            .setMemo("test")
            .setTransfer(transfer)
            .build()

        val signingInput = Pactus.SigningInput.newBuilder()
            .setPrivateKey(privateKey)
            .setTransaction(transaction)
            .build()

        val output: Pactus.SigningOutput = PactusSigner.sign(signingInput)

        assertEquals(
            "34cd4656a98f7eb996e83efdc384cefbe3a9c52dca79a99245b4eacc0b0b4311",
            Numeric.toHexString(output.transactionId.toByteArray())
        )

        assertEquals(
            "50ac25c7125271489b0cd230549257c93fb8c6265f2914a988ba7b81c1bc47fff027412dd59447867911035ff69742d171060a1f132ac38b95acc6e39ec0bd09",
            Numeric.toHexString(output.signature.toByteArray())
        )

        assertEquals(
            "000101020300e807047465737401037098338e0b6808119dfd4457ab806b9c2059b89b037a14ae24533816e7faaa6ed28fcdde8e55a7df21a09c0150ac25c7125271489b0cd230549257c93fb8c6265f2914a988ba7b81c1bc47fff027412dd59447867911035ff69742d171060a1f132ac38b95acc6e39ec0bd0995794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa",
            Numeric.toHexString(output.signedTransactionData.toByteArray())
        )
    }
}
