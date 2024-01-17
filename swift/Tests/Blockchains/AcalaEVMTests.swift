// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class AcalaEVMTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "828c4c48c2cef521f0251920891ed79e871faa24f64f43cde83d07bc99f8dbf0")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .acalaEVM)
        let expected = AnyAddress(string: "0xe32DC46bfBF78D1eada7b0a68C96903e01418D64", coin: .acalaEVM)!

        XCTAssertEqual(address.description, expected.description)
    }
}
