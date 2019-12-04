// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class PolkadotTests: XCTestCase {

    func testAddressValidation() {
        let polkadot = CoinType.polkadot
        // cosmos
        XCTAssertFalse(polkadot.validate(address: "cosmos1l4f4max9w06gqrvsxf74hhyzuqhu2l3zyf0480"))
        // Bitcoin p2sh
        XCTAssertFalse(polkadot.validate(address: "3317oFJC9FvxU2fwrKVsvgnMGCDzTZ5nyf"))
        // Kusama
        XCTAssertFalse(polkadot.validate(address: "ELmaX1aPkyEF7TSmYbbyCjmSgrBpGHv9EtpwR2tk1kmpwvG"))
        // polkadot sr25519
        XCTAssertTrue(polkadot.validate(address: "14PhJGbzPxhQbiq7k9uFjDQx3MNiYxnjFRSiVBvBBBfnkAoM"))
    }

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "0xd65ed4c1a742699b2e20c0c1f1fe780878b1b9f7d387f934fe0a7dc36f1f9008")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = PolkadotAddress(publicKey: pubkey)
        let addressFromString = PolkadotAddress(string: "12twBQPiG5yVSf3jQSBkTAKBKqCShQ5fm33KQhH3Hf6VDoKW")!

        XCTAssertEqual(pubkey.data.hexString, "53d82211c4aadb8c67e1930caef2058a93bc29d7af86bf587fba4aa3b1515037")
        XCTAssertEqual(address.description, addressFromString.description)
    }
}
