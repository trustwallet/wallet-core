// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class SolanaTests: XCTestCase {

    let privateKeyData = Data(Base58.decodeNoCheck( string: "A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr")!)

    func testAddressFromPrivateKey() {
        let privateKey = PrivateKey(data: privateKeyData)!
        let publicKey = privateKey.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: publicKey, coin: .solana)
        XCTAssertEqual(address.description, "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q")
    }

    func testAddressFromPublicKey() {
        let publicKey = PublicKey(data: Data(Base58.decodeNoCheck(string: "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q")!), type: PublicKeyType.ed25519)!
        let address = AnyAddress(publicKey: publicKey, coin: .solana)
        XCTAssertEqual(address.description, "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q")
    }

    func testAddressFromString() {
        let addressString = "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q"
        let address = AnyAddress(string: addressString, coin: .solana)
        XCTAssertEqual(address!.description, "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q")
    }

    func testTransferSigner() throws {
        let transferMessage = SolanaTransfer.with {
            $0.recipient = "EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd"
            $0.value = 42
        }
        let input = SolanaSigningInput.with {
            $0.transferTransaction = transferMessage
            $0.recentBlockhash = "11111111111111111111111111111111"
            $0.privateKey = privateKeyData
        }

        let output: SolanaSigningOutput = AnySigner.sign(input: input, coin: .solana)

        let expectedString = """
        3p2kzZ1DvquqC6LApPuxpTg5CCDVPqJFokGSnGhnBHrta4uq7S2EyehV1XNUVXp51D69GxGzQZU\
        jikfDzbWBG2aFtG3gHT1QfLzyFKHM4HQtMQMNXqay1NAeiiYZjNhx9UvMX4uAQZ4Q6rx6m2AYfQ\
        7aoMUrejq298q1wBFdtS9XVB5QTiStnzC7zs97FUEK2T4XapjF1519EyFBViTfHpGpnf5bfizDz\
        sW9kYUtRDW1UC2LgHr7npgq5W9TBmHf9hSmRgM9XXucjXLqubNWE7HUMhbKjuBqkirRM
        """

        XCTAssertEqual(output.encoded, expectedString)
    }

    func testDelegateStakeSigner() throws {
        let delegateStakeMessage = SolanaDelegateStake.with {
            $0.validatorPubkey = "4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC"
            $0.value = 42
            $0.stakeAccount = ""
        }
        let input = SolanaSigningInput.with {
            $0.delegateStakeTransaction = delegateStakeMessage
            $0.recentBlockhash = "11111111111111111111111111111111"
            $0.privateKey = Data(Base58.decodeNoCheck( string: "AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746")!)
        }

        let output: SolanaSigningOutput = AnySigner.sign(input: input, coin: .solana)

        let expectedString = """
        j24mVM9Zgu5vDZhPLGGuCRXQnP9djNtxdHh4txN3S7dwJsNNL5fbhzGpPgSUAcLGoMVCfF9TuqTYfpfJnb4sJFe1ah\
        M8yPL5HwuKL6py5AZJFi8SWx9fvaVB699dCPo1GT3JoEBLPCZ9o2jQtnwzLkzTYJnKv2axqhKWFE2sz6TBA5J39eZc\
        jMFUYgyxz6Q5S4MWqYQCb8UET2NAEZoKcfy7j8N25WXL6Gj4j3hBZjpHQQNaGaNEprEqyma3ZuVhpGiCALSsuzVLX3\
        wZVo4icXwe952deMFA4tH3BK1jcSQCgfmcKDJ9nd7bdrnUUs4BoMdF1uDZB5LxE2UH8QiqtYvaUcorF4SJ3gPxM5yk\
        byPsNK1cSYZF9NMpW2GofyC17eELwnHQTQB2kqphxJZu7BahvkwiDPPeeydiXAkBspJ3nc3PCBujv6WJw22ZHw5j6z\
        AP8ZGnCW44pqtWD5qifF9tTKhySKdANNiWifs3tSCCPQqjfJXu14drNinR6VG8rJxS1qgmRYiRQUa7m1vtoaZFRN5q\
        KUeAfoFKkAVaNnMdwgsNqNH4dqBodTCJFs1LkYwhgRZdZGbwXTn1j7vpR3DSnv4g72i2H556srzK53jdUmdv6yfxt5\
        16XDSshqZtHnKZ1tudxKjBXwsqT3imDiZFVka9wKWUAYMCi4XZ79CY6Xpsd9c18U2e9TCngQmgkTATFgrqysfraokN\
        ffgqWxvsPMugksbvbPjJs3iCzByvphkC9p7hCf6LwbeF8XnVB91EAgRDA4VLE1f9wkcq5zjy879YWJ4r516h3PQszT\
        z1EaJXNAXdbk5Em7eyuuabGP1Q3nijFTL2yhMDsXpgrjAuEAABNxFMd4J1JRMaic615mHrhwociksrsfQK
        """

        XCTAssertEqual(output.encoded, expectedString)
    }

    func testDeactivateStakeSigner() throws {
        let deactivateStakeMessage = SolanaDeactivateStake.with {
            $0.stakeAccount = "6u9vJH9pRj66N5oJFCBADEbpMTrLxQATcL6q5p5MXwYv"
        }
        let input = SolanaSigningInput.with {
            $0.deactivateStakeTransaction = deactivateStakeMessage
            $0.recentBlockhash = "11111111111111111111111111111111"
            $0.privateKey = Data(Base58.decodeNoCheck( string: "AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746")!)
        }

        let output: SolanaSigningOutput = AnySigner.sign(input: input, coin: .solana)

        let expectedString = """
        AhfB77PTGTKBfbGPGuEz2khbBy8m8Kou1zqZST9dP7PLJNSeEze5NJuCh5qecPLa3S8xAQ6mTULmnAWiW81ib87nhy\
        wFtx5nKiUvmhdXsvKCSX6NNtNXdRz5yZi3UEop4obco85SY2czS6n4SJwmtDedHLtg9urqdZVth7AUM8KAtrRsksyv\
        ZRYXh64Z8QGyNY7ekj31ae11avGiSDNWYZZHqx7VPWRsKeatGyGk5zPmnRdL8ABMQgJ1Te7wAWwVnNn5QcoAxDuPw6\
        uDctP8Q5S4TieRVatCnukQFj5BTJisez3E2ZJPWhVrMh4K3wEFkPHA7dR
        """

        XCTAssertEqual(output.encoded, expectedString)
    }

    func testWithdrawStakeSigner() throws {
        let withdrawMessage = SolanaWithdrawStake.with {
            $0.stakeAccount = "6u9vJH9pRj66N5oJFCBADEbpMTrLxQATcL6q5p5MXwYv"
            $0.value = 42
        }
        let input = SolanaSigningInput.with {
            $0.withdrawTransaction = withdrawMessage
            $0.recentBlockhash = "11111111111111111111111111111111"
            $0.privateKey = Data(Base58.decodeNoCheck( string: "AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746")!)
        }

        let output: SolanaSigningOutput = AnySigner.sign(input: input, coin: .solana)

        let expectedString = """
        NL7WgagucfLd6AkTtcKe1dqd47xxzF356Q7tEhPrz1LRzZiAmokAaUkpwJ7X71Pmz97zZf9gZQU5BNswdcdpqUL8n1\
        jwn4CoZMaPJhX5LF43Sj817cgreSG14TEWfKertpVpTtc5zY7vkDM7t9wjYhkaqgYz76HQtqAqRHnHF2Qr9EEfLj4z\
        YRerWtyfS3EVyVUaasPxJ5vkcaonEfpGc6uWecaFr2A3YbzEBQpWXjMaXLqmMDtNS8rTNZmwvToa71ddFZKDgaHDcc\
        6Lkg8qriZ3aQbUqL1TbeYp2mk9dWTKY62L1YFE2DyZV5P2qz5feywcMZ9JW6X1wBmiHFCseC42QbnbTibr1VdqLbGx\
        7UWn5tHWk5jCN2aatEPfbFDZ
        """

        XCTAssertEqual(output.encoded, expectedString)
    }

    func testCreateTokenAccountSigner() throws {
        let createAccountMessage = SolanaCreateTokenAccount.with {
            $0.mainAddress = "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V"
            $0.tokenMintAddress = "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt"
            $0.tokenAddress = "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP"
        }
        let input = SolanaSigningInput.with {
            $0.createTokenAccountTransaction = createAccountMessage
            $0.recentBlockhash = "9ipJh5xfyoyDaiq8trtrdqQeAhQbQkWy2eANizKvx75K"
            $0.privateKey = Data(Base58.decodeNoCheck( string: "9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5")!)
        }

        let output: SolanaSigningOutput = AnySigner.sign(input: input, coin: .solana)

        let expectedString = """
        CKzRLx3AQeVeLQ7T4hss2rdbUpuAHdbwXDazxtRnSKBuncCk3WnYgy7XTrEiya19MJviYHYdTxi9gmWJY8qnR2vHVn\
        H2DbPiKA8g72rD3VvMnjosGUBBvCwbBLge6FeQdgczMyRo9n5PcHvg9yJBTJaEEvuewyBVHwCGyGQci7eYd26xtZtC\
        jAjwcTq4gGr3NZbeRW6jZp6j6APuew7jys4MKYRV4xPodua1TZFCkyWZr1XKzmPh7KTavtN5VzPDA8rbsvoEjHnKzj\
        B2Bszs6pDjcBFSHyQqGsHoF8XPD35BLfjDghNtBmf9cFqo5axa6oSjANAuYg6cMSP4Hy28waSj8isr6gQjE315hWi3\
        W1swwwPcn322gYZx6aMAcmjczaxX9aktpHYgZxixF7cYWEHxJs5QUK9mJePu9Xc6yW75UB4Ynx6dUgaSTEUzoQthF2\
        TN3xXwu1
        """

        XCTAssertEqual(output.encoded, expectedString)
    }

    func testTokenTransferSigner() throws {
        let tokenTransferMessage = SolanaTokenTransfer.with {
            $0.tokenMintAddress = "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt"
            $0.senderTokenAddress = "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP"
            $0.recipientTokenAddress = "3WUX9wASxyScbA7brDipioKfXS1XEYkQ4vo3Kej9bKei"
            $0.amount = 4000  // 0.004
            $0.decimals = 6
        }
        let input = SolanaSigningInput.with {
            $0.tokenTransferTransaction = tokenTransferMessage
            $0.recentBlockhash = "CNaHfvqePgGYMvtYi9RuUdVxDYttr1zs4TWrTXYabxZi"
            $0.privateKey = Data(Base58.decodeNoCheck( string: "9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5")!)
        }

        let output: SolanaSigningOutput = AnySigner.sign(input: input, coin: .solana)

        let expectedString = """
        PGfKqEaH2zZXDMZLcU6LUKdBSzU1GJWJ1CJXtRYCxaCH7k8uok38WSadZfrZw3TGejiau7nSpan2GvbK26hQim24jR\
        e2AupmcYJFrgsdaCt1Aqs5kpGjPqzgj9krgxTZwwob3xgC1NdHK5BcNwhxwRtrCphGEH7zUFpGFrFrHzgpf2KY8FvP\
        iPELQyxzTBuyNtjLjMMreehSKShEjD9Xzp1QeC1pEF8JL6vUKzxMXuveoEYem8q8JiWszYzmTMfDk13JPgv7pXFGMq\
        DV3yNGCLsWccBeSFKN4UKECre6x2QbUEiKGkHkMc4zQwwyD8tGmEMBAGm339qdANssEMNpDeJp2LxLDStSoWShHnot\
        crH7pUa94xCVvCPPaomF
        """

        XCTAssertEqual(output.encoded, expectedString)
    }

    func testCreateAndTransferTokenSigner() throws {
        let createAndTransferTokenMessage = SolanaCreateAndTransferToken.with {
            $0.recipientMainAddress = "71e8mDsh3PR6gN64zL1HjwuxyKpgRXrPDUJT7XXojsVd"
            $0.tokenMintAddress = "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt"
            $0.recipientTokenAddress = "EF6L8yJT1SoRoDCkAZfSVmaweqMzfhxZiptKi7Tgj5XY"
            $0.senderTokenAddress = "ANVCrmRw7Ww7rTFfMbrjApSPXEEcZpBa6YEiBdf98pAf"
            $0.amount = 2900
            $0.decimals = 6
        }
        let input = SolanaSigningInput.with {
            $0.createAndTransferTokenTransaction = createAndTransferTokenMessage
            $0.recentBlockhash = "DMmDdJP41M9mw8Z4586VSvxqGCrqPy5uciF6HsKUVDja"
            $0.privateKey = Data(Base58.decodeNoCheck( string: "66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c")!)
        }

        let output: SolanaSigningOutput = AnySigner.sign(input: input, coin: .solana)

        //https://explorer.solana.com/tx/449VaYo48LrkMJF6XVKt9sJwVQN6Seqrmh9erDCLtiuj6BgFG3wpF5TwjNkxgJ7qzNa6NTj3TFsU3h9hKszfkA7w
        let expectedString = """
        3Y2MVz2VVi7aEyC9q1awwdk1ModDBPHRSacKmTYnSgkmbbJeZ62Fub1bVPSHaTy4LUcQpzCQYhHAKtTKXUDYijEeLs\
        MAUqPBEMAq1w8zCdqDpdXy6M4PuwNtYVV1WgqeiEsiMWpPp4BGWKfcziwFbmYueUGituacJq4wTnt92fho8mFi49XW\
        64gEG4iNGScDtJkY7Geq8PKiLh1E9JMJoceiHxKbmxzCmmLTxEHdhySYHcDUSXnXWogZskeZNBMtR9dNjEMkCzEjrx\
        RpBtJPtUNshciY45mDPNmw4j3xyLCBTRikyfFLc5g11r3UgyVD4YokoPRvrEXsgt6W3yjBshropBm6mY2eJYvfY2eZ\
        z4Yq8kLcUatCHVKtjcb1mP9Ww57KisJ9bRhipC8sodFaMYhZARMEa4a1u9eH4MyNUATRGNXarwQSBY46PWS3nKP6QB\
        K7Dw7Ppp9MmYkdPcXKaLScbyLF3jKu6dHWMkHw3WdXSsM1wwXjXnWF9LxdwaEVcDmySWybj6aKD9QCWTU5kdncqJU5\
        6f7SYNRTN289WdUFGNDmSh56tj2v1
        """

        XCTAssertEqual(output.encoded, expectedString)
    }

    func testDefaultTokenAddress() throws {
        let mainAddress = SolanaAddress(string: "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V")!
        let defaultAddress = mainAddress.defaultTokenAddress(tokenMintAddress: "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt")
        XCTAssertEqual(defaultAddress, "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP")
    }

    func testSignJSON() {
        let json = """
            {"recentBlockhash":"11111111111111111111111111111111","transferTransaction":{"recipient":"EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd","value":"42"}}
        """
        let key = Data(hexString: "8778cc93c6596387e751d2dc693bbd93e434bd233bc5b68a826c56131821cb63")!
        let result = AnySigner.signJSON(json, key: key, coin: .solana)

        XCTAssertEqual(result, "3p2kzZ1DvquqC6LApPuxpTg5CCDVPqJFokGSnGhnBHrta4uq7S2EyehV1XNUVXp51D69GxGzQZUjikfDzbWBG2aFtG3gHT1QfLzyFKHM4HQtMQMNXqay1NAeiiYZjNhx9UvMX4uAQZ4Q6rx6m2AYfQ7aoMUrejq298q1wBFdtS9XVB5QTiStnzC7zs97FUEK2T4XapjF1519EyFBViTfHpGpnf5bfizDzsW9kYUtRDW1UC2LgHr7npgq5W9TBmHf9hSmRgM9XXucjXLqubNWE7HUMhbKjuBqkirRM")
    }

    func testDecodeUpdateBlockhashAndSign() throws {
        // https://explorer.solana.com/tx/3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej?cluster=devnet
        let encodedTx = "AnQTYwZpkm3fs4SdLxnV6gQj3hSLsyacpxDdLMALYWObm722f79IfYFTbZeFK9xHtMumiDOWAM2hHQP4r/GtbARpncaXgOVFv7OgbRLMbuCEJHO1qwcdCbtH72VzyzU8yw9sqqHIAaCUE8xaQTgT6Z5IyZfeyMe2QGJIfOjz65UPAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8Aqe6sdLXiXSDILEtzckCjkjchiSf6zVGpMYiAE5BE2IqHAQUEAgQDAQoMoA8AAAAAAAAG"
        let newBlockhash = "CyPYVsYWrsJNfVpi8aazu7WsrswNFuDd385z6GNoBGUg"
        let encodedTxData = Base64.decode(string: encodedTx)!
        
        let senderPrivateKey = Data(hexString: "7f0932159226ddec9e1a4b0b8fe7cdc135049f9e549a867d722aa720dd64f32e")!
        let feePayerPrivateKey = Data(hexString: "4b9d6f57d28b06cbfa1d4cc710953e62d653caf853415c56ffd9d150acdeb7f7")!
        
        // Step 1: Decode the transaction.
        
        let decodeOutputData = TransactionDecoder.decode(coinType: .solana, encodedTx: encodedTxData)
        var decodeOutput = try SolanaDecodingTransactionOutput(serializedData: decodeOutputData)
        
        XCTAssertEqual(decodeOutput.error, .ok)
        
        // Step 2: Update recent blockhash.

        decodeOutput.transaction.legacy.recentBlockhash = newBlockhash
        
        // Step 3: Re-sign the updated transaction.
        
        let signingInput = SolanaSigningInput.with {
            $0.privateKey = senderPrivateKey
            $0.feePayerPrivateKey = feePayerPrivateKey
            $0.rawMessage = decodeOutput.transaction
            $0.txEncoding = .base64
        }
        
        let output: SolanaSigningOutput = AnySigner.sign(input: signingInput, coin: .solana)
        XCTAssertEqual(output.error, .ok)
        XCTAssertEqual(output.encoded, "Ajzc/Tke0CG8Cew5qFa6xZI/7Ya3DN0M8Ige6tKPsGzhg8Bw9DqL18KUrEZZ1F4YqZBo4Rv+FsDT8A7Nss7p4A6BNVZzzGprCJqYQeNg0EVIbmPc6mDitNniHXGeKgPZ6QZbM4FElw9O7IOFTpOBPvQFeqy0vZf/aayncL8EK/UEAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8AqbHiki6ThNH3auuyZPQpJntnN0mA//56nMpK/6HIuu8xAQUEAgQDAQoMoA8AAAAAAAAG")
    }
    
    func testSignFromWalletConnectRequest() throws {
        // Step 1: Parse a signing request received through WalletConnect.
        
        let requestPayload = """
        {"transaction":"AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAAEDZsL1CMnFVcrMn7JtiOiN1U4hC7WovOVof2DX51xM0H/GizyJTHgrBanCf8bGbrFNTn0x3pCGq30hKbywSTr6AgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAgIAAQwCAAAAKgAAAAAAAAA="}
        """
        let parsingInput = WalletConnectParseRequestInput.with {
            $0.method = .solanaSignTransaction
            $0.payload = requestPayload
        }
        let parsingInputBytes = try parsingInput.serializedData()
        
        let parsingOutputBytes = WalletConnectRequest.parse(coin: .solana, input: parsingInputBytes)
        let parsingOutput = try WalletConnectParseRequestOutput(serializedData: parsingOutputBytes)
        
        var signingInput = parsingOutput.solana
        
        // Step 2: Set missing fields.
        
        signingInput.privateKey = Base58.decodeNoCheck(string: "A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr")!
        signingInput.txEncoding = .base64
        
        // Step 3: Sign the transaction.
        
        let output: SolanaSigningOutput = AnySigner.sign(input: signingInput, coin: .solana)
        
        let expected = "AQPWaOi7dMdmQpXi8HyQQKwiqIftrg1igGQxGtZeT50ksn4wAnyH4DtDrkkuE0fqgx80LTp4LwNN9a440SrmoA8BAAEDZsL1CMnFVcrMn7JtiOiN1U4hC7WovOVof2DX51xM0H/GizyJTHgrBanCf8bGbrFNTn0x3pCGq30hKbywSTr6AgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAgIAAQwCAAAAKgAAAAAAAAA="
        XCTAssertEqual(output.encoded, expected)
    }
    
    func testSetPriorityFee() throws {
        let privateKey = Data(hexString: "baf2b2dbbbad7ca96c1fa199c686f3d8fbd2c7b352f307e37e04f33df6741f18")!
        let originalTx = "AX43+Ir2EDqf2zLEvgzFrCZKRjdr3wCdp8CnvYh6N0G/s86IueX9BbiNUl16iLRGvwREDfi2Srb0hmLNBFw1BwABAAEDODI+iWe7g68B9iwCy8bFkJKvsIEj350oSOpcv4gNnv/st+6qmqipl9lwMK6toB9TiL7LrJVfij+pKwr+pUKxfwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAG6GdPcA92ORzVJe2jfG8KQqqMHr9YTLu30oM4i7MFEoBAgIAAQwCAAAA6AMAAAAAAAA="

        // Step 1 - Check if there are no price and limit instructions in the original transaction.
        XCTAssertEqual(SolanaTransaction.getComputeUnitPrice(encodedTx: originalTx), nil)
        XCTAssertEqual(SolanaTransaction.getComputeUnitLimit(encodedTx: originalTx), nil)

        // Step 2 - Set price and limit instructions.
        let txWithPrice = SolanaTransaction.setComputeUnitPrice(encodedTx: originalTx, price: "1000")!
        let updatedTx = SolanaTransaction.setComputeUnitLimit(encodedTx: txWithPrice, limit: "10000")!

        XCTAssertEqual(updatedTx, "AX43+Ir2EDqf2zLEvgzFrCZKRjdr3wCdp8CnvYh6N0G/s86IueX9BbiNUl16iLRGvwREDfi2Srb0hmLNBFw1BwABAAIEODI+iWe7g68B9iwCy8bFkJKvsIEj350oSOpcv4gNnv/st+6qmqipl9lwMK6toB9TiL7LrJVfij+pKwr+pUKxfwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwZGb+UhFzL/7K26csOb57yM5bvF9xJrLEObOkAAAAAboZ09wD3Y5HNUl7aN8bwpCqowev1hMu7fSgziLswUSgMDAAUCECcAAAICAAEMAgAAAOgDAAAAAAAAAwAJA+gDAAAAAAAA")

        // Step 3 - Check if price and limit instructions are set successfully.
        XCTAssertEqual(SolanaTransaction.getComputeUnitPrice(encodedTx: updatedTx), "1000")
        XCTAssertEqual(SolanaTransaction.getComputeUnitLimit(encodedTx: updatedTx), "10000")

        // Step 4 - Decode transaction into a `RawMessage` Protobuf.
        let updatedTxData = Base64.decode(string: updatedTx)!
        let decodeOutputData = TransactionDecoder.decode(coinType: .solana, encodedTx: updatedTxData)
        var decodeOutput = try SolanaDecodingTransactionOutput(serializedData: decodeOutputData)

        XCTAssertEqual(decodeOutput.error, .ok)

        // Step 5 - Sign the decoded `RawMessage` transaction.
        let signingInput = SolanaSigningInput.with {
            $0.privateKey = privateKey
            $0.rawMessage = decodeOutput.transaction
            $0.txEncoding = .base64
        }

        let output: SolanaSigningOutput = AnySigner.sign(input: signingInput, coin: .solana)
        XCTAssertEqual(output.error, .ok)
        // Successfully broadcasted tx:
        // https://explorer.solana.com/tx/2ho7wZUXbDNz12xGfsXg2kcNMqkBAQjv7YNXNcVcuCmbC4p9FZe9ELeM2gMjq9MKQPpmE3nBW5pbdgwVCfNLr1h8
        XCTAssertEqual(output.encoded, "AVUye82Mv+/aWeU2G+B6Nes365mUU2m8iqcGZn/8kFJvw4wY6AgKGG+vJHaknHlCDwE1yi1SIMVUUtNCOm3kHg8BAAIEODI+iWe7g68B9iwCy8bFkJKvsIEj350oSOpcv4gNnv/st+6qmqipl9lwMK6toB9TiL7LrJVfij+pKwr+pUKxfwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwZGb+UhFzL/7K26csOb57yM5bvF9xJrLEObOkAAAAAboZ09wD3Y5HNUl7aN8bwpCqowev1hMu7fSgziLswUSgMDAAUCECcAAAICAAEMAgAAAOgDAAAAAAAAAwAJA+gDAAAAAAAA")
    }

    func testSetFeePayer() throws {
        // base64 encoded
        let originalTx = "AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAQABA2uEKrOPvZNBtdUtSFXcg8+kj4O/Z1Ht/hwvnaqq5s6mTXd3KtwUyJFfRs2PBfeQW8xCEZvNr/5J/Tx8ltbn0pwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACo+QRbvXWNKoOfaOL4cSpfYrmn/2TV+dBmct+HsmmwdAQICAAEMAgAAAACcnwYAAAAAAA=="

        // Step 1 - Add fee payer to the transaction.
        let updatedTx = SolanaTransaction.setFeePayer(encodedTx: originalTx, feePayer: "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ")!

        XCTAssertEqual(updatedTx, "AgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgAIAAQTLKvCJtWpVdze8Fxjgy/Iyz1sC4U7gqnxmdSM/X2+bV2uEKrOPvZNBtdUtSFXcg8+kj4O/Z1Ht/hwvnaqq5s6mTXd3KtwUyJFfRs2PBfeQW8xCEZvNr/5J/Tx8ltbn0pwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACo+QRbvXWNKoOfaOL4cSpfYrmn/2TV+dBmct+HsmmwdAQMCAQIMAgAAAACcnwYAAAAAAA==")

        // Step 2 - Decode transaction into a `RawMessage` Protobuf.
        let updatedTxData = Base64.decode(string: updatedTx)!
        let decodeOutputData = TransactionDecoder.decode(coinType: .solana, encodedTx: updatedTxData)
        let decodeOutput = try SolanaDecodingTransactionOutput(serializedData: decodeOutputData)

        XCTAssertEqual(decodeOutput.error, .ok)

        // Step 3 - Obtain preimage hash.
        let signingInput = SolanaSigningInput.with {
            $0.rawMessage = decodeOutput.transaction
            $0.txEncoding = .base64
        }
        let txInputData = try signingInput.serializedData() // Serialize input
        let preImageHashes = TransactionCompiler.preImageHashes(coinType: .solana, txInputData: txInputData)
        let preSigningOutput: SolanaPreSigningOutput = try SolanaPreSigningOutput(serializedData: preImageHashes)
        XCTAssertEqual(preSigningOutput.error, CommonSigningError.ok)
        XCTAssertEqual(preSigningOutput.data.hexString, "8002000104cb2af089b56a557737bc1718e0cbf232cf5b02e14ee0aa7c6675233f5f6f9b576b842ab38fbd9341b5d52d4855dc83cfa48f83bf6751edfe1c2f9daaaae6cea64d77772adc14c8915f46cd8f05f7905bcc42119bcdaffe49fd3c7c96d6e7d29c00000000000000000000000000000000000000000000000000000000000000002a3e4116ef5d634aa0e7da38be1c4a97d8ae69ffd9357e74199cb7e1ec9a6c1d01030201020c02000000009c9f060000000000")

        // Step 4 - Compile transaction info.
        // Simulate signature, normally obtained from signature server.
        let signatureVec = DataVector()
        let pubkeyVec = DataVector()
        let feePayerSignature = Data(hexString: "feb9f15cc345fa156450676100033860edbe80a6f61dab8199e94fdc47678ecfdb95e3bc10ec0a7f863ab8ef5c38edae72db7e5d72855db225fd935fd59b700a")!
        let feePayerPublicKey = Data(hexString: "cb2af089b56a557737bc1718e0cbf232cf5b02e14ee0aa7c6675233f5f6f9b57")!
        signatureVec.add(data: feePayerSignature)
        pubkeyVec.add(data: feePayerPublicKey)
        let solSenderSignature = Data(hexString: "936cd6d176e701d1f748031925b2f029f6f1ab4b99aec76e24ccf05649ec269569a08ec0bd80f5fee1cb8d13ecd420bf50c5f64ae74c7afa267458cabb4e5804")!
        let solSenderPublicKey = Data(hexString: "6b842ab38fbd9341b5d52d4855dc83cfa48f83bf6751edfe1c2f9daaaae6cea6")!
        signatureVec.add(data: solSenderSignature)
        pubkeyVec.add(data: solSenderPublicKey)

        let compileWithSignatures = TransactionCompiler.compileWithSignatures(coinType: .solana, txInputData: txInputData, signatures: signatureVec, publicKeys: pubkeyVec)
        let expectedTx = "Av658VzDRfoVZFBnYQADOGDtvoCm9h2rgZnpT9xHZ47P25XjvBDsCn+GOrjvXDjtrnLbfl1yhV2yJf2TX9WbcAqTbNbRducB0fdIAxklsvAp9vGrS5mux24kzPBWSewmlWmgjsC9gPX+4cuNE+zUIL9QxfZK50x6+iZ0WMq7TlgEgAIAAQTLKvCJtWpVdze8Fxjgy/Iyz1sC4U7gqnxmdSM/X2+bV2uEKrOPvZNBtdUtSFXcg8+kj4O/Z1Ht/hwvnaqq5s6mTXd3KtwUyJFfRs2PBfeQW8xCEZvNr/5J/Tx8ltbn0pwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACo+QRbvXWNKoOfaOL4cSpfYrmn/2TV+dBmct+HsmmwdAQMCAQIMAgAAAACcnwYAAAAAAA=="
        let output: SolanaSigningOutput = try SolanaSigningOutput(serializedData: compileWithSignatures)
        XCTAssertEqual(output.encoded, expectedTx)
        // Successfully broadcasted tx:
        // https://explorer.solana.com/tx/66PAVjxFVGP4ctrkXmyNRhp6BdFT7gDe1k356DZzCRaBDTmJZF1ewGsbujWRjDTrt5utnz8oHZw3mg8qBNyct41w?cluster=devnet
    }
    
    func testSignUserMessage() throws {
        let privateKey = Data(hexString: "44f480ca27711895586074a14c552e58cc52e66a58edb6c58cf9b9b7295d4a2d")!

        let input = SolanaMessageSigningInput.with {
            $0.privateKey = privateKey
            $0.message = "Hello world"
        }
        let outputData = MessageSigner.sign(coin: .solana, input: try input.serializedData())!
        let output = try SolanaMessageSigningOutput(serializedData: outputData)
        XCTAssertEqual(output.error, .ok)
        XCTAssertEqual(output.signature, "2iBZ6zrQRKHcbD8NWmm552gU5vGvh1dk3XV4jxnyEdRKm8up8AeQk1GFr9pJokSmchw7i9gMtNyFBdDt8tBxM1cG")
    }
}
