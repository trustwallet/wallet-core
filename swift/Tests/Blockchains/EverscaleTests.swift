// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class EverscaleTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .everscale)
        let addressFromString = AnyAddress(string: "0:269fee242eb410786abe1777a14785c8bbeb1e34100c7570e17698b36ad66fb0", coin: .everscale)!

        XCTAssertEqual(pubkey.data.hexString, "e4925f9932df8d7fd0042efff3e2178a972028b644ded3a3b66f6d0577f82e78")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        // TODO: Create implementation
    }
}
