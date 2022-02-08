// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class ZcoinTests: XCTestCase {
    let coin = CoinType.firo

    func testValidAddresses() {
        XCTAssertTrue(coin.validate(address: "a4YtT82mWWxHZhLmdx7e5aroW92dqJoRs3"))
        XCTAssertTrue(coin.validate(address: "4CFa4fnAQvFz4VpikGNzQ9XfCDXMmdk6sh"))
    }

    func testInvalidAddresses() {
        for addr in [
            "bc1qvtvte5tzlqlfhcdmph94lxk8jcz54q6psyvgla",
            "18eqGohuqvrZLL3LMR4Wv81qvKeDHsGpjH",
            "LV7LV7Z4bWDEjYkfx9dQo6k6RjGbXsg6hS",
            "Xm1iDLBP5tdxTxc6t7uJBCVjC4L2A5vB2J",
            "TKjdnbJxP4yHeLTHZ86DGnFFY6QhTjuBv2",
            ] {
            XCTAssertFalse(coin.validate(address: addr))
        }
    }
}
