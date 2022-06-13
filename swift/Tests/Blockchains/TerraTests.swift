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
        XCTAssertEqual(output.error, "")
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
        XCTAssertEqual(output.error, "")
    }
}
