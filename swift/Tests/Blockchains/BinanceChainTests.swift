// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class BinanceChainTests: XCTestCase {

    func testAddress() {
        let publicKey = PublicKey(data: Data(hexString: "0x026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502")!, type: .secp256k1)!
        let address = AnyAddress(publicKey: publicKey, coin: .binance)

        XCTAssertEqual("bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2", address.description)
    }

    func testBinanceMainnet() {
        let wallet = HDWallet(mnemonic: "rabbit tilt arm protect banner ill produce vendor april bike much identify pond upset front easily glass gallery address hair priority focus forest angle", passphrase: "")
        let key = wallet.getKeyForCoin(coin: .binance)
        let address = CoinType.binance.deriveAddress(privateKey: key)

        XCTAssertEqual(key.data.hexString, "727f677b390c151caf9c206fd77f77918f56904b5504243db9b21e51182c4c06")
        XCTAssertEqual("bnb1devga6q804tx9fqrnx0vtu5r36kxgp9tmk4xkm", address.description)
    }

    func testSigning() {
        let privateKey = PrivateKey(data: Data(hexString: "95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)

        let token = BinanceSendOrder.Token.with {
            $0.denom = "BNB" // BNB or BEP2 token symbol
            $0.amount = 1
        }

        let orderInput = BinanceSendOrder.Input.with {
            $0.address = AnyAddress(publicKey: publicKey, coin: .binance).data
            $0.coins = [token]
        }

        let orderOutput = BinanceSendOrder.Output.with {
            $0.address = AnyAddress(string: "bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5", coin: .binance)!.data
            $0.coins = [token]
        }

        let input = BinanceSigningInput.with {
            $0.chainID = "Binance-Chain-Nile" // Chain id (network id),                 from /v1/node-info api
            $0.accountNumber = 0              // On chain account / address number,     from /v1/account/<address> api
            $0.sequence = 0                   // Sequence number, plus 1 for new order, from /v1/account/<address> api
            $0.source = 0                     // BEP10 source id
            $0.privateKey = privateKey.data
            $0.memo = ""
            $0.sendOrder = BinanceSendOrder.with {
                $0.inputs = [orderInput]
                $0.outputs = [orderOutput]
            }
        }

        let output: BinanceSigningOutput = AnySigner.sign(input: input, coin: .binance)

        // swiftlint:disable:next line_length
        XCTAssertEqual(output.encoded.hexString, "b801f0625dee0a462a2c87fa0a1f0a1440c2979694bbc961023d1d27be6fc4d21a9febe612070a03424e421001121f0a14bffe47abfaede50419c577f1074fee6dd1535cd112070a03424e421001126a0a26eb5ae98721026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e50212401b1181faec30b60a2ddaa2804c253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a504f9e8310679")
    }
}
