package com.trustwallet.core.app.blockchains.solana

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHex
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.Base58
import wallet.core.java.AnySigner
import wallet.core.jni.CoinType.SOLANA
import wallet.core.jni.proto.Solana
import wallet.core.jni.proto.Solana.SigningOutput

class TestSolanaSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    private val blockhash = "11111111111111111111111111111111"
    private val commonValidatorPubkey = "4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC"
    private val commonPrivateKey = "AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"

    @Test
    fun testTransferSign() {
        val transferMessage = Solana.Transfer.newBuilder().apply {
            recipient = "EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd"
            value = 42
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            transferTransaction = transferMessage
            recentBlockhash = blockhash
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr"))
        }.build()

        val output = AnySigner.sign(signingInput, SOLANA, SigningOutput.parser())

        val expectedHexString = "0x0103d668e8bb74c7664295e2f07c9040ac22a887edae0d628064311ad65e4f9d24b27e30027c87e03b43ae492e1347ea831f342d3a782f034df5ae38d12ae6a00f0100010366c2f508c9c555cacc9fb26d88e88dd54e210bb5a8bce5687f60d7e75c4cd07fc68b3c894c782b05a9c27fc6c66eb14d4e7d31de9086ab7d2129bcb0493afa020000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001020200010c020000002a00000000000000"
        assertEquals(output.encoded.toByteArray().toHex(), expectedHexString)
    }

    @Test
    fun testDelegateStakeSign() {
        val delegateStakeMessage = Solana.Stake.newBuilder().apply {
            validatorPubkey = commonValidatorPubkey
            value = 42
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            stakeTransaction = delegateStakeMessage
            recentBlockhash = blockhash
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck(commonPrivateKey))
        }.build()

        val output = AnySigner.sign(signingInput, SOLANA, SigningOutput.parser())

        val expectedHexString = "0x01a9ec9c553c565d232ac98892a1d926ccedb50b85ea227f3d2acef5a5e203573de157d843e578f0c01f2d9f83dd70e6adcc396ee474626e3168bb6b93176be602010006080eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab857a6b8aba83da905fff9c3e62acc748cf497bd503f11b7f9c337b7f35346df0306a7d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a00000000378ba8d9f9881e9be69cf1d70ee0a93ed0378b83203f42fa29f9df5c887f1c0d06a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b210000000006a1d817a502050b680791e6ce6db88e1e5b7150f61fc6790a4eb4d100000000000000000000000000000000000000000000000000000000000000000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc000000000000000000000000000000000000000000000000000000000000000000000000003060200017c030000000eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab82000000000000000346a707754717431715a6f52377536753633397a32416e675946474e336e616b2a00000000000000d80700000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc0000000000702010274000000000eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab80eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab8000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000070501030405000402000000"
        assertEquals(output.encoded.toByteArray().toHex(), expectedHexString)
    }

    @Test
    fun testDeactivateStakeSign() {
        val deactivateStakeMessage = Solana.DeactivateStake.newBuilder().apply {
            validatorPubkey = commonValidatorPubkey
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            deactivateStakeTransaction = deactivateStakeMessage
            recentBlockhash = blockhash
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck(commonPrivateKey))
        }.build()

        val output = AnySigner.sign(signingInput, SOLANA, SigningOutput.parser())

        val expectedHexString = "0x01f8edf7c745a5ac56682800b79ca94a2ca890e642f391b90e4f130119254d3b5e61bdec185b3650c5ec4e9b63dd62b553809d9a960b947e7c0c4ad1b23a2cba07010002040eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab857a6b8aba83da905fff9c3e62acc748cf497bd503f11b7f9c337b7f35346df0306a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b210000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc00000000000000000000000000000000000000000000000000000000000000000000000000103030102000405000000"
        assertEquals(output.encoded.toByteArray().toHex(), expectedHexString)
    }

    @Test
    fun testWithdrawStakeSign() {
        val withdrawStakeMessage = Solana.WithdrawStake.newBuilder().apply {
            validatorPubkey = commonValidatorPubkey
            value = 42
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            withdrawTransaction = withdrawStakeMessage
            recentBlockhash = blockhash
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck(commonPrivateKey))
        }.build()

        val output = AnySigner.sign(signingInput, SOLANA, SigningOutput.parser())

        val expectedHexString = "0x015f5eb11f1e3416c38d30bc9d966948ff58dc56849bf4b58e2538e2ab3bd7e951aeca5a317518dee81da76025471435257cde34a5d8ed8745e4abd7c7d3d9af0d010003050eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab857a6b8aba83da905fff9c3e62acc748cf497bd503f11b7f9c337b7f35346df0306a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b210000000006a7d517193584d0feed9bb3431d13206be544281b57b8566cc5375ff400000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc0000000000000000000000000000000000000000000000000000000000000000000000000010404010002030c040000002a00000000000000"
        assertEquals(output.encoded.toByteArray().toHex(), expectedHexString)
    }
}
