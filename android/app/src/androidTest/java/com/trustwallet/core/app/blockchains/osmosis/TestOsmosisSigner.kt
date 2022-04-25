// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.osmosis

import android.util.Log
import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import com.trustwallet.core.app.utils.toHex
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*
import wallet.core.jni.CoinType.OSMOSIS
import wallet.core.jni.proto.Cosmos
import wallet.core.jni.proto.Cosmos.SigningOutput
import wallet.core.jni.proto.Cosmos.SigningMode
import wallet.core.java.AnySigner

class TestOsmosisSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun OsmosisTransactionSigning() {
        val key = PrivateKey("8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af".toHexByteArray())
        val publicKey = key.getPublicKeySecp256k1(true)
        val from = AnyAddress(publicKey, OSMOSIS).description()

        val txAmount = Cosmos.Amount.newBuilder().apply {
            amount = "99800"
            denom = "uosmo"
        }.build()

        val sendCoinsMsg = Cosmos.Message.Send.newBuilder().apply {
            fromAddress = from
            toAddress = "osmo18s0hdnsllgcclweu9aymw4ngktr2k0rkvn7jmn"
            addAllAmounts(listOf(txAmount))
        }.build()

        val message = Cosmos.Message.newBuilder().apply {
            sendCoinsMessage = sendCoinsMsg
        }.build()

        val feeAmount = Cosmos.Amount.newBuilder().apply {
            amount = "200"
            denom = "uosmo"
        }.build()

        val osmosisFee = Cosmos.Fee.newBuilder().apply {
            gas = 200000
            addAllAmounts(listOf(feeAmount))
        }.build()

        val signingInput = Cosmos.SigningInput.newBuilder().apply {
            signingMode = SigningMode.Protobuf
            accountNumber = 124703
            chainId = "osmosis-1"
            memo = ""
            sequence = 0
            fee = osmosisFee
            privateKey = ByteString.copyFrom(key.data())
            addAllMessages(listOf(message))
        }.build()

        val output = AnySigner.sign(signingInput, OSMOSIS, SigningOutput.parser())

        assertEquals(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"Co0BCooBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmoKK29zbW8xbWt5Njljbjhla3R3eTA4NDV2ZWM5dXBzZHBoa3R4dDBlbjk3ZjUSK29zbW8xOHMwaGRuc2xsZ2NjbHdldTlheW13NG5na3RyMmswcmt2bjdqbW4aDgoFdW9zbW8SBTk5ODAwEmQKTgpGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQLs71zkN6MCxn+VRo3ksx826RH0Z9fmpStBweE+HVY2SRIECgIIARISCgwKBXVvc21vEgMyMDAQwJoMGkAMY//Md5GRUR4lVZhk558hFS3kii9QZYoYKfg4+ac/xgNeyoiEweVDhcmEvlH1orVwjLUOnYs4ly2a/yIurYVj\"}")
        assertEquals(output.errorMessage, "")
    }
}
