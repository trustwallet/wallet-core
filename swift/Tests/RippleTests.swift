// Copyright © 2017-2019 Binance.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class RippleTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "9c3d42d0515f0406ed350ab2abf3eaf761f8907802469b64052ac17e2250ae13")!)!
        let pubkey = key.getPublicKey(compressed: true)
        let address = RippleAddress(publicKey: pubkey)

        XCTAssertEqual(address?.description, "r36yxStAh7qgTQNHTzjZvXybCTzUFhrfav")
    }
}
