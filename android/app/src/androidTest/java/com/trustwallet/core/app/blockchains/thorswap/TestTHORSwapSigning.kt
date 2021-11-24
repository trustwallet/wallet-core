package com.trustwallet.core.app.blockchains.ethereum

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.PrivateKey
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType
import wallet.core.jni.CoinType.ETHEREUM
import wallet.core.jni.proto.Ethereum
import wallet.core.jni.proto.Ethereum.SigningOutput
import wallet.core.jni.proto.Ethereum.TransactionMode
import wallet.core.jni.proto.THORSwap
import com.trustwallet.core.app.utils.Numeric
import org.junit.Assert.assertArrayEquals

class TestTHORSwap {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSwapEthBnb() {
        // prepare swap input
        val input = THORSwap.SwapInput.newBuilder()
        input.apply {
            fromChain = THORSwap.Chain.ETH
            fromAddress = "0xb9f5771c27664bf2282d98e09d7f50cec7cb01a7"
            toAsset = THORSwap.Asset.newBuilder().apply {
                chain = THORSwap.Chain.BNB
                symbol = "BNB"
                tokenID = ""
            }
            toAddress = "bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx"
            vaultAddress = "0x1091c4De6a3cF09CdA00AbDAeD42c7c3B69C83EC"
            routerAddress = "0x42A5Ed456650a09Dc10EBc6361A7480fDd61f27B"
            fromAmount = "50000000000000000"
            toAmountLimit = "600003"
        }

        /* TODO remove
        val signingInput = Ethereum.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom(PrivateKey("0x4646464646464646464646464646464646464646464646464646464646464646".toHexByteArray()).data())
            toAddress = "0x3535353535353535353535353535353535353535"
            chainId = ByteString.copyFrom("0x1".toHexByteArray())
            nonce = ByteString.copyFrom("0x9".toHexByteArray())
            // txMode not set, Legacy is the default
            gasPrice = ByteString.copyFrom("0x04a817c800".toHexByteArray())
            gasLimit = ByteString.copyFrom("0x5208".toHexByteArray())
            transaction = Ethereum.Transaction.newBuilder().apply {
                transfer = Ethereum.Transaction.Transfer.newBuilder().apply {
                    amount = ByteString.copyFrom("0x0de0b6b3a7640000".toHexByteArray())
                }.build()
            }.build()
        }

        val output = AnySigner.sign(signingInput.build(), ETHEREUM, SigningOutput.parser())

        assertEquals(Numeric.toHexString(output.encoded.toByteArray()), "0xf86c098504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a028ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276a067cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83")
        */
    }
}
