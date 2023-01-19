// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class SuiTests: XCTestCase {
    func testAddress() {
        let anyAddress = AnyAddress(string: "0x061ce2b2100a71bb7aa0da98998887ad82597948", coin: .sui)

        XCTAssertEqual(anyAddress?.description, "0x061ce2b2100a71bb7aa0da98998887ad82597948")
        XCTAssertEqual(anyAddress?.coin, .sui)

        let invalid = "MQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4"
        XCTAssertNil(Data(hexString: invalid))
        XCTAssertNil(AnyAddress(string: invalid, coin: .sui))
        XCTAssertFalse(AnyAddress.isValid(string: invalid, coin: .sui))
    }

    func testSign() {
        // TODO: Create implementation
    }
}
