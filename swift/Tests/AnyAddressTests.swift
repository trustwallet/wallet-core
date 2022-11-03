// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class AnyAddressTests: XCTestCase {
    let ANY_ADDRESS_TEST_ADDRESS = "bc1qcj2vfjec3c3luf9fx9vddnglhh9gawmncmgxhz"
    let ANY_ADDRESS_TEST_PUBKEY = "02753f5c275e1847ba4d2fd3df36ad00af2e165650b35fe3991e9c9c46f68b12bc"

    func testCreateWithString() {
        let coin = CoinType.bitcoin
        let address = AnyAddress(string: ANY_ADDRESS_TEST_ADDRESS, coin: coin)!
        XCTAssertEqual(address.coin, coin)
        XCTAssertEqual(address.description, ANY_ADDRESS_TEST_ADDRESS)
    }

    func testCreateWithStringBech32() {
        let coin = CoinType.bitcoin
        let address = AnyAddress(string: ANY_ADDRESS_TEST_ADDRESS, coin: coin, hrp: "bc")!
        XCTAssertEqual(address.coin, coin)
        XCTAssertEqual(address.description, ANY_ADDRESS_TEST_ADDRESS)
    }

    func testCreateWithPublicKey() {
        let coin = CoinType.bitcoin
        let pubkey = PublicKey(data: Data(hexString: ANY_ADDRESS_TEST_PUBKEY)!, type: .secp256k1)!
        let address = AnyAddress(publicKey: pubkey, coin: coin)
        XCTAssertEqual(address.description, ANY_ADDRESS_TEST_ADDRESS)
    }

    func testCreateWithPublicKeyDerivation() {
        let coin = CoinType.bitcoin
        let pubkey = PublicKey(data: Data(hexString: ANY_ADDRESS_TEST_PUBKEY)!, type: .secp256k1)!
        let address1 = AnyAddress(publicKey: pubkey, coin: coin, derivation: .bitcoinSegwit)
        XCTAssertEqual(address1.description, ANY_ADDRESS_TEST_ADDRESS)

        let address3 = AnyAddress(publicKey: pubkey, coin: coin, derivation: .bitcoinLegacy)
        XCTAssertEqual(address3.description, "1JvRfEQFv5q5qy9uTSAezH7kVQf4hqnHXx")
    }

    func testIsValid() {
        let coin = CoinType.bitcoin
        XCTAssertTrue(AnyAddress.isValid(string: ANY_ADDRESS_TEST_ADDRESS, coin: coin));
        XCTAssertFalse(AnyAddress.isValid(string: ANY_ADDRESS_TEST_ADDRESS, coin: .ethereum));
        XCTAssertFalse(AnyAddress.isValid(string: "__INVALID_ADDRESS__", coin: .ethereum));
    }

    func testIsValidBech32() {
        let coin = CoinType.bitcoin
        XCTAssertTrue(AnyAddress.isValidBech32(string: ANY_ADDRESS_TEST_ADDRESS, coin: coin, hrp: "bc"));
        XCTAssertFalse(AnyAddress.isValidBech32(string: ANY_ADDRESS_TEST_ADDRESS, coin: coin, hrp: "tb"));
    }
}
