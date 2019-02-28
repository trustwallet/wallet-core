// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class WanchainTests: XCTestCase {

    func testAddressForString() {
        let blockchain = Wanchain()

        XCTAssertEqual(
            blockchain.address(string: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed")?.description,
            EthereumAddress(string: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed")?.description
        )
    }
}
