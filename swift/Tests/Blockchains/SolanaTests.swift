// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
        let delegateStakeMessage = SolanaStake.with {
            $0.validatorPubkey = "4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC"
            $0.value = 42
        }
        let input = SolanaSigningInput.with {
            $0.stakeTransaction = delegateStakeMessage
            $0.recentBlockhash = "11111111111111111111111111111111"
            $0.privateKey = Data(Base58.decodeNoCheck( string: "AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746")!)
        }

        let output: SolanaSigningOutput = AnySigner.sign(input: input, coin: .solana)

        let expectedString = """
        W1EAswaWK7mF4r9eZ2hHBZnfPnqLuNPiYkEMzFbwQsgSQu6XbSTL9AN92iyMbAMxPoRpt9ipUyztrmszAnm688N3k7\
        uhiKn2osm9nxi6YkGLfu31jHTSu7mn3RtmenV3qopfPDAM7jtGoYQFb7eFVbujUb6tbeQ9UqLJq1sJ7uMZ4wqecmQP\
        ouDmJnpmJk4CHMzLnPNTwyGmGio6sYAS3xKZ7DFXvjwGPuD8PyYHSfdPro1p3jy9igPZNAbQ6fgK7LL3sERKCUdvPy\
        7k14xgHbtsVy2mu54LY5c8F9sFst2uzQiTsXRTdjPFAyCVwB5pccNVotCrJ6Q2aKSC2D2knVH7LgWzSBMSreJG75xy\
        ATneu922wSzz7QJDieqhDtdePtSbPtoCdtPNmDfdaeDbHxVAxMios9F7RSRmH2dq86NfWDvF8TuEbYY7gPnygz6jGv\
        wfqSSoSnY8TnUhhceC7wJSMc8Hcf1kyfi8dqKm7rF57YjnrQoMmL5bWqJLKoJtdfFu24ceQN21k38U2tUMWJaBASWu\
        kgTJUbNSCemNPZt4P3cNbeB3L1wBj4GEYXVTbTFYKME5JscU5RsnkMJZZ1PgzU285SkncqNSgxkpZVhmenTXpuZv74\
        rXzariX8P4sprRgKUoj4b7Nu72Pya1zr7k45isMwgxtLnnnTK5k7mrZRDw3jBSBuukJBja93zaidm8HCQdwQsBt5CN\
        SgSXug1R2t6Sdm5tjJrsd1gyRv7udFbHCdbVEeatzULNSSGdwjwwJDy1DTC12ddBNHd8k5ic5TDwrWdfCxbDRoFYw8\
        49YNNUuyNAPz1jDCkLG9af6KFFLxfuR9pnF8jSyTcQAq95YiiD9sC3mAUoe8AkYfy929XzTEatP1vasMvo
        """

        XCTAssertEqual(output.encoded, expectedString)
    }

    func testDeactivateStakeSigner() throws {
        let deactivateStakeMessage = SolanaDeactivateStake.with {
            $0.validatorPubkey = "4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC"
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
            $0.validatorPubkey = "4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC"
            $0.value = 42
        }
        let input = SolanaSigningInput.with {
            $0.withdrawTransaction = withdrawMessage
            $0.recentBlockhash = "11111111111111111111111111111111"
            $0.privateKey = Data(Base58.decodeNoCheck( string: "AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746")!)
        }

        let output: SolanaSigningOutput = AnySigner.sign(input: input, coin: .solana)

        let expectedString = """
        7Y1Wg1yHNs8MgWFiFSfcsRtqdMwZg8oGeQnTABYDfyDnof4VSFw63s3PuSxvUCJqqHKgYNVb8UTNcNiYHY8kng4NqT\
        cVV5SA1KAWRzKHVGUxNWioAEXXVot5iJ1XbUWuuZUZBtsraaBjNyfmgWEDje3ESdGhiVL7vadU1uHeBuUKwM3nqB6y\
        oeggeNyzmT34hs9utyehTFg48MAfrKEFKxaby7YZD6JbXFS1SyG1kxKWnCpoPgX3efwDwukmyDwxrKdABt9eTwmaiX\
        KbTnK1hzBTatNfnJ9ePuWkhWFrjyDrGdx5S5KpybxET2vV9CSpExcD51BA6NPemTpjbhLYnJEzHWBGfYqfxu7p3257\
        NHhpQQrSU56adk4dAQFjEYP
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
}
