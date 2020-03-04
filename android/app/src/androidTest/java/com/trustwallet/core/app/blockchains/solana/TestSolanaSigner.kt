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

        val expectedString = "3p2kzZ1DvquqC6LApPuxpTg5CCDVPqJFokGSnGhnBHrta4uq7S2EyehV1XNUVXp51D69GxGzQZUjikfDzbWBG2aFtG3gHT1QfLzyFKHM4HQtMQMNXqay1NAeiiYZjNhx9UvMX4uAQZ4Q6rx6m2AYfQ7aoMUrejq298q1wBFdtS9XVB5QTiStnzC7zs97FUEK2T4XapjF1519EyFBViTfHpGpnf5bfizDzsW9kYUtRDW1UC2LgHr7npgq5W9TBmHf9hSmRgM9XXucjXLqubNWE7HUMhbKjuBqkirRM"
        assertEquals(output.encoded, expectedString)
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

        val expectedString = "7gcsGMV8fPkuSDegrBk5KF4eZtmGqdixEr5Kxx7m74nErH5EWxkKcvy5LKg3kqKHLMtzqGXrM8EdVskLu5k6cQUVmznfd6iGpuDkn4BWi8HaqSoaeL7DCx6QHLUwcyvorfqPkgSaYopY9MeB6MGKS4bKCQi1paP7eEKZj9JgAZTHnpzAuFzqE5dC98MrbThoBmHqfybw1jHssp4NZjrxsuMz4oYcHe837WmnpinE68QEVf9FzuoXKrMDwYZQYogJHUZgxghvMmJu3XVZk7hL1h7SgE9ow2SvvNfAUYfUTuz8N9m6JcXgwmNL51ZweW9F1fekmSKE3vLEAaVvvYFsDxn1gt4bXuTdAMuUDKdH33YMLQr8eEsdUDZUm33KtuE7Ddy84NEg3KbpWAy8T4vkwiLyvoyjmMdzCkMuobyXWQV1rfRk5vTJ4x6dMgznfAeKEqmd6xZ2hN8JBmTenP5dsvZCCSCf4G7cUkdndKu552KALqiNHd5msgLJvJKSHmZjFhS43fDCvkG7njF8yaZzWjmAknWUBbV6YaGmD3XmcWfJgvB1zivZJhiMbzopP8Nm5wL5iDbCrSZTGq2tzEsTvje75wv2RtuAcgiicEPTuPAin9fKyMbCpf67pGgWPwH5DwYumMwd8zwoJyuakyqFsFLnBKTvp8pFCijdj7fEhyC31xuMV7crwyrN5X3y7QKCE7PZcBP637YHEPtTvt1ECp4CqBSnvPc8vRD8EMhHe5jRFSDkQriUenEPFc51dTDTJWL26xuiTivktEm6ahHq5d6MPr4NRDvcRG2cZvEgxHBLpKfuB5XL3JfQZ3Nn3B916gaK8owz9Rk2e3"
        assertEquals(output.encoded, expectedString)
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

        val expectedString = "AhfB77PTGTKBfbGPGuEz2khbBy8m8Kou1zqZST9dP7PLJNSeEze5NJuCh5qecPLa3S8xAQ6mTULmnAWiW81ib87nhywFtx5nKiUvmhdXsvKCSX6NNtNXdRz5yZi3UEop4obco85SY2czS6n4SJwmtDedHLtg9urqdZVth7AUM8KAtrRsksyvZRYXh64Z8QGyNY7ekj31ae11avGiSDNWYZZHqx7VPWRsKeatGyGk5zPmnRdL8ABMQgJ1Te7wAWwVnNn5QcoAxDuPw6uDctP8Q5S4TieRVatCnukQFj5BTJisez3E2ZJPWhVrMh4K3wEFkPHA7dR"
        assertEquals(output.encoded, expectedString)
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

        val expectedString = "7Y1Wg1yHNs8MgWFiFSfcsRtqdMwZg8oGeQnTABYDfyDnof4VSFw63s3PuSxvUCJqqHKgYNVb8UTNcNiYHY8kng4NqTcVV5SA1KAWRzKHVGUxNWioAEXXVot5iJ1XbUWuuZUZBtsraaBjNyfmgWEDje3ESdGhiVL7vadU1uHeBuUKwM3nqB6yoeggeNyzmT34hs9utyehTFg48MAfrKEFKxaby7YZD6JbXFS1SyG1kxKWnCpoPgX3efwDwukmyDwxrKdABt9eTwmaiXKbTnK1hzBTatNfnJ9ePuWkhWFrjyDrGdx5S5KpybxET2vV9CSpExcD51BA6NPemTpjbhLYnJEzHWBGfYqfxu7p3257NHhpQQrSU56adk4dAQFjEYP"
        assertEquals(output.encoded, expectedString)
    }
}
