// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
import XCTest
import WalletCore

class BluzelleAddressTests: XCTestCase {
    func testAddressValidation() {
        let bluzelle = CoinType.bluzelle
        for address in [
            "bluzelle1yhtq5zm293m2r3sp2guj9m5pg5e273n6r0szul",
            "bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund",
        ] {
            XCTAssertTrue(bluzelle.validate(address: address))
            XCTAssertEqual(bluzelle.address(string: address)?.description, address)
        }
    }
}

class BluzelleSignerTests: XCTestCase {

    let privateKeyData = Data(hexString: "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005")!
    let myAddress = "bluzelle1hsk6jryyqjfhp5dhc55tc9jtckygx0epzzw0fm"
    
    
    func testSigningJSON() {
        
        let toAddress = "bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"
        let inputJson =
        """
        {
          "accountNumber": "8733",
          "chainId": "bluzelle",
          "fee": {
            "amounts": [
              {
                "denom": "ubnt",
                "amount": "5000"
              }
            ],
            "gas": "200000"
          },
          "memo": "Testing",
          "messages": [
            {
              "sendCoinsMessage": {
                "fromAddress": "\(myAddress)",
                "toAddress": "\(toAddress)",
                "amounts": [
                  {
                    "denom": "ubnt",
                    "amount": "995000"
                  }
                ]
              }
            }
          ]
        }
        """
        
        let expectedSignedJson =
        """
        {
          "mode": "block",
          "tx": {
            "fee": {
              "amount": [
                {
                  "amount": "5000",
                  "denom": "ubnt"
                }
              ],
              "gas": "200000"
            },
            "memo": "Testing",
            "msg": [
              {
                "type": "cosmos-sdk/MsgSend",
                "value": {
                  "amount": [
                    {
                      "amount": "995000",
                      "denom": "ubnt"
                    }
                  ],
                  "from_address": "bluzelle1hsk6jryyqjfhp5dhc55tc9jtckygx0epzzw0fm",
                  "to_address": "bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"
                }
              }
            ],
            "signatures": [
              {
                "pub_key": {
                  "type": "tendermint/PubKeySecp256k1",
                  "value": "AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"
                },
                "signature": "gKWKk5lku9H/wosvnsD6auWUt7AGNjLEudWS+Fdv4TQr16HUBVpkq9A6gZYqZIawFukq2I/qrtyw9MKR4pVkhg=="
              }
            ]
          }
        }
        """

        let actualSignedJson = AnySigner.signJSON(inputJson, key: privateKeyData, coin: .bluzelle)
        
        XCTAssertJSONEqual(expectedSignedJson, actualSignedJson)

    }

    func testSigningMessage() {
        let privateKey = PrivateKey(data: privateKeyData)!

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = myAddress
            $0.toAddress = "bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"
            $0.amounts = [CosmosAmount.with {
                $0.amount = 1
                $0.denom = "ubnt"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = 200
                $0.denom = "ubnt"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 1037
            $0.chainID = "bluzelle"
            $0.memo = ""
            $0.sequence = 8
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .bluzelle)

        let expectedJSON: String =
        """
        {
          "mode": "block",
          "tx": {
            "fee": {
              "amount": [
                {
                  "amount": "200",
                  "denom": "ubnt"
                }
              ],
              "gas": "200000"
            },
            "memo": "",
            "msg": [
              {
                "type": "cosmos-sdk/MsgSend",
                "value": {
                  "amount": [
                    {
                      "amount": "1",
                      "denom": "ubnt"
                    }
                  ],
                  "from_address": "bluzelle1hsk6jryyqjfhp5dhc55tc9jtckygx0epzzw0fm",
                  "to_address": "bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"
                }
              }
            ],
            "signatures": [
              {
                "pub_key": {
                  "type": "tendermint/PubKeySecp256k1",
                  "value": "AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"
                },
                "signature": "0StN9LHr4fIbEzCzyx5bzWJmfgiUqkwoavYGNUDm2shXKmAFogVtOviC4zMNNHKBtFTnHQ07DO1UjqEEWS/2BA=="
              }
            ]
          }
        }
        """

        XCTAssertJSONEqual(expectedJSON, output.json)
    }
}
