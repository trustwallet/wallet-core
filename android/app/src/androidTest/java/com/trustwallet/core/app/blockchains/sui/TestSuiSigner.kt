// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.sui

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType
import wallet.core.jni.proto.Sui

class TestSuiSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSuiDirectSigning() {
        // Successfully broadcasted https://explorer.sui.io/txblock/HkPo6rYPyDY53x1MBszvSZVZyixVN7CHvCJGX381czAh?network=devnet
        val txBytes = """
            AAACAAgQJwAAAAAAAAAgJZ/4B0q0Jcu0ifI24Y4I8D8aeFa998eih3vWT3OLUBUCAgABAQAAAQEDAAAAAAEBANV1rX8Y6UhGKlz2mPVk7zlKdSpx/sYkk6+KBVwBLA1QAQbywsjB2JZN8QGdZhbpcFcZvrq9kx2idVy5SM635olk7AIAAAAAAAAgYEVuxmf1zRBGdoDr+VDtMpIFF12s2Ua7I2ru1XyGF8/Vda1/GOlIRipc9pj1ZO85SnUqcf7GJJOvigVcASwNUAEAAAAAAAAA0AcAAAAAAAAA
        """.trimIndent()
        val key =
            "3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266".toHexBytesInByteString()
        val signDirect = Sui.SignDirect.newBuilder().setUnsignedTxMsg(txBytes).build()
        val signingInput =
            Sui.SigningInput.newBuilder().setSignDirectMessage(signDirect).setPrivateKey(key).build()
        val result = AnySigner.sign(signingInput, CoinType.SUI, Sui.SigningOutput.parser())
        val expectedSignature = "APxPduNVvHj2CcRcHOtiP2aBR9qP3vO2Cb0g12PI64QofDB6ks33oqe/i/iCTLcop2rBrkczwrayZuJOdi7gvwNqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg=="
        assertEquals(result.unsignedTx, txBytes);
        assertEquals(result.signature, expectedSignature)
    }

    @Test
    fun testSuiTransfer() {
        // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/D4Ay9TdBJjXkGmrZSstZakpEWskEQHaWURP6xWPRXbAm
        val txBytes = """
            AAAEAAjoAwAAAAAAAAAIUMMAAAAAAAAAIKcXWr3V7ZLr4605DbNmxqcGR4zfUXzebPmGMAZc2jd6ACBU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsgMCAAIBAAABAQABAQMAAAAAAQIAAQEDAAABAAEDAFToDXbXkMJ39aRPPOkvU9JvWJSJK/OV3uY3WYiHa+ayAWNgILOn3HsRw6pvQZsX+KnBLn95ox0b3S3mcLTt1jAFeHEaBQAAAAAgGGuNnxrqusosgjP3gQ3jBjnhapGNBlcU0yTaupXpa0BU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsu4CAAAAAAAAwMYtAAAAAAAA
        """.trimIndent()
        val key =
            "7e6682f7bf479ef0f627823cffd4e1a940a7af33e5fb39d9e0f631d2ecc5daff".toHexBytesInByteString()

        val paySui = Sui.PaySui.newBuilder()
            .addInputCoins(Sui.ObjectRef.newBuilder().apply {
                objectId = "0x636020b3a7dc7b11c3aa6f419b17f8a9c12e7f79a31d1bdd2de670b4edd63005"
                version = 85619064
                objectDigest = "2eKuWbZSVfpFVfg8FXY9wP6W5AFXnTchSoUdp7obyYZ5"
            })
            .addRecipients("0xa7175abdd5ed92ebe3ad390db366c6a706478cdf517cde6cf98630065cda377a")
            .addRecipients("0x54e80d76d790c277f5a44f3ce92f53d26f5894892bf395dee6375988876be6b2")
            .addAmounts(1000)
            .addAmounts(50000)

        val signingInput = Sui.SigningInput.newBuilder()
            .setPaySui(paySui)
            .setPrivateKey(key)
            .setGasBudget(3000000)
            .setReferenceGasPrice(750)
            .build()

        val result = AnySigner.sign(signingInput, CoinType.SUI, Sui.SigningOutput.parser())
        val expectedSignature = "AEh44B7iGArEHF1wOLAQJMLNgGnaIwn3gKPC92vtDJqITDETAM5z9plaxio1xomt6/cZReQ5FZaQsMC6l7E0BwmF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g=="
        assertEquals(result.unsignedTx, txBytes);
        assertEquals(result.signature, expectedSignature)
    }
}
