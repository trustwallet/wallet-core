// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class TezosAddressTests: XCTestCase {
  public func testDescription() {
    let validAddressString = "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don"
    guard let address = TezosAddress(string: validAddressString) else {
      XCTFail("Expected a valid address")
      return
    }
    XCTAssertEqual(address.description, validAddressString)
  }

  public func testValidAddress() {
    let validAddressString = "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don"
    guard let validTezosAddressData = Base58.decodeNoCheck(string: validAddressString) else {
      XCTFail("Couldn't decode address to data.")
      return
    }

    // Verify static helper methods.
    XCTAssertTrue(TezosAddress.isValidString(string: validAddressString))
    XCTAssertTrue(TezosAddress.isValid(data: validTezosAddressData))

    // Verify an address can be instantiated from the valid address.
    XCTAssertNotNil(TezosAddress(string: validAddressString))
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

  public func testAddressFromPublicKey() {
    let privateKey = PrivateKey(data: Data(hexString: "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5")!)!
    let publicKey = privateKey.getPublicKey(compressed: false)

    let address = TezosAddress(publicKey: publicKey)

    XCTAssertEqual(address.description, "tz1d1qQL3mYVuiH4JPFvuikEpFwaDm85oabM")
  }
}
