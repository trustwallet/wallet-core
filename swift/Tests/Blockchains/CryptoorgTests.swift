// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class Crypto.orgTests: XCTestCase {
    // TODO: Check and finalize implementation

    func testAddress() {
        // TODO: Check and finalize implementation

        let key = PrivateKey(data: Data(hexString: "7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .crypto.org)
        let addressFromString = AnyAddress(string: "__ADDRESS_DATA__", coin: .crypto.org)!

        XCTAssertEqual(pubkey.data.hexString, "cro1z53wwe7md6cewz9sqwqzn0aavpaun0gw39h3rd")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        // TODO: Create implementation
    }
}
