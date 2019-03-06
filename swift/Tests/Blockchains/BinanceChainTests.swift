// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class BinanceChainTests: XCTestCase {

    func testDefault() {
        let mainnet = BinanceChain()
        let testnet = BinanceChainTestnet()

        XCTAssertEqual(mainnet.hrp, .binance)
        XCTAssertEqual(testnet.hrp, .binanceTest)

    }

    func testAddress() {
        let publicKey = PublicKey(data: Data(hexString: "0x026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502")!)!
        let address = TendermintAddress(hrp: .binance, publicKey: publicKey)

        XCTAssertEqual("bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2", address?.description)
    }

    func testBinanceTestnet() {
        let blockchain = BinanceChainTestnet()
        let wallet = HDWallet(mnemonic: "rabbit tilt arm protect banner ill produce vendor april bike much identify pond upset front easily glass gallery address hair priority focus forest angle", passphrase: "")
        let key = wallet.getKey(at: blockchain.derivationPath(at: 0))
        let address = blockchain.address(for: key.getPublicKeySecp256k1(compressed: true))

        XCTAssertEqual(key.data.hexString, "727f677b390c151caf9c206fd77f77918f56904b5504243db9b21e51182c4c06")
        XCTAssertEqual("tbnb1devga6q804tx9fqrnx0vtu5r36kxgp9t4ruzk2", address.description)
    }
}
