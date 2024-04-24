// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class BinanceSmartChainTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "727f677b390c151caf9c206fd77f77918f56904b5504243db9b21e51182c4c06")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .smartChain)
        let expected = AnyAddress(string: "0xf3d468DBb386aaD46E92FF222adDdf872C8CC064", coin: .smartChain)!

        XCTAssertEqual(address.description, expected.description)
    }
}
