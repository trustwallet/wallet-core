// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class WanchainTests: XCTestCase {

    func testAddress() {
        let address = AnyAddress(string: "0xb08f432a3346e90e2ab61830ec227043131f70ff", coin: .wanchain)
        XCTAssertEqual(address?.description, "0xb08f432A3346E90e2AB61830Ec227043131f70Ff")
    }
}
