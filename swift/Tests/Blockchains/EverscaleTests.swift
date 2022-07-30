// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class EverscaleTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "15d126cb1a84acdbcd1d9c3f6975968c2beb18cc43c95849d4b0226e1c8552aa")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .everscale)
        let addressFromString = AnyAddress(string: "0:0c39661089f86ec5926ea7d4ee4223d634ba4ed6dcc2e80c7b6a8e6d59f79b04", coin: .everscale)!

        XCTAssertEqual(pubkey.data.hexString, "a0303f8fc89a3c2124f5dc6f3ab9a9cb246b7d1e24897eaf5e63eeee20085db0")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        // TODO: Create implementation
    }
}
