// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class CoinTypeConfigurationTests: XCTestCase {
    func testNativeTokenNameDiffersFromNameForSmartChain() {
        let name = CoinTypeConfiguration.getName(type: .smartChain)
        let nativeTokenName = CoinTypeConfiguration.getNativeTokenName(type: .smartChain)

        XCTAssertEqual(name, "BNB Smart Chain")
        XCTAssertEqual(nativeTokenName, "BNB")
        XCTAssertNotEqual(name, nativeTokenName)
    }

    func testNativeTokenNameMatchesNameForBitcoin() {
        let name = CoinTypeConfiguration.getName(type: .bitcoin)
        let nativeTokenName = CoinTypeConfiguration.getNativeTokenName(type: .bitcoin)

        XCTAssertEqual(name, nativeTokenName)
    }
}
