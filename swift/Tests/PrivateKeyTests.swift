// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class PrivateKeyTests: XCTestCase {
    func testCreateNew() {
        let privateKey = PrivateKey()

        XCTAssertEqual(privateKey.data.count, TWPrivateKeySize)
        XCTAssertTrue(PrivateKey.isValid(data: privateKey.data, curve: .secp256k1))
        XCTAssertTrue(PrivateKey.isValid(data: privateKey.data, curve: .ed25519))
    }

    func testCreateFromInvalid() {
        let privateKey = PrivateKey(data: Data(hexString: "0xdeadbeef")!)
        XCTAssertNil(privateKey)
    }

    func testIsValidString() {
        let data = Data(hexString: "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5")!

        XCTAssertTrue(PrivateKey.isValid(data: data, curve: .secp256k1))
        XCTAssertTrue(PrivateKey.isValid(data: data, curve: .ed25519))
    }

    func testPublicKey() {
        let privateKey = PrivateKey(data: Data(hexString: "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)

        XCTAssertEqual(publicKey.data.hexString, "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91")
    }

    func testSignSchnorr() {
        let privateKey = PrivateKey(data: Data(hexString: "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)

        let message = "hello schnorr".data(using: .utf8)!

        let sig = privateKey.signSchnorr(message: message, curve: .secp256k1)!
        let verified = publicKey.verifySchnorr(signature: sig, message: message)

        XCTAssertEqual(sig.hexString, "d166b1ae7892c5ef541461dc12a50214d0681b63d8037cda29a3fe6af8bb973e4ea94624d85bc0010bdc1b38d05198328fae21254adc2bf5feaf2804d54dba55")
        XCTAssertTrue(verified)
    }
}
