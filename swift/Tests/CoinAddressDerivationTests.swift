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
                let derivedAddress = coin.deriveAddress(privateKey: privateKey)
                let address = coin.address(string: derivedAddress)

                switch coin {
                case .aion:
                    let expectedResult = "0xa0629f34c9ea4757ad0b275628d4d02e3db6c9009ba2ceeba76a5b55fb2ca42e"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .binance:
                    let expectedResult = "bnb12vtaxl9952zm6rwf7v8jerq74pvaf77fcmvzhw"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .cosmos:
                    let expectedResult = "cosmos142j9u5eaduzd7faumygud6ruhdwme98qsy2ekn"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .bitcoin:
                    let expectedResult = "bc1quvuarfksewfeuevuc6tn0kfyptgjvwsvrprk9d"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .bitcoinCash:
                    let expectedResult = "bitcoincash:qpzl3jxkzgvfd9flnd26leud5duv795fnv7vuaha70"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .callisto:
                    let expectedResult = "0x3E6FFC80745E6669135a76F4A7ce6BCF02436e04"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .dash:
                    let expectedResult = "XqHiz8EXYbTAtBEYs4pWTHh7ipEDQcNQeT"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .dogecoin:
                    let expectedResult = "DJRFZNg8jkUtjcpo2zJd92FUAzwRjitw6f"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .ellaism:
                    let expectedResult = "0x1Ae593CAdb8BAC47D0394617396650474c303F9C"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .ethereum:
                    let expectedResult = "0x8f348F300873Fd5DA36950B2aC75a26584584feE"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .ethereumClassic:
                    let expectedResult = "0x078bA3228F3E6C08bEEac9A005de0b7e7089aD1c"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .ethersocial:
                    let expectedResult = "0x182dd55D97C1F8D3781CDaDFC275948Ac38Ea1fe"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .goChain:
                    let expectedResult = "0x5940ce4A14210d4Ccd0ac206CE92F21828016aC2"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .icon:
                    let expectedResult = "hx18b380b53c23dc4ee9f6666bc20d1be02f3fe106"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .litecoin:
                    let expectedResult = "ltc1qhd8fxxp2dx3vsmpac43z6ev0kllm4n53t5sk0u"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .ontology:
                    let expectedResult = "AHKTnybvnWo3TeY8uvNXekvYxMrXogUjeT"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .poanetwork:
                    let expectedResult = "0xe8a3e8bE17E172B6926130eAfB521e9D2849aca9"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .xrp:
                    let expectedResult = "rPwE3gChNKtZ1mhH3Ko8YFGqKmGRWLWXV3"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .tezos:
                    let expectedResult = "tz1acnY9VbMagps26Kj3RfoGRWD9nYG5qaRX"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .thunderToken:
                    let expectedResult = "0x4b92b3ED6d8b24575Bf5ce4C6a86ED261DA0C8d7"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .tomoChain:
                    let expectedResult = "0xC74b6D8897cBa9A4b659d43fEF73C9cA852cE424"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .tron:
                    let expectedResult = "TQ5NMqJjhpQGK7YJbESKtNCo86PJ89ujio"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .veChain:
                    let expectedResult = "0x1a553275dF34195eAf23942CB7328AcF9d48c160"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .wanchain:
                    let expectedResult = "0xd5CA90B928279fe5d06144136A25dEd90127Ac15"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .xdai:
                    let expectedResult = "0x364d0551599B97EAf997bc06c8c40Aaf73124402"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .zcash:
                    let expectedResult = "t1YYnByMzdGhQv3W3rnjHMrJs6HH4Y231gy"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .zcoin:
                    let expectedResult = "aEd5XFChyXobvEics2ppAqgK3Bgusjxtik"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .nimiq:
                    let expectedResult = "NQ76 7AVR EHDA N05U X7SY XB14 XJU7 8ERV GM6H"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .stellar:
                    let expectedResult = "GA3H6I4C5XUBYGVB66KXR27JV5KS3APSTKRUWOIXZ5MVWZKVTLXWKZ2P"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .nano:
                    let expectedResult = "nano_39gsbcishxn3n7wd17ono4otq5wazwzusqgqigztx73wbrh5jwbdbshfnumc"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .neo:
                    let expectedResult = "AUa3rzbGJe7MbHf8Kra9em8oaLBL1MDYKF"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .kin:
                    let expectedResult = "GBL3MT2ICHHM5OJ2QJ44CAHGDK6MLPINVXBKOKLHGBWQDVRWTWQ7U2EA"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .decred:
                    let expectedResult = "DsidJiDGceqHTyqiejABy1ZQ3FX4SiWZkYG"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .theta:
                    let expectedResult = "0x0d1fa20c218Fec2f2C55d52aB267940485fa5DA4"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .groestlcoin:
                    let expectedResult = "grs1qexwmshts5pdpeqglkl39zyl6693tmfwp0cue4j"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .iocoin:
                    let expectedResult = "ieCTzUZhc8a5T8Y8suosPw7eMgTtN5PE1H"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .iost:
                    let expectedResult = "EKRQPgX7nKt8hJABwm9m3BKWGj7kcSECkJnCBauHQWin"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .lux:
                    let expectedResult = "LYL6SZG8S6dyXRFT8Bw4FHUoVef3cWCoPi"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .viacoin:
                    let expectedResult = "via1qnmsgjd6cvfprnszdgmyg9kewtjfgqflz67wwhc"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .qtum:
                    let expectedResult = "QhceuaTdeCZtcxmVc6yyEDEJ7Riu5gWFoF"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .nuls:
                    let expectedResult = "NsdtNvsfmPerWk4BhcapHTB3LptF8Sbe"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .bravoCoin:
                    let expectedResult = "BRV8g4c7Uve8NNsqDMeADLqPeFyohMQ7d9MzarSE23KogTS71BaiQ"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .steem:
                    let expectedResult = "STM6WSusVTidc1e7TbLjhqQPYctbsndTRwXHpi82gMuJyKEkJVLvg"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .eos:
                    let expectedResult = "EOS6hs8sRvGSzuQtq223zwJipMzqTJpXUVjyvHPvPwBSZWWrJTJkg"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .ioTeX:
                    let expectedResult = "io1qw9cccecw09q7p5kzyqtuhfhvah2mhfrc84jfk"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .zilliqa:
                    let expectedResult = "0xDdb41006F7B6FA8e5FBF06A71c01F789FeBC66e8"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .semux:
                    let expectedResult = "0xfe604170382452f77bc922bc19eb4b53504b09c2"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .dexon:
                    let expectedResult = "0x6F3E6a6dDf2C2B4B32B8Bb452eA3F36B2BB489BF"
                  AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                case .zelcash:
                    let expectedResult = "t1UKbRPzL4WN8Rs8aZ8RNiWoD2ftCMHKGUf"
                    AssetCoinDerivation(coin, expectedResult, derivedAddress, address)
                }
            }
        }
    }

    private func AssetCoinDerivation(_ coin: CoinType, _ expected: String, _ derivedAddress: String, _ address: Address?) {
        XCTAssertNotNil(address, "\(coin) is not implemented CoinType.address(string: String)")
        XCTAssertEqual(expected, derivedAddress, "\(coin) failed to match address")
        XCTAssertEqual(expected, address?.description, "\(coin) is not implemented CoinType.address(string: String)")
    }
}
