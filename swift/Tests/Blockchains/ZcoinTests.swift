// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class ZcoinTests: XCTestCase {
    let zcoin = Zcoin()

    func testWIF() {
        let wif = "YAWpCGY26oVprEs8gqEAcBmJYZKU9PxbjE39CwumegK248fPn5To"
        let privKey = PrivateKeySecp256k1(wif: wif)!
        let pubKey = privKey.getPublicKey(compressed: true)

        XCTAssertEqual(pubKey.data.hexString, "025bb27ffa8dde90ea269e591fcf93e01e58a31077d76c92924cb42c7c7ea39090")
    }

    func testValidAddresses() {
        XCTAssertTrue(zcoin.validate(address: "a4YtT82mWWxHZhLmdx7e5aroW92dqJoRs3"))
        XCTAssertTrue(zcoin.validate(address: "4CFa4fnAQvFz4VpikGNzQ9XfCDXMmdk6sh"))
    }

    func testInvalidAddresses() {
        for addr in [
            "bc1qvtvte5tzlqlfhcdmph94lxk8jcz54q6psyvgla",
            "18eqGohuqvrZLL3LMR4Wv81qvKeDHsGpjH",
            "LV7LV7Z4bWDEjYkfx9dQo6k6RjGbXsg6hS",
            "Xm1iDLBP5tdxTxc6t7uJBCVjC4L2A5vB2J",
            "TKjdnbJxP4yHeLTHZ86DGnFFY6QhTjuBv2",
            ] {
                XCTAssertFalse(zcoin.validate(address: addr))
        }
    }
}
