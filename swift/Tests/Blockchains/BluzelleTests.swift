// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class BluzelleTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "1037f828ca313f4c9e120316e8e9ff25e17f07fe66ba557d5bc5e2eeb7cba8f6")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .bluzelle)
        let addressFromString = AnyAddress(string: "bluzelle1yhtq5zm293m2r3sp2guj9m5pg5e273n6r0szul", coin: .bluzelle)!

        XCTAssertEqual(pubkey.data.hexString, "0x381207e7f7fa7c44564534b0f47dfef3fc942e42defc10b2bf614e4dfa773e5a")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        // TODO: Create implementation
    }
}
