package com.trustwallet.core.app.blockchains.samplecoin

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import junit.framework.Assert.assertEquals
import org.junit.Test
//import wallet.core.jni.SampleCoinSigner
//import wallet.core.jni.proto.SampleCoin

class TestSampleCoinSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun SampleCoinTransactionSigning() {
        // TODO: Finalize implementation

        //val transaction = SampleCoin.TransactionPay.newBuilder()
        //    .setToAddress("...")
        //    .setAmount(...)
        //    ...
        //    .build()
        //val signingInput = SampleCoin.SigningInput.newBuilder()
        //    ...
        //    .build()

        //val output: SampleCoin.SigningOutput = SampleCoinSigner.sign(signingInput)

        //assertEquals(
        //    "__EXPECTED_RESULT_DATA__",
        //    Numeric.toHexString(output.encoded.toByteArray())
        //)
    }
}
