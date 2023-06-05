// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.mayachain

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.MAYAChainSigner
import wallet.core.jni.proto.MAYAChain

class TestMAYAChainSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun MAYAChainTransactionSigning() {
        // TODO: Finalize implementation

        //val transfer = MAYAChain.TransferMessage.newBuilder()
        //    .setTo("...")
        //    .setAmount(...)
        //    ...
        //    .build()
        //val signingInput = MAYAChain.SigningInput.newBuilder()
        //    ...
        //    .build()

        //val output: MAYAChain.SigningOutput = MAYAChainSigner.sign(signingInput)

        //assertEquals(
        //    "__EXPECTED_RESULT_DATA__",
        //    Numeric.toHexString(output.encoded.toByteArray())
        //)
    }
}
