// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class OsmosisTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let address = AnyAddress(publicKey: pubkey, coin: .osmosis)
        let addressFromString = AnyAddress(string: "osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f5", coin: .osmosis)!

        XCTAssertEqual(pubkey.data.hexString, "02ecef5ce437a302c67f95468de4b31f36e911f467d7e6a52b41c1e13e1d563649")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSigningTransaction() {
        let privateKey = PrivateKey(data: Data(hexString: "8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .osmosis)

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = fromAddress.description
            $0.toAddress = "osmo18s0hdnsllgcclweu9aymw4ngktr2k0rkvn7jmn"
            $0.amounts = [CosmosAmount.with {
                $0.amount = "99800"
                $0.denom = "uosmo"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "200"
                $0.denom = "uosmo"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 124703
            $0.chainID = "osmosis-1"
            $0.memo = ""
            $0.sequence = 0
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .osmosis)

        XCTAssertJSONEqual(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"Co0BCooBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmoKK29zbW8xbWt5Njljbjhla3R3eTA4NDV2ZWM5dXBzZHBoa3R4dDBlbjk3ZjUSK29zbW8xOHMwaGRuc2xsZ2NjbHdldTlheW13NG5na3RyMmswcmt2bjdqbW4aDgoFdW9zbW8SBTk5ODAwEmQKTgpGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQLs71zkN6MCxn+VRo3ksx826RH0Z9fmpStBweE+HVY2SRIECgIIARISCgwKBXVvc21vEgMyMDAQwJoMGkAMY//Md5GRUR4lVZhk558hFS3kii9QZYoYKfg4+ac/xgNeyoiEweVDhcmEvlH1orVwjLUOnYs4ly2a/yIurYVj\"}")
        XCTAssertEqual(output.errorMessage, "")
    }
}
