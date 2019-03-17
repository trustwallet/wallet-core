// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class CosmosTests: XCTestCase {
    func testAddressFromPrivateKey() {
        let key = PrivateKey(data: Data(hexString: "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let address = TendermintAddress(hrp: .cosmos, publicKey: pubkey)

        XCTAssertEqual(pubkey.data.hexString, "0257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc5".lowercased())
        XCTAssertEqual(address?.description, "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02")
    }
}
