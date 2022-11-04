// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class SmartBitcoinCashTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "155cbd57319f3d938977b4c18000473eb3c432c4e31b667b63e88559c497d82d")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .smartBitcoinCash)
        let addressFromString = AnyAddress(string: "0x8bFC9477684987dcAf0970b9bce5E3D9267C99C0", coin: .smartBitcoinCash)!

        XCTAssertEqual(pubkey.data.hexString, "046439f94100c802691c53ef18523be2c24d301f0e2bd3b425e832378a5405eff4331d5e57303785969073321fc76a8504a3854bdb21e6ab7b268a1737882a29c0")
        XCTAssertEqual(address.description, addressFromString.description)
    }

}
