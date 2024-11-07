// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class Bech32Tests: XCTestCase {
    func testEncode()  {
        let data = Data(hexString: "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f")!
        let encoded = Bech32.encode(hrp: "abcdef", data: data)
        XCTAssertEqual(encoded, "abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw")
    }

    func testDecode()  {
        guard let decoded = Bech32.decode(string: "abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw") else {
            return XCTFail()
        }

        XCTAssertEqual(decoded.hexString, "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f")
    }
}
