// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.groestlcoin

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import org.junit.Assert.assertEquals
import org.junit.Test

import wallet.core.java.AnySigner
import wallet.core.jni.BitcoinSigHashType
import wallet.core.jni.CoinType.GROESTLCOIN
import wallet.core.jni.proto.Common.SigningError
import wallet.core.jni.proto.Bitcoin
import wallet.core.jni.proto.Bitcoin.SigningOutput
import wallet.core.jni.proto.BitcoinV2
import wallet.core.jni.proto.Utxo

class TestGroestlcoinSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSignV2P2PKH() {
        // Successfully broadcasted: https://explorer.zcha.in/transactions/ec9033381c1cc53ada837ef9981c03ead1c7c41700ff3a954389cfaddc949256
        val privateKeyData = Numeric.hexStringToByteArray("dc334e7347f2f9f72fce789b11832bdf78adf0158bc6617e6d2d2a530a0d4bc6")
        val dustSatoshis = 546.toLong()
        val senderAddress = "grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne"
        val toAddress = "31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P"
        val changeAddress = "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM"

        val txid0 = Numeric.hexStringToByteArray("8f4ecc7844e19aa1d3183e47eee89d795f9e7c875a55ec0203946d6c9eb06895").reversedArray()
        val utxo0 = BitcoinV2.Input.newBuilder()
            .setOutPoint(Utxo.OutPoint.newBuilder().apply {
                hash = ByteString.copyFrom(txid0)
                vout = 1
            })
            .setValue(4774)
            .setSighashType(BitcoinSigHashType.ALL.value())
            .setReceiverAddress(senderAddress)

        val out0 = BitcoinV2.Output.newBuilder()
            .setValue(2500)
            .setToAddress(toAddress)

        val changeOut = BitcoinV2.Output.newBuilder()
            .setValue(2048)
            .setToAddress(changeAddress)

        val builder = BitcoinV2.TransactionBuilder.newBuilder()
            .setVersion(BitcoinV2.TransactionVersion.UseDefault)
            .addInputs(utxo0)
            .addOutputs(out0)
            .addOutputs(changeOut)
            .setInputSelector(BitcoinV2.InputSelector.UseAll)
            .setFixedDustThreshold(dustSatoshis)
        val signingInput = BitcoinV2.SigningInput.newBuilder()
            .setBuilder(builder)
            .addPrivateKeys(ByteString.copyFrom(privateKeyData))
            .setChainInfo(BitcoinV2.ChainInfo.newBuilder().apply {
                p2PkhPrefix = 36
                p2ShPrefix = 5
            })
            .build()

        val legacySigningInput = Bitcoin.SigningInput.newBuilder().apply {
            signingV2 = signingInput
            coinType = GROESTLCOIN.value()
        }

        val output = AnySigner.sign(legacySigningInput.build(), GROESTLCOIN, SigningOutput.parser())

        assertEquals(output.error, SigningError.OK)
        assertEquals(output.signingResultV2.errorMessage, "")
        assertEquals(output.signingResultV2.error, SigningError.OK)
        assertEquals(Numeric.toHexString(output.signingResultV2.encoded.toByteArray()), "0x010000000001019568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f0100000000ffffffff02c40900000000000017a9140055b0c94df477ee6b9f75185dfc9aa8ce2e52e48700080000000000001976a91498af0aaca388a7e1024f505c033626d908e3b54a88ac024830450221009bbd0228dcb7343828633ded99d216555d587b74db40c4a46f560187eca222dd022032364cf6dbf9c0213076beb6b4a20935d4e9c827a551c3f6f8cbb22d8b464467012102e9c9b9b76e982ad8fa9a7f48470eafbeeba9bf6d287579318c517db5157d936e00000000")
        assertEquals(Numeric.toHexString(output.signingResultV2.txid.toByteArray()), "0x40b539c578934c9863a93c966e278fbeb3e67b0da4eb9e3030092c1b717e7a64")
    }
}
