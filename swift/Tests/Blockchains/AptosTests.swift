// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class AptosTests: XCTestCase {
    func testAddress() {
        let anyAddress = AnyAddress(string: "6af7d07b8a541913dfa87a9f99628faa255c70241ef9ebd9b82a7e715ee13108", coin: .aptos)

        XCTAssertEqual(anyAddress?.description, "6af7d07b8a541913dfa87a9f99628faa255c70241ef9ebd9b82a7e715ee13108")
        XCTAssertEqual(anyAddress?.coin, .aptos)

        let invalid = "MQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4"
        XCTAssertNil(Data(hexString: invalid))
        XCTAssertNil(AnyAddress(string: invalid, coin: .aptos))
        XCTAssertFalse(AnyAddress.isValid(string: invalid, coin: .aptos))
    }

    func testSign() {
        // TODO: Create implementation
    }
}
