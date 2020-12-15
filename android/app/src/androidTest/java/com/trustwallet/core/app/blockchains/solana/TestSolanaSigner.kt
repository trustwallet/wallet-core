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

        val expectedString = "W1EAswaWK7mF4r9eZ2hHBZnfPnqLuNPiYkEMzFbwQsgSQu6XbSTL9AN92iyMbAMxPoRpt9ipUyztrmszAnm688N3k7uhiKn2osm9nxi6YkGLfu31jHTSu7mn3RtmenV3qopfPDAM7jtGoYQFb7eFVbujUb6tbeQ9UqLJq1sJ7uMZ4wqecmQPouDmJnpmJk4CHMzLnPNTwyGmGio6sYAS3xKZ7DFXvjwGPuD8PyYHSfdPro1p3jy9igPZNAbQ6fgK7LL3sERKCUdvPy7k14xgHbtsVy2mu54LY5c8F9sFst2uzQiTsXRTdjPFAyCVwB5pccNVotCrJ6Q2aKSC2D2knVH7LgWzSBMSreJG75xyATneu922wSzz7QJDieqhDtdePtSbPtoCdtPNmDfdaeDbHxVAxMios9F7RSRmH2dq86NfWDvF8TuEbYY7gPnygz6jGvwfqSSoSnY8TnUhhceC7wJSMc8Hcf1kyfi8dqKm7rF57YjnrQoMmL5bWqJLKoJtdfFu24ceQN21k38U2tUMWJaBASWukgTJUbNSCemNPZt4P3cNbeB3L1wBj4GEYXVTbTFYKME5JscU5RsnkMJZZ1PgzU285SkncqNSgxkpZVhmenTXpuZv74rXzariX8P4sprRgKUoj4b7Nu72Pya1zr7k45isMwgxtLnnnTK5k7mrZRDw3jBSBuukJBja93zaidm8HCQdwQsBt5CNSgSXug1R2t6Sdm5tjJrsd1gyRv7udFbHCdbVEeatzULNSSGdwjwwJDy1DTC12ddBNHd8k5ic5TDwrWdfCxbDRoFYw849YNNUuyNAPz1jDCkLG9af6KFFLxfuR9pnF8jSyTcQAq95YiiD9sC3mAUoe8AkYfy929XzTEatP1vasMvo"
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

    @Test
    fun testCreateAndTransferTokenSign() {
        val createAndTransferTokenMessage = Solana.CreateAndTransferToken.newBuilder().apply {
            recipientMainAddress = "3xJ3MoUVFPNFEHfWdtNFa8ajXUHsJPzXcBSWMKLd76ft"
            tokenMintAddress = "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt"
            recipientTokenAddress = "67BrwFYt7qUnbAcYBVx7sQ4jeD2KWN1ohP6bMikmmQV3"
            senderTokenAddress = "ANVCrmRw7Ww7rTFfMbrjApSPXEEcZpBa6YEiBdf98pAf"
            amount = 6100  // 0.0061
            decimals = 6
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            createAndTransferTokenTransaction = createAndTransferTokenMessage
            recentBlockhash = "11111111111111111111111111111111"
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5"))
        }.build()

        val output = AnySigner.sign(signingInput, SOLANA, SigningOutput.parser())

        val expectedString = "3tfkCabr1GxxXTBoGaLryVW1Bzfm4LUTrVpqNdKCCJd2v61UUpNtim3iPzp4JtMB6ir6LLvmgUDbjQAseX9sZTPVz7vqbQaBNBBh5ogDTppwccNrMSwTDaN32Lyf5aA8R1gABCpc1L4jeCBRvsg9MT4jmQnS8FzWo7GXqc1V9jNCEYgcz68JmSGkaXQVvjpRfkY4ZnGtqNWEK8Ntj45F9Zrq8dQ8jiNUPjQYiN1JujnmecekGU3GZNdqEBv5785UQBd5DEWFDxY2hQvCZAi8YF1KFYwPL1xUw2Vcanc6bRjfhPVAcuH5oty16dY9KpTgBowmHuGCbBazbR9Ym2kAR3EPvFuZuEi32Z4rzK98SDYpUBBKEHg7HTSh4NfZ34W6xPzLcw9DRZyAJHVCWiw6TY2xYVfMp3zTyPxnX4Cgdv73P7mAvZdJWBCvPbfmZ3amD8KPTogDQxhawb8bzihTSGbMCdufd6EA16i6pMhX8Qofx9f9RNmhFsrfJH98SHfNPR9oiuTJGarCuYf5kiVzMEhf3iMDZD9WX3VnpjnWG6rkiZKfZTwh7Qb8q"
        assertEquals(output.encoded, expectedString)
    }
}
