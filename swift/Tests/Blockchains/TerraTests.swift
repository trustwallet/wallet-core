// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class TerraTests: XCTestCase {

    func testSigningTransaction() {
        let privateKey = PrivateKey(data: Data(hexString: "1037f828ca313f4c9e120316e8e9ff25e17f07fe66ba557d5bc5e2eeb7cba8f6")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = CosmosAddress(hrp: .terra, publicKey: publicKey)!.description

        let message = CosmosMessage.with {
            $0.rawJsonMessage = CosmosMessage.RawJSON.with {
                $0.type = "pay/MsgSend"
                $0.value = """
                {
                    "amount": [{
                        "amount": "100000",
                        "denom": "uluna"
                    }],
                    "from_address": "\(fromAddress)",
                    "to_address": "terra1szvgdsasnffunwakp9w040unxdresk7a34cfka"
                }
                """
            }
        }

        let feeAmount = CosmosAmount.with {
            $0.amount = 30000
            $0.denom = "uluna"
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [feeAmount]
        }

        let signingInput = CosmosSigningInput.with {
            $0.accountNumber = 1516
            $0.chainID = "columbus-2"
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
            $0.typePrefix = "pay/MsgSend"
        }

        let output = CosmosSigner.sign(input: signingInput)

        let expectedJSON: String =
        """
{
    "mode": "block",
    "tx": {
        "fee": {
            "amount": [{
                "amount": "30000",
                "denom": "uluna"
            }],
            "gas": "200000"
        },
        "memo": "",
        "msg": [{
            "type": "pay/MsgSend",
            "value": {
                "amount": [{
                    "amount": "100000",
                    "denom": "uluna"
                }],
                "from_address": "terra1jf9aaj9myrzsnmpdr7twecnaftzmku2mhs2hfe",
                "to_address": "terra1szvgdsasnffunwakp9w040unxdresk7a34cfka"
            }
        }],
        "signatures": [{
            "pub_key": {
                "type": "tendermint/PubKeySecp256k1",
                "value": "A13xhVZlIdangCMZ7gbhoo6Xt3ct+1/dE8pvBXVRiWjk"
            },
            "signature": "uJYs+ob1GP25WOx+WCBH0LVfPP4rsV2wEkLB/VTbgVpBZOiWGaf+QJC/J/R8bE6LpcVI3V7cCejX4BtyLnxDjA=="
        }],
        "type": "pay/MsgSend"
    }
}
"""

        XCTAssertJSONEqual(expectedJSON, output.json)
    }
}
