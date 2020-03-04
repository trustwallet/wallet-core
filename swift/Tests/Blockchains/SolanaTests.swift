// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

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
        7gcsGMV8fPkuSDegrBk5KF4eZtmGqdixEr5Kxx7m74nErH5EWxkKcvy5LKg3kqKHLMtzqGXrM8EdVskLu5k6cQUVmz\
        nfd6iGpuDkn4BWi8HaqSoaeL7DCx6QHLUwcyvorfqPkgSaYopY9MeB6MGKS4bKCQi1paP7eEKZj9JgAZTHnpzAuFzq\
        E5dC98MrbThoBmHqfybw1jHssp4NZjrxsuMz4oYcHe837WmnpinE68QEVf9FzuoXKrMDwYZQYogJHUZgxghvMmJu3X\
        VZk7hL1h7SgE9ow2SvvNfAUYfUTuz8N9m6JcXgwmNL51ZweW9F1fekmSKE3vLEAaVvvYFsDxn1gt4bXuTdAMuUDKdH\
        33YMLQr8eEsdUDZUm33KtuE7Ddy84NEg3KbpWAy8T4vkwiLyvoyjmMdzCkMuobyXWQV1rfRk5vTJ4x6dMgznfAeKEq\
        md6xZ2hN8JBmTenP5dsvZCCSCf4G7cUkdndKu552KALqiNHd5msgLJvJKSHmZjFhS43fDCvkG7njF8yaZzWjmAknWU\
        BbV6YaGmD3XmcWfJgvB1zivZJhiMbzopP8Nm5wL5iDbCrSZTGq2tzEsTvje75wv2RtuAcgiicEPTuPAin9fKyMbCpf\
        67pGgWPwH5DwYumMwd8zwoJyuakyqFsFLnBKTvp8pFCijdj7fEhyC31xuMV7crwyrN5X3y7QKCE7PZcBP637YHEPtT\
        vt1ECp4CqBSnvPc8vRD8EMhHe5jRFSDkQriUenEPFc51dTDTJWL26xuiTivktEm6ahHq5d6MPr4NRDvcRG2cZvEgxH\
        BLpKfuB5XL3JfQZ3Nn3B916gaK8owz9Rk2e3
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

}
