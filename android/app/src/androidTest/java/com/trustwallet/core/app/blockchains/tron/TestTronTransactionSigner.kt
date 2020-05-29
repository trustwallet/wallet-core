package com.trustwallet.core.app.blockchains.tron

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.proto.Tron
import wallet.core.jni.proto.Tron.SigningOutput
import com.trustwallet.core.app.utils.Numeric
import org.junit.Assert.assertArrayEquals
import wallet.core.jni.CoinType.TRON

class TestTronTransactionSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSignTransferTrc20Contract() {
        val trc20Contract = Tron.TransferTRC20Contract.newBuilder()
            .setOwnerAddress("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC")
            .setContractAddress("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV")
            .setToAddress("TW1dU4L3eNm7Lw8WvieLKEHpXWAussRG9Z")
            // 1000
            .setAmount(ByteString.copyFrom("0x00000000000000000000000000000000000000000000000000000000000003e8".toHexByteArray()))

        val blockHeader = Tron.BlockHeader.newBuilder()
            .setTimestamp(1539295479000)
            .setTxTrieRoot(ByteString.copyFrom("0x64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d".toHexByteArray()))
            .setParentHash(ByteString.copyFrom("0x00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d".toHexByteArray()))
            .setNumber(3111739)
            .setWitnessAddress(ByteString.copyFrom("0x415863f6091b8e71766da808b1dd3159790f61de7d".toHexByteArray()))
            .setVersion(3)
            .build()

        val transaction = Tron.Transaction.newBuilder()
            .setTransferTrc20Contract(trc20Contract)
            .setTimestamp(1539295479000)
            .setBlockHeader(blockHeader)
            .build()

        val signingInput = Tron.SigningInput.newBuilder()
            .setTransaction(transaction)
            .setPrivateKey(ByteString.copyFrom("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54".toHexByteArray()))

        val output = AnySigner.sign(signingInput.build(), TRON, Tron.SigningOutput.parser())

        assertEquals(Numeric.toHexString(output.id.toByteArray()), "0x0d644290e3cf554f6219c7747f5287589b6e7e30e1b02793b48ba362da6a5058")
        assertEquals(Numeric.toHexString(output.signature.toByteArray()), "0xbec790877b3a008640781e3948b070740b1f6023c29ecb3f7b5835433c13fc5835e5cad3bd44360ff2ddad5ed7dc9d7dee6878f90e86a40355b7697f5954b88c01")
    }
}
