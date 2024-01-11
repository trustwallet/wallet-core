// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class OntologyAddressTests: XCTestCase {

    func testInvalid() {
        XCTAssertNil(AnyAddress(string: "abc", coin: .ontology))
        XCTAssertNil(AnyAddress(string: "dshadghasdghsadadsadjsad", coin: .ontology))
    }

    func testIsValid() {
        XCTAssertFalse(AnyAddress.isValid(string: "abc", coin: .ontology))
        XCTAssertFalse(AnyAddress.isValid(string: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed", coin: .ontology))
        XCTAssertTrue(AnyAddress.isValid(string: "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD", coin: .ontology))
    }

    func testFromPublicKey() {
        let publicKey = PublicKey(data: Data(hexString: "031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486")!, type: .nist256p1)!
        let address = AnyAddress(publicKey: publicKey, coin: .ontology)
        XCTAssertEqual(address.description, "AeicEjZyiXKgUeSBbYQHxsU1X3V5Buori5")
    }

    func testFromPrivateKey() {
        let privateKey = PrivateKey(data: Data(hexString: "4cbd05e59cbe5faba43bbf5a15fdaf27ad72c232f8d88d987c6b3d4d98300af5")!)!
        let address = AnyAddress(publicKey: privateKey.getPublicKeyNist256p1(), coin: .ontology)
        XCTAssertEqual(address.description, "AH11LGtFk6VU9Z7suuM5eNpho1bAoE5Gbz")
    }

    func testDescription() {
        let address = AnyAddress(string: "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD", coin: .ontology)!
        XCTAssertEqual(address.description, "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD")
    }
}
