// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class EthereumTests: XCTestCase {

    func testAddress() {
        let anyAddress = AnyAddress(string: "0x7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1", coin: .ethereum)

        XCTAssertEqual(anyAddress?.description, "0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1")
        XCTAssertEqual(anyAddress?.coin, .ethereum)

        let invalid = "0xMQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4"
        XCTAssertNil(Data(hexString: invalid))
        XCTAssertNil(AnyAddress(string: invalid, coin: .ethereum))
        XCTAssertFalse(AnyAddress.isValid(string: invalid, coin: .ethereum))
    }

    func testSigner() {
        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.nonce = Data(hexString: "09")!
            $0.gasPrice = Data(hexString: "04a817c800")!
            $0.gasLimit = Data(hexString: "5208")!
            $0.toAddress = "0x3535353535353535353535353535353535353535"
            $0.amount = Data(hexString: "0de0b6b3a7640000")!
            $0.privateKey = Data(hexString: "0x4646464646464646464646464646464646464646464646464646464646464646")!
        }

        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)
        let encoded = AnySigner.encode(input: input, coin: .ethereum)

        XCTAssertEqual(encoded, output.encoded)
        XCTAssertEqual(output.v.hexString, "25")
        XCTAssertEqual(output.r.hexString, "28ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276")
        XCTAssertEqual(output.s.hexString, "67cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83")
        XCTAssertEqual(output.encoded.hexString, "f86c098504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a028ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276a067cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83")
    }

    func testSignJSON() {
        let json = """
        {
            "chainId": "AQ==",
            "gasPrice": "1pOkAA==",
            "gasLimit": "Ugg=",
            "toAddress": "0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1",
            "amount": "A0i8paFgAA=="
        }
        """
        let key = Data(hexString: "17209af590a86462395d5881e60d11c7fa7d482cfb02b5a01b93c2eeef243543")!
        let result = AnySigner.signJSON(json, key: key, coin: .ethereum)

        XCTAssertEqual(result, "f86a8084d693a400825208947d8bf18c7ce84b3e175b339c4ca93aed1dd166f1870348bca5a160008025a0fe5802b49e04c6b1705088310e133605ed8b549811a18968ad409ea02ad79f21a05bf845646fb1e1b9365f63a7fd5eb5e984094e3ed35c3bed7361aebbcbf41f10")
    }

    func testDecode() {
        let rawTx = Data(hexString: "0xf8a86484b2d05e008277fb9400000000000c2e074ec69a0dfb2997ba6c7d2e1e80b8441896f70ae71cd96d4ba1c4b512b0c5bee30d2b6becf61e574c32a17a67156fa9ed3c4c6f0000000000000000000000004976fb03c32e5b8cfe2b6ccb31c09ba78ebaba4125a0b55e479d5872b7531437621780ead128cd25d8988fb3cda9bcfb4baeb0eda4dfa077b096cf0cb4bee6eb8c756e9cdba95a6cf62af74e05e7e4cdaa8100271a508d")!
        let decoded = AnySigner.decode(data: rawTx, coin: .ethereum)

        let expected = """
        {
          "gas": "0x77fb",
          "gasPrice": "0xb2d05e00",
          "input": "0x1896f70ae71cd96d4ba1c4b512b0c5bee30d2b6becf61e574c32a17a67156fa9ed3c4c6f0000000000000000000000004976fb03c32e5b8cfe2b6ccb31c09ba78ebaba41",
          "nonce": "0x64",
          "to": "0x00000000000c2e074ec69a0dfb2997ba6c7d2e1e",
          "value": "0x",
          "v": "0x25",
          "r": "0xb55e479d5872b7531437621780ead128cd25d8988fb3cda9bcfb4baeb0eda4df",
          "s": "0x77b096cf0cb4bee6eb8c756e9cdba95a6cf62af74e05e7e4cdaa8100271a508d"
        }
        """
        XCTAssertJSONEqual(String(data: decoded, encoding: .utf8)!, expected)
    }
}
