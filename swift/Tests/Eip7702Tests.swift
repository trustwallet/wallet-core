// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class Eip7702Tests: XCTestCase {
    func testAuthorizationHash() {
        let chainId = Data(hexString: "0x01")!
        let contractAddress = "0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1"
        let nonce = Data(hexString: "0x01")!
        
        let authorizationHash = Eip7702.getAuthorizationHash(chainId: chainId, contractAddress: contractAddress, nonce: nonce)!
        XCTAssertEqual(authorizationHash.hexString, "3ae543b2fa103a39a6985d964a67caed05f6b9bb2430ad6d498cda743fe911d9") // Verified with viem
    }
}
