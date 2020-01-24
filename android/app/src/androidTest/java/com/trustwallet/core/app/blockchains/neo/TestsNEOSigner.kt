// Copyright ? 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.neo

import com.trustwallet.core.app.utils.Numeric
import org.junit.Test
import wallet.core.jni.NEOSigner
import wallet.core.jni.proto.NEO
import com.trustwallet.core.app.utils.toHexBytesInByteString
import junit.framework.Assert.assertEquals

class TestNEOSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun NEOTransactionSigning() {
        val signerBuilder = NEO.SigningInput.newBuilder().apply {
            privateKey = "F18B2F726000E86B4950EBEA7BFF151F69635951BC4A31C44F28EE6AF7AEC128".toHexBytesInByteString()
        }

        val input = NEO.TransactionInput.newBuilder()
            .setPrevHash("de24ccbca45777a86c2ca5b272d1f1a293dae811a7f86bfd2b7e67d59cb3859c".toHexBytesInByteString())
            .setPrevIndex(1)
            .build()

        signerBuilder.addInputs(input)

        val output0 = NEO.TransactionOutput.newBuilder()
            .setAssetId("9b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc5")
            .setValue(100000000)
            .setAddress("Ad9A1xPbuA5YBFr1XPznDwBwQzdckAjCev")
            .build()
        val output1 = NEO.TransactionOutput.newBuilder()
            .setAssetId("9b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc5")
            .setValue(89200000000)
            .setAddress("AdtSLMBqACP4jv8tRWwyweXGpyGG46eMXV")
            .build()

        signerBuilder.addOutputs(output0)
        signerBuilder.addOutputs(output1)

        val result = NEOSigner.sign(signerBuilder.build()).encoded.toByteArray()
        val hex = Numeric.toHexString(result, 0, result.size, false)

        assertEquals(
            "800000019c85b39cd5677e2bfd6bf8a711e8da93a2f1d172b2a52c6ca87757a4bccc24de0100029b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500e1f50500000000ea610aa6db39bd8c8556c9569d94b5e5a5d0ad199b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500fcbbc414000000f2908c7efc0c9e43ffa7e79170ba37e501e1b4ac0141405046619c8e20e1fdeec92ce95f3019f6e7cc057294eb16b2d5e55c105bf32eb27e1fc01c1858576228f1fef8c0945a8ad69688e52a4ed19f5b85f5eff7e961d7232102a41c2aea8568864b106553729d32b1317ec463aa23e7a3521455d95992e17a7aac",
            hex)
    }
}
