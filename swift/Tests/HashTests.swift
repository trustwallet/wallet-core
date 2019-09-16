// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class HashTests: XCTestCase {

    func testSha512_256() {
        let message = "hello".data(using: .utf8)!
        let hashed = Hash.sha512_256(data: message)

        XCTAssertEqual(hashed.hexString, "e30d87cfa2a75db545eac4d61baf970366a8357c7f72fa95b52d0accb698f13a")
    }
}
