// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class AvalancheTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "0x98cb077f972feb0481f1d894f272c6a1e3c15e272a1658ff716444f465200070")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .avalanche)
        let addressFromString = AnyAddress(string: "X-avax1apmh7wxg3js48fhacfv5y9md9065jxuft30vup", coin: .avalanche)!

        XCTAssertEqual(pubkey.data.hexString, "0x02b33c917f2f6103448d7feb42614037d05928433cb25e78f01a825aa829bb3c27")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        // TODO: Create implementation
    }
}
