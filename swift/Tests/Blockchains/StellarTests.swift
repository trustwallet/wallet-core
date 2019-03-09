// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class StellarTests: XCTestCase {
    
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "50ac56cb8d869d44fd39c8996e286b16ca878955eb6c40c82d8977221357bb54")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = StellarAddress(publicKey: pubkey)
        
        XCTAssertEqual(address.description, "GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI")
    }
}
