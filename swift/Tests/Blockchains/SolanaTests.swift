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

    func testSigner() throws {
        let input = SolanaSigningInput.with {
            $0.privateKey = Data(Base58.decodeNoCheck( string: "A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr")!)
            $0.recipient = "EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd"
            $0.lamports = 42
            $0.recentBlockhash = "11111111111111111111111111111111"
        }

        let output = SolanaSigner.sign(input: input)

        let expectedTransactionBytes = Data([
            1,   253, 161, 200, 173, 136, 114, 217, 79,  126, 171, 82,  249, 195, 141, 199, 126,
            16,  97,  244, 137, 126, 61,  226, 184, 70,  158, 176, 153, 34,  105, 246, 250, 31,
            23,  62,  147, 219, 178, 218, 115, 138, 180, 232, 149, 159, 250, 80,  205, 8,   124,
            223, 168, 137, 243, 161, 184, 172, 221, 98,  85,  47,  124, 29,  7,   1,   0,   2,
            3,   102, 194, 245, 8,   201, 197, 85,  202, 204, 159, 178, 109, 136, 232, 141, 213,
            78,  33,  11,  181, 168, 188, 229, 104, 127, 96,  215, 231, 92,  76,  208, 127, 198,
            139, 60,  137, 76,  120, 43,  5,   169, 194, 127, 198, 198, 110, 177, 77,  78,  125,
            49,  222, 144, 134, 171, 125, 33,  41,  188, 176, 73,  58,  250, 2,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   2,   2,   0,   1,   12,  2,
            0,   0,   0,   42,  0,   0,   0,   0,   0,   0,   0
        ])

        // swiftlint:disable:next line_length
        XCTAssertEqual(output.encoded, expectedTransactionBytes)
    }

}
