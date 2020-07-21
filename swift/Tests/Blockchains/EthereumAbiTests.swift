// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class EthereumAbiTests: XCTestCase {
    func testAbiEncoder() {
        let function = EthereumAbiFunction(name: "sam")
        // add params
        XCTAssertEqual(0, function.addParamBytes(val: Data(hexString: "0x64617665")!, isOutput: false))
        XCTAssertEqual(1, function.addParamBool(val: true, isOutput: false))
        let idx = function.addParamArray(isOutput: false)
        XCTAssertEqual(2, idx)
        XCTAssertEqual(0, function.addInArrayParamUInt256(arrayIdx: idx, val: Data(hexString: "0x01")!))
        XCTAssertEqual(1, function.addInArrayParamUInt256(arrayIdx: idx, val: Data(hexString: "0x02")!))
        XCTAssertEqual(2, function.addInArrayParamUInt256(arrayIdx: idx, val: Data(hexString: "0x03")!))
        XCTAssertEqual(0, function.addParamUInt64(val: 0, isOutput: true))

        // check signature
        XCTAssertEqual(function.getType(), "sam(bytes,bool,uint256[])")

        // encode into binrary
        XCTAssertEqual(EthereumAbi.encode(fn: function).hexString, "a5643bf20000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000000464617665000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000003")

        // original output value
        XCTAssertEqual(0, function.getParamUInt64(idx: 0, isOutput: true))
        // decode output from binary
        let encodedOutput = Data(hexString: "0x0000000000000000000000000000000000000000000000000000000000000045")!
        let decodeRes = EthereumAbi.decodeOutput(fn: function, encoded: encodedOutput)
        XCTAssertEqual(true, decodeRes)
        // new output value
        XCTAssertEqual(0x45, function.getParamUInt64(idx: 0, isOutput: true))
    }

    func testValueEncoder() {
        let data2 = EthereumAbiValue.encodeInt32(value: 69)
        XCTAssertEqual(data2.hexString, "0000000000000000000000000000000000000000000000000000000000000045")
    }

    func testValueDecoder() {
        let expected = "1234567890987654321"
        let inputs = [
            "112210f4b16c1cb1",
            "000000000000000000000000000000000000000000000000112210f4b16c1cb1",
            "000000000000000000000000000000000000000000000000112210f4b16c1cb10000000000000000000000000000000000000000000000000000000000000000"
        ]
        for input in inputs {
            let data = Data(hexString: input)!
            XCTAssertEqual(expected, EthereumAbiValue.decodeUInt256(input: data))
        }
    }

    func testDecodeApprove() throws {
        let data = Data(hexString: "095ea7b30000000000000000000000005aaeb6053f3e94c9b9a09f33669435e7ef1beaed0000000000000000000000000000000000000000000000000000000000000001")!
        let url = Bundle(for: EthereumAbiTests.self).url(forResource: "erc20", withExtension: "json")!
        let abi = try String(contentsOf: url)
        let decoded = EthereumAbi.decodeCall(data: data, abi: abi)!
        let expected = """
        {
            "function": "approve(address,uint256)",
            "inputs": [{
                "name": "_spender",
                "type": "address",
                "value": "0x5aaeb6053f3e94c9b9a09f33669435e7ef1beaed"
            }, {
                "name": "_value",
                "type": "uint256",
                "value": "1"
            }]
        }
        """
        XCTAssertJSONEqual(decoded, expected)
    }
}
