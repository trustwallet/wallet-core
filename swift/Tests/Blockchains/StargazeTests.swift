// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class StargazeTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let address = AnyAddress(publicKey: pubkey, coin: .stargaze)
        let addressFromString = AnyAddress(string: "stars1mry47pkga5tdswtluy0m8teslpalkdq02a8nhy", coin: .stargaze)!

        XCTAssertEqual(pubkey.data.hexString, "02cbfdb5e472893322294e60cf0883d43df431e1089d29ecb447a9e6d55045aae5")
        XCTAssertEqual(address.description, addressFromString.description)
    }
    
    func testSignCW721() {
        let privateKey = PrivateKey(data: Data(hexString: "a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433")!)!

        let txMessage = "{\"transfer_nft\": {\"recipient\": \"stars1kd5q7qejlqz94kpmd9pvr4v2gzgnca3lvt6xnp\",\"token_id\": \"1209\"}}";
        let wasmMsg = CosmosMessage.WasmExecuteContractGeneric.with {
            $0.contractAddress = "stars14gmjlyfz5mpv5d8zrksn0tjhz2wwvdc4yk06754alfasq9qen7fsknry42"
            $0.senderAddress = "stars1mry47pkga5tdswtluy0m8teslpalkdq02a8nhy"
            $0.executeMsg = txMessage
        }

        let message = CosmosMessage.with {
            $0.wasmExecuteContractGeneric = wasmMsg
        }

        let fee = CosmosFee.with {
            $0.gas = 666666
            $0.amounts = [CosmosAmount.with {
                $0.amount = "1000"
                $0.denom = "ustars"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 188393
            $0.chainID = "stargaze-1"
            $0.memo = ""
            $0.sequence = 5
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .stargaze)

        // Successfully broadcasted: https://www.mintscan.io/stargaze/txs/300836A5BF9002CF38EE34A8C56E8E7E6854FA64F1DEB3AE108F381A48150F7C
        let expected = """
        {
                            "mode":"BROADCAST_MODE_BLOCK",
                            "tx_bytes":"CoACCv0BCiQvY29zbXdhc20ud2FzbS52MS5Nc2dFeGVjdXRlQ29udHJhY3QS1AEKLHN0YXJzMW1yeTQ3cGtnYTV0ZHN3dGx1eTBtOHRlc2xwYWxrZHEwMmE4bmh5EkBzdGFyczE0Z21qbHlmejVtcHY1ZDh6cmtzbjB0amh6Mnd3dmRjNHlrMDY3NTRhbGZhc3E5cWVuN2Zza25yeTQyGmJ7InRyYW5zZmVyX25mdCI6IHsicmVjaXBpZW50IjogInN0YXJzMWtkNXE3cWVqbHF6OTRrcG1kOXB2cjR2Mmd6Z25jYTNsdnQ2eG5wIiwidG9rZW5faWQiOiAiMTIwOSJ9fRJoClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiECy/215HKJMyIpTmDPCIPUPfQx4QidKey0R6nm1VBFquUSBAoCCAEYBRIUCg4KBnVzdGFycxIEMTAwMBCq2CgaQMx+l2sdM5DAPbDyY1p173MLnjGyNWIcRmaFiVNphLuTV3tjhwPbsXEA0hyRxyWS3vN0/xUF/JEsO9wRspj2aJ4="
        }
        """;
        XCTAssertJSONEqual(output.serialized, expected)
        XCTAssertEqual(output.errorMessage, "")
    }

    func testSign() {
        let privateKey = PrivateKey(data: Data(hexString: "a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .stargaze)

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = fromAddress.description
            $0.toAddress = "stars1mry47pkga5tdswtluy0m8teslpalkdq02a8nhy"
            $0.amounts = [CosmosAmount.with {
                $0.amount = "10000"
                $0.denom = "ustars"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 80000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "1000"
                $0.denom = "ustars"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 188393
            $0.chainID = "stargaze-1"
            $0.memo = ""
            $0.sequence = 0
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .stargaze)

        // Successfully broadcasted: https://www.mintscan.io/stargaze/txs/98D5E36CA7080DDB286FE924A5A9976ABD4EBE49C92A09D322F29AD30DE4BE4D
        let expected = """
        {
                            "mode":"BROADCAST_MODE_BLOCK",
                            "tx_bytes":"CpABCo0BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm0KLHN0YXJzMW1yeTQ3cGtnYTV0ZHN3dGx1eTBtOHRlc2xwYWxrZHEwMmE4bmh5EixzdGFyczFtcnk0N3BrZ2E1dGRzd3RsdXkwbTh0ZXNscGFsa2RxMDJhOG5oeRoPCgZ1c3RhcnMSBTEwMDAwEmYKTgpGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQLL/bXkcokzIilOYM8Ig9Q99DHhCJ0p7LRHqebVUEWq5RIECgIIARIUCg4KBnVzdGFycxIEMTAwMBCA8QQaQHAkntxzC1oH7Yde4+KEmnB+K3XbJIYw0q6MqMPEY65YAwBDNDOdaTu/rpehus/20MvBfbAEZiw9+whzXLpkQ5A="
        }
        """;
        XCTAssertJSONEqual(output.serialized, expected)
        XCTAssertEqual(output.errorMessage, "")
    }
}
