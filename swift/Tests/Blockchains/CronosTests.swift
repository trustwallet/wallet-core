// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class CronosTests: XCTestCase {

    func testAddress() {
        let address = AnyAddress(string: "0xEC49280228b0D05Aa8e8b756503254e1eE7835ab", coin: .cronosChain)!

        XCTAssertEqual(address.data.hexString, "ec49280228b0d05aa8e8b756503254e1ee7835ab")
    }
}
