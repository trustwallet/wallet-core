// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class AionTests: XCTestCase {

    func testAddress() {
        let privateKey = PrivateKey(data: Data(hexString: "db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9")!)!
        let publicKey = privateKey.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: publicKey, coin: .aion)
        XCTAssertEqual(address.description, "0xa0d2312facea71b740679c926d040c9056a65a4bfa2ddd18ec160064f82909e7")
    }

    func testSigner() {
        let input = AionSigningInput.with {
            $0.nonce = Data(hexString: "09")!
            $0.gasPrice = Data(hexString: "04a817c800")!
            $0.gasLimit = Data(hexString: "5208")!
            $0.toAddress = "0xa082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e"
            $0.amount = Data(hexString: "2710")!
            $0.timestamp = 155157377101
            $0.privateKey = Data(hexString: "db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9")!
        }

        let output: AionSigningOutput = AnySigner.sign(input: input, coin: .aion)

        XCTAssertEqual(output.signature.hexString, "a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a7d3d3386742c2716031b79950cef5fcb49c079a5cab095c8b08915e126b9741389924ba2d5c00036a3b39c2a8562fa0800f1a13a566ce6e027274ce63a41dec07")
        XCTAssertEqual(output.encoded.hexString, "f89b09a0a082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e8227108085242019b04d8252088800000004a817c80001b860a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a7d3d3386742c2716031b79950cef5fcb49c079a5cab095c8b08915e126b9741389924ba2d5c00036a3b39c2a8562fa0800f1a13a566ce6e027274ce63a41dec07")
    }

}
