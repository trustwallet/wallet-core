// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest

func XCTAssertJSONEqual(_ lhs: String, _ rhs: String) {
    do {
        let ljson = try JSONSerialization.jsonObject(with: lhs.data(using: .utf8)!, options: [.fragmentsAllowed])
        let rjson = try JSONSerialization.jsonObject(with: rhs.data(using: .utf8)!, options: [.fragmentsAllowed])

        let lstring = try JSONSerialization.data(withJSONObject: ljson, options: [.sortedKeys, .prettyPrinted])
        let rstring = try JSONSerialization.data(withJSONObject: rjson, options: [.sortedKeys, .prettyPrinted])

        XCTAssertEqual(lstring, rstring, "\(lhs) is not equal to \(rhs)")
    } catch let error {
        XCTFail(error.localizedDescription)
    }
}
