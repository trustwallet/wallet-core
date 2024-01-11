// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class BinanceAddressTests: XCTestCase {
    func testIsValid() {
        XCTAssertTrue(AnyAddress.isValid(string: "bnb12vtaxl9952zm6rwf7v8jerq74pvaf77fcmvzhw", coin: .binance))

        XCTAssertFalse(AnyAddress.isValid(string: "bad1devga6q804tx9fqrnx0vtu5r36kxgp9tqx8h9k", coin: .binance))
        XCTAssertFalse(AnyAddress.isValid(string: "tbnb1devga6q804tx9fqrnx0vtu5r36kxgp9t4ruzk2", coin: .binance))
    }

    func testIsValidBech32() {
        XCTAssertTrue(AnyAddress.isValidBech32(string: "bnb12vtaxl9952zm6rwf7v8jerq74pvaf77fcmvzhw", coin: .binance, hrp: "bnb"));
        XCTAssertTrue(AnyAddress.isValidBech32(string: "tbnb1devga6q804tx9fqrnx0vtu5r36kxgp9t4ruzk2", coin: .binance, hrp: "tbnb"));
        
        XCTAssertFalse(AnyAddress.isValidBech32(string: "bnb12vtaxl9952zm6rwf7v8jerq74pvaf77fcmvzhw", coin: .binance, hrp: "tbnb"));
        XCTAssertFalse(AnyAddress.isValidBech32(string: "tbnb1devga6q804tx9fqrnx0vtu5r36kxgp9t4ruzk2", coin: .binance, hrp: "bnb"));
    }
}
