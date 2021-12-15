// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.elrond

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.java.Marshalizer
import wallet.core.jni.CoinType
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Elrond
import wallet.core.jni.ElrondNetworkConfig
import wallet.core.jni.ElrondTransactionFactory

class TestElrondTransactionsFactory {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    val aliceBech32 = "erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"
    val bobBech32 = "erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"

    @Test
    fun createEGLDTransfer() {
        val networkConfig = ElrondNetworkConfig()
        val factory = ElrondTransactionFactory(networkConfig)

        val transaction = Marshalizer.unmarshalProto(factory.createEGLDTransfer(
            aliceBech32,
            bobBech32,
            "1000000000000000000"
        ), Elrond.TransactionMessage.parser())

        assertEquals(aliceBech32, transaction.sender)
        assertEquals(bobBech32, transaction.receiver)
        assertEquals("", transaction.data)
        assertEquals("1000000000000000000", transaction.value)
        assertEquals(1000000000, transaction.gasPrice)
        assertEquals(50000, transaction.gasLimit)
        assertEquals("1", transaction.chainId)
        assertEquals(1, transaction.version)
    }

    @Test
    fun createESDTTransfer() {
        val networkConfig = ElrondNetworkConfig()
        val factory = ElrondTransactionFactory(networkConfig)
        
        val transaction = Marshalizer.unmarshalProto(factory.createESDTTransfer(
            aliceBech32,
            bobBech32,
            "MYTOKEN-1234",
            "10000000000000"
        ), Elrond.TransactionMessage.parser())

        assertEquals(aliceBech32, transaction.sender)
        assertEquals(bobBech32, transaction.receiver)
        assertEquals("ESDTTransfer@4d59544f4b454e2d31323334@09184e72a000", transaction.data)
        assertEquals("0", transaction.value)
        assertEquals(1000000000, transaction.gasPrice)
        assertEquals(425000, transaction.gasLimit)
        assertEquals("1", transaction.chainId)
        assertEquals(1, transaction.version)
    }

    @Test
    fun createESDTNFTTransfer() {
        val networkConfig = ElrondNetworkConfig()
        val factory = ElrondTransactionFactory(networkConfig)
        
        val transaction = Marshalizer.unmarshalProto(factory.createESDTNFTTransfer(
            aliceBech32,
            bobBech32,
            "LKMEX-aab910",
            4,
            "184300000000000000"
        ), Elrond.TransactionMessage.parser())

        assertEquals(aliceBech32, transaction.sender)
        assertEquals(aliceBech32, transaction.receiver)
        assertEquals("ESDTNFTTransfer@4c4b4d45582d616162393130@04@028ec3dfa01ac000@c70cf50b238372fffaf7b7c5723b06b57859d424a2da621bcc1b2f317543aa36", transaction.data)
        assertEquals("0", transaction.value)
        assertEquals(1000000000, transaction.gasPrice)
        assertEquals(937500, transaction.gasLimit)
        assertEquals("1", transaction.chainId)
        assertEquals(1, transaction.version)
    }

    @Test
    fun createTransfersWithChangedNetworkConfig() {
        val networkConfig = ElrondNetworkConfig()
        networkConfig.setChainId("T")
        networkConfig.setMinGasPrice(1500000000)
        networkConfig.setMinGasLimit(60000)
        networkConfig.setGasPerDataByte(2000)
        networkConfig.setGasCostESDTTransfer(300000)
        networkConfig.setGasCostESDTNFTTransfer(300000)

        val factory = ElrondTransactionFactory(networkConfig)
        
        val tx1 = Marshalizer.unmarshalProto(factory.createEGLDTransfer(
            aliceBech32,
            bobBech32,
            "1000000000000000000"
        ), Elrond.TransactionMessage.parser())

        val tx2 = Marshalizer.unmarshalProto(factory.createESDTTransfer(
            aliceBech32,
            bobBech32,
            "MYTOKEN-1234",
            "10000000000000"
        ), Elrond.TransactionMessage.parser())

        val tx3 = Marshalizer.unmarshalProto(factory.createESDTNFTTransfer(
            aliceBech32,
            bobBech32,
            "LKMEX-aab910",
            4,
            "184300000000000000"
        ), Elrond.TransactionMessage.parser())

        assertEquals(1500000000, tx1.gasPrice)
        assertEquals(60000, tx1.gasLimit)
        assertEquals("T", tx1.chainId)

        assertEquals(1500000000, tx2.gasPrice)
        assertEquals(560000, tx2.gasLimit)
        assertEquals("T", tx2.chainId)

        assertEquals(1500000000, tx3.gasPrice)
        assertEquals(1110000, tx3.gasLimit)
        assertEquals("T", tx3.chainId)
    }
}
