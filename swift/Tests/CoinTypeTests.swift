// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class CoinTypeTests: XCTestCase {

    func testCoinType() {
        XCTAssertEqual(CoinType.bitcoin.rawValue, 0)
        XCTAssertEqual(CoinType.litecoin.rawValue, 2)
        XCTAssertEqual(CoinType.tron.rawValue, 195)
        XCTAssertEqual(CoinType.ethereum.rawValue, 60)
        XCTAssertEqual(CoinType.thunderCore.rawValue, 1001)
        XCTAssertEqual(CoinType.wanchain.rawValue, 5718350)
        XCTAssertEqual(CoinType.callisto.rawValue, 820)
        XCTAssertEqual(CoinType.ethereumClassic.rawValue, 61)
        XCTAssertEqual(CoinType.goChain.rawValue, 6060)
        XCTAssertEqual(CoinType.ontology.rawValue, 1024)
        XCTAssertEqual(CoinType.poanetwork.rawValue, 178)
        XCTAssertEqual(CoinType.veChain.rawValue, 818)
        XCTAssertEqual(CoinType.icon.rawValue, 74)
        XCTAssertEqual(CoinType.viction.rawValue, 889)
        XCTAssertEqual(CoinType.tezos.rawValue, 1729)
        XCTAssertEqual(CoinType.qtum.rawValue, 2301)
        XCTAssertEqual(CoinType.nebulas.rawValue, 2718)
        XCTAssertEqual(CoinType.avalancheCChain.rawValue, 10009000)
        XCTAssertEqual(CoinType.xdai.rawValue, 10000100)
    }
    
    func testCoinDerivation() {
        XCTAssertEqual(CoinType.bitcoin.derivationPath(), "m/84'/0'/0'/0/0")
        XCTAssertEqual(CoinType.bitcoin.derivationPathWithDerivation(derivation: Derivation.bitcoinLegacy), "m/44'/0'/0'/0/0")
        XCTAssertEqual(CoinType.solana.derivationPathWithDerivation(derivation: Derivation.solanaSolana), "m/44'/501'/0'/0'")
    }
    
    func testDeriveAddressFromPublicKeyAndDerivation() {
        let pkData = Data(hexString: "0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798")!
        let publicKey = PublicKey(data: pkData, type: .secp256k1)!
        
        let address = CoinType.bitcoin.deriveAddressFromPublicKeyAndDerivation(publicKey: publicKey, derivation: Derivation.bitcoinSegwit)
        XCTAssertEqual(address, "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4")
    }
}
