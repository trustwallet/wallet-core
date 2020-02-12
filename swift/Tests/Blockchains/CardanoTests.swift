// Copyright © 2017-2020 Trust Wallet.
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
        let address = AnyAddress(publicKey: pubkey, coin: .cardano)
        let addressFromString = AnyAddress(string: "addr1s3tl64970vuthz2j0qkz7kd2ya5j3fxuhdnv333vu38e6c37e4dq80ek4raf7hs3adag2tzpuxz7895a2x8xde5f8jqa8lrjyuqfj5k50pm668", coin: .cardano)!

        XCTAssertEqual(pubkey.data.hexString, "57fd54be7b38bb8952782c2f59aa276928a4dcbb66c8c62ce44f9d623ecd5a03bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4")
        XCTAssertEqual(address.description, addressFromString.description)
    }
}
