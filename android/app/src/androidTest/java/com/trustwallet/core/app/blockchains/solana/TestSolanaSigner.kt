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

        val expectedString = "5Wwh6XZkH7KuykAbAk9tGthyRXEaTYYWA2p2y46b4TBCh1X2Pea5cb9KTA1FnvDvkMeTZmeKkCGNbH6xFkNKfKymuCrKRKiYbLTEmjt2bq5ff6Ex8G2ZJc9AYQ87nhozqsJxUWAy1oayp62xvtJc51YbPZYact54JUyEAMNEH6VN9hyDXtLHCvHwUTeE46ZzU2K6Qsj9eUxaUtETacGjJdkLFHXioMyTq1npjkUf7CCTQ65eeByVfFYzedJGoy2sh9cRSuegwkgz6SHtHtYpo4f8BcpePEPg2McJw9rpfSdu8jGmRZkfW95FJr3umBHRL5YrZEgHNG13D1wsQdprV4wY3DuoTsnX6MFfpoqteBEb7fvCw4PVXvNdUzUqwwR5wd9dyWp6r9jagTVTvJWNbWHo2N7ikpJhdRubfUiHA5M9PBFJxHvGszviSJRPuArMi8bAWom4xFUfvigp5Mhz6swCgVU2Fo68kD6RxmFJ2Pdqa1yr3GWD1caYRu1peNUbkE5a7c769s7pdeatDGADR6Pe8BUj1gZaFkckwRGtrmRrvfoJpGcGvpLtXycsUvzD5QxEZLJmWnEYtLhnoKqycdFStS3va5xBmiKzrHsqWca4RBBB2Hu58RZyUk88erRuGC8KY64gJZRK5kB2oSDeqimBtfrGhpbddX3hqJKgLQ3htSUxsM9BV4hmdFNgzh2dUb2msrAeGUsJ496MmjLgufPtkv8SQrNJbDYLjCmzJN1h1nDwoc5UDnL2e6TMgRWHKAR8BCSGM1V32QnyW5WKdJMAnhLQx7qoQ22AocvPbrfTdbM4kWAaQy1ujyvLJuYkaRx5tTteMbsmWF"
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

    @Test
    fun testCreateTokenAccountSign() {
        val createAccountMessage = Solana.CreateTokenAccount.newBuilder().apply {
            mainAddress = "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V"
            tokenMintAddress = "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt"
            tokenAddress = "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP"
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            createTokenAccountTransaction = createAccountMessage
            recentBlockhash = "9ipJh5xfyoyDaiq8trtrdqQeAhQbQkWy2eANizKvx75K"
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5"))
        }.build()

        val output = AnySigner.sign(signingInput, SOLANA, SigningOutput.parser())

        val expectedString = "CKzRLx3AQeVeLQ7T4hss2rdbUpuAHdbwXDazxtRnSKBuncCk3WnYgy7XTrEiya19MJviYHYdTxi9gmWJY8qnR2vHVnH2DbPiKA8g72rD3VvMnjosGUBBvCwbBLge6FeQdgczMyRo9n5PcHvg9yJBTJaEEvuewyBVHwCGyGQci7eYd26xtZtCjAjwcTq4gGr3NZbeRW6jZp6j6APuew7jys4MKYRV4xPodua1TZFCkyWZr1XKzmPh7KTavtN5VzPDA8rbsvoEjHnKzjB2Bszs6pDjcBFSHyQqGsHoF8XPD35BLfjDghNtBmf9cFqo5axa6oSjANAuYg6cMSP4Hy28waSj8isr6gQjE315hWi3W1swwwPcn322gYZx6aMAcmjczaxX9aktpHYgZxixF7cYWEHxJs5QUK9mJePu9Xc6yW75UB4Ynx6dUgaSTEUzoQthF2TN3xXwu1"
        assertEquals(output.encoded, expectedString)
    }

    @Test
    fun testTokenTransferSign() {
        val tokenTransferMessage = Solana.TokenTransfer.newBuilder().apply {
            tokenMintAddress = "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt"
            senderTokenAddress = "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP"
            recipientTokenAddress = "3WUX9wASxyScbA7brDipioKfXS1XEYkQ4vo3Kej9bKei"
            amount = 4000  // 0.004
            decimals = 6
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            tokenTransferTransaction = tokenTransferMessage
            recentBlockhash = "CNaHfvqePgGYMvtYi9RuUdVxDYttr1zs4TWrTXYabxZi"
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5"))
        }.build()

        val output = AnySigner.sign(signingInput, SOLANA, SigningOutput.parser())

        val expectedString = "PGfKqEaH2zZXDMZLcU6LUKdBSzU1GJWJ1CJXtRYCxaCH7k8uok38WSadZfrZw3TGejiau7nSpan2GvbK26hQim24jRe2AupmcYJFrgsdaCt1Aqs5kpGjPqzgj9krgxTZwwob3xgC1NdHK5BcNwhxwRtrCphGEH7zUFpGFrFrHzgpf2KY8FvPiPELQyxzTBuyNtjLjMMreehSKShEjD9Xzp1QeC1pEF8JL6vUKzxMXuveoEYem8q8JiWszYzmTMfDk13JPgv7pXFGMqDV3yNGCLsWccBeSFKN4UKECre6x2QbUEiKGkHkMc4zQwwyD8tGmEMBAGm339qdANssEMNpDeJp2LxLDStSoWShHnotcrH7pUa94xCVvCPPaomF"
        assertEquals(output.encoded, expectedString)
    }
}
