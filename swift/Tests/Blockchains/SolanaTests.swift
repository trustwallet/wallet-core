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

        let expectedHexString = """
        0103d668e8bb74c7664295e2f07c9040ac22a887edae0d628064311ad65e4f9d24b27e30027c87e03b43ae492e\
        1347ea831f342d3a782f034df5ae38d12ae6a00f0100010366c2f508c9c555cacc9fb26d88e88dd54e210bb5a8\
        bce5687f60d7e75c4cd07fc68b3c894c782b05a9c27fc6c66eb14d4e7d31de9086ab7d2129bcb0493afa020000\
        000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\
        000000000000000000000000000000000001020200010c020000002a00000000000000
        """

        XCTAssertEqual(output.encoded.hexString, expectedHexString)
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

        let expectedHexString = """
        01a9ec9c553c565d232ac98892a1d926ccedb50b85ea227f3d2acef5a5e203573de157d843e578f0c01f2d9f83\
        dd70e6adcc396ee474626e3168bb6b93176be602010006080eba44e56f060007284dc037275a15094c1d6c0697\
        ddb28b2be661dfb0f4bab857a6b8aba83da905fff9c3e62acc748cf497bd503f11b7f9c337b7f35346df0306a7\
        d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a00000000378ba8d9f9881e9be69cf1d70ee0a9\
        3ed0378b83203f42fa29f9df5c887f1c0d06a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b21\
        0000000006a1d817a502050b680791e6ce6db88e1e5b7150f61fc6790a4eb4d100000000000000000000000000\
        000000000000000000000000000000000000000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78\
        722b68a49dc0000000000000000000000000000000000000000000000000000000000000000000000000030602\
        00017c030000000eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab8200000000000\
        0000346a707754717431715a6f52377536753633397a32416e675946474e336e616b2a00000000000000d80700\
        000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc0000000000702010274000000\
        000eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab80eba44e56f060007284dc037\
        275a15094c1d6c0697ddb28b2be661dfb0f4bab800000000000000000000000000000000000000000000000000\
        0000000000000000000000000000000000000000000000070501030405000402000000
        """

        XCTAssertEqual(output.encoded.hexString, expectedHexString)
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

        let expectedHexString = """
        01f8edf7c745a5ac56682800b79ca94a2ca890e642f391b90e4f130119254d3b5e61bdec185b3650c5ec4e9b63\
        dd62b553809d9a960b947e7c0c4ad1b23a2cba07010002040eba44e56f060007284dc037275a15094c1d6c0697\
        ddb28b2be661dfb0f4bab857a6b8aba83da905fff9c3e62acc748cf497bd503f11b7f9c337b7f35346df0306a7\
        d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b210000000006a1d8179137542a983437bdfe2a7a\
        b2557f535c8a78722b68a49dc00000000000000000000000000000000000000000000000000000000000000000\
        000000000103030102000405000000
        """

        XCTAssertEqual(output.encoded.hexString, expectedHexString)
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

        let expectedHexString = """
        015f5eb11f1e3416c38d30bc9d966948ff58dc56849bf4b58e2538e2ab3bd7e951aeca5a317518dee81da76025\
        471435257cde34a5d8ed8745e4abd7c7d3d9af0d010003050eba44e56f060007284dc037275a15094c1d6c0697\
        ddb28b2be661dfb0f4bab857a6b8aba83da905fff9c3e62acc748cf497bd503f11b7f9c337b7f35346df0306a7\
        d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b210000000006a7d517193584d0feed9bb3431d13\
        206be544281b57b8566cc5375ff400000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc0\
        000000000000000000000000000000000000000000000000000000000000000000000000010404010002030c04\
        0000002a00000000000000
        """

        XCTAssertEqual(output.encoded.hexString, expectedHexString)
    }

}
