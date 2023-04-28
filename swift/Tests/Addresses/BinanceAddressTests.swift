// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
