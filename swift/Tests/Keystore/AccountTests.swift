// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class AccountTests: XCTestCase {
    let words = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal"
    let password = "password"

    func testSignHash() throws {
        let privateKeyData = Data(hexString: "D30519BCAE8D180DBFCC94FE0B8383DC310185B0BE97B4365083EBCECCD75759")!
        let key = StoredKey.importPrivateKey(privateKey: privateKeyData, name: "name", password: password, coin: .ethereum)!
        let wallet = Wallet(keyURL: URL(fileURLWithPath: "/"), key: key)

        let hash = Data(hexString: "3F891FDA3704F0368DAB65FA81EBE616F4AA2A0854995DA4DC0B59D2CADBD64F")!
        let privateKey = PrivateKey(data: wallet.key.decryptPrivateKey(password: password)!)!
        let result = privateKey.sign(digest: hash, curve: .secp256k1)!

        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        XCTAssertEqual(result.count, 65)
        XCTAssertTrue(publicKey.verify(signature: result, message: hash))
    }

    func testSignHashHD() throws {
        let key = StoredKey.importHDWallet(mnemonic: words, name: "name", password: password, coin: .ethereum)!
        let wallet = Wallet(keyURL: URL(fileURLWithPath: "/"), key: key)

        let hash = Data(hexString: "3F891FDA3704F0368DAB65FA81EBE616F4AA2A0854995DA4DC0B59D2CADBD64F")!
        let hdwallet = wallet.key.wallet(password: password)!
        let privateKey = hdwallet.getKeyForCoin(coin: .ethereum)
        let result = privateKey.sign(digest: hash, curve: .secp256k1)!

        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        XCTAssertEqual(result.count, 65)
        XCTAssertTrue(publicKey.verify(signature: result, message: hash))
    }

    func testExtendedPubkey() throws {
        let key = StoredKey.importHDWallet(mnemonic: words, name: "name", password: password, coin: .ethereum)!
        let wallet = Wallet(keyURL: URL(fileURLWithPath: "/"), key: key)
        _ = try wallet.getAccount(password: password, coin: .bitcoin)
        _ = try wallet.getAccount(password: password, coin: .bitcoinCash)
        _ = try wallet.getAccount(password: password, coin: .ethereumClassic)

        XCTAssertEqual(wallet.accounts[0].extendedPublicKey, "")
        XCTAssertEqual(wallet.accounts[1].extendedPublicKey, "zpub6rNUNtxSa9Gxvm4Bdxf1MPMwrvkzwDx6vP96Hkzw3jiQKdg3fhXBStxjn12YixQB8h88B3RMSRscRstf9AEVaYr3MAqVBEWBDuEJU4PGaT9")
        XCTAssertEqual(wallet.accounts[2].extendedPublicKey, "xpub6DCGEUmR2vcLbecgHEDN8ksNiEnanDUo3kNiT8XEHoiFCW2UWJLizkzwciXYJFzNQsFDbsJMwtGTJAkCgAQSdKTfCAmDu87rReGC9vcZVNH")
        XCTAssertEqual(wallet.accounts[3].extendedPublicKey, "")
    }

    func testBTCPrivateKeyWithPaths() throws {
        let key = StoredKey.importHDWallet(mnemonic: words, name: "name", password: password, coin: .bitcoin)!
        let wallet = Wallet(keyURL: URL(fileURLWithPath: "/"), key: key)

        let hdwallet = wallet.key.wallet(password: password)!
        let privateKey = hdwallet.getKeyForCoin(coin: .bitcoin)

        XCTAssertEqual(privateKey.data.hexString, "b511e175dc474c810ad567557a13a29f9e82576990d5f36dab342dd2d00fb5c4")
    }

    func testBCHPrivateKeyWithPaths() throws {
        let key = StoredKey.importHDWallet(mnemonic: words, name: "name", password: password, coin: .bitcoinCash)!
        let wallet = Wallet(keyURL: URL(fileURLWithPath: "/"), key: key)

        let hdwallet = wallet.key.wallet(password: password)!
        let privateKey = hdwallet.getKeyForCoin(coin: .bitcoinCash)

        XCTAssertEqual(privateKey.data.hexString, "04b02272b75eaee0b7f1a96d667cb4629b400e2152a841c6791f802b336a8af8")
    }
}
