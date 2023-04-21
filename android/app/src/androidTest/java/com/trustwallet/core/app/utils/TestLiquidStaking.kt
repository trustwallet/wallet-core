// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.utils

import com.google.protobuf.ByteString
import org.junit.Assert
import wallet.core.jni.*
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.proto.Ethereum
import wallet.core.jni.proto.LiquidStaking
import wallet.core.jni.LiquidStaking as WCLiquidStaking

class TestLiquidStaking {
    init {
        System.loadLibrary("TrustWalletCore");
    }

    @Test
    fun testStraderStakeMatic() {
        val input = LiquidStaking.Input.newBuilder()
        input.apply {
            blockchain = LiquidStaking.Blockchain.POLYGON
            protocol = LiquidStaking.Protocol.Strader
            smartContractAddress = "0xfd225c9e6601c9d38d8f98d8731bf59efcf8c0e3"
            stake = LiquidStaking.Stake.newBuilder().apply {
                amount = "1000000000000000000"
                asset = LiquidStaking.Asset.newBuilder().apply {
                    stakingToken = LiquidStaking.Coin.MATIC
                }.build()
            }.build()
        }


        val inputSerialized = input.build().toByteArray()
        assertEquals(Numeric.toHexString(inputSerialized), "0x0a170a00121331303030303030303030303030303030303030222a3078666432323563396536363031633964333864386639386438373331626635396566636638633065333001")
        val outputData = WCLiquidStaking.buildRequest(inputSerialized)
        assertEquals(outputData.count(), 68)

        val outputProto = LiquidStaking.Output.newBuilder().mergeFrom(outputData)
        Assert.assertTrue(outputProto.hasEthereum())
        val txInput = outputProto.ethereum

        val txInputFull = txInput.toBuilder().apply {
            chainId = ByteString.copyFrom("0x89".toHexByteArray())
            nonce = ByteString.copyFrom("0x01".toHexByteArray())
            maxFeePerGas = ByteString.copyFrom("0x8fbcc8fcd8".toHexByteArray())
            maxInclusionFeePerGas = ByteString.copyFrom("0x085e42c7c0".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x01c520".toHexByteArray())
            privateKey = ByteString.copyFrom(PrivateKey("0x4a160b803c4392ea54865d0c5286846e7ad5e68fbd78880547697472b22ea7ab".toHexByteArray()).data())
        }.build()

        val output = AnySigner.sign(txInputFull, CoinType.ETHEREUM, Ethereum.SigningOutput.parser())

        assertEquals(Numeric.toHexString(output.encoded.toByteArray()), "0x02f87a81890185085e42c7c0858fbcc8fcd88301c52094fd225c9e6601c9d38d8f98d8731bf59efcf8c0e3880de0b6b3a764000084c78cf1a0c001a04bcf92394d53d4908130cc6d4f7b2491967f9d6c59292b84c1f56adc49f6c458a073e09f45d64078c41a7946ffdb1dee8e604eb76f318088490f8f661bb7ddfc54")
        // Successfully broadcasted: https://polygonscan.com/tx/0x0f6c4f7a893c3f08be30d2ea24479d7ed4bdba40875d07cfd607cf97980b7cf0
    }
}
