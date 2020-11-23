// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class TronTests: XCTestCase {

    func testTronAddress() {
        let address = AnyAddress(string: "TLWEciM1CjP5fJqM2r9wymAidkkYtTU5k3", coin: .tron)!
        XCTAssertEqual(address.description, "TLWEciM1CjP5fJqM2r9wymAidkkYtTU5k3")
    }

    func testSign() {
        let contract = TronTransferContract.with {
            $0.ownerAddress = "TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC"
            $0.toAddress = "THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV"
            $0.amount = 2000000
        }

        let input = TronSigningInput.with {
            $0.transaction = TronTransaction.with {
                $0.contractOneof = .transfer(contract)
                $0.timestamp = 1539295479000
                $0.blockHeader = TronBlockHeader.with {
                    $0.timestamp = 1539295479000
                    $0.number = 3111739
                    $0.version = 3
                    $0.txTrieRoot = Data(hexString: "64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d")!
                    $0.parentHash = Data(hexString: "00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d")!
                    $0.witnessAddress = Data(hexString: "415863f6091b8e71766da808b1dd3159790f61de7d")!
                }
                $0.expiration = 1539295479000 + 10 * 60 * 60 * 1000
            }
            $0.privateKey = Data(hexString: "ba005cd605d8a02e3d5dfd04234cef3a3ee4f76bfbad2722d1fb5af8e12e6764")!
        }

        let output: TronSigningOutput = AnySigner.sign(input: input, coin: .tron)
        let expectedJSON = """
        {
            "raw_data": {
                "contract": [{
                    "parameter": {
                        "type_url": "type.googleapis.com/protocol.TransferContract",
                        "value": {
                            "amount": 2000000,
                            "owner_address": "415cd0fb0ab3ce40f3051414c604b27756e69e43db",
                            "to_address": "41521ea197907927725ef36d70f25f850d1659c7c7"
                        }
                    },
                    "type": "TransferContract"
                }],
                "expiration": 1539331479000,
                "ref_block_bytes": "7b3b",
                "ref_block_hash": "b21ace8d6ac20e7e",
                "timestamp": 1539295479000
            },
            "signature": ["6b5de85a80b2f4f02351f691593fb0e49f14c5cb42451373485357e42d7890cd77ad7bfcb733555c098b992da79dabe5050f5e2db77d9d98f199074222de037701"],
            "txID": "dc6f6d9325ee44ab3c00528472be16e1572ab076aa161ccd12515029869d0451"
        }
        """
        XCTAssertJSONEqual(output.json, expectedJSON)
    }
}
