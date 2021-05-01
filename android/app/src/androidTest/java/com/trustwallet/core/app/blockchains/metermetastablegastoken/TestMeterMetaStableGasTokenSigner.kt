// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.metermetastablegastoken

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.MeterMetaStableGasTokenSigner
import wallet.core.jni.proto.MeterMetaStableGasToken

class TestMeterMetaStableGasTokenSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun MeterMetaStableGasTokenTransactionSigning() {
        // TODO: Finalize implementation

        //val transfer = MeterMetaStableGasToken.TransferMessage.newBuilder()
        //    .setTo("...")
        //    .setAmount(...)
        //    ...
        //    .build()
        //val signingInput = MeterMetaStableGasToken.SigningInput.newBuilder()
        //    ...
        //    .build()

        //val output: MeterMetaStableGasToken.SigningOutput = MeterMetaStableGasTokenSigner.sign(signingInput)

        //assertEquals(
        //    "__EXPECTED_RESULT_DATA__",
        //    Numeric.toHexString(output.encoded.toByteArray())
        //)
    }
}
