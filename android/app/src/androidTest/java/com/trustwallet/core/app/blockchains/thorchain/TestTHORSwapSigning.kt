package com.trustwallet.core.app.blockchains.thorchainswap

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Assert.assertTrue
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.ETHEREUM
import wallet.core.jni.proto.Ethereum.SigningOutput
import wallet.core.jni.proto.THORChainSwap
import wallet.core.jni.THORChainSwap.buildSwap
import com.trustwallet.core.app.utils.Numeric

class TestTHORChainSwap {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSwapEthBnb() {
        // prepare swap input
        val input = THORChainSwap.SwapInput.newBuilder()
        input.apply {
            fromChain = THORChainSwap.Chain.ETH
            fromAddress = "0xb9f5771c27664bf2282d98e09d7f50cec7cb01a7"
            toAsset = THORChainSwap.Asset.newBuilder().apply {
                chain = THORChainSwap.Chain.BNB
                symbol = "BNB"
                tokenId = ""
            }.build()
            toAddress = "bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx"
            vaultAddress = "0x1091c4De6a3cF09CdA00AbDAeD42c7c3B69C83EC"
            routerAddress = "0x42A5Ed456650a09Dc10EBc6361A7480fDd61f27B"
            fromAmount = "50000000000000000"
            toAmountLimit = "600003"
        }

        // serialize input
        val inputSerialized = input.build().toByteArray()
        assertEquals(Numeric.toHexString(inputSerialized), "0x0802122a3078623966353737316332373636346266323238326439386530396437663530636563376362303161371a0708031203424e42222a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372782a2a307831303931633444653661336346303943644130304162444165443432633763334236394338334543322a3078343241354564343536363530613039446331304542633633363141373438306644643631663237423a1135303030303030303030303030303030304206363030303033")

        // invoke swap
        val outputData = buildSwap(inputSerialized)
        assertEquals(outputData.count(), 311)

        // parse result in proto
        val outputProto = THORChainSwap.SwapOutput.newBuilder().mergeFrom(outputData)
        assertEquals(outputProto.fromChain, THORChainSwap.Chain.ETH)
        assertEquals(outputProto.toChain, THORChainSwap.Chain.BNB)
        assertEquals(outputProto.error.code, THORChainSwap.ErrorCode.OK)
        assertTrue(outputProto.hasEthereum())
        val txInput = outputProto.ethereum

        // set few fields before signing
        val txInputFull = txInput.toBuilder().apply {
            chainId = ByteString.copyFrom("0x01".toHexByteArray())
            nonce = ByteString.copyFrom("0x03".toHexByteArray())
            gasPrice = ByteString.copyFrom("0x06FC23AC00".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x013880".toHexByteArray())
            privateKey = ByteString.copyFrom(PrivateKey("0x4f96ed80e9a7555a6f74b3d658afdd9c756b0a40d4ca30c42c2039eb449bb904".toHexByteArray()).data())
        }.build()

        // sign and encode resulting input
        val output = AnySigner.sign(txInputFull, ETHEREUM, SigningOutput.parser())

        assertEquals(Numeric.toHexString(output.encoded.toByteArray()), "0xf90151038506fc23ac00830138809442a5ed456650a09dc10ebc6361a7480fdd61f27b87b1a2bc2ec50000b8e41fece7b40000000000000000000000001091c4de6a3cf09cda00abdaed42c7c3b69c83ec000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000b1a2bc2ec500000000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000003e535741503a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a363030303033000025a06ae104be3201baca38315352f81fac70ca4dd47339981914e64e91149813e780a066a3f0b2c44ddf5a96a38481274f623f552a593d723237d6742185f4885c0064")
    }
}
