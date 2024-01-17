// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.greenfield

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Greenfield

class TestGreenfieldSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun GreenfieldTransactionSigningSend() {
        // Successfully broadcasted: https://greenfieldscan.com/tx/ED8508F3C174C4430B8EE718A6D6F0B02A8C516357BE72B1336CF74356529D19

        val key =
            PrivateKey("825d2bb32965764a98338139412c7591ed54c951dd65504cd8ddaeaa0fea7b2a".toHexByteArray())

        val msgSend = Greenfield.Message.Send.newBuilder().apply {
            fromAddress = "0xA815ae0b06dC80318121745BE40e7F8c6654e9f3"
            toAddress = "0x8dbD6c7Ede90646a61Bbc649831b7c298BFd37A0"
            addAmounts(Greenfield.Amount.newBuilder().apply {
                amount = "1234500000000000"
                denom = "BNB"
            })
        }.build()

        val greenfieldFee = Greenfield.Fee.newBuilder().apply {
            gas = 1200
            addAmounts(Greenfield.Amount.newBuilder().apply {
                amount = "6000000000000"
                denom = "BNB"
            })
        }.build()

        val signingInput = Greenfield.SigningInput.newBuilder().apply {
            signingMode = Greenfield.SigningMode.Eip712
            encodingMode = Greenfield.EncodingMode.Protobuf
            accountNumber = 15952
            ethChainId = "5600"
            cosmosChainId = "greenfield_5600-1"
            memo = "Trust Wallet test memo"
            sequence = 0
            fee = greenfieldFee
            mode = Greenfield.BroadcastMode.SYNC
            privateKey = ByteString.copyFrom(key.data())
            addMessages(Greenfield.Message.newBuilder().apply {
                sendCoinsMessage = msgSend
            })
        }.build()

        val output = AnySigner.sign(signingInput, CoinType.GREENFIELD, Greenfield.SigningOutput.parser())

        assertEquals(
            output.serialized,
            "{\"mode\":\"BROADCAST_MODE_SYNC\",\"tx_bytes\":\"CqwBCpEBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnEKKjB4QTgxNWFlMGIwNmRDODAzMTgxMjE3NDVCRTQwZTdGOGM2NjU0ZTlmMxIqMHg4ZGJENmM3RWRlOTA2NDZhNjFCYmM2NDk4MzFiN2MyOThCRmQzN0EwGhcKA0JOQhIQMTIzNDUwMDAwMDAwMDAwMBIWVHJ1c3QgV2FsbGV0IHRlc3QgbWVtbxJzClYKTQomL2Nvc21vcy5jcnlwdG8uZXRoLmV0aHNlY3AyNTZrMS5QdWJLZXkSIwohAhm/mQgs8vzaqBLW66HrqQNv86PYTBgXyElU1OiuKD/sEgUKAwjIBRIZChQKA0JOQhINNjAwMDAwMDAwMDAwMBCwCRpBwbRb1qEAWwaqVfmp1Mn7iMi7wwV/oPi2J2eW9NBIdNoky+ZL+uegS/kY+funCOrqVZ+Kbol9/djAV+bQaNUB0xw=\"}"
        )
        assertEquals(output.errorMessage, "")
    }

    @Test
    fun GreenfieldTransactionSigningTransferOut() {
        // Successfully broadcasted Greenfield: https://greenfieldscan.com/tx/38C29C530A74946CFD22EE07DA642F5EF9399BC9AEA59EC56A9B5BE16DE16CE7
        // BSC (parent transaction): https://testnet.bscscan.com/tx/0x7f73c8a362e14e58cb5e0ec17616afc50eff7aa398db472383a6d017c8a5861a

        val key =
            PrivateKey("9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0".toHexByteArray())

        val msgTransferOut = Greenfield.Message.BridgeTransferOut.newBuilder().apply {
            fromAddress = "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1"
            toAddress = "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1"
            amount = Greenfield.Amount.newBuilder().apply {
                amount = "5670000000000000"
                denom = "BNB"
            }.build()
        }.build()

        val greenfieldFee = Greenfield.Fee.newBuilder().apply {
            gas = 1200
            addAmounts(Greenfield.Amount.newBuilder().apply {
                amount = "6000000000000"
                denom = "BNB"
            })
        }.build()

        val signingInput = Greenfield.SigningInput.newBuilder().apply {
            signingMode = Greenfield.SigningMode.Eip712
            encodingMode = Greenfield.EncodingMode.Protobuf
            accountNumber = 15560
            ethChainId = "5600"
            cosmosChainId = "greenfield_5600-1"
            sequence = 7
            fee = greenfieldFee
            mode = Greenfield.BroadcastMode.SYNC
            privateKey = ByteString.copyFrom(key.data())
            addMessages(Greenfield.Message.newBuilder().apply {
                bridgeTransferOut = msgTransferOut
            })
        }.build()

        val output = AnySigner.sign(signingInput, CoinType.GREENFIELD, Greenfield.SigningOutput.parser())

        assertEquals(
            output.serialized,
            "{\"mode\":\"BROADCAST_MODE_SYNC\",\"tx_bytes\":\"CpkBCpYBCiEvZ3JlZW5maWVsZC5icmlkZ2UuTXNnVHJhbnNmZXJPdXQScQoqMHg5ZDFkOTdhREZjZDMyNEJiZDYwM0QzODcyQkQ3OGUwNDA5ODUxMGIxEioweDlkMWQ5N2FERmNkMzI0QmJkNjAzRDM4NzJCRDc4ZTA0MDk4NTEwYjEaFwoDQk5CEhA1NjcwMDAwMDAwMDAwMDAwEnUKWApNCiYvY29zbW9zLmNyeXB0by5ldGguZXRoc2VjcDI1NmsxLlB1YktleRIjCiECee80Bk2hDbBGPHBIBha6AgcD7DpFAm3ve+vSCC9db8gSBQoDCMgFGAcSGQoUCgNCTkISDTYwMDAwMDAwMDAwMDAQsAkaQc4DDByhu80Uy/M3sQePvAmhmbFWZeGq359rugtskEiXKfCzSB86XmBi+l+Q5ETDS2folMxbufHSE8gM4WBCHzgc\"}"
        )
        assertEquals(output.errorMessage, "")
    }
}
