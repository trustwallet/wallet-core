// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
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
    
    func testStarkKeyCreation() {
        let data = Data(hexString: "06cf0a8bf113352eb863157a45c5e5567abb34f8d32cddafd2c22aa803f4892c")!
        XCTAssertTrue(PrivateKey.isValid(data: data, curve: .starkex))
        let privateKey = PrivateKey(data: data)!
        let pubKey = privateKey.getPublicKeyByType(pubkeyType: .starkex)
        XCTAssertEqual(pubKey.data.hexString, "02d2bbdc1adaf887b0027cdde2113cfd81c60493aa6dc15d7887ddf1a82bc831")
    }
    
    func testStarkKeySigning() {
        let data = Data(hexString: "0139fe4d6f02e666e86a6f58e65060f115cd3c185bd9e98bd829636931458f79")!
        let privateKey = PrivateKey(data: data)!
        let digest = Data(hexString: "06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76")!
        let signature = privateKey.sign(digest: digest, curve: .starkex)!
        XCTAssertEqual(signature.hexString, "061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9a")
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

        let sig = privateKey.signZilliqaSchnorr(message: message)!
        let verified = publicKey.verifyZilliqaSchnorr(signature: sig, message: message)

        XCTAssertEqual(sig.hexString, "d166b1ae7892c5ef541461dc12a50214d0681b63d8037cda29a3fe6af8bb973e4ea94624d85bc0010bdc1b38d05198328fae21254adc2bf5feaf2804d54dba55")
        XCTAssertTrue(verified)
    }
}
