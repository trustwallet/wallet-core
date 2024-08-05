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
}
