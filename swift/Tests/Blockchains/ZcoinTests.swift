// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class ZcoinTests: XCTestCase {
    let zcoin = CoinType.zcoin

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
