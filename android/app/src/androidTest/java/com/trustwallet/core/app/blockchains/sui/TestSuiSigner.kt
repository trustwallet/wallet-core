// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    fun SuiTransactionSigning() {
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
}
