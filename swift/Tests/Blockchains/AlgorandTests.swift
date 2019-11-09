// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class AlgorandTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "a6c4394041e64fe93d889386d7922af1b9a87f12e433762759608e61434d6cf7")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AlgorandAddress(publicKey: pubkey)
        let addressFromString = AlgorandAddress(string: "ADIYK65L3XR5ODNNCUIQVEET455L56MRKJHRBX5GU4TZI2752QIWK4UL5A")!

        XCTAssertEqual(pubkey.data.hexString, "00d1857babdde3d70dad15110a9093e77abef991524f10dfa6a727946bfdd411")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        let transaction = AlgorandTransactionPay.with {
            $0.toAddress = "CRLADAHJZEW2GFY2UPEHENLOGCUOU74WYSTUXQLVLJUJFHEUZOHYZNWYR4"
            $0.fee = 263000
            $0.amount = 1000000000000
            $0.firstRound = 1937767
            $0.lastRound = 1938767
        }
        let input = AlgorandSigningInput.with {
            $0.genesisID = "mainnet-v1.0"
            $0.genesisHash = Data(base64Encoded: "wGHE2Pwdvd7S12BL5FaOP20EGYesN73ktiC1qzkkit8=")!
            $0.note = "hello".data(using: .utf8)!
            $0.privateKey = Data(hexString: "d5b43d706ef0cb641081d45a2ec213b5d8281f439f2425d1af54e2afdaabf55b")!
            $0.transactionPay = transaction
        }
        let output = AlgorandSigner.sign(input: input)

        XCTAssertEqual(output.encoded.hexString, "82a3736967c440baa00062adcdcb5875e4435cdc6885d26bfe5308ab17983c0fda790b7103051fcb111554e5badfc0ac7edf7e1223a434342a9eeed5cdb047690827325051560ba374786e8aa3616d74cf000000e8d4a51000a3666565ce00040358a26676ce001d9167a367656eac6d61696e6e65742d76312e30a26768c420c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adfa26c76ce001d954fa46e6f7465c40568656c6c6fa3726376c42014560180e9c92da3171aa3c872356e30a8ea7f96c4a74bc1755a68929c94cb8fa3736e64c42061bf060efc02e2887dfffc8ed85268c8c091c013eedf315bc50794d02a8791ada474797065a3706179")
    }
}
