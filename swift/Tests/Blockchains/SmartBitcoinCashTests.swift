// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class SmartBitcoinCashTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "ab4accc9310d90a61fc354d8f353bca4a2b3c0590685d3eb82d0216af3badddc")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .smartBitcoinCash)
        let addressFromString = AnyAddress(string: "0xA3Dcd899C0f3832DFDFed9479a9d828c6A4EB2A7", coin: .smartBitcoinCash)!

        XCTAssertEqual(pubkey.data.hexString, "0448a9ffac8022f1c7eb5253746e24d11d9b6b2737c0aecd48335feabb95a179916b1f3a97bed6740a85a2d11c663d38566acfb08af48a47ce0c835c65c9b23d0d")
        XCTAssertEqual(address.description, addressFromString.description)
    }

}
