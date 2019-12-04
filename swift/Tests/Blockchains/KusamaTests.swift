// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class KusamaTests: XCTestCase {

    func testAddressValidation() {
        let kusama = CoinType.kusama
        // polkadot sr25519
        XCTAssertFalse(kusama.validate(address: "14PhJGbzPxhQbiq7k9uFjDQx3MNiYxnjFRSiVBvBBBfnkAoM"))
        // cosmos
        XCTAssertFalse(kusama.validate(address: "cosmos1l4f4max9w06gqrvsxf74hhyzuqhu2l3zyf0480"))
        // Bitcoin p2sh
        XCTAssertFalse(kusama.validate(address: "3317oFJC9FvxU2fwrKVsvgnMGCDzTZ5nyf"))

        XCTAssertTrue(kusama.validate(address: "ELmaX1aPkyEF7TSmYbbyCjmSgrBpGHv9EtpwR2tk1kmpwvG"))
    }

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "0x85fca134b3fe3fd523d8b528608d803890e26c93c86dc3d97b8d59c7b3540c97")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = KusamaAddress(publicKey: pubkey)
        let addressFromString = KusamaAddress(string: "HewiDTQv92L2bVtkziZC8ASxrFUxr6ajQ62RXAnwQ8FDVmg")!

        XCTAssertEqual(pubkey.data.hexString, "e0b3fcccfe0283cc0f8c105c68b5690aab8c5c1692a868e55eaca836c8779085")
        XCTAssertEqual(address.description, addressFromString.description)
    }
}
