// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class CosmosAddressTests: XCTestCase {
    func testAddressValidation() {
        let cosmos = CoinType.cosmos
        for address in [
            "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02",
            "cosmospub1addwnpepqftjsmkr7d7nx4tmhw4qqze8w39vjq364xt8etn45xqarlu3l2wu2n7pgrq",
            "cosmosvaloper1sxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08",
            "cosmosvalconspub1zcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa",
        ] {
            XCTAssertTrue(cosmos.validate(address: address))
            XCTAssertEqual(cosmos.address(string: address)?.description, address)
        }
    }
}

class CosmosSignerTests: XCTestCase {

    let privateKey = PrivateKey(data: Data(hexString: "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005")!)!

    func testSigningTransaction() {
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .cosmos)

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = fromAddress.description
            $0.toAddress = "cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"
            $0.amounts = [CosmosAmount.with {
                $0.amount = 1
                $0.denom = "muon"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = 200
                $0.denom = "muon"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 1037
            $0.chainID = "gaia-13003"
            $0.memo = ""
            $0.sequence = 8
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .cosmos)

        let expectedJSON: String =
"""
{
  "mode": "block",
  "tx": {
    "fee": {
      "amount": [
        {
          "amount": "200",
          "denom": "muon"
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
              "amount": "1",
              "denom": "muon"
            }
          ],
          "from_address": "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02",
          "to_address": "cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"
        }
      }
    ],
    "signatures": [
      {
        "pub_key": {
          "type": "tendermint/PubKeySecp256k1",
          "value": "AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"
        },
        "signature": "/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg=="
      }
    ]
  }
}
"""

        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testStaking() {
        let stakeMessage = CosmosMessage.Delegate.with {
            $0.delegatorAddress = "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02"
            $0.validatorAddress = "cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp"
            $0.amount = CosmosAmount.with {
                $0.amount = 10
                $0.denom = "muon"
            }
        }

        let message = CosmosMessage.with {
            $0.stakeMessage = stakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 101721
            $0.amounts = [CosmosAmount.with {
                $0.amount = 1018
                $0.denom = "muon"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 1037
            $0.chainID = "gaia-13003"
            $0.memo = ""
            $0.sequence = 7
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .cosmos)

        let expectedJSON = """
{
  "mode": "block",
  "tx": {
    "fee": {
      "amount": [
        {
          "amount": "1018",
          "denom": "muon"
        }
      ],
      "gas": "101721"
    },
    "memo": "",
    "msg": [
      {
        "type": "cosmos-sdk/MsgDelegate",
        "value": {
          "amount": {
            "amount": "10",
            "denom": "muon"
          },
          "delegator_address": "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02",
          "validator_address": "cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp"
        }
      }
    ],
    "signatures": [
      {
        "pub_key": {
          "type": "tendermint/PubKeySecp256k1",
          "value": "AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"
        },
        "signature": "wIvfbCsLRCjzeXXoXTKfHLGXRbAAmUp0O134HVfVc6pfdVNJvvzISMHRUHgYcjsSiFlLyR32heia/yLgMDtIYQ=="
      }
    ]
  }
}

"""
        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testWithdraw() {
        // https://stargate.cosmos.network/txs/DE2FF0BC39E70DB576DF86C263A5BDB42FF3D5D5B914A4A30E8C13B14A950FFF
        let delegator = "cosmos100rhxclqasy6vnrcervgh99alx5xw7lkfp4u54"
        let validators = [
            "cosmosvaloper1ey69r37gfxvxg62sh4r0ktpuc46pzjrm873ae8",
            "cosmosvaloper1sjllsnramtg3ewxqwwrwjxfgc4n4ef9u2lcnj0",
            "cosmosvaloper1648ynlpdw7fqa2axt0w2yp3fk542junl7rsvq6"
        ]

        let withdrawals = validators.map { validator in
            CosmosMessage.WithdrawDelegationReward.with {
                $0.delegatorAddress = delegator
                $0.validatorAddress = validator
            }
        }.map { withdraw in
            CosmosMessage.with {
                $0.withdrawStakeRewardMessage = withdraw
            }
        }

        let fee = CosmosFee.with {
            $0.amounts = [CosmosAmount.with {
                $0.denom = "uatom"
                $0.amount = 1
            }]
            $0.gas = 220000
        }

        let input = CosmosSigningInput.with {
            $0.fee = fee
            $0.accountNumber = 8698
            $0.chainID = "cosmoshub-2"
            $0.sequence = 318
            $0.messages = withdrawals
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .cosmos)

        let expectedJSON = """
        {
          "mode": "block",
          "tx": {
            "fee": {
              "amount": [
                {
                  "amount": "1",
                  "denom": "uatom"
                }
              ],
              "gas": "220000"
            },
            "memo": "",
            "msg": [
              {
                "type": "cosmos-sdk/MsgWithdrawDelegationReward",
                "value": {
                  "delegator_address": "cosmos100rhxclqasy6vnrcervgh99alx5xw7lkfp4u54",
                  "validator_address": "cosmosvaloper1ey69r37gfxvxg62sh4r0ktpuc46pzjrm873ae8"
                }
              },{
                "type": "cosmos-sdk/MsgWithdrawDelegationReward",
                "value": {
                  "delegator_address": "cosmos100rhxclqasy6vnrcervgh99alx5xw7lkfp4u54",
                  "validator_address": "cosmosvaloper1sjllsnramtg3ewxqwwrwjxfgc4n4ef9u2lcnj0"
                }
              },{
                "type": "cosmos-sdk/MsgWithdrawDelegationReward",
                "value": {
                  "delegator_address": "cosmos100rhxclqasy6vnrcervgh99alx5xw7lkfp4u54",
                  "validator_address": "cosmosvaloper1648ynlpdw7fqa2axt0w2yp3fk542junl7rsvq6"
                }
              }
            ],
            "signatures": [
              {
                "pub_key": {
                  "type": "tendermint/PubKeySecp256k1",
                  "value": "AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"
                },
                "signature": "2k5bSnfWxaauXHBNJTKmf4CpLiCWLg7UAC/q2SVhZNkU+n0DdLBSTdmYhKYmmtpl/Njm4YrcxE0WLb/hVccQ+g=="
              }
            ]
          }
        }

        """
        XCTAssertJSONEqual(expectedJSON, output.json)
    }
}
