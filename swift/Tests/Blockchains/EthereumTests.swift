// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class EthereumTests: XCTestCase {
    func testSigner() {
        let input = TW_Ethereum_Proto_SigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.nonce = Data(hexString: "09")!
            $0.gasPrice = Data(hexString: "04a817c800")!
            $0.gasLimit = Data(hexString: "5208")!
            $0.toAddress = "0x3535353535353535353535353535353535353535"
            $0.amount = Data(hexString: "0de0b6b3a7640000")!
            $0.privateKey = Data(hexString: "0x4646464646464646464646464646464646464646464646464646464646464646")!
        }

        let output = EthereumSigner.sign(input: input)

        XCTAssertEqual(output.v.hexString, "25")
        XCTAssertEqual(output.r.hexString, "28ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276")
        XCTAssertEqual(output.s.hexString, "67cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83")
    }

    func testEncoder() {
        let encoder = EthAbiFunction(name: "sam")!
        _ = encoder.addInParamByteArray(val: Data(hexString: "0x64617665")!)
        _ = encoder.addInParamBool(val: true)

        let idx = encoder.addInParamArray()

        _ = encoder.addInArrayParamUInt256(arrayIdx: idx, val: Data(hexString: "0x01")!)
        _ = encoder.addInArrayParamUInt256(arrayIdx: idx, val: Data(hexString: "0x02")!)
        _ = encoder.addInArrayParamUInt256(arrayIdx: idx, val: Data(hexString: "0x03")!)

        XCTAssertEqual(encoder.getType(), "sam(bytes,bool,uint256[])")

        XCTAssertEqual(encoder.encode().hexString, "a5643bf20000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000000464617665000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000003")
    }
}
