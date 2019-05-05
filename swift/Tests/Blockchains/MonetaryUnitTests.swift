// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class MonetaryUnitTests: XCTestCase {
    let monetaryunit = CoinType.monetaryUnit

    func testValidAddresses() {
        XCTAssertTrue(monetaryunit.validate(address: "7UQUGq9UQD57R5Kn6hHoYrpaRjDdWtZWF7"))
        XCTAssertTrue(monetaryunit.validate(address: "7oBSZsdN6jDoEn2agojLuUqoZbjWBbcwCm"))
    }

    func testInvalidAddresses() {
        for addr in [
            "7oBSZsdN6jDoEn2agojLuUqoZbjWBbcwCms",
            "7oBSZsdN6jDoEn2XgojLuUqoZbjWBbcwCm",
            "xoBSZsdN6jDoEn2agojLuUqoZbjWBbcwCm",
            "1oBSZsdN6jDoEn2agojLuUqoZbjWBbcwCm",
            "ToBSZsdN6jDoEn2agojLuUqoZbjWBbcwCm",
            ] {
            XCTAssertFalse(monetaryunit.validate(address: addr))
        }
    }
}
