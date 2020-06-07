// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.bandchain

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import junit.framework.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.BandChainSigner
import wallet.core.jni.proto.BandChain

class TestBandChainSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun BandChainTransactionSigning() {
        // TODO: Finalize implementation

        //val transfer = BandChain.TransferMessage.newBuilder()
        //    .setTo("...")
        //    .setAmount(...)
        //    ...
        //    .build()
        //val signingInput = BandChain.SigningInput.newBuilder()
        //    ...
        //    .build()

        //val output: BandChain.SigningOutput = BandChainSigner.sign(signingInput)

        //assertEquals(
        //    "__EXPECTED_RESULT_DATA__",
        //    Numeric.toHexString(output.encoded.toByteArray())
        //)
    }
}
