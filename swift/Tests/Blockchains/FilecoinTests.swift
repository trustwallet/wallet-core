// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

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
            $0.gasLimit = 1000
            $0.gasFeeCap = Data(hexString: "25f273933db5700000")!
            $0.gasPremium = Data(hexString: "2b5e3af16b18800000")!
        }

        let output: FilecoinSigningOutput = AnySigner.sign(input: input, coin: .filecoin)

        XCTAssertEqual(output.encoded.hexString, "828a00583103a33d476e13eb8bde5d21becf2b86dd60642f0297cc6a5b914de86bb1d096861ba99bb13c577fee003e72f51e89f837c45501cf01bf485f61435e6770b52615bf455e043a2361024a002086ac3510526000001903e84a0025f273933db57000004a002b5e3af16b1880000000405842018cc46ef8e67f95fa69826a927c6b27b45ad12d6667520dd96262534d12ec56de15d77d74bd1dd5286269684e92e2c36f0ce13ab0482637d6267d390f54adaa3201")
    }

}
