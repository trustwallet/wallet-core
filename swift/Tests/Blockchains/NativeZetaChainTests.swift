// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class NativeZetaChainTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "8d2a3bd62d300a148c89dc8635f87b7a24a951bd1c4e78675fe40e1a640d46ed")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .nativeZetaChain)
        let addressFromString = AnyAddress(string: "zeta14py36sx57ud82t9yrks9z6hdsrpn5x6kmxs0ne", coin: .nativeZetaChain)!

        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        let privateKey = PrivateKey(data: Data(hexString: "8d2a3bd62d300a148c89dc8635f87b7a24a951bd1c4e78675fe40e1a640d46ed")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .nativeZetaChain)

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = CosmosMessage.Send.with {
                $0.fromAddress = fromAddress.description
                $0.toAddress = "zeta1cscf4ldnkkz7f0wpveur6dpd0d6p2zxnsuu70y"
                $0.amounts = [CosmosAmount.with {
                    $0.amount = "300000000000000000"
                    $0.denom = "azeta"
                }]
            }
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 2726346
            $0.chainID = "athens_7001-1"
            $0.sequence = 2
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
            $0.txHasher = CosmosTxHasher.keccak256
            $0.signerInfo = CosmosSignerInfo.with {
                $0.publicKeyType = CosmosSignerPublicKeyType.secp256K1
                $0.jsonType = "ethermint/PubKeyEthSecp256k1"
                $0.protobufType = "/ethermint.crypto.v1.ethsecp256k1.PubKey"
            }
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .nativeZetaChain)

        XCTAssertJSONEqual(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CpoBCpcBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEncKK3pldGExNHB5MzZzeDU3dWQ4MnQ5eXJrczl6Nmhkc3JwbjV4NmtteHMwbmUSK3pldGExY3NjZjRsZG5ra3o3ZjB3cHZldXI2ZHBkMGQ2cDJ6eG5zdXU3MHkaGwoFYXpldGESEjMwMDAwMDAwMDAwMDAwMDAwMBJhClkKTwooL2V0aGVybWludC5jcnlwdG8udjEuZXRoc2VjcDI1NmsxLlB1YktleRIjCiECho5+FjRBfbKt/Z/jggW/oP6gGJin/TBWXRP3BWo3wGUSBAoCCAEYAhIEEMCaDBpAgGvqca0w2N9wnHnnxS9HiVud4aQ9lNCumzgNIW6wOR4kvPScacGS1G3kwCw7wyI2NJL8M1eVYjafFIt2FpKl3w==\"}")
    }
}
