// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.everscale

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.EVERSCALE
import wallet.core.jni.proto.Everscale
import wallet.core.jni.proto.Everscale.SigningOutput

class TestEverscaleSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSign() {
        val transferMessage = Everscale.Transfer.newBuilder().apply {
            bounce = false
            behavior = Everscale.MessageBehavior.SimpleTransfer
            amount = 100000000
            expiredAt = 1680770631
            to = "0:db18a67f4626f15ac0537a18445937f685f9b30184f0d7b28be4bdeb92d2fd90"
            encodedContractData = "te6ccgEBAQEAKgAAUAAAAAFLqS2KOWKN+7Y5OSiKhKisiw6t/h2ovvR3WbapyAtrdctwupw="
        }.build()
        val signingInput = Everscale.SigningInput.newBuilder().apply {
            transfer = transferMessage
            privateKey = ByteString.copyFrom("542bd4288352f1c6b270046f153d406aec054a0a06000ab9b36b5c6dd3050ad4".toHexByteArray())
        }.build()

        val output = AnySigner.sign(signingInput, EVERSCALE, SigningOutput.parser())

        // Link to the external message: https://everscan.io/messages/73807b0a3ca2d8564c023dccd5b9da222a270f68338c6fc2c064dda376a2c59d
        val expectedString = "te6ccgICAAIAAQAAAKoAAAHfiAG+Ilaz1wTyTEauoymMGl6o+NGqhszIlHS8BXAmXniYrAImASIQKH2jIwoA65IGC6aua4gAA4fFo/Nuxgb3sIRELhZnSXIS7IsE2E4D+8hk3EWGVZX+ICqlN/ka9DvXduhaXUlsUyF0MjgAAAAIHAABAGhCAG2MUz+jE3itYCm9DCIsm/tC/NmAwnhr2UXyXvXJaX7IIC+vCAAAAAAAAAAAAAAAAAAA"
        assertEquals(output.encoded, expectedString)
    }
}
