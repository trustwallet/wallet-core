// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class THORChainAddressTests: XCTestCase {
    func testAddressValidation() {
        let thorchain = CoinType.thorchain
        for address in [
            "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r",
            "thor1c8jd7ad9pcw4k3wkuqlkz4auv95mldr2kyhc65",
        ] {
            XCTAssertTrue(thorchain.validate(address: address))
            XCTAssertEqual(thorchain.address(string: address)?.description, address)
        }
    }
}

class THORChainSignerTests: XCTestCase {

    let privateKey = PrivateKey(data: Data(hexString: "7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e")!)!

    func testJsonModeSigning() {
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .thorchain)

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = fromAddress.description
            $0.toAddress = "thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"
            $0.amounts = [CosmosAmount.with {
                $0.amount = "10000000"
                $0.denom = "rune"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "2000000"
                $0.denom = "rune"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 593
            $0.chainID = "thorchain"
            $0.memo = ""
            $0.sequence = 2
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .thorchain)

        let expectedJSON: String = """
        {
            "mode": "block",
            "tx": {
                "fee": {
                    "amount": [
                        {
                            "amount": "2000000",
                            "denom": "rune"
                        }
                    ],
                    "gas": "200000"
                },
                "memo": "",
                "msg": [
                    {
                        "type": "thorchain/MsgSend",
                        "value": {
                            "amount": [
                                {
                                    "amount": "10000000",
                                    "denom": "rune"
                                }
                            ],
                            "from_address": "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r",
                            "to_address": "thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"
                        }
                    }
                ],
                "signatures": [
                    {
                        "pub_key": {
                            "type": "tendermint/PubKeySecp256k1",
                            "value": "A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"
                        },
                        "signature": "ZPhcYubhAd6iz/pBrtLfSJaK04ISnEo+jBFvFFzoToMJA9NGhhCFmsmXMQ1AtoJ6C1aylvUnck93A7ork8ZzEQ=="
                    }
                ]
            }
        }
        """

        XCTAssertJSONEqual(expectedJSON, output.json)
    }

    func testProtobufModeSigning() {
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .thorchain)

        let sendCoinsMessage = CosmosMessage.THORChainSend.with {
            $0.fromAddress = fromAddress.data
            $0.toAddress = AnyAddress(string: "thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn", coin: .thorchain)!.data
            $0.amounts = [CosmosAmount.with {
                $0.amount = "38000000"
                $0.denom = "rune"
            }]
        }

        let message = CosmosMessage.with {
            $0.thorchainSendMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 2500000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "200"
                $0.denom = "rune"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 593
            $0.chainID = "thorchain-mainnet-v1"
            $0.sequence = 21
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
            $0.signingMode = .protobuf
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .thorchain)
        let expectedJSON = """
        {
            "mode": "BROADCAST_MODE_BLOCK",
            "tx_bytes": "ClIKUAoOL3R5cGVzLk1zZ1NlbmQSPgoUFSLnZ9tusZcIsAOAKb+9YHvJvQ4SFMqGRZ+wBVHH30JUDF54aRksgzrbGhAKBHJ1bmUSCDM4MDAwMDAwEmYKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQPtmX45bPQpL1/OWkK7pBWZzNXZbjExVKfJ6nBJ3jF8dxIECgIIARgVEhIKCwoEcnVuZRIDMjAwEKDLmAEaQKZtS3ATa26OOGvqdKm14ZbHeNfkPtIajXi5MkZ5XaX2SWOeX+YnCPZ9TxF9Jj5cVIo71m55xq4hVL3yDbRe89g="
        }
        """

        XCTAssertJSONEqual(expectedJSON, output.serialized)
    }
}
