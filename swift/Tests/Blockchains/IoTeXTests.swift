// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class IoTeXTests: XCTestCase {

    func testSign() {
        let privateKey = PrivateKey(data: Data(hexString: "0x68ffa8ec149ce50da647166036555f73d57f662eb420e154621e5f24f6cf9748")!)!

        let input = IoTeXSigningInput.with {
            $0.version = 1
            $0.nonce = 1
            $0.gasLimit = 1
            $0.gasPrice = "1"
            $0.transfer = TW_IoTeX_Proto_Transfer.with {
                $0.amount = "1"
                $0.recipient = IoTeXAddress(string: "io1e2nqsyt7fkpzs5x7zf2uk0jj72teu5n6aku3tr")!.description
            }
            $0.privateKey = privateKey.data
        }

        let encoded = IoTeXSigner.sign(input: input).encoded

        XCTAssertEqual(encoded.hexString, "0a39080110011801220131522e0a01311229696f3165326e7173797437666b707a733578377a6632756b306a6a3732746575356e36616b75337472124104fb30b196ce3e976593ecc2da220dca9cdea8c84d2373770042a930b892ac0f5cf762f20459c9100eb9d4d7597f5817bf21e10b53a0120b9ec1ba5cddfdcb669b1a41ec9757ae6c9009315830faaab250b6db0e9535b00843277f596ae0b2b9efc0bd4e14138c056fc4cdfa285d13dd618052b3d1cb7a3f554722005a2941bfede96601");
    }
}
