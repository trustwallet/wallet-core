// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class MeterMetaStableGasTokenTests: XCTestCase {
    // TODO: Check and finalize implementation

    func testAddress() {
        // TODO: Check and finalize implementation

        let key = PrivateKey(data: Data(hexString: "f9b3c9cf1f2d154a0bc5d8d78d81687517b386427b22d564d762a957b571b045")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .metermetastablegastoken)
        let addressFromString = AnyAddress(string: "0xe9061c2517bba8a7e2d2c20053cd8323b577efe7", coin: .metermetastablegastoken)!

        XCTAssertEqual(pubkey.data.hexString, "0x24428a4aAf04C30D27627f879c4a3BFe005ef14c")
        XCTAssertEqual(address.description, addressFromString.description)
    }

}
