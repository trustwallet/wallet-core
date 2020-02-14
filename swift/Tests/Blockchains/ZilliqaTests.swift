// Copyright © 2017-2020 Trust Wallet.
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
        let keyHash = "7FCcaCf066a5F26Ee3AFfc2ED1FA9810Deaa632C"
        let address = AnyAddress(publicKey: pubKey, coin: .zilliqa)
        let address2 = AnyAddress(string: "zil10lx2eurx5hexaca0lshdr75czr025cevqu83uz", coin: .zilliqa)!

        XCTAssertEqual(String(data: address.data, encoding: .utf8)!, keyHash)
        XCTAssertEqual(address.description, address2.description)
    }

    func testSigner() {
        let privateKey = PrivateKey(data: Data(hexString: "0x68ffa8ec149ce50da647166036555f73d57f662eb420e154621e5f24f6cf9748")!)!
        // 1 ZIL
        let input = ZilliqaSigningInput.with {
            $0.version = 65537 // mainnet tx version
            $0.nonce = 2
            $0.toAddress = "zil10lx2eurx5hexaca0lshdr75czr025cevqu83uz"
            $0.amount = Data(hexString: "e8d4a51000")!
            $0.gasPrice = Data(hexString: "3b9aca00")!
            $0.gasLimit = 1
            $0.privateKey = privateKey.data
        }

        let output: ZilliqaSigningOutput = AnySigner.sign(input: input, coin: .zilliqa)
        let expectedJSON = """
{
    "amount": "1000000000000",
    "toAddr": "7FCcaCf066a5F26Ee3AFfc2ED1FA9810Deaa632C",
    "pubKey": "03fb30b196ce3e976593ecc2da220dca9cdea8c84d2373770042a930b892ac0f5c",
    "data": "",
    "code": "",
    "signature": "001fa4df08c11a4a79e96e69399ee48eeecc78231a78b0355a8ca783c77c139436e37934fecc2252ed8dac00e235e22d18410461fb896685c4270642738ed268",
    "gasLimit": "1",
    "version": 65537,
    "gasPrice": "1000000000",
    "nonce": 2
}
"""
        XCTAssertEqual(output.signature.hexString, "001fa4df08c11a4a79e96e69399ee48eeecc78231a78b0355a8ca783c77c139436e37934fecc2252ed8dac00e235e22d18410461fb896685c4270642738ed268")
        XCTAssertJSONEqual(output.json, expectedJSON)
    }
}
