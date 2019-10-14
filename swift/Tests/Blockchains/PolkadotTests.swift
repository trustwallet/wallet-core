//
//  PolkadotTests.swift
//  TrustWalletCoreTests
//
//  Created by Aliksandr Andrashuk on 10/16/19.
//

import XCTest
import TrustWalletCore

class PolkadotTests: XCTestCase {
    func testValidAddress() {
        XCTAssertTrue(PolkadotAddress.isValidString(string: "DMZFfVn8yxNXroTXzuoRoVKnsitJJx4jnS3DoaficA4woPL"))
        XCTAssertTrue(PolkadotAddress.isValidString(string: "FM4N1JNemHSJAnW8V99ijj7ncHSQ5X9fDs38pqkZZfW62Pr"))
    }
    
    func testInvalidAddress() {
        XCTAssertFalse(PolkadotAddress.isValidString(string: "non-valid-address"))
        XCTAssertFalse(PolkadotAddress.isValidString(string: "FM5N1JNemHSJAnW8V99ijj7ncHSQ5X9fDs38pqkZZfW62Pr"))
    }
    
    func testAddressFromString() {
        let addrString = "DtbB3Uy8zX9khHRf7SsSVk7esY5AngKvC9EJVhpjSzza2MU"
        let address = PolkadotAddress(string: addrString)
        XCTAssertEqual(addrString, address!.description)
    }
    
    func testAddressFromPublicKey() {
        let keyData = Data(hexString: "5cc8df138a881319a75ab3a4f9621cf4fe614948f45a87118093edfab1679c64")
        let publicKey = PublicKey(data: keyData!, type: PublicKeyType.ed25519)
        let address = PolkadotAddress(publicKey: publicKey!)
        XCTAssertEqual(address.description, "136f5StL4ANS6oAdjCWZzEhdCXPcd5P3ZB4LtHnFBbWYv27r")
    }

    func testAddressFromPrivateKey() {
        let keyData = Data(hexString: "ae92f15b17ad7a306f9e3e24d293d3988e625990c716ca099b623611695f02e0")
        let privateKey = PrivateKey(data: keyData!)
        let publicKey = privateKey!.getPublicKeyEd25519()
        let address = PolkadotAddress(publicKey: publicKey)
        XCTAssertEqual(address.description, "136f5StL4ANS6oAdjCWZzEhdCXPcd5P3ZB4LtHnFBbWYv27r")
    }

    func testInvalidKeyType() {
        let keyData = Data(hexString: "5cc8df138a881319a75ab3a4f9621cf4fe614948f45a87118093edfab1679c64")
        let publicKey = PublicKey(data: keyData!, type: PublicKeyType.nist256p1)
        if let pbKey = publicKey {
            let address = PolkadotAddress(publicKey: pbKey)
            XCTAssertNotEqual(address.description, "136f5StL4ANS6oAdjCWZzEhdCXPcd5P3ZB4LtHnFBbWYv27r")
        }
        else {
            XCTAssertTrue(true)
        }
    }
}
