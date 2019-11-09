// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class OntologyAddressTests: XCTestCase {
    
    func testInvalid() {
        XCTAssertNil(OntologyAddress(string: "abc"))
        XCTAssertNil(OntologyAddress(string: "dshadghasdghsadadsadjsad"))
    }
    
    func testIsValid() {
        XCTAssertFalse(OntologyAddress.isValidString(string: "abc"))
        XCTAssertFalse(OntologyAddress.isValidString(string: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"))
        XCTAssertTrue(OntologyAddress.isValidString(string: "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD"))
    }
    
    func testFromPublicKey() {
        let publicKey = PublicKey(data: Data(hexString: "031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486")!, type: .nist256p1)!
        let address = OntologyAddress(publicKey: publicKey)
        XCTAssertEqual(address.description, "AeicEjZyiXKgUeSBbYQHxsU1X3V5Buori5")
    }
    
    func testFromPrivateKey() {
        let privateKey = PrivateKey(data: Data(hexString: "4cbd05e59cbe5faba43bbf5a15fdaf27ad72c232f8d88d987c6b3d4d98300af5")!)!
        let address = OntologyAddress(publicKey: privateKey.getPublicKeyNist256p1())
        XCTAssertEqual(address.description, "AH11LGtFk6VU9Z7suuM5eNpho1bAoE5Gbz")
    }
    
    func testDescription() {
        let address = OntologyAddress(string: "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD")!
        XCTAssertEqual(address.description, "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD")
    }
}
