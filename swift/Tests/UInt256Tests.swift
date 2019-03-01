// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class UInt256Tests: XCTestCase {
    func testCreateWithData() {
        let data = Data(bytes: Array(repeating: 0, count: 24) + [0x1b, 0xc1, 0x6d, 0x67, 0x4e, 0xc8, 0x00, 0x00])
        let number = UInt256(data: data)!

        XCTAssertEqual(number.format(decimals: 18), "2.0")
        XCTAssertEqual(Array(number.data), Array(data))
    }

    func testZeroData() {
        XCTAssertEqual(Array(UInt256.zero.data), Array(repeating: 0, count: 32))
    }

    func testIsZero() {
        XCTAssertTrue(UInt256.zero.isZero)
        XCTAssertFalse(UInt256.one.isZero)
    }

    func testFormatZeroOne() {
        XCTAssertEqual(UInt256.zero.format(decimals: 0), "0")
        XCTAssertEqual(UInt256.one.format(decimals: 0), "1")
        XCTAssertEqual(UInt256.zero.format(decimals: 1), "0.0")
        XCTAssertEqual(UInt256.one.format(decimals: 1), "0.1")
        XCTAssertEqual(UInt256.zero.format(decimals: 2), "0.0")
        XCTAssertEqual(UInt256.one.format(decimals: 2), "0.01")
    }

    func testFormatEndingInZero() {
        let x = UInt256(1234567890 as UInt64)
        XCTAssertEqual(x.format(decimals: 0), "1234567890")
        XCTAssertEqual(x.format(decimals: 1), "123456789.0")
        XCTAssertEqual(x.format(decimals: 5), "12345.6789")
        XCTAssertEqual(x.description, "1234567890")
    }

    func testFormatPad() {
        let x = UInt256(1234 as UInt64)
        XCTAssertEqual(x.format(decimals: 1), "123.4")
        XCTAssertEqual(x.format(decimals: 2), "12.34")
        XCTAssertEqual(x.format(decimals: 3), "1.234")
        XCTAssertEqual(x.format(decimals: 4), "0.1234")
        XCTAssertEqual(x.format(decimals: 5), "0.01234")
    }
}
