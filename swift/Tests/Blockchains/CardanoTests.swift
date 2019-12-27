// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class CardanoTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71effbf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4")!)!
        let pubkey = key.getPublicKeyEd25519Extended()
        let address = CardanoAddress(publicKey: pubkey)
        let addressFromString = CardanoAddress(string: "Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W")!

        XCTAssertEqual(pubkey.data.hexString, "57fd54be7b38bb8952782c2f59aa276928a4dcbb66c8c62ce44f9d623ecd5a03bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    /*
    func testSign() {
    }
    */
}
