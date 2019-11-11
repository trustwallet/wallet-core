// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class SampleCoinTests: XCTestCase {
    // TODO: Check and finalize implementation

    func testAddress() {
        // TODO: Check and finalize implementation

        let key = PrivateKey(data: Data(hexString: "__PRIVATE_KEY_DATA__")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = SampleCoinAddress(publicKey: pubkey)
        let addressFromString = SampleCoinAddress(string: "__ADDRESS_DATA__")!

        XCTAssertEqual(pubkey.data.hexString, "__EXPECTED_PUBKEY_DATA__")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        // TODO: Create implementation
    }
}
