// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class EthereumFeeTests: XCTestCase {

    func testBaseFeeOnly() throws {
        let url = Bundle(for: EthereumFeeTests.self).url(forResource: "eth_feeHistory", withExtension: "json")!
        let data = try String(contentsOf: url)

        guard let result = EthereumFee.suggest(feeHistory: data) else {
            XCTFail("fail to suggest fee")
            return
        }

        let expected = """
        {
            "baseFee": "80885125075",
            "maxPriorityFee": "2000000000"
        }
        """

        XCTAssertJSONEqual(result, expected)
    }

    func testBaseFeeAndPriority() throws {
        let url = Bundle(for: EthereumFeeTests.self).url(forResource: "eth_feeHistory2", withExtension: "json")!
        let data = try String(contentsOf: url)

        guard let result = EthereumFee.suggest(feeHistory: data) else {
            XCTFail("fail to suggest fee")
            return
        }

        let expected = """
        {
            "baseFee": "87408740684",
            "maxPriorityFee": "1500000000"
        }
        """

        XCTAssertJSONEqual(result, expected)
    }
}
