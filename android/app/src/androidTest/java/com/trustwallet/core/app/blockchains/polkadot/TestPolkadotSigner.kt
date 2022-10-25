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
    val iOSTestKey = "7f44b19b391a8015ca4c7d94097b3695867a448d1391e7f3243f06987bdb6858".toHexBytesInByteString()

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
            network = POLKADOT.ss58Prefix()
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
        val call = Polkadot.Staking.BondAndNominate.newBuilder().apply {
            controller = "13ZLCqJNPsRZYEbwjtZZFpWt9GyFzg5WahXCVWKpWdUJqrQ5"
            value = "0x02540be400".toHexBytesInByteString() // 1 DOT
            rewardDestination = Polkadot.RewardDestination.STASH
            addNominators("1zugcavYA9yCuYwiEYeMHNJm9gXznYjNfXQjZsZukF1Mpow")
            addNominators("15oKi7HoBQbwwdQc47k71q4sJJWnu5opn1pqoGx4NAEYZSHs")
        }

        val input = Polkadot.SigningInput.newBuilder().apply {
            genesisHash = genesisHashStr
            blockHash = genesisHashStr
            nonce = 4
            specVersion = 30
            network = POLKADOT.ss58Prefix()
            transactionVersion = 7
            privateKey = iOSTestKey
            stakingCall = Polkadot.Staking.newBuilder().apply {
                bondAndNominate = call.build()
            }.build()
        }

        val output = AnySigner.sign(input.build(), POLKADOT, SigningOutput.parser())
        val encoded = Numeric.toHexString(output.encoded.toByteArray())

        // https://polkadot.subscan.io/extrinsic/4955314-2
        val expected = "0x6103840036092fac541e0e5feda19e537c679b487566d7101141c203ac8322c27e5f076a00a8b1f859d788f11a958e98b731358f89cf3fdd41a667ea992522e8d4f46915f4c03a1896f2ac54bdc5f16e2ce8a2a3bf233d02aad8192332afd2113ed6688e0d0010001a02080700007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0700e40b540201070508002c2a55b5ffdca266bd0207df97565b03255f70783ca1a349be5ed9f44589c36000d44533a4d21fd9d6f5d57c8cd05c61a6f23f9131cec8ae386b6b437db399ec3d"
        assertEquals(encoded, expected)
    }

    @Test
    fun PolkadotTransactionSignChillAndUnbond() {
        val call = Polkadot.Staking.ChillAndUnbond.newBuilder().apply {
            value = "0x1766444D00".toHexBytesInByteString() // 10.05 DOT
        }

        val input = Polkadot.SigningInput.newBuilder().apply {
            genesisHash = genesisHashStr
            blockHash = "0x35ba668bb19453e8da6334cadcef2a27c8d4141bfc8b49e78e853c3d73e1ecd0".toHexBytesInByteString()
            nonce = 6
            specVersion = 9200
            network = POLKADOT.ss58Prefix()
            transactionVersion = 12
            privateKey = "298fcced2b497ed48367261d8340f647b3fca2d9415d57c2e3c5ef90482a2266".toHexBytesInByteString()
            era = Polkadot.Era.newBuilder().apply {
                blockNumber = 10541373
                period = 64
            }.build()
            stakingCall = Polkadot.Staking.newBuilder().apply {
                chillAndUnbond = call.build()
            }.build()
        }

        val output = AnySigner.sign(input.build(), POLKADOT, SigningOutput.parser())
        val encoded = Numeric.toHexString(output.encoded.toByteArray())

        // https://polkadot.subscan.io/extrinsic/10541383-2
        val expected = "0xd10184008361bd08ddca5fda28b5e2aa84dc2621de566e23e089e555a42194c3eaf2da7900c891ba102db672e378945d74cf7f399226a76b43cab502436971599255451597fc2599902e4b62c7ce85ecc3f653c693fef3232be620984b5bb5bcecbbd7b209d50318001a02080706070207004d446617"
        assertEquals(encoded, expected)
    }
}
