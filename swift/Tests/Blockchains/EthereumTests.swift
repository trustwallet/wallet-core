// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore
import BigInt

class EthereumTests: XCTestCase {

    func testBigIntSerialize() {
        let data = BigInt(21000).magnitude.serialize()
        let input = TW_Ethereum_Proto_SigningInput.with {
            $0.gasLimit = data
        }

        XCTAssertEqual(data.hexString, "5208")
        XCTAssertEqual(input.gasLimit, data)
    }

    func testSigner() {
        let input = TW_Ethereum_Proto_SigningInput.with {
            $0.chainID = BigUInt(1).serialize()
            $0.nonce = BigInt(9).magnitude.serialize()
            $0.gasPrice = BigInt(20_000_000_000).magnitude.serialize()
            $0.gasLimit = BigInt(21_000).magnitude.serialize()
            $0.toAddress = "0x3535353535353535353535353535353535353535"
            $0.amount = BigInt(1_000_000_000_000_000_000).magnitude.serialize()
            $0.privateKey = Data(hexString: "0x4646464646464646464646464646464646464646464646464646464646464646")!
        }

        XCTAssertEqual(input.chainID.hexString, "01")
        XCTAssertEqual(input.nonce.hexString, "09")
        XCTAssertEqual(input.gasPrice.hexString, "04a817c800")
        XCTAssertEqual(input.gasLimit.hexString, "5208")
        XCTAssertEqual(input.amount.hexString, "0de0b6b3a7640000")

        let output = EthereumSigner.sign(input: input)

        XCTAssertEqual(output.v.hexString, "25")
        XCTAssertEqual(output.r.hexString, "28ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276")
        XCTAssertEqual(output.s.hexString, "67cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83")
    }
}
