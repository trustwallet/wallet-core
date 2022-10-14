// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class JunoAddressTests: XCTestCase {
    func testAnyAddressValidation() {
        let addr = AnyAddress(string: "juno1gckvjxau7k56f8wg8c8xj80khyp83y8x8eqc94", coin: .cosmos, hrp: "juno")!;
        XCTAssertTrue(AnyAddress.isValidBech32(string: addr.description, coin: .cosmos, hrp: "juno"));
        XCTAssertFalse(AnyAddress.isValidBech32(string: addr.description, coin: .bitcoin, hrp: "juno"));
        XCTAssertFalse(AnyAddress.isValid(string: addr.description, coin: .bitcoin));
        XCTAssertFalse(AnyAddress.isValid(string: addr.description, coin: .cosmos));
    }
    
    func testAnyAddressFromPubkey() {
        let data = Data(hexString: "02753f5c275e1847ba4d2fd3df36ad00af2e165650b35fe3991e9c9c46f68b12bc")!;
        let pubkey = PublicKey(data: data, type: .secp256k1)!;
        let anyAddr = AnyAddress(publicKey: pubkey, coin: .cosmos, hrp: "juno");
        XCTAssertEqual(anyAddr.description, "juno1cj2vfjec3c3luf9fx9vddnglhh9gawmncn4k5n");
    }
}
