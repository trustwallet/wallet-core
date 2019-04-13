//
//  ConfigurationTests.swift
//  TrustWalletCoreTests
//
//  Created by Viktor on 4/13/19.
//

import XCTest
import TrustWalletCore

class ConfigurationTests: XCTestCase {

    func testNodeSource() {
        XCTAssertEqual("https://github.com/ethereum/go-ethereum", CoinTypeConfiguration.getNodeSource(type: .ethereum))
    }
}
