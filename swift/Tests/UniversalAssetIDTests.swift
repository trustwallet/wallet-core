// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

public extension UniversalAssetID {
    init(coin: CoinType) {
        self.init(coin: coin, token: "")
    }
}

class UniversalAssetIDTests: XCTestCase {

    func testDescription() {
        let btc = UniversalAssetID(coin: .bitcoin)
        let eth = UniversalAssetID(coin: .ethereum)
        let dai = UniversalAssetID(coin: .ethereum, token: "0x6B175474E89094C44Da98b954EedeAC495271d0F")
        let trx = UniversalAssetID(coin: .tron, token: "TR7NHqjeKQxGTCi8q8ZY4pL8otSzgjLj6t")
        let busd = UniversalAssetID(coin: .binance, token: "BUSD-BD1")

        XCTAssertEqual(btc.description, "c0")
        XCTAssertEqual(eth.description, "c60")
        XCTAssertEqual(dai.description, "c60_t0x6B175474E89094C44Da98b954EedeAC495271d0F")
        XCTAssertEqual(trx.description, "c195_tTR7NHqjeKQxGTCi8q8ZY4pL8otSzgjLj6t")
        XCTAssertEqual(busd.description, "c714_tBUSD-BD1")
    }

    func testEquatable() {

        XCTAssertEqual(
            UniversalAssetID(coin: .ethereum),
            UniversalAssetID(string: "c60")
        )

        XCTAssertEqual(
            UniversalAssetID(coin: .ethereum, token: "0x1"),
            UniversalAssetID(coin: .ethereum, token: "0x1")
        )

        XCTAssertNotEqual(
            UniversalAssetID(coin: .ethereum),
            UniversalAssetID(coin: .bitcoin)
        )

        XCTAssertNotEqual(
            UniversalAssetID(coin: .ethereum, token: "123"),
            UniversalAssetID(coin: .ethereum, token: "")
        )
    }

    func testParsingString() throws {
        for string in [
            "c0",
            "c60",
            "c714",
            "c60_t0x6b175474e89094c44da98b954eedeac495271d0f",
            "c714_tTWT-8C2"
        ] {
            XCTAssertNotNil(UniversalAssetID(string: string))
        }

        for string in [
            "",
            "60",
            "c1a",
            "c60_0xasdf"
        ] {
            XCTAssertNil(UniversalAssetID(string: string))
        }
    }

    func testCodable() throws {
        let decoder = JSONDecoder()

        XCTAssertEqual(
            UniversalAssetID(coin: .ethereum, token: "0x6B175474E89094C44Da98b954EedeAC495271d0F"),
            try decoder.decode(UniversalAssetID.self, from: Data("\"c60_t0x6B175474E89094C44Da98b954EedeAC495271d0F\"".utf8))
        )

        XCTAssertEqual(
            UniversalAssetID(coin: .ethereum),
           try decoder.decode(UniversalAssetID.self, from: Data("\"c60\"".utf8))
       )

        XCTAssertThrowsError(try decoder.decode(UniversalAssetID.self, from: Data("\"60\"".utf8)))
        XCTAssertThrowsError(try decoder.decode(UniversalAssetID.self, from: Data("\"4\"".utf8)))
        XCTAssertThrowsError(try decoder.decode(UniversalAssetID.self, from: Data()))
    }
}
