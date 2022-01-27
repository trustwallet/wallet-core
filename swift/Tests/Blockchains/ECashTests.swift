// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class ECashTests: XCTestCase {

    func testExtendedKeys() {
        let wallet = HDWallet.test

        let xprv = wallet.getExtendedPrivateKey(purpose: .bip44, coin: .ecash, version: .xprv)
        let xpub = wallet.getExtendedPublicKey(purpose: .bip44, coin: .ecash, version: .xpub)

        XCTAssertEqual(xprv, "xprv9xjBcTizebJaV61xMkuMJ89vis7saMmwFgTYeF83KwinEksJ4frk7wB4mDiKiwXDCbJmgmh6Bp1FkF8SopNZhbF3B5wyX32cuDVFZtuUDvB")
        XCTAssertEqual(xpub, "xpub6BiY1yFtUxrsha6RTnSMfG6fGtxMypVncuP9SdXetHFm7ZCScDAzfjVYcW32bkNCGJ5DTqawAHSTbJdTBL8wVxqUDGpxnRtukrhhBoS7Wy7")
    }

    func testDeriveFromXPub() {
        let xpub = "xpub6BiY1yFtUxrsha6RTnSMfG6fGtxMypVncuP9SdXetHFm7ZCScDAzfjVYcW32bkNCGJ5DTqawAHSTbJdTBL8wVxqUDGpxnRtukrhhBoS7Wy7"

        let coin = CoinType.ecash
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

        XCTAssertEqual(coin.deriveAddressFromPublicKey(publicKey: xpubAddr2), "ecash:qpttymfhuq3v8tasfv7drlglhq6ne6zxquqltu3dcj")
        XCTAssertEqual(coin.deriveAddressFromPublicKey(publicKey: xpubAddr9), "ecash:qqjraw2s5pwqwzql4znjpvp4vtvy3c9gmugq62r2j7")
    }

    func testAddress() {
        XCTAssertEqual(
            "ecash:prm3srpqu4kmx00370m4wt5qr3cp7sekmc0adf8na6",
            AnyAddress(string: "ecash:prm3srpqu4kmx00370m4wt5qr3cp7sekmc0adf8na6", coin: .ecash)?.description
        )
        XCTAssertEqual(
            "ecash:prm3srpqu4kmx00370m4wt5qr3cp7sekmc0adf8na6",
            AnyAddress(string: "prm3srpqu4kmx00370m4wt5qr3cp7sekmc0adf8na6", coin: .ecash)?.description
        )
    }

    func testLockScript() {
        let address = AnyAddress(string: "pzukqjmcyzrkh3gsqzdcy3e3d39cqxhl3gkypy0vjg", coin: .ecash)!
        let script = BitcoinScript.lockScriptForAddress(address: address.description, coin: .ecash)
        XCTAssertEqual(script.data.hexString, "a914b9604b7820876bc510009b8247316c4b801aff8a87")

        let address2 = AnyAddress(string: "qphr8l8ns8wd99a8653ctfe5qcrxaumz5qck55dsl3", coin: .ecash)!
        let script2 = BitcoinScript.lockScriptForAddress(address: address2.description, coin: .ecash)
        XCTAssertEqual(script2.data.hexString, "76a9146e33fcf381dcd297a7d52385a73406066ef362a088ac")
    }

    func testSign() throws {
        let utxoTxId = "050d00e2e18ef13969606f1ceee290d3f49bd940684ce39898159352952b8ce2"
        let privateKey = PrivateKey(data: Data(hexString: "7fdafb9db5bc501f2096e7d13d331dc7a75d9594af3d251313ba8b6200f4e384")!)!
        let address = CoinType.ecash.deriveAddress(privateKey: privateKey)
        let utxo = BitcoinUnspentTransaction.with {
            $0.outPoint.hash = Data.reverse(hexString: utxoTxId) // reverse of UTXO tx id, Bitcoin internal expects network byte order
            $0.outPoint.index = 2                                // outpoint index of this this UTXO
            $0.outPoint.sequence = UINT32_MAX
            $0.amount = 5151                                     // value of this UTXO
            $0.script = BitcoinScript.lockScriptForAddress(address: address, coin: .ecash).data // Build lock script from address or public key hash
        }

        let input = BitcoinSigningInput.with {
            $0.hashType = BitcoinScript.hashTypeForCoin(coinType: .ecash)
            $0.amount = 600
            $0.byteFee = 1
            $0.toAddress = "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx"
            $0.changeAddress = "1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU"
            $0.utxo = [utxo]
            $0.privateKey = [privateKey.data]
        }

        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .ecash)
        XCTAssertEqual(output.error, TW_Common_Proto_SigningError.ok)

        XCTAssertEqual(output.transactionID, "96ee20002b34e468f9d3c5ee54f6a8ddaa61c118889c4f35395c2cd93ba5bbb4")
        XCTAssertEqual(output.encoded.hexString, "0100000001e28c2b955293159898e34c6840d99bf4d390e2ee1c6f606939f18ee1e2000d05020000006b483045022100b70d158b43cbcded60e6977e93f9a84966bc0cec6f2dfd1463d1223a90563f0d02207548d081069de570a494d0967ba388ff02641d91cadb060587ead95a98d4e3534121038eab72ec78e639d02758e7860cdec018b49498c307791f785aa3019622f4ea5bffffffff0258020000000000001976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ace5100000000000001976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac00000000")
    }
}
