// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class PublicKeyTests: XCTestCase {

    let privateKey = PrivateKey(data: Data(hexString: "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5")!)!

    func testCompressed() {
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        XCTAssertEqual(publicKey.compressed.data.hexString, "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1")
    }

    func testVerify() {

        let message = Hash.sha256(data: "hello".data(using: .utf8)!)
        let sig1 = privateKey.sign(digest: message, curve: .ed25519)!
        let result1 = privateKey.getPublicKeyEd25519()
                            .verify(signature: sig1, message: message)

        let sig2 = privateKey.sign(digest: message, curve: .ed25519Blake2bNano)!
        let result2 = privateKey.getPublicKeyEd25519Blake2b()
            .verify(signature: sig2, message: message)

        let sig3 = privateKey.sign(digest: message, curve: .secp256k1)!
        let result3 = privateKey.getPublicKeySecp256k1(compressed: true)
                            .verify(signature: sig3, message: message)

        XCTAssertTrue(result1)
        XCTAssertTrue(result2)
        XCTAssertTrue(result3)
    }
}
