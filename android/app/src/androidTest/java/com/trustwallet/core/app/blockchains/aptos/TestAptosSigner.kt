// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.aptos

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType
import wallet.core.jni.proto.Aptos

class TestAptosSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun AptosTransactionSigning() {
        // Successfully broadcasted https://explorer.aptoslabs.com/txn/0xbb3b3c33781c27e486afa2db854fb0a5c846d0967672feb2c6c3297a2b14e1ce?network=Devnet
        val key =
            "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec".toHexBytesInByteString()

        val transfer = Aptos.TransferMessage.newBuilder().setAmount(1000)
            .setTo("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30").build()
        val signingInput = Aptos.SigningInput.newBuilder().setChainId(32)
            .setSender("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30")
            .setSequenceNumber(15)
            .setGasUnitPrice(100)
            .setMaxGasAmount(3296766)
            .setExpirationTimestampSecs(3664390082)
            .setTransfer(transfer)
            .setPrivateKey(key)
            .build()

        val result = AnySigner.sign(signingInput, CoinType.APTOS, Aptos.SigningOutput.parser())
        assertEquals(
            Numeric.cleanHexPrefix(Numeric.toHexString(result.rawTxn.toByteArray())),
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f300f0000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e00022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada0000000020"
        )
        assertEquals(
            Numeric.cleanHexPrefix(Numeric.toHexString(result.authenticator.signature.toByteArray())),
            "2ac7acac0e597d04017b8d9ecad1ee7c2e07f3346957e507ac06508fe5c42c74892a347875d8d8826485a6e9b267bb7a0f24212be29c333c941c5db79c93ce05"
        )
        assertEquals(
            Numeric.cleanHexPrefix(Numeric.toHexString(result.encoded.toByteArray())),
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f300f0000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e00022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada00000000200020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c402ac7acac0e597d04017b8d9ecad1ee7c2e07f3346957e507ac06508fe5c42c74892a347875d8d8826485a6e9b267bb7a0f24212be29c333c941c5db79c93ce05"
        )
    }
}
