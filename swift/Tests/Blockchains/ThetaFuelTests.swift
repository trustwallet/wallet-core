// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class ThetaFuelTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "4646464646464646464646464646464646464646464646464646464646464646")!)!
                let pubkey = key.getPublicKeySecp256k1(compressed: false)
                let address = AnyAddress(publicKey: pubkey, coin: .thetaFuel)
                let expected = AnyAddress(string: "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F", coin: .thetaFuel)!
        
                XCTAssertEqual(address.description, expected.description)
    }
}
