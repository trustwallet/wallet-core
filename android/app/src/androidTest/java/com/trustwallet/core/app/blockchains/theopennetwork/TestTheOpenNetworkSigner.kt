// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.theopennetwork

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.TheOpenNetworkSigner
import wallet.core.jni.proto.TheOpenNetwork

class TestTheOpenNetworkSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun TheOpenNetworkTransactionSigning() {
        // TODO: Finalize implementation

        //val transfer = TheOpenNetwork.TransferMessage.newBuilder()
        //    .setTo("...")
        //    .setAmount(...)
        //    ...
        //    .build()
        //val signingInput = TheOpenNetwork.SigningInput.newBuilder()
        //    ...
        //    .build()

        //val output: TheOpenNetwork.SigningOutput = TheOpenNetworkSigner.sign(signingInput)

        //assertEquals(
        //    "__EXPECTED_RESULT_DATA__",
        //    Numeric.toHexString(output.encoded.toByteArray())
        //)
    }
}
