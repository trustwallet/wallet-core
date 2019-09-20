// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class NEARAddressTests: XCTestCase {
    func testAddressValidation() {
        let near = CoinType.near
        for address in [
            "NEAR2758Nk7CMUcxTwXdjVdSxNEidiZQWMZN3USJzj76q5ia3v2v2v",
            "NEAR6Y66fCzeKqWiwxoPox5oGeDN9VhNCu7CEQ9M86iniqoN9vg2X",
        ] {
            XCTAssertTrue(near.validate(address: address))
            XCTAssertEqual(near.address(string: address)?.description, address)
        }
    }
}

class NEARSignerTests: XCTestCase {

    func testSigningTransaction() {
        // swiftlint:disable:next line_length
        let privateKey = PrivateKey(data: Base58.decodeNoCheck(string: "3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv")!.subdata(in: 0..<32))!

        let signingInput = NEARSigningInput.with {
            $0.signerID = "test.near"
            $0.nonce = 1
            $0.receiverID = "whatever.near"

            $0.actions = [
                NEARAction.with({
                    $0.transfer = NEARTransfer.with {
                        $0.deposit = Data(hexString: "01000000000000000000000000000000")!
                    }
                }),
            ]

            $0.blockHash = Base58.decodeNoCheck(string: "244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM")!
            $0.privateKey = privateKey.data
        }

        let output = NEARSigner.sign(input: signingInput)
        // swiftlint:disable:next line_length
        let expectedBase64Encoded = "CQAAAHRlc3QubmVhcgCRez0mjUtY9/7BsVC9aNab4+5dTMOYVeNBU4Rlu3eGDQEAAAAAAAAADQAAAHdoYXRldmVyLm5lYXIPpHP9JpAd8pa+atxMxN800EDvokNSJLaYaRDmMML+9gEAAAADAQAAAAAAAAAAAAAAAAAAAACWmoMzIYbul1Xkg5MlUlgG4Ymj0tK7S0dg6URD6X4cTyLe7vAFmo6XExAO2m4ZFE2n6KDvflObIHCLodjQIb0B"
        XCTAssertEqual(Data(base64Encoded: expectedBase64Encoded), output.signedTransaction)
    }
}
