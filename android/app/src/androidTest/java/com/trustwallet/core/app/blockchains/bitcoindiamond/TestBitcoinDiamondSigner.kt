// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.bitcoindiamond

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test

import wallet.core.java.AnySigner
import wallet.core.jni.BitcoinScript
import wallet.core.jni.BitcoinSigHashType
import wallet.core.jni.CoinType
import wallet.core.jni.proto.Bitcoin
import wallet.core.jni.proto.Common.SigningError

class TestBitcoinDiamondSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun BitcoinDiamondTransactionSigning() {
        val toScript = BitcoinScript.lockScriptForAddress("1HevQVTSEc8cEpDm65UpfCdj5erd4xxBhx", CoinType.BITCOINDIAMOND);
        assertEquals(Numeric.toHexString(toScript.data()), "0x76a914b6adfbbf15c8f6fa53f1edb37054dce5c7c145c688ac");
        
        // prepare SigningInput
        val input = Bitcoin.SigningInput.newBuilder()
            .setHashType(BitcoinScript.hashTypeForCoin(CoinType.BITCOINDIAMOND))
            .setAmount(17615)
            .setByteFee(1)
            .setToAddress("1HevQVTSEc8cEpDm65UpfCdj5erd4xxBhx")
            .setChangeAddress("1G15VvshDxwFTnahZZECJfFwEkq9fP79o8")
            .setCoinType(CoinType.BITCOINDIAMOND.value())

        val utxoKey0 =
            (Numeric.hexStringToByteArray("d2b9f2846d3adcead910ee0124a3ba7ae29e8a4729787d27f9bea1f532928eee"))
        input.addPrivateKey(ByteString.copyFrom(utxoKey0))

        // build utxo
        val txHash0 = (Numeric.hexStringToByteArray("034f4667301711e8a69236a93476ed798f9c11aaae472da5b315191a0453461d"))
        val outpoint0 = Bitcoin.OutPoint.newBuilder()
            .setHash(ByteString.copyFrom(txHash0))
            .setIndex(0)
            .setSequence(Long.MAX_VALUE.toInt())
            .build()

        val utxo0 = Bitcoin.UnspentTransaction.newBuilder()
            .setAmount(27615)
            .setOutPoint(outpoint0)
            .setScript(ByteString.copyFrom("76a914a48da46386ce52cccad178de900c71f06130c31088ac".toHexBytes()))
            .build()

        input.addUtxo(utxo0)

        val plan = AnySigner.plan(input.build(), CoinType.BITCOINDIAMOND, Bitcoin.TransactionPlan.parser())

        input.plan = Bitcoin.TransactionPlan.newBuilder()
            .mergeFrom(plan)
            .setAmount(17615)
            .setFee(10000)
            .setChange(0)
            .setPreblockhash(ByteString.copyFrom("4bfa9d92e1e99e72597e1e9162bdaaab624f1bb6faa83b2f46c6777caf8d6699".toHexBytes()))
            .build()


        val output = AnySigner.sign(input.build(), CoinType.BITCOINDIAMOND, Bitcoin.SigningOutput.parser())

        assertEquals(output.error, SigningError.OK)
        val signedTransaction = output.transaction
        assert(signedTransaction.isInitialized)
        assertEquals(12, signedTransaction.version)
        assertEquals(1, signedTransaction.inputsCount)
        assertEquals(1, signedTransaction.outputsCount)

        val encoded = output.encoded
        assertEquals("0x0c0000004bfa9d92e1e99e72597e1e9162bdaaab624f1bb6faa83b2f46c6777caf8d669901034f4667301711e8a69236a93476ed798f9c11aaae472da5b315191a0453461d000000006b483045022100da0146d41744f53d4af823d5901785014cbcad874941f72984d58a53f1bbeafa02207b4f77aa97bc3a08cf0ae5c6c36fe97d68496294286bbdaa9933e392fcb54fa0012102485a209514cc896f8ed736e205bc4c35bd5299ef3f9e84054475336b964c02a3ffffffff01cf440000000000001976a914b6adfbbf15c8f6fa53f1edb37054dce5c7c145c688ac00000000",
            Numeric.toHexString(encoded.toByteArray()));
    }
}
