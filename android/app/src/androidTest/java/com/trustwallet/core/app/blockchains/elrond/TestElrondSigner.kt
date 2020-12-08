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
import wallet.core.jni.CoinType
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Elrond

class TestElrondSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    val aliceBech32 = "erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"
    var aliceSeedHex = "1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf"
    var alicePubKeyHex = "fd691bb5e85d102687d81079dffce842d4dc328276d2d4c60d8fd1c3433c3293"

    val bobBech32 = "erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"
    var bobSeedHex = "e3a3a3d1ac40d42d8fd4c569a9749b65a1250dd3d10b6f4e438297662ea4850e"
    var bobPubKeyHex = "c70cf50b238372fffaf7b7c5723b06b57859d424a2da621bcc1b2f317543aa36"

    @Test
    fun signTransaction() {
        val transaction = Elrond.TransactionMessage.newBuilder()
            .setNonce(0)
            .setValue("0")
            .setSender(aliceBech32)
            .setReceiver(bobBech32)
            .setGasPrice(1000000000)
            .setGasLimit(50000)
            .setData("foo")
            .setChainId("1")
            .setVersion(1)
            .build()
        
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val signingInput = Elrond.SigningInput.newBuilder()
            .setPrivateKey(privateKey)
            .setTransaction(transaction)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.ELROND, Elrond.SigningOutput.parser())
        val expectedSignature = "b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00"

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":0,"value":"0","receiver":"$bobBech32","sender":"$aliceBech32","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1,"signature":"$expectedSignature"}""", output.encoded)
    }
}
