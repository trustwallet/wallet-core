// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class WanchainAddressTests: XCTestCase {

    func testDescription() {
        let address = WanchainAddress(string: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed")!
        XCTAssertEqual(address.description, "0x5aaeb6053f3e94c9b9a09f33669435e7ef1beaed")
    }
}
