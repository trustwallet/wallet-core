// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class SolanaTests: XCTestCase {

    func testAddressFromPrivateKey() {
        let privateKey = PrivateKey(data: Data(Base58.decodeNoCheck(string: "A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr")!))!
        let publicKey = privateKey.getPublicKeyEd25519()
        let address = SolanaAddress(publicKey: publicKey)
        XCTAssertEqual(address.description, "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q")
    }

    func testAddressFromPublicKey() {
        let publicKey = PublicKey(data: Data(Base58.decodeNoCheck(string: "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q")!), type: PublicKeyType.ed25519)!
        let address = SolanaAddress(publicKey: publicKey)
        XCTAssertEqual(address.description, "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q")
    }

    func testAddressFromString() {
        let addressString = "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q"
        let address = SolanaAddress(string: addressString)
        XCTAssertEqual(address!.description, "7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q")
    }

    func testTransferSigner() throws {
        let transferMessage = SolanaTransfer.with {
            $0.privateKey = Data(Base58.decodeNoCheck( string: "A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr")!)
            $0.recipient = "EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd"
            $0.amount = 42
        }
        let input = SolanaSigningInput.with {
            $0.transferTransaction = transferMessage
            $0.recentBlockhash = "11111111111111111111111111111111"
        }

        let output = SolanaSigner.sign(input: input)

        let expectedTransactionBytes = Data([
            1, 253, 161, 200, 173, 136, 114, 217, 79, 126, 171, 82, 249, 195, 141, 199, 126,
            16, 97, 244, 137, 126, 61, 226, 184, 70, 158, 176, 153, 34, 105, 246, 250, 31,
            23, 62, 147, 219, 178, 218, 115, 138, 180, 232, 149, 159, 250, 80, 205, 8, 124,
            223, 168, 137, 243, 161, 184, 172, 221, 98, 85, 47, 124, 29, 7, 1, 0, 2,
            3, 102, 194, 245, 8, 201, 197, 85, 202, 204, 159, 178, 109, 136, 232, 141, 213,
            78, 33, 11, 181, 168, 188, 229, 104, 127, 96, 215, 231, 92, 76, 208, 127, 198,
            139, 60, 137, 76, 120, 43, 5, 169, 194, 127, 198, 198, 110, 177, 77, 78, 125,
            49, 222, 144, 134, 171, 125, 33, 41, 188, 176, 73, 58, 250, 2, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 0, 1, 12, 2,
            0, 0, 0, 42, 0, 0, 0, 0, 0, 0, 0,
        ])

        XCTAssertEqual(output.encoded, expectedTransactionBytes)
    }

    func testDelegateStakeSigner() throws {
        let delegateStakeMessage = SolanaStake.with {
            $0.fromPrivateKey = Data(Base58.decodeNoCheck( string: "GGT4G41n1K3E4MTjb7VwADSFNJA3Jx7wUxm54Fpcje6w")!)
            $0.stakePrivateKey = Data(Base58.decodeNoCheck( string: "2bwUDLUVYCfUhQHiAiwvHzM8oNT7pdk5J1XjhTLeumP5")!)
            $0.votePubkey = "FkL2bzbUbp3J9MQEX3toMBA4q8ZcHcjeacdtn2Ti8Qec"
            $0.amount = 42
        }
        let input = SolanaSigningInput.with {
            $0.stakeTransaction = delegateStakeMessage
            $0.recentBlockhash = "11111111111111111111111111111111"
        }

        let output = SolanaSigner.sign(input: input)

        let expectedTransactionBytes = Data([
            2, 214, 192, 155, 86, 43, 203, 65, 232, 21, 242, 217, 163, 5, 17, 169, 50, 70,
            29, 245, 160, 199, 42, 14, 96, 43, 202, 132, 255, 81, 6, 125, 99, 156, 250, 166,
            59, 197, 110, 189, 146, 114, 115, 31, 136, 33, 169, 150, 87, 69, 171, 207, 226, 162,
            10, 243, 80, 88, 147, 60, 88, 135, 115, 154, 10, 175, 18, 96, 67, 123, 25, 16,
            205, 51, 44, 228, 165, 27, 10, 92, 10, 18, 84, 86, 20, 44, 14, 150, 171, 123,
            123, 16, 179, 69, 92, 249, 202, 144, 51, 164, 146, 35, 182, 110, 46, 56, 159, 9,
            147, 104, 155, 46, 193, 164, 140, 22, 159, 207, 11, 146, 5, 16, 243, 14, 24, 0,
            254, 166, 6, 2, 0, 5, 7, 175, 143, 134, 108, 1, 179, 182, 122, 138, 126, 223,
            218, 36, 36, 164, 134, 146, 16, 243, 97, 169, 141, 183, 130, 133, 135, 133, 111, 14,
            184, 239, 180, 73, 166, 2, 145, 8, 184, 74, 27, 252, 116, 189, 17, 204, 230, 224,
            205, 103, 45, 171, 233, 211, 153, 58, 2, 143, 63, 235, 221, 183, 24, 55, 115, 219,
            29, 27, 60, 1, 43, 59, 72, 110, 238, 229, 98, 127, 161, 92, 84, 236, 139, 51,
            200, 13, 212, 54, 215, 45, 15, 189, 205, 4, 83, 117, 105, 6, 167, 213, 23, 24,
            199, 116, 201, 40, 86, 99, 152, 105, 29, 94, 182, 139, 94, 184, 163, 155, 75, 109,
            92, 115, 85, 91, 33, 0, 0, 0, 0, 6, 161, 216, 23, 165, 2, 5, 11, 104,
            7, 145, 230, 206, 109, 184, 142, 30, 91, 113, 80, 246, 31, 198, 121, 10, 78, 180,
            209, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 6, 161, 216, 23, 145, 55, 84, 42, 152, 52, 55, 189, 254, 42, 122, 178, 85,
            127, 83, 92, 138, 120, 114, 43, 104, 164, 157, 192, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 5, 2, 0, 1, 52, 0,
            0, 0, 0, 42, 0, 0, 0, 0, 0, 0, 0, 96, 0, 0, 0, 0, 0, 0,
            0, 6, 161, 216, 23, 145, 55, 84, 42, 152, 52, 55, 189, 254, 42, 122, 178, 85,
            127, 83, 92, 138, 120, 114, 43, 104, 164, 157, 192, 0, 0, 0, 0, 6, 4, 1,
            2, 3, 4, 12, 0, 0, 0, 0, 42, 0, 0, 0, 0, 0, 0, 0,
        ])

        XCTAssertEqual(output.encoded, expectedTransactionBytes)
    }

    func testDeactivateStakeSigner() throws {
        let deactivateStakeMessage = SolanaDeactivateStake.with {
            $0.stakePrivateKey = Data(Base58.decodeNoCheck( string: "5PcaDJjTMnZEqJzayijWhYJAbUuURjtkJq8Zi2HD2k7Q")!)
            $0.votePubkey = "B7Cx2wYAry78VNR8uoewzFDq3FRKJh8exNMyzrpQSfLB"
        }
        let input = SolanaSigningInput.with {
            $0.deactivateStakeTransaction = deactivateStakeMessage
            $0.recentBlockhash = "11111111111111111111111111111111"
        }

        let output = SolanaSigner.sign(input: input)

        let expectedTransactionBytes = Data([
            1, 154, 85, 52, 1, 42, 231, 52, 8, 111, 220, 239, 94, 120, 56, 200, 138, 2,
            94, 226, 221, 150, 185, 69, 117, 59, 221, 93, 144, 217, 180, 54, 99, 180, 127, 103,
            78, 32, 131, 87, 89, 203, 124, 80, 163, 35, 141, 177, 241, 217, 97, 190, 83, 247,
            180, 216, 203, 152, 155, 0, 61, 22, 22, 60, 12, 1, 0, 3, 4, 187, 37, 41,
            98, 48, 195, 77, 3, 240, 119, 243, 235, 53, 196, 180, 162, 202, 67, 107, 239, 46,
            70, 40, 183, 122, 50, 187, 116, 61, 109, 248, 234, 150, 43, 221, 94, 220, 188, 111,
            147, 132, 31, 104, 173, 232, 106, 39, 151, 86, 115, 77, 187, 146, 27, 109, 28, 241,
            170, 84, 96, 219, 228, 15, 40, 6, 167, 213, 23, 24, 199, 116, 201, 40, 86, 99,
            152, 105, 29, 94, 182, 139, 94, 184, 163, 155, 75, 109, 92, 115, 85, 91, 33, 0,
            0, 0, 0, 6, 161, 216, 23, 145, 55, 84, 42, 152, 52, 55, 189, 254, 42, 122,
            178, 85, 127, 83, 92, 138, 120, 114, 43, 104, 164, 157, 192, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 0, 1,
            2, 4, 3, 0, 0, 0,
        ])

        XCTAssertEqual(output.encoded, expectedTransactionBytes)
    }

    func testWithdrawStakeSigner() throws {
        let withdrawMessage = SolanaWithdrawStake.with {
            $0.stakePrivateKey = Data(Base58.decodeNoCheck( string: "5PcaDJjTMnZEqJzayijWhYJAbUuURjtkJq8Zi2HD2k7Q")!)
            $0.recipient = "C3e7ryQjYJFSUetohBofTaWEBbNcq4yVX43hi7igVtcP"
            $0.amount = 42
        }
        let input = SolanaSigningInput.with {
            $0.withdrawTransaction = withdrawMessage
            $0.recentBlockhash = "11111111111111111111111111111111"
        }

        let output = SolanaSigner.sign(input: input)

        let expectedTransactionBytes = Data([
            1, 15, 250, 1, 156, 204, 232, 44, 124, 190, 212, 143, 118, 137, 113, 69, 224, 206,
            114, 111, 158, 115, 170, 244, 155, 211, 1, 195, 186, 6, 53, 232, 64, 245, 72, 56,
            223, 110, 51, 120, 163, 210, 57, 192, 23, 120, 50, 81, 192, 15, 3, 15, 115, 247,
            220, 161, 200, 166, 78, 107, 52, 105, 238, 204, 1, 1, 0, 4, 5, 187, 37, 41,
            98, 48, 195, 77, 3, 240, 119, 243, 235, 53, 196, 180, 162, 202, 67, 107, 239, 46,
            70, 40, 183, 122, 50, 187, 116, 61, 109, 248, 234, 164, 29, 176, 7, 10, 227, 174,
            227, 12, 108, 226, 89, 248, 26, 73, 205, 21, 96, 63, 226, 155, 71, 186, 128, 172,
            115, 79, 243, 145, 20, 98, 160, 6, 167, 213, 23, 24, 199, 116, 201, 40, 86, 99,
            152, 105, 29, 94, 182, 139, 94, 184, 163, 155, 75, 109, 92, 115, 85, 91, 33, 0,
            0, 0, 0, 6, 167, 213, 23, 25, 53, 132, 208, 254, 237, 155, 179, 67, 29, 19,
            32, 107, 229, 68, 40, 27, 87, 184, 86, 108, 197, 55, 95, 244, 0, 0, 0, 6,
            161, 216, 23, 145, 55, 84, 42, 152, 52, 55, 189, 254, 42, 122, 178, 85, 127, 83,
            92, 138, 120, 114, 43, 104, 164, 157, 192, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 4, 0, 1, 2, 3, 12, 2,
            0, 0, 0, 42, 0, 0, 0, 0, 0, 0, 0,
        ])

        XCTAssertEqual(output.encoded, expectedTransactionBytes)
    }

}
