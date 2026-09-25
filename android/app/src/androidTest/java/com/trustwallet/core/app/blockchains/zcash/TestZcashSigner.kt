// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.zcash

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import org.junit.Assert.assertEquals
import org.junit.Test

import wallet.core.java.AnySigner
import wallet.core.jni.BitcoinSigHashType
import wallet.core.jni.CoinType.ZCASH
import wallet.core.jni.proto.Common.SigningError
import wallet.core.jni.proto.Bitcoin
import wallet.core.jni.proto.Bitcoin.SigningOutput
import wallet.core.jni.proto.BitcoinV2
import wallet.core.jni.proto.Utxo
import wallet.core.jni.proto.Zcash

class TestZcashSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSignZcashV2() {
        // Successfully broadcasted: https://explorer.zcha.in/transactions/ec9033381c1cc53ada837ef9981c03ead1c7c41700ff3a954389cfaddc949256
        val privateKeyData = Numeric.hexStringToByteArray("a9684f5bebd0e1208aae2e02bc9e9163bd1965ad23d8538644e1df8b99b99559")
        val dustSatoshis = 546.toLong()
        val senderAddress = "t1gWVE2uyrET2CxSmCaBiKzmWxQdHhnvMSz"
        val toAddress = "t1QahNjDdibyE4EdYkawUSKBBcVTSqv64CS"
        val sapplingBranchId = Numeric.hexStringToByteArray("0xbb09b876")

        val txid0 = Numeric.hexStringToByteArray("3a19dd44032dfed61bfca5ba5751aab8a107b30609cbd5d70dc5ef09885b6853").reversedArray()
        val utxo0 = BitcoinV2.Input.newBuilder()
            .setOutPoint(Utxo.OutPoint.newBuilder().apply {
                hash = ByteString.copyFrom(txid0)
                vout = 0
            })
            .setValue(494_000)
            .setSighashType(BitcoinSigHashType.ALL.value())
            .setReceiverAddress(senderAddress)

        val out0 = BitcoinV2.Output.newBuilder()
            .setValue(488_000)
            .setToAddress(toAddress)

        val builder = BitcoinV2.TransactionBuilder.newBuilder()
            .setVersion(BitcoinV2.TransactionVersion.UseDefault)
            .addInputs(utxo0)
            .addOutputs(out0)
            .setInputSelector(BitcoinV2.InputSelector.UseAll)
            .setFixedDustThreshold(dustSatoshis)
            // Set ZCash specific extra parameters.
            .setZcashExtraData(Zcash.TransactionBuilderExtraData.newBuilder().apply {
                branchId = ByteString.copyFrom(sapplingBranchId)
            })
        val signingInput = BitcoinV2.SigningInput.newBuilder()
            .setBuilder(builder)
            .addPrivateKeys(ByteString.copyFrom(privateKeyData))
            .setChainInfo(BitcoinV2.ChainInfo.newBuilder().apply {
                p2PkhPrefix = 184
                p2ShPrefix = 189
            })
            .build()

        val legacySigningInput = Bitcoin.SigningInput.newBuilder().apply {
            signingV2 = signingInput
            coinType = ZCASH.value()
        }

        val output = AnySigner.sign(legacySigningInput.build(), ZCASH, SigningOutput.parser())

        assertEquals(output.error, SigningError.OK)
        assertEquals(output.signingResultV2.errorMessage, "")
        assertEquals(output.signingResultV2.error, SigningError.OK)
        assertEquals(Numeric.toHexString(output.signingResultV2.encoded.toByteArray()), "0x0400008085202f890153685b8809efc50dd7d5cb0906b307a1b8aa5157baa5fc1bd6fe2d0344dd193a000000006b483045022100ca0be9f37a4975432a52bb65b25e483f6f93d577955290bb7fb0060a93bfc92002203e0627dff004d3c72a957dc9f8e4e0e696e69d125e4d8e275d119001924d3b48012103b243171fae5516d1dc15f9178cfcc5fdc67b0a883055c117b01ba8af29b953f6ffffffff0140720700000000001976a91449964a736f3713d64283fd0018626ba50091c7e988ac00000000000000000000000000000000000000")
        assertEquals(Numeric.toHexString(output.signingResultV2.txid.toByteArray()), "0xec9033381c1cc53ada837ef9981c03ead1c7c41700ff3a954389cfaddc949256")
    }
}
