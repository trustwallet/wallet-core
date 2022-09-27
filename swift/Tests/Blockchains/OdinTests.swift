// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class OdinTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "83e68457212d6026ec2fe18cf1ecb1ed2cb2aa8ecdec38d884c89c40dfb68f62")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let address = AnyAddress(publicKey: pubkey, coin: .odin)
        let addressFromString = AnyAddress(string: "odin1d224epchv57ftr6qwq98nzllfw9mmtzpfg4zla", coin: .odin)!

        XCTAssertEqual(pubkey.data.hexString, "028edd997cd83ecc9f82c70483d116de363efa9ee5ba6888e8483b8c7c6679fc29")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSigningTransaction() {
        let privateKey = PrivateKey(data: Data(hexString: "a39b28abca9b410e6f53d87a42229150cb176211996391c989336fbbcb9606cb")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .odin)

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = fromAddress.description
            $0.toAddress = "odin13d7rfa8v8jmmsh63jetd9euqftaqfc3hre5ake"
            $0.amounts = [CosmosAmount.with {
                $0.amount = "99800"
                $0.denom = "loki"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "200"
                $0.denom = "loki"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 124703
            $0.chainID = "odin-mainnet-freya"
            $0.memo = ""
            $0.sequence = 0
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .odin)

        XCTAssertJSONEqual(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKK29kaW4xZnpqbnN1d3p4em5xZDNxbGt0NW5qZDR2dnBjanFjYTZ6Y2tmdXgSK29kaW4xM2Q3cmZhOHY4am1tc2g2M2pldGQ5ZXVxZnRhcWZjM2hyZTVha2UaDQoEbG9raRIFOTk4MDASYwpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAzdXdUgXyztVVpl5pFOdmj8kUEWei9xLNcZQihXXmt5IEgQKAggBEhEKCwoEbG9raRIDMjAwEMCaDBpAfXlsIu1Ke+kalRDx2vkE8anvcsajlru8RETlqBqEQWYD2JENyS0fmxTNsEmNu40Unjth3c0O8aUFYnj4D38P3A==\"}")
        XCTAssertEqual(output.error, "")
    }
}
