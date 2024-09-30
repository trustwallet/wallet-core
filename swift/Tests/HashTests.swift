// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class HashTests: XCTestCase {

    func testSha512_256() {
        let message = "hello".data(using: .utf8)!
        let hashed = Hash.sha512_256(data: message)

        XCTAssertEqual(hashed.hexString, "e30d87cfa2a75db545eac4d61baf970366a8357c7f72fa95b52d0accb698f13a")
    }
}
