// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

/// Static constants for tests.
extension String {
  public static let validTezosAddress = "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don"
}

class TezosAddressTests: XCTestCase {
  public func testDescription() {
    guard let address = TezosAddress(string: .validTezosAddress) else {
      XCTFail("Expected a valid address")
      return
    }
    XCTAssertEqual(address.description, .validTezosAddress)
  }

  public func testValidAddress() {
    guard let validTezosAddressData = Base58.decodeNoCheck(string: .validTezosAddress) else {
      XCTFail("Couldn't decode address to data.")
      return
    }

    // Verify static helper methods.
    XCTAssertTrue(TezosAddress.isValidString(string: .validTezosAddress))
    XCTAssertTrue(TezosAddress.isValid(data: validTezosAddressData))

    // Verify an address can be instantiated from the valid address.
    XCTAssertNotNil(TezosAddress(string: .validTezosAddress))
  }

  public func testInvalidAddresses() {
    let invalidAddresses = [
      "NmH7tmeJUmHcncBDvpr7aJNEBk7rp5zYsB1qt", // Invalid prefix, valid checksum
      "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3AAAA", // Valid prefix, invalid checksum
      "1tzeZwq8b5cvE2bPKokatLkVMzkxz24zAAAAA"  // Invalid prefix, invalid checksum
    ]

    for invalidAddress in invalidAddresses {
      guard let invalidTezosAddressData = Base58.decodeNoCheck(string: invalidAddress) else {
        XCTFail("Couldn't decode address to data.")
        return
      }

      // Verify static helper methods.
      XCTAssertFalse(TezosAddress.isValidString(string: invalidAddress))
      XCTAssertFalse(TezosAddress.isValid(data: invalidTezosAddressData))

      // Verify an address cannot be instantiated from the valid address.
      XCTAssertNil(TezosAddress(string: invalidAddress))
    }
  }
}
