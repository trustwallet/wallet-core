// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class TONTests: XCTestCase {
    func testAddress() {
        let pubkey = PublicKey(data: Data(hexString: "F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3")!, type: PublicKeyType.ed25519)!
        let address = TONAddress(publicKey: pubkey)
        let addressFromString = TONAddress(string: "EQAkAJCrZkWb9uYePf1D97nB8efUvYHTsqSscyPMGpcHUx3Y")!

        XCTAssertEqual(pubkey.data.hexString, "f61cf0bc8e891ad7636e0cd35229d579323aa2da827eb85d8071407464dc2fa3")
        XCTAssertEqual(address.description, addressFromString.description)
    }
}
