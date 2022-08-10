// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.everscale

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.EVERSCALE
import wallet.core.jni.proto.Everscale
import wallet.core.jni.proto.Everscale.SigningOutput

class TestEverscaleSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testSign() {
        val transferMessage = Everscale.Transfer.newBuilder().apply {
            bounce = false
            flags = 3
            amount = 500000000
            expiredAt = 1660261731
        }.build()
        val signingInput = Everscale.SigningInput.newBuilder().apply {
            transfer = transferMessage
            privateKey = ByteString.copyFrom("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f".toHexByteArray())
        }.build()

        val output = AnySigner.sign(signingInput, EVERSCALE, SigningOutput.parser())

        val expectedString = "te6ccgICAAQAAQAAAUoAAAPhiABNP9xIXWgg8NV8Lu9CjwuRd9Y8aCAY6uHC7TFm1azfYBGTp6HO3zHcMkuRWEKShVeKcgHJt5kYQQy+Qn296nFFjD0XqbeVvMtTL3N7ud7Ad8aFTFWSgEKVN2n4NzfUBTugCXUlsUxesqxgAAAAAHAAAwACAAEAaEIAE0/3EhdaCDw1Xwu70KPC5F31jxoIBjq4cLtMWbVrN9gg7msoAAAAAAAAAAAAAAAAAAAAUAAAAABLqS2K5JJfmTLfjX/QBC7/8+IXipcgKLZE3tOjtm9tBXf4LngA3v8AIN0gggFMl7ohggEznLqxn3Gw7UTQ0x/THzHXC//jBOCk8mCDCNcYINMf0x/TH/gjE7vyY+1E0NMf0x/T/9FRMrryoVFEuvKiBPkBVBBV+RDyo/gAkyDXSpbTB9QC+wDo0QGkyMsfyx/L/8ntVA=="
        assertEquals(output.encoded, expectedString)
    }
}
