// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class TerraSignerTests: XCTestCase {

    let privateKey = PrivateKey(data: Data(hexString: "cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616")!)!

    func testSigningWasmTransferTxProtobuf() {
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .terra)

        let wasmTransferMessage = CosmosMessage.WasmExecuteContractTransfer.with {
            $0.senderAddress = fromAddress.description
            $0.contractAddress = "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76" // ANC
            $0.amount = 250000
            $0.recipientAddress = "terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp"
        }

        let message = CosmosMessage.with {
            $0.wasmExecuteContracctTransferMessage = wasmTransferMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = 3000
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

        XCTAssertJSONEqual(output.serialized, "{\"tx_bytes\": \"CucBCuQBCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBK5AQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMTR6NTZsMGZwMmxzZjg2enkzaHR5Mno0N2V6a2hudGh0cjl5cTc2Glt7InRyYW5zZmVyIjp7ImFtb3VudCI6IjI1MDAwMCIsInJlY2lwaWVudCI6InRlcnJhMWpsZ2FxeTludm4yaGY1dDJzcmE5eWN6OHM3N3duZjlsMGttZ2NwIn19EmcKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQNwZjrHsPmJKW/rXOWfukpQ1+lOHOJW3/IlFFnKLNmsABIECgIIARgDEhMKDQoFdWx1bmESBDMwMDAQwJoMGkAaprIEMLPH2HmFdwFGoaipb2GIyhXt6ombz+WMnG2mORBI6gFt0M+IymYgzZz6w1SW52R922yafDnn7yXfutRw\", \"mode\": \"BROADCAST_MODE_BLOCK\"}")
        XCTAssertEqual(output.error, "")
    }
}
