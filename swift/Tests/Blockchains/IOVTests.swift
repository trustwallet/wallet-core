// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
import TrustWalletCore
import XCTest

class IOVTests: XCTestCase {

    let privateKey = PrivateKey(data: Data(hexString: "1037f828ca313f4c9e120316e8e9ff25e17f07fe66ba557d5bc5e2eeb7cba8f6")!)!

    func testAddress() {
        let address = CoinType.iov.deriveAddress(privateKey: privateKey)

        XCTAssertEqual(address, "star1jf9aaj9myrzsnmpdr7twecnaftzmku2msw77u4")
        XCTAssertTrue(CoinType.iov.validate(address: "star1478t4fltj689nqu83vsmhz27quk7uggjwe96yk"))
        XCTAssertTrue(CoinType.iov.validate(address: "star1y4t33z7ugz2323vnuhjwftz33quns0t4znr7ps"))
        XCTAssertFalse(CoinType.iov.validate(address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02"))
    }

    func testSigningTransaction() {

        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .iov).description

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = fromAddress
            $0.toAddress = "star1g7sfzcxfuzqn2xtxr246h8zjcxrpvdmn8tk5gu"
            $0.amounts = [CosmosAmount.with {
                $0.amount = 1000000
                $0.denom = "uiov"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = 100
                $0.denom = "uiov"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 204
            $0.chainID = "band-wenchang-testnet2"
            $0.memo = ""
            $0.sequence = 0
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .iov)

        let expectedJSON: String =
                """
                {
                  "mode": "block",
                  "tx": {
                    "fee": {
                      "amount": [
                        {
                          "amount": "100",
                          "denom": "uiov"
                        }
                      ],
                      "gas": "200000"
                    },
                    "memo": "",
                    "msg": [
                      {
                        "type": "cosmos-sdk/MsgSend",
                        "value": {
                          "amount": [
                            {
                              "amount": "1000000",
                              "denom": "uiov"
                            }
                          ],
                          "from_address": "star1jf9aaj9myrzsnmpdr7twecnaftzmku2msw77u4",
                          "to_address": "star1g7sfzcxfuzqn2xtxr246h8zjcxrpvdmn8tk5gu"
                        }
                      }
                    ],
                    "signatures": [
                      {
                        "pub_key": {
                          "type": "tendermint/PubKeySecp256k1",
                          "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
                        },
                        "signature": "pKyoyA7ODGW9f9yFE2LDFN0sAlHmCBoV5Ek27wHO5goclCKG3M6CYgYbU1vpnKwcF5bgfLqH9Po72zRfQVR2Fg=="
                      }
                    ]
                  }
                }
                """
        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testStaking() {
        // https://scan-wenchang-testnet2.bandchain.org/tx/ca86ad13b7d3add04a926f9f6184f1134b964f4f67f39c3a7169540553119915
        let stakeMessage = CosmosMessage.Delegate.with {
            $0.delegatorAddress = "star1g7sfzcxfuzqn2xtxr246h8zjcxrpvdmn8tk5gu"
            $0.validatorAddress = "starvaloper1mw08m28g2gt62py9yufdwduz52al0wn3fmrg6z"
            $0.amount = CosmosAmount.with {
                $0.amount = 1000000
                $0.denom = "uiov"
            }
        }

        let message = CosmosMessage.with {
            $0.stakeMessage = stakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = 100
                $0.denom = "uiov"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 204
            $0.chainID = "band-wenchang-testnet2"
            $0.memo = ""
            $0.sequence = 1
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .iov)

        let expectedJSON = """
                           {
                             "mode": "block",
                             "tx": {
                               "fee": {
                                 "amount": [
                                   {
                                     "amount": "100",
                                     "denom": "uiov"
                                   }
                                 ],
                                 "gas": "200000"
                               },
                               "memo": "",
                               "msg": [
                                 {
                                   "type": "cosmos-sdk/MsgDelegate",
                                   "value": {
                                     "amount": {
                                       "amount": "1000000",
                                       "denom": "uiov"
                                     },
                                     "delegator_address": "star1g7sfzcxfuzqn2xtxr246h8zjcxrpvdmn8tk5gu",
                                     "validator_address": "starvaloper1mw08m28g2gt62py9yufdwduz52al0wn3fmrg6z"
                                   }
                                 }
                               ],
                               "signatures": [
                                 {
                                   "pub_key": {
                                     "type": "tendermint/PubKeySecp256k1",
                                     "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
                                   },
                                   "signature": "IOP3nSocf8hsI2Z+p/lqXHG6mCtioOYpFUZZTCdJMG1t5GF6HtMS8N6ZH4li83qq8Jo580arqu9jZnnpomaxMg=="
                                 }
                               ]
                             }
                           }
                           """
        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testWithdraw() {
        // https://scan-wenchang-testnet2.bandchain.org/tx/0df1a6742647224646ed0213abfc012fca54ca42311414c158a3b8799fb61d56
        let withdrawMessage = CosmosMessage.WithdrawDelegationReward.with {
            $0.delegatorAddress = "star1g7sfzcxfuzqn2xtxr246h8zjcxrpvdmn8tk5gu"
            $0.validatorAddress = "starvaloper1mw08m28g2gt62py9yufdwduz52al0wn3fmrg6z"
        }

        let message = CosmosMessage.with {
            $0.withdrawStakeRewardMessage = withdrawMessage
        }

        let fee = CosmosFee.with {
            $0.amounts = [CosmosAmount.with {
                $0.amount = 100
                $0.denom = "uiov"
            }]
            $0.gas = 200000
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 204
            $0.chainID = "band-wenchang-testnet2"
            $0.memo = ""
            $0.sequence = 2
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .iov)

        let expectedJSON = """
                           {
                             "mode": "block",
                             "tx": {
                               "fee": {
                                 "amount": [
                                   {
                                     "amount": "100",
                                     "denom": "uiov"
                                   }
                                 ],
                                 "gas": "200000"
                               },
                               "memo": "",
                               "msg": [
                                 {
                                   "type": "cosmos-sdk/MsgWithdrawDelegationReward",
                                   "value": {
                                     "delegator_address": "star1g7sfzcxfuzqn2xtxr246h8zjcxrpvdmn8tk5gu",
                                     "validator_address": "starvaloper1mw08m28g2gt62py9yufdwduz52al0wn3fmrg6z"
                                   }
                                 }
                               ],
                               "signatures": [
                                 {
                                   "pub_key": {
                                     "type": "tendermint/PubKeySecp256k1",
                                     "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
                                   },
                                   "signature": "YDUdaBMbP38OlxAaiFvHCOA1vnDkaXcuYR4ZuX9qbDtKX3zIKAEK7o1SqiyTU3WHJ1uNFukoZAz1Me8VGhIijw=="
                                 }
                               ]
                             }
                           }
                           """
        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testUndelegate() {
        // https://scan-wenchang-testnet2.bandchain.org/tx/cbf84d227f5a8d36e7b529d94b0f93e7107d3706148c79d8c539c09ce4698447
        let unstakeMessage = CosmosMessage.Undelegate.with {
            $0.delegatorAddress = "star1g7sfzcxfuzqn2xtxr246h8zjcxrpvdmn8tk5gu"
            $0.validatorAddress = "starvaloper1mw08m28g2gt62py9yufdwduz52al0wn3fmrg6z"
            $0.amount = CosmosAmount.with {
                $0.amount = 500000
                $0.denom = "uiov"
            }
        }

        let message = CosmosMessage.with {
            $0.unstakeMessage = unstakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = 100
                $0.denom = "uiov"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 204
            $0.chainID = "band-wenchang-testnet2"
            $0.memo = ""
            $0.sequence = 3
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .iov)

        let expectedJSON = """
                           {
                             "mode": "block",
                             "tx": {
                               "fee": {
                                 "amount": [
                                   {
                                     "amount": "100",
                                     "denom": "uiov"
                                   }
                                 ],
                                 "gas": "200000"
                               },
                               "memo": "",
                               "msg": [
                                 {
                                   "type": "cosmos-sdk/MsgUndelegate",
                                   "value": {
                                     "amount": {
                                       "amount": "500000",
                                       "denom": "uiov"
                                     },
                                     "delegator_address": "star1g7sfzcxfuzqn2xtxr246h8zjcxrpvdmn8tk5gu",
                                     "validator_address": "starvaloper1mw08m28g2gt62py9yufdwduz52al0wn3fmrg6z"
                                   }
                                 }
                               ],
                               "signatures": [
                                 {
                                   "pub_key": {
                                     "type": "tendermint/PubKeySecp256k1",
                                     "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
                                   },
                                   "signature": "0qv2RtnJ+5Kw3jfRC8tB9MdSIATtg7vjg0ogwGwoa2RbIYjUl+BYdTB7aXfmMfrsw+ZDhl87TGOEDNc45B67QQ=="
                                 }
                               ]
                             }
                           }
                           """
        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testRedlegate() {
        // https://scan-wenchang-testnet2.bandchain.org/tx/92b351e9aa2f7faae6cd556db04737a1fff778ea0306e2dfb064ccec76a41b13
        let restakeMessage = CosmosMessage.BeginRedelegate.with {
            $0.delegatorAddress = "star1g7sfzcxfuzqn2xtxr246h8zjcxrpvdmn8tk5gu"
            $0.validatorSrcAddress = "starvaloper1mw08m28g2gt62py9yufdwduz52al0wn3fmrg6z"
            $0.validatorDstAddress = "starvaloper1mw08m28g2gt62py9yufdwduz52al0wn3fmrg6z"
            $0.amount = CosmosAmount.with {
                $0.amount = 500000
                $0.denom = "uiov"
            }
        }

        let message = CosmosMessage.with {
            $0.restakeMessage = restakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = 100
                $0.denom = "uiov"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 204
            $0.chainID = "band-wenchang-testnet2"
            $0.memo = ""
            $0.sequence = 4
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .iov)

        let expectedJSON = """
                           {
                             "mode": "block",
                             "tx": {
                               "fee": {
                                 "amount": [
                                   {
                                     "amount": "100",
                                     "denom": "uiov"
                                   }
                                 ],
                                 "gas": "200000"
                               },
                               "memo": "",
                               "msg": [
                                 {
                                   "type": "cosmos-sdk/MsgBeginRedelegate",
                                   "value": {
                                     "amount": {
                                       "amount": "500000",
                                       "denom": "uiov"
                                     },
                                     "delegator_address": "star1g7sfzcxfuzqn2xtxr246h8zjcxrpvdmn8tk5gu",
                                     "validator_dst_address": "starvaloper1mw08m28g2gt62py9yufdwduz52al0wn3fmrg6z",
                                     "validator_src_address": "starvaloper1mw08m28g2gt62py9yufdwduz52al0wn3fmrg6z"
                                   }
                                 }
                               ],
                               "signatures": [
                                 {
                                   "pub_key": {
                                     "type": "tendermint/PubKeySecp256k1",
                                     "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
                                   },
                                   "signature": "atHXVoNIa6hA7PcG1z/5t16kMhDykuBSf5o+wXfwTz8Cp3KnNm9idL/BDjuTkXZE8rCzjTmchGzTC7c4vFU6lg=="
                                 }
                               ]
                             }
                           }
                           """
        XCTAssertJSONEqual(expectedJSON, output.json)
    }
}
