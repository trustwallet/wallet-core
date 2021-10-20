// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

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
            $0.signingMode = .protobuf;
            $0.accountNumber = 1037
            $0.chainID = "gaia-13003"
            $0.memo = ""
            $0.sequence = 8
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .cosmos)

        XCTAssertEqual(output.serialized.hexString, "0a8c010a89010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e6412690a2d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b796778306570683664643032122d636f736d6f73317a743530617a7570616e716c66616d356166687633686578777975746e756b656834633537331a090a046d756f6e12013112650a500a460a1f2f636f736d6f732e63727970746f2e736563703235366b312e5075624b657912230a210257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc512040a020801180812110a0b0a046d756f6e120332303010c09a0c1a40f9e1f4001657a42009c4eb6859625d2e41e961fc72efd2842909c898e439fc1f549916e4ecac676ee353c7d54c5ae30a29b4210b8bff0ebfdcb375e105002f47")
        XCTAssertEqual(output.serializedBase64, "CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhItY29zbW9zMXp0NTBhenVwYW5xbGZhbTVhZmh2M2hleHd5dXRudWtlaDRjNTczGgkKBG11b24SATESZQpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAgSEQoLCgRtdW9uEgMyMDAQwJoMGkD54fQAFlekIAnE62hZYl0uQelh/HLv0oQpCciY5Dn8H1SZFuTsrGdu41PH1Uxa4woptCELi/8Ov9yzdeEFAC9H")
        XCTAssertEqual(output.error, "")
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
            $0.signingMode = .protobuf;
            $0.accountNumber = 1037
            $0.chainID = "gaia-13003"
            $0.memo = ""
            $0.sequence = 7
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .cosmos)

        XCTAssertEqual(output.serialized.hexString, "0a9b010a98010a232f636f736d6f732e7374616b696e672e763162657461312e4d736744656c656761746512710a2d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b7967783065706836646430321234636f736d6f7376616c6f706572317a6b757072383368727a6b6e33757035656c6b747a63713374756674386e78736d77647167701a0a0a046d756f6e1202313012660a500a460a1f2f636f736d6f732e63727970746f2e736563703235366b312e5075624b657912230a210257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc512040a020801180712120a0c0a046d756f6e12043130313810d99a061a40f0ef499bf90be996b6237a680ece6fa4ca3060980dbd808905153fbf1023b3494d658b2ae34aa94dbc0e4db3918c903952343a6ae738d2feae0854f8ab8cfeb8")
        XCTAssertEqual(output.serializedBase64, "CpsBCpgBCiMvY29zbW9zLnN0YWtpbmcudjFiZXRhMS5Nc2dEZWxlZ2F0ZRJxCi1jb3Ntb3MxaHNrNmpyeXlxamZocDVkaGM1NXRjOWp0Y2t5Z3gwZXBoNmRkMDISNGNvc21vc3ZhbG9wZXIxemt1cHI4M2hyemtuM3VwNWVsa3R6Y3EzdHVmdDhueHNtd2RxZ3AaCgoEbXVvbhICMTASZgpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAcSEgoMCgRtdW9uEgQxMDE4ENmaBhpA8O9Jm/kL6Za2I3poDs5vpMowYJgNvYCJBRU/vxAjs0lNZYsq40qpTbwOTbORjJA5UjQ6auc40v6uCFT4q4z+uA==")
        XCTAssertEqual(output.error, "")
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
            $0.signingMode = .json;
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
