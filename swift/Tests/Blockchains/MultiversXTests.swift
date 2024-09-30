// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class MultiversXTests: XCTestCase {

    let aliceBech32 = "erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th"
    let alicePubKeyHex = "0139472eff6886771a982f3083da5d421f24c29181e63888228dc81ca60d69e1"
    let aliceSeedHex = "413f42575f7f26fad3317a778771212fdb80245850981e48b58a4f25e344e8f9"
    let bobBech32 = "erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx"
    let carolBech32 = "erd1k2s324ww2g0yj38qn2ch2jwctdy8mnfxep94q9arncc6xecg3xaq6mjse8"

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: aliceSeedHex)!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .multiversX)
        let addressFromString = AnyAddress(string: aliceBech32, coin: .multiversX)!

        XCTAssertEqual(pubkey.data.hexString, alicePubKeyHex)
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSignGenericAction() {
        let privateKey = PrivateKey(data: Data(hexString: aliceSeedHex)!)!

        let input = MultiversXSigningInput.with {
            $0.genericAction = MultiversXGenericAction.with {
                $0.accounts = MultiversXAccounts.with {
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

        let output: MultiversXSigningOutput = AnySigner.sign(input: input, coin: .multiversX)
        let expectedSignature = "e8647dae8b16e034d518a1a860c6a6c38d16192d0f1362833e62424f424e5da660770dff45f4b951d9cc58bfb9d14559c977d443449bfc4b8783ff9c84065700"
        let expectedEncoded = #"{"nonce":7,"value":"0","receiver":"\#(bobBech32)","sender":"\#(aliceBech32)","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1,"signature":"\#(expectedSignature)"}"#

        XCTAssertEqual(output.signature, expectedSignature)
        XCTAssertEqual(output.encoded, expectedEncoded)
    }

    func testSignGenericActionWithGuardian() {
        let privateKey = PrivateKey(data: Data(hexString: aliceSeedHex)!)!

        let input = MultiversXSigningInput.with {
            $0.genericAction = MultiversXGenericAction.with {
                $0.accounts = MultiversXAccounts.with {
                    $0.senderNonce = 42
                    $0.sender = aliceBech32
                    $0.receiver = bobBech32
                    $0.guardian = carolBech32
                }
                $0.value = "1000000000000000000"
                $0.data = ""
                $0.version = 2
                $0.options = 2
            }
            $0.gasPrice = 1000000000
            $0.gasLimit = 100000
            $0.chainID = "1"
            $0.privateKey = privateKey.data
        }

        let output: MultiversXSigningOutput = AnySigner.sign(input: input, coin: .multiversX)
        let expectedSignature = "dae30e5cddb4a1f050009f939ce2c90843770870f9e6c77366be07e5cd7b3ebfdda38cd45d04e9070037d57761b6a68cee697e6043057f9dc565a4d0e632480d"
        let expectedEncoded = #"{"nonce":42,"value":"1000000000000000000","receiver":"\#(bobBech32)","sender":"\#(aliceBech32)","gasPrice":1000000000,"gasLimit":100000,"chainID":"1","version":2,"signature":"\#(expectedSignature)","options":2,"guardian":"\#(carolBech32)"}"#

        XCTAssertEqual(output.signature, expectedSignature)
        XCTAssertEqual(output.encoded, expectedEncoded)
    }
    
    func testSignGenericActionUndelegate() {
        // Successfully broadcasted https://explorer.multiversx.com/transactions/3301ae5a6a77f0ab9ceb5125258f12539a113b0c6787de76a5c5867f2c515d65
        let privateKey = PrivateKey(data: Data(hexString: aliceSeedHex)!)!

        let input = MultiversXSigningInput.with {
            $0.genericAction = MultiversXGenericAction.with {
                $0.accounts = MultiversXAccounts.with {
                    $0.senderNonce = 6
                    $0.sender = "erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa"
                    $0.receiver = "erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r"
                }
                $0.value = "0"
                $0.data = "unDelegate@0de0b6b3a7640000"
                $0.version = 1
            }
            $0.gasPrice = 1000000000
            $0.gasLimit = 12000000
            $0.chainID = "1"
            $0.privateKey = privateKey.data
        }

        let output: MultiversXSigningOutput = AnySigner.sign(input: input, coin: .multiversX)
        let expectedSignature = "89f9683af92f7b835bff4e1d0dbfcff5245b3367df4d23538eb799e0ad0a90be29ac3bd3598ce55b35b35ebef68bfa5738eed39fd01adc33476f65bd1b966e0b"
        let expectedEncoded = #"{"nonce":6,"value":"0","receiver":"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r","sender":"erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa","gasPrice":1000000000,"gasLimit":12000000,"data":"dW5EZWxlZ2F0ZUAwZGUwYjZiM2E3NjQwMDAw","chainID":"1","version":1,"signature":"\#(expectedSignature)"}"#

        XCTAssertEqual(output.signature, expectedSignature)
        XCTAssertEqual(output.encoded, expectedEncoded)
    }
    
    func testSignGenericActionDelegate() {
        // Successfully broadcasted https://explorer.multiversx.com/transactions/e5007662780f8ed677b37b156007c24bf60b7366000f66ec3525cfa16a4564e7
        let privateKey = PrivateKey(data: Data(hexString: aliceSeedHex)!)!

        let input = MultiversXSigningInput.with {
            $0.genericAction = MultiversXGenericAction.with {
                $0.accounts = MultiversXAccounts.with {
                    $0.senderNonce = 1
                    $0.sender = "erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa"
                    $0.receiver = "erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r"
                }
                $0.value = "1"
                $0.data = "delegate"
                $0.version = 1
            }
            $0.gasPrice = 1000000000
            $0.gasLimit = 12000000
            $0.chainID = "1"
            $0.privateKey = privateKey.data
        }

        let output: MultiversXSigningOutput = AnySigner.sign(input: input, coin: .multiversX)
        let expectedSignature = "3b9164d47a4e3c0330ae387cd29ba6391f9295acf5e43a16a4a2611645e66e5fa46bf22294ca68fe1948adf45cec8cb47b8792afcdb248bd9adec7c6e6c27108"
        let expectedEncoded = #"{"nonce":1,"value":"1","receiver":"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r","sender":"erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa","gasPrice":1000000000,"gasLimit":12000000,"data":"ZGVsZWdhdGU=","chainID":"1","version":1,"signature":"\#(expectedSignature)"}"#

        XCTAssertEqual(output.signature, expectedSignature)
        XCTAssertEqual(output.encoded, expectedEncoded)
    }

    func testSignEGLDTransfer() {
        let privateKey = PrivateKey(data: Data(hexString: aliceSeedHex)!)!

        let input = MultiversXSigningInput.with {
            $0.egldTransfer = MultiversXEGLDTransfer.with {
                $0.accounts = MultiversXAccounts.with {
                    $0.senderNonce = 7
                    $0.sender = aliceBech32
                    $0.receiver = bobBech32
                }
                $0.amount = "1000000000000000000"
            }
            $0.chainID = "1"
            $0.privateKey = privateKey.data
        }

        let output: MultiversXSigningOutput = AnySigner.sign(input: input, coin: .multiversX)
        let expectedSignature = "0f40dec9d37bde3c67803fc535088e536344e271807bb7c1aa24af3c69bffa9b705e149ff7bcaf21678f4900c4ee72741fa6ef08bf4c67fc6da1c6b0f337730e"
        let expectedEncoded = #"{"nonce":7,"value":"1000000000000000000","receiver":"\#(bobBech32)","sender":"\#(aliceBech32)","gasPrice":1000000000,"gasLimit":50000,"chainID":"1","version":2,"signature":"\#(expectedSignature)"}"#

        XCTAssertEqual(output.signature, expectedSignature)
        XCTAssertEqual(output.encoded, expectedEncoded)
    }

    func testSignEGLDTransferWithGuardian() {
        let privateKey = PrivateKey(data: Data(hexString: aliceSeedHex)!)!

        let input = MultiversXSigningInput.with {
            $0.egldTransfer = MultiversXEGLDTransfer.with {
                $0.accounts = MultiversXAccounts.with {
                    $0.senderNonce = 7
                    $0.sender = aliceBech32
                    $0.receiver = bobBech32
                    $0.guardian = carolBech32
                }
                $0.amount = "1000000000000000000"
            }
            $0.chainID = "1"
            $0.privateKey = privateKey.data
        }

        let output: MultiversXSigningOutput = AnySigner.sign(input: input, coin: .multiversX)
        let expectedSignature = "741dd0d24db4df37a050f693f8481b6e51b8dd6dfc2f01a4f90aa1af3e59c89a8b0ef9d710af33103970e353d9f0cb9fd128a2e174731cbc88265d9737ed5604"
        let expectedEncoded = #"{"nonce":7,"value":"1000000000000000000","receiver":"\#(bobBech32)","sender":"\#(aliceBech32)","gasPrice":1000000000,"gasLimit":100000,"chainID":"1","version":2,"signature":"\#(expectedSignature)","options":2,"guardian":"\#(carolBech32)"}"#

        XCTAssertEqual(output.signature, expectedSignature)
        XCTAssertEqual(output.encoded, expectedEncoded)
    }

    func testSignESDTTransfer() {
        let privateKey = PrivateKey(data: Data(hexString: aliceSeedHex)!)!

        let input = MultiversXSigningInput.with {
            $0.esdtTransfer = MultiversXESDTTransfer.with {
                $0.accounts = MultiversXAccounts.with {
                    $0.senderNonce = 7
                    $0.sender = aliceBech32
                    $0.receiver = bobBech32
                }
                $0.amount = "10000000000000"
                $0.tokenIdentifier = "MYTOKEN-1234"
            }

            $0.privateKey = privateKey.data
        }

        let output: MultiversXSigningOutput = AnySigner.sign(input: input, coin: .multiversX)
        let expectedSignature = "dd7cdc90aa09da6034b00a99e3ba0f1a2a38fa788fad018d53bf2e706f99e1a42c80063c28e6b48a5f2574c4054986f34c8eb36b1da63a22d19cf3ea5990b306"
        let expectedData = "RVNEVFRyYW5zZmVyQDRkNTk1NDRmNGI0NTRlMmQzMTMyMzMzNEAwOTE4NGU3MmEwMDA="
        let expectedEncoded = #"{"nonce":7,"value":"0","receiver":"\#(bobBech32)","sender":"\#(aliceBech32)","gasPrice":1000000000,"gasLimit":425000,"data":"\#(expectedData)","chainID":"1","version":2,"signature":"\#(expectedSignature)"}"#

        XCTAssertEqual(output.signature, expectedSignature)
        XCTAssertEqual(output.encoded, expectedEncoded)
    }

    func testSignESDTNFTTransfer() {
        let privateKey = PrivateKey(data: Data(hexString: aliceSeedHex)!)!

        let input = MultiversXSigningInput.with {
            $0.esdtnftTransfer = MultiversXESDTNFTTransfer.with {
                $0.accounts = MultiversXAccounts.with {
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

        let output: MultiversXSigningOutput = AnySigner.sign(input: input, coin: .multiversX)
        let expectedSignature = "59af89d9a9ece1f35bc34323c42061cae27bb5f9830f5eb26772e680732cbd901a86caa7c3eadacd392fe1024bef4c1f08ce1dfcafec257d6f41444ccea30a0c"
        let expectedData = "RVNEVE5GVFRyYW5zZmVyQDRjNGI0ZDQ1NTgyZDYxNjE2MjM5MzEzMEAwNEAwMjhlYzNkZmEwMWFjMDAwQDgwNDlkNjM5ZTVhNjk4MGQxY2QyMzkyYWJjY2U0MTAyOWNkYTc0YTE1NjM1MjNhMjAyZjA5NjQxY2MyNjE4Zjg="
        let expectedEncoded = #"{"nonce":7,"value":"0","receiver":"\#(aliceBech32)","sender":"\#(aliceBech32)","gasPrice":1000000000,"gasLimit":937500,"data":"\#(expectedData)","chainID":"1","version":2,"signature":"\#(expectedSignature)"}"#

        XCTAssertEqual(output.signature, expectedSignature)
        XCTAssertEqual(output.encoded, expectedEncoded)
    }
}
