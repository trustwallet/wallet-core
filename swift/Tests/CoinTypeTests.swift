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
        XCTAssertEqual(CoinType.pactus.rawValue, 21888)
    }

    func testCoinDerivation() {
        XCTAssertEqual(CoinType.bitcoin.derivationPath(), "m/84'/0'/0'/0/0")
        XCTAssertEqual(CoinType.bitcoin.derivationPathWithDerivation(derivation: Derivation.bitcoinLegacy), "m/44'/0'/0'/0/0")
        XCTAssertEqual(CoinType.bitcoin.derivationPathWithDerivation(derivation: Derivation.bitcoinTaproot), "m/86'/0'/0'/0/0")
        XCTAssertEqual(CoinType.solana.derivationPathWithDerivation(derivation: Derivation.solanaSolana), "m/44'/501'/0'/0'")
        XCTAssertEqual(CoinType.pactus.derivationPathWithDerivation(derivation: Derivation.pactusMainnet), "m/44'/21888'/3'/0'")
        XCTAssertEqual(CoinType.pactus.derivationPathWithDerivation(derivation: Derivation.pactusTestnet), "m/44'/21777'/3'/0'")
    }

    func testDeriveAddressFromPublicKeyAndDerivationBitcoin() {
        let pkData = Data(hexString: "0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798")!
        let publicKey = PublicKey(data: pkData, type: .secp256k1)!

        let address = CoinType.bitcoin.deriveAddressFromPublicKeyAndDerivation(publicKey: publicKey, derivation: Derivation.bitcoinSegwit)
        XCTAssertEqual(address, "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4")
    }

    func testDeriveAddressFromPublicKeyAndDerivationPactus() {
        let pkData = Data(hexString: "95794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa")!
        let publicKey = PublicKey(data: pkData, type: .ed25519)!

        let mainnet_address = CoinType.pactus.deriveAddressFromPublicKeyAndDerivation(publicKey: publicKey, derivation: Derivation.default)
        XCTAssertEqual(mainnet_address, "pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr")

        let testnet_address = CoinType.pactus.deriveAddressFromPublicKeyAndDerivation(publicKey: publicKey, derivation: Derivation.pactusTestnet)
        XCTAssertEqual(testnet_address, "tpc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymzqkcrg")
    }
}
