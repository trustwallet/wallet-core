// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class Base64Tests: XCTestCase {
    func testEncode()  {
        let encoded = Base64.encode(data: Data.init(bytes: "HelloWorld", count: 10));
        XCTAssertEqual(encoded, "SGVsbG9Xb3JsZA==");
    }

    func testDecode()  {
        guard let decoded = Base64.decode(string: "SGVsbG9Xb3JsZA==") else {
            return XCTFail();
        }
        let toCompare = String(data: decoded, encoding:.utf8);

        XCTAssertEqual(toCompare, "HelloWorld");
    }
    
    func testUrlEncode() {
        let encoded = Base64.encodeUrl(data: Data.init(bytes: "+\\?ab", count: 5));
        XCTAssertEqual(encoded, "K1w_YWI=");
    }
    
    func testUrlDecode() {
        guard let decoded = Base64.decodeUrl(string: "K1w_YWI=") else {
            return XCTFail();
        }
        let toCompare = String(data: decoded, encoding:.utf8);

        XCTAssertEqual(toCompare, "+\\?ab");
    }
}
