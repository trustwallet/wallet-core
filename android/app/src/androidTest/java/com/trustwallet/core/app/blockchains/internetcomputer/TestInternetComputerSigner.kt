// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.internetcomputer

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.*
import wallet.core.jni.proto.InternetComputer
import wallet.core.jni.proto.InternetComputer.SigningOutput

class TestInternetComputerSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun InternetComputerTransactionSigning() {
        val key = PrivateKey("227102911bb99ce7285a55f952800912b7d22ebeeeee59d77fc33a5d7c7080be".toHexByteArray())

        val input = InternetComputer.SigningInput.newBuilder()
            .setTransaction(InternetComputer.Transaction.newBuilder().apply {
                transfer = InternetComputer.Transaction.Transfer.newBuilder().apply {
                    toAccountIdentifier = "943d12e762f43806782f524b8f90297298a6d79e4749b41b585ec427409c826a"
                    amount = 100000000
                    memo = 0
                    currentTimestampSecs = 1691709940
                }.build()
            }.build())
            .setPrivateKey(ByteString.copyFrom(key.data()))
        val output = AnySigner.sign(input.build(), CoinType.INTERNETCOMPUTER, SigningOutput.parser())
        assertEquals(output.signedTransaction.toByteArray().toHex(), "0x81826b5452414e53414354494f4e81a266757064617465a367636f6e74656e74a66c726571756573745f747970656463616c6c6e696e67726573735f6578706972791b177a297215cfe8006673656e646572581d971cd2ddeecd1cf1b28be914d7a5c43441f6296f1f9966a7c8aff68d026b63616e69737465725f69644a000000000000000201016b6d6574686f645f6e616d656773656e645f70626361726758400a0012070a050880c2d72f1a0308904e2a220a20943d12e762f43806782f524b8f90297298a6d79e4749b41b585ec427409c826a3a0a088090caa5a3a78abd176d73656e6465725f7075626b65799858183018561830100607182a1886184818ce183d02010605182b188104000a0318420004183d18ab183a182118a81838184d184c187e1852188a187e18dc18d8184418ea18cd18c5189518ac188518b518bc181d188515186318bc18e618ab18d2184318d3187c184f18cd18f018de189b18b5181918dd18ef1889187218e71518c40418d4189718881843187218c611182e18cc18e6186b182118630218356a73656e6465725f736967984018d8189d18ee188a1118a81858184018da188d188c18b800184c18f611182718ea18931899186f183318c518711848186718841718351825181e187c18710018a21871183618f6184b18cd18e618e418ea182c18d118c91857188d140c18b4182a188018c51871189f1418b518cf182e18b618a418fd18a36a726561645f7374617465a367636f6e74656e74a46c726571756573745f747970656a726561645f73746174656e696e67726573735f6578706972791b177a297215cfe8006673656e646572581d971cd2ddeecd1cf1b28be914d7a5c43441f6296f1f9966a7c8aff68d0265706174687381824e726571756573745f7374617475735820b20f43257a5e87542693561e20a6076d515395e49623fcecd6c5b5640b8db8c36d73656e6465725f7075626b65799858183018561830100607182a1886184818ce183d02010605182b188104000a0318420004183d18ab183a182118a81838184d184c187e1852188a187e18dc18d8184418ea18cd18c5189518ac188518b518bc181d188515186318bc18e618ab18d2184318d3187c184f18cd18f018de189b18b5181918dd18ef1889187218e71518c40418d4189718881843187218c611182e18cc18e6186b182118630218356a73656e6465725f736967984018a8189b12186d18a4188d18fb18f71869187918f70e1825181d185a0318440b18e0186e1820183f1834188016186818dd183b18d518de18941849187e1882186e18591861187218ac0a18de18df1858183718b6182818930c18431864183718f60a18ef1824185e18ed184e18841839185718d5091888")
    }

    @Test
    fun InternetComputerTransactionSigningWithInvalidToAccountIdentifier() {
        val key = PrivateKey("227102911bb99ce7285a55f952800912b7d22ebeeeee59d77fc33a5d7c7080be".toHexByteArray())

        val input = InternetComputer.SigningInput.newBuilder()
            .setTransaction(InternetComputer.Transaction.newBuilder().apply {
                transfer = InternetComputer.Transaction.Transfer.newBuilder().apply {
                    toAccountIdentifier = "643d12e762f43806782f524b8f90297298a6d79e4749b41b585ec427409c826b"
                    amount = 100000000
                    memo = 0
                    currentTimestampSecs = 1691709940
                }.build()
            }.build())
            .setPrivateKey(ByteString.copyFrom(key.data()))
        val output = AnySigner.sign(input.build(), CoinType.INTERNETCOMPUTER, SigningOutput.parser())
        assertEquals(output.error.number, 16)
    }

    @Test
    fun InternetComputerTransactionSigningWithInvalidAmount() {
        val key = PrivateKey("227102911bb99ce7285a55f952800912b7d22ebeeeee59d77fc33a5d7c7080be".toHexByteArray())

        val input = InternetComputer.SigningInput.newBuilder()
            .setTransaction(InternetComputer.Transaction.newBuilder().apply {
                transfer = InternetComputer.Transaction.Transfer.newBuilder().apply {
                    toAccountIdentifier = "943d12e762f43806782f524b8f90297298a6d79e4749b41b585ec427409c826a"
                    amount = 0
                    memo = 0
                    currentTimestampSecs = 1691709940
                }.build()
            }.build())
            .setPrivateKey(ByteString.copyFrom(key.data()))
        val output = AnySigner.sign(input.build(), CoinType.INTERNETCOMPUTER, SigningOutput.parser())
        assertEquals(output.error.number, 23)
    }
}