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
        let bc = BitcoinCash()
        let xpubAddr2 = bc.derive(from: xpub, at: bc.derivationPath(at: 2))!
        let xpubAddr9 = bc.derive(from: xpub, at: bc.derivationPath(at: 9))!

        XCTAssertEqual(xpubAddr2.description, "bitcoincash:qq4cm0hcc4trsj98v425f4ackdq7h92rsy6zzstrgy")
        XCTAssertEqual(xpubAddr9.description, "bitcoincash:qqyqupaugd7mycyr87j899u02exc6t2tcg9frrqnve")
    }

    func testAddress() {
        XCTAssertEqual(
            "bitcoincash:prm3srpqu4kmx00370m4wt5qr3cp7sekmcksezufmd",
            BitcoinCash().address(string: "bitcoincash:prm3srpqu4kmx00370m4wt5qr3cp7sekmcksezufmd")?.description
        )
        XCTAssertEqual(
            "bitcoincash:prm3srpqu4kmx00370m4wt5qr3cp7sekmcksezufmd",
            BitcoinCash().address(string: "prm3srpqu4kmx00370m4wt5qr3cp7sekmcksezufmd")?.description
        )
    }
}
