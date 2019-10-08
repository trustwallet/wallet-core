package com.trustwallet.core.app.blockchains.solana

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Assert.assertArrayEquals
import org.junit.Test
import wallet.core.jni.Base58
import wallet.core.jni.PrivateKey
import wallet.core.jni.SolanaSigner
import wallet.core.jni.proto.Solana

class TestSolanaSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    private val blockhash = "11111111111111111111111111111111"

    @Test
    fun testTransferSign() {
        val transferMessage = Solana.Transfer.newBuilder().apply {
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr"))
            recipient = "EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd"
            value = 42
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            transferTransaction = transferMessage
            recentBlockhash = blockhash
        }.build()

        val output: Solana.SigningOutput = SolanaSigner.sign(signingInput)

        val expectedHexString = "0x01fda1c8ad8872d94f7eab52f9c38dc77e1061f4897e3de2b8469eb0992269f6fa1f173e93dbb2da738ab4e8959ffa50cd087cdfa889f3a1b8acdd62552f7c1d070100020366c2f508c9c555cacc9fb26d88e88dd54e210bb5a8bce5687f60d7e75c4cd07fc68b3c894c782b05a9c27fc6c66eb14d4e7d31de9086ab7d2129bcb0493afa020000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001020200010c020000002a00000000000000"
        assertEquals(output.encoded.toByteArray().toHex(), expectedHexString)
    }

    @Test
    fun testDelegateStakeSign() {
        val setStakePubkey = "Bqa7hbY1McviVybz8pyBZEDcJRuy6ZYen3XjAh6VLcsk"
        val delegateStakeMessage = Solana.Stake.newBuilder().apply {
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"))
            votePubkey = "4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC"
            value = 42
            stakePubkey = setStakePubkey
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            stakeTransaction = delegateStakeMessage
            recentBlockhash = blockhash
        }.build()

        val output: Solana.SigningOutput = SolanaSigner.sign(signingInput)

        val expectedHexString = "0x0146236d3c6553ad8120c3ac0e97a1bb75ec2ceebe388982bf01a14e8b0d9c3453415120a0db0fa1a1125d88e9c12219d6660560338261e957dcadd9ac4afda40d010005080eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab8a106312279be880f9aef61c849c4ec109af4766f3fdcbe7db3ff48c06e9f90c306a7d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a00000000378ba8d9f9881e9be69cf1d70ee0a93ed0378b83203f42fa29f9df5c887f1c0d06a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b210000000006a1d817a502050b680791e6ce6db88e1e5b7150f61fc6790a4eb4d100000000000000000000000000000000000000000000000000000000000000000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc0000000000000000000000000000000000000000000000000000000000000000000000000030602000134000000002a00000000000000d80600000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc000000000070201026c000000000eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab80eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab800000000000000000000000000000000000000000000000000000000000000000000000000000000070501030405000402000000"
        assertEquals(output.encoded.toByteArray().toHex(), expectedHexString)
        assertEquals(output.stakePubkey, setStakePubkey)
    }

    @Test
    fun testDeactivateStakeSign() {
        val deactivateStakeMessage = Solana.DeactivateStake.newBuilder().apply {
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"))
            stakePubkey = "Bqa7hbY1McviVybz8pyBZEDcJRuy6ZYen3XjAh6VLcsk"
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            deactivateStakeTransaction = deactivateStakeMessage
            recentBlockhash = blockhash
        }.build()

        val output: Solana.SigningOutput = SolanaSigner.sign(signingInput)

        val expectedHexString = "0x017828896b4aa8cb1fb4378f4fb565bae8381fdab077ba9ea4f75f3eac5ed7251190dccde7c49ee1c5d4cd120a6495df12cd46e9e9dc202282e30530805132d102010102040eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab8a106312279be880f9aef61c849c4ec109af4766f3fdcbe7db3ff48c06e9f90c306a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b210000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc00000000000000000000000000000000000000000000000000000000000000000000000000103030102000405000000"
        assertEquals(output.encoded.toByteArray().toHex(), expectedHexString)
    }

    @Test
    fun testWithdrawStakeSign() {
        val withdrawStakeMessage = Solana.WithdrawStake.newBuilder().apply {
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"))
            stakePubkey = "Bqa7hbY1McviVybz8pyBZEDcJRuy6ZYen3XjAh6VLcsk"
            recipient = "C3e7ryQjYJFSUetohBofTaWEBbNcq4yVX43hi7igVtcP"
            value = 42
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            withdrawTransaction = withdrawStakeMessage
            recentBlockhash = blockhash
        }.build()

        val output: Solana.SigningOutput = SolanaSigner.sign(signingInput)

        val expectedHexString = "0x0108684dc006c04eff052c1f2a4791537c4207d3efd21ec64c4845dbee8d83e74c589269a02795034c4d766d732c6f38db86d4f41a314a570d44d47f506feafb06010104060eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab8a106312279be880f9aef61c849c4ec109af4766f3fdcbe7db3ff48c06e9f90c3a41db0070ae3aee30c6ce259f81a49cd15603fe29b47ba80ac734ff3911462a006a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b210000000006a7d517193584d0feed9bb3431d13206be544281b57b8566cc5375ff400000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc000000000000000000000000000000000000000000000000000000000000000000000000001050501020304000c040000002a00000000000000"
        assertEquals(output.encoded.toByteArray().toHex(), expectedHexString)
    }
}
