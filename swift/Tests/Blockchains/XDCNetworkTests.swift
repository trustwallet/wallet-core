// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
import TrustWalletCore
import XCTest

class XinFinTests: XCTestCase {
    // TODO: Check and finalize implementation
    func testAddress() {
        // TODO: Check and finalize implementation
        let key = PrivateKey(data: Data(hexString: "861932c871f8a5181cfaa54ba6dbbd9f7fb9d41925021246c10c0d28f3b661c0")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .xinfin)
        let addressFromString = AnyAddress(string: "xdccf808d69db8ddc8d5fc90bf2586074f328058082", coin: .xinfin)!

        XCTAssertEqual(pubkey.data.hexString, "xdccf808d69db8ddc8d5fc90bf2586074f328058082")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        // TODO: Create implementation
    }
}