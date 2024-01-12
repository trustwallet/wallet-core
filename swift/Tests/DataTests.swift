// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class DataTests: XCTestCase {
    func testCreateWithBytes() {
        let bytes = [0x1, 0x2, 0x3, 0x4] as [UInt8]
        let twdata = bytes.withUnsafeBufferPointer { TWDataCreateWithBytes($0.baseAddress!, 4) }
        let data = TWDataNSData(twdata)

        XCTAssertEqual(Array(data), bytes)
    }

    func testOddLength() {
        XCTAssertNil(Data(hexString: "0x0"))
        XCTAssertNil(Data(hexString: "0x28fa6ae00"))
        XCTAssertNil(Data(hexString: "28fa6ae00"))
    }
    
    func testOddCharacters() {
        XCTAssertNotNil(Data(hexString: String(repeating: "a", count: 64)))
        XCTAssertNil(Data(hexString: String(repeating: "a", count: 63) + "z"))
    }
}
