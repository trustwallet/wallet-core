// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class FIOTests: XCTestCase {

    func testAddressFromString() {
        let valid = "FIO8KkL3ne7pv9evoNwFMYuCFZ4xRQKY2kgGN8N9PeRnEUc42itKK"
        let invalid = "EOS7y9UXLsC4jYU3k3NFweZZPWgRpRtvxBQj5TC3cnDVPsgyKfmW1"
        let address = AnyAddress(string: valid, coin: .fio)

        XCTAssertNotNil(address)
        XCTAssertEqual(address?.description, valid)
        XCTAssertTrue(AnyAddress.isValidString(string: valid, coin: .fio))

        XCTAssertNil(AnyAddress(string: invalid, coin: .fio))
        XCTAssertFalse(AnyAddress.isValidString(string: invalid, coin: .fio))
    }

    func testAddressFromKey() {
        let key = PrivateKey(data: Data(hexString: "ea8eb60b7e5868e218f248e032769020b4fea5dcfd02f2992861eaf4fb534854")!)!

        let address = AnyAddress(publicKey: key.getPublicKeySecp256k1(compressed: true), coin: .fio)
        XCTAssertEqual(address.description, "FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o")
    }
}
