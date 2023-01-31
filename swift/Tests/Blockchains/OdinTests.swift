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
    
    func testCoinSend() {
        let wallet = HDWallet(mnemonic: "shoot island position soft burden budget tooth cruel issue economy destroy above", passphrase: "")!
        let privateKey = wallet.getKey(coin: .odin, derivationPath: "m/44\'/118\'/0\'/0/0")
        let from = wallet.getAddressForCoin(coin: .odin)

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = from.description
            $0.toAddress = "odin13d7rfa8v8jmmsh63jetd9euqftaqfc3hre5ake"
            $0.amounts = [CosmosAmount.with {
                $0.amount = "20000"
                $0.denom = "loki"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 80000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "3200"
                $0.denom = "loki"
            }]
        }
        
        
        let url = URL(string: "https://node.odin-freya-website.odinprotocol.io/mainnet/a/api/cosmos/auth/v1beta1/accounts/" + from)!
            
        let expectation = XCTestExpectation(description: "Esperando la respuesta de la llamada de red XXX")
        let session = URLSession.shared
        session.dataTask(with: url) { data, response, error in
            guard let data = data, error == nil else { return }

            do {
                let json = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as! [String:Any]
                let account = json["account"]
                if let accountDict = account as? [String:Any],
                   let seq = accountDict["sequence"] as? String,
                   let accountNum = accountDict["account_number"] as? String
                {
                    let input = CosmosSigningInput.with {
                        $0.signingMode = .protobuf;
                        $0.accountNumber = UInt64(accountNum) ?? 0
                        $0.chainID = "odin-mainnet-freya"
                        $0.memo = ""
                        $0.sequence = UInt64(seq) ?? 0
                        $0.messages = [message]
                        $0.fee = fee
                        $0.privateKey = privateKey.data
                    }
                    let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .odin)
                    
                    let simulateUrl = URL(string: "https://node.odin-freya-website.odinprotocol.io/mainnet/a/api/cosmos/tx/v1beta1/simulate")!
                    var request = URLRequest(url: simulateUrl)
                    request.httpMethod = "POST"
                    let cleanOutput = output.serialized.replacingOccurrences(of: "\\", with: "")
                    request.httpBody = Data(cleanOutput.utf8)
                    let simulateSession = URLSession.shared
                    simulateSession.dataTask(with: request) { data, response, error in
                        guard let data = data, error == nil else { return }

                        do {
                            let json = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as! [String:Any]
                            let gasInfo = json["gas_info"]
                            if let gasInfoDict = gasInfo as? [String:Any],
                               let gasUsed = gasInfoDict["gas_used"] as? String
                            {
                                let estimatedFee = CosmosFee.with {
                                    $0.gas = UInt64((Double(gasUsed) ?? 80000) * 1.2)
                                    $0.amounts = [CosmosAmount.with {
                                        $0.amount = "3200"
                                        $0.denom = "loki"
                                    }]
                                }
                                
                                let estimatedInput = CosmosSigningInput.with {
                                    $0.signingMode = .protobuf;
                                    $0.accountNumber = UInt64(accountNum) ?? 0
                                    $0.chainID = "odin-mainnet-freya"
                                    $0.memo = ""
                                    $0.sequence = UInt64(seq) ?? 0
                                    $0.messages = [message]
                                    $0.fee = estimatedFee
                                    $0.privateKey = privateKey.data
                                }
                                let estimatedOutput: CosmosSigningOutput = AnySigner.sign(input: estimatedInput, coin: .odin)
                                
                                let sendTxUrl = URL(string: "https://node.odin-freya-website.odinprotocol.io/mainnet/a/api/cosmos/tx/v1beta1/txs")!
                                var sendTxRequest = URLRequest(url: sendTxUrl)
                                sendTxRequest.httpMethod = "POST"
                                let sendTxCleanOutput = estimatedOutput.serialized.replacingOccurrences(of: "\\", with: "")
                                sendTxRequest.httpBody = Data(sendTxCleanOutput.utf8)
                                let sendTxSession = URLSession.shared
                                sendTxSession.dataTask(with: sendTxRequest) { data, response, error in
                                    guard let data = data, error == nil else { return }

                                    do {
                                        let json = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as! [String:Any]
                                        let txResponse = json["tx_response"]
                                        if let txResponseDict = txResponse as? [String:Any],
                                           let txHash = txResponseDict["txhash"] as? String
                                        {
                                            print(txHash)
                                        }
                                    } catch let error as NSError {
                                        print(error)
                                    }
                                    expectation.fulfill()
                                }.resume()
                            }
                        } catch let error as NSError {
                            print(error)
                        }
                    }.resume()
                }
            } catch let error as NSError {
                print(error)
            }
           
        }.resume()
        wait(for: [expectation], timeout: 300.0)
    }
    
    func testDelegate() {
        let wallet = HDWallet(mnemonic: "shoot island position soft burden budget tooth cruel issue economy destroy above", passphrase: "")!
        let privateKey = wallet.getKey(coin: .odin, derivationPath: "m/44\'/118\'/0\'/0/0")
        let from = wallet.getAddressForCoin(coin: .odin)

        let stakeMessage = CosmosMessage.Delegate.with {
            $0.delegatorAddress = from.description
            $0.validatorAddress = "odinvaloper1tkv6yfg7ukeea58tf3fyhtmdm2hely8d8hnn2m"
            $0.amount = CosmosAmount.with {
                $0.amount = "20000"
                $0.denom = "loki"
            }
        }

        let message = CosmosMessage.with {
            $0.stakeMessage = stakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 80000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "3200"
                $0.denom = "loki"
            }]
        }
        
        
        let url = URL(string: "https://node.odin-freya-website.odinprotocol.io/mainnet/a/api/cosmos/auth/v1beta1/accounts/" + from)!
            
        let expectation = XCTestExpectation(description: "Esperando la respuesta de la llamada de red XXX")
        let session = URLSession.shared
        session.dataTask(with: url) { data, response, error in
            guard let data = data, error == nil else { return }

            do {
                let json = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as! [String:Any]
                let account = json["account"]
                if let accountDict = account as? [String:Any],
                   let seq = accountDict["sequence"] as? String,
                   let accountNum = accountDict["account_number"] as? String
                {
                    let input = CosmosSigningInput.with {
                        $0.signingMode = .protobuf;
                        $0.accountNumber = UInt64(accountNum) ?? 0
                        $0.chainID = "odin-mainnet-freya"
                        $0.memo = ""
                        $0.sequence = UInt64(seq) ?? 0
                        $0.messages = [message]
                        $0.fee = fee
                        $0.privateKey = privateKey.data
                    }
                    let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .odin)
                    
                    let simulateUrl = URL(string: "https://node.odin-freya-website.odinprotocol.io/mainnet/a/api/cosmos/tx/v1beta1/simulate")!
                    var request = URLRequest(url: simulateUrl)
                    request.httpMethod = "POST"
                    let cleanOutput = output.serialized.replacingOccurrences(of: "\\", with: "")
                    request.httpBody = Data(cleanOutput.utf8)
                    let simulateSession = URLSession.shared
                    simulateSession.dataTask(with: request) { data, response, error in
                        guard let data = data, error == nil else { return }

                        do {
                            let json = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as! [String:Any]
                            let gasInfo = json["gas_info"]
                            if let gasInfoDict = gasInfo as? [String:Any],
                               let gasUsed = gasInfoDict["gas_used"] as? String
                            {
                                let estimatedFee = CosmosFee.with {
                                    $0.gas = UInt64((Double(gasUsed) ?? 80000) * 1.2)
                                    $0.amounts = [CosmosAmount.with {
                                        $0.amount = "3200"
                                        $0.denom = "loki"
                                    }]
                                }
                                
                                let estimatedInput = CosmosSigningInput.with {
                                    $0.signingMode = .protobuf;
                                    $0.accountNumber = UInt64(accountNum) ?? 0
                                    $0.chainID = "odin-mainnet-freya"
                                    $0.memo = ""
                                    $0.sequence = UInt64(seq) ?? 0
                                    $0.messages = [message]
                                    $0.fee = estimatedFee
                                    $0.privateKey = privateKey.data
                                }
                                let estimatedOutput: CosmosSigningOutput = AnySigner.sign(input: estimatedInput, coin: .odin)
                                
                                let sendTxUrl = URL(string: "https://node.odin-freya-website.odinprotocol.io/mainnet/a/api/cosmos/tx/v1beta1/txs")!
                                var sendTxRequest = URLRequest(url: sendTxUrl)
                                sendTxRequest.httpMethod = "POST"
                                let sendTxCleanOutput = estimatedOutput.serialized.replacingOccurrences(of: "\\", with: "")
                                sendTxRequest.httpBody = Data(sendTxCleanOutput.utf8)
                                let sendTxSession = URLSession.shared
                                sendTxSession.dataTask(with: sendTxRequest) { data, response, error in
                                    guard let data = data, error == nil else { return }

                                    do {
                                        let json = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as! [String:Any]
                                        let txResponse = json["tx_response"]
                                        if let txResponseDict = txResponse as? [String:Any],
                                           let txHash = txResponseDict["txhash"] as? String
                                        {
                                            print(txHash)
                                        }
                                    } catch let error as NSError {
                                        print(error)
                                    }
                                    expectation.fulfill()
                                }.resume()
                            }
                        } catch let error as NSError {
                            print(error)
                        }
                    }.resume()
                }
            } catch let error as NSError {
                print(error)
            }
           
        }.resume()
        wait(for: [expectation], timeout: 300.0)
    }
    
    func testUndelegate() {
        let wallet = HDWallet(mnemonic: "shoot island position soft burden budget tooth cruel issue economy destroy above", passphrase: "")!
        let privateKey = wallet.getKey(coin: .odin, derivationPath: "m/44\'/118\'/0\'/0/0")
        let from = wallet.getAddressForCoin(coin: .odin)

        let unstakeMessage = CosmosMessage.Undelegate.with {
            $0.delegatorAddress = from.description
            $0.validatorAddress = "odinvaloper1tkv6yfg7ukeea58tf3fyhtmdm2hely8d8hnn2m"
            $0.amount = CosmosAmount.with {
                $0.amount = "20000"
                $0.denom = "loki"
            }
        }

        let message = CosmosMessage.with {
            $0.unstakeMessage = unstakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 80000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "3200"
                $0.denom = "loki"
            }]
        }
        
        
        let url = URL(string: "https://node.odin-freya-website.odinprotocol.io/mainnet/a/api/cosmos/auth/v1beta1/accounts/" + from)!
            
        let expectation = XCTestExpectation(description: "Esperando la respuesta de la llamada de red XXX")
        let session = URLSession.shared
        session.dataTask(with: url) { data, response, error in
            guard let data = data, error == nil else { return }

            do {
                let json = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as! [String:Any]
                let account = json["account"]
                if let accountDict = account as? [String:Any],
                   let seq = accountDict["sequence"] as? String,
                   let accountNum = accountDict["account_number"] as? String
                {
                    let input = CosmosSigningInput.with {
                        $0.signingMode = .protobuf;
                        $0.accountNumber = UInt64(accountNum) ?? 0
                        $0.chainID = "odin-mainnet-freya"
                        $0.memo = ""
                        $0.sequence = UInt64(seq) ?? 0
                        $0.messages = [message]
                        $0.fee = fee
                        $0.privateKey = privateKey.data
                    }
                    let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .odin)
                    
                    let simulateUrl = URL(string: "https://node.odin-freya-website.odinprotocol.io/mainnet/a/api/cosmos/tx/v1beta1/simulate")!
                    var request = URLRequest(url: simulateUrl)
                    request.httpMethod = "POST"
                    let cleanOutput = output.serialized.replacingOccurrences(of: "\\", with: "")
                    request.httpBody = Data(cleanOutput.utf8)
                    let simulateSession = URLSession.shared
                    simulateSession.dataTask(with: request) { data, response, error in
                        guard let data = data, error == nil else { return }

                        do {
                            let json = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as! [String:Any]
                            let gasInfo = json["gas_info"]
                            if let gasInfoDict = gasInfo as? [String:Any],
                               let gasUsed = gasInfoDict["gas_used"] as? String
                            {
                                let estimatedFee = CosmosFee.with {
                                    $0.gas = UInt64((Double(gasUsed) ?? 80000) * 1.2)
                                    $0.amounts = [CosmosAmount.with {
                                        $0.amount = "3200"
                                        $0.denom = "loki"
                                    }]
                                }
                                
                                let estimatedInput = CosmosSigningInput.with {
                                    $0.signingMode = .protobuf;
                                    $0.accountNumber = UInt64(accountNum) ?? 0
                                    $0.chainID = "odin-mainnet-freya"
                                    $0.memo = ""
                                    $0.sequence = UInt64(seq) ?? 0
                                    $0.messages = [message]
                                    $0.fee = estimatedFee
                                    $0.privateKey = privateKey.data
                                }
                                let estimatedOutput: CosmosSigningOutput = AnySigner.sign(input: estimatedInput, coin: .odin)
                                
                                let sendTxUrl = URL(string: "https://node.odin-freya-website.odinprotocol.io/mainnet/a/api/cosmos/tx/v1beta1/txs")!
                                var sendTxRequest = URLRequest(url: sendTxUrl)
                                sendTxRequest.httpMethod = "POST"
                                let sendTxCleanOutput = estimatedOutput.serialized.replacingOccurrences(of: "\\", with: "")
                                sendTxRequest.httpBody = Data(sendTxCleanOutput.utf8)
                                let sendTxSession = URLSession.shared
                                sendTxSession.dataTask(with: sendTxRequest) { data, response, error in
                                    guard let data = data, error == nil else { return }

                                    do {
                                        let json = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as! [String:Any]
                                        let txResponse = json["tx_response"]
                                        if let txResponseDict = txResponse as? [String:Any],
                                           let txHash = txResponseDict["txhash"] as? String
                                        {
                                            print(txHash)
                                        }
                                    } catch let error as NSError {
                                        print(error)
                                    }
                                    expectation.fulfill()
                                }.resume()
                            }
                        } catch let error as NSError {
                            print(error)
                        }
                    }.resume()
                }
            } catch let error as NSError {
                print(error)
            }
           
        }.resume()
        wait(for: [expectation], timeout: 300.0)
    }
    
    func testDelegateTransfer() {
        let wallet = HDWallet(mnemonic: "shoot island position soft burden budget tooth cruel issue economy destroy above", passphrase: "")!
        let privateKey = wallet.getKey(coin: .odin, derivationPath: "m/44\'/118\'/0\'/0/0")
        let from = wallet.getAddressForCoin(coin: .odin)

        let delegateTransferMessage = CosmosMessage.BeginRedelegate.with {
            $0.delegatorAddress = from.description
            $0.validatorSrcAddress = "odinvaloper103yy6yxn7u3tfs25fr4xt4hyfq9h44ydw98n3z"
            $0.validatorDstAddress = "odinvaloper1tkv6yfg7ukeea58tf3fyhtmdm2hely8d8hnn2m"
            $0.amount = CosmosAmount.with {
                $0.amount = "200000"
                $0.denom = "loki"
            }
        }

        let message = CosmosMessage.with {
            $0.restakeMessage = delegateTransferMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 80000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "5000"
                $0.denom = "loki"
            }]
        }
        
        
        let url = URL(string: "https://node.odin-freya-website.odinprotocol.io/mainnet/a/api/cosmos/auth/v1beta1/accounts/" + from)!
            
        let expectation = XCTestExpectation(description: "Esperando la respuesta de la llamada de red XXX")
        let session = URLSession.shared
        session.dataTask(with: url) { data, response, error in
            guard let data = data, error == nil else { return }

            do {
                let json = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as! [String:Any]
                let account = json["account"]
                if let accountDict = account as? [String:Any],
                   let seq = accountDict["sequence"] as? String,
                   let accountNum = accountDict["account_number"] as? String
                {
                    let input = CosmosSigningInput.with {
                        $0.signingMode = .protobuf;
                        $0.accountNumber = UInt64(accountNum) ?? 0
                        $0.chainID = "odin-mainnet-freya"
                        $0.memo = ""
                        $0.sequence = UInt64(seq) ?? 0
                        $0.messages = [message]
                        $0.fee = fee
                        $0.privateKey = privateKey.data
                    }
                    let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .odin)
                    
                    let simulateUrl = URL(string: "https://node.odin-freya-website.odinprotocol.io/mainnet/a/api/cosmos/tx/v1beta1/simulate")!
                    var request = URLRequest(url: simulateUrl)
                    request.httpMethod = "POST"
                    let cleanOutput = output.serialized.replacingOccurrences(of: "\\", with: "")
                    request.httpBody = Data(cleanOutput.utf8)
                    let simulateSession = URLSession.shared
                    simulateSession.dataTask(with: request) { data, response, error in
                        guard let data = data, error == nil else { return }

                        do {
                            let json = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as! [String:Any]
                            let gasInfo = json["gas_info"]
                            if let gasInfoDict = gasInfo as? [String:Any],
                               let gasUsed = gasInfoDict["gas_used"] as? String
                            {
                                let estimatedFee = CosmosFee.with {
                                    $0.gas = UInt64((Double(gasUsed) ?? 80000) * 1.2)
                                    $0.amounts = [CosmosAmount.with {
                                        $0.amount = "5000"
                                        $0.denom = "loki"
                                    }]
                                }
                                
                                let estimatedInput = CosmosSigningInput.with {
                                    $0.signingMode = .protobuf;
                                    $0.accountNumber = UInt64(accountNum) ?? 0
                                    $0.chainID = "odin-mainnet-freya"
                                    $0.memo = ""
                                    $0.sequence = UInt64(seq) ?? 0
                                    $0.messages = [message]
                                    $0.fee = estimatedFee
                                    $0.privateKey = privateKey.data
                                }
                                let estimatedOutput: CosmosSigningOutput = AnySigner.sign(input: estimatedInput, coin: .odin)
                                
                                let sendTxUrl = URL(string: "https://node.odin-freya-website.odinprotocol.io/mainnet/a/api/cosmos/tx/v1beta1/txs")!
                                var sendTxRequest = URLRequest(url: sendTxUrl)
                                sendTxRequest.httpMethod = "POST"
                                let sendTxCleanOutput = estimatedOutput.serialized.replacingOccurrences(of: "\\", with: "")
                                sendTxRequest.httpBody = Data(sendTxCleanOutput.utf8)
                                let sendTxSession = URLSession.shared
                                sendTxSession.dataTask(with: sendTxRequest) { data, response, error in
                                    guard let data = data, error == nil else { return }

                                    do {
                                        let json = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as! [String:Any]
                                        let txResponse = json["tx_response"]
                                        if let txResponseDict = txResponse as? [String:Any],
                                           let txHash = txResponseDict["txhash"] as? String
                                        {
                                            print(txHash)
                                        }
                                    } catch let error as NSError {
                                        print(error)
                                    }
                                    expectation.fulfill()
                                }.resume()
                            }
                        } catch let error as NSError {
                            print(error)
                        }
                    }.resume()
                }
            } catch let error as NSError {
                print(error)
            }
           
        }.resume()
        wait(for: [expectation], timeout: 300.0)
    }
    
    func testClaimRewards() {
        let wallet = HDWallet(mnemonic: "shoot island position soft burden budget tooth cruel issue economy destroy above", passphrase: "")!
        let privateKey = wallet.getKey(coin: .odin, derivationPath: "m/44\'/118\'/0\'/0/0")
        let from = wallet.getAddressForCoin(coin: .odin)

        let claimRewardsMessage = CosmosMessage.WithdrawDelegationReward.with {
            $0.delegatorAddress = from.description
            $0.validatorAddress = "odinvaloper1tkv6yfg7ukeea58tf3fyhtmdm2hely8d8hnn2m"

        }

        let message = CosmosMessage.with {
            $0.withdrawStakeRewardMessage = claimRewardsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 80000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "3200"
                $0.denom = "loki"
            }]
        }
        
        
        let url = URL(string: "https://node.odin-freya-website.odinprotocol.io/mainnet/a/api/cosmos/auth/v1beta1/accounts/" + from)!
            
        let expectation = XCTestExpectation(description: "Esperando la respuesta de la llamada de red XXX")
        let session = URLSession.shared
        session.dataTask(with: url) { data, response, error in
            guard let data = data, error == nil else { return }

            do {
                let json = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as! [String:Any]
                let account = json["account"]
                if let accountDict = account as? [String:Any],
                   let seq = accountDict["sequence"] as? String,
                   let accountNum = accountDict["account_number"] as? String
                {
                    let input = CosmosSigningInput.with {
                        $0.signingMode = .protobuf;
                        $0.accountNumber = UInt64(accountNum) ?? 0
                        $0.chainID = "odin-mainnet-freya"
                        $0.memo = ""
                        $0.sequence = UInt64(seq) ?? 0
                        $0.messages = [message]
                        $0.fee = fee
                        $0.privateKey = privateKey.data
                    }
                    let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .odin)
                    
                    let simulateUrl = URL(string: "https://node.odin-freya-website.odinprotocol.io/mainnet/a/api/cosmos/tx/v1beta1/simulate")!
                    var request = URLRequest(url: simulateUrl)
                    request.httpMethod = "POST"
                    let cleanOutput = output.serialized.replacingOccurrences(of: "\\", with: "")
                    request.httpBody = Data(cleanOutput.utf8)
                    let simulateSession = URLSession.shared
                    simulateSession.dataTask(with: request) { data, response, error in
                        guard let data = data, error == nil else { return }

                        do {
                            let json = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as! [String:Any]
                            let gasInfo = json["gas_info"]
                            if let gasInfoDict = gasInfo as? [String:Any],
                               let gasUsed = gasInfoDict["gas_used"] as? String
                            {
                                let estimatedFee = CosmosFee.with {
                                    $0.gas = UInt64((Double(gasUsed) ?? 80000) * 1.2)
                                    $0.amounts = [CosmosAmount.with {
                                        $0.amount = "3200"
                                        $0.denom = "loki"
                                    }]
                                }
                                
                                let estimatedInput = CosmosSigningInput.with {
                                    $0.signingMode = .protobuf;
                                    $0.accountNumber = UInt64(accountNum) ?? 0
                                    $0.chainID = "odin-mainnet-freya"
                                    $0.memo = ""
                                    $0.sequence = UInt64(seq) ?? 0
                                    $0.messages = [message]
                                    $0.fee = estimatedFee
                                    $0.privateKey = privateKey.data
                                }
                                let estimatedOutput: CosmosSigningOutput = AnySigner.sign(input: estimatedInput, coin: .odin)
                                
                                let sendTxUrl = URL(string: "https://node.odin-freya-website.odinprotocol.io/mainnet/a/api/cosmos/tx/v1beta1/txs")!
                                var sendTxRequest = URLRequest(url: sendTxUrl)
                                sendTxRequest.httpMethod = "POST"
                                let sendTxCleanOutput = estimatedOutput.serialized.replacingOccurrences(of: "\\", with: "")
                                sendTxRequest.httpBody = Data(sendTxCleanOutput.utf8)
                                let sendTxSession = URLSession.shared
                                sendTxSession.dataTask(with: sendTxRequest) { data, response, error in
                                    guard let data = data, error == nil else { return }

                                    do {
                                        let json = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as! [String:Any]
                                        let txResponse = json["tx_response"]
                                        if let txResponseDict = txResponse as? [String:Any],
                                           let txHash = txResponseDict["txhash"] as? String
                                        {
                                            print(txHash)
                                        }
                                    } catch let error as NSError {
                                        print(error)
                                    }
                                    expectation.fulfill()
                                }.resume()
                            }
                        } catch let error as NSError {
                            print(error)
                        }
                    }.resume()
                }
            } catch let error as NSError {
                print(error)
            }
           
        }.resume()
        wait(for: [expectation], timeout: 300.0)
    }
}


