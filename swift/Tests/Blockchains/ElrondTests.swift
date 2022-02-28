// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class ElrondTests: XCTestCase {

    let aliceBech32 = "erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th"
    let alicePubKeyHex = "0139472eff6886771a982f3083da5d421f24c29181e63888228dc81ca60d69e1"
    let aliceSeedHex = "413f42575f7f26fad3317a778771212fdb80245850981e48b58a4f25e344e8f9"
    let bobBech32 = "erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx"

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: aliceSeedHex)!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .elrond)
        let addressFromString = AnyAddress(string: aliceBech32, coin: .elrond)!

        XCTAssertEqual(pubkey.data.hexString, alicePubKeyHex)
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSignGenericAction() {
        let privateKey = PrivateKey(data: Data(hexString: aliceSeedHex)!)!

        let input = ElrondSigningInput.with {
            $0.genericAction = ElrondGenericAction.with {
                $0.accounts = ElrondAccounts.with {
                    $0.senderNonce = 7
                    $0.sender = aliceBech32
                    $0.receiver = bobBech32 
                }
                $0.value = "0"
                $0.data = "foo"
                $0.version = 1
            }
            $0.gasPrice = 1000000000
            $0.gasLimit = 50000
            $0.chainID = "1"
            $0.privateKey = privateKey.data
        }

        let output: ElrondSigningOutput = AnySigner.sign(input: input, coin: .elrond)
        let expectedSignature = "e8647dae8b16e034d518a1a860c6a6c38d16192d0f1362833e62424f424e5da660770dff45f4b951d9cc58bfb9d14559c977d443449bfc4b8783ff9c84065700"
        let expectedEncoded = #"{"nonce":7,"value":"0","receiver":"\#(bobBech32)","sender":"\#(aliceBech32)","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1,"signature":"\#(expectedSignature)"}"#

        XCTAssertEqual(output.signature, expectedSignature)
        XCTAssertEqual(output.encoded, expectedEncoded)
    }

    func testSignEGLDTransfer() {
        let privateKey = PrivateKey(data: Data(hexString: aliceSeedHex)!)!

        let input = ElrondSigningInput.with {
            $0.egldTransfer = ElrondEGLDTransfer.with {
                $0.accounts = ElrondAccounts.with {
                    $0.senderNonce = 7
                    $0.sender = aliceBech32
                    $0.receiver = bobBech32
                }
                $0.amount = "1000000000000000000"
            }
            $0.chainID = "1"
            $0.privateKey = privateKey.data
        }

        let output: ElrondSigningOutput = AnySigner.sign(input: input, coin: .elrond)
        let expectedSignature = "7e1c4c63b88ea72dcf7855a54463b1a424eb357ac3feb4345221e512ce07c7a50afb6d7aec6f480b554e32cf2037082f3bc17263d1394af1f3ef240be53c930b"
        let expectedEncoded = #"{"nonce":7,"value":"1000000000000000000","receiver":"\#(bobBech32)","sender":"\#(aliceBech32)","gasPrice":1000000000,"gasLimit":50000,"chainID":"1","version":1,"signature":"\#(expectedSignature)"}"#

        XCTAssertEqual(output.signature, expectedSignature)
        XCTAssertEqual(output.encoded, expectedEncoded)
    }

    func testSignESDTTransfer() {
        let privateKey = PrivateKey(data: Data(hexString: aliceSeedHex)!)!

        let input = ElrondSigningInput.with {
            $0.esdtTransfer = ElrondESDTTransfer.with {
                $0.accounts = ElrondAccounts.with {
                    $0.senderNonce = 7
                    $0.sender = aliceBech32
                    $0.receiver = bobBech32
                }
                $0.amount = "10000000000000"
                $0.tokenIdentifier = "MYTOKEN-1234"
            }

            $0.privateKey = privateKey.data
        }

        let output: ElrondSigningOutput = AnySigner.sign(input: input, coin: .elrond)
        let expectedSignature = "9add6d9ac3f1a1fddb07b934e8a73cad3b8c232bdf29d723c1b38ad619905f03e864299d06eb3fe3bbb48a9f1d9b7f14e21dc5eaffe0c87f5718ad0c4198bb0c"
        let expectedData = "RVNEVFRyYW5zZmVyQDRkNTk1NDRmNGI0NTRlMmQzMTMyMzMzNEAwOTE4NGU3MmEwMDA="
        let expectedEncoded = #"{"nonce":7,"value":"0","receiver":"\#(bobBech32)","sender":"\#(aliceBech32)","gasPrice":1000000000,"gasLimit":425000,"data":"\#(expectedData)","chainID":"1","version":1,"signature":"\#(expectedSignature)"}"#

        XCTAssertEqual(output.signature, expectedSignature)
        XCTAssertEqual(output.encoded, expectedEncoded)
    }

    func testSignESDTNFTTransfer() {
        let privateKey = PrivateKey(data: Data(hexString: aliceSeedHex)!)!

        let input = ElrondSigningInput.with {
            $0.esdtnftTransfer = ElrondESDTNFTTransfer.with {
                $0.accounts = ElrondAccounts.with {
                    $0.senderNonce = 7
                    $0.sender = aliceBech32
                    $0.receiver = bobBech32
                }
                $0.tokenCollection = "LKMEX-aab910"
                $0.tokenNonce = 4
                $0.amount = "184300000000000000"
            }

            $0.privateKey = privateKey.data
        }

        let output: ElrondSigningOutput = AnySigner.sign(input: input, coin: .elrond)
        let expectedSignature = "cc935685d5b31525e059a16a832cba98dee751983a5a93de4198f6553a2c55f5f1e0b4300fe9077376fa754546da0b0f6697e66462101a209aafd0fc775ab60a"
        let expectedData = "RVNEVE5GVFRyYW5zZmVyQDRjNGI0ZDQ1NTgyZDYxNjE2MjM5MzEzMEAwNEAwMjhlYzNkZmEwMWFjMDAwQDgwNDlkNjM5ZTVhNjk4MGQxY2QyMzkyYWJjY2U0MTAyOWNkYTc0YTE1NjM1MjNhMjAyZjA5NjQxY2MyNjE4Zjg="
        let expectedEncoded = #"{"nonce":7,"value":"0","receiver":"\#(aliceBech32)","sender":"\#(aliceBech32)","gasPrice":1000000000,"gasLimit":937500,"data":"\#(expectedData)","chainID":"1","version":1,"signature":"\#(expectedSignature)"}"#

        XCTAssertEqual(output.signature, expectedSignature)
        XCTAssertEqual(output.encoded, expectedEncoded)
    }
}
