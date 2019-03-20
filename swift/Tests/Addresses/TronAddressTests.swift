// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
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

        XCTAssertEqual("THRF3GuPnvvPzKoaT8pJex5XHmo8NNbCb3", TronAddress(publicKey: publicKey).description)
    }

    func testInvalid() {
        XCTAssertNil(TronAddress(string: "abc"))
        XCTAssertNil(TronAddress(string: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"))
        XCTAssertNil(TronAddress(string: "175tWpb8K1S7NmH4Zx6rewF9WQrcZv245W"))
    }

    func testInitWithString() {
        let address = TronAddress(string: "TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC")

        XCTAssertNotNil(address)
        XCTAssertEqual(address!.description, "TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC")
    }
}
