// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class NEOAddressTests: XCTestCase {

    func testInvalid() {
        XCTAssertNil(AnyAddress(string: "abc", coin: .neo))
        XCTAssertNil(AnyAddress(string: "dshadghasdghsadadsadjsad", coin: .neo))
    }

    func testIsValid() {
        XCTAssertFalse(AnyAddress.isValid(string: "abc", coin: .neo))
        XCTAssertFalse(AnyAddress.isValid(string: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed", coin: .neo))
        XCTAssertTrue(AnyAddress.isValid(string: "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD", coin: .neo))
    }

    func testFromPublicKey() {
        let publicKey = PublicKey(data: Data(hexString: "031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486")!, type: .nist256p1)!
        let address = AnyAddress(publicKey: publicKey, coin: .neo)
        XCTAssertEqual(address.description, "AeicEjZyiXKgUeSBbYQHxsU1X3V5Buori5")
    }

    func testFromPrivateKey() {
        let privateKey = PrivateKey(data: Data(hexString: "4cbd05e59cbe5faba43bbf5a15fdaf27ad72c232f8d88d987c6b3d4d98300af5")!)!
        let address = AnyAddress(publicKey: privateKey.getPublicKeyNist256p1(), coin: .neo)
        XCTAssertEqual(address.description, "AH11LGtFk6VU9Z7suuM5eNpho1bAoE5Gbz")
    }

    func testDescription() {
        let address = AnyAddress(string: "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD", coin: .neo)!
        XCTAssertEqual(address.description, "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD")
    }
}
