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
    fun testSwapEthBnbWithFee() {
        // prepare swap input
        val input = THORChainSwap.SwapInput.newBuilder()
        input.apply {
            fromAsset = THORChainSwap.Asset.newBuilder().apply {
                chain = THORChainSwap.Chain.ETH
            }.build()
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
            affiliateFeeAddress = "tthor1ql2tcqyrqsgnql2tcqyj2n8kfdmt9lh0yzql2tcqy"
            affiliateFeeRateBp = "10"
        }

        // serialize input
        val inputSerialized = input.build().toByteArray()
        assertEquals(Numeric.toHexString(inputSerialized), "0x0a020802122a3078623966353737316332373636346266323238326439386530396437663530636563376362303161371a0708031203424e42222a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372782a2a307831303931633444653661336346303943644130304162444165443432633763334236394338334543322a3078343241354564343536363530613039446331304542633633363141373438306644643631663237423a11353030303030303030303030303030303042063630303030334a2f7474686f7231716c3274637179727173676e716c32746371796a326e386b66646d74396c6830797a716c327463717952023130")

        // invoke swap
        val outputData = buildSwap(inputSerialized)
        assertEquals(outputData.count(), 192)

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

        assertEquals(Numeric.toHexString(output.encoded.toByteArray()), "0x02f8d90103808083013880941091c4de6a3cf09cda00abdaed42c7c3b69c83ec87b1a2bc2ec50000b86e3d3a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a3630303030333a7474686f7231716c3274637179727173676e716c32746371796a326e386b66646d74396c6830797a716c32746371793a3130c001a05c16871b66fd0fa8f658d6f171310bab332d09e0533d6c97329a59ddc93a9a11a05ed2be94e6dbb640e58920c8be4fa597cd5f0a918123245acb899042dd43777f")
    }
}
