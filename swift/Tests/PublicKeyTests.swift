// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
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
    
    func testVerifyStarkey() {
        let data = Data(hexString: "02c5dbad71c92a45cc4b40573ae661f8147869a91d57b8d9b8f48c8af7f83159")!
        let publicKey = PublicKey(data: data, type: .starkex)!
        let signature = Data(hexString: "061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9a")!
        let hash = Data(hexString: "06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76")!
        XCTAssertTrue(publicKey.verify(signature: signature, message: hash))
        let invalidSignature = Data(hexString: "061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9b")!
        XCTAssertFalse(publicKey.verify(signature: invalidSignature, message: hash))
    }
}
