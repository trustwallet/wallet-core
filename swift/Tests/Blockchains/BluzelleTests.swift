// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
import XCTest
import WalletCore

class BluzelleAddressTests: XCTestCase {

    func testAddressPublicKey() {

        let privateKeyData = Data(hexString: "1037f828ca313f4c9e120316e8e9ff25e17f07fe66ba557d5bc5e2eeb7cba8f6")!
        let privateKey = PrivateKey(data: privateKeyData)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)

        let expectedAddress = "bluzelle1jf9aaj9myrzsnmpdr7twecnaftzmku2myvn4dg"
        let actualAddress = AnyAddress(publicKey: publicKey, coin: .bluzelle).description

        let expectedPublicKeyData = "035df185566521d6a7802319ee06e1a28e97b7772dfb5fdd13ca6f0575518968e4"
        let actualPublicKeyData = publicKey.data.hexString

        XCTAssertEqual(expectedAddress, actualAddress)
        XCTAssertEqual(expectedPublicKeyData, actualPublicKeyData)
    }

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
        
        // Submitted realworld tx for the following test : https://bigdipper.net.bluzelle.com/transactions/DEF394BE0DD1075CDC8B8618A7858AAA4A03A43D04476381224E316E06FD3A5B

        let toAddress = "bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"
        let inputJson =
        """
        {
            "accountNumber": "590",
            "chainId": "net-6",
            "sequence": "3",
            "fee": {
                "amounts": [{
                    "denom": "ubnt",
                    "amount": "1000"
                }],
                "gas": "500000"
            },
            "memo": "Testing",
            "messages": [{
                "sendCoinsMessage": {
                    "fromAddress": "\(myAddress)",
                    "toAddress": "\(toAddress)",
                    "amounts": [{
                        "denom": "ubnt",
                        "amount": "2"
                    }]
                }
            }]
        }
        """
        
        let expectedSignedJson =
        """
        {"mode":"block","tx":{"fee":{"amount":[{"amount":"1000","denom":"ubnt"}],"gas":"500000"},"memo":"Testing","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"2","denom":"ubnt"}],"from_address":"bluzelle1hsk6jryyqjfhp5dhc55tc9jtckygx0epzzw0fm","to_address":"bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"2JcfVwq9N3UAk5Lfki7+CngqcefgjfH1q/8chtJMJvEHRe6PvuYKMv5pjeN0Z5Vk2BArJT3V3EHxbpbiY2eLWw=="}]}}
        """

        let actualSignedJson = AnySigner.signJSON(inputJson, key: privateKeyData, coin: .bluzelle)
        
        XCTAssertJSONEqual(expectedSignedJson, actualSignedJson)

    }

    func testSigningMessage() {
        // Submitted Realworld tx for the following test : https://bigdipper.net.bluzelle.com/transactions/B3A7F30539CCDF72D210BC995FAF65B43F9BE04FA9F8AFAE0EC969660744002F

        let privateKey = PrivateKey(data: privateKeyData)!

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = myAddress
            $0.toAddress = "bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"
            $0.amounts = [CosmosAmount.with {
                $0.amount = "1"
                $0.denom = "ubnt"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 500000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "1000"
                $0.denom = "ubnt"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.accountNumber = 590
            $0.chainID = "net-6"
            $0.memo = ""
            $0.sequence = 2
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .bluzelle)

        let expectedJSON: String =
        """
        {"mode":"block","tx":{"fee":{"amount":[{"amount":"1000","denom":"ubnt"}],"gas":"500000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"ubnt"}],"from_address":"bluzelle1hsk6jryyqjfhp5dhc55tc9jtckygx0epzzw0fm","to_address":"bluzelle1xccvees6ev4wm2r49rc6ptulsdxa8x8jfpmund"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"5e3e13x1F+Y4+cPNvE1jQ/Mrz0J2RQCY69re3g4xuTY3Gw7MNGQ+8E34d9DgvcNLPM05nehdOv/0SvekY/ihIQ=="}]}}
        """

        XCTAssertJSONEqual(expectedJSON, output.json)
    }
}
