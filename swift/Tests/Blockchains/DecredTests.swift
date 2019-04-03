// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class DecredTests: XCTestCase {

    func testExtendedKeys() {
        let wallet = HDWallet.test

        // .bip44
        let dprv = wallet.getExtendedPrivateKey(purpose: .bip44, coin: .decred, version: .dprv)
        let dpub = wallet.getExtendedPubKey(purpose: .bip44, coin: .decred, version: .dpub)

        XCTAssertEqual(dprv, "dprv3oggQ2FQ1chcr18hbW7Aur5x8SxQdES3FGa4WqeTZnFY88SNMzLdB7LkZLroF4bGAqWS8sDm3w4DKyYV7sDKfC6JMSVHnVJdpDLgHioq1vq")
        XCTAssertEqual(dpub, "dpubZFUmm9oh5zmQkR2Tr2AXS4tCkTWg4B27SpCPFkapZrrAqgU1EwgEFgrmi6EnLGXhak86yDHhXPxFAnGU58W5S4e8NCKG1ASUVaxwRqqNdfP")
    }

    func testDeriveFromDpub() {
        let dpub = "dpubZFUmm9oh5zmQkR2Tr2AXS4tCkTWg4B27SpCPFkapZrrAqgU1EwgEFgrmi6EnLGXhak86yDHhXPxFAnGU58W5S4e8NCKG1ASUVaxwRqqNdfP"
        let pubkey0 = HDWallet.derive(from: dpub, at: DerivationPath(purpose: .bip44, coinType: .decred, account: 0, change: 0, address: 0))!

        XCTAssertEqual(DecredAddress(publicKey: pubkey0).description, "DsksmLD2wDoA8g8QfFvm99ASg8KsZL8eJFd")
    }

}
