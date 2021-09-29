package com.trustwallet.core.app.blockchains.solana

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
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
        val delegateStakeMessage = Solana.DelegateStake.newBuilder().apply {
            validatorPubkey = commonValidatorPubkey
            value = 42
            stakeAccount = ""
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            delegateStakeTransaction = delegateStakeMessage
            recentBlockhash = blockhash
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck(commonPrivateKey))
        }.build()

        val output = AnySigner.sign(signingInput, SOLANA, SigningOutput.parser())

        val expectedString = "j24mVM9Zgu5vDZhPLGGuCRXQnP9djNtxdHh4txN3S7dwJsNNL5fbhzGpPgSUAcLGoMVCfF9TuqTYfpfJnb4sJFe1ahM8yPL5HwuKL6py5AZJFi8SWx9fvaVB699dCPo1GT3JoEBLPCZ9o2jQtnwzLkzTYJnKv2axqhKWFE2sz6TBA5J39eZcjMFUYgyxz6Q5S4MWqYQCb8UET2NAEZoKcfy7j8N25WXL6Gj4j3hBZjpHQQNaGaNEprEqyma3ZuVhpGiCALSsuzVLX3wZVo4icXwe952deMFA4tH3BK1jcSQCgfmcKDJ9nd7bdrnUUs4BoMdF1uDZB5LxE2UH8QiqtYvaUcorF4SJ3gPxM5ykbyPsNK1cSYZF9NMpW2GofyC17eELwnHQTQB2kqphxJZu7BahvkwiDPPeeydiXAkBspJ3nc3PCBujv6WJw22ZHw5j6zAP8ZGnCW44pqtWD5qifF9tTKhySKdANNiWifs3tSCCPQqjfJXu14drNinR6VG8rJxS1qgmRYiRQUa7m1vtoaZFRN5qKUeAfoFKkAVaNnMdwgsNqNH4dqBodTCJFs1LkYwhgRZdZGbwXTn1j7vpR3DSnv4g72i2H556srzK53jdUmdv6yfxt516XDSshqZtHnKZ1tudxKjBXwsqT3imDiZFVka9wKWUAYMCi4XZ79CY6Xpsd9c18U2e9TCngQmgkTATFgrqysfraokNffgqWxvsPMugksbvbPjJs3iCzByvphkC9p7hCf6LwbeF8XnVB91EAgRDA4VLE1f9wkcq5zjy879YWJ4r516h3PQszTz1EaJXNAXdbk5Em7eyuuabGP1Q3nijFTL2yhMDsXpgrjAuEAABNxFMd4J1JRMaic615mHrhwociksrsfQK"
        assertEquals(output.encoded, expectedString)
    }

    @Test
    fun testDeactivateStakeSign() {
        val deactivateStakeMessage = Solana.DeactivateStake.newBuilder().apply {
            stakeAccount = "6u9vJH9pRj66N5oJFCBADEbpMTrLxQATcL6q5p5MXwYv"
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
            stakeAccount = "6u9vJH9pRj66N5oJFCBADEbpMTrLxQATcL6q5p5MXwYv"
            value = 42
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            withdrawTransaction = withdrawStakeMessage
            recentBlockhash = blockhash
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck(commonPrivateKey))
        }.build()

        val output = AnySigner.sign(signingInput, SOLANA, SigningOutput.parser())

        val expectedString = "NL7WgagucfLd6AkTtcKe1dqd47xxzF356Q7tEhPrz1LRzZiAmokAaUkpwJ7X71Pmz97zZf9gZQU5BNswdcdpqUL8n1jwn4CoZMaPJhX5LF43Sj817cgreSG14TEWfKertpVpTtc5zY7vkDM7t9wjYhkaqgYz76HQtqAqRHnHF2Qr9EEfLj4zYRerWtyfS3EVyVUaasPxJ5vkcaonEfpGc6uWecaFr2A3YbzEBQpWXjMaXLqmMDtNS8rTNZmwvToa71ddFZKDgaHDcc6Lkg8qriZ3aQbUqL1TbeYp2mk9dWTKY62L1YFE2DyZV5P2qz5feywcMZ9JW6X1wBmiHFCseC42QbnbTibr1VdqLbGx7UWn5tHWk5jCN2aatEPfbFDZ"
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
            recipientMainAddress = "71e8mDsh3PR6gN64zL1HjwuxyKpgRXrPDUJT7XXojsVd"
            tokenMintAddress = "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt"
            recipientTokenAddress = "EF6L8yJT1SoRoDCkAZfSVmaweqMzfhxZiptKi7Tgj5XY"
            senderTokenAddress = "ANVCrmRw7Ww7rTFfMbrjApSPXEEcZpBa6YEiBdf98pAf"
            amount = 2900
            decimals = 6
        }.build()
        val signingInput = Solana.SigningInput.newBuilder().apply {
            createAndTransferTokenTransaction = createAndTransferTokenMessage
            recentBlockhash = "DMmDdJP41M9mw8Z4586VSvxqGCrqPy5uciF6HsKUVDja"
            privateKey = ByteString.copyFrom(Base58.decodeNoCheck("66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c"))
        }.build()

        val output = AnySigner.sign(signingInput, SOLANA, SigningOutput.parser())

        //https://explorer.solana.com/tx/449VaYo48LrkMJF6XVKt9sJwVQN6Seqrmh9erDCLtiuj6BgFG3wpF5TwjNkxgJ7qzNa6NTj3TFsU3h9hKszfkA7w
        val expectedString = "3Y2MVz2VVi7aEyC9q1awwdk1ModDBPHRSacKmTYnSgkmbbJeZ62Fub1bVPSHaTy4LUcQpzCQYhHAKtTKXUDYijEeLsMAUqPBEMAq1w8zCdqDpdXy6M4PuwNtYVV1WgqeiEsiMWpPp4BGWKfcziwFbmYueUGituacJq4wTnt92fho8mFi49XW64gEG4iNGScDtJkY7Geq8PKiLh1E9JMJoceiHxKbmxzCmmLTxEHdhySYHcDUSXnXWogZskeZNBMtR9dNjEMkCzEjrxRpBtJPtUNshciY45mDPNmw4j3xyLCBTRikyfFLc5g11r3UgyVD4YokoPRvrEXsgt6W3yjBshropBm6mY2eJYvfY2eZz4Yq8kLcUatCHVKtjcb1mP9Ww57KisJ9bRhipC8sodFaMYhZARMEa4a1u9eH4MyNUATRGNXarwQSBY46PWS3nKP6QBK7Dw7Ppp9MmYkdPcXKaLScbyLF3jKu6dHWMkHw3WdXSsM1wwXjXnWF9LxdwaEVcDmySWybj6aKD9QCWTU5kdncqJU56f7SYNRTN289WdUFGNDmSh56tj2v1"
        assertEquals(output.encoded, expectedString)
    }

    @Test
    fun testSignJSON() {
        val json = """
            {"recentBlockhash":"11111111111111111111111111111111","transferTransaction":{"recipient":"EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd","value":"42"}}
        """
        val key = "8778cc93c6596387e751d2dc693bbd93e434bd233bc5b68a826c56131821cb63".toHexByteArray()
        val result = AnySigner.signJSON(json, key, SOLANA.value())

        assertEquals("3p2kzZ1DvquqC6LApPuxpTg5CCDVPqJFokGSnGhnBHrta4uq7S2EyehV1XNUVXp51D69GxGzQZUjikfDzbWBG2aFtG3gHT1QfLzyFKHM4HQtMQMNXqay1NAeiiYZjNhx9UvMX4uAQZ4Q6rx6m2AYfQ7aoMUrejq298q1wBFdtS9XVB5QTiStnzC7zs97FUEK2T4XapjF1519EyFBViTfHpGpnf5bfizDzsW9kYUtRDW1UC2LgHr7npgq5W9TBmHf9hSmRgM9XXucjXLqubNWE7HUMhbKjuBqkirRM", result)
    }
}
