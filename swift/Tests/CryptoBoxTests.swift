// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class CryptoBoxTests: XCTestCase {
    func testEncryptDecryptEasy() {
        let mySecret = CryptoBoxSecretKey()
        let myPubkey = mySecret.getPublicKey()
        
        let otherSecret = CryptoBoxSecretKey()
        let otherPubkey = otherSecret.getPublicKey()
        
        let message = "Well done is better than well said. -Benjamin Franklin"
        let encrypted = CryptoBox.encryptEasy(mySecret: mySecret, otherPubkey: otherPubkey, message: Data(message.utf8))
        
        // Step 2. Make sure the Box can be decrypted by the other side.
        let decrypted = CryptoBox.decryptEasy(mySecret: otherSecret, otherPubkey: myPubkey, encrypted: encrypted)!
        let decryptedStr = String(bytes: decrypted, encoding: .utf8)
        XCTAssertEqual(decryptedStr, message)
    }
    
    func testSecretKeyFromToBytes() {
        let secretBytes = Data(hexString: "dd87000d4805d6fbd89ae1352f5e4445648b79d5e901c92aebcb610e9be468e4")!
        XCTAssert(CryptoBoxSecretKey.isValid(data: secretBytes))
        let secret = CryptoBoxSecretKey(data: secretBytes)
        XCTAssertEqual(secret?.data, secretBytes)
    }
    
    func testPublicKeyFromToBytes() {
        let publicBytes = Data(hexString: "afccabc5b28a8a1fd1cd880516f9c854ae2498d0d1b978b53a59f38e4ae55747")!
        XCTAssert(CryptoBoxPublicKey.isValid(data: publicBytes))
        let pubkey = CryptoBoxPublicKey(data: publicBytes)
        XCTAssertEqual(pubkey?.data, publicBytes)
    }
}
