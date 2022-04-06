// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class KuCoinCommunityChainTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "33b85056aabab539bcb68540735ecf054e38bc58b29b751530e2b54ecb4ca564")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .kuCoinCommunityChain)
        let addressFromString = AnyAddress(string: "0xE5cA667d795685E9915E5F4b4254ca832eEB398B", coin: .kuCoinCommunityChain)!

        XCTAssertEqual(pubkey.data.hexString, "0413bde18e3329af54d51a24f424fe09a8d7d42c324c07e10e53a6e139cbee80e6288142dec2ed46f7b81dccbb28d6168cdc7b208928730cbeeb911f8db6a707bb")
        XCTAssertEqual(address.description, addressFromString.description)
    }

}
