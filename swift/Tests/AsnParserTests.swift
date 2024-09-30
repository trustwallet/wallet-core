// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class AsnParserTests: XCTestCase {
    func testEcdsaSignatureFromDer()  {
        let encoded = Data(hexString: "3046022100db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495da022100ff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1")!
        let expected = Data(hexString: "db421231f23d0320dbb8f1284b600cd34b8e9218628139539ff4f1f6c05495daff715aab70d5317dbf8ee224eb18bec3120cfb9db1000dbb31eadaf96c71c1b1")!
        let actual = AsnParser.ecdsaSignatureFromDer(encoded: encoded)
        XCTAssertEqual(actual, expected);
    }
}
