// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class TerraClassicTests: XCTestCase {

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
                $0.amount = "3000"
                $0.denom = "uluna"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 158
            $0.chainID = "soju-0013"
            $0.memo = ""
            $0.sequence = 0
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .terra)

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
                    $0.amount = "1000000"
                    $0.denom = "uluna"
                }]
                $0.typePrefix = "bank/MsgSend"
            }
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "3000"
                $0.denom = "uluna"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 158
            $0.chainID = "soju-0013"
            $0.memo = ""
            $0.sequence = 0
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .terra)

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
                $0.amount = "1000000"
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
                $0.amount = "3000"
                $0.denom = "uluna"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 158
            $0.chainID = "soju-0013"
            $0.memo = ""
            $0.sequence = 1
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .terra)

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
                $0.amount = "3000"
                $0.denom = "uluna"
            }]
            $0.gas = 200000
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 158
            $0.chainID = "soju-0013"
            $0.memo = ""
            $0.sequence = 2
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .terra)

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
                $0.amount = "500000"
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
                $0.amount = "3000"
                $0.denom = "uluna"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 158
            $0.chainID = "soju-0013"
            $0.memo = ""
            $0.sequence = 3
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .terra)

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
                $0.amount = "500000"
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
                $0.amount = "3000"
                $0.denom = "uluna"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 158
            $0.chainID = "soju-0013"
            $0.memo = ""
            $0.sequence = 4
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .terra)

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

    func testSigningWasmTerraTransferTxProtobuf() {
        let privateKey = PrivateKey(data: Data(hexString: "cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .terra)

        let wasmTransferMessage = CosmosMessage.WasmTerraExecuteContractTransfer.with {
            $0.senderAddress = fromAddress.description
            $0.contractAddress = "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76" // ANC
            $0.amount = Data(hexString: "03D090")! // 250000
            $0.recipientAddress = "terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp"
        }

        let message = CosmosMessage.with {
            $0.wasmTerraExecuteContractTransferMessage = wasmTransferMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "3000"
                $0.denom = "uluna"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 3407705
            $0.chainID = "columbus-5"
            $0.memo = ""
            $0.sequence = 3
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .terra)

        XCTAssertJSONEqual(output.serialized,
"""
{
    "tx_bytes": "CucBCuQBCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBK5AQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMTR6NTZsMGZwMmxzZjg2enkzaHR5Mno0N2V6a2hudGh0cjl5cTc2Glt7InRyYW5zZmVyIjp7ImFtb3VudCI6IjI1MDAwMCIsInJlY2lwaWVudCI6InRlcnJhMWpsZ2FxeTludm4yaGY1dDJzcmE5eWN6OHM3N3duZjlsMGttZ2NwIn19EmcKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQNwZjrHsPmJKW/rXOWfukpQ1+lOHOJW3/IlFFnKLNmsABIECgIIARgDEhMKDQoFdWx1bmESBDMwMDAQwJoMGkAaprIEMLPH2HmFdwFGoaipb2GIyhXt6ombz+WMnG2mORBI6gFt0M+IymYgzZz6w1SW52R922yafDnn7yXfutRw",
    "mode": "BROADCAST_MODE_BLOCK"
}
"""
        )
        XCTAssertEqual(output.errorMessage, "")
    }

    func testSigningWasmTerraGenericProtobuf() {
            let privateKey = PrivateKey(data: Data(hexString: "cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616")!)!
            let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
            let fromAddress = AnyAddress(publicKey: publicKey, coin: .terra)

            let wasmGenericMessage = CosmosMessage.WasmTerraExecuteContractGeneric.with {
                $0.senderAddress = fromAddress.description
                $0.contractAddress = "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76" // ANC
                $0.executeMsg = """
                {"transfer": { "amount": "250000", "recipient": "terra1d7048csap4wzcv5zm7z6tdqem2agyp9647vdyj" } }
                """
            }

            let message = CosmosMessage.with {
                $0.wasmTerraExecuteContractGeneric = wasmGenericMessage
            }

            let fee = CosmosFee.with {
                $0.gas = 200000
                $0.amounts = [CosmosAmount.with {
                    $0.amount = "3000"
                    $0.denom = "uluna"
                }]
            }

            let input = CosmosSigningInput.with {
                $0.signingMode = .protobuf;
                $0.accountNumber = 3407705
                $0.chainID = "columbus-5"
                $0.memo = ""
                $0.sequence = 7
                $0.messages = [message]
                $0.fee = fee
                $0.privateKey = privateKey.data
            }

            let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .terra)

            XCTAssertJSONEqual(output.serialized,
    """
    {
        "tx_bytes": "Cu4BCusBCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBLAAQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMTR6NTZsMGZwMmxzZjg2enkzaHR5Mno0N2V6a2hudGh0cjl5cTc2GmJ7InRyYW5zZmVyIjogeyAiYW1vdW50IjogIjI1MDAwMCIsICJyZWNpcGllbnQiOiAidGVycmExZDcwNDhjc2FwNHd6Y3Y1em03ejZ0ZHFlbTJhZ3lwOTY0N3ZkeWoiIH0gfRJnClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEDcGY6x7D5iSlv61zln7pKUNfpThziVt/yJRRZyizZrAASBAoCCAEYBxITCg0KBXVsdW5hEgQzMDAwEMCaDBpAkPsS7xlSng2LMc9KiD1soN5NLaDcUh8I9okPmsdJN3le1B7yxRGNB4aQfhaRl/8Z0r5vitRT0AWuxDasd8wcFw==",
        "mode": "BROADCAST_MODE_BLOCK"
    }
    """
            )
            XCTAssertEqual(output.errorMessage, "")
        }

        func testSigningWasmTerraGenericWithCoins() {
            let privateKey = PrivateKey(data: Data(hexString: "cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616")!)!
            let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
            let fromAddress = AnyAddress(publicKey: publicKey, coin: .terra)

            let wasmGenericMessage = CosmosMessage.WasmTerraExecuteContractGeneric.with {
                $0.senderAddress = fromAddress.description
                $0.contractAddress = "terra1sepfj7s0aeg5967uxnfk4thzlerrsktkpelm5s" // ANC Market
                $0.executeMsg = """
                { "deposit_stable": {} }
                """
                $0.coins = [CosmosAmount.with {
                    $0.amount = "1000"
                    $0.denom = "uusd"
                }]
            }

            let message = CosmosMessage.with {
                $0.wasmTerraExecuteContractGeneric = wasmGenericMessage
            }

            let fee = CosmosFee.with {
                $0.gas = 600000
                $0.amounts = [CosmosAmount.with {
                    $0.amount = "7000"
                    $0.denom = "uluna"
                }]
            }

            let input = CosmosSigningInput.with {
                $0.signingMode = .protobuf;
                $0.accountNumber = 3407705
                $0.chainID = "columbus-5"
                $0.memo = ""
                $0.sequence = 9
                $0.messages = [message]
                $0.fee = fee
                $0.privateKey = privateKey.data
            }

            let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .terra)

            XCTAssertJSONEqual(output.serialized,
    """
    {
            "tx_bytes": "CrIBCq8BCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBKEAQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMXNlcGZqN3MwYWVnNTk2N3V4bmZrNHRoemxlcnJza3RrcGVsbTVzGhh7ICJkZXBvc2l0X3N0YWJsZSI6IHt9IH0qDAoEdXVzZBIEMTAwMBJnClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEDcGY6x7D5iSlv61zln7pKUNfpThziVt/yJRRZyizZrAASBAoCCAEYCRITCg0KBXVsdW5hEgQ3MDAwEMDPJBpAGyi7f1ioY8XV6pjFq1s86Om4++CIUnd3rLHif2iopCcYvX0mLkTlQ6NUERg8nWTYgXcj6fOTO/ptgPuAtv0NWg==",
            "mode": "BROADCAST_MODE_BLOCK"
    }
    """
            )
            XCTAssertEqual(output.errorMessage, "")
        }
    }
