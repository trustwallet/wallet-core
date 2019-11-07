// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class EthereumAbiTests: XCTestCase {
    func testEncoder() {
        let function = EthereumAbiEncoder.buildFunction(name: "sam")!
        XCTAssertEqual(0, function.addParamByteArray(val: Data(hexString: "0x64617665")!, isOutput: false))
        XCTAssertEqual(1, function.addParamBool(val: true, isOutput: false))
        let idx = function.addParamArray(isOutput: false)
        XCTAssertEqual(2, idx)
        XCTAssertEqual(0, function.addInArrayParamUInt256(arrayIdx: idx, val: Data(hexString: "0x01")!))
        XCTAssertEqual(1, function.addInArrayParamUInt256(arrayIdx: idx, val: Data(hexString: "0x02")!))
        XCTAssertEqual(2, function.addInArrayParamUInt256(arrayIdx: idx, val: Data(hexString: "0x03")!))

        XCTAssertEqual(function.getType(), "sam(bytes,bool,uint256[])")

        XCTAssertEqual(EthereumAbiEncoder.encode(func_in: function).hexString, "a5643bf20000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000000464617665000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000003")
    }
}
