package com.trustwallet.core.app.blockchains.algorand

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import junit.framework.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.AlgorandSigner
import wallet.core.jni.proto.Algorand

class TestAlgorandSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun AlgorandTransactionSigning() {
        val transaction = Algorand.TransactionPay.newBuilder()
            .setToAddress("CRLADAHJZEW2GFY2UPEHENLOGCUOU74WYSTUXQLVLJUJFHEUZOHYZNWYR4")
            .setAmount(1000000000000)
            .setFee(263000)
            .setFirstRound(1937767)
            .setLastRound(1938767)
            .build()
        val signingInput = Algorand.SigningInput.newBuilder()
            .setGenesisId("mainnet-v1.0")
            .setGenesisHash(ByteString.copyFrom("c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adf".toHexByteArray()))
            .setNote(ByteString.copyFrom("68656c6c6f".toHexByteArray()))
            .setPrivateKey("d5b43d706ef0cb641081d45a2ec213b5d8281f439f2425d1af54e2afdaabf55b".toHexBytesInByteString())
            .setTransactionPay(transaction)
            .build()

        val output: Algorand.SigningOutput = AlgorandSigner.sign(signingInput)

        assertEquals(
            Numeric.toHexString(output.encoded.toByteArray()),
            "0x82a3736967c440baa00062adcdcb5875e4435cdc6885d26bfe5308ab17983c0fda790b7103051fcb111554e5badfc0ac7edf7e1223a434342a9eeed5cdb047690827325051560ba374786e8aa3616d74cf000000e8d4a51000a3666565ce00040358a26676ce001d9167a367656eac6d61696e6e65742d76312e30a26768c420c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adfa26c76ce001d954fa46e6f7465c40568656c6c6fa3726376c42014560180e9c92da3171aa3c872356e30a8ea7f96c4a74bc1755a68929c94cb8fa3736e64c42061bf060efc02e2887dfffc8ed85268c8c091c013eedf315bc50794d02a8791ada474797065a3706179"
        )
    }
}
