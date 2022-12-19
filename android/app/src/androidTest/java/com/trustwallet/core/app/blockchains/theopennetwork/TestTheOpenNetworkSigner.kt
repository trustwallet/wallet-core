// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.theopennetwork

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.TheOpenNetwork
import wallet.core.jni.proto.TheOpenNetwork.SigningOutput

class TestTheOpenNetworkSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun TheOpenNetworkTransactionSigning() {
        val privateKey = PrivateKey("c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0".toHexByteArray())

        val transfer = TheOpenNetwork.Transfer.newBuilder()
            .setWalletVersion(TheOpenNetwork.WalletVersion.WalletV4R2)
            .setDest("EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q")
            .setAmount(10)
            .setSeqno(6)
            .setMode(3)//TheOpenNetwork.SendMode.PAY_FEES_SEPARATELY or TheOpenNetwork.SendMode.IGNORE_ACTION_PHASE_ERRORS)
            .setExpireAt(1671132440)
            .build()

        val input = TheOpenNetwork.SigningInput.newBuilder()
            .setTransfer(transfer)
            .setPrivateKey(ByteString.copyFrom(privateKey.data()))
            .build()

        val output = AnySigner.sign(input, CoinType.THEOPENNETWORK, SigningOutput.parser())

        // tx: https://tonscan.org/tx/3Z4tHpXNLyprecgu5aTQHWtY7dpHXEoo11MAX61Xyg0=
        val expectedString = "te6ccgICAAQAAQAAALAAAAFFiAGwt/q8k4SrjbFbQCjJZfQr64ExRxcUMsWqaQODqTUijgwAAQGcEUPkil2aZ4s8KKparSep/OKHMC8vuXafFbW2HGp/9AcTRv0J5T4dwyW1G0JpHw+g5Ov6QI3Xo0O9RFr3KidICimpoxdjm3UYAAAABgADAAIBYmIAM33x4uAd+uQTyXyCZPxflESlNVHpCeoOECtNsqVW9tmIUAAAAAAAAAAAAAAAAAEAAwAA"

        assertEquals(output.encoded, expectedString)
    }
}
