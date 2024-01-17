// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class TronAddressTests: XCTestCase {

    func testFromPrivateKey() {
        let privateKey =  PrivateKey(data: Data(hexString: "2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54")!)!

        let address = CoinType.tron.deriveAddress(privateKey: privateKey)

        XCTAssertEqual(address.description, "TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC")
    }

    func testFromPublicKey() {
        let privateKey = PrivateKey(data: Data(hexString: "BE88DF1D0BF30A923CB39C3BB953178BAAF3726E8D3CE81E7C8462E046E0D835")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)

        XCTAssertEqual("THRF3GuPnvvPzKoaT8pJex5XHmo8NNbCb3", AnyAddress(publicKey: publicKey, coin: .tron).description)
    }

    func testInvalid() {
        XCTAssertNil(AnyAddress(string: "abc", coin: .tron))
        XCTAssertNil(AnyAddress(string: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed", coin: .tron))
        XCTAssertNil(AnyAddress(string: "175tWpb8K1S7NmH4Zx6rewF9WQrcZv245W", coin: .tron))
    }

    func testInitWithString() {
        let address = AnyAddress(string: "TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC", coin: .tron)

        XCTAssertNotNil(address)
        XCTAssertEqual(address!.description, "TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC")
    }
}
