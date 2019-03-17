// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class CosmosTests: XCTestCase {
    func testAddressFromPrivateKey() {
        let key = PrivateKey(data: Data(hexString: "124e69c2c2dacc76600f806a31333c100b41b1d4374e99f539e41156c2792c0c")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        //let address = TendermintAddress(hrp: .cosmos, publicKey: pubkey)

        XCTAssertEqual(pubkey.data.hexString, "03F7707B1E983FE6231D46E586DCCAB76501302481C9656859C830A42A2C1BD414".lowercased())
        //XCTAssertEqual(address?.description, "cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573")
    }
}
