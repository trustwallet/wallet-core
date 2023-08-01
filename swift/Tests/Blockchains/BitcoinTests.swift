// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class BitcoinTransactionSignerTests: XCTestCase {
    override func setUp() {
        continueAfterFailure = false
    }
    
    func testSignBrc20Transfer() throws {
        // Successfully broadcasted: https://www.blockchain.com/explorer/transactions/btc/3e3576eb02667fac284a5ecfcb25768969680cc4c597784602d0a33ba7c654b7
        let privateKeyData = Data(hexString: "e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129")!
        let fullAmount = 26400 as Int64;
        let minerFee = 3000 as Int64;
        let brcInscribeAmount = 7000 as Int64;
        let dustSatoshis = 546 as Int64
        let forFeeAmount = fullAmount - brcInscribeAmount - minerFee;
        let txIdInscription = Data.reverse(hexString: "7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca");
        let txIDForFees = Data.reverse(hexString: "797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1");
        
        let privateKey = PrivateKey(data: privateKeyData)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        let pubKeyHash = publicKey.bitcoinKeyHash
        let bobPubkeyHash = PublicKey(data: Data(hexString: "02f453bb46e7afc8796a9629e89e07b5cb0867e9ca340b571e7bcc63fc20c43f2e")!, type: .secp256k1)!.bitcoinKeyHash
        let p2wpkh = BitcoinScript.buildPayToWitnessPubkeyHash(hash: pubKeyHash)
        let outputP2wpkh = BitcoinScript.buildPayToWitnessPubkeyHash(hash: bobPubkeyHash)
        
        var input = BitcoinSigningInput.with {
            $0.isItBrcOperation = true
            $0.privateKey = [privateKeyData]
        }
        let utxo0 = BitcoinUnspentTransaction.with {
            $0.script = p2wpkh.data
            $0.amount = dustSatoshis
            $0.variant = .p2Wpkh
            $0.outPoint.hash = txIdInscription
            $0.outPoint.index = 0
        }
        let utxo1 = BitcoinUnspentTransaction.with {
            $0.script = p2wpkh.data
            $0.amount = forFeeAmount
            $0.variant = .p2Wpkh
            $0.outPoint.hash = txIDForFees
            $0.outPoint.index = 1
        }
        input.utxo.append(utxo0)
        input.utxo.append(utxo1)
        
        let utxos = [
            BitcoinUnspentTransaction.with {
                $0.script = outputP2wpkh.data
                $0.amount = dustSatoshis
                $0.variant = .p2Wpkh
            },
            BitcoinUnspentTransaction.with {
                $0.script = p2wpkh.data
                $0.amount = forFeeAmount - minerFee
                $0.variant = .p2Wpkh
            }
        ]

        let plan = BitcoinTransactionPlan.with {
            $0.utxos = utxos
        }
        input.plan = plan
        
        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .bitcoin)
        let transactionId = output.transactionID
        XCTAssertEqual(transactionId, "3e3576eb02667fac284a5ecfcb25768969680cc4c597784602d0a33ba7c654b7")
        let encoded = output.encoded
        XCTAssertEqual(encoded.hexString, "02000000000102ca3edda74a46877efa5364ab85947e148508713910ada23e147ea28926dc46700000000000ffffffffb11f1782607a1fe5f033ccf9dc17404db020a0dedff94183596ee67ad4177d790100000000ffffffff022202000000000000160014e891850afc55b64aa8247b2076f8894ebdf889015834000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d024830450221008798393eb0b7390217591a8c33abe18dd2f7ea7009766e0d833edeaec63f2ec302200cf876ff52e68dbaf108a3f6da250713a9b04949a8f1dcd1fb867b24052236950121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb0248304502210096bbb9d1f0596d69875646689e46f29485e8ceccacde9d0025db87fd96d3066902206d6de2dd69d965d28df3441b94c76e812384ab9297e69afe3480ee4031e1b2060121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000");
    }
    
    func testSignBrc20Commit() throws {
        // Successfully broadcasted: https://www.blockchain.com/explorer/transactions/btc/797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1
        let privateKeyData = Data(hexString: "e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129")!
        let fullAmount = 26400 as Int64;
        let minerFee = 3000 as Int64;
        let brcInscribeAmount = 7000 as Int64;
        let forFeeAmount = fullAmount - brcInscribeAmount - minerFee;
        let txId = Data(hexString: "089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e")!;
        
        let privateKey = PrivateKey(data: privateKeyData)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        let pubKeyHash = publicKey.bitcoinKeyHash
        let p2wpkh = BitcoinScript.buildPayToWitnessPubkeyHash(hash: pubKeyHash)
        let outputInscribe = BitcoinScript.buildBRC20InscribeTransfer(ticker: "oadf", amount: "20", pubkey: publicKey.data)
        let outputProto = try BitcoinTransactionOutput(serializedData: outputInscribe)
        
        var input = BitcoinSigningInput.with {
            $0.isItBrcOperation = true
            $0.privateKey = [privateKeyData]
        }
        let utxo0 = BitcoinUnspentTransaction.with {
            $0.script = p2wpkh.data
            $0.amount = fullAmount
            $0.variant = .p2Wpkh
            $0.outPoint.hash = txId
            $0.outPoint.index = 1
        }
        input.utxo.append(utxo0)
        
        let utxos = [
            BitcoinUnspentTransaction.with {
                $0.script = outputProto.script
                $0.amount = brcInscribeAmount
                $0.variant = .brc20Transfer
            },
            BitcoinUnspentTransaction.with {
                $0.script = p2wpkh.data
                $0.amount = forFeeAmount
                $0.variant = .p2Wpkh
            }
        ]

        let plan = BitcoinTransactionPlan.with {
            $0.utxos = utxos
        }
        input.plan = plan
        
        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .bitcoin)
        let transactionId = output.transactionID
        XCTAssertEqual(transactionId, "797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1")
        let encoded = output.encoded
        XCTAssertEqual(encoded.hexString, "02000000000101089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e0100000000ffffffff02581b000000000000225120e8b706a97732e705e22ae7710703e7f589ed13c636324461afa443016134cc051040000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d02483045022100a44aa28446a9a886b378a4a65e32ad9a3108870bd725dc6105160bed4f317097022069e9de36422e4ce2e42b39884aa5f626f8f94194d1013007d5a1ea9220a06dce0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000");
    }
    
    func testSignBrc20Reveal() throws {
        // Successfully broadcasted: https://www.blockchain.com/explorer/transactions/btc/7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca
        let privateKeyData = Data(hexString: "e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129")!
        let dustSatoshis = 546 as Int64;
        let brcInscribeAmount = 7000 as Int64;
        let txId = Data(hexString: "b11f1782607a1fe5f033ccf9dc17404db020a0dedff94183596ee67ad4177d79")!;
        
        let privateKey = PrivateKey(data: privateKeyData)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        let pubKeyHash = publicKey.bitcoinKeyHash
        let p2wpkh = BitcoinScript.buildPayToWitnessPubkeyHash(hash: pubKeyHash)
        let outputInscribe = BitcoinScript.buildBRC20InscribeTransfer(ticker: "oadf", amount: "20", pubkey: publicKey.data)
        let outputProto = try BitcoinTransactionOutput(serializedData: outputInscribe)
        
        var input = BitcoinSigningInput.with {
            $0.isItBrcOperation = true
            $0.privateKey = [privateKeyData]
        }
        let utxo0 = BitcoinUnspentTransaction.with {
            $0.script = outputProto.script
            $0.amount = brcInscribeAmount
            $0.variant = .brc20Transfer
            $0.spendingScript = outputProto.spendingScript
            $0.outPoint.hash = txId
            $0.outPoint.index = 0
        }
        input.utxo.append(utxo0)
        
        let utxos = [
            BitcoinUnspentTransaction.with {
                $0.script = p2wpkh.data
                $0.amount = dustSatoshis
                $0.variant = .p2Wpkh
            }
        ]

        let plan = BitcoinTransactionPlan.with {
            $0.utxos = utxos
        }
        input.plan = plan
        
        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .bitcoin)
        let transactionId = output.transactionID
        XCTAssertEqual(transactionId, "7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca")
        let encoded = output.encoded
        XCTAssertTrue(encoded.hexString.hasPrefix("02000000000101b11f1782607a1fe5f033ccf9dc17404db020a0dedff94183596ee67ad4177d790000000000ffffffff012202000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d0340"));
        
        XCTAssertTrue(encoded.hexString.hasSuffix("5b0063036f7264010118746578742f706c61696e3b636861727365743d7574662d3800377b2270223a226272632d3230222c226f70223a227472616e73666572222c227469636b223a226f616466222c22616d74223a223230227d6821c00f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000"));
    }

    func testSignNftInscriptionCommit() throws {
        // Successfully broadcasted: https://www.blockchain.com/explorer/transactions/btc/f1e708e5c5847339e16accf8716c14b33717c14d6fe68f9db36627cecbde7117
        let privateKeyData = Data(hexString: "e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129")!
        let fullAmount = 32400 as Int64;
        let minerFee = 1300 as Int64;
        let inscribeAmount = fullAmount - minerFee;
        let txId = Data(hexString: "992faa0d60f29d77cdae687c300d288a3b075b3c7e1e3b42ad537222c3909557")!;
        let payload = Data(hexString: nftInscriptionImageData)!;
        
        let privateKey = PrivateKey(data: privateKeyData)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        let pubKeyHash = publicKey.bitcoinKeyHash
        let p2wpkh = BitcoinScript.buildPayToWitnessPubkeyHash(hash: pubKeyHash)
        let outputInscribe = BitcoinScript.buildOrdinalNftInscription(mimeType: "image/png", payload: payload, pubkey: publicKey.data)
        let outputProto = try BitcoinTransactionOutput(serializedData: outputInscribe)
        
        var input = BitcoinSigningInput.with {
            $0.isItBrcOperation = true
            $0.privateKey = [privateKeyData]
        }
        let utxo0 = BitcoinUnspentTransaction.with {
            $0.script = p2wpkh.data
            $0.amount = fullAmount
            $0.variant = .p2Wpkh
            $0.outPoint.hash = txId
            $0.outPoint.index = 0
        }
        input.utxo.append(utxo0)
        
        let utxos = [
            BitcoinUnspentTransaction.with {
                $0.script = outputProto.script
                $0.amount = inscribeAmount
                $0.variant = .nftinscription
            }
        ]

        let plan = BitcoinTransactionPlan.with {
            $0.utxos = utxos
        }
        input.plan = plan

        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .bitcoin)
        let transactionId = output.transactionID
        XCTAssertEqual(transactionId, "f1e708e5c5847339e16accf8716c14b33717c14d6fe68f9db36627cecbde7117")
        let encoded = output.encoded
        XCTAssertEqual(encoded.hexString, "02000000000101992faa0d60f29d77cdae687c300d288a3b075b3c7e1e3b42ad537222c39095570000000000ffffffff017c790000000000002251202ac69a7e9dba801e9fcba826055917b84ca6fba4d51a29e47d478de603eedab602473044022054212984443ed4c66fc103d825bfd2da7baf2ab65d286e3c629b36b98cd7debd022050214cfe5d3b12a17aaaf1a196bfeb2f0ad15ffb320c4717eb7614162453e4fe0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000");
    }
    
    func testSignNftInscriptionReveal() throws {
        // Successfully broadcasted: https://www.blockchain.com/explorer/transactions/btc/173f8350b722243d44cc8db5584de76b432eb6d0888d9e66e662db51584f44ac
        let privateKeyData = Data(hexString: "e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129")!
        let inscribeAmount = 31100 as Int64;
        let dustSatoshis = 546 as Int64;
        let txId = Data(hexString: "1771decbce2766b39d8fe66f4dc11737b3146c71f8cc6ae1397384c5e508e7f1")!;
        let payload = Data(hexString: nftInscriptionImageData)!;

        let privateKey = PrivateKey(data: privateKeyData)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        let pubKeyHash = publicKey.bitcoinKeyHash
        let inputInscribe = BitcoinScript.buildOrdinalNftInscription(mimeType: "image/png", payload: payload, pubkey: publicKey.data)
        let p2wpkh = BitcoinScript.buildPayToWitnessPubkeyHash(hash: pubKeyHash)
        let inputProto = try BitcoinTransactionOutput(serializedData: inputInscribe)
        
        var input = BitcoinSigningInput.with {
            $0.isItBrcOperation = true
            $0.privateKey = [privateKeyData]
        }
        let utxo0 = BitcoinUnspentTransaction.with {
            $0.script = inputProto.script
            $0.amount = inscribeAmount
            $0.variant = .nftinscription
            $0.spendingScript = inputProto.spendingScript
            $0.outPoint.hash = txId
            $0.outPoint.index = 0
        }
        input.utxo.append(utxo0)
        
        let utxos = [
            BitcoinUnspentTransaction.with {
                $0.script = p2wpkh.data
                $0.amount = dustSatoshis
                $0.variant = .p2Wpkh
            }
        ]

        let plan = BitcoinTransactionPlan.with {
            $0.utxos = utxos
        }
        input.plan = plan

        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .bitcoin)
        let transactionId = output.transactionID
        XCTAssertEqual(transactionId, "173f8350b722243d44cc8db5584de76b432eb6d0888d9e66e662db51584f44ac")
        let encoded = output.encoded

        let expectedHex = nftInscriptionRawHex;

        // Compare ..164
        let start1 = encoded.hexString.index(encoded.hexString.startIndex, offsetBy: 0)
        let end1 = encoded.hexString.index(encoded.hexString.startIndex, offsetBy: 164)
        let subString1 = encoded.hexString[start1..<end1]

        let start2 = expectedHex.index(expectedHex.startIndex, offsetBy: 0)
        let end2 = expectedHex.index(expectedHex.startIndex, offsetBy: 164)
        let subString2 = expectedHex[start2..<end2]

        XCTAssertEqual(subString1, subString2)

        // Compare 292.., skipping the Schnorr signature from 164..291
        let start3 = encoded.hexString.index(encoded.hexString.startIndex, offsetBy: 292)
        let subString3 = encoded.hexString[start3...]

        let start4 = expectedHex.index(expectedHex.startIndex, offsetBy: 292)
        let subString4 = expectedHex[start4...]

        XCTAssertEqual(subString3, subString4)
    }

    func testSignP2WSH() throws {
        // set up input
        var input = BitcoinSigningInput.with {
            $0.hashType = BitcoinScript.hashTypeForCoin(coinType: .bitcoin)
            $0.amount = 1000
            $0.byteFee = 1
            $0.toAddress = "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx"
            $0.changeAddress = "1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU"
        }

        input.scripts["593128f9f90e38b706c18623151e37d2da05c229"] = Data(hexString: "2103596d3451025c19dbbdeb932d6bf8bfb4ad499b95b6f88db8899efac102e5fc71ac")!

        let p2sh = BitcoinScript.buildPayToWitnessScriptHash(scriptHash: Data(hexString: "ff25429251b5a84f452230a3c75fd886b7fc5a7865ce4a7bb7a9d7c5be6da3db")!)
        let utxo0 = BitcoinUnspentTransaction.with {
            $0.script = p2sh.data
            $0.amount = 1226
            $0.outPoint.hash = Data(hexString: "0001000000000000000000000000000000000000000000000000000000000000")!
            $0.outPoint.index = 0
            $0.outPoint.sequence = UInt32.max
        }
        input.utxo.append(utxo0)

        // Plan
        let plan: BitcoinTransactionPlan = AnySigner.plan(input: input, coin: .bitcoin)

        XCTAssertEqual(plan.amount, 1000)
        XCTAssertEqual(plan.fee, 147)
        XCTAssertEqual(plan.change, 79)

        // Extend input with private key
        input.privateKey.append(Data(hexString: "ed00a0841cd53aedf89b0c616742d1d2a930f8ae2b0fb514765a17bb62c7521a")!)
        input.privateKey.append(Data(hexString: "619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9")!)

        // Sign
        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .bitcoin)
        XCTAssertEqual(output.error, TW_Common_Proto_SigningError.ok)

        let signedTx = output.transaction
        XCTAssertEqual(signedTx.version, 1)

        let txId = output.transactionID
        XCTAssertEqual(txId, "dc60991ff61a6061f55854ce6fb3203b7c8291ed7b2ce799040114c608391583")

        XCTAssertEqual(signedTx.inputs.count, 1)  // Only one UTXO available
        XCTAssertEqual(signedTx.inputs[0].script.hexString, "")

        XCTAssertEqual(signedTx.outputs.count, 2) // Exact amount
        XCTAssertEqual(signedTx.outputs[0].value, 1000)
        XCTAssertEqual(signedTx.outputs[1].value, 79)

        let encoded = output.encoded
        let witnessHash = Data(Hash.sha256SHA256(data: encoded).reversed())
        XCTAssertEqual(witnessHash.hexString, "ec57de0d46eb45e8019b82e388e458e72fb834dba971e3a45ff8fa7bb7bdb799")
        XCTAssertEqual(encoded.hexString,
            "01000000" + // version
            "0001" + // marker & flag
            "01" + // inputs
                "0001000000000000000000000000000000000000000000000000000000000000" + "00000000" + "00" + "ffffffff" +
            "02" + // outputs
                "e803000000000000" + "19" + "76a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ac" +
                "4f00000000000000" + "19" + "76a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac" +
            // witness
                "02" +
                    "48" + "30450221009eefc1befe96158f82b74e6804f1f713768c6172636ca11fcc975c316ea86f75022057914c48bc24f717498b851a47a2926f96242e3943ebdf08d5a97a499efc8b9001" +
                    "23" + "2103596d3451025c19dbbdeb932d6bf8bfb4ad499b95b6f88db8899efac102e5fc71ac" +
            "00000000" // nLockTime
        )
    }

    func testSignP2SH_P2WPKH() {
        let address = "3LGoLac9mtCwDy2q8PYyvwL8kMyrCWCYQW"
        let lockScript = BitcoinScript.lockScriptForAddress(address: address, coin: .bitcoin)
        let key = PrivateKey(data: Data(hexString: "e240ef3419d038577e48426c8c37c3c13bec1a0ed3f5270b82e7377bc48699dd")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let utxos = [
            BitcoinUnspentTransaction.with {
                $0.outPoint.hash = Data.reverse(hexString: "8b5f4861c6d4a4ea361aa4066d720067f73854d9a1b1d01e2b0e3c9e150bc5a3")
                $0.outPoint.index = 0
                $0.outPoint.sequence = UINT32_MAX
                $0.script = lockScript.data
                $0.amount = 54700
            }
        ]

        let plan = BitcoinTransactionPlan.with {
            $0.amount = 43980
            $0.fee = 10720
            $0.change = 0
            $0.utxos = utxos
        }

        // redeem p2wpkh nested in p2sh
        let scriptHash = lockScript.matchPayToScriptHash()!
        let input = BitcoinSigningInput.with {
            $0.amount = 43980
            $0.toAddress = "3NqULUrjZ7NL36YtBGsSVzqr5q1x9CJWwu"
            $0.hashType = BitcoinScript.hashTypeForCoin(coinType: .bitcoin)
            $0.coinType = CoinType.bitcoin.rawValue
            $0.scripts = [
                scriptHash.hexString: BitcoinScript.buildPayToWitnessPubkeyHash(hash: pubkey.bitcoinKeyHash).data
            ]
            $0.privateKey = [key.data]
            $0.plan = plan
        }

        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .bitcoin)

        // https://blockchair.com/bitcoin/transaction/da2a9ce5d71ff7490bc9025e2888ca109b68ec0bd0e7d26195e1783305c00117
        XCTAssertEqual(output.encoded.hexString, "01000000000101a3c50b159e3c0e2b1ed0b1a1d95438f76700726d06a41a36eaa4d4c661485f8b00000000171600140a3cca78017f46ac23e463148adb7231aef81956ffffffff01ccab00000000000017a914e7f40472c54fc93078c5129568cf95c27be3b2c287024830450221008dc29a5430facd4078ad93e72517d87b298d7a73b55d2828acab040ccf713ed5022063a13e348655fa7cdcfff084380611629babf165607b529bcc35bf6ddfab1f8101210370386469db8302c3092955724f56bcca9a36f31df82655aa79be46b08744cd1200000000")
    }

    func testHashTypeForCoin() {
        XCTAssertEqual(BitcoinScript.hashTypeForCoin(coinType: .bitcoin), TWBitcoinSigHashTypeAll.rawValue)
        XCTAssertEqual(BitcoinScript.hashTypeForCoin(coinType: .bitcoinCash), 0x41)
        XCTAssertEqual(BitcoinScript.hashTypeForCoin(coinType: .bitcoinGold), 0x4f41)
    }

    func testSignExtendedPubkeyUTXO() {
        // compressed WIF, real key is 5KCr
        let wif = "L4BeKzm3AHDUMkxLRVKTSVxkp6Hz9FcMQPh18YCKU1uioXfovzwP"
        let decoded = Base58.decode(string: wif)!
        let key = PrivateKey(data: decoded[1 ..< 33])!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)

        // shortcut methods only support compressed public key
        let address = BitcoinAddress(data: [0x0] + Hash.sha256RIPEMD(data: pubkey.data))!
        let script = BitcoinScript.lockScriptForAddress(address: address.description, coin: .bitcoin)

        // utxos from: https://blockchair.com/bitcoin/address/1KRhiKNai3ke3hZgSPZ5TpJoSJvs1aZfWo
        let utxos: [BitcoinUnspentTransaction] = [
            .with {
                $0.outPoint.hash = Data.reverse(hexString: "6ae3f1d245521b0ea7627231d27d613d58c237d6bf97a1471341a3532e31906c")
                $0.outPoint.index = 0
                $0.outPoint.sequence = UINT32_MAX
                $0.amount = 16874
                $0.script = script.data
            },
            .with {
                $0.outPoint.hash = Data.reverse(hexString: "fd1ea8178228e825d4106df0acb61a4fb14a8f04f30cd7c1f39c665c9427bf13")
                $0.outPoint.index = 0
                $0.outPoint.sequence = UINT32_MAX
                $0.amount = 10098
                $0.script = script.data
            }
        ]

        let input = BitcoinSigningInput.with {
            $0.utxo = utxos
            $0.privateKey = [key.data]
            $0.hashType = BitcoinScript.hashTypeForCoin(coinType: .bitcoin)
            $0.useMaxAmount = true
            $0.byteFee = 10
            $0.toAddress = "1FeyttPotRsSd4equWr678dbEvXaNSqmDT"
            $0.coinType = CoinType.bitcoin.rawValue
            $0.amount = utxos.map { $0.amount } .reduce(0, +)
        }

        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .bitcoin)

        // https://blockchair.com/bitcoin/transaction/1d73706d33ec249beae6804c2e636ab9d7adbc2e9548757f6fcf8118771cb311
        XCTAssertEqual(output.error, .ok)
        XCTAssertEqual(output.encoded.hexString, "01000000026c90312e53a3411347a197bfd637c2583d617dd2317262a70e1b5245d2f1e36a000000008a47304402201a631068ea5ddea19467ef7c932a0f3b04f366ca2beaf70e18958e47456124980220614816c449e39cf6acc6625e1cf3100db1db7c0b755bdbb6804d4fa3c4b735d10141041b3937fac1f14074447cde9d3a324ed292d2865ed0d7a7da26cb43558ce4db4ef33c47e820e53031ae16bb0c39205def059a5ca8e1d617650eabc72c5206a81dffffffff13bf27945c669cf3c1d70cf3048f4ab14f1ab6acf06d10d425e8288217a81efd000000008a473044022051d381d8f48a9a4866ca4109f12647922514604a4733e8da8aac046e19275f700220797c3ebf20df7d2a9fed283f9d0ad14cbd656cafb5ec70a2b1c85646ea7485190141041b3937fac1f14074447cde9d3a324ed292d2865ed0d7a7da26cb43558ce4db4ef33c47e820e53031ae16bb0c39205def059a5ca8e1d617650eabc72c5206a81dffffffff0194590000000000001976a914a0c0a50f986924e65ae9bd18eafae448f83117ed88ac00000000")
    }

    func testBitcoinMessageSigner() {
        let verifyResult = BitcoinMessageSigner.verifyMessage(
            address: "1B8Qea79tsxmn4dTiKKRVvsJpHwL2fMQnr",
            message: "test signature",
            signature: "H+3L5IbSVcejp4S2VwLXCxLEMQAWDvKbE8lQyq0ocdvyM1aoEudkzN/S/qLI3vnNOFY6V13BXWSFrPr3OjGa5Dk="
        )
        XCTAssertTrue(verifyResult)
    }

    func testBitcoinP2pkhCalculateFee() throws {
        let satVb = "10"
        let tx = Data(hexString: "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000")!
        let fee = BitcoinFee.calculateFee(data: tx, satVb: satVb)!
        let feeInt = UInt64(fee)!
        XCTAssertEqual(feeInt, 191 * 10)
    }

    func testBitcoinP2wpkhCalculateFee() throws {
        let satVb = "12"
        let tx = Data(hexString: "020000000111b9f62923af73e297abb69f749e7a1aa2735fbdfd32ac5f6aa89e5c96841c18000000006b483045022100df9ed0b662b759e68b89a42e7144cddf787782a7129d4df05642dd825930e6e6022051a08f577f11cc7390684bbad2951a6374072253ffcf2468d14035ed0d8cd6490121028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28fffffffff01c0aff629010000001600140d0e1cec6c2babe8badde5e9b3dea667da90036d00000000")!
        let fee = BitcoinFee.calculateFee(data: tx, satVb: satVb)!
        let feeInt = UInt64(fee)!
        XCTAssertEqual(feeInt, 189 * 12)
    }

    // Metadata can be observed live on:
    // https://www.blockchain.com/explorer/transactions/btc/797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1
    //
    // Fee/VB   19.608 sat/vByte
    // Size     235 Bytes
    // Weight   610
    func testBitcoinBrc20TransferCommitCalculateFee() throws {
        let satVb = "19"
        let tx = Data(hexString: "02000000000101089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e0100000000ffffffff02581b000000000000225120e8b706a97732e705e22ae7710703e7f589ed13c636324461afa443016134cc051040000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d02483045022100a44aa28446a9a886b378a4a65e32ad9a3108870bd725dc6105160bed4f317097022069e9de36422e4ce2e42b39884aa5f626f8f94194d1013007d5a1ea9220a06dce0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000")!
        let fee = BitcoinFee.calculateFee(data: tx, satVb: satVb)!
        let feeInt = UInt64(fee);
        XCTAssertEqual(feeInt, 153 * 19) // 153 = ceil(610/4)
    }
}

