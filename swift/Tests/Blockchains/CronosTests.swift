// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class CronosTests: XCTestCase {

    func testAddress() {
        let address = AnyAddress(string: "0xEC49280228b0D05Aa8e8b756503254e1eE7835ab", coin: .cronosChain)!

        XCTAssertEqual(address.data.hexString, "ec49280228b0d05aa8e8b756503254e1ee7835ab")
    }
}
