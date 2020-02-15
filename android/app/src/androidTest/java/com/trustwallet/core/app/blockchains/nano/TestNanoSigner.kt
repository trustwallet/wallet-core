package com.trustwallet.core.app.blockchains.nano

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.CoinType.NANO
import wallet.core.java.AnySigner
import wallet.core.jni.proto.Nano
import wallet.core.jni.proto.Nano.SigningOutput

class TestNanoSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSign() {
        val signingInput = Nano.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = ByteString.copyFrom("173c40e97fe2afcd24187e74f6b603cb949a5365e72fbdd065a6b165e2189e34".toHexByteArray())
            linkBlock = ByteString.copyFrom("491fca2c69a84607d374aaf1f6acd3ce70744c5be0721b5ed394653e85233507".toHexByteArray())
            representative = "xrb_3arg3asgtigae3xckabaaewkx3bzsh7nwz7jkmjos79ihyaxwphhm6qgjps4"
            balance = "96242336390000000000000000000"
        }

        val output = AnySigner.sign(signingInput.build(), NANO, SigningOutput.parser())
        val blockHash = output.blockHash.toByteArray()
        val signature = output.signature.toByteArray()
        assertEquals(blockHash.toHex(), "0xf9a323153daefe041efb94d69b9669c882c935530ed953bbe8a665dfedda9696")
        assertEquals(signature.toHex(), "0xd247f6b90383b24e612569c75a12f11242f6e03b4914eadc7d941577dcf54a3a7cb7f0a4aba4246a40d9ebb5ee1e00b4a0a834ad5a1e7bef24e11f62b95a9e09")
    }
}
