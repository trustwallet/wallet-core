// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class Bech32mTests: XCTestCase {
    func testEncode()  {
        let data = Data(hexString: "1f1e1d1c1b1a191817161514131211100f0e0d0c0b0a09080706050403020100")!
        let encoded = Bech32m.encode(hrp: "abcdef", data: data)
        XCTAssertEqual(encoded, "abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx")
    }

    func testDecode()  {
        guard let decoded = Bech32m.decode(string: "abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx") else {
            return XCTFail()
        }

        XCTAssertEqual(decoded.hexString, "1f1e1d1c1b1a191817161514131211100f0e0d0c0b0a09080706050403020100")
    }
}
