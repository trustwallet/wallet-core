// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class ZcashTests: XCTestCase {

    let zcash = CoinType.zcash

    func testValidAddresses() {

        XCTAssertTrue(zcash.validate(address: "t1TKCtCETHPrAdA6eY1fdhhnTkTmb371oPt"))
        XCTAssertTrue(zcash.validate(address: "t3Pnbg7XjP7FGPBUuz75H65aczphHgkpoJW"))

        // XCTAssertTrue(zcash.validate(address: "zcXkH2LxmKKQH9D1q7xCrFT6gEKscT1K5f2HGFKxtXUfwczjLy1BZXKMuCdCU616yAVSkrdwzazN6DDh3zL82ULqpggEw51"))
        // XCTAssertTrue(zcash.validate(address: "zs10lpewlejk9lvdnnx4jwf7vssstcag2whqxj8e6s38kdwu6xtsh85d25fhqcrt6fz9wpxvsw0nsl"))
    }

    func testInvalidAddresses() {
        for addr in [
            "bc1qvtvte5tzlqlfhcdmph94lxk8jcz54q6psyvgla",
            "18eqGohuqvrZLL3LMR4Wv81qvKeDHsGpjH",
            "LV7LV7Z4bWDEjYkfx9dQo6k6RjGbXsg6hS",
            "Xm1iDLBP5tdxTxc6t7uJBCVjC4L2A5vB2J",
            "TKjdnbJxP4yHeLTHZ86DGnFFY6QhTjuBv2",
            ] {
            XCTAssertFalse(zcash.validate(address: addr))
        }
    }

    func testDeriveFromXpub() {
        let xpub = "xpub6C7HhMqpir3KBA6ammv5B58RT3XFTJqoZFoj3J56dz9XwehZ2puSH38ERtnz7HaXGxaZP8AHT4M2bSRHpBXUZrbsJ2xg3xs53DGKYCqj8mr"
        let pubkey = HDWallet.derive(from: xpub, at: DerivationPath(purpose: zcash.purpose, coinType: zcash))!
        let address = ZcashTAddress(publicKey: pubkey, prefix: zcash.p2pkhPrefix)!

        XCTAssertEqual(address.description, "t1TKCtCETHPrAdA6eY1fdhhnTkTmb371oPt")
    }
}
