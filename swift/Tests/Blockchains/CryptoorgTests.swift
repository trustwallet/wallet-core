// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
                $0.amount = 50000000
                $0.denom = "basecro"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = 5000
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
        XCTAssertEqual(output.serialized.hexString, "0a90010a8d010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e64126d0a2a63726f3163747774637770676b736b7939383864687468366a736c787665756d6775306434357a676630122a63726f317870616879366337776c6478616376366c643939683433356d6876666e7375703234766375731a130a076261736563726f1208353030303030303012690a500a460a1f2f636f736d6f732e63727970746f2e736563703235366b312e5075624b657912230a21038831b061623e7e8be59d888c71084a45c8d089307cbf5ec55dc62c1d7bd56be312040a020801180212150a0f0a076261736563726f12043530303010c09a0c1a4007312bdc64eabebd826cfed5459a0a763136e5cf5d9769e7d61ca8a3c913977a7e9f882024c13b0776aecf0c880a5c7fc90d4ab6d9ea8102c5c19001dc45d122")
        XCTAssertEqual(output.serializedBase64, "CpABCo0BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm0KKmNybzFjdHd0Y3dwZ2tza3k5ODhkaHRoNmpzbHh2ZXVtZ3UwZDQ1emdmMBIqY3JvMXhwYWh5NmM3d2xkeGFjdjZsZDk5aDQzNW1odmZuc3VwMjR2Y3VzGhMKB2Jhc2Vjcm8SCDUwMDAwMDAwEmkKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQOIMbBhYj5+i+WdiIxxCEpFyNCJMHy/XsVdxiwde9Vr4xIECgIIARgCEhUKDwoHYmFzZWNybxIENTAwMBDAmgwaQAcxK9xk6r69gmz+1UWaCnYxNuXPXZdp59YcqKPJE5d6fp+IICTBOwd2rs8MiApcf8kNSrbZ6oECxcGQAdxF0SI=")
        XCTAssertEqual(output.error, "")
    }
}
