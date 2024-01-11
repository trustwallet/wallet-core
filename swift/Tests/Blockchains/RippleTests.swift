// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class RippleTests: XCTestCase {

    func testAddressValidation() {
        let coin = CoinType.xrp
        let string = "XVfvixWZQKkcenFRYApCjpTUyJ4BePTe3jJv7beatUZvQYh"
        let xaddr = RippleXAddress(string: string)

        XCTAssertTrue(coin.validate(address: "rDpysuumkweqeC7XdNgYNtzL5GxbdsmrtF"))
        XCTAssertTrue(coin.validate(address: "XVfvixWZQKkcenFRYApCjpTUyJ4BePTe3jJv7beatUZvQYh"))
        XCTAssertEqual(xaddr?.description, string)
        XCTAssertEqual(xaddr?.tag, 12345)
    }

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "9c3d42d0515f0406ed350ab2abf3eaf761f8907802469b64052ac17e2250ae13")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let address = AnyAddress(publicKey: pubkey, coin: .xrp)

        XCTAssertEqual(address.description, "r36yxStAh7qgTQNHTzjZvXybCTzUFhrfav")
    }

    func testSigner() {
        let operation = RippleOperationPayment.with {
            $0.destination = "rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF"
            $0.amount = 10
        }
        let input = RippleSigningInput.with {
            $0.fee = 10
            $0.sequence = 32268248 // from account info api
            $0.lastLedgerSequence = 32268269
            $0.account = "rfxdLwsZnoespnTDDb1Xhvbc8EFNdztaoq"
            $0.privateKey = Data(hexString: "a5576c0f63da10e584568c8d134569ff44017b0a249eb70657127ae04f38cc77")!
            $0.opPayment = operation
        }

        let output: RippleSigningOutput = AnySigner.sign(input: input, coin: .xrp)
        XCTAssertEqual(output.encoded.hexString, "12000022000000002401ec5fd8201b01ec5fed61400000000000000a68400000000000000a732103d13e1152965a51a4a9fd9a8b4ea3dd82a4eba6b25fcad5f460a2342bb650333f74463044022037d32835c9394f39b2cfd4eaf5b0a80e0db397ace06630fa2b099ff73e425dbc02205288f780330b7a88a1980fa83c647b5908502ad7de9a44500c08f0750b0d9e8481144c55f5a78067206507580be7bb2686c8460adff983148132e4e20aecf29090ac428a9c43f230a829220d")

        let operation2 = RippleOperationPayment.with {
            $0.destination = "XVfvixWZQKkcenFRYApCjpTUyJ4BePMjMaPqnob9QVPiVJV"
            $0.amount = 10
        }
        let input2 = RippleSigningInput.with {
            $0.fee = 10
            $0.sequence = 32268248 // from account info api
            $0.lastLedgerSequence = 32268269
            $0.account = "rfxdLwsZnoespnTDDb1Xhvbc8EFNdztaoq"
            $0.privateKey = Data(hexString: "a5576c0f63da10e584568c8d134569ff44017b0a249eb70657127ae04f38cc77")!
            $0.opPayment = operation2
        }
        let output2: RippleSigningOutput = AnySigner.sign(input: input2, coin: .xrp)
        XCTAssertEqual(output2.encoded, output.encoded)
    }
}
