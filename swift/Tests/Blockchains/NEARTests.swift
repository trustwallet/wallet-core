// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class NEARTests: XCTestCase {

    func testAddress() {
        let hex = "3b83b07cab54824a59c3d3f2e203a7cd913b7fcdc4439595983e2402c2cf791d"
        let string = "NEARTDDWrUMdoC2rA1eU6gNrSU2zyGKdR71TNucTvsQHyfAXjKcJb"
        let pubKey = PublicKey(data: Data(hexString: hex)!, type: .ed25519)!
        let address = AnyAddress(publicKey: pubKey, coin: .near)
        let expected = AnyAddress(string: string, coin: .near)!

        XCTAssertEqual(address, expected)
        XCTAssertEqual(address.description, hex)
        XCTAssertEqual(expected.data.hexString, hex)
    }

    func testAccount() {
        let string = "NEARTDDWrUMdoC2rA1eU6gNrSU2zyGKdR71TNucTvsQHyfAXjKcJb"
        let account = NEARAccount(string: string)!
        XCTAssertEqual(account.description, "3b83b07cab54824a59c3d3f2e203a7cd913b7fcdc4439595983e2402c2cf791d")

        let account2 = NEARAccount(string: "test.near")!
        XCTAssertEqual(account2.description, "test.near")

        let account3 = NEARAccount(string: "3b83b07cab54824a59c3d3f2e203a7cd913b7fcdc4439595983e2402c2cf791d")!
        XCTAssertEqual(account3.description, "3b83b07cab54824a59c3d3f2e203a7cd913b7fcdc4439595983e2402c2cf791d")
    }

    func testAddressValidation() {
        let near = CoinType.near
        for address in [
            ("NEAR2758Nk7CMUcxTwXdjVdSxNEidiZQWMZN3USJzj76q5ia3v2v2v", "917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d"),
            ("NEARTDDWrUMdoC2rA1eU6gNrSU2zyGKdR71TNucTvsQHyfAXjKcJb",
            "3b83b07cab54824a59c3d3f2e203a7cd913b7fcdc4439595983e2402c2cf791d")
        ] {
            XCTAssertTrue(near.validate(address: address.0))
            XCTAssertEqual(near.address(string: address.0)?.description, address.1)
        }
    }

    func testSigningTransaction() {
        // swiftlint:disable:next line_length
        let privateKey = PrivateKey(data: Base58.decodeNoCheck(string: "3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv")!.subdata(in: 0..<32))!

        let input = NEARSigningInput.with {
            $0.signerID = "test.near"
            $0.nonce = 1
            $0.receiverID = "whatever.near"

            $0.actions = [
                NEARAction.with({
                    $0.transfer = NEARTransfer.with {
                        // uint128_t / little endian byte order
                        $0.deposit = Data(hexString: "01000000000000000000000000000000")!
                    }
                }),
            ]

            $0.blockHash = Base58.decodeNoCheck(string: "244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM")!
            $0.privateKey = privateKey.data
        }

        let output: NEARSigningOutput = AnySigner.sign(input: input, coin: .near)
        // swiftlint:disable:next line_length
        let expectedBase64Encoded = "CQAAAHRlc3QubmVhcgCRez0mjUtY9/7BsVC9aNab4+5dTMOYVeNBU4Rlu3eGDQEAAAAAAAAADQAAAHdoYXRldmVyLm5lYXIPpHP9JpAd8pa+atxMxN800EDvokNSJLaYaRDmMML+9gEAAAADAQAAAAAAAAAAAAAAAAAAAACWmoMzIYbul1Xkg5MlUlgG4Ymj0tK7S0dg6URD6X4cTyLe7vAFmo6XExAO2m4ZFE2n6KDvflObIHCLodjQIb0B"

        XCTAssertEqual(Data(base64Encoded: expectedBase64Encoded), output.signedTransaction)
    }
}
