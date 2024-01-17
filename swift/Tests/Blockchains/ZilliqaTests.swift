// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class ZilliqaTests: XCTestCase {

    let coin = CoinType.zilliqa
    let privateKey = PrivateKey(data: Data(hexString: "0x68ffa8ec149ce50da647166036555f73d57f662eb420e154621e5f24f6cf9748")!)!

    func testConfig() {
        XCTAssertEqual(coin.hrp, .zilliqa)

        let address1 = coin.address(string: "0x7FCcaCf066a5F26Ee3AFfc2ED1FA9810Deaa632C")
        XCTAssertNil(address1)

        let address2 = coin.address(string: "zil10lx2eurx5hexaca0lshdr75czr025cevqu83uz")
        XCTAssertEqual("zil10lx2eurx5hexaca0lshdr75czr025cevqu83uz", address2?.description)
    }

    func testAddress() {
        let data = Data(hexString: "029d25b68a18442590e113132a34bb524695c4291d2c49abf2e4cdd7d98db862c3")!
        let pubKey = PublicKey(data: data, type: .secp256k1)!
        let keyHash = "7FCcaCf066a5F26Ee3AFfc2ED1FA9810Deaa632C"
        let address = AnyAddress(publicKey: pubKey, coin: .zilliqa)
        let address2 = AnyAddress(string: "zil10lx2eurx5hexaca0lshdr75czr025cevqu83uz", coin: .zilliqa)!

        XCTAssertEqual(String(data: address.data, encoding: .utf8)!, keyHash)
        XCTAssertEqual(address.description, address2.description)
    }

    func testSignTransfer() {
        let input = ZilliqaSigningInput.with {
            $0.version = 65537 // mainnet tx version
            $0.nonce = 2
            $0.to = "zil10lx2eurx5hexaca0lshdr75czr025cevqu83uz"

            $0.gasPrice = Data(hexString: "3b9aca00")!
            $0.gasLimit = 1
            $0.privateKey = privateKey.data
            $0.transaction = ZilliqaTransaction.with {
                $0.transfer = ZilliqaTransaction.Transfer.with {
                    // 1 ZIL
                    $0.amount = Data(hexString: "e8d4a51000")!
                }
            }
        }

        let output: ZilliqaSigningOutput = AnySigner.sign(input: input, coin: .zilliqa)
        let expectedJSON = """
        {
            "amount": "1000000000000",
            "toAddr": "7FCcaCf066a5F26Ee3AFfc2ED1FA9810Deaa632C",
            "pubKey": "03fb30b196ce3e976593ecc2da220dca9cdea8c84d2373770042a930b892ac0f5c",
            "data": "",
            "code": "",
            "signature": "001fa4df08c11a4a79e96e69399ee48eeecc78231a78b0355a8ca783c77c139436e37934fecc2252ed8dac00e235e22d18410461fb896685c4270642738ed268",
            "gasLimit": "1",
            "version": 65537,
            "gasPrice": "1000000000",
            "nonce": 2
        }
        """
        XCTAssertEqual(output.signature.hexString, "001fa4df08c11a4a79e96e69399ee48eeecc78231a78b0355a8ca783c77c139436e37934fecc2252ed8dac00e235e22d18410461fb896685c4270642738ed268")
        XCTAssertJSONEqual(output.json, expectedJSON)
    }

    func testSignData() throws {

        // https://viewblock.io/zilliqa/tx/0x6228b3d7e69fc3481b84fd00e892cec359a41654f58948ff7b1b932396b00ad9
        // real key 1p
        let key = privateKey

        let delegator = AnyAddress(string: "zil1zg3pnn82ksgfq85kcwsw2hjxyv2gqdqmnr6jwc", coin: .zilliqa)!
        let delegatorHex = "0x" + String(data: delegator.data, encoding: .utf8)!

        let json: [String: Any] = [
            "_tag": "DelegateStake",
            "params": [
                [
                    "type": "ByStr20",
                    "value": delegatorHex,
                    "vname": "ssnaddr"
                ]
            ]
        ]
        let jsonData = try JSONSerialization.data(withJSONObject: json, options: [.sortedKeys])

        let input = ZilliqaSigningInput.with {
            $0.version = 65537 // mainnet tx version
            $0.nonce = 56
            $0.to = "zil1g029nmzsf36r99vupp4s43lhs40fsscx3jjpuy"

            $0.gasPrice = Data(hexString: "77359400")!
            $0.gasLimit = 5000
            $0.privateKey = key.data
            $0.transaction = ZilliqaTransaction.with {
                $0.rawTransaction = ZilliqaTransaction.Raw.with {
                    // 10 ZIL
                    $0.amount = Data(hexString: "09184e72a000")!
                    $0.data = jsonData
                }
            }
        }

        let output: ZilliqaSigningOutput = AnySigner.sign(input: input, coin: .zilliqa)

        let url = Bundle(for: ZilliqaTests.self).url(forResource: "zilliqa_data_tx", withExtension: "json")!
        let expected = try String(contentsOf: url)

        XCTAssertJSONEqual(expected, output.json)
    }

    func testSignJSON() {
        let json = """
            {"version":65537,"nonce":"2","to":"zil10lx2eurx5hexaca0lshdr75czr025cevqu83uz","gasPrice":"O5rKAA==","gasLimit":"1","transaction":{"transfer":{"amount":"6NSlEAA="}}}
        """
        let key = Data(hexString: "0x68ffa8ec149ce50da647166036555f73d57f662eb420e154621e5f24f6cf9748")!
        let result = AnySigner.signJSON(json, key: key, coin: .zilliqa)

        XCTAssertEqual(result, "7b22616d6f756e74223a2231303030303030303030303030222c22636f6465223a22222c2264617461223a22222c226761734c696d6974223a2231222c226761735072696365223a2231303030303030303030222c226e6f6e6365223a322c227075624b6579223a22303366623330623139366365336539373635393365636332646132323064636139636465613863383464323337333737303034326139333062383932616330663563222c227369676e6174757265223a223030316661346466303863313161346137396539366536393339396565343865656563633738323331613738623033353561386361373833633737633133393433366533373933346665636332323532656438646163303065323335653232643138343130343631666238393636383563343237303634323733386564323638222c22746f41646472223a2237464363614366303636613546323645653341466663324544314641393831304465616136333243222c2276657273696f6e223a36353533377d")
    }
}
