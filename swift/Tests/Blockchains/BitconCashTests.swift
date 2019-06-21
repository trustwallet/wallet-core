// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class BitcoinCashTests: XCTestCase {

    func testExtendedKeys() {
        let wallet = HDWallet.test

        let xprv = wallet.getExtendedPrivateKey(purpose: .bip44, coin: .bitcoinCash, version: .xprv)
        let xpub = wallet.getExtendedPubKey(purpose: .bip44, coin: .bitcoinCash, version: .xpub)

        XCTAssertEqual(xprv, "xprv9yEvwSfPanK5gLYVnYvNyF2CEWJx1RsktQtKDeT6jnCnqASBiPCvFYHFSApXv39bZbF6hRaha1kWQBVhN1xjo7NHuhAn5uUfzy79TBuGiHh")
        XCTAssertEqual(xpub, "xpub6CEHLxCHR9sNtpcxtaTPLNxvnY9SQtbcFdov22riJ7jmhxmLFvXAoLbjHSzwXwNNuxC1jUP6tsHzFV9rhW9YKELfmR9pJaKFaM8C3zMPgjw")
    }

    func testDeriveFromXPub() {
        let xpub = "xpub6CEHLxCHR9sNtpcxtaTPLNxvnY9SQtbcFdov22riJ7jmhxmLFvXAoLbjHSzwXwNNuxC1jUP6tsHzFV9rhW9YKELfmR9pJaKFaM8C3zMPgjw"

        let coin = CoinType.bitcoinCash
        let xpubAddr2 = HDWallet.derive(from: xpub, at: DerivationPath(purpose: coin.purpose, coinType: coin, account: 0, change: 0, address: 2))!

        let xpubAddr9 = HDWallet.derive(from: xpub, at: DerivationPath(purpose: coin.purpose, coinType: coin, account: 0, change: 0, address: 9))!

        XCTAssertEqual(coin.deriveAddressFromPublicKey(publicKey: xpubAddr2), "bitcoincash:qq4cm0hcc4trsj98v425f4ackdq7h92rsy6zzstrgy")
        XCTAssertEqual(coin.deriveAddressFromPublicKey(publicKey: xpubAddr9), "bitcoincash:qqyqupaugd7mycyr87j899u02exc6t2tcg9frrqnve")
    }

    func testAddress() {
        XCTAssertEqual(
            "bitcoincash:prm3srpqu4kmx00370m4wt5qr3cp7sekmcksezufmd",
            BitcoinCashAddress(string: "bitcoincash:prm3srpqu4kmx00370m4wt5qr3cp7sekmcksezufmd")?.description
        )
        XCTAssertEqual(
            "bitcoincash:prm3srpqu4kmx00370m4wt5qr3cp7sekmcksezufmd",
            BitcoinCashAddress(string: "prm3srpqu4kmx00370m4wt5qr3cp7sekmcksezufmd")?.description
        )
    }

    func testLockScript() {
        let address = BitcoinCashAddress(string: "pzukqjmcyzrkh3gsqzdcy3e3d39cqxhl3g0f405k5l")!
        let script = BitcoinScript.buildForAddress(address: address.description, coin: .bitcoinCash)
        XCTAssertEqual(script.data.hexString, "a914b9604b7820876bc510009b8247316c4b801aff8a87")

        let address2 = BitcoinCashAddress(string: "qphr8l8ns8wd99a8653ctfe5qcrxaumz5qpmqlk2ex")!
        let script2 = BitcoinScript.buildForAddress(address: address2.description, coin: .bitcoinCash)
        XCTAssertEqual(script2.data.hexString, "76a9146e33fcf381dcd297a7d52385a73406066ef362a088ac")
    }
}
