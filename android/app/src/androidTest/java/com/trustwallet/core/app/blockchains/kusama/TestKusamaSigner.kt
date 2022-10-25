// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.polkadot

import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.KUSAMA
import wallet.core.jni.proto.Polkadot
import wallet.core.jni.proto.Polkadot.SigningOutput

class TestKusamaSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun KusamaTransactionSigning() {
        val key = "0x8cdc538e96f460da9d639afc5c226f477ce98684d77fb31e88db74c1f1dd86b2".toHexBytesInByteString()
        val hash = "0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe".toHexBytesInByteString()

        val call = Polkadot.Balance.Transfer.newBuilder().apply {
            toAddress = "CtwdfrhECFs3FpvCGoiE4hwRC4UsSiM8WL899HjRdQbfYZY"
            value = "0x02540be400".toHexBytesInByteString()
        }

        val input = Polkadot.SigningInput.newBuilder().apply {
            genesisHash = hash
            blockHash = hash
            nonce = 1
            specVersion = 2019
            network = KUSAMA.ss58Prefix()
            transactionVersion = 2
            privateKey = key
            balanceCall = Polkadot.Balance.newBuilder().apply {
                transfer = call.build()
            }.build()
        }

        val output = AnySigner.sign(input.build(), KUSAMA, SigningOutput.parser())
        val encoded = Numeric.toHexString(output.encoded.toByteArray())

        val expected = "0x350284f41296779fd61a5bed6c2f506cc6c9ea93d6aeb357b9c69717193f434ba24ae700cd78b46eff36c433e642d7e9830805aab4f43eef70067ef32c8b2a294c510673a841c5f8a6e8900c03be40cfa475ae53e6f8aa61961563cb7cc0fa169ef9630d00040004000e33fdfb980e4499e5c3576e742a563b6a4fc0f6f598b1917fd7a6fe393ffc720700e40b5402"
        assertEquals(encoded, expected)
    }
}
