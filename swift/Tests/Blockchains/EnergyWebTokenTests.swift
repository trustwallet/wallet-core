// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class EnergyWebTokenTests: XCTestCase {

    func testAddress() {

        let key = PrivateKey(data: Data(hexString: "d429aff03fce610ca3530f08158a96c77f684a1eae39eab5e78b28c46fe4d92c")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .energyWebToken)
        let expected = AnyAddress(string: "0x0C6365602b334158C32b1fa6EEE7A210304960EA", coin: .energyWebToken)!

        XCTAssertEqual(address.description, expected.description)
    }

}
