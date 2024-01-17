// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class KavaTests: XCTestCase {

    let privateKey = PrivateKey(data: Data(hexString: "1037f828ca313f4c9e120316e8e9ff25e17f07fe66ba557d5bc5e2eeb7cba8f6")!)!

    func testAddress() {
        let address = CoinType.kava.deriveAddress(privateKey: privateKey)

        XCTAssertEqual(address, "kava1jf9aaj9myrzsnmpdr7twecnaftzmku2mdpy2a7")
        XCTAssertTrue(CoinType.kava.validate(address: "kava1hdp298kaz0eezpgl6scsykxljrje3667hmlv0h"))
        XCTAssertTrue(CoinType.kava.validate(address: "kavavaloper17498ffqdj49zca4jm7mdf3eevq7uhcsgjvm0uk"))
        XCTAssertFalse(CoinType.kava.validate(address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02"))
    }

    func testSigningTransaction() {
        // https://kava.mintscan.io/txs/2988DF83FCBFAA38179D583A96734CBD071541D6768221BB23111BC8136D5E6A
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .kava).description

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = fromAddress
            $0.toAddress = "kava1hdp298kaz0eezpgl6scsykxljrje3667hmlv0h"
            $0.amounts = [CosmosAmount.with {
                $0.amount = "1000000"
                $0.denom = "ukava"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "100"
                $0.denom = "ukava"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 204
            $0.chainID = "kava-2"
            $0.memo = ""
            $0.sequence = 0
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .kava)

        let expectedJSON: String =
"""
{
    "mode": "block",
    "tx": {
        "msg": [{
            "type": "cosmos-sdk/MsgSend",
            "value": {
                "from_address": "kava1jf9aaj9myrzsnmpdr7twecnaftzmku2mdpy2a7",
                "to_address": "kava1hdp298kaz0eezpgl6scsykxljrje3667hmlv0h",
                "amount": [{
                    "denom": "ukava",
                    "amount": "1000000"
                }]
            }
        }],
        "fee": {
            "amount": [{
                "denom": "ukava",
                "amount": "100"
            }],
            "gas": "200000"
        },
        "signatures": [{
            "pub_key": {
                "type": "tendermint/PubKeySecp256k1",
                "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
            },
            "signature": "vcZiNGhw2qmVClI8uQDJhwfFuEDYunZ9XW01FOuOjBcGCSdmK/IOwiD616+ARYtpLZsBX8p2Y+cqbHEascIjCg=="
        }],
        "memo": ""
    }
}
"""
        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testStaking() {
        // https://kava.mintscan.io/txs/57958B4C00963186888BFA8BBB71CDF0EF3B2EA8E321164CB2DD7D44043803A2
        let stakeMessage = CosmosMessage.Delegate.with {
            $0.delegatorAddress = "kava1jf9aaj9myrzsnmpdr7twecnaftzmku2mdpy2a7"
            $0.validatorAddress = "kavavaloper17498ffqdj49zca4jm7mdf3eevq7uhcsgjvm0uk"
            $0.amount = CosmosAmount.with {
                $0.amount = "1000000"
                $0.denom = "ukava"
            }
        }

        let message = CosmosMessage.with {
            $0.stakeMessage = stakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "100"
                $0.denom = "ukava"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 204
            $0.chainID = "kava-2"
            $0.memo = ""
            $0.sequence = 1
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .kava)

        let expectedJSON = """
{
    "mode": "block",
    "tx": {
        "fee": {
            "amount": [{
                "amount": "100",
                "denom": "ukava"
            }],
            "gas": "200000"
        },
        "memo": "",
        "msg": [{
            "type": "cosmos-sdk/MsgDelegate",
            "value": {
                "amount": {
                    "amount": "1000000",
                    "denom": "ukava"
                },
                "delegator_address": "kava1jf9aaj9myrzsnmpdr7twecnaftzmku2mdpy2a7",
                "validator_address": "kavavaloper17498ffqdj49zca4jm7mdf3eevq7uhcsgjvm0uk"
            }
        }],
        "signatures": [{
            "pub_key": {
                "type": "tendermint/PubKeySecp256k1",
                "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
            },
            "signature": "SmgctT4b8yt4/L2qOkW+Jy2SxMflIl6gOFfhKX/o+Cpqa6xCF6rqy0p3WSz0c/vnfZTWVG7rX+612xCVcJjQCg=="
        }]
    }
}
"""
        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testWithdraw() {
        // https://kava.mintscan.io/txs/C268E414EC4DC47A9909B518C4EB68CCA3BC7A8CE1566B909534BA6D216663D2
        let withdrawMessage = CosmosMessage.WithdrawDelegationReward.with {
            $0.delegatorAddress = "kava1jf9aaj9myrzsnmpdr7twecnaftzmku2mdpy2a7"
            $0.validatorAddress = "kavavaloper17498ffqdj49zca4jm7mdf3eevq7uhcsgjvm0uk"
        }

        let message = CosmosMessage.with {
            $0.withdrawStakeRewardMessage = withdrawMessage
        }

        let fee = CosmosFee.with {
            $0.amounts = [CosmosAmount.with {
                $0.amount = "100"
                $0.denom = "ukava"
            }]
            $0.gas = 200000
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 204
            $0.chainID = "kava-2"
            $0.memo = ""
            $0.sequence = 2
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .kava)

        let expectedJSON = """
{
    "mode": "block",
    "tx": {
        "fee": {
            "amount": [{
                "amount": "100",
                "denom": "ukava"
            }],
            "gas": "200000"
        },
        "memo": "",
        "msg": [{
            "type": "cosmos-sdk/MsgWithdrawDelegationReward",
            "value": {
                "delegator_address": "kava1jf9aaj9myrzsnmpdr7twecnaftzmku2mdpy2a7",
                "validator_address": "kavavaloper17498ffqdj49zca4jm7mdf3eevq7uhcsgjvm0uk"
            }
        }],
        "signatures": [{
            "pub_key": {
                "type": "tendermint/PubKeySecp256k1",
                "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
            },
            "signature": "xF4I6dKOjuKR7ii3i+Q754/R9IFHULwrkNG1zO8w0VoZBQhNWITg3jZJwpgDnAyl6C7we7BuLhutHaSIhvutEg=="
        }]
    }
}
"""
        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testUndelegate() {
      // https://kava.mintscan.io/txs/7C1F50CFFB42E91329000494036E3ED59A9F7180A261DD3C2BDCCA1D13D33D59
      let unstakeMessage = CosmosMessage.Undelegate.with {
            $0.delegatorAddress = "kava1jf9aaj9myrzsnmpdr7twecnaftzmku2mdpy2a7"
            $0.validatorAddress = "kavavaloper17498ffqdj49zca4jm7mdf3eevq7uhcsgjvm0uk"
            $0.amount = CosmosAmount.with {
                $0.amount = "500000"
                $0.denom = "ukava"
            }
        }

        let message = CosmosMessage.with {
            $0.unstakeMessage = unstakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "100"
                $0.denom = "ukava"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 204
            $0.chainID = "kava-2"
            $0.memo = ""
            $0.sequence = 3
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .kava)

        let expectedJSON = """
{
    "mode": "block",
    "tx": {
        "fee": {
            "amount": [{
                "amount": "100",
                "denom": "ukava"
            }],
            "gas": "200000"
        },
        "memo": "",
        "msg": [{
            "type": "cosmos-sdk/MsgUndelegate",
            "value": {
                "amount": {
                    "amount": "500000",
                    "denom": "ukava"
                },
                "delegator_address": "kava1jf9aaj9myrzsnmpdr7twecnaftzmku2mdpy2a7",
                "validator_address": "kavavaloper17498ffqdj49zca4jm7mdf3eevq7uhcsgjvm0uk"
            }
        }],
        "signatures": [{
            "pub_key": {
                "type": "tendermint/PubKeySecp256k1",
                "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
            },
            "signature": "p6n0wv1L4YrK6Etmpp6cLjmUO3CNE1p6/LkZF73tastw4x0sW4Jg8b9R5x45EQMVbmNP87wZ4X/6VHUXluHstQ=="
        }]
    }
}
"""
        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testRedlegate() {
      // https://kava.mintscan.io/txs/ACBDAB9BBBBA9BE09F21F728334BBF3E1F14DFCBC0DB1C2175EAD07D4C1B6230
      let restakeMessage = CosmosMessage.BeginRedelegate.with {
            $0.delegatorAddress = "kava1jf9aaj9myrzsnmpdr7twecnaftzmku2mdpy2a7"
            $0.validatorSrcAddress = "kavavaloper17498ffqdj49zca4jm7mdf3eevq7uhcsgjvm0uk"
            $0.validatorDstAddress = "kavavaloper14fkp35j5nkvtztmxmsxh88jks6p3w8u7p76zs9"
            $0.amount = CosmosAmount.with {
                $0.amount = "500000"
                $0.denom = "ukava"
            }
        }

        let message = CosmosMessage.with {
            $0.restakeMessage = restakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "100"
                $0.denom = "ukava"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 204
            $0.chainID = "kava-2"
            $0.memo = ""
            $0.sequence = 4
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .kava)
        let expectedJSON = """
{
    "mode": "block",
    "tx": {
        "fee": {
            "amount": [{
                "amount": "100",
                "denom": "ukava"
            }],
            "gas": "200000"
        },
        "memo": "",
        "msg": [{
            "type": "cosmos-sdk/MsgBeginRedelegate",
            "value": {
                "amount": {
                    "amount": "500000",
                    "denom": "ukava"
                },
                "delegator_address": "kava1jf9aaj9myrzsnmpdr7twecnaftzmku2mdpy2a7",
                "validator_dst_address": "kavavaloper14fkp35j5nkvtztmxmsxh88jks6p3w8u7p76zs9",
                "validator_src_address": "kavavaloper17498ffqdj49zca4jm7mdf3eevq7uhcsgjvm0uk"
            }
        }],
        "signatures": [{
            "pub_key": {
                "type": "tendermint/PubKeySecp256k1",
                "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
            },
            "signature": "mDC0NuQJOJoYcbDE96D2kQy2RlodlGrhedKOX/2xAy0WrK2TzxDAYbV822xIhLtr2T67jpZfzDWNI0iRg2j/5Q=="
        }]
    }
}
"""
        XCTAssertJSONEqual(expectedJSON, output.json)
    }
}
