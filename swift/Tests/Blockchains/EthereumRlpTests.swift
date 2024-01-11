// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class EthereumRlpTests: XCTestCase {
    func testRlpEncodeEip1559() throws {
        let chainId = Data(hexString: "0x0a")!
        let nonce = Data(hexString: "0x06")!
        let maxInclusionFeePerGas = Data(hexString: "0x77359400")! // 2000000000
        let maxFeePerGas = Data(hexString: "0xb2d05e00")! // 3000000000
        let gasLimit = Data(hexString: "0x526c")! // 21100
        let to = "0x6b175474e89094c44da98b954eedeac495271d0f"
        let amount = Data(hexString: "0x00")!
        let payload = Data(hexString: "a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000001ee0c29f50cb1")!
        // Empty `access_list`.
        let accessList = EthereumRlpRlpList()
        
        let rlpList = EthereumRlpRlpList.with {
            $0.items = [
                EthereumRlpRlpItem.with { $0.numberU256 = chainId },
                EthereumRlpRlpItem.with { $0.numberU256 = nonce },
                EthereumRlpRlpItem.with { $0.numberU256 = maxInclusionFeePerGas },
                EthereumRlpRlpItem.with { $0.numberU256 = maxFeePerGas },
                EthereumRlpRlpItem.with { $0.numberU256 = gasLimit },
                EthereumRlpRlpItem.with { $0.address = to },
                EthereumRlpRlpItem.with { $0.numberU256 = amount },
                EthereumRlpRlpItem.with { $0.data = payload },
                EthereumRlpRlpItem.with { $0.list = accessList },
            ]
        }
        
        let encodingInput = EthereumRlpEncodingInput.with {
            $0.item.list = rlpList
        }
        let inputData = try encodingInput.serializedData()
        let outputData = EthereumRlp.encode(coin: .ethereum, input: inputData)
        
        let encodingOutput = try EthereumRlpEncodingOutput(serializedData: outputData)
        XCTAssertEqual(encodingOutput.error, CommonSigningError.ok)
        XCTAssertEqual(encodingOutput.encoded.hexString, "f86c0a06847735940084b2d05e0082526c946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000001ee0c29f50cb1c0")
    }
}
