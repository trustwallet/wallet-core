// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.cardano

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.*
import wallet.core.jni.CoinType.CARDANO
import wallet.core.jni.proto.Cardano
import wallet.core.jni.proto.Cardano.SigningInput
import wallet.core.jni.proto.Cardano.SigningOutput
import wallet.core.jni.proto.Common.SigningError


class TestCardanoSigning {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSignTransfer1() {
        val message = Cardano.Transfer.newBuilder()
            .setToAddress("addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5")
            .setChangeAddress("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23")
            .setAmount(7_000_000)
            .build()
        val input = Cardano.SigningInput.newBuilder()
            .setTransferMessage(message)
            .setTtl(53333333)

        val privKey = (Numeric.hexStringToByteArray("089b68e458861be0c44bf9f7967f05cc91e51ede86dc679448a3566990b7785bd48c330875b1e0d03caaed0e67cecc42075dce1c7a13b1c49240508848ac82f603391c68824881ae3fc23a56a1a75ada3b96382db502e37564e84a5413cfaf1290dbd508e5ec71afaea98da2df1533c22ef02a26bb87b31907d0b2738fb7785b38d53aa68fc01230784c9209b2b2a2faf28491b3b1f1d221e63e704bbd0403c4154425dfbb01a2c5c042da411703603f89af89e57faae2946e2a5c18b1c5ca0e"))
        input.addPrivateKey(ByteString.copyFrom(privKey))

        val outpoint1 = Cardano.OutPoint.newBuilder()
            .setTxHash(ByteString.copyFrom(Numeric.hexStringToByteArray("f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767")))
            .setOutputIndex(1)
            .build()
        val utxo1 = Cardano.TxInput.newBuilder()
            .setOutPoint(outpoint1)
            .setAddress("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23")
            .setAmount(1_500_000)
            .build()
        input.addUtxos(utxo1)
        
        val outpoint2 = Cardano.OutPoint.newBuilder()
            .setTxHash(ByteString.copyFrom(Numeric.hexStringToByteArray("554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af0")))
            .setOutputIndex(0)
            .build()
        val utxo2 = Cardano.TxInput.newBuilder()
            .setOutPoint(outpoint2)
            .setAddress("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23")
            .setAmount(6_500_000)
            .build()
        input.addUtxos(utxo2)
        
        val output = AnySigner.sign(input.build(), CARDANO, SigningOutput.parser())
        assertEquals(output.error, SigningError.OK)

        val encoded = output.encoded
        assertEquals(Numeric.toHexString(encoded.toByteArray()),
            "0x83a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a006acfc082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a000ca96c021a000298d4031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058407cf591599852b5f5e007fdc241062405c47e519266c0d884b0767c1d4f5eacce00db035998e53ed10ca4ba5ce4aac8693798089717ce6cf4415f345cc764200ef6");

        val txid = output.txId
        assertEquals(Numeric.toHexString(txid.toByteArray()), "0x9b5b15e133cd73ccaa85307d2986aebc846505118a2eb4e6111e6b4b67d1f389");
    }

    @Test
    fun testSignTransferToken1() {
        val toToken = Cardano.TokenAmount.newBuilder()
            .setPolicyId("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77")
            .setAssetName("SUNDAE")
            .setAmount(ByteString.copyFrom(Numeric.hexStringToByteArray("01312d00"))) // 20000000
            .build()
        val toTokenBundle = Cardano.TokenBundle.newBuilder()
            .addToken(toToken)
            .build()

        // check min ADA amount, set it
        val minAmount = wallet.core.jni.Cardano.minAdaAmount(toTokenBundle.toByteArray())
        assertEquals(minAmount, 1_444_443)

        val message = Cardano.Transfer.newBuilder()
            .setToAddress("addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5")
            .setChangeAddress("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq")
            .setAmount(minAmount)
            .setUseMaxAmount(false)
            .setTokenAmount(toTokenBundle)
            .build()
        val input = Cardano.SigningInput.newBuilder()
            .setTransferMessage(message)
            .setTtl(53333333)

        val privKey = (Numeric.hexStringToByteArray("089b68e458861be0c44bf9f7967f05cc91e51ede86dc679448a3566990b7785bd48c330875b1e0d03caaed0e67cecc42075dce1c7a13b1c49240508848ac82f603391c68824881ae3fc23a56a1a75ada3b96382db502e37564e84a5413cfaf1290dbd508e5ec71afaea98da2df1533c22ef02a26bb87b31907d0b2738fb7785b38d53aa68fc01230784c9209b2b2a2faf28491b3b1f1d221e63e704bbd0403c4154425dfbb01a2c5c042da411703603f89af89e57faae2946e2a5c18b1c5ca0e"))
        input.addPrivateKey(ByteString.copyFrom(privKey))

        val outpoint1 = Cardano.OutPoint.newBuilder()
            .setTxHash(ByteString.copyFrom(Numeric.hexStringToByteArray("f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767")))
            .setOutputIndex(1)
            .build()
        val utxo1 = Cardano.TxInput.newBuilder()
            .setOutPoint(outpoint1)
            .setAddress("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23")
            .setAmount(8_051_373)
        val token3 = Cardano.TokenAmount.newBuilder()
            .setPolicyId("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77")
            .setAssetName("CUBY")
            .setAmount(ByteString.copyFrom(Numeric.hexStringToByteArray("2dc6c0"))) // 3000000
            .build()
        utxo1.addTokenAmount(token3)
        input.addUtxos(utxo1.build())

        val outpoint2 = Cardano.OutPoint.newBuilder()
            .setTxHash(ByteString.copyFrom(Numeric.hexStringToByteArray("f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767")))
            .setOutputIndex(2)
            .build()
        val utxo2 = Cardano.TxInput.newBuilder()
            .setOutPoint(outpoint2)
            .setAddress("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23")
            .setAmount(2_000_000)
        val token1 = Cardano.TokenAmount.newBuilder()
            .setPolicyId("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77")
            .setAssetName("SUNDAE")
            .setAmount(ByteString.copyFrom(Numeric.hexStringToByteArray("04d3e8d9"))) // 80996569
            .build()
        utxo2.addTokenAmount(token1)
        val token2 = Cardano.TokenAmount.newBuilder()
            .setPolicyId("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77")
            .setAssetName("CUBY")
            .setAmount(ByteString.copyFrom(Numeric.hexStringToByteArray("1e8480"))) // 2000000
            .build()
        utxo2.addTokenAmount(token2)
        input.addUtxos(utxo2.build())
        
        val output = AnySigner.sign(input.build(), CARDANO, SigningOutput.parser())
        assertEquals(output.error, SigningError.OK)

        val encoded = output.encoded
        assertEquals(Numeric.toHexString(encoded.toByteArray()),
            "0x83a40082825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76702018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd5821a00160a5ba1581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a14653554e4441451a01312d00825839018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468821a0080a574a2581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a144435542591a004c4b40581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a14653554e4441451a03a2bbd9021a0002af5e031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df290584000feb412442f8851faa59742eb2c37f3994b0d143a424367143490cf828246991e504fa8eac61c403bfa7634bd1f0adc44f3f54f6a474856701e2cbb15fb5b04f6");

        val txid = output.txId
        assertEquals(Numeric.toHexString(txid.toByteArray()), "0xdacb3a0c5b3b7fa36b49f25a0a59b941ab8a21f0db5770e9e6982ff120122649");
    }
}
