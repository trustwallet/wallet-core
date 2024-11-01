// Copyright © 2017-2021 Trust Wallet.
 //
 // This file is part of Trust. The full Trust copyright notice, including
 // terms governing use, modification, and redistribution, is contained in the
 // file LICENSE at the root of the source code distribution tree.

 import XCTest
 import WalletCore

 class MAYAChainAddressTests: XCTestCase {
     func testAddressValidation() {
         let mayachain = CoinType.mayachain
         for address in [
             "maya1uuds8pd92qnnq0udw0rpg0szpgcslc9prlsqk8",
             "maya1zf3gsk7edzwl9syyefvfhle37cjtql35n6emmw",
         ] {
             XCTAssertTrue(mayachain.validate(address: address))
             XCTAssertEqual(mayachain.address(string: address)?.description, address)
         }
     }
 }
