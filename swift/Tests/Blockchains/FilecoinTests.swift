// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class FilecoinTests: XCTestCase {

    func testAddress() {
        let privateKey = PrivateKey(data: Data(hexString: "1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: publicKey, coin: .filecoin)
        XCTAssertEqual(address.description, "f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq")
    }

    func testSigner() {
        let input = FilecoinSigningInput.with {
            $0.privateKey = Data(hexString: "1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe")!
            $0.toAddress = "f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq"
            $0.nonce = 2
            // 600 FIL
            $0.value = Data(hexString: "2086ac351052600000")!
            $0.gasPrice = Data(hexString: "02")!
            $0.gasLimit = 1000
        }

        let output: FilecoinSigningOutput = AnySigner.sign(input: input, coin: .filecoin)

        XCTAssertEqual(output.encoded.hexString, "828900583103a33d476e13eb8bde5d21becf2b86dd60642f0297cc6a5b914de86bb1d096861ba99bb13c577fee003e72f51e89f837c45501cf01bf485f61435e6770b52615bf455e043a2361024a002086ac3510526000004200021903e80040584201d7320c388135b20dea596493156495519db9e3cfcb78ccb18462dde4aa1c507750a79ef014caf717acbe2ce562e5e55a4c990607c32a741f1c073fa922176cc900")
    }

}