let nftInscriptionImageData = """
89504e470d0a1a0a0000000d4948445200000360000002be0803000000f3\
0f8d7d000000d8504c54450000003070bf3070af3173bd3078b73870b730\
70b73575ba3075ba3075b53070ba3078bb3474bb3474b73074bb3074b733\
76b93076b93373bc3373b93073b93376bc3275ba3075ba3572ba3272ba33\
75bc3276b83474bb3274bb3274b83276bd3276bb3474bd3474bb3276b934\
74bb3474b93274bb3274b93476bb3276bb3375ba3275ba3373bc3273ba32\
77bc3375bc3375ba3373bc3374ba3174b93376bb3375bc3375bb3375b933\
75bb3374bb3376bb3475bb3375bb3375ba3374bb3276bc3276ba3375bc32\
75bc3375bb3275bb3374bc3374bb3375bb7edf10e10000004774524e5300\
10101f20202030303030404040404050505050505f606060606f70707070\
7f7f7f7f80808080808f8f909090909f9f9f9fa0a0afafafb0bfbfcfcfcf\
cfcfdfdfdfdfefefefef6a89059600001c294944415478daeddd6b7bd3d6\
ba2ee0d8350eb00c81ec5533cbac69d6da99383334dbe510904b66129383\
feff3fda17506888751892255b52eefb634b1259d2a3f1ead5d0f0d61600\
000000000000000000000000000000000000000000000000000000000000\
000000000000000000000000000000000000000000000000000000000000\
000000000000000000000000000000000000000000000000000000000000\
000000000000000000000000000000000084eaef3c9b4e274f87f604546e\
27ba8abf3a19db1b50a96114dfb01031a8d0cf57f18f0e7a760a54e4b778\
c9c2ad1854a27f1c27586cdb3350c1edd7224eb667dfc0aa9e5fc569666e\
c460b5f2f0df71063762b08afb8b389b3211ca978771aec82006a5fcf870\
39cd8587ce5066f8ba8ac3cc0c6250d0cec738d8c53fed2f2820bb79a89d\
08ab185fc545a91321b03a8ce21216ea44c8372c152f2fb1400d375fb722\
b6630f427abcf6aee2d544220675c54bc420adb5318b2be25e0c6ec72b8a\
2bb4186bdac3dfb5e1495cb5d903fb15bed48657711d4ec67d3b973b6eb8\
574fbafe1ac6343cb8cba5e1248a6bb69031eeead8557bba648c3b3a74ed\
1c9cc56b74f54e5b913b13aebde82a5ebf9399af8da0ebe17a7ab09170fd\
1db2b1ee3dddbce57a3a79b77a5978383e5fb95c8c0e9eba29a33ba3d683\
6793d94925e3d6f9686b6bf0a69ab12c3a98ec2819696baa86c39d67cfa6\
b377671556846fbe3e385e7d10fbdb59f4ee60fa6c6767e899340d8dd2b3\
835974d3c9d9592db759974fbefdc98a06b1a5b09d9dfdf041a2d94429c9\
66ed44ebea4f1cde1c63aa1cc4b29fa1ed2923d9dce8b5b6787dbefbbaa9\
a641ccbb2f3448eec2f1955587fbcbb748bbeb1ac4e203479a8de46b5d4f\
b4e683c4bfbf7fbda6bf3f73ac59bfe16233d5e106ea446318eb77bc9eea\
f0d7ac6d189cac27618f1c6ed66cbca99baf5b9bb1965bb185e3cd9a9dae\
bb35bfd18819c258af876be82d0c4207d3fa231639e2acd5a431f15a4bc4\
2e7cdd3a6bf57ec3f75eeb8e98b75ce8cc2d58f1787d89d8bcce80fde290\
b34ef5bdf13f2f3d37a9cee762024617027679385a65ab06b5558a0246fb\
03369facfe5ad6e88d802160950f5e3787b1530143c07e48d7eb51955b37\
383c1730da2caa345d35ac22bf5d6dc6048c7606ecf27054d77a18db93ea\
6ac5270e39ed0bd87c7f54ef660ec6c7972623721703f6e970773d4b398d\
267301e34e05ecf470cddfdd359acc2f058c3b10b04faf27a3cd2c42b8bd\
7b583e6502c65a9598ec7b393fdc54b66ea66c7a7c2a6034dddb2283d6e9\
ebe978bb496be76eef4e0ee7a702467b03f6e9d3fcf8f574321edd6bee87\
e88f46e3e9e1f1fcf4536ee9e875159a12b07fddbb77af7d1fa87fefde76\
c6f4949f1c721a12b0f64e7a1030044cc0b8038ed2cfc51702062b9aa69f\
8bd3d67ea86b0143c0ea93d14574c411300143c0040c5609d82b01831565\
7cf7c31f020602b66490b174b6238e80d51730df5f848009187721601f04\
0c56f43863259bb67ea68cef3cfbd31147c07c2604cc6782554ec63f050c\
3404040c015ba72e764611b03604ec0f479ca604ec42c060551d9c183b11\
3004ac3e9d7cc70d011330b8ad830bc40818cdd1c125ce8e040c01ab4f27\
175345c0040c6e3bedde57fdbc17301a23ea5ec03af891e862c09e0818a8\
a7040c1d81353af3059708d84602e6cb5558b30e3e95f5ed45344707e715\
59391b011330ee84eebd3cd513309aa37bafff5ad8977604ec8380c18a76\
3bb7c65917977aa4b51edfa580591691757bd8b9824ac070c7e2ae923b1f\
b08bce05ccaa6dac5bf79e1a4d058c06b94b01b3e60d6bd7b9a9b11695a2\
493af77247c60b382f1c6e1a14b076be9e68cd1b9aa4732fd877709511ba\
19b05f5c31a0be5b967606ec4cc0d013d848c0ac18c0da75aeab9df160af\
e770b36e19cf655f752d608e366bd7b5a97b0301a325016be5e4f3875e68\
a649baf64ab3179a69cb09b9e8d8e7f1be258dba676965c0bc6f89a6408d\
265e07a3513af6dcc8eb60344bc75e087b2b60344ac7de57f1b60acdd2b1\
af41f7b60aad39237f71bd80fa6e5ada389dde17c8d22c1d9b4e1f7b5b85\
46e9d63a82be1d8c86e9d674fa879d5ba998ee06ac8593f71e9b4c8f53b2\
3ebbe6fad22cddfafa8789b9beb4a72dd0bec988befa81a6e95463db5444\
9aa6538f66232bd3d330a75d9a2b652a224df3be4b177debfae2b6c50d25\
7748971a6f5645a471ba3495e3b1995234cd6e87a672ec5a1591a6e9d2fc\
d8899952b4e9bea56d53398e4ce4a071e2ee3c697e6f22078d73d69d87b3\
9135a568d359d9b627cdd79e33d3386fbbf31d7cb1256fd019a8cd43df1f\
4bf38c3bf3f0c873661a68b733a7a5c7603450771e841d593080e6e975a6\
35f0be3bed1a3ae4ac2b4f8f4ebdcf4cbbcecb76cd7f883d06a381de76a4\
4fffd063309a68da913efd63af5bd244e38ef4e927de06a36da5559b56b2\
38f2188c26caead3b7693e7da44b4f235d77a3bd7dad4b4f239d76a28dd8\
d3a5a799de76a23bf0d8a28834d3a4136dc489b9f434d36e27aefdef3511\
69a64127da88a7e6d2d350d75d6870c79a883454d4818bff634d449aeaa8\
03fd8189053968aa7107ba1c334d449a2a6b36625bdeb93cd544a4a9b226\
41bcf211604519ab0644edef71f87a66362c63b2d4453b3a0453af33d35c\
93d6f7b8233d0edad9e568c7c237b11e070d76ddf29bb0c7adbf42d06951\
cb6fc2a6e671d064472d3f4123f33868b2dd763f46ca7a0a6645299ca177\
fbfa40f79db57a3ae2cc6366da7b13d6fcd7a916195b3f7070d9bcac09f5\
8d6fd43f760b468b6fc21adf879bb905a3e94e5b5c232edc82d1e69bb086\
d7888f634fc168ba517bcfd299898834df756ba7f35d5b508ae6cb78272c\
fed8e40dcf6a807a178c569ca74d9e8f98350ff1dc71a521321bf50d6e73\
0c634d7ada206a679b63167b558536c85a37a0b96d8ecc014c85484b6ac4\
c6be76993980cd1c555a5223367408cb1cc0f4106992710b87b0990a91d6\
d488d7ad1bc2b207b0b1634a931c659eaec3b60d605e05a35932e72336f1\
59d8fdcc0df6c596b4a9cdd1c0373f162ddb5eeeb8cc4761f1a2697d8edf\
622d0ebad3e668da63a5ec0e871607cdf332fb9cfd6793b6b59f5d206a71\
d0c0212cfb9cbd68522731bb836816074df4266ecb6d58f60d98018c461a\
e59cb68de9d5ff1c1bc068a1a81d27eefd2b03185d1cc2e2bd56e4cb0046\
3befc21a91b0dc7c19c068aac175dcf4d161e72a3680d1562ff3cedef8dd\
667b893fe76ee0b9018cc6eae50e61f16293cfc37ecbddbcf8574791e69a\
e49fc18b8d4da4ed1fe76f9d5988345a947f0e6faad5717f11b06ddb0e21\
edee73c4717cb28932f179c086e970d0fe3ec7e73271edd3d5872143ab0e\
07dd2812e378b6de41ecf955d0568d1c3e3a5124c6f1628defaf840d5f71\
fc2f478fe69b849dcd6b6bd8f7f7023748079156f83df0848e0fd611b19d\
45e0d65cba01a3157a27a109abbfd9713f0add16eb04d0b1dbb02f11dba9\
b53afc77f086b801a33db6c3cfeb1afb89fdbdabf0cd78e3a8d11ee378f3\
117bb228b00dffe93b68b4c87e91842d6a983cb51315d9024f98e972c22a\
ef760c0bc54bbee87ac22a8d5891de867c713712164715dd8a15ea6dc817\
77266195743b0ac74bbeb833095b3d62e3452c5fdc1593c2095bada158ac\
75f8c55c7f9ef61a5d178f58e96ec7fde2f18a0f1d23da6c705efca42fb7\
6a47d1d6a1f951742261c725cefbe2b762c57b1b9fe7cf7bc192bbd8ea28\
1eb1e2bd8dcfd3a3b437e8c48d588932b1d01bcf257a1b6ebfe85099f8a6\
4c0042bb1dc35999dfae3ca44326d775d589a56ebe74e7e9da2056aa8a8b\
f7f296b2df59941abe2c904de706b1f3eaebc461b9d8ce75377027f6edfb\
5886155787e74f1c0b3a697c5eae4eacb23a8c0fdd7dd159a59e8925d689\
fde372d5e17d070175626e3ff179a9eaf0d2d26ca813f307b172cd8dcb7d\
d5212296ffc2f35ea951f058ef903b5227febe4ab363e763a99b2f333770\
2b96ff7511a5de4a89cfc58b3b16b179a941ac546fdec40ddc8ad5466f03\
11ab8f07cbdc5dfb7547ccb443743bea8b97de06225657bcb40ea1744351\
eb1036d6edd03a84fa2236132ff8a14edcd7db8016743bf43620d1fdf32a\
6ebeec47a8eb56ccbc0dc8aa137f77f3057546ecbd275fd0bc3a517508b5\
d589aa43088fd8b9ea106a54e8eb22ac67034507b1379e2cc3e69b1d9a1b\
50db2066f882fa0631c317d4368819bea0be41ccf005b50d62862fa844e2\
33b163c3175434889d9bba0135dab79e28ace94eecc4175542e5111b1f9f\
7efaf4e9f850730300000000000000000000000000000000000000000000\
000000000000000000000000000000000000000000000000000000000000\
00000000000000000068bff16c49effbfffc9fe5ff698f41016fe3253f7d\
ff9fd1f2ffb4c740c040c040c0046c15b3aa0ceedcae1b26ec8583b01f7d\
b2f483e3b01f7cbef483ff2b608d1657e5d19ddb75bdebd2bb61f9bcbd08\
fb9bcb7f7226600276778aab781a34f695dd7fbb35ec770113b0861a25ec\
8628e407c7093ff82aa8a05ffab173f7600276a76ac49f4a55888135620d\
15a28009585dfefbd992b5d488c3b23b30a1427cd2fa80f5968fc28e8075\
22602b9f3d256bc471e20e0ca81167657b234d0ed860f94ffc216002965a\
23f64a55884159a9a3421430016b6cc0126bc417e52ac4803d584b852860\
02d6dc808dcbd488e3943d987bf7564b85286002d6dc8095aa11a3943db8\
c8fb6b8b3a2a440113b0e6062c312c2fca5588b9bbf0f1f24f8c054cc03a\
1db01235e23875174e8b5688010d150113b036072ca946bce895aa107393\
b95c217ed8123001eb74c012e3f24bb90a316716484d15a2800958930356\
b8461c67ecc3e9062a4401abd728d5ff4938030ed3ff79ff6e06ac708d78\
9a11b0680315a2806dcaa068f17327039658233e2a5721660e4975558802\
d6ad800dd734d2f587c3dc3f55c9d933890b9d2093cc80bd2854210e56fb\
f8b5062cfcef0b5858c03e1fd21ffdb083fb3b93d9c9591cc7f3bf767fd6\
3fcefdd5c3f403fbec60767276f56d83cfa2e860b2d3af3160bdb8508d78\
9a19b0a84885384fd8530f6e7ffc77d3a70fd619b0efc7f9b3ab93e820ef\
cf0b5860c01e67eca6fe24baba795a14dca7d3c0226c78e3cfdc9a24f16e\
b25353c08ad588c39c5651af7c85387c363b4bfea557efc6c3b5042cf900\
2c664f05acce80ed1c5fddbaeed611b0fe5e9473f2465f4fb383e89b8493\
21baa5648df8aa5c85985123e654883b7b27d9bff8ddd3ba03d69fa41f80\
abd952c2bfefe2840d5fdc3a0a0702961eb061b45cd8541fb0fede55c8b3\
84689cf5a0372e35a2f58abc7a729ab781652ac47e5ebabe9eb5e33a0396\
7b00a25b254491a33017b0d4803dbf8aeb0f5860bcbe9c66412763a192b1\
408d38ccfd83bdc215e2f020f4c32f86b505ec79c036fc388a09581501eb\
1f27eeac8a03b6b388eb52b68ff8aa5c8598ba87532bc4e1acc8c7d9ab27\
603b1f8bff7901ab2060c38ff11a02f63c8e371ab00235e269ee1f8c8a55\
88e143f75727c31a02167e006e8ca102b67ac0fa8b780d01fb2dde70c0c2\
6bc4fc0a31a5c39f52210e3f16fe40cb65e2aa01eb17b9a95d6c0b587501\
3b8ed710b0c2f95a541eb049e8b4c2e57f781db68b8f922bc4de79bc7ac2\
560cd8b0607dfeb3805515b0ab780d012b9caf595479c0926ac4455085f8\
e751d88e48ab10c72506e545afca800d0bdffffe2c6015052c5e43c08685\
cfaf41f5010bad119737f6c528a8467c98da432c3184dd5e6760a5800d4b\
f4971e09587b02b6287e76d510b0c01a719250e75d8724f328f529739921\
ecd65254ab04ac4cbee28ba180b52560c5cfaf411d01eb85b503972bc4a4\
ecfc115221fef9ed7f9519c27e1c245709585426dff1494fc05a12b01203\
581d014bfc9d3fe55788e3a4c5812f422ac4ef53aa4665cef06945012bdb\
c03d10b076042cf96f5ebe9eec6edffb6c7bb43b3d3ebdf57375042ca946\
7c1152216e85d488e9156272043e7d9acf4f3f850e61e5039676037c7a38\
feb2ff479337e7a9b76102d682801d25fdfed1d2839addefc779be554fc0\
826ac4d3c4f3e6287f16484685786b08bb3c9eecdefbf67fb677d3ceef1f\
c25f3e6089f5c3e5e1f6cd6d1f9fa73c4e17b016042c6162c4bf927fc5e8\
cdf7dedbd1fc9b84b7fde7b784eeaca4d4f6022ac4901af1617640a2bf4f\
eda56b4bcaf9fd43f84b072c7102c77c10b409e3adbff77142486f1d05b3\
e9730336ff7665bd77afc28005e7ebf3dfdc3fbffd6d7515be0fff32bf46\
7c995ce7f5726bc4495685f83da1cb23f7d7b1f5f7c4e3d15b3d60891dc4\
5f93f6fc49e66342ef83ad1cb0cbfd7eeeaf2e13b0e54b7bf6f73dee3fa9\
2d6083fc1a719152f8bccdab114fb32ac4bf3e464abcbe7cea9c1ab16cc0\
923a1cff480ef971ea733c01ab2060fbfd805f5d2660cb7fb2d83a4b55ae\
e8925b233e4c3bc7f26ac48456c28f091cc5e7a3ac4d4b1ac3fe583d608b\
b0f1eb4bc24e32ae3e02b65ac0920e7e6d01fb636301cbad118fd2eabcbc\
1a31a142bcb5ccc5ef39cbcb649768250336ce9d2272f380677c46015b29\
60e783ad3506eccf8d052cb7465ca4b6c6a2ece754a7050be1840e4fe6e8\
5a326051d8a14e8de32b01ab22609783ad7506ecfb5cedb5072ce997dd7c\
dcf430fd2e649c35bee4578801aeb306c172011b66dd5685ec9e85805511\
b0f1568d014bdac27f6e2a602fb377d7517a2730bb46ccaf10f31d656d5a\
b9808d836629670d613f09d8ea017bbf5567c0121f562e0e1e6c24603935\
e222e3e169668db87a859878bff462c580bd2f7007967c15792160ab07ec\
51bd014b5d073075add1fa0296b4317f5fd41f668dede38c64565121263d\
aa9eae18b045ce1cfd25efd38eb880950fd89f5bf506ec28e3d1f655c692\
beb504ec65d605e628eb59712fe3141f075fb58a1dca57ab05ac97fdf03b\
c124ede410b0f2017b5173c0f2a7929f640d66d5066c94f5b8699139bd2e\
a3468c8a5588fd074f9f3d9bde7694b5bf4b052ce1684fb31da73ded13b0\
f2017b5073c002df863a3978daaf3f60893562488598d4c888d22bc4b47b\
9de1647612bec6d48a012bf5a267f25f11b0d201bbd8aa3b6093e0a3194d\
8675072ca3463cca2ea87aa9cfa9422bc49d8382ebb7ad18b06905017b20\
602b066c5e7bc00abd7b727bf1e6aa03965423be0aa91093b6e445810ab1\
3f39297c76af18b0a30a02f648c0560cd887fa033628f4cafcadaf20a8fa\
dbe5526bc48779cf07d36ac4a00af149998531560cd8db0a02f68b80ad18\
b069fd01dbda2e94b08b719d014bad118ff25a6e6935624085d83f2e7576\
0b98800505ace018f6c3f2e855072cb546ccab10536bc4fc0a7158725d7e\
0113b0b0806d0dde944d58e55f407c9d5c23e656886935627e8558365f4d\
08d813016b48c08e720aa571a141ec9ff505ec28b946ccad10d36ac4dc0a\
b174be9a10304d8ea604ec7dee9d48a1883daa2d604935e234a4424cda96\
5f022ac459bca18069d3b72d6059ef229f063c0c1abd09ce58545bc0926a\
c445488598d42089f22bc4151ef7ae18b0490501f3a0799d01cb7a553270\
16f1f6f8f8bcd010567dc0926ac407b390697bcbbbe4a2975b21a61488a7\
afa7d3f14d93ca03b6bbfcdfffefb8a02d015b63c02e0afde6d409af83db\
2b8d665dbdab0f58628d18522126d68851ce5e4a1cc0e6a37eb192bc54c0\
06e1134f4b9c6702b662c07a71d6410d6c1d64d8de9dbe9e5fa63f0dab2d\
604935e2554885985823e6558809ff2079f99bea0396f04123016b4cc092\
8abef40be0a270c0be8d6687f3ccdbeb1a0216348568103af8a575b6532f\
5329cb62d410b0f7a97b55c01a10b0b338fc02388e4b06eccb8748ec2ffe\
525bc04262320fde27d91562caf7caae27609390495c02b6a98025dd260d\
0b0c60455e3adc4fddb01a0296542306a6e065c05cca9c87d3e75b6b0bd8\
60b5832260f506ec6d1c7c01fc2d5e31600927c9abfa027654b2420c1afc\
9ee4fca90feb0b58d2ff59f4caecb15eb16736021612b0497068eec72b07\
6c9c7676bd2fd26aa9ac469c17a89b333badef8377f86e0d017b19172e12\
9f6f07de902f046cc580259e868b8422f1fe555ec0a271de07495d00f86d\
75bde60235e2b8f4e0f72170640e18c3570f58efba60c2faffefaf2f8f0d\
d8633d015b2d608947272161cf73a73b8de378b193fd4126691b7654a0d5\
525d8d38283df88df346b03fc2db442b072cf9839ea4dd496f0d3fa65c43\
9346eea980ad16b0b43791f77ef8473b51e07cc245e6287692364e8d736f\
736aa811e7e507bf5ede8d6cf2d3fac419c1ab076c90b88d2987a2bf7795\
5aa52454ea1743015b2d606973d916b3bf1681eaefec4501b39d1e7fffb9\
d4a730bfa5fef4c3a4fb9c9bf7093b653acfd7252bc4dcc1ef43fe3f4fba\
3cf43fc6b5046cebf7383462dfe295dc0839ca1b099f8f05ac68c07a1927\
d2d5d9d959de0dffb780cd12b2f9e3e53be985dfcc8dd81b7e8f57a99b81\
b7252bc4dcc16f9c5ff9250c10c38f714d01eb5da75e259fded88e0793e8\
e68df4492fa0057323a6fdff8e2a28dcef5cc08a7d53726ac06ecf373f39\
98ecfc7d6cfb3b9328732848de88ab683a9d1ebcbb2a7933302a5b21e60d\
7ebd8003b494b09d455c57c09293f17d2746b3d96c169de53742522eb657\
ef0ea6d3d9d7703e10b0a2011b5512b0e4f7a1cecea2283a39bbca1d0af2\
9fed7e2cf15ce7ba64859853237e08cbe3cd857dfacfa29a5e57f9ea4da9\
63372b7eb19d0a58d180ad38843d4a1cc0c29c0715aae56727bc2d320c85\
5f7596a399727d880e9eedececec3c3b88aef2df28582960bdf35207602f\
f0863ce85d0b01ab65087b94318015e876e7a7bcf8372d647eb20fe51b24\
83a02314aa9280155d71282561bdeb3aae73773d60694da802011b5e97f9\
d179a1945f146f73649d2fe3d2835fd2cddbf1a603b6b55dea10dcfe16b7\
fc523d12b0c2012b595fdc08d86fa50ac441b142b5c4dc8eb7252bc4ccc0\
27457370bde980954cd83f0a0f613d012b7c6bba7dbd62c0ca2ca97479bf\
e0295ae2da392a5b21669d6983228f1397ae2ad77505ac5495f88fe29f63\
2a60c5774ae86dd87f0e13035666c997cb27f90f9352cfb6d56bc471e9c1\
2fa5bd1fd8c71b9fd516b0ad41d185f12fff51e273440256e2aab31b3486\
cdfbd3c480bd2c9eaff3fb4bdbb05fc3b5f37de9426754309abda0b3fb70\
abc68005ecc2bc6310f2391e0958895333a4be384c5a85efcbee1e15ad4e\
e6497556dee951a2453c2e5921660c7e6913407a018d8e375bf506ac5099\
78dc2f77a578256065aefdb9eb5e5ffeba951ab0adadf1bc4869f26bf236\
4c2abf76a6c524604e5d547802c87e40be6a0ed8d6d6243062e74fca7695\
2f7a0256aab8ca5e94f7eb98334d3fe907a10b8e5eeef7cb8da39725a69a\
46a55b61e3e2d1ccd9fc2f9795ba03b635d83f5fe918e49e09ff190858c9\
bb97f41d3bff6b11b269e6a8b21b90b193cc439b756c67fd12fb6e5cb242\
4c1dfc06652f52b3af3f597bc002ca89f92467570ef62fd3a3a9c9b1427b\
20f1d05cbeffbec6df34af6c1b4d3396418ce7fba3fc4d382e7ec92d1a93\
71e9c16f9edb2e4adefcc36fc15c47c03eafe1355fe5187cfe05a7a5a2d9\
4abd84d58e0785fef9fdf03fd6df3dbc1991cbd3c39b0bd4de0fd990edcf\
8bfade8ad9e5e9ebc9a85f6a13e24fc79351e99db79bb45674d05dc420e9\
27ef17dffccb1f4ee9a5edf9fb2df0ffcadad0ff4a5df13ab50f3ab97d14\
3e1d1fee86076430fe7169e64faf3b99ae8de86f8fbe1cc2d1bd157ec7bd\
d1d75f32de1ddd2b7e64faa36f9bd0caa3fa7df3b7fb9b3e92bb7f6d48a9\
63f9d741dc5de54c00000000000000000000000000000000000000000000\
000000000000000000000000000000000000000000000000000000000000\
000000000000000000000000000000000000000000000000000000000000\
00000000000000000000000000000000000000000000008066fbffddd184\
8d4adc88950000000049454e44ae426082
""";

