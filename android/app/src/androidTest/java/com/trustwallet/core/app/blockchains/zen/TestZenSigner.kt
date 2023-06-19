// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.zen

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

class TestZenSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun ZenTransactionSigning() {
        // prepare SigningInput
        val input = Bitcoin.SigningInput.newBuilder()
            .setHashType(BitcoinScript.hashTypeForCoin(CoinType.ZEN))
            .setAmount(10000)
            .setByteFee(1)
            .setToAddress("zngBGZGKaaBamofSuFw5WMnvU2HQAtwGeb5")
            .setChangeAddress("znk19H1wcARcCa7TM6zgmJUbWoWWtZ8k5cg")
            .setCoinType(CoinType.ZEN.value())

        val utxoKey0 =
            (Numeric.hexStringToByteArray("3a8e0a528f62f4ca2c77744c8a571def2845079b50105a9f7ef6b1b823def67a"))
        input.addPrivateKey(ByteString.copyFrom(utxoKey0))

        // build utxo
        val txHash0 = (Numeric.hexStringToByteArray("a39e13b5ab406547e31284cd96fb40ed271813939c195ae7a86cd67fb8a4de62"))
        val outpoint0 = Bitcoin.OutPoint.newBuilder()
            .setHash(ByteString.copyFrom(txHash0))
            .setIndex(0)
            .setSequence(Long.MAX_VALUE.toInt())
            .build()

        val utxo0 = Bitcoin.UnspentTransaction.newBuilder()
            .setAmount(17600)
            .setOutPoint(outpoint0)
            .setScript(ByteString.copyFrom("76a914cf83669620de8bbdf2cefcdc5b5113195603c56588ac2081dc725fd33fada1062323802eefb54d3325d924d4297a69221456040000000003e88211b4".toHexBytes()))
            .build()

        input.addUtxo(utxo0)

        val plan = AnySigner.plan(input.build(), CoinType.ZEN, Bitcoin.TransactionPlan.parser())

        input.plan = Bitcoin.TransactionPlan.newBuilder()
            .mergeFrom(plan)
            .setPreblockhash(ByteString.copyFrom("81dc725fd33fada1062323802eefb54d3325d924d4297a692214560400000000".toHexBytes()))
            .setPreblockheight(1147624)
            .build()


        val output = AnySigner.sign(input.build(), CoinType.ZEN, Bitcoin.SigningOutput.parser())
        
        assertEquals(output.error, SigningError.OK)

        val encoded = output.encoded
        assertEquals("0x0100000001a39e13b5ab406547e31284cd96fb40ed271813939c195ae7a86cd67fb8a4de62000000006a473044022014d687c0bee0b7b584db2eecbbf73b545ee255c42b8edf0944665df3fa882cfe02203bce2412d93c5a56cb4806ddd8297ff05f8fc121306e870bae33377a58a02f05012102b4ac9056d20c52ac11b0d7e83715dd3eac851cfc9cb64b8546d9ea0d4bb3bdfeffffffff0210270000000000003f76a914a58d22659b1082d1fa8698fc51996b43281bfce788ac2081dc725fd33fada1062323802eefb54d3325d924d4297a69221456040000000003e88211b4ce1c0000000000003f76a914cf83669620de8bbdf2cefcdc5b5113195603c56588ac2081dc725fd33fada1062323802eefb54d3325d924d4297a69221456040000000003e88211b400000000",
            Numeric.toHexString(encoded.toByteArray()));
    }
}
