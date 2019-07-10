// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class IOSTTests: XCTestCase {

    let iost = CoinType.iost

    func testDeriveIOST() {
        let wallet = HDWallet.test
        let key = wallet.getKeyForCoin(coin: iost)
        let pubKey = key.getPublicKeyEd25519()
        let base58Key = Base58.encodeNoCheck(data: key.data + pubKey.data)
        XCTAssertEqual(base58Key, "3MuzPF34N1KwgAVZkqqEtWWjyHk6LoncmYsyxVkRqrLGhzHbyhwWjhJKjmynU4Zn5xHeUN9TE2BENWPvsQTSxqoZ")
        let address = iost.deriveAddress(privateKey: key)
        XCTAssertEqual(address, Base58.encodeNoCheck(data: pubKey.data).description)
        XCTAssertEqual(address.description, "CxxfGeYbUmXgvBiDMqTvqWXZmeTVoPWBie3jbaAoMLNf")
    }

    func testSign() {
        let amountLimit = IOSTAmountLimit.with {
            $0.token = "*"
            $0.value = "unlimited"
        }

        let tx = IOSTTransaction.with {
            $0.time = 1550137587000000000
            $0.expiration = $0.time + Int64(1000000000) * 300
            $0.gasRatio = 1
            $0.gasLimit = 1000000
            $0.chainID = 1024
            $0.amountLimit = [amountLimit]
        }

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

        let output = IOSTSigner.sign(input: input)
        XCTAssertEqual(output.transaction.publisherSigs[0].signature.hexString, "e8ce15214bad39683021c15dd318e963da8541fd8f3d8484df5042b4ea7fdafb7f46505b85841367d6e1736c7d3b433ca72089b88a23f43661dfb0429a10cb03")
    }
}
