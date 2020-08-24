// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.oasisprotocol

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import junit.framework.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.OasisProtocolSigner
import wallet.core.jni.proto.OasisProtocol

class TestOasisProtocolSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun OasisProtocolTransactionSigning() {
        // TODO: Finalize implementation

        //val transfer = OasisProtocol.TransferMessage.newBuilder()
        //    .setTo("...")
        //    .setAmount(...)
        //    ...
        //    .build()
        //val signingInput = OasisProtocol.SigningInput.newBuilder()
        //    ...
        //    .build()

        //val output: OasisProtocol.SigningOutput = OasisProtocolSigner.sign(signingInput)

        //assertEquals(
        //    "__EXPECTED_RESULT_DATA__",
        //    Numeric.toHexString(output.encoded.toByteArray())
        //)
    }
}
