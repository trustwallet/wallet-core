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

    func testSignJSON() {
        let json = """
        {
            "chainId": "Binance-Chain-Tigris",
            "accountNumber": "13186",
            "source": "2",
            "memo": "Testing",
            "sendOrder": {
                "inputs": [{
                    "address": "EuZU7e+eUIuDNzaph9Bp2lqJrts=",
                    "coins": [{
                        "denom": "BNB",
                        "amount": "1345227"
                    }]
                }],
                "outputs": [{
                    "address": "M7vzB7mBRvE9IGk8+UbC13pMryg=",
                    "coins": [{
                        "denom": "BNB",
                        "amount": "1345227"
                    }]
                }]
            }
        }
        """
        let key = Data(hexString: "f947b3554a1c2fa70e1caa9de53fbda353348d1e856c593848f3a29737d31f11")!
        let result = AnySigner.signJSON(json, key: key, coin: .binance)

        XCTAssertEqual(result, "ca01f0625dee0a4a2a2c87fa0a210a1412e654edef9e508b833736a987d069da5a89aedb12090a03424e4210cb8d5212210a1433bbf307b98146f13d20693cf946c2d77a4caf2812090a03424e4210cb8d52126d0a26eb5ae9872102e58176f271a9796b4288908be85094a2ac978e25535fd59a37b58626e3a84d9e1240015b4beb3d6ef366a7a92fd283f66b8f0d8cdb6b152a9189146b27f84f507f047e248517cf691a36ebc2b7f3b7f64e27585ce1c40f1928d119c31af428efcf3e1882671a0754657374696e672002")
    }
}
