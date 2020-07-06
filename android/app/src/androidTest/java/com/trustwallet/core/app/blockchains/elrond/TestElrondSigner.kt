// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.elrond

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import junit.framework.Assert.assertEquals
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
            .setGasPrice(200000000000000)
            .setGasLimit(500000000)
            .setData("foo")
            .setChainId("m1.0")
            .setVersion(1)
            .build()
        
        val privateKey = ByteString.copyFrom(PrivateKey(aliceSeedHex.toHexByteArray()).data())

        val signingInput = Elrond.SigningInput.newBuilder()
            .setPrivateKey(privateKey)
            .setTransaction(transaction)
            .build()

        val output = AnySigner.sign(signingInput, CoinType.ELROND, Elrond.SigningOutput.parser())
        val expectedSignature = "fd77f627294c2cad3c4b0c761cad70e886fa1cfd119803caa2bcbc2d5ed3518df3e7531de9daa8ab47b278ac97a0cca5797868bdaba759845ce8c2c91162c30f"

        assertEquals(expectedSignature, output.signature)
        assertEquals("""{"nonce":0,"value":"0","receiver":"$bobBech32","sender":"$aliceBech32","gasPrice":200000000000000,"gasLimit":500000000,"data":"foo","chainID":"m1.0","version":1,"signature":"$expectedSignature"}""", output.encoded)
    }
}
