// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class EOSTests: XCTestCase {
    func testAddress() {
        let privateKey = PrivateKey(wif: "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3")!
        let publicKey = privateKey.getPublicKey(compressed: false)

        let address = EOS().address(for: publicKey)
        let string = "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"
        let fromString = EOSAddress(string: string)

        XCTAssertEqual(address.description, string)
        XCTAssertTrue(EOSAddress.isValidString(string: string))
        XCTAssertEqual(fromString!.description, string)
    }
}
