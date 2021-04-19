// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class WanchainTests: XCTestCase {

    func testAddress() {
        let address = AnyAddress(string: "0xb08f432a3346e90e2ab61830ec227043131f70ff", coin: .wanchain)
        XCTAssertEqual(address?.description, "0xB08F432a3346e90E2ab61830eC227043131F70fF")
    }

    func testSigning() {
        // https://www.wanscan.org/tx/0xde8e48ff34ff94b55015c40ea32ac6053d1a5af7f15e33961c069f6ca8d1e0f9
        // real key is h8s test/main
        let key = Data(hexString: "0x4646464646464646464646464646464646464646464646464646464646464646")!
        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "0378")!
            $0.nonce = Data(hexString: "02")!
            $0.gasPrice = Data(hexString: "4190ab00")!
            $0.gasLimit = Data(hexString: "5208")!
            $0.toAddress = "0xc6D3DBf8dF90BA3f957A9634677805eee0e43bBe"
            $0.privateKey = key
            $0.transaction = EthereumTransaction.with {
                $0.transfer = EthereumTransaction.Transfer.with {
                    $0.amount = Data(hexString: "038d7ea4c68000")!
                }
            }
        }

        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .wanchain)

        XCTAssertEqual(output.v.hexString, "0714")
        XCTAssertEqual(output.encoded.hexString, "f86c02844190ab0082520894c6d3dbf8df90ba3f957a9634677805eee0e43bbe87038d7ea4c6800080820714a06205c010a8654ca92e7a8572ef6c3683c2eb3d6c497b3b569481b76655036396a04450df813e931e58688c02306bb98434d6c503f40f8411b31cc773d31c017af5")
    }
}
