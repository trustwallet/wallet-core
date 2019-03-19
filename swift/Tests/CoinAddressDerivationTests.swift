// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class CoinAddressDerivationTests: XCTestCase {

    func testDerive() {
        let wallet = HDWallet(mnemonic: "shoot island position soft burden budget tooth cruel issue economy destroy above", passphrase: "")

        for _ in 0..<4 {
            for coin in CoinType.allCases {
                let privateKey = wallet.getKeyForCoin(coin: coin)
                let address = coin.deriveAddress(privateKey: privateKey)
                switch coin {
                case .aion:
                    XCTAssertEqual("0xa0629f34c9ea4757ad0b275628d4d02e3db6c9009ba2ceeba76a5b55fb2ca42e", address)
                case .binance:
                    XCTAssertEqual("tbnb12vtaxl9952zm6rwf7v8jerq74pvaf77fkw9xhl", address)
                case .cosmos:
                    XCTAssertEqual("cosmos142j9u5eaduzd7faumygud6ruhdwme98qsy2ekn", address)
                case .bitcoin:
                    XCTAssertEqual("bc1quvuarfksewfeuevuc6tn0kfyptgjvwsvrprk9d", address)
                case .bitcoinCash:
                    XCTAssertEqual("bitcoincash:qpzl3jxkzgvfd9flnd26leud5duv795fnv7vuaha70", address)
                case .callisto:
                    XCTAssertEqual("0x3E6FFC80745E6669135a76F4A7ce6BCF02436e04", address)
                case .dash:
                    XCTAssertEqual("XqHiz8EXYbTAtBEYs4pWTHh7ipEDQcNQeT", address)
                case .ethereum:
                    XCTAssertEqual("0x8f348F300873Fd5DA36950B2aC75a26584584feE", address)
                case .ethereumClassic:
                    XCTAssertEqual("0x078bA3228F3E6C08bEEac9A005de0b7e7089aD1c", address)
                case .go:
                    XCTAssertEqual("0x5940ce4A14210d4Ccd0ac206CE92F21828016aC2", address)
                case .icon:
                    XCTAssertEqual("hx18b380b53c23dc4ee9f6666bc20d1be02f3fe106", address)
                case .litecoin:
                    XCTAssertEqual("ltc1qhd8fxxp2dx3vsmpac43z6ev0kllm4n53t5sk0u", address)
                case .ontology:
                    XCTAssertEqual("AHKTnybvnWo3TeY8uvNXekvYxMrXogUjeT", address)
                case .poa:
                    XCTAssertEqual("0xe8a3e8bE17E172B6926130eAfB521e9D2849aca9", address)
                case .ripple:
                    XCTAssertEqual("rPwE3gChNKtZ1mhH3Ko8YFGqKmGRWLWXV3", address)
                case .tezos:
                    XCTAssertEqual("tz1acnY9VbMagps26Kj3RfoGRWD9nYG5qaRX", address)
                case .thunderToken:
                    XCTAssertEqual("0x4b92b3ED6d8b24575Bf5ce4C6a86ED261DA0C8d7", address)
                case .tomoChain:
                    XCTAssertEqual("0xC74b6D8897cBa9A4b659d43fEF73C9cA852cE424", address)
                case .tron:
                    XCTAssertEqual("TQ5NMqJjhpQGK7YJbESKtNCo86PJ89ujio", address)
                case .veChain:
                    XCTAssertEqual("0x1a553275dF34195eAf23942CB7328AcF9d48c160", address)
                case .wanChain:
                    XCTAssertEqual("0xD5ca90b928279FE5D06144136a25DeD90127aC15", address)
                case .xdai:
                    XCTAssertEqual("0x364d0551599B97EAf997bc06c8c40Aaf73124402", address)
                case .zcash:
                    XCTAssertEqual("t3ZEo7WrSBB1o1DCUytPsnUfoEoZn2qL5er", address)
                case .zcoin:
                    XCTAssertEqual("aEd5XFChyXobvEics2ppAqgK3Bgusjxtik", address)
                case .nimiq:
                    XCTAssertEqual("NQ76 7AVR EHDA N05U X7SY XB14 XJU7 8ERV GM6H", address)
                case .stellar:
                    XCTAssertEqual("GA3H6I4C5XUBYGVB66KXR27JV5KS3APSTKRUWOIXZ5MVWZKVTLXWKZ2P", address)
                case .neo:
                    XCTAssertEqual("AUa3rzbGJe7MbHf8Kra9em8oaLBL1MDYKF", address)
                case .kin:
                    XCTAssertEqual("GBL3MT2ICHHM5OJ2QJ44CAHGDK6MLPINVXBKOKLHGBWQDVRWTWQ7U2EA", address)
                }
            }
        }
    }
}
