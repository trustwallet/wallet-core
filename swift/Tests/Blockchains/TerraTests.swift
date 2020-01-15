// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class TerraTests: XCTestCase {

    let privateKey = PrivateKey(data: Data(hexString: "1037f828ca313f4c9e120316e8e9ff25e17f07fe66ba557d5bc5e2eeb7cba8f6")!)!

    func testAddress() {
        let address = CoinType.terra.deriveAddress(privateKey: privateKey)

        XCTAssertEqual(address, "terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe")
        XCTAssertTrue(CoinType.terra.validate(address: "terra1hdp298kaz0eezpgl6scsykxljrje3667d233ms"))
        XCTAssertTrue(CoinType.terra.validate(address: "terravaloper1pdx498r0hrc2fj36sjhs8vuhrz9hd2cw0yhqtk"))
        XCTAssertFalse(CoinType.terra.validate(address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02"))
    }

    func testRawJSON() {
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .terra).description

        let message = CosmosMessage.with {
            $0.rawJsonMessage = CosmosMessage.RawJSON.with {
                $0.type = "bank/MsgSend"
                $0.value = """
                    {
                        "amount": [{
                            "amount": "1000000",
                            "denom": "uluna"
                        }],
                        "from_address": "\(fromAddress)",
                        "to_address": "terra1hdp298kaz0eezpgl6scsykxljrje3667d233ms"
                    }
                """
            }
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = 3000
                $0.denom = "uluna"
            }]
        }

        let signingInput = CosmosSigningInput.with {
            $0.accountNumber = 158
            $0.chainID = "soju-0013"
            $0.memo = ""
            $0.sequence = 0
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output = CosmosSigner.sign(input: signingInput)

        let expectedJSON: String =
"""
{
    "mode": "block",
    "tx": {
        "msg": [{
            "type": "bank/MsgSend",
            "value": {
                "from_address": "terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe",
                "to_address": "terra1hdp298kaz0eezpgl6scsykxljrje3667d233ms",
                "amount": [{
                    "denom": "uluna",
                    "amount": "1000000"
                }]
            }
        }],
        "fee": {
            "amount": [{
                "denom": "uluna",
                "amount": "3000"
            }],
            "gas": "200000"
        },
        "signatures": [{
            "pub_key": {
                "type": "tendermint/PubKeySecp256k1",
                "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
            },
            "signature": "KPdiVsKpY12JG/VKEJVa/FpMKclxlS0qNNG6VOAypj10R5vY5UX5IgRJET1zNYnH0wvcXxfNXV+s8jtwN2UXiQ=="
        }],
        "memo": ""
    }
}
"""
        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testSigningTransaction() {
        // https://finder.terra.money/soju-0013/tx/1403B07F2D218BCE961CB92D83377A924FEDB54C1F0B62E25C8B93B63470EBF7
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .terra).description

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = CosmosMessage.Send.with {
                $0.fromAddress = fromAddress
                $0.toAddress = "terra1hdp298kaz0eezpgl6scsykxljrje3667d233ms"
                $0.amounts = [CosmosAmount.with {
                    $0.amount = 1000000
                    $0.denom = "uluna"
                }]
                $0.typePrefix = "bank/MsgSend"
            }
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = 3000
                $0.denom = "uluna"
            }]
        }

        let signingInput = CosmosSigningInput.with {
            $0.accountNumber = 158
            $0.chainID = "soju-0013"
            $0.memo = ""
            $0.sequence = 0
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output = CosmosSigner.sign(input: signingInput)

        let expectedJSON: String =
"""
{
    "mode": "block",
    "tx": {
        "msg": [{
            "type": "bank/MsgSend",
            "value": {
                "from_address": "terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe",
                "to_address": "terra1hdp298kaz0eezpgl6scsykxljrje3667d233ms",
                "amount": [{
                    "denom": "uluna",
                    "amount": "1000000"
                }]
            }
        }],
        "fee": {
            "amount": [{
                "denom": "uluna",
                "amount": "3000"
            }],
            "gas": "200000"
        },
        "signatures": [{
            "pub_key": {
                "type": "tendermint/PubKeySecp256k1",
                "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
            },
            "signature": "KPdiVsKpY12JG/VKEJVa/FpMKclxlS0qNNG6VOAypj10R5vY5UX5IgRJET1zNYnH0wvcXxfNXV+s8jtwN2UXiQ=="
        }],
        "memo": ""
    }
}
"""
        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testStaking() {
        // https://finder.terra.money/soju-0013/tx/4C0A6690ECB601ACB42D3ECAF4C24C0555B5E32E45B09C3B1607B144CD191F87
        let stakeMessage = CosmosMessage.Delegate.with {
            $0.delegatorAddress = "terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe"
            $0.validatorAddress = "terravaloper1pdx498r0hrc2fj36sjhs8vuhrz9hd2cw0yhqtk"
            $0.amount = CosmosAmount.with {
                $0.amount = 1000000
                $0.denom = "uluna"
            }
            $0.typePrefix = "staking/MsgDelegate"
        }

        let message = CosmosMessage.with {
            $0.stakeMessage = stakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = 3000
                $0.denom = "uluna"
            }]
        }

        let signingInput = CosmosSigningInput.with {
            $0.accountNumber = 158
            $0.chainID = "soju-0013"
            $0.memo = ""
            $0.sequence = 1
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output = CosmosSigner.sign(input: signingInput)

        let expectedJSON = """
{
    "mode": "block",
    "tx": {
        "fee": {
            "amount": [{
                "amount": "3000",
                "denom": "uluna"
            }],
            "gas": "200000"
        },
        "memo": "",
        "msg": [{
            "type": "staking/MsgDelegate",
            "value": {
                "amount": {
                    "amount": "1000000",
                    "denom": "uluna"
                },
                "delegator_address": "terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe",
                "validator_address": "terravaloper1pdx498r0hrc2fj36sjhs8vuhrz9hd2cw0yhqtk"
            }
        }],
        "signatures": [{
            "pub_key": {
                "type": "tendermint/PubKeySecp256k1",
                "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
            },
            "signature": "F8UJxbkqa0j6dYTk8PymrudBKI3WYhZImRxMFCw0ecFCmPGgNTg7yfpKZo6K6JtnoJaP7bQ4db5e4wnhMCJyAQ=="
        }]
    }
}
"""
        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testWithdraw() {
        // https://finder.terra.money/soju-0013/tx/AE0E4F2B254449950A3A7F41FABCE0B3C846D70F809380313CE3BB323E490BBD
        let withdrawMessage = CosmosMessage.WithdrawDelegationReward.with {
            $0.delegatorAddress = "terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe"
            $0.validatorAddress = "terravaloper1pdx498r0hrc2fj36sjhs8vuhrz9hd2cw0yhqtk"
            $0.typePrefix = "distribution/MsgWithdrawDelegationReward"
        }

        let message = CosmosMessage.with {
            $0.withdrawStakeRewardMessage = withdrawMessage
        }

        let fee = CosmosFee.with {
            $0.amounts = [CosmosAmount.with {
                $0.amount = 3000
                $0.denom = "uluna"
            }]
            $0.gas = 200000
        }

        let signingInput = CosmosSigningInput.with {
            $0.accountNumber = 158
            $0.chainID = "soju-0013"
            $0.memo = ""
            $0.sequence = 2
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output = CosmosSigner.sign(input: signingInput)
        let expectedJSON = """
{
    "mode": "block",
    "tx": {
        "fee": {
            "amount": [{
                "amount": "3000",
                "denom": "uluna"
            }],
            "gas": "200000"
        },
        "memo": "",
        "msg": [{
            "type": "distribution/MsgWithdrawDelegationReward",
            "value": {
                "delegator_address": "terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe",
                "validator_address": "terravaloper1pdx498r0hrc2fj36sjhs8vuhrz9hd2cw0yhqtk"
            }
        }],
        "signatures": [{
            "pub_key": {
                "type": "tendermint/PubKeySecp256k1",
                "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
            },
            "signature": "Kfwi1uJplzLucXDyQZsJI9v8lMFJFUBLD46+MpwBwYwPJgqPRzSOfyjRpmNou0G/Qe1hbsGEgqb85FQpsgLz+g=="
        }]
    }
}
"""
        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testUndelegate() {
      // https://finder.terra.money/soju-0013/tx/FCF50C180303AECA97F916D0CE0E0937BA4C4D2F6777FFF2AA0D52A9DAF9CCBA
      let unstakeMessage = CosmosMessage.Undelegate.with {
            $0.delegatorAddress = "terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe"
            $0.validatorAddress = "terravaloper1pdx498r0hrc2fj36sjhs8vuhrz9hd2cw0yhqtk"
            $0.amount = CosmosAmount.with {
                $0.amount = 500000
                $0.denom = "uluna"
            }
            $0.typePrefix = "staking/MsgUndelegate"
        }

        let message = CosmosMessage.with {
            $0.unstakeMessage = unstakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = 3000
                $0.denom = "uluna"
            }]
        }

        let signingInput = CosmosSigningInput.with {
            $0.accountNumber = 158
            $0.chainID = "soju-0013"
            $0.memo = ""
            $0.sequence = 3
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output = CosmosSigner.sign(input: signingInput)

        let expectedJSON = """
{
    "mode": "block",
    "tx": {
        "fee": {
            "amount": [{
                "amount": "3000",
                "denom": "uluna"
            }],
            "gas": "200000"
        },
        "memo": "",
        "msg": [{
            "type": "staking/MsgUndelegate",
            "value": {
                "amount": {
                    "amount": "500000",
                    "denom": "uluna"
                },
                "delegator_address": "terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe",
                "validator_address": "terravaloper1pdx498r0hrc2fj36sjhs8vuhrz9hd2cw0yhqtk"
            }
        }],
        "signatures": [{
            "pub_key": {
                "type": "tendermint/PubKeySecp256k1",
                "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
            },
            "signature": "THf/RxsBr2EhHE2OMHLXfv+qSP9ORbvHgo4OSOS2P95xxGH73wW+t1zIl9cGlIVvcoChwaCg5/iEuvbgXUWpNw=="
        }]
    }
}
"""
        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testRedlegate() {
      // https://finder.terra.money/soju-0013/tx/36CE381BDF72AD7407EEE3859E3349F83B723BE9AD407E9D8C38DEE0C4434D29
      let restakeMessage = CosmosMessage.BeginRedelegate.with {
            $0.delegatorAddress = "terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe"
            $0.validatorSrcAddress = "terravaloper1pdx498r0hrc2fj36sjhs8vuhrz9hd2cw0yhqtk"
            $0.validatorDstAddress = "terravaloper1rhrptnx87ufpv62c7ngt9yqlz2hr77xr9nkcr9"
            $0.amount = CosmosAmount.with {
                $0.amount = 500000
                $0.denom = "uluna"
            }
            $0.typePrefix = "staking/MsgBeginRedelegate"
        }

        let message = CosmosMessage.with {
            $0.restakeMessage = restakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = 3000
                $0.denom = "uluna"
            }]
        }

        let signingInput = CosmosSigningInput.with {
            $0.accountNumber = 158
            $0.chainID = "soju-0013"
            $0.memo = ""
            $0.sequence = 4
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output = CosmosSigner.sign(input: signingInput)
        let expectedJSON = """
{
    "mode": "block",
    "tx": {
        "fee": {
            "amount": [{
                "amount": "3000",
                "denom": "uluna"
            }],
            "gas": "200000"
        },
        "memo": "",
        "msg": [{
            "type": "staking/MsgBeginRedelegate",
            "value": {
                "amount": {
                    "amount": "500000",
                    "denom": "uluna"
                },
                "delegator_address": "terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe",
                "validator_dst_address": "terravaloper1rhrptnx87ufpv62c7ngt9yqlz2hr77xr9nkcr9",
                "validator_src_address": "terravaloper1pdx498r0hrc2fj36sjhs8vuhrz9hd2cw0yhqtk"
            }
        }],
        "signatures": [{
            "pub_key": {
                "type": "tendermint/PubKeySecp256k1",
                "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
            },
            "signature": "HyEpSz48dkebmBFvwh5xDiiZD0jUdOvzTD3ACMw0rOQ9F3JhK2cPaEx6/ZmYNIrdsPqMNkUnHcDYD1o4IztoEg=="
        }]
    }
}
"""
        XCTAssertJSONEqual(expectedJSON, output.json)
    }
}
