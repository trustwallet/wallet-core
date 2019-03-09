// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class StellarTests: XCTestCase {

    func testAddressFromPrivateKey() {
        let key = PrivateKey(data: Data(hexString: "50ac56cb8d869d44fd39c8996e286b16ca878955eb6c40c82d8977221357bb54")!)!
        let pubkey = key.getPublicKeyEd25519()

        //FIXME PrivateKey should support Ed25519
        //XCTAssertEqual(pubkey.data.hexString, "09A966BCAACC103E38896BAAE3F8C2F06C21FD47DD4F864FF0D33F9819DF5CA2".lowercased())
    }

    func testAddressFromPublicKey() {
        let pubkey = PublicKey(data: Data(hexString: "0303E20EC6B4A39A629815AE02C0A1393B9225E3B890CAE45B59F42FA29BE9668D")!)!
        let address = StellarAddress(publicKey: pubkey)

        XCTAssertEqual(address.description, "GAB6EDWGWSRZUYUYCWXAFQFBHE5ZEJPDXCIMVZC3LH2C7IU35FTI2NOQ")
    }
}
