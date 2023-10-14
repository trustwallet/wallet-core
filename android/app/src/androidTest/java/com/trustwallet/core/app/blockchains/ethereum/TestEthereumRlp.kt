package com.trustwallet.core.app.blockchains.ethereum

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import com.trustwallet.core.app.utils.Numeric
import wallet.core.jni.CoinType
import wallet.core.jni.proto.Common
import wallet.core.jni.proto.EthereumRlp

class TestEthereumRlp {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testEthereumRlpEncodeEip1559() {
        val chainId = ByteString.copyFrom("0x0a".toHexByteArray())
        val nonce = ByteString.copyFrom("0x06".toHexByteArray())
        val maxInclusionFeePerGas = ByteString.copyFrom("0x77359400".toHexByteArray()) // 2000000000
        val maxFeePerGas = ByteString.copyFrom("0xb2d05e00".toHexByteArray()) // 3000000000
        val gasLimit = ByteString.copyFrom("0x526c".toHexByteArray()) // 21100
        val to = "0x6b175474e89094c44da98b954eedeac495271d0f"
        val amount = 0.toLong()
        val payload = ByteString.copyFrom("a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000001ee0c29f50cb1".toHexByteArray())
        // An empty `accessList`.
        val accessList = EthereumRlp.RlpList.newBuilder().build()

        val rlpList = EthereumRlp.RlpList.newBuilder()
            .addItems(EthereumRlp.RlpItem.newBuilder().setNumberU256(chainId))
            .addItems(EthereumRlp.RlpItem.newBuilder().setNumberU256(nonce))
            .addItems(EthereumRlp.RlpItem.newBuilder().setNumberU256(maxInclusionFeePerGas))
            .addItems(EthereumRlp.RlpItem.newBuilder().setNumberU256(maxFeePerGas))
            .addItems(EthereumRlp.RlpItem.newBuilder().setNumberU256(gasLimit))
            .addItems(EthereumRlp.RlpItem.newBuilder().setAddress(to))
            .addItems(EthereumRlp.RlpItem.newBuilder().setNumberU64(amount))
            .addItems(EthereumRlp.RlpItem.newBuilder().setData(payload))
            .addItems(EthereumRlp.RlpItem.newBuilder().setList(accessList))
            .build()

        val encodingInput = EthereumRlp.EncodingInput.newBuilder().apply {
            item = EthereumRlp.RlpItem.newBuilder().setList(rlpList).build()
        }.build()

        val outputData = wallet.core.jni.EthereumRlp.encode(CoinType.ETHEREUM, encodingInput.toByteArray())
        val output = EthereumRlp.EncodingOutput.parseFrom(outputData)

        assertEquals(output.error, Common.SigningError.OK)
        assert(output.errorMessage.isEmpty())
        assertEquals(
            Numeric.toHexString(output.encoded.toByteArray()),
            "0xf86c0a06847735940084b2d05e0082526c946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000001ee0c29f50cb1c0"
        )
    }
}