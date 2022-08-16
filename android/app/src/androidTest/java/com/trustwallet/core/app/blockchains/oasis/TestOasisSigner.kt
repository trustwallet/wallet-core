// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.oasis

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import org.junit.Test
import org.junit.Assert.assertEquals
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.OASIS
import wallet.core.jni.proto.Oasis
import wallet.core.jni.proto.Oasis.SigningOutput

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

            gasAmount = "0"
            
            nonce = 0

            amount = "10000000"
            context = "oasis-core/consensus: tx for chain a245619497e580dd3bc1aa3256c07f68b8dcc13f92da115eadc3b231b083d3c4"
        }

        val signingInput = Oasis.SigningInput.newBuilder()
        signingInput.apply {
            privateKey = (ByteString.copyFrom(Numeric.hexStringToByteArray("4f8b5676990b00e23d9904a92deb8d8f428ff289c8939926358f1d20537c21a0")))
            transfer = transferMsg.build()
        }

        val output = AnySigner.sign(signingInput.build(), OASIS, SigningOutput.parser())
        assertEquals(
            "0xa2697369676e6174757265a2697369676e617475726558406e51c18c9b2015c9b49414b3307336597f51ff331873d214ce2db81c9651a34d99529ccaa294a39ccd01c6b0bc2c2239d87c624e5ba4840cf99ac8f9283e240c6a7075626c69635f6b6579582093d8f8a455f50527976a8aa87ebde38d5606efa86cb985d3fb466aff37000e3b73756e747275737465645f7261775f76616c7565585ea463666565a2636761730066616d6f756e74410064626f6479a262746f5500c73cc001463434915ba3f39751beb7c0905b45eb66616d6f756e744400989680656e6f6e636500666d6574686f64707374616b696e672e5472616e73666572",
            Numeric.toHexString(output.encoded.toByteArray())
        )
    }
}
