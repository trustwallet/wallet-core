// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.acala

import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType
import wallet.core.jni.proto.Polkadot

class TestAcalaSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun AcalaTransactionSigning() {
        val transferCallIndices = Polkadot.CallIndices.newBuilder().apply {
            custom = Polkadot.CustomCallIndices.newBuilder().apply {
                moduleIndex = 0x0a
                methodIndex = 0x00
            }.build()
        }

        val call = Polkadot.Balance.Transfer.newBuilder().apply {
            value = "0xe8d4a51000".toHexBytesInByteString() // 1 ACA
            toAddress = "25Qqz3ARAvnZbahGZUzV3xpP1bB3eRrupEprK7f2FNbHbvsz"
            callIndices = transferCallIndices.build()
        }

        val acalaGenesisHashStr = "0xfc41b9bd8ef8fe53d58c7ea67c794c7ec9a73daf05e6d54b14ff6342c99ba64c".toHexBytesInByteString()

        val input = Polkadot.SigningInput.newBuilder().apply {
            genesisHash = acalaGenesisHashStr
            blockHash = "0x707ffa05b7dc6cdb6356bd8bd51ff20b2757c3214a76277516080a10f1bc7537".toHexBytesInByteString()
            nonce = 0
            specVersion = 2170
            network = CoinType.ACALA.ss58Prefix()
            transactionVersion = 2
            privateKey = "9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0".toHexBytesInByteString()
            era = Polkadot.Era.newBuilder().apply {
                blockNumber = 3893613
                period = 64
            }.build()
            balanceCall = Polkadot.Balance.newBuilder().apply {
                transfer = call.build()
            }.build()
            multiAddress = true
        }

        val output = AnySigner.sign(input.build(), CoinType.ACALA, Polkadot.SigningOutput.parser())
        val encoded = Numeric.toHexString(output.encoded.toByteArray())

        // https://acala.subscan.io/extrinsic/3893620-3
        val expected = "0x41028400e9590e4d99264a14a85e21e69537e4a64f66a875d38cb8f76b305f41fabe24a900dd54466dffd1e3c80b76013e9459fbdcd17805bd5fdbca0961a643bad1cbd2b7fe005c62c51c18b67f31eb9e61b187a911952fee172ef18402d07c703eec3100d50200000a0000c8c602ded977c56076ae38d98026fa669ca10d6a2b5a0bfc4086ae7668ed1c60070010a5d4e8"
        assertEquals(encoded, expected)
    }
}
