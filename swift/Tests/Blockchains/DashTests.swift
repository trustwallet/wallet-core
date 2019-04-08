// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class DashAddressTests: XCTestCase {
    func testAddress() {
        let privateKey = PrivateKey(data: Data(hexString: "4b45e94800b9a2c3a45296f8de718bf9577cbe444773c39508d7f957355c759c")!)!
        let address = CoinType.dash.deriveAddress(privateKey: privateKey)

        XCTAssertEqual(address, "Xw7HTXGY3TFeA3ZsVuMRrYh96GtwWb4hQb")
    }

    func testExtendedKeys() {
        let wallet = HDWallet.test

        let xprv = wallet.getExtendedPrivateKey(purpose: .bip44, coin: .dash, version: .xprv)
        let xpub = wallet.getExtendedPubKey(purpose: .bip44, coin: .dash, version: .xpub)

        XCTAssertEqual(xprv, "xprv9zSMAfz7nQUZDQXMifsT5Cbss1Kh8XgnsKsrFfx83bvbuubs6ra84k95XMpAJmt51jymfNrXid81bu9tUTW2W2g7CBU5e6F297XBuXfSmjJ")
        XCTAssertEqual(xpub, "xpub6DRhaBX1cn2rRtbpphQTSLYcR3ABXzQeEYoT44MjbwTanhw1ePtNcYTZNeHyrJMsMGTbig4iFMSvht7RviohzFxkpjURgHDThygLqbZ1tib")
    }

    func testDeriveFromXPub() {
        let xpub = "xpub6DRhaBX1cn2rRtbpphQTSLYcR3ABXzQeEYoT44MjbwTanhw1ePtNcYTZNeHyrJMsMGTbig4iFMSvht7RviohzFxkpjURgHDThygLqbZ1tib"

        let coin = CoinType.dash
        let xpubAddr2 = HDWallet.derive(from: xpub, at: DerivationPath(purpose: coin.purpose, coinType: coin, account: 0, change: 1, address: 2))!
        let xpubAddr9 = HDWallet.derive(from: xpub, at: DerivationPath(purpose: coin.purpose, coinType: coin, account: 0, change: 1, address: 9))!

        XCTAssertEqual(coin.deriveAddressFromPublicKey(publicKey: xpubAddr2), "Xh4D3Mv6ikL5iR45bEsCtaR8Ub4jkRLpU2")
        XCTAssertEqual(coin.deriveAddressFromPublicKey(publicKey:xpubAddr9), "XvwNJsXVBpvAU92xPwU8phT6wKjJVaBMkk")
    }
}
