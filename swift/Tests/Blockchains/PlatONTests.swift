// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class PlatONTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "03a4130e4abb887a296eb38c15bbd83253ab09492a505b10a54b008b7dcc1668")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .platON)
        let addressFromString = AnyAddress(string: "0xC115CEADF9e5923330e5f42903Fe7f926DDA65D2", coin: .platON)!

        XCTAssertEqual(pubkey.data.hexString, "04013f756ad4762f5109dba9e652ae43905730c882a12dedc17edca788f51dcb3b8a74ce86d8b6674fa167798d30f1149d9ab86dffcc976278966b5584450107a6")
        XCTAssertEqual(address.description, addressFromString.description)
    }
}
