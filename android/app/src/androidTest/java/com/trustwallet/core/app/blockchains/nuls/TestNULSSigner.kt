package com.trustwallet.core.app.blockchains.nuls

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.CoinType
import wallet.core.java.AnySigner
import wallet.core.jni.proto.NULS
import wallet.core.jni.proto.NULS.SigningOutput

class TestNULSSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun NULSTransactionSigning() { 
        val signingInput = NULS.SigningInput.newBuilder()
            .setPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("9ce21dad67e0f0af2599b41b515a7f7018059418bab892a7b68f283d489abc4b")))
            .setFrom("NULSd6Hgj7ZoVgsPN9ybB4C1N2TbvkgLc8Z9H")
            .setTo("NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe")
            .setAmount(ByteString.copyFrom("0x989680".toHexByteArray()))
            .setChainId(1)
            .setIdassetsId(1)
            .setNonce(ByteString.copyFrom("0000000000000000".toByteArray()))
            .setRemark("")
            .setBalance(ByteString.copyFrom("0x05f5e100".toHexByteArray()))
            .setTimestamp(0x5d8885f8)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.NULS, SigningOutput.parser())
        val encoded = output.encoded.toByteArray()
        val hex = Numeric.toHexString(encoded, 0, encoded.size, false)
        assertEquals(hex,
            "0200f885885d00008c0117010001f7ec6473df12e751d64cf20a8baa7edd50810f8101000100201d9a0000000000000000000000000000000000000000000000000000000000080000000000000000000117010001f05e7878971f3374515eabb6f16d75219d8873120100010080969800000000000000000000000000000000000000000000000000000000000000000000000000692103958b790c331954ed367d37bac901de5c2f06ac8368b37d7bd6cd5ae143c1d7e3463044022028019c0099e2233c7adb84bb03a9a5666ece4a5b65a026a090fa460f3679654702204df0fcb8762b5944b3aba033fa1a287ccb098150035dd8b66f52dc58d3d0843a"
        )
    }

    @Test
    fun NULSTokenTransactionSigning() {
        val signingInput = NULS.SigningInput.newBuilder()
            .setPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1")))
            .setFrom("NULSd6HgWabfcG6H7NDK2TJvtoU3wxY1YLKwJ")
            .setTo("NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe")
            .setAmount(ByteString.copyFrom("0x989680".toHexByteArray()))
            .setChainId(9)
            .setIdassetsId(1)
            .setNonce(ByteString.copyFrom("0000000000000000".toByteArray()))
            .setRemark("")
            .setBalance(ByteString.copyFrom("0x05f5e100".toHexByteArray()))
            .setTimestamp(0x5d8885f8)
            .setAssetNonce(ByteString.copyFrom("0000000000000000".toByteArray()))
            .build()

        val output = AnySigner.sign(signingInput, CoinType.NULS, SigningOutput.parser())
        val encoded = output.encoded.toByteArray()
        val hex = Numeric.toHexString(encoded, 0, encoded.size, false)
        assertEquals(hex,
            "0200f885885d0000d202170100012c177a01a7afbe98e094007b99476534fb7926b709000100809698000000000000000000000000000000000000000000000000000000000008000000000000000000170100012c177a01a7afbe98e094007b99476534fb7926b701000100a086010000000000000000000000000000000000000000000000000000000000080000000000000000000117010001f05e7878971f3374515eabb6f16d75219d88731209000100809698000000000000000000000000000000000000000000000000000000000000000000000000006921033c87a3d9b812556b3034b6471cad5131a01e210c1d7ca06dd53b7d0aff0ee04546304402205ddea604c6cdfcf6cbe32f5873937641676ee5f9aee3c40aa9857c59aefedff202205b77429cf62307d43a6a79b4c106123e6232e3981032573770fe2726bf9fc07c"
        )
    }
}
