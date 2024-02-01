// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class DydxTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let address = AnyAddress(publicKey: pubkey, coin: .dydx)
        let addressFromString = AnyAddress(string: "dydx1mry47pkga5tdswtluy0m8teslpalkdq0hc72uz", coin: .dydx)!

        XCTAssertEqual(address.description, addressFromString.description)
    }
}
