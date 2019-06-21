// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class DogeTests: XCTestCase {

    private let coin = CoinType.dogecoin

    func testAddress() {
        let string = "DLrjRgrVqbbpGrSQUtSYgsiWWMvRz5skQE"

        XCTAssertEqual(
            coin.address(string: string)?.description,
            BitcoinAddress(string: string)?.description
        )
    }

    func testAddressValidate() {
        XCTAssertTrue(
            coin.validate(address: "DLrjRgrVqbbpGrSQUtSYgsiWWMvRz5skQE")
        )
    }

    func testExtendedKeys() {
        let wallet = HDWallet.test

        // .bip44
        let dgpv = wallet.getExtendedPrivateKey(purpose: .bip44, coin: coin, version: .dgpv)
        let dgub = wallet.getExtendedPubKey(purpose: .bip44, coin: coin, version: .dgub)

        XCTAssertEqual(dgpv, "dgpv57ru95KiYUB5oWm2CVQH4heh1f7E9dNGdRHHVThcQkLeQ2HHxVJfFYefnpKrWZ6L6EDKJHUVq4Yyd5kPZKnRePfkCz3EzkySTydXKbgjcxN")
        XCTAssertEqual(dgub, "dgub8rjvUmFc6cqR6NRBEj2FBZCHUDUrykPyv24Vea6bCsPex5PzNFrRtr4KN37XgwuVzzC2MikJRW2Ddcp99Ehsqp2iaU4eerNCJVruKxz6Gci")
    }

    func testDeriveFromDpub() {
        let dgub = "dgub8rjvUmFc6cqR6NRBEj2FBZCHUDUrykPyv24Vea6bCsPex5PzNFrRtr4KN37XgwuVzzC2MikJRW2Ddcp99Ehsqp2iaU4eerNCJVruKxz6Gci"
        let pubkey8 = HDWallet.derive(from: dgub, at: DerivationPath(purpose: .bip44, coinType: coin, account: 0, change: 0, address: 8))!

        let address = BitcoinAddress(publicKey: pubkey8, prefix: coin.p2pkhPrefix)!
        XCTAssertEqual(address.description, "DLrjRgrVqbbpGrSQUtSYgsiWWMvRz5skQE")
    }
}
