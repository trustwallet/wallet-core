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
import wallet.core.jni.CoinType.POLKADOT
import wallet.core.jni.proto.Polkadot
import wallet.core.jni.proto.Polkadot.SigningOutput

class TestPolkadotSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    val genesisHashStr = "0x91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3".toHexBytesInByteString()
    val privateKeyThrow2 = "0x70a794d4f1019c3ce002f33062f45029c4f930a56b3d20ec477f7668c6bbc37f".toHexBytesInByteString()

    @Test
    fun PolkadotTransactionSigning() {
        val key = "0x37932b086586a6675e66e562fe68bd3eeea4177d066619c602fe3efc290ada62".toHexBytesInByteString()

        val call = Polkadot.Staking.Bond.newBuilder().apply {
            controller = "14CeRumfZBNBVux9GgaiR5qw9E8RndNsiFWvhcHs76HEPjbP"
            value = "0x02540be400".toHexBytesInByteString()
            rewardDestination = Polkadot.RewardDestination.STAKED
        }

        val input = Polkadot.SigningInput.newBuilder().apply {
            genesisHash = genesisHashStr
            blockHash = genesisHashStr
            nonce = 0
            specVersion = 17
            network = Polkadot.Network.POLKADOT
            transactionVersion = 3
            privateKey = key
            stakingCall = Polkadot.Staking.newBuilder().apply {
                bond = call.build()
            }.build()
        }

        val output = AnySigner.sign(input.build(), POLKADOT, SigningOutput.parser())
        val encoded = Numeric.toHexString(output.encoded.toByteArray())

        val expected = "0x3902848d96660f14babe708b5e61853c9f5929bc90dd9874485bf4d6dc32d3e6f22eaa00a559867d1304cc95bac7cfe5d1b2fd49aed9f43c25c7d29b9b01c1238fa1f6ffef34b9650e42325de41e20fd502af7b074c67a9ec858bd9a1ba6d4212e3e0d0f00000007008d96660f14babe708b5e61853c9f5929bc90dd9874485bf4d6dc32d3e6f22eaa0700e40b540200"
        assertEquals(encoded, expected)
    }

    @Test
    fun PolkadotTransactionSignBondAndNominate() {
        val eraP = Polkadot.Era.newBuilder().apply {
            blockNumber = 3856651
            period = 64
        }
        val call = Polkadot.Staking.BondAndNominate.newBuilder().apply {
            controller = "14Ztd3KJDaB9xyJtRkREtSZDdhLSbm7UUKt8Z7AwSv7q85G2"
            value = "0x77359400".toHexBytesInByteString() // 0.2
            rewardDestination = Polkadot.RewardDestination.STASH
            addNominators("14xKzzU1ZYDnzFj7FgdtDAYSMJNARjDc2gNw4XAFDgr4uXgp")
            addNominators("1REAJ1k691g5Eqqg9gL7vvZCBG7FCCZ8zgQkZWd4va5ESih")
        }

        val input = Polkadot.SigningInput.newBuilder().apply {
            genesisHash = genesisHashStr
            blockHash = "0x3a886617f4bbd4fe2bbe7369acae4163ed0b19ffbf061083abc5e0836ad58f77".toHexBytesInByteString()
            nonce = 6
            specVersion = 27
            network = Polkadot.Network.POLKADOT
            transactionVersion = 5
            privateKey = privateKeyThrow2
            era = eraP.build()
            stakingCall = Polkadot.Staking.newBuilder().apply {
                bondAndNominate = call.build()
            }.build()
        }

        val output = AnySigner.sign(input.build(), POLKADOT, SigningOutput.parser())
        val encoded = Numeric.toHexString(output.encoded.toByteArray())

        // https://polkadot.subscan.io/extrinsic/0xc7a016f961dbf35d58feea22694e7d79ac77175a8cc40cb017bb5e87d56142ce
        val expected = "0x5103849dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f783007d549324f270eb5932b898ce5fc166c3f30942c96668f52d6cc86c7b61a8d65680cd0a979f1e0a43ef9418e6571edab6d9c391a1696abdf56db2af348862d50eb50018001a000807009dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f783030094357701070508aee72821ca00e62304e4f0d858122a65b87c8df4f0eae224ae064b951d39f610127a30e486492921e58f2564b36ab1ca21ff630672f0e76920edd601f8f2b89a"
        assertEquals(encoded, expected)
    }
}
