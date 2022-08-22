// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class TerraTests: XCTestCase {

    let privateKey80e8 = PrivateKey(data: Data(hexString: "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005")!)! // terra1hsk6jryyqjfhp5dhc55tc9jtckygx0ep37hdd2
    let privateKeycf08 = PrivateKey(data: Data(hexString: "cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616")!)! // terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf
    let privateKey1037 = PrivateKey(data: Data(hexString: "1037f828ca313f4c9e120316e8e9ff25e17f07fe66ba557d5bc5e2eeb7cba8f6")!)!

    func testAddress() {
        let address = CoinType.terraV2.deriveAddress(privateKey: privateKey1037)

        XCTAssertEqual(address, "terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe")
        XCTAssertTrue(CoinType.terraV2.validate(address: "terra1hdp298kaz0eezpgl6scsykxljrje3667d233ms"))
        XCTAssertTrue(CoinType.terraV2.validate(address: "terravaloper1pdx498r0hrc2fj36sjhs8vuhrz9hd2cw0yhqtk"))
        XCTAssertFalse(CoinType.terraV2.validate(address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02"))
    }

    func testSignSendTx() {
        let publicKey = privateKey80e8.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .terraV2).description
        XCTAssertEqual(fromAddress, "terra1hsk6jryyqjfhp5dhc55tc9jtckygx0ep37hdd2")

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = CosmosMessage.Send.with {
                $0.fromAddress = fromAddress
                $0.toAddress = "terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp"
                $0.amounts = [CosmosAmount.with {
                    $0.amount = "1000000"
                    $0.denom = "uluna"
                }]
            }
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "30000"
                $0.denom = "uluna"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 1037
            $0.chainID = "phoenix-1"
            $0.memo = ""
            $0.sequence = 1
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey80e8.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .terraV2)

        let expectedJSON: String =
        """
        {
            "tx_bytes": "CpEBCo4BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm4KLHRlcnJhMWhzazZqcnl5cWpmaHA1ZGhjNTV0YzlqdGNreWd4MGVwMzdoZGQyEix0ZXJyYTFqbGdhcXk5bnZuMmhmNXQyc3JhOXljejhzNzd3bmY5bDBrbWdjcBoQCgV1bHVuYRIHMTAwMDAwMBJoClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiECVyhuw/N9M1V7u6oACyd0SskCOqmWfK51oYHR/5H6ncUSBAoCCAEYARIUCg4KBXVsdW5hEgUzMDAwMBDAmgwaQPh0C3rjzdixIUiyPx3FlWAxzbKILNAcSRVeQnaTl1vsI5DEfYa2oYlUBLqyilcMCcU/iaJLhex30No2ak0Zn1Q=",
            "mode": "BROADCAST_MODE_BLOCK"
        }
        """
        XCTAssertJSONEqual(expectedJSON, output.serialized)
        XCTAssertEqual(output.errorMessage, "")
    }

    func testSignWasmTransferTx() {
        let publicKey = privateKeycf08.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .terraV2).description
        XCTAssertEqual(fromAddress, "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf")

        let wasmTransferMessage = CosmosMessage.WasmExecuteContractTransfer.with {
            $0.senderAddress = fromAddress.description
            $0.contractAddress = "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76"
            $0.amount = Data(hexString: "03D090")! // 250000
            $0.recipientAddress = "terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp"
        }

        let message = CosmosMessage.with {
            $0.wasmExecuteContractTransferMessage = wasmTransferMessage
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
            $0.chainID = "phoenix-1"
            $0.memo = ""
            $0.sequence = 3
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKeycf08.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .terraV2)

        XCTAssertJSONEqual(output.serialized,
        """
        {
            "tx_bytes": "CuUBCuIBCiQvY29zbXdhc20ud2FzbS52MS5Nc2dFeGVjdXRlQ29udHJhY3QSuQEKLHRlcnJhMTh3dWtwODRkcTIyN3d1NG1naDBqbTZuOW5sbmo2cnM4MnBwOXdmEix0ZXJyYTE0ejU2bDBmcDJsc2Y4Nnp5M2h0eTJ6NDdlemtobnRodHI5eXE3NhpbeyJ0cmFuc2ZlciI6eyJhbW91bnQiOiIyNTAwMDAiLCJyZWNpcGllbnQiOiJ0ZXJyYTFqbGdhcXk5bnZuMmhmNXQyc3JhOXljejhzNzd3bmY5bDBrbWdjcCJ9fRJnClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEDcGY6x7D5iSlv61zln7pKUNfpThziVt/yJRRZyizZrAASBAoCCAEYAxITCg0KBXVsdW5hEgQzMDAwEMCaDBpAiBGbQaj+jsXE6/FssD3fC77QOxpli9GqsPea+KoNyMIEgVj89Hii+oU1bAEQS4qV0SaE2V6RNy24uCcFTIRbcQ==",
            "mode": "BROADCAST_MODE_BLOCK"
        }
        """
        )
        XCTAssertEqual(output.errorMessage, "")
    }

    func testSignStakeTx() throws {

        let stakeMessage = CosmosMessage.Delegate.with {
            $0.delegatorAddress = "terra1ncfyexz3nrrdru37ahqpp4wen48v7p5nany478"
            $0.validatorAddress = "terravaloper1ekq8xuypdxtf3nfmffmydnhny59pjuy0p8wpn7"
            $0.amount = CosmosAmount.with {
                $0.amount = "1000000" // 5 Luna
                $0.denom = "uluna"
            }
        }

        let message = CosmosMessage.with {
            $0.stakeMessage = stakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 254682
            $0.amounts = [CosmosAmount.with {
                $0.amount = "38203"
                $0.denom = "uluna"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 127185
            $0.chainID = "phoenix-1"
            $0.sequence = 6
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey1037.data // real key is terra: define...
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .terraV2)
        let expected = """
        {
            "mode": "BROADCAST_MODE_BLOCK",
            "tx_bytes": "Cp8BCpwBCiMvY29zbW9zLnN0YWtpbmcudjFiZXRhMS5Nc2dEZWxlZ2F0ZRJ1Cix0ZXJyYTFuY2Z5ZXh6M25ycmRydTM3YWhxcHA0d2VuNDh2N3A1bmFueTQ3OBIzdGVycmF2YWxvcGVyMWVrcTh4dXlwZHh0ZjNuZm1mZm15ZG5obnk1OXBqdXkwcDh3cG43GhAKBXVsdW5hEgcxMDAwMDAwEmgKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQNd8YVWZSHWp4AjGe4G4aKOl7d3Lftf3RPKbwV1UYlo5BIECgIIARgGEhQKDgoFdWx1bmESBTM4MjAzENrFDxpAamKyAvWhWCv0nUKz7yiYETpkZETflDvfe1vmuFIy31g+s0u1cgLNo+7jBRXRuzYJXukigtwoLUrxY/C8rowiJw=="
        }
        """

        // https://finder.terra.money/mainnet/tx/4441c65f24783b8f59b20b1b80ee43f1f4f6ff827597d87b6bbc94982b45be0c
        XCTAssertJSONEqual(output.serialized, expected)
        XCTAssertEqual(output.errorMessage, "")
    }

    func testSignClaimRewards() throws {
        let delegator = "terra1ncfyexz3nrrdru37ahqpp4wen48v7p5nany478"
        let validators = [
            "terravaloper1ekq8xuypdxtf3nfmffmydnhny59pjuy0p8wpn7"
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
                $0.amount = "29513"
                $0.denom = "uluna"
            }]
            $0.gas = 196749
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.fee = fee
            $0.accountNumber = 127185
            $0.chainID = "phoenix-1"
            $0.sequence = 5
            $0.messages = withdrawals
            $0.fee = fee
            $0.privateKey = privateKey1037.data // real key is terra: define...
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .terraV2)

        let expected = """
        {
            "mode": "BROADCAST_MODE_BLOCK",
            "tx_bytes": "CqEBCp4BCjcvY29zbW9zLmRpc3RyaWJ1dGlvbi52MWJldGExLk1zZ1dpdGhkcmF3RGVsZWdhdG9yUmV3YXJkEmMKLHRlcnJhMW5jZnlleHozbnJyZHJ1MzdhaHFwcDR3ZW40OHY3cDVuYW55NDc4EjN0ZXJyYXZhbG9wZXIxZWtxOHh1eXBkeHRmM25mbWZmbXlkbmhueTU5cGp1eTBwOHdwbjcSaApQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjkEgQKAggBGAUSFAoOCgV1bHVuYRIFMjk1MTMQjYEMGkA/bh2va6RRZvkSLnej84dJgSSvbgcHgYDbkRt8wDge03W747BZcuBcg/U5EuE7zBqSJrKUTZl7oUCp//rYlJKV"
        }
        """

        // https://finder.terra.money/mainnet/tx/0e62170ed5407992251d7e161f23c3467e1bea54c7f601953953bdabc7f0c30c
        XCTAssertJSONEqual(output.serialized, expected)
        XCTAssertEqual(output.errorMessage, "")
    }

}
