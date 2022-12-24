// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class AgoricTests: XCTestCase {
    // TODO: Check and finalize implementation

    func testAddress() {
        // TODO: Check and finalize implementation

        let key = PrivateKey(data: Data(hexString: "037048190544fa57651452f477c096de4f3073e7835cf3845b04602563a73f73")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let address = AnyAddress(publicKey: pubkey, coin: .agoric)
        let addressFromString = AnyAddress(string: "agoric18zvvgk6j3eq5wd7mqxccgt20gz2w94cy88aek5", coin: .agoric)!

        XCTAssertEqual(pubkey.data.hexString, "0x03df9a5e4089f89d45913fb2b856de984c7e8bf1344cc6444cc9705899a48c939d")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        // TODO: Create implementation
    }
}
