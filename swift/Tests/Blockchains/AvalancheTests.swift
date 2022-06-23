// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class AvalancheTests: XCTestCase {

    func testCChainAddress() {
        let key = PrivateKey(data: Data(hexString: "98cb077f972feb0481f1d894f272c6a1e3c15e272a1658ff716444f465200070")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .avalancheCChain)
        let addressETH = AnyAddress(publicKey: pubkey, coin: .ethereum)

        XCTAssertEqual(address.description, addressETH.description)
        XCTAssertEqual(address.data.hexString, addressETH.data.hexString)
    }

    func testXPub() {
        let wallet = HDWallet(mnemonic: "liquid spider narrow follow black west cabbage intact stadium resource gentle raccoon", passphrase: "")!

        let xpub1 = wallet.getExtendedPublicKey(purpose: .bip44, coin: .ethereum, version: .xpub)
        let xpub2 = wallet.getExtendedPublicKey(purpose: .bip44, coin: .avalancheCChain, version: .xpub)

        XCTAssertEqual(xpub1, xpub2)

        let xpri1 = wallet.getExtendedPrivateKey(purpose: .bip44, coin: .ethereum, version: .xprv)
        let xpri2 = wallet.getExtendedPrivateKey(purpose: .bip44, coin: .smartChain, version: .xprv)

        XCTAssertEqual(xpri1, xpri2)
    }
}
