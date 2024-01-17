// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class CeloTests: XCTestCase {
    func testTransfer() {
        // real key is bnb-eth
        let keyData = Data(hexString: "0x4646464646464646464646464646464646464646464646464646464646464646")!

        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "a4ec")!
            $0.nonce = Data(hexString: "00")!
            $0.gasPrice = Data(hexString: "0x1dcd6500")! // 500000000
            $0.gasLimit = Data(hexString: "0x520c")! // 21004
            $0.toAddress = "0x01EfD468c9e58B63e54BEFa100cF1690880fFCcB"
            $0.privateKey = keyData
            $0.transaction = EthereumTransaction.with {
                $0.transfer = EthereumTransaction.Transfer.with {
                    $0.amount = Data(hexString: "0DE0B6B3A7640000")! // 1 CELO
                }
            }
        }

        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .celo)

        // https://explorer.celo.org/tx/0x381252b2de73e157cb669d2aff8f9e13964e3e70cbacab39aaadf05abca9b7ca
        XCTAssertEqual("0x" + output.encoded.hexString, "0xf86e80841dcd650082520c9401efd468c9e58b63e54befa100cf1690880ffccb880de0b6b3a764000080830149fca0b5702b6a650264e23917fcd2c9c73c687ab4ad0a825deff3519cf3f74ad69508a078d6c8afbcff9e2af748d065a926498b129c33f9dac1efc64a0432270bb6ed10")
    }
}
