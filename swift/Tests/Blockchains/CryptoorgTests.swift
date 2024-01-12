// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class CryptoorgTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let address = AnyAddress(publicKey: pubkey, coin: .cryptoOrg)
        let addressFromString = AnyAddress(string: "cro1z53wwe7md6cewz9sqwqzn0aavpaun0gw39h3rd", coin: .cryptoOrg)!

        XCTAssertEqual(pubkey.data.hexString, "03ed997e396cf4292f5fce5a42bba41599ccd5d96e313154a7c9ea7049de317c77")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    let privateKey = PrivateKey(data: Data(hexString: "200e439e39cf1aad465ee3de6166247f914cbc0f823fc2dd48bf16dcd556f39d")!)!

    func testSign() {
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .cryptoOrg)

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = fromAddress.description
            $0.toAddress = "cro1xpahy6c7wldxacv6ld99h435mhvfnsup24vcus"
            $0.amounts = [CosmosAmount.with {
                $0.amount = "50000000"
                $0.denom = "basecro"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "5000"
                $0.denom = "basecro"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 125798
            $0.chainID = "crypto-org-chain-mainnet-1"
            $0.memo = ""
            $0.sequence = 2
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .cryptoOrg)

        // https://crypto.org/explorer/tx/BCB213B0A121F0CF11BECCF52475F1C8328D6070F3CFDA9E14C42E6DB30E847E
        XCTAssertJSONEqual(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CpABCo0BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm0KKmNybzFjdHd0Y3dwZ2tza3k5ODhkaHRoNmpzbHh2ZXVtZ3UwZDQ1emdmMBIqY3JvMXhwYWh5NmM3d2xkeGFjdjZsZDk5aDQzNW1odmZuc3VwMjR2Y3VzGhMKB2Jhc2Vjcm8SCDUwMDAwMDAwEmkKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQOIMbBhYj5+i+WdiIxxCEpFyNCJMHy/XsVdxiwde9Vr4xIECgIIARgCEhUKDwoHYmFzZWNybxIENTAwMBDAmgwaQAcxK9xk6r69gmz+1UWaCnYxNuXPXZdp59YcqKPJE5d6fp+IICTBOwd2rs8MiApcf8kNSrbZ6oECxcGQAdxF0SI=\"}")
        XCTAssertEqual(output.errorMessage, "")
    }
}