let nftInscriptionRawHex = """
020000000001011771decbce2766b39d8fe66f4dc11737b3146c71f8cc6a\
e1397384c5e508e7f10000000000ffffffff012202000000000000160014\
e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d0340cc1e7b0b5fa18b28\
dce702e4e8ed2e91069d682b8daa3a773774bfc7d0e6f737d403016a9016\
b58a92592ad0b41682e6209167444eb56605532b28e9be922d3afdda1d00\
63036f7264010109696d6167652f706e67004d080289504e470d0a1a0a00\
00000d4948445200000360000002be0803000000f30f8d7d000000d8504c\
54450000003070bf3070af3173bd3078b73870b73070b73575ba3075ba30\
75b53070ba3078bb3474bb3474b73074bb3074b73376b93076b93373bc33\
73b93073b93376bc3275ba3075ba3572ba3272ba3375bc3276b83474bb32\
74bb3274b83276bd3276bb3474bd3474bb3276b93474bb3474b93274bb32\
74b93476bb3276bb3375ba3275ba3373bc3273ba3277bc3375bc3375ba33\
73bc3374ba3174b93376bb3375bc3375bb3375b93375bb3374bb3376bb34\
75bb3375bb3375ba3374bb3276bc3276ba3375bc3275bc3375bb3275bb33\
74bc3374bb3375bb7edf10e10000004774524e530010101f202020303030\
30404040404050505050505f606060606f707070707f7f7f7f8080808080\
8f8f909090909f9f9f9fa0a0afafafb0bfbfcfcfcfcfcfdfdfdfdfefefef\
ef6a89059600001c294944415478daeddd6b7bd3d6ba2ee0d8350eb00c81\
ec5533cbac69d6da99383334dbe510904b66129383feff3fda1750688875\
1892255b52eefb634b1259d2a3f1ead5d0f0d61600000000000000000000\
000000000000000000000000000000000000000000000000000000000000\
000000000000000000000000000000000000000000000000000000000000\
000000000000000000000000000000000000000000000000000000000000\
004d08020000000000000084eaef3c9b4e274f87f604546e27ba8abf3a19\
db1b50a96114dfb01031a8d0cf57f18f0e7a760a54e4b778c9c2ad1854a2\
7f1c27586cdb3350c1edd7224eb667dfc0aa9e5fc569666ec460b5f2f0df\
71063762b08afb8b389b3211ca978771aec82006a5fcf87039cd8587ce50\
66f8ba8ac3cc0c6250d0cec738d8c53fed2f2820bb79a89d08ab185fc545\
a91321b03a8ce21216ea44c8372c152f2fb1400d375fb722b6630f427abc\
f6aee2d544220675c54bc420adb5318b2be25e0c6ec72b8a2bb4186bdac3\
dfb5e1495cb5d903fb15bed48657711d4ec67d3b973b6eb8574fbafe1ac6\
343cb8cba5e1248a6bb69031eeead8557bba648c3b3a74ed1c9cc56b74f5\
4e5b913b13aebde82a5ebf9399af8da0ebe17a7ab09170fd1db2b1ee3ddd\
bce57a3a79b77a5978383e5fb95c8c0e9eba29a33ba3d6836793d94925e3\
d6f9686b6bf0a69ab12c3a98ec2819696baa86c39d67cfa6b37767155684\
6fbe3e385e7d10fbdb59f4ee60fa6c6767e899340d8dd2b3835974d3c9d9\
592db759974fbefdc98a06b1a5b09d9dfdf041a2d94429c966ed44ebea4f\
1cde1c63aa1cc4b29fa1ed2923d9dce8b5b6787dbefbbaa9a641ccbb2f34\
48eec2f1955587fbcbb748bbeb1ac4e203479a8de46b5d4fb4e683c4bfbf\
7fbda6bf3f73ac59bfe16233d5e106ea446318eb77bc9eeaf0d7ac6d189c\
ac27618f1c6ed66cbca99baf5b9b4d0802b1965bb185e3cd9a9daebb35bf\
d18819c258af876be82d0c4207d3fa231639e2acd5a431f15a4bc42e7cdd\
3a6bf57ec3f75eeb8e98b75ce8cc2d58f1787d89d8bcce80fde290b34ef5\
bdf13f2f3d37a9cee762024617027679385a65ab06b5558a0246fb03369f\
acfe5ad6e88d802160950f5e3787b1530143c07e48d7eb51955b37383c17\
30da2caa345d35ac22bf5d6dc6048c7606ecf27054d77a18db93ea6ac527\
0e39ed0bd87c7f54ef660ec6c7972623721703f6e970773d4b398d267301\
e34e05ecf470cddfdd359acc2f058c3b10b04faf27a3cd2c42b8bd7b583e\
6502c65a9598ec7b393fdc54b66ea66c7a7c2a6034dddb2283d6e9ebe978\
bb496be76eef4e0ee7a702467b03f6e9d3fcf8f574321edd6bee87e88f46\
e3e9e1f1fcf4536ee9e875159a12b07fddbb77af7d1fa87fefde76c6f494\
9f1c721a12b0f64e7a1030044cc0b8038ed2cfc51702062b9aa69f8bd3d6\
7ea86b0143c0ea93d14574c411300143c0040c5609d82b018315657cf7c3\
1f020602b66490b174b6238e80d51730df5f848009187721601f040c56f4\
3863259bb67ea68cef3cfbd31147c07c2604cc6782554ec63f050c340404\
0c015ba72e764611b03604ec0f479ca604ec42c060551d9c183b113004ac\
3e9d7cc70d011330b8ad830bc40818cdd1c125ce8e040c01ab4f27175345\
c0040c6e3bedde57fdbc17301a23ea5ec03af891e862c09e0818a84d0802\
a7040c1d81353af3059708d84602e6cb5558b30e3e95f5ed45344707e715\
59391b011330ee84eebd3cd513309aa37bafff5ad8977604ec8380c18a76\
3bb7c65917977aa4b51edfa580591691757bd8b9824ac070c7e2ae923b1f\
b08bce05ccaa6dac5bf79e1a4d058c06b94b01b3e60d6bd7b9a9b11695a2\
493af77247c60b382f1c6e1a14b076be9e68cd1b9aa4732fd877709511ba\
19b05f5c31a0be5b967606ec4cc0d013d848c0ac18c0da75aeab9df160af\
e770b36e19cf655f752d608e366bd7b5a97b0301a325016be5e4f3875e68\
a649baf64ab3179a69cb09b9e8d8e7f1be258dba676965c0bc6f89a6408d\
265e07a3513af6dcc8eb60344bc75e087b2b60344ac7de57f1b60acdd2b1\
af41f7b60aad39237f71bd80fa6e5ada389dde17c8d22c1d9b4e1f7b5b85\
46e9d63a82be1d8c86e9d674fa879d5ba998ee06ac8593f71e9b4c8f53b2\
3ebbe6fad22cddfafa8789b9beb4a72dd0bec988befa81a6e95463db5444\
9aa6538f66232bd3d330a75d9a2b652a224df3be4b177debfae2b6c50d25\
7748971a6f5645a471ba3495e3b1995234cd6e87a672ec5a1591a6e9d2fc\
d8899952b4e9bea56d53398e4ce4a071e2ee3c697e6f22078d73d69d87b3\
9135a568d359d9b627cdd79e33d3386fbbf31d7cb1256fd019a8cd43df1f\
4bf38c3bf3f0c873661a68b733a7a5c7603450771e841d593080e6e975a6\
35f0be3bed1a3ae4ac2b4d08024f8f4ebdcf4cbbcecb76cd7f883d06a381\
de76a44fffd063309a68da913efd63af5bd244e38ef4e927de06a36da555\
9b56b238f2188c26caead3b7693e7da44b4f235d77a3bd7dad4b4f239d76\
a28dd8d3a5a799de76a23bf0d8a28834d3a4136dc489b9f434d36e27aefd\
ef351169a64127da88a7e6d2d350d75d6870c79a883454d4818bff634d44\
9aeaa803fd8189053968aa7107ba1c334d449a2a6b36625bdeb93cd544a4\
a9b22641bcf211604519ab0644edef71f87a66362c63b2d4453b3a0453af\
33d35c93d6f7b8233d0edad9e568c7c237b11e070d76ddf29bb0c7adbf42\
d06951cb6fc2a6e671d064472d3f4123f33868b2dd763f46ca7a0a664529\
9ca177fbfa40f79db57a3ae2cc6366da7b13d6fcd7a916195b3f7070d9bc\
ac09f58d6fd43f760b468b6fc21adf879bb905a3e94e5b5c232edc82d1e6\
9bb086d7888f634fc168ba517bcfd299898834df756ba7f35d5b508ae6cb\
78272cfed8e40dcf6a807a178c569ca74d9e8f98350ff1dc71a521321bf5\
0d6e730c634d7ada206a679b63167b558536c85a37a0b96d8ecc014c8548\
4b6ac4c6be76993980cd1c555a5223367408cb1cc0f4106992710b87b099\
0a91d6d488d7ad1bc2b207b0b1634a931c659eaec3b60d605e05a35932e7\
2336f159d8fdcc0df6c596b4a9cdd1c0373f162ddb5eeeb8cc4761f1a269\
7d8edf622d0ebad3e668da63a5ec0e871607cdf332fb9c4d0802fd6793b6\
b59f5d206a71d0c0212cfb9cbd68522731bb836816074df4266ecb6d58f6\
0d98018c461ae59cb68de9d5ff1c1bc068a1a81d27eefd2b03185d1cc2e2\
bd56e4cb00463befc21a91b0dc7c19c068aac175dcf4d161e72a3680d156\
2ff3cedef8dd667b893fe76ee0b9018cc6eae50e61f16293cfc37ecbddbc\
f8574791e69ae49fc18b8d4da4ed1fe76f9d5988345a947f0e6faad5717f\
11b06ddb0e21edee73c4717cb28932f179c086e970d0fe3ec7e73271edd3\
d5872143ab0e07dd2812e378b6de41ecf955d0568d1c3e3a5124c6f1628d\
efaf840d5f71fc2f478fe69b849dcd6b6bd8f7f7023748079156f83df084\
8e0fd611b19d45e0d65cba01a3157a27a109abbfd9713f0add16eb04d0b1\
dbb02f11dba9b53afc77f086b801a33db6c3cfeb1afb89fdbdabf0cd78e3\
a8d11ee378f3117bb228b00dffe93b68b4c87e91842d6a983cb51315d902\
4f98e972c22aef760c0bc54bbee87ac22a8d5891de867c713712164715dd\
8a15ea6dc81777266195743b0ac74bbeb833095b3d62e3452c5fdc1593c2\
095bada158ac75f8c55c7f9ef61a5d178f58e96ec7fde2f18a0f1d23da6c\
705efca42fb76a47d1d6a1f951742261c725cefbe2b762c57b1b9fe7cf7b\
c192bbd8ea281eb1e2bd8dcfd3a3b437e8c48d588932b1d01bcf257a1b6e\
bfe85099f8a64c0042bb1dc35999dfae3ca44326d775d589a56ebe74e7e9\
da2056aa8a8b4d0802f7f296b2df59941abe2c904de706b1f3eaebc461b9\
d8ce75377027f6edfb5886155787e74f1c0b3a697c5eae4eacb23a8c0fdd\
7dd159a59e8925d689fde372d5e17d070175626e3ff179a9eaf0d2d26ca8\
13f307b172cd8dcb7dd5212296ffc2f35ea951f058ef903b5227febe4ab3\
63e763a99b2f3337702b96ff7511a5de4a89cfc58b3b16b179a941ac546f\
dec40ddc8ad5466f0311ab8f07cbdc5dfb7547ccb443743bea8b97de0622\
5657bcb40ea1744351eb1036d6edd03a84fa2236132ff8a14edcd7db8016\
743bf43620d1fdf32a6ebeec47a8eb56ccbc0dc8aa137f77f3057546ecbd\
275fd0bc3a517508b5d589aa43088fd8b9ea106a54e8eb22ac67034507b1\
379e2cc3e69b1d9a1b50db2066f882fa0631c317d4368819bea0be41ccf0\
05b50d62862fa844e233b163c3175434889d9bba0135dab79e28ace94eec\
c4175542e5111b1f9f7efaf4e9f850730300000000000000000000000000\
000000000000000000000000000000000000000000000000000000000000\
00000000000000000000000000000000000068bff16c49effbfffc9fe5ff\
698f41016fe3253f7dff9fd1f2ffb4c740c040c040c0046c15b3aa0ceedc\
ae1b26ec8583b01f7db2f483e3b01f7cbef483ff2b608d1657e5d19ddb75\
bdebd2bb61f9bcbd08fb9bcb7f7226600276778aab781a34f695dd7fbb35\
ec770113b0861a25ec8628e407c7093ff82aa84d0802a05ffab173f76002\
76a76ac49f4a55888135620d15a28009585dfefbd992b5d488c3b23b30a1\
427cd2fa80f5968fc28e807522602b9f3d256bc471e20e0ca81167657b23\
4d0ed860f94ffc216002965a23f64a55884159a9a3421430016b6cc0126b\
c417e52ac4803d584b85286002d6dc808dcbd488e3943d987bf7564b8528\
6002d6dc8095aa11a3943db8c8fb6b8b3a2a440113b0e6062c312c2fca55\
88b9bbf0f1f24f8c054cc03a1db01235e23875174e8b5688010d150113b0\
36072ca946bce895aa107393b95c217ed8123001eb74c012e3f24bb90a31\
6716484d15a2800958930356b8461c67ecc3e9062a4401abd728d5ff4938\
030ed3ff79ff6e06ac708d789a11b0680315a2806dcaa068f17327039658\
233e2a5721660e4975558802d6ad800dd734d2f587c3dc3f55c9d933890b\
9d2093cc80bd2854210e56fbf8b5062cfcef0b5858c03e1fd21ffdb083fb\
3b93d9c9591cc7f3bf767fd63fcefdd5c3f403fbec60767276f56d83cfa2\
e860b2d3af3160bdb8508d789a19b0a84885384fd8530f6e7ffc77d3a70f\
d619b0efc7f9b3ab93e820efcf0b5860c01e67eca6fe24baba795a14dca7\
d3c0226c78e3cfdc9a24f16eb25353c08ad588c39c5651af7c85387c363b\
4bfea557efc6c3b5042cf9002c664f05acce80ed1c5fddbaeed611b0fe5e\
9473f2465f4fb383e89b849321baa5648df8aa5c85985123e654883b7b27\
d9bf4d0802f8ddd3ba03d69fa41f80abd952c2bfefe2840d5fdc3a0a0702\
961eb061b45cd8541fb0fede55c8b384689cf5a0372e35a2f58abc7a729a\
b781652ac47e5ebabe9eb5e33a03967b00a25b254491a33017b0d4803dbf\
8aeb0f5860bcbe9c66412763a192b1408d38ccfd83bdc215e2f020f4c32f\
86b505ec79c036fc388a09581501eb1f27eeac8a03b6b388eb52b68ff8aa\
5c8598ba87532bc4e1acc8c7d9ab27603b1f8bff7901ab2060c38ff11a02\
f63c8e371ab00235e269ee1f8c8a5588e143f75727c31a02167e006e8ca1\
02b67ac0fa8b780d01fb2dde70c0c26bc4fc0a31a5c39f52210e3f16fe40\
cb65e2aa01eb17b9a95d6c0b5875013b8ed710b0c2f95a541eb049e8b4c2\
e57f781db68b8f922bc4de79bc7ac2560cd8b0607dfeb3805515b0ab780d\
012b9caf595479c0926ac4455085f8e751d88e48ab10c72506e545afca80\
0d0bdffffe2c6015052c5e43c08685cfaf41f5010bad119737f6c528a846\
7c98da432c3184dd5e6760a5800d4bf4971e09587b02b6287e76d510b0c0\
1a719250e75d8724f328f529739921ecd65254ab04ac4cbee28ba180b525\
60c5cfaf411d01eb85b503972bc4a4ecfc115221fef9ed7f9519c27e1c24\
5709585426dff1494fc05a12b01203581d014bfc9d3fe55788e3a4c5812f\
422ac4ef53aa4665cef06945012bdbc03d10b076042cf96f5ebe9eec6edf\
fb6c7bb43b3d3ebdf57375042ca9464d08027c1152216e85d488e9156272\
043e7d9acf4f3f850e61e5039676037c7a38feb2ff479337e7a9b76102d6\
82801d25fdfed1d2839addefc779be554fc0826ac4d3c4f3e6287f164846\
85786b08bb3c9eecdefbf67fb677d3ceef1fc25f3e6089f5c3e5e1f6cd6d\
1f9fa73c4e17b016042c6162c4bf927fc5e8cdf7dedbd1fc9b84b7fde7b7\
84eeaca4d4f6022ac4901af1617640a2bf4feda56b4bcaf9fd43f84b072c\
7102c77c10b409e3adbff77142486f1d05b3e9730336ff7665bd77afc280\
05e7ebf3dfdc3fbffd6d7515be0fff32bf467c995ce7f5726bc4495685f8\
3da1cb23f7d7b1f5f7c4e3d15b3d60891dc45f93f6fc49e66342ef83ad1c\
b0cbfd7eeeaf2e13b0e54b7bf6f73dee3fa92d6083fc1a719152f8bccdab\
114fb32ac4bf3e464abcbe7cea9c1ab16cc0923a1cff480ef971ea733c01\
ab2060fbfd805f5d2660cb7fb2d83a4b55aee8925b233e4c3bc7f26ac484\
56c28f091cc5e7a3ac4d4b1ac3fe583d608bb0f1eb4bc24e32ae3e02b65a\
c0920e7e6d01fb636301cbad118fd2eabcbc1a31a142bcb5ccc5ef39cbcb\
649768250336ce9d2272f380677c46015b2960e783ad3506eccf8d052cb7\
465ca4b6c6a2ece754a7050be1840e4fe6e85a326051d8a14e8de32b01ab\
22609783ad7506ecfb5cedb5072ce997dd7cdcf430fd2e649c35bee45788\
01aeb306c172011b66dd5685ec9e85805511b0f1568d014bdac27f6e4d08\
022a602fb377d7517a2730bb46ccaf10f31d656d5ab9808d836629670d61\
3f09d8ea017bbf5567c0121f562e0e1e6c24603935e222e3e169668db87a\
859878bff462c580bd2f7007967c15792160ab07ec51bd014b5d073075ad\
d1fa0296b4317f5fd41f668dede38c64565121263daa9eae18b045ce1cfd\
25efd38eb880950fd89f5bf506ec28e3d1f655c692beb504ec65d605e628\
eb59712fe3141f075fb58a1dca57ab05ac97fdf03bc124ede410b0f2017b\
5173c0f2a7929f640d66d5066c94f5b8699139bd2ea3468c8a5588fd074f\
9f3d9bde7694b5bf4b052ce1684fb31da73ded13b0f2017b5073c002df86\
3a3978daaf3f60893562488598d4c888d22bc4b47b9de1647612bec6d48a\
012bf5a267f25f11b0d201bbd8aa3b6093e0a3194d8675072ca3463cca2e\
a87aa9cfa9422bc49d8382ebb7ad18b06905017b20602b066c5e7bc00abd\
7b727bf1e6aa03965423be0aa91093b6e445810ab13f39297c76af18b0a3\
0a02f648c0560cd887fa033628f4cafcadaf20a8fadbe5526bc48779cf07\
d36ac4a00af149998531560cd8db0a02f68b80ad18b069fd01dbda2e94b0\
8b719d014bad118ff25a6e6935624085d83f2e75760b98800505ace018f6\
c3f2e855072cb546ccab10536bc4fc0a7158725d7e0113b0b0806d0dde94\
4d58e55f407c9d5c23e656886935627e8558365f4d08d813016b48c08e72\
0aa571a141ec9ff505ec284d0802b946ccad10d36ac4dc0ab174be9a1030\
4d8ea604ec7dee9d48a1883daa2d604935e234a4424cda965f022ac459bc\
a18069d3b72d6059ef229f063c0c1abd09ce58545bc0926ac445488598d4\
2089f22bc4151ef7ae18b0490501f3a0799d01cb7a55327016f1f6f8f8bc\
d010567dc0926ac407b390697bcbbbe4a2975b21a61488a7afa7d3f14d93\
ca03b6bbfcdfffefb8a02d015b63c02e0afde6d409af83db2b8d665dbdab\
0f58628d18522126d68851ce5e4a1cc0e6a37eb192bc54c006e1134f4b9c\
6702b662c07a71d6410d6c1d64d8de9dbe9e5fa63f0dab2d604935e25548\
85985823e6558809ff2079f99bea0396f04123016b4cc0928abef40be0a2\
70c0be8d6687f3ccdbeb1a0216348568103af8a575b6532f5329cb62d410\
b0f7a97b55c01a10b0b338fc02388e4b06eccb8748ec2ffe525bc0426232\
0fde27d91562caf7caae27609390495c02b6a98025dd260d0b0c60455e3a\
dc4fddb01a0296542306a6e065c05cca9c87d3e75b6b0bd860b5832260f5\
06ec6d1c7c01fc2d5e31600927c9abfa027654b2420c1afc9ee4fca90feb\
0b58d2ff59f4caecb15eb16736021612b0497068eec72b076c9c7676bd2f\
d26aa9ac469c17a89b333badef8377f86e0d017b19172e129f6f07de902f\
046cc580259e868b8422f1fe555ec0a271de07495d00f86d75bde60235e2\
b8f4e0f72170640e18c3570f58efba60c2faffefaf2f8f0d4d0802d8633d\
015b2d608947272161cf73a73b8de378b193fd4126691b7654a0d5525d8d\
38283df88df346b03fc2db442b072cf9839ea4dd496f0d3fa65c439346ee\
a980ad16b0b43791f77ef8473b51e07cc245e6287692364e8d736f736aa8\
11e7e507bf5ede8d6cf2d3fac419c1ab076c90b88d2987a2bf77955aa524\
54ea1743015b2d606973d916b3bf1681eaefec4501b39d1e7fffb9d4a730\
bfa5fef4c3a4fb9c9bf7093b653acfd7252bc4dcc1ef43fe3f4fba3cf43f\
c6b5046cebf7383462dfe295dc0839ca1b099f8f05ac68c07a1927d2d5d9\
d959de0dffb780cd12b2f9e3e53be985dfcc8dd81b7e8f57a99b81b7252b\
c4dcc16f9c5ff9250c10c38f714d01eb5da75e259fded88e0793e8e68df4\
492fa0057323a6fdff8e2a28dcef5cc08a7d53726ac06ecf373f3998ecfc\
7d6cfb3b9328732848de88ab683a9d1ebcbb2a7933302a5b21e60d7ebd80\
03b494b09d455c57c09293f17d2746b3d96c169de53742522eb657ef0ea6\
d3d9d7703e10b0a2011b5512b0e4f7a1cecea2283a39bbca1d0af29fed7e\
2cf15ce7ba64859853237e08cbe3cd857dfacfa29a5e57f9ea4da963372b\
7eb19d0a58d180ad38843d4a1cc0c29c0715aae56727bc2d320c855f7596\
a399727d880e9eedececec3c3b88aef2df28582960bdf35207602ff0863c\
e85d0b01ab65087b94318015e876e7a7bcf8372d647eb20fe51b2483a023\
14aa9280155d714d0802282561bdeb3aae73773d60694da802011b5e97f9\
d179a1945f146f73649d2fe3d2835fd2cddbf1a603b6b55dea10dcfe16b7\
fc523d12b0c2012b595fdc08d86fa50ac441b142b5c4dc8eb7252bc4ccc0\
27457370bde980954cd83f0a0f613d012b7c6bba7dbd62c0ca2ca97479bf\
e0295ae2da392a5b21669d6983228f1397ae2ad77505ac5495f88fe29f63\
2a60c5774ae86dd87f0e13035666c997cb27f90f9352cfb6d56bc471e9c1\
2fa5bd1fd8c71b9fd516b0ad41d185f12fff51e273440256e2aab31b3486\
cdfbd3c480bd2c9eaff3fb4bdbb05fc3b5f37de9426754309abda0b3fb70\
abc68005ecc2bc6310f2391e0958895333a4be384c5a85efcbee1e15ad4e\
e6497556dee951a2453c2e5921660c7e6913407a018d8e375bf506ac5099\
78dc2f77a578256065aefdb9eb5e5ffeba951ab0adadf1bc4869f26bf236\
4c2abf76a6c524604e5d547802c87e40be6a0ed8d6d6243062e74fca7695\
2f7a0256aab8ca5e94f7eb98334d3fe907a10b8e5eeef7cb8da39725a69a\
46a55b61e3e2d1ccd9fc2f9795ba03b635d83f5fe918e49e09ff190858c9\
bb97f41d3bff6b11b269e6a8b21b90b193cc439b756c67fd12fb6e5cb242\
4c1dfc06652f52b3af3f597bc002ca89f92467570ef62fd3a3a9c9b1427b\
20f1d05cbeffbec6df34af6c1b4d3396418ce7fba3fc4d382e7ec92d1a93\
71e9c16f9edb2e4adefcc36fc15c47c03eafe1354d29015fe5187cfe05a7\
a5a2d94abd84d58e0785fef9fdf03fd6df3dbc1991cbd3c39b0bd4de0fd9\
90edcf8bfade8ad9e5e9ebc9a85f6a13e24fc79351e99db79bb45674d05d\
c420e927ef17dffccb1f4ee9a5edf9fb2df0ffcadad0ff4a5df13ab50f3a\
b97d143e1d1fee86076430fe7169e64faf3b99ae8de86f8fbe1cc2d1bd15\
7ec7bdd1d75f32de1ddd2b7e64faa36f9bd0caa3fa7df3b7fb9b3e92bb7f\
6d48a963f9d741dc5de54c00000000000000000000000000000000000000\
000000000000000000000000000000000000000000000000000000000000\
000000000000000000000000000000000000000000000000000000000000\
00000000000000000000000000000000000000000000000000008066fbff\
ddd1848d4adc88950000000049454e44ae4260826821c00f209b6ada5edb\
42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000
""";

