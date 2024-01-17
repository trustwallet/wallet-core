// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class BandChainTests: XCTestCase {
    let privateKey = PrivateKey(data: Data(hexString: "1037f828ca313f4c9e120316e8e9ff25e17f07fe66ba557d5bc5e2eeb7cba8f6")!)!

    func testAddress() {
        let address = CoinType.bandChain.deriveAddress(privateKey: privateKey)

        XCTAssertEqual(address, "band1jf9aaj9myrzsnmpdr7twecnaftzmku2mgms4n3")
        XCTAssertTrue(CoinType.bandChain.validate(address: "band1pnndgfwsrff86263xzpc5cd3t6yfvgjyqc87jh"))
        XCTAssertTrue(CoinType.bandChain.validate(address: "band1jf9aaj9myrzsnmpdr7twecnaftzmku2mgms4n3"))
        XCTAssertFalse(CoinType.bandChain.validate(address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02"))
    }

    func testSigningTransaction() {
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .bandChain).description

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = fromAddress
            $0.toAddress = "band1pnndgfwsrff86263xzpc5cd3t6yfvgjyqc87jh"
            $0.amounts = [CosmosAmount.with {
                $0.amount = "1000000"
                $0.denom = "uband"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "100"
                $0.denom = "uband"
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

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .bandChain)
        
        let expectedJSON: String =
"""
{
  "mode": "block",
  "tx": {
    "fee": {
      "amount": [
        {
          "amount": "100",
          "denom": "uband"
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
              "denom": "uband"
            }
          ],
          "from_address": "band1jf9aaj9myrzsnmpdr7twecnaftzmku2mgms4n3",
          "to_address": "band1pnndgfwsrff86263xzpc5cd3t6yfvgjyqc87jh"
        }
      }
    ],
    "signatures": [
      {
        "pub_key": {
          "type": "tendermint/PubKeySecp256k1",
          "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
        },
        "signature": "C6X5J08I1kkebqxa9LiFRRSJsp8U9E/IulruGTtOvpcpn/kMwWAxbFTDzvrDV5SnTWDSlimTkeZq8OuwL7j9nQ=="
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
            $0.delegatorAddress = "band13nzgys7y9c693u0pq089an4pq6q87hf9kqgkrz"
            $0.validatorAddress = "bandvaloper13fwr8rmugu2mfuurfx4sfmyv05haw9sujnqzd8"
            $0.amount = CosmosAmount.with {
                $0.amount = "1000000"
                $0.denom = "uband"
            }
        }

        let message = CosmosMessage.with {
            $0.stakeMessage = stakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "100"
                $0.denom = "uband"
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

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .bandChain)

        let expectedJSON = """
{
  "mode": "block",
  "tx": {
    "fee": {
      "amount": [
        {
          "amount": "100",
          "denom": "uband"
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
            "denom": "uband"
          },
          "delegator_address": "band13nzgys7y9c693u0pq089an4pq6q87hf9kqgkrz",
          "validator_address": "bandvaloper13fwr8rmugu2mfuurfx4sfmyv05haw9sujnqzd8"
        }
      }
    ],
    "signatures": [
      {
        "pub_key": {
          "type": "tendermint/PubKeySecp256k1",
          "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
        },
        "signature": "My+OzXqBOImtuDOoqoO9YBdlAhl6weWZvtJfkm4KDZ8I/wnQHNFBa41ql1e2LYSk3jnR/14LZ6E3pY8YW3WU9w=="
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
            $0.delegatorAddress = "band13nzgys7y9c693u0pq089an4pq6q87hf9kqgkrz"
            $0.validatorAddress = "bandvaloper13fwr8rmugu2mfuurfx4sfmyv05haw9sujnqzd8"
        }

        let message = CosmosMessage.with {
            $0.withdrawStakeRewardMessage = withdrawMessage
        }

        let fee = CosmosFee.with {
            $0.amounts = [CosmosAmount.with {
                $0.amount = "100"
                $0.denom = "uband"
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

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .bandChain)

        let expectedJSON = """
{
  "mode": "block",
  "tx": {
    "fee": {
      "amount": [
        {
          "amount": "100",
          "denom": "uband"
        }
      ],
      "gas": "200000"
    },
    "memo": "",
    "msg": [
      {
        "type": "cosmos-sdk/MsgWithdrawDelegationReward",
        "value": {
          "delegator_address": "band13nzgys7y9c693u0pq089an4pq6q87hf9kqgkrz",
          "validator_address": "bandvaloper13fwr8rmugu2mfuurfx4sfmyv05haw9sujnqzd8"
        }
      }
    ],
    "signatures": [
      {
        "pub_key": {
          "type": "tendermint/PubKeySecp256k1",
          "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
        },
        "signature": "yV2ORYqs66hq9wECEqOgLoBx7OghdRCnN8MqZk5cY/40PAm1EGjFKkdNVLNTXuBrskcT0pP/AT0XfMyywvhkWg=="
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
            $0.delegatorAddress = "band13tug898kgtwprg7fevzzqgh45draa3cyffw3kp"
            $0.validatorAddress = "bandvaloper1jp633fleakzv4uxxvl707j9u2jj6j5x2rg7glv"
            $0.amount = CosmosAmount.with {
                $0.amount = "500000"
                $0.denom = "uband"
            }
        }

        let message = CosmosMessage.with {
            $0.unstakeMessage = unstakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "100"
                $0.denom = "uband"
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

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .bandChain)

        let expectedJSON = """
{
  "mode": "block",
  "tx": {
    "fee": {
      "amount": [
        {
          "amount": "100",
          "denom": "uband"
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
            "denom": "uband"
          },
          "delegator_address": "band13tug898kgtwprg7fevzzqgh45draa3cyffw3kp",
          "validator_address": "bandvaloper1jp633fleakzv4uxxvl707j9u2jj6j5x2rg7glv"
        }
      }
    ],
    "signatures": [
      {
        "pub_key": {
          "type": "tendermint/PubKeySecp256k1",
          "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
        },
        "signature": "mdWnaIfRASZoCs0HKEk0dCL3S3ky1fbh1wp76M7Cov0D8fiByoxOfNknGgDwZecmwhZ4Gf66E+25B5hBCJpY/A=="
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
            $0.delegatorAddress = "band1hln9scsl9yqup8nxyum06rmggql5m5zqzslg52"
            $0.validatorSrcAddress = "bandvaloper1hln9scsl9yqup8nxyum06rmggql5m5zqwxmt3p"
            $0.validatorDstAddress = "bandvaloper1hydxm5h8v6tty2x623az65x3r39tl3paxyxtr0"
            $0.amount = CosmosAmount.with {
                $0.amount = "500000"
                $0.denom = "uband"
            }
        }

        let message = CosmosMessage.with {
            $0.restakeMessage = restakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "100"
                $0.denom = "uband"
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

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .bandChain)
        
        let expectedJSON = """
{
  "mode": "block",
  "tx": {
    "fee": {
      "amount": [
        {
          "amount": "100",
          "denom": "uband"
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
            "denom": "uband"
          },
          "delegator_address": "band1hln9scsl9yqup8nxyum06rmggql5m5zqzslg52",
          "validator_dst_address": "bandvaloper1hydxm5h8v6tty2x623az65x3r39tl3paxyxtr0",
          "validator_src_address": "bandvaloper1hln9scsl9yqup8nxyum06rmggql5m5zqwxmt3p"
        }
      }
    ],
    "signatures": [
      {
        "pub_key": {
          "type": "tendermint/PubKeySecp256k1",
          "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
        },
        "signature": "Y+nCZxjvrXPs++VDLiJxDQmp/59Mdwv7OEhgaH4oObtZ7N9+ZVraAiAcxJO26bbcW3cptFf88jxGpWdp6XG9Tg=="
      }
    ]
  }
}
"""
        XCTAssertJSONEqual(expectedJSON, output.json)
    }
}
