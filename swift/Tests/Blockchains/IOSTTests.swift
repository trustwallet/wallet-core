// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class IOSTTests: XCTestCase {
    func testAddress() {
        let hex = "14d8c92fceefdc967f1f6cbace65a9f0740bfe45742a478cc0c04cca341cfb4e"
        let string = "2QNt4yU73VfwLyGdKj5yirtRJn5Zy7pB3KAGyUiTSfmP"
        let pubKey = PublicKey(data: Data(hexString: hex)!, type: .ed25519)!
        let address = AnyAddress(publicKey: pubKey, coin: .iost)
        let expected = AnyAddress(string: string, coin: .iost)!

        XCTAssertEqual(address, expected)
        XCTAssertEqual(expected.data.hexString, hex)
    }

    func testAccount() {
        let account = IOSTAccount(string: "iosttest")!
        XCTAssertEqual(account.description, "iosttest")
    }

    func testSign() {
        var tx = IOSTTransaction()
        tx.time = 1550137587000000000
        tx.expiration = tx.time + Int64(1000000000) * 300
        tx.gasRatio = 1
        tx.gasLimit = 1000000
        tx.chainID = 1024

        var amountLimit = IOSTAmountLimit()
        amountLimit.token = "*"
        amountLimit.value = "unlimited"
        tx.amountLimit = [amountLimit]

        let secKeyData = Data(hexString: "63095105a37b4e896e5ebbd740e751c6f9df7cca2410beba3261dc5680299cebe812b52ea9ad5cba9a9af03afcc6f2942a4524b0df3c0344dc195072831670c4")!

        let account = IOSTAccountInfo.with {
            $0.activeKey = secKeyData
            $0.name = "myname"
        }
        let input = IOSTSigningInput.with {
            $0.account = account
            $0.transactionTemplate = tx
            $0.transferDestination = "admin"
            $0.transferAmount = "10"
            $0.transferMemo = ""
        }
        let output: IOSTSigningOutput = AnySigner.sign(input: input, coin: .iost)
        XCTAssertEqual(output.transaction.publisherSigs[0].signature.hexString, "e8ce15214bad39683021c15dd318e963da8541fd8f3d8484df5042b4ea7fdafb7f46505b85841367d6e1736c7d3b433ca72089b88a23f43661dfb0429a10cb03")

    }
}
