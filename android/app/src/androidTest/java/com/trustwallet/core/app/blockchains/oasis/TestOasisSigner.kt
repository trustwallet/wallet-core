// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.oasis

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import junit.framework.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.OASIS
import wallet.core.jni.proto.Oasis
import wallet.core.jni.proto.Oasis.SigningOutput
import java.math.BigInteger

class TestOasisSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun OasisTransactionSigning() {
        val transferMsg = Oasis.TransferMessage.newBuilder()
        transferMsg.apply {
            to = "oasis1qrrnesqpgc6rfy2m50eew5d7klqfqk69avhv4ak5"
            gasPrice = 0
            gasAmount = ByteString.copyFrom("\u00000".toByteArray())
            nonce = 0
            method = "staking.Transfer"
            amount = ByteString.copyFrom("\u000010000000".toByteArray()) // decimal 10000000
            context = "oasis-core/consensus: tx for chain bc1c715319132305795fa86bd32e93291aaacbfb5b5955f3ba78bdba413af9e1"
        }

        val signingInput = Oasis.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = (ByteString.copyFrom(Numeric.hexStringToByteArray("4f8b5676990b00e23d9904a92deb8d8f428ff289c8939926358f1d20537c21a0")))
            transfer = transferMsg.build()
        }

        val output = AnySigner.sign(signingInput.build(), OASIS, SigningOutput.parser())
        assertEquals(
            "0x82880000420030404900313030303030303000707374616b696e672e5472616e7366657278636f617369732d636f72652f636f6e73656e7375733a20747820666f7220636861696e20626331633731353331393133323330353739356661383662643332653933323931616161636266623562353935356633626137386264626134313361663965315840cfe091da8d2d148a053591f435d711adf6f79105a73343bdfe32a30520c5a421d32429a52eb2d5d70ef3d161712abf3696d4072d4289c9771a6e78f5c0299d0b",
            Numeric.toHexString(output.encoded.toByteArray())
        )
    }
}
