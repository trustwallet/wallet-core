// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class CoinTypeTests: XCTestCase {

    func testValidate() {
        XCTAssertTrue(CoinType.cosmos.validate(address: "cosmos1xsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02"))
        XCTAssertTrue(CoinType.cosmos.validate(address: "cosmospub1xddwnpepqftjsmkr7d7nx4tmhw4qqze8w39vjq364xt8etn45xqarlu3l2wu2n7pgrq"))
        XCTAssertTrue(CoinType.cosmos.validate(address: "cosmosvaloper1xxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08"))
        XCTAssertTrue(CoinType.cosmos.validate(address: "cosmosvalconspub1xcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa"))
    }
}
