// Copyright © 2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class NebulasTests: XCTestCase {

    func testAddressFromPublicKey() {
        let privateKey = PrivateKey(data: Data(hexString: "d2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        let address = NebulasAddress(publicKey: publicKey)

        XCTAssertEqual(address.description, "n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY")
    }

    func testSigner() {
        let input = NebulasSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.fromAddress = "n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY"
            $0.nonce = Data(hexString: "07")!
            $0.gasPrice = Data(hexString: "0f4240")!     //1000000
            $0.gasLimit = Data(hexString: "030d40")!     //200000
            $0.toAddress = "n1SAeQRVn33bamxN4ehWUT7JGdxipwn8b17"
            $0.amount = Data(hexString: "98a7d9b8314c0000")!  //11000000000000000000ULL
            $0.payload = ""
            $0.timestamp = Data(hexString: "5cfc84ca")!   //1560052938
            $0.privateKey = PrivateKey(data: Data(hexString: "d2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b")!)!.data
        }
        let output = NebulasSigner.sign(input: input)
        XCTAssertEqual(output.algorithm, 1)
        //swiftlint:disable:next line_length
        XCTAssertEqual(output.signature.hexString, "f53f4a9141ff8e462b094138eccd8c3a5d7865f9e9ab509626c78460a9e0b0fc35f7ed5ba1795ceb81a5e46b7580a6f7fb431d44fdba92515399cf6a8e47e71500")
        //swiftlint:disable:next line_length
        XCTAssertEqual(output.raw, "CiBQXdR2neMqnEu21q/U+OHqZHSBX9Q0hNiRfL2eCZO4hRIaGVefwtw23wEobqA40/7aIwQHghETxH4r+50aGhlXf89CeLWgHFjKu9/6tn4KNbelsMDAIIi2IhAAAAAAAAAAAJin2bgxTAAAKAcwyony5wU6CAoGYmluYXJ5QAFKEAAAAAAAAAAAAAAAAAAPQkBSEAAAAAAAAAAAAAAAAAADDUBYAWJB9T9KkUH/jkYrCUE47M2MOl14Zfnpq1CWJseEYKngsPw19+1boXlc64Gl5Gt1gKb3+0MdRP26klFTmc9qjkfnFQA=")
    }
}
