// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class ZilliqaTests: XCTestCase {
    let coin = CoinType.zilliqa

    func testConfig() {
        XCTAssertEqual(coin.hrp, .zilliqa)

        let address1 = coin.address(string: "0x7FCcaCf066a5F26Ee3AFfc2ED1FA9810Deaa632C")
        XCTAssertNil(address1)

        let address2 = coin.address(string: "zil10lx2eurx5hexaca0lshdr75czr025cevqu83uz")
        XCTAssertEqual("zil10lx2eurx5hexaca0lshdr75czr025cevqu83uz", address2?.description)
    }

    func testAddress() {
        let data = Data(hexString: "029d25b68a18442590e113132a34bb524695c4291d2c49abf2e4cdd7d98db862c3")!
        let pubKey = PublicKey(data: data, type: .secp256k1)!
        let keyHash = "0x7FCcaCf066a5F26Ee3AFfc2ED1FA9810Deaa632C"
        let address = ZilliqaAddress(publicKey: pubKey)
        let address2 = ZilliqaAddress(string: "zil10lx2eurx5hexaca0lshdr75czr025cevqu83uz")!
        let address3 = ZilliqaAddress(keyHash: Data(hexString: keyHash)!)!

        XCTAssertEqual(address.keyHash, keyHash)
        XCTAssertEqual(address.description, address2.description)
        XCTAssertEqual(address.description, address3.description)
    }

    func testSigner() {

        let privateKey = PrivateKey(data: Data(hexString: "0x68ffa8ec149ce50da647166036555f73d57f662eb420e154621e5f24f6cf9748")!)!

        // 1 ZIL
        let input = ZilliqaSigningInput.with {
            $0.version = TWZilliqaTxVersion
            $0.nonce = 2
            $0.toAddress = "zil10lx2eurx5hexaca0lshdr75czr025cevqu83uz"
            $0.amount = Data(hexString: "e8d4a51000")!
            $0.gasPrice = Data(hexString: "3b9aca00")!
            $0.gasLimit = 1
            $0.privateKey = privateKey.data
        }

        let signature = ZilliqaSigner.sign(input: input).signature

        XCTAssertEqual(signature.hexString, "001fa4df08c11a4a79e96e69399ee48eeecc78231a78b0355a8ca783c77c139436e37934fecc2252ed8dac00e235e22d18410461fb896685c4270642738ed268")
    }
}
