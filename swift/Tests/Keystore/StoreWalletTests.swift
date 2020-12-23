// Copyright © 2019-2020 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class StoreWalletTests: XCTestCase {
    let password = "password"
    let walletPath = "testwallet"

    func createWallet() -> StoreWallet {
        let keyStore = StoredKey(name: "name", password: Data(password.utf8), mnemonic: "team engine square letter hero song dizzy scrub tornado fabric divert saddle")
        let path = "./" + walletPath
        return StoreWallet(path: path, key: keyStore)
    }
    
    func testCreate() {
        let wallet = createWallet()
        XCTAssertEqual(walletPath, wallet.indentifier)
    }
    
    func testGetAccount() {
        let wallet = createWallet()
        let coin = CoinType.bitcoin
        let account = wallet.getAccount(password: password, coin: coin)
        XCTAssertEqual(coin, account.coin)
        XCTAssertEqual("bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny", account.address)
    }
    
    func testGetPrivateKey() {
        let wallet = createWallet()
        let coin = CoinType.bitcoin
        let privateKey = wallet.privateKey(password: password, coin: coin)
        XCTAssertEqual("d2568511baea8dc347f14c4e0479eb8ebe29eb5f664ed796e755896250ffd11f", privateKey.data.hexString)
    }
}
