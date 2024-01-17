// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
        XCTAssertEqual(xpub2, "xpub6Bmo35QfCNvffj8tZsTVRvFxA5ULv2aHDV8dDCa7q6SzkMLJffxWRNE5vSJ4hANoKpmSp3p7Nbcp1vEz5F785HV4Aq2A6jWwHoyWp3EFgYp")

        let xpri1 = wallet.getExtendedPrivateKey(purpose: .bip44, coin: .ethereum, version: .xprv)
        let xpri2 = wallet.getExtendedPrivateKey(purpose: .bip44, coin: .smartChain, version: .xprv)

        XCTAssertEqual(xpri1, xpri2)
        XCTAssertEqual(xpri2, "xprv9xnSdZsmN1NNTF4RTqvV4nKDc3drWZrRrGD2QpAWGkv1sZ1A88eFsZuc59vKRr4mxJELH7C18ymaHENodYzEbeLq1JmPUAy3CmpA2inVCwo")
    }
}
