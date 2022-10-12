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
        // Successfully broadcasted https://explorer.aptoslabs.com/txn/0xb4d62afd3862116e060dd6ad9848ccb50c2bc177799819f1d29c059ae2042467?network=devnet
        val key =
            "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec".toHexBytesInByteString()

        val transfer = Aptos.TransferMessage.newBuilder().setAmount(1000)
            .setTo("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30").build()
        val signingInput = Aptos.SigningInput.newBuilder().setChainId(33)
            .setSender("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30")
            .setSequenceNumber(99)
            .setGasUnitPrice(100)
            .setMaxGasAmount(3296766)
            .setExpirationTimestampSecs(3664390082)
            .setTransfer(transfer)
            .setPrivateKey(key)
            .build()

        val result = AnySigner.sign(signingInput, CoinType.APTOS, Aptos.SigningOutput.parser())
        assertEquals(
            Numeric.cleanHexPrefix(Numeric.toHexString(result.rawTxn.toByteArray())),
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada0000000021"
        )
        assertEquals(
            Numeric.cleanHexPrefix(Numeric.toHexString(result.authenticator.signature.toByteArray())),
            "5707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01"
        )
        assertEquals(
            Numeric.cleanHexPrefix(Numeric.toHexString(result.encoded.toByteArray())),
            "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada00000000210020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c405707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01"
        )
    }
}
