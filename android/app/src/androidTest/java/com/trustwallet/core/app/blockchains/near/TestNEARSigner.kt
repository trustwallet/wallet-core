package com.trustwallet.core.app.blockchains.solana

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import java.util.Base64
import org.junit.Assert.assertEquals
import org.junit.Assert.assertArrayEquals
import org.junit.Test
import wallet.core.jni.Base58
import wallet.core.jni.PrivateKey
import wallet.core.jni.NEARSigner
import wallet.core.jni.proto.NEAR

class TestNEARSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testTransferSign() {
        val transferAction = NEAR.Transfer.newBuilder().apply {
            deposit = ByteString.copyFrom("01000000000000000000000000000000".toHexByteArray())
        }.build()
        val signingInput = NEAR.SigningInput.newBuilder().apply {
            signerId = "test.near"
            nonce = 1
            receiverId = "whatever.near"
            addActionsBuilder().apply {
                transfer = transferAction
            }
            blockHash = ByteString.copyFrom(Base58.decodeNoCheck("244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM"))
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv").sliceArray(0..31))
        }.build()

        val output: NEAR.SigningOutput = NEARSigner.sign(signingInput)

        val expectedBase64String = "CQAAAHRlc3QubmVhcgCRez0mjUtY9/7BsVC9aNab4+5dTMOYVeNBU4Rlu3eGDQEAAAAAAAAADQAAAHdoYXRldmVyLm5lYXIPpHP9JpAd8pa+atxMxN800EDvokNSJLaYaRDmMML+9gEAAAADAQAAAAAAAAAAAAAAAAAAAACWmoMzIYbul1Xkg5MlUlgG4Ymj0tK7S0dg6URD6X4cTyLe7vAFmo6XExAO2m4ZFE2n6KDvflObIHCLodjQIb0B"
        assertEquals(Base64.getEncoder().encodeToString(output.signedTransaction.toByteArray()), expectedBase64String)
    }

}
