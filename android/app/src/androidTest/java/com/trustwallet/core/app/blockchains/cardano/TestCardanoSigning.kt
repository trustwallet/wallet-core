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
            "0x83a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a006acfc082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a000ca99d021a000298a3031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058403c1cbde706d10550f5f966a5071e9e01e18f7b555626ed7616a214bd951d29d0ff0458edf5c555eefe4d4280302160149fc6e7e56a4298a368b7ab357643190df6");

        val txid = output.txId
        assertEquals(Numeric.toHexString(txid.toByteArray()), "0xefcf8ec01fb8cd32bc5289c609c470b473cc79bc60b0667e1d68dc3962df1082");
    }
}
