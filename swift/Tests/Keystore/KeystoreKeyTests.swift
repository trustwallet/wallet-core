// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class KeystoreKeyTests: XCTestCase {
    func testReadWallet() {
        let url = Bundle(for: type(of: self)).url(forResource: "key", withExtension: "json")!
        let key = StoredKey.load(path: url.path)!

        XCTAssertEqual(key.identifier, "e13b209c-3b2f-4327-bab0-3bef2e51630d")
    }

    func testReadMyEtherWallet() {
        let url = Bundle(for: type(of: self)).url(forResource: "myetherwallet", withExtension: "uu")!

        XCTAssertNotNil(StoredKey.load(path: url.path))
    }

    func testInvalidPassword() {
        let url = Bundle(for: type(of: self)).url(forResource: "key", withExtension: "json")!
        let key = StoredKey.load(path: url.path)!

        XCTAssertNil(key.decryptPrivateKey(password: "password"))
    }

    func testDecrypt() {
        let url = Bundle(for: type(of: self)).url(forResource: "key", withExtension: "json")!
        let key = StoredKey.load(path: url.path)!
        let privateKey = key.decryptPrivateKey(password: "testpassword")!

        XCTAssertEqual(privateKey.hexString, "7a28b5ba57c53603b0b07b56bba752f7784bf506fa95edc395f5cf6c7514fe9d")
    }

    func testCreateWallet() {
        let privateKeyData = Data(hexString: "3a1076bf45ab87712ad64ccb3b10217737f7faacbf2872e88fdd9a537d8fe266")!
        let key = StoredKey.importPrivateKey(privateKey: privateKeyData, name: "name", password: "password", coin: .ethereum)!
        let decrypted = key.decryptPrivateKey(password: "password")!

        XCTAssertEqual(decrypted.hexString, privateKeyData.hexString)
    }

    func testDecodingEthereumAddress() {
        let url = Bundle(for: type(of: self)).url(forResource: "key", withExtension: "json")!
        let key = StoredKey.load(path: url.path)!
        let account = key.accountForCoin(coin: .ethereum, wallet: nil)!

        XCTAssertEqual(account.address.description, "0x008AeEda4D805471dF9b2A5B0f38A0C3bCBA786b")
    }

    func testDecodingBitcoinAddress() {
        let url = Bundle(for: type(of: self)).url(forResource: "key_bitcoin", withExtension: "json")!
        let key = StoredKey.load(path: url.path)!
        let account = key.accountForCoin(coin: .bitcoin, wallet: nil)!

        XCTAssertEqual(account.address.description, "3PWazDi9n1Hfyq9gXFxDxzADNL8RNYyK2y")
    }
}
