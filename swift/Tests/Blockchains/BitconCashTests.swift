// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class BitcoinCashTests: XCTestCase {

    func testExtendedKeys() {
        let wallet = HDWallet.test

        let xprv = wallet.getExtendedPrivateKey(purpose: .bip44, coin: .bitcoinCash, version: .xprv)
        let xpub = wallet.getExtendedPublicKey(purpose: .bip44, coin: .bitcoinCash, version: .xpub)

        XCTAssertEqual(xprv, "xprv9yEvwSfPanK5gLYVnYvNyF2CEWJx1RsktQtKDeT6jnCnqASBiPCvFYHFSApXv39bZbF6hRaha1kWQBVhN1xjo7NHuhAn5uUfzy79TBuGiHh")
        XCTAssertEqual(xpub, "xpub6CEHLxCHR9sNtpcxtaTPLNxvnY9SQtbcFdov22riJ7jmhxmLFvXAoLbjHSzwXwNNuxC1jUP6tsHzFV9rhW9YKELfmR9pJaKFaM8C3zMPgjw")
    }

    func testDeriveFromXPub() {
        let xpub = "xpub6CEHLxCHR9sNtpcxtaTPLNxvnY9SQtbcFdov22riJ7jmhxmLFvXAoLbjHSzwXwNNuxC1jUP6tsHzFV9rhW9YKELfmR9pJaKFaM8C3zMPgjw"

        let coin = CoinType.bitcoinCash
        let xpubAddr2 = HDWallet.getPublicKeyFromExtended(
            extended: xpub,
            coin: coin,
            derivationPath: DerivationPath(purpose: .bip44, coin: coin.slip44Id, account: 0, change: 0, address: 2).description
        )!

        let xpubAddr9 = HDWallet.getPublicKeyFromExtended(
            extended: xpub,
            coin: coin,
            derivationPath: DerivationPath(purpose: .bip44, coin: coin.slip44Id, account: 0, change: 0, address: 9).description
        )!

        XCTAssertEqual(coin.deriveAddressFromPublicKey(publicKey: xpubAddr2), "bitcoincash:qq4cm0hcc4trsj98v425f4ackdq7h92rsy6zzstrgy")
        XCTAssertEqual(coin.deriveAddressFromPublicKey(publicKey: xpubAddr9), "bitcoincash:qqyqupaugd7mycyr87j899u02exc6t2tcg9frrqnve")
    }

    func testAddress() {
        XCTAssertEqual(
            "bitcoincash:prm3srpqu4kmx00370m4wt5qr3cp7sekmcksezufmd",
            AnyAddress(string: "bitcoincash:prm3srpqu4kmx00370m4wt5qr3cp7sekmcksezufmd", coin: .bitcoinCash)?.description
        )
        XCTAssertEqual(
            "bitcoincash:prm3srpqu4kmx00370m4wt5qr3cp7sekmcksezufmd",
            AnyAddress(string: "prm3srpqu4kmx00370m4wt5qr3cp7sekmcksezufmd", coin: .bitcoinCash)?.description
        )
    }

    func testLockScript() {
        let address = AnyAddress(string: "pzukqjmcyzrkh3gsqzdcy3e3d39cqxhl3g0f405k5l", coin: .bitcoinCash)!
        let script = BitcoinScript.lockScriptForAddress(address: address.description, coin: .bitcoinCash)
        XCTAssertEqual(script.data.hexString, "a914b9604b7820876bc510009b8247316c4b801aff8a87")

        let address2 = AnyAddress(string: "qphr8l8ns8wd99a8653ctfe5qcrxaumz5qpmqlk2ex", coin: .bitcoinCash)!
        let script2 = BitcoinScript.lockScriptForAddress(address: address2.description, coin: .bitcoinCash)
        XCTAssertEqual(script2.data.hexString, "76a9146e33fcf381dcd297a7d52385a73406066ef362a088ac")
    }

    func testSign() throws {
        let utxoTxId = "050d00e2e18ef13969606f1ceee290d3f49bd940684ce39898159352952b8ce2"
        let privateKey = PrivateKey(data: Data(hexString: "7fdafb9db5bc501f2096e7d13d331dc7a75d9594af3d251313ba8b6200f4e384")!)!
        let address = CoinType.bitcoinCash.deriveAddress(privateKey: privateKey)
        let utxo = BitcoinUnspentTransaction.with {
            $0.outPoint.hash = Data.reverse(hexString: utxoTxId) // reverse of UTXO tx id, Bitcoin internal expects network byte order
            $0.outPoint.index = 2                                // outpoint index of this this UTXO
            $0.outPoint.sequence = UINT32_MAX
            $0.amount = 5151                                     // value of this UTXO
            $0.script = BitcoinScript.lockScriptForAddress(address: address, coin: .bitcoinCash).data // Build lock script from address or public key hash
        }

        let input = BitcoinSigningInput.with {
            $0.hashType = BitcoinScript.hashTypeForCoin(coinType: .bitcoinCash)
            $0.amount = 600
            $0.byteFee = 1
            $0.toAddress = "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx"
            $0.changeAddress = "1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU"
            $0.utxo = [utxo]
            $0.privateKey = [privateKey.data]
        }

        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .bitcoinCash)
        XCTAssertEqual(output.error, TW_Common_Proto_SigningError.ok)

        XCTAssertEqual(output.transactionID, "96ee20002b34e468f9d3c5ee54f6a8ddaa61c118889c4f35395c2cd93ba5bbb4")
        XCTAssertEqual(output.encoded.hexString, "0100000001e28c2b955293159898e34c6840d99bf4d390e2ee1c6f606939f18ee1e2000d05020000006b483045022100b70d158b43cbcded60e6977e93f9a84966bc0cec6f2dfd1463d1223a90563f0d02207548d081069de570a494d0967ba388ff02641d91cadb060587ead95a98d4e3534121038eab72ec78e639d02758e7860cdec018b49498c307791f785aa3019622f4ea5bffffffff0258020000000000001976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ace5100000000000001976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac00000000")
    }
}
