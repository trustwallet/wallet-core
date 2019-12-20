// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest

func XCTAssertJSONEqual(_ lhs: String, _ rhs: String) {
    let ljson = try! JSONSerialization.jsonObject(with: lhs.data(using: .utf8)!, options: [.fragmentsAllowed])
    let rjson = try! JSONSerialization.jsonObject(with: rhs.data(using: .utf8)!, options: [.fragmentsAllowed])

    let lstring = try! JSONSerialization.data(withJSONObject: ljson, options: [.sortedKeys, .prettyPrinted])
    let rstring = try! JSONSerialization.data(withJSONObject: rjson, options: [.sortedKeys, .prettyPrinted])

    XCTAssertEqual(lstring, rstring, "\(lhs) is not equal to \(rhs)")
}
