package com.trustwallet.core.app.blockchains.solana

import com.google.protobuf.ByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.Base58
import wallet.core.jni.CoinType.SOLANA
import wallet.core.jni.WalletConnectRequest
import wallet.core.jni.proto.Common.SigningError
import wallet.core.jni.proto.Solana.Encoding
import wallet.core.jni.proto.Solana.SigningOutput
import wallet.core.jni.proto.WalletConnect

class TestSolanaWalletConnectSigning {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSignSolanaTransactionFromWalletConnectRequest() {
        // Step 1: Parse a signing request received through WalletConnect.

        val parsingInput = WalletConnect.ParseRequestInput.newBuilder().apply {
            method = WalletConnect.Method.SolanaSignTransaction
            payload = "{\"transaction\":\"AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAAEDZsL1CMnFVcrMn7JtiOiN1U4hC7WovOVof2DX51xM0H/GizyJTHgrBanCf8bGbrFNTn0x3pCGq30hKbywSTr6AgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAgIAAQwCAAAAKgAAAAAAAAA=\"}"
        }.build()

        val parsingOutputBytes = WalletConnectRequest.parse(SOLANA, parsingInput.toByteArray())
        val parsingOutput = WalletConnect.ParseRequestOutput.parseFrom(parsingOutputBytes)

        assertEquals(parsingOutput.error, SigningError.OK)

        // Step 2: Set missing fields.

        val signingInput = parsingOutput.solana.toBuilder().apply {
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr"))
            txEncoding = Encoding.Base64
        }.build()

        // Step 3: Sign the transaction.

        val output = AnySigner.sign(signingInput, SOLANA, SigningOutput.parser())

        assertEquals(output.error, SigningError.OK)
        assertEquals(output.encoded, "AQPWaOi7dMdmQpXi8HyQQKwiqIftrg1igGQxGtZeT50ksn4wAnyH4DtDrkkuE0fqgx80LTp4LwNN9a440SrmoA8BAAEDZsL1CMnFVcrMn7JtiOiN1U4hC7WovOVof2DX51xM0H/GizyJTHgrBanCf8bGbrFNTn0x3pCGq30hKbywSTr6AgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAgIAAQwCAAAAKgAAAAAAAAA=")

        assertEquals(output.getSignatures(0).pubkey, "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q")
        assertEquals(output.getSignatures(0).signature, "5T6uZBHnHFd8uWErDBTFRVkbKuhbcm94K5MJ2beTYDruzqv4FjS7EMKvC94ZfxNAiWUXZ6bZxS3WXUbhJwYNPWn")
    }
}