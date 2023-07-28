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
            .setWalletVersion(TheOpenNetwork.WalletVersion.WALLET_V4_R2)
            .setDest("EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q")
            .setAmount(10)
            .setSequenceNumber(6)
            .setMode(TheOpenNetwork.SendMode.PAY_FEES_SEPARATELY_VALUE or TheOpenNetwork.SendMode.IGNORE_ACTION_PHASE_ERRORS_VALUE)
            .setExpireAt(1671132440)
            .setBounceable(true)
            .build()

        val input = TheOpenNetwork.SigningInput.newBuilder()
            .setTransfer(transfer)
            .setPrivateKey(ByteString.copyFrom(privateKey.data()))
            .build()

        val output = AnySigner.sign(input, CoinType.TON, SigningOutput.parser())

        // tx: https://tonscan.org/tx/3Z4tHpXNLyprecgu5aTQHWtY7dpHXEoo11MAX61Xyg0=
        val expectedString = "te6ccgICAAQAAQAAALAAAAFFiAGwt/q8k4SrjbFbQCjJZfQr64ExRxcUMsWqaQODqTUijgwAAQGcEUPkil2aZ4s8KKparSep/OKHMC8vuXafFbW2HGp/9AcTRv0J5T4dwyW1G0JpHw+g5Ov6QI3Xo0O9RFr3KidICimpoxdjm3UYAAAABgADAAIBYmIAM33x4uAd+uQTyXyCZPxflESlNVHpCeoOECtNsqVW9tmIUAAAAAAAAAAAAAAAAAEAAwAA"

        assertEquals(output.encoded, expectedString)
    }

    @Test
    fun TheOpenNetworkJettonTransferSigning() {
        val privateKey = PrivateKey("c054900a527538c1b4325688a421c0469b171c29f23a62da216e90b0df2412ee".toHexByteArray())

        val transferData = TheOpenNetwork.Transfer.newBuilder()
            .setWalletVersion(TheOpenNetwork.WalletVersion.WALLET_V4_R2)
            .setDest("EQBiaD8PO1NwfbxSkwbcNT9rXDjqhiIvXWymNO-edV0H5lja")
            .setAmount(100 * 1000 * 1000)
            .setSequenceNumber(1)
            .setMode(TheOpenNetwork.SendMode.PAY_FEES_SEPARATELY_VALUE or TheOpenNetwork.SendMode.IGNORE_ACTION_PHASE_ERRORS_VALUE)
            .setExpireAt(1787693046)
            .setComment("test comment")
            .setBounceable(true)

        val jettonTransfer = TheOpenNetwork.JettonTransfer.newBuilder()
            .setTransfer(transferData)
            .setJettonAmount(500 * 1000 * 1000)
            .setToOwner("EQAFwMs5ha8OgZ9M4hQr80z9NkE7rGxUpE1hCFndiY6JnDx8")
            .setResponseAddress("EQBaKIMq5Am2p_rfR1IFTwsNWHxBkOpLTmwUain5Fj4llTXk")
            .setForwardAmount(1)
            .build()

        val input = TheOpenNetwork.SigningInput.newBuilder()
            .setJettonTransfer(jettonTransfer)
            .setPrivateKey(ByteString.copyFrom(privateKey.data()))
            .build()

        val output = AnySigner.sign(input, CoinType.TON, SigningOutput.parser())

        // tx: https://testnet.tonscan.org/tx/Er_oT5R3QK7D-qVPBKUGkJAOOq6ayVls-mgEphpI9Ck=
        val expectedString = "te6ccgICAAQAAQAAARgAAAFFiAC0UQZVyBNtT/W+jqQKnhYasPiDIdSWnNgo1FPyLHxLKgwAAQGcaIWVosi1XnveAmoG9y0/mPeNUqUu7GY76mdbRAaVeNeDOPDlh5M3BEb26kkc6XoYDekV60o2iOobN+TGS76jBSmpoxdqjgf2AAAAAQADAAIBaGIAMTQfh52puD7eKUmDbhqfta4cdUMRF662Uxp3zzqug/MgL68IAAAAAAAAAAAAAAAAAAEAAwDKD4p+pQAAAAAAAAAAQdzWUAgAC4GWcwteHQM+mcQoV+aZ+myCd1jYqUiawhCzuxMdEzkAFoogyrkCban+t9HUgVPCw1YfEGQ6ktObBRqKfkWPiWVCAgAAAAB0ZXN0IGNvbW1lbnQ="

        assertEquals(output.encoded, expectedString)
    }
}
