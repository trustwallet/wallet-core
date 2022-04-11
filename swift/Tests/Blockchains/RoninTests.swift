// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class RoninTests: XCTestCase {

    func testTransferAXS() {
        // real key is 1p
        let keyData = Data(hexString: "0x4646464646464646464646464646464646464646464646464646464646464646")!

        // How to send free 0 gas tx:
        // 1. Registered your address at https://marketplace.axieinfinity.com/
        // 2. Query free quota by rpc eth_getFreeGasRequests
        // 3. Send tx to proxy node
        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "0x07e4")!
            $0.nonce = Data(hexString: "0x02")!
            $0.gasPrice = Data()
            $0.gasLimit = Data(hexString: "0xca98")! // 51,864
            $0.toAddress = "ronin:97a9107c1793bc407d6f527b77e7fff4d812bece" // AXS
            $0.privateKey = keyData
            $0.transaction = EthereumTransaction.with {
                $0.erc20Transfer = EthereumTransaction.ERC20Transfer.with {
                    $0.to = "ronin:47331175b23c2f067204b506ca1501c26731c990"
                    $0.amount = Data(hexString: "0x016345785d8a0000")! // 0.1 AXS
                }
            }
        }

        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ronin)

        // https://explorer.roninchain.com/tx/0x021e77da4ce4cfab90c4522ab8a5affddbd81dda819543f42eac3aa7102ea5bc
        XCTAssertEqual("0x" + output.encoded.hexString, "0xf8a6028082ca989497a9107c1793bc407d6f527b77e7fff4d812bece80b844a9059cbb00000000000000000000000047331175b23c2f067204b506ca1501c26731c990000000000000000000000000000000000000000000000000016345785d8a0000820feba0de07884db1de6db69cb3d3612f7974d9078cc86fd23103de0414be7684967708a071119e5d151f8616e004e7e8e8aed0e078a019d3f84cde7d08a94f62dee99f59")
    }
}
