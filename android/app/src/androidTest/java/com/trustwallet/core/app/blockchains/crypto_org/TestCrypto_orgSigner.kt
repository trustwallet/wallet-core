// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.crypto_org

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.Crypto_orgSigner
import wallet.core.jni.proto.Crypto_org

class TestCrypto_orgSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun Crypto_orgTransactionSigning() {
        // TODO: Finalize implementation

        //val transfer = Crypto_org.TransferMessage.newBuilder()
        //    .setTo("...")
        //    .setAmount(...)
        //    ...
        //    .build()
        //val signingInput = Crypto_org.SigningInput.newBuilder()
        //    ...
        //    .build()

        //val output: Crypto_org.SigningOutput = Crypto_orgSigner.sign(signingInput)

        //assertEquals(
        //    "__EXPECTED_RESULT_DATA__",
        //    Numeric.toHexString(output.encoded.toByteArray())
        //)
    }
}
