// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
            .setDest("EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q")
            .setAmount(ByteString.copyFrom("0A".toHexByteArray())) // 10
            .setMode(TheOpenNetwork.SendMode.PAY_FEES_SEPARATELY_VALUE or TheOpenNetwork.SendMode.IGNORE_ACTION_PHASE_ERRORS_VALUE)
            .setBounceable(true)
            .build()

        val input = TheOpenNetwork.SigningInput.newBuilder()
            .setPrivateKey(ByteString.copyFrom(privateKey.data()))
            .addMessages(transfer)
            .setSequenceNumber(6)
            .setExpireAt(1671132440)
            .setWalletVersion(TheOpenNetwork.WalletVersion.WALLET_V4_R2)
            .build()

        val output = AnySigner.sign(input, CoinType.TON, SigningOutput.parser())

        // tx: https://tonscan.org/tx/3Z4tHpXNLyprecgu5aTQHWtY7dpHXEoo11MAX61Xyg0=
        val expectedString = "te6cckEBBAEArQABRYgBsLf6vJOEq42xW0AoyWX0K+uBMUcXFDLFqmkDg6k1Io4MAQGcEUPkil2aZ4s8KKparSep/OKHMC8vuXafFbW2HGp/9AcTRv0J5T4dwyW1G0JpHw+g5Ov6QI3Xo0O9RFr3KidICimpoxdjm3UYAAAABgADAgFiYgAzffHi4B365BPJfIJk/F+URKU1UekJ6g4QK02ypVb22YhQAAAAAAAAAAAAAAAAAQMAAA08Nzs="

        assertEquals(output.encoded, expectedString)
    }

    @Test
    fun TheOpenNetworkJettonTransferSigning() {
        val privateKey = PrivateKey("c054900a527538c1b4325688a421c0469b171c29f23a62da216e90b0df2412ee".toHexByteArray())

        val jettonTransfer = TheOpenNetwork.JettonTransfer.newBuilder()
            .setJettonAmount(ByteString.copyFrom("1DCD6500".toHexByteArray())) // 500 * 1000 * 1000
            .setToOwner("EQAFwMs5ha8OgZ9M4hQr80z9NkE7rGxUpE1hCFndiY6JnDx8")
            .setResponseAddress("EQBaKIMq5Am2p_rfR1IFTwsNWHxBkOpLTmwUain5Fj4llTXk")
            .setForwardAmount(ByteString.copyFrom("01".toHexByteArray())) // 1
            .build()

        val transfer = TheOpenNetwork.Transfer.newBuilder()
            .setDest("EQBiaD8PO1NwfbxSkwbcNT9rXDjqhiIvXWymNO-edV0H5lja")
            .setAmount(ByteString.copyFrom("05F5E100".toHexByteArray())) // 100 * 1000 * 1000
            .setMode(TheOpenNetwork.SendMode.PAY_FEES_SEPARATELY_VALUE or TheOpenNetwork.SendMode.IGNORE_ACTION_PHASE_ERRORS_VALUE)
            .setComment("test comment")
            .setBounceable(true)
            .setJettonTransfer(jettonTransfer)

        val input = TheOpenNetwork.SigningInput.newBuilder()
            .setPrivateKey(ByteString.copyFrom(privateKey.data()))
            .addMessages(transfer)
            .setSequenceNumber(1)
            .setExpireAt(1787693046)
            .setWalletVersion(TheOpenNetwork.WalletVersion.WALLET_V4_R2)
            .build()

        val output = AnySigner.sign(input, CoinType.TON, SigningOutput.parser())

        // tx: https://testnet.tonscan.org/tx/Er_oT5R3QK7D-qVPBKUGkJAOOq6ayVls-mgEphpI9Ck=
        val expectedString = "te6cckECBAEAARUAAUWIALRRBlXIE21P9b6OpAqeFhqw+IMh1Jac2CjUU/IsfEsqDAEBnGiFlaLItV573gJqBvctP5j3jVKlLuxmO+pnW0QGlXjXgzjw5YeTNwRG9upJHOl6GA3pFetKNojqGzfkxku+owUpqaMXao4H9gAAAAEAAwIBaGIAMTQfh52puD7eKUmDbhqfta4cdUMRF662Uxp3zzqug/MgL68IAAAAAAAAAAAAAAAAAAEDAMoPin6lAAAAAAAAAABB3NZQCAALgZZzC14dAz6ZxChX5pn6bIJ3WNipSJrCELO7Ex0TOQAWiiDKuQJtqf630dSBU8LDVh8QZDqS05sFGop+RY+JZUICAAAAAHRlc3QgY29tbWVudG/bd5c="

        assertEquals(output.encoded, expectedString)
    }

    @Test
    fun TheOpenNetworkTransferCustomPayload() {
        val privateKey = PrivateKey("5525e673087587bc0efd7ab09920ef7d3c1bf6b854a661430244ca59ab19e9d1".toHexByteArray())

        // Doge chatbot contract payload to be deployed.
        // Docs: https://docs.ton.org/develop/dapps/ton-connect/transactions#smart-contract-deployment
        val dogeChatbotStateInit = "te6cckEBBAEAUwACATQBAgEU/wD0pBP0vPLICwMAEAAAAZDrkbgQAGrTMAGCCGlJILmRMODQ0wMx+kAwi0ZG9nZYcCCAGMjLBVAEzxaARfoCE8tqEssfAc8WyXP7AO4ioYU="
        // Doge chatbot's address after the contract is deployed.
        val dogeChatbotDeployingAddress = "0:3042cd5480da232d5ac1d9cbe324e3c9eb58f167599f6b7c20c6e638aeed0335"

        // The comment has nothing to do with Doge chatbot.
        // It's just used to attach the following ASCII comment to the transaction:
        // "This transaction deploys Doge Chatbot contract"
        val commentPayload = "te6cckEBAQEANAAAZAAAAABUaGlzIHRyYW5zYWN0aW9uIGRlcGxveXMgRG9nZSBDaGF0Ym90IGNvbnRyYWN0v84vSg=="

        val transfer = TheOpenNetwork.Transfer.newBuilder()
            .setDest(dogeChatbotDeployingAddress)
            // 0.069 TON
            .setAmount(ByteString.copyFrom("041CDB40".toHexByteArray())) // 69_000_000
            .setMode(TheOpenNetwork.SendMode.PAY_FEES_SEPARATELY_VALUE or TheOpenNetwork.SendMode.IGNORE_ACTION_PHASE_ERRORS_VALUE)
            .setBounceable(false)
            .setStateInit(dogeChatbotStateInit)
            .setCustomPayload(commentPayload)

        val input = TheOpenNetwork.SigningInput.newBuilder()
            .setPrivateKey(ByteString.copyFrom(privateKey.data()))
            .addMessages(transfer)
            .setSequenceNumber(4)
            .setExpireAt(1721939714)
            .setWalletVersion(TheOpenNetwork.WalletVersion.WALLET_V4_R2)
            .build()

        val output = AnySigner.sign(input, CoinType.TON, SigningOutput.parser())

        // Successfully broadcasted: https://tonviewer.com/transaction/f4b7ed2247b1adf54f33dd2fd99216fbd61beefb281542d0b330ccea9b8d0338
        val expectedString = "te6cckECCAEAATcAAUWIAfq4NsPLegfou/MPhtHE9YuzV3gnI/q6jm3MRJh2PtpaDAEBnPbyCSsWrOZpEjb7ZFxz5yYi+an6M6Lnq7rI7TFWdDS76LEtGBrVVrhMGziwxuy6LCVtsMBikI7RPVQ89FCIAAYpqaMXZqK3AgAAAAQAAwICaUIAGCFmqkBtEZatYOzl8ZJx5PWseLOsz7W+EGNzHFd2gZqgIObaAAAAAAAAAAAAAAAAAAPAAwQCATQFBgBkAAAAAFRoaXMgdHJhbnNhY3Rpb24gZGVwbG95cyBEb2dlIENoYXRib3QgY29udHJhY3QBFP8A9KQT9LzyyAsHABAAAAGQ65G4EABq0zABgghpSSC5kTDg0NMDMfpAMItGRvZ2WHAggBjIywVQBM8WgEX6AhPLahLLHwHPFslz+wAa2r/S"

        assertEquals(output.encoded, expectedString)
    }
}
