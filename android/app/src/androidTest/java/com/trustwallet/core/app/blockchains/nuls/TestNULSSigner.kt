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
            .setPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("9ce21dad67e0f0af2599b41b515a7f7018059418bab892a7b68f283d489abc4b")))
            .setFrom("NULSd6Hgj7ZoVgsPN9ybB4C1N2TbvkgLc8Z9H")
            .setTo("NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe")
            .setAmount(ByteString.copyFrom("0x989680".toHexByteArray()))
            .setChainId(9)
            .setIdassetsId(1)
            .setNonce(ByteString.copyFrom("0000000000000000".toByteArray()))
            .setRemark("")
            .setBalance(ByteString.copyFrom("0x5F5E100".toHexByteArray()))
            .setTimestamp(0x5D8885F8)
            .setFeePayer("NULSd6Hgj7ZoVgsPN9ybB4C1N2TbvkgLc8Z9H")
            .setFeePayerBalance(ByteString.copyFrom("0xF4240".toHexByteArray()))
            .setFeePayerNonce(ByteString.copyFrom("0000000000000000".toByteArray()))
            .build()

        val output = AnySigner.sign(signingInput, CoinType.NULS, SigningOutput.parser())
        val encoded = output.encoded.toByteArray()
        val hex = Numeric.toHexString(encoded, 0, encoded.size, false)
        assertEquals(hex,
            "0200f885885d0000d20217010001f7ec6473df12e751d64cf20a8baa7edd50810f810900010080969800000000000000000000000000000000000000000000000000000000000800000000000000000017010001f7ec6473df12e751d64cf20a8baa7edd50810f8101000100a086010000000000000000000000000000000000000000000000000000000000080000000000000000000117010001f05e7878971f3374515eabb6f16d75219d8873120900010080969800000000000000000000000000000000000000000000000000000000000000000000000000692103958b790c331954ed367d37bac901de5c2f06ac8368b37d7bd6cd5ae143c1d7e346304402204629e7e39708468a405f8d8dd208d0133a686beb5d3ae829b7a2f5867c74480902203d0dffac8189b1caa9087e480fd57581e8c382cc4e17034b492dd2178dac851d"
        )
    }

    @Test
    fun NULSTransactionWithFeePayerSigning() { 
        val signingInput = NULS.SigningInput.newBuilder()
            .setPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("48c91cd24a27a1cdc791022ff39316444229db1c466b3b1841b40c919dee3002")))
            .setFrom("NULSd6HgYx7bdWWv7PxYhYeTRBhD6kZs1o5Ac")
            .setTo("NULSd6HgaHjzhMEUjd4T5DFnLz9EvV4TntrdV")
            .setAmount(ByteString.copyFrom("0x186A0".toHexByteArray()))
            .setChainId(1)
            .setIdassetsId(1)
            .setNonce(ByteString.copyFrom("0000000000000000".toByteArray()))
            .setRemark("")
            .setBalance(ByteString.copyFrom("0xF4240".toHexByteArray()))
            .setTimestamp(0x62FB3F9F)
            .setFeePayer("NULSd6HgYj81NrQBFZYXvyQhHCJCkGYWDTNeA")
            .setFeePayerNonce(ByteString.copyFrom("0000000000000000".toByteArray()))
            .setFeePayerBalance(ByteString.copyFrom("0xF4240".toHexByteArray()))
            .setFeePayerPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("9401fd554cb700777e57b05338f9ff47597add8b23ce9f1c8e041e9b4e2116b6")))
            .build()

        val output = AnySigner.sign(signingInput, CoinType.NULS, SigningOutput.parser())
        val encoded = output.encoded.toByteArray()
        val hex = Numeric.toHexString(encoded, 0, encoded.size, false)
        assertEquals(hex,
            "02009f3ffb620000d202170100014f019a4227bff89d51590fbf53fbd98d994485f801000100a086010000000000000000000000000000000000000000000000000000000000080000000000000000001701000152a6388c8bf54e8fcd73cc824813bfef0912299b01000100a086010000000000000000000000000000000000000000000000000000000000080000000000000000000117010001686a3c9cd2ac45aee0ef825b0443d1eb209368b701000100a0860100000000000000000000000000000000000000000000000000000000000000000000000000d22102764370693450d654d6fc061d1d4dbfbe0c95715fd3cde7e15df073ab0983b8c8463044022040b5820b93fd5e272f2a00518af45a722571834934ba20d9a866de8e6d6409ab022003c610c647648444c1e2193634b2c51817a5a6ac3fe781da1a9ea773506afd8221025910df09ca768909cce9224d182401044c7b5bd44b0adee2ec5f2e64446573ff4630440220140e46b260942a8475f38df39bf54a2eea56c77199fc7ba775aa4b7f147d0d2102206c82705cba509f37ba0e35520a97bccb71a9e35cadcb8d95dd7fde5c8aa9e428"
        )
    }

    @Test
    fun NULSTokenTransactionWithFeePayerSigning() {
        val signingInput = NULS.SigningInput.newBuilder()
            .setPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("48c91cd24a27a1cdc791022ff39316444229db1c466b3b1841b40c919dee3002")))
            .setFrom("NULSd6HgYx7bdWWv7PxYhYeTRBhD6kZs1o5Ac")
            .setTo("NULSd6HgaHjzhMEUjd4T5DFnLz9EvV4TntrdV")
            .setAmount(ByteString.copyFrom("0x186A0".toHexByteArray()))
            .setChainId(9)
            .setIdassetsId(1)
            .setNonce(ByteString.copyFrom("0000000000000000".toByteArray()))
            .setRemark("")
            .setBalance(ByteString.copyFrom("0xDBBA0".toHexByteArray()))
            .setTimestamp(0x62FB4E4C)
            .setFeePayer("NULSd6HgYj81NrQBFZYXvyQhHCJCkGYWDTNeA")
            .setFeePayerBalance(ByteString.copyFrom("0xF4240".toHexByteArray()))
            .setFeePayerNonce(ByteString.copyFrom("e05d03df6ede0e22".toByteArray()))
            .setFeePayerPrivateKey(ByteString.copyFrom(Numeric.hexStringToByteArray("9401fd554cb700777e57b05338f9ff47597add8b23ce9f1c8e041e9b4e2116b6")))
            .build()

        val output = AnySigner.sign(signingInput, CoinType.NULS, SigningOutput.parser())
        val encoded = output.encoded.toByteArray()
        val hex = Numeric.toHexString(encoded, 0, encoded.size, false)
        assertEquals(hex,
            "02004c4efb620000d2021701000152a6388c8bf54e8fcd73cc824813bfef0912299b09000100a08601000000000000000000000000000000000000000000000000000000000008000000000000000000170100014f019a4227bff89d51590fbf53fbd98d994485f801000100a08601000000000000000000000000000000000000000000000000000000000008e05d03df6ede0e22000117010001686a3c9cd2ac45aee0ef825b0443d1eb209368b709000100a0860100000000000000000000000000000000000000000000000000000000000000000000000000d32102764370693450d654d6fc061d1d4dbfbe0c95715fd3cde7e15df073ab0983b8c8463044022025cb5b40bda4e6fc0ba719bb0c1a907b2a0faa91316ef2c836310d49f906b6a802207d530a56a6c56d974036c86125da06d6e47f9d8ba1544ac3e620cebd883a707821025910df09ca768909cce9224d182401044c7b5bd44b0adee2ec5f2e64446573ff473045022100ff6f45a1c3856f9ea954baca6b2988295bbb22c958f87f0d3baf9989930549530220426ecb1520513710b99ab50e1f6c7e21b0175adef08aa05070bb9bfca8a001d8"
        )
    }
}
