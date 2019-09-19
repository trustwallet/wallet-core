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

    func testEncode() {
        XCTAssertTrue(AlgorandAddress.isValidString(string: "ZPKWEGSSSUHYEZNU5REKXN6WYIFEOQ42WQY2DAJA4QO4GJ4LJDO5TEF5HA"))
        print("amt".data(using: .utf8)!.hexString)
        let data = Data(hexString: "82a3736967c440a764385b86da330b373750eeeaafd78e0e1525c534569d17f5d47d44b57f1b14a1b4a0c3239467c344b8a11723bb478b33ce1dfc65e541adc4248a6d58e5f309a374786e88a3616d74cd03e8a3666565cd03e8a26676ce001df153a26768c420c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adfa26c76ce001df53ba3726376c4203dd160d60673bd9b13adc25dad5d988d0d9f4ccdbe95a2122f9ef28b3ce4e896a3736e64c420cbd5621a52950f8265b4ec48abb7d6c20a47439ab431a18120e41dc3278b48dda474797065a3706179")!
        let base64 = data.base64EncodedString()
        print(base64)
    }
}
