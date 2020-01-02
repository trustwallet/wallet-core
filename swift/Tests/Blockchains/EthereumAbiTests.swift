// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class EthereumAbiTests: XCTestCase {
    func testEncoder() {
        let function = EthereumAbiEncoder.buildFunction(name: "sam")!
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
        XCTAssertEqual(EthereumAbiEncoder.encode(func_in: function).hexString, "a5643bf20000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000000464617665000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000003")

        // original output value
        XCTAssertEqual(0, function.getParamUInt64(idx: 0, isOutput: true))
        // decode output from binary
        let encodedOutput = Data(hexString: "0x0000000000000000000000000000000000000000000000000000000000000045")!
        let decodeRes = EthereumAbiEncoder.decodeOutput(func_in: function, encoded: encodedOutput)
        XCTAssertEqual(true, decodeRes)
        // new output value
        XCTAssertEqual(0x45, function.getParamUInt64(idx: 0, isOutput: true))
    }
}
