// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class AlgorandTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "a6c4394041e64fe93d889386d7922af1b9a87f12e433762759608e61434d6cf7")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AlgorandAddress(publicKey: pubkey)
        let addressFromString = AlgorandAddress(string: "ADIYK65L3XR5ODNNCUIQVEET455L56MRKJHRBX5GU4TZI2752QIWK4UL5A")!

        XCTAssertEqual(pubkey.data.hexString, "00d1857babdde3d70dad15110a9093e77abef991524f10dfa6a727946bfdd411")
        XCTAssertEqual(address.description, addressFromString.description)
    }
}
