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
    private val commonValidatorPubkey = "4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC"
    private val commonPrivateKey = "AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"

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
        val delegateStakeMessage = Solana.Stake.newBuilder().apply {
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck(commonPrivateKey))
            validatorPubkey = commonValidatorPubkey
            value = 42
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            stakeTransaction = delegateStakeMessage
            recentBlockhash = blockhash
        }.build()

        val output: Solana.SigningOutput = SolanaSigner.sign(signingInput)

        val expectedHexString = "0x018efed47208bf63df029951cea09600a6fa697c1a8b715a24347e4b43750694af40d870dbaad894d7b5a8d45ccd9eeaa366a1c3d5ccca93d5c707f8221f9d5904010005080eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab8e26f634c7f6a8f05eea8b1e74a93d6aaee9a07c907e9a825931fe678b26c5f3206a7d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a00000000378ba8d9f9881e9be69cf1d70ee0a93ed0378b83203f42fa29f9df5c887f1c0d06a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b210000000006a1d817a502050b680791e6ce6db88e1e5b7150f61fc6790a4eb4d100000000000000000000000000000000000000000000000000000000000000000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc0000000000000000000000000000000000000000000000000000000000000000000000000030602000134000000002a00000000000000d80600000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc000000000070201026c000000000eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab80eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab800000000000000000000000000000000000000000000000000000000000000000000000000000000070501030405000402000000"
        assertEquals(output.encoded.toByteArray().toHex(), expectedHexString)
    }

    @Test
    fun testDeactivateStakeSign() {
        val deactivateStakeMessage = Solana.DeactivateStake.newBuilder().apply {
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck(commonPrivateKey))
            validatorPubkey = commonValidatorPubkey
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            deactivateStakeTransaction = deactivateStakeMessage
            recentBlockhash = blockhash
        }.build()

        val output: Solana.SigningOutput = SolanaSigner.sign(signingInput)

        val expectedHexString = "0x010edc9131bbfd79536d9e7fc403f5a0fcb36095c1fd2561147f1086173104d028dbe0ff228d11d2e5ea03752e2ef0c66d2a8d232387cf7bf311068e992566e40f010002040eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab8e26f634c7f6a8f05eea8b1e74a93d6aaee9a07c907e9a825931fe678b26c5f3206a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b210000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc00000000000000000000000000000000000000000000000000000000000000000000000000103030102000405000000"
        assertEquals(output.encoded.toByteArray().toHex(), expectedHexString)
    }

    @Test
    fun testWithdrawStakeSign() {
        val withdrawStakeMessage = Solana.WithdrawStake.newBuilder().apply {
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck(commonPrivateKey))
            validatorPubkey = commonValidatorPubkey
            value = 42
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            withdrawTransaction = withdrawStakeMessage
            recentBlockhash = blockhash
        }.build()

        val output: Solana.SigningOutput = SolanaSigner.sign(signingInput)

        val expectedHexString = "0x015095806889455cdef7ff520bf8e7ed4cf67e3d04559c806400995273f4d88a20d85203e9aa15012dd321022420dc6c9ae6a6776fb81496819b1fd9756b20bf0f010003050eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab8e26f634c7f6a8f05eea8b1e74a93d6aaee9a07c907e9a825931fe678b26c5f3206a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b210000000006a7d517193584d0feed9bb3431d13206be544281b57b8566cc5375ff400000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc0000000000000000000000000000000000000000000000000000000000000000000000000010404010002030c040000002a00000000000000"
        assertEquals(output.encoded.toByteArray().toHex(), expectedHexString)
    }
}
