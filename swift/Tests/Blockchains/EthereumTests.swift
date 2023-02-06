// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class EthereumTests: XCTestCase {
    
    func testAddress() {
        let anyAddress = AnyAddress(string: "0x7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1", coin: .ethereum)

        XCTAssertEqual(anyAddress?.description, "0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1")
        XCTAssertEqual(anyAddress?.coin, .ethereum)

        let invalid = "0xMQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4"
        XCTAssertNil(Data(hexString: invalid))
        XCTAssertNil(AnyAddress(string: invalid, coin: .ethereum))
        XCTAssertFalse(AnyAddress.isValid(string: invalid, coin: .ethereum))
    }

    func testSigner() throws {
        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.nonce = Data(hexString: "09")!
            // txMode not set, Legacy is the default
            $0.gasPrice = Data(hexString: "04a817c800")!
            $0.gasLimit = Data(hexString: "5208")!
            $0.toAddress = "0x3535353535353535353535353535353535353535"
            $0.privateKey = Data(hexString: "0x4646464646464646464646464646464646464646464646464646464646464646")!
            $0.transaction = EthereumTransaction.with {
                $0.transfer = EthereumTransaction.Transfer.with {
                    $0.amount = Data(hexString: "0de0b6b3a7640000")!
                }
            }
        }

        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)

        XCTAssertEqual(try input.serializedData().hexString, "0a0101120109220504a817c8002a025208422a3078333533353335333533353335333533353335333533353335333533353335333533353335333533354a204646464646464646464646464646464646464646464646464646464646464646520c0a0a0a080de0b6b3a7640000")
        XCTAssertEqual(output.encoded.hexString, "f86c098504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a028ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276a067cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83")
    }

    func testSignERC20Transfer() {
        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.nonce = Data(hexString: "00")!
            // txMode not set, Legacy is the default
            $0.gasPrice = Data(hexString: "09c7652400")! // 42000000000
            $0.gasLimit = Data(hexString: "0130B9")! // 78009
            $0.toAddress = "0x6b175474e89094c44da98b954eedeac495271d0f" // DAI
            $0.privateKey = Data(hexString: "0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151")!
            $0.transaction = EthereumTransaction.with {
                $0.erc20Transfer = EthereumTransaction.ERC20Transfer.with {
                    $0.to = "0x5322b34c88ed0691971bf52a7047448f0f4efc84"
                    $0.amount = Data(hexString: "1bc16d674ec80000")! // 2000000000000000000
                }
            }
        }
        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)

        XCTAssertEqual(output.encoded.hexString, "f8aa808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec8000025a0724c62ad4fbf47346b02de06e603e013f26f26b56fdc0be7ba3d6273401d98cea0032131cae15da7ddcda66963e8bef51ca0d9962bfef0547d3f02597a4a58c931")
        XCTAssertEqual(output.data.hexString, "a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec80000")
    }

    func testSignERC20Transfer_1559() {
        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.nonce = Data(hexString: "00")!
            $0.txMode = .enveloped
            $0.gasLimit = Data(hexString: "0130B9")! // 78009
            $0.maxInclusionFeePerGas = Data(hexString: "0077359400")! // 2000000000
            $0.maxFeePerGas = Data(hexString: "00B2D05E00")! // 3000000000
            $0.toAddress = "0x6b175474e89094c44da98b954eedeac495271d0f" // DAI
            $0.privateKey = Data(hexString: "0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151")!
            $0.transaction = EthereumTransaction.with {
                $0.erc20Transfer = EthereumTransaction.ERC20Transfer.with {
                    $0.to = "0x5322b34c88ed0691971bf52a7047448f0f4efc84"
                    $0.amount = Data(hexString: "1bc16d674ec80000")! // 2000000000000000000
                }
            }
        }
        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)

        XCTAssertEqual(output.encoded.hexString, "02f8b00180847735940084b2d05e00830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec80000c080a0adfcfdf98d4ed35a8967a0c1d78b42adb7c5d831cf5a3272654ec8f8bcd7be2ea011641e065684f6aa476f4fd250aa46cd0b44eccdb0a6e1650d658d1998684cdf")
    }

    func testSignERC20Approve() {
        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.nonce = Data(hexString: "00")!
            // txMode not set, Legacy is the default
            $0.gasPrice = Data(hexString: "09c7652400")! // 42000000000
            $0.gasLimit = Data(hexString: "0130B9")! // 78009
            $0.toAddress = "0x6b175474e89094c44da98b954eedeac495271d0f" // DAI
            $0.privateKey = Data(hexString: "0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151")!
            $0.transaction = EthereumTransaction.with {
                $0.erc20Approve = EthereumTransaction.ERC20Approve.with {
                    $0.spender = "0x5322b34c88ed0691971bf52a7047448f0f4efc84"
                    $0.amount = Data(hexString: "1bc16d674ec80000")! // 2000000000000000000
                }
            }
        }
        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)

        XCTAssertEqual(output.encoded.hexString, "f8aa808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844095ea7b30000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec8000025a0d8136d66da1e0ba8c7208d5c4f143167f54b89a0fe2e23440653bcca28b34dc1a049222a79339f1a9e4641cb4ad805c49c225ae704299ffc10627bf41c035c464a")
    }

    func testSignERC721Transfer() {
        // https://etherscan.io/tx/0x3cde660762810d951d5cc57f8d907fbf229054908f1a770c3fb0b7b375cf4f82
        // real key 1p
        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.nonce = Data(hexString: "02de")! // 734
            // txMode not set, Legacy is the default
            $0.gasPrice = Data(hexString: "22ecb25c00")! // 150000000000
            $0.gasLimit = Data(hexString: "0130b9")! // 78009
            $0.toAddress = "0x0d8c864DA1985525e0af0acBEEF6562881827bd5" // contract
            $0.privateKey = Data(hexString: "0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151")!
            $0.transaction = EthereumTransaction.with {
                $0.erc721Transfer = EthereumTransaction.ERC721Transfer.with {
                    $0.from = "0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1"
                    $0.to = "0x47331175b23C2f067204B506CA1501c26731C990"
                    $0.tokenID = Data(hexString: "0fd8")! // 4056
                }
            }
        }
        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)

        XCTAssertEqual(output.encoded.hexString, "f8cc8202de8522ecb25c00830130b9940d8c864da1985525e0af0acbeef6562881827bd580b86423b872dd0000000000000000000000007d8bf18c7ce84b3e175b339c4ca93aed1dd166f100000000000000000000000047331175b23c2f067204b506ca1501c26731c9900000000000000000000000000000000000000000000000000000000000000fd825a04c5d8242a8c2db1cfa352a3486dd85c82824e01b9bcf0ce4170fcd2329fb7bcaa02d85ab09e750a73fd4dd26b142830ada1e991f8474795b43d96d93e65caaefe7")
        XCTAssertEqual(output.data.hexString, "23b872dd0000000000000000000000007d8bf18c7ce84b3e175b339c4ca93aed1dd166f100000000000000000000000047331175b23c2f067204b506ca1501c26731c9900000000000000000000000000000000000000000000000000000000000000fd8")
    }

    func testSignERC1155Transfer() {
        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.nonce = Data(hexString: "00")!
            // txMode not set, Legacy is the default
            $0.gasPrice = Data(hexString: "09C7652400")! // 42000000000
            $0.gasLimit = Data(hexString: "0130b9")! // 78009
            $0.toAddress = "0x4e45e92ed38f885d39a733c14f1817217a89d425" // contract
            $0.privateKey = Data(hexString: "0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151")!
            $0.transaction = EthereumTransaction.with {
                $0.erc1155Transfer = EthereumTransaction.ERC1155Transfer.with {
                    $0.from = "0x718046867b5b1782379a14eA4fc0c9b724DA94Fc"
                    $0.to = "0x5322b34c88ed0691971bf52a7047448f0f4efc84"
                    $0.tokenID = Data(hexString: "0x23c47ee5")!
                    $0.value = Data(hexString: "0x1BC16D674EC80000")! // 2000000000000000000
                    $0.data = Data(hexString: "0x01020304")!
                }
            }
        }
        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)

        XCTAssertEqual(output.encoded.hexString, "f9014a808509c7652400830130b9944e45e92ed38f885d39a733c14f1817217a89d42580b8e4f242432a000000000000000000000000718046867b5b1782379a14ea4fc0c9b724da94fc0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000000000023c47ee50000000000000000000000000000000000000000000000001bc16d674ec8000000000000000000000000000000000000000000000000000000000000000000a00000000000000000000000000000000000000000000000000000000000000004010203040000000000000000000000000000000000000000000000000000000026a010315488201ac801ce346bffd1570de147615462d7e7db3cf08cf558465c6b79a06643943b24593bc3904a9fda63bb169881730994c973ab80f07d66a698064573")
        XCTAssertEqual(output.data.hexString, "f242432a000000000000000000000000718046867b5b1782379a14ea4fc0c9b724da94fc0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000000000023c47ee50000000000000000000000000000000000000000000000001bc16d674ec8000000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000040102030400000000000000000000000000000000000000000000000000000000")
    }

    func testSignStakeRocketPool() {
        let function = EthereumAbiFunction(name: "deposit")

        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.nonce = Data(hexString: "01")!
            $0.txMode = .enveloped
            $0.gasPrice = Data(hexString: "77541880")! // 2002000000
            $0.gasLimit = Data(hexString: "0320c8")! // 205000
            $0.maxFeePerGas = Data(hexString: "067ef83700")! // 27900000000
            $0.maxInclusionFeePerGas = Data(hexString: "3b9aca00")! // 1000000000
            $0.toAddress = "0x2cac916b2a963bf162f076c0a8a4a8200bcfbfb4" // contract
            $0.privateKey = Data(hexString: "9f56448d33de406db1561aae15fce64bdf0e9706ff15c45d4409e8fcbfd1a498")!

            $0.transaction = EthereumTransaction.with {
                $0.transfer = EthereumTransaction.Transfer.with {
                    $0.amount = Data(hexString: "2386f26fc10000")! // 0.01 ETH
                    $0.data = Data(hexString: EthereumAbi.encode(fn: function).hexString)!
                }
            }
        }
        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)

        // https://etherscan.io/tx/0xfeba0c579f3e964fbc4eafa500e86891b9f4113735b1364edd4433d765506f1e
        XCTAssertEqual(output.r.hexString, "fb39e5079d7a0598ec45785d73a06b91fe1db707b9c6a150c87ffce2492c66d6")
        XCTAssertEqual(output.v.hexString, "00")
        XCTAssertEqual(output.s.hexString, "7fbd43a6f4733b2b4f98ad1bc4678ea2615f5edf56ad91408337adec2f07c0ac")
        XCTAssertEqual(output.encoded.hexString, "02f8770101843b9aca0085067ef83700830320c8942cac916b2a963bf162f076c0a8a4a8200bcfbfb4872386f26fc1000084d0e30db0c080a0fb39e5079d7a0598ec45785d73a06b91fe1db707b9c6a150c87ffce2492c66d6a07fbd43a6f4733b2b4f98ad1bc4678ea2615f5edf56ad91408337adec2f07c0ac")
    }

    func testSignUnstakeRocketPool() {
        let function = EthereumAbiFunction(name: "burn")
        function.addParamUInt256(val: Data(hexString: "0x21faa32ab2502b")!, isOutput: false)

        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.nonce = Data(hexString: "03")!
            $0.txMode = .enveloped
            $0.gasPrice = Data(hexString: "77541880")! // 2002000000
            $0.gasLimit = Data(hexString: "055730")! // 350000
            $0.maxFeePerGas = Data(hexString: "067ef83700")! // 27900000000
            $0.maxInclusionFeePerGas = Data(hexString: "3b9aca00")! // 1000000000
            $0.toAddress = "0xae78736Cd615f374D3085123A210448E74Fc6393" // contract
            $0.privateKey = Data(hexString: "9f56448d33de406db1561aae15fce64bdf0e9706ff15c45d4409e8fcbfd1a498")!

            $0.transaction = EthereumTransaction.with {
                $0.contractGeneric = EthereumTransaction.ContractGeneric.with {
                    $0.amount = Data(hexString: "00")!
                    $0.data = Data(hexString: EthereumAbi.encode(fn: function).hexString)!
                }
            }
        }
        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)

        // https://etherscan.io/tx/0x7fd3c0e9b8b309b4258baa7677c60f5e00e8db7b647fbe3a52adda25058a4b37
        XCTAssertEqual(output.r.hexString, "1fc6e94908107584357799e952b4e3fb87f088aeb66d7930a7015643f19c9e7f")
        XCTAssertEqual(output.v.hexString, "00")
        XCTAssertEqual(output.s.hexString, "2c56a0b70ff2e52bf374a3dcd404bc42317d5ca15d319f5e33665352eb48f06f")
        XCTAssertEqual(output.encoded.hexString, "02f8900103843b9aca0085067ef837008305573094ae78736cd615f374d3085123a210448e74fc639380a442966c680000000000000000000000000000000000000000000000000021faa32ab2502bc080a01fc6e94908107584357799e952b4e3fb87f088aeb66d7930a7015643f19c9e7fa02c56a0b70ff2e52bf374a3dcd404bc42317d5ca15d319f5e33665352eb48f06f")
    }

    func testSignJSON() {
        let json = """
        {
            "chainId": "AQ==",
            "gasPrice": "1pOkAA==",
            "gasLimit": "Ugg=",
            "toAddress": "0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1",
            "transaction": {
                "transfer": {
                    "amount":"A0i8paFgAA=="
                }
            }
        }
        """
        let key = Data(hexString: "17209af590a86462395d5881e60d11c7fa7d482cfb02b5a01b93c2eeef243543")!
        let result = AnySigner.signJSON(json, key: key, coin: .ethereum)

        XCTAssertEqual(result, "f86a8084d693a400825208947d8bf18c7ce84b3e175b339c4ca93aed1dd166f1870348bca5a160008025a0fe5802b49e04c6b1705088310e133605ed8b549811a18968ad409ea02ad79f21a05bf845646fb1e1b9365f63a7fd5eb5e984094e3ed35c3bed7361aebbcbf41f10")
    }

    func testGetPublicKeyFromXpub() throws {
        let wallet = HDWallet(mnemonic: "broom ramp luggage this language sketch door allow elbow wife moon impulse", passphrase: "")!
        let path = "m/44'/60'/0'/0/1"
        let xpub = wallet.getExtendedPublicKey(purpose: .bip44, coin: .ethereum, version: .xpub)

        XCTAssertEqual(xpub, "xpub6C7LtZJgtz1BKXG9mExKUxYvX7HSF38UMMmGbpqNQw3DfYwAw8E6sH7VSVxFipvEEm2afSqTjoRgcLmycXX4zfxCWJ4HY73a9KdgvfHEQGB")

        let key = wallet.getKey(coin: .ethereum, derivationPath: path)
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        XCTAssertEqual(pubkey.data.hexString, "024516c4aa5352035e1bb5be132694e1389a4ac37d32e5e717d35ee4c4dfab5132")

        let pubkey2 = HDWallet.getPublicKeyFromExtended(extended: xpub, coin: .ethereum, derivationPath: path)!
        XCTAssertEqual(pubkey2.data.hexString, "044516c4aa5352035e1bb5be132694e1389a4ac37d32e5e717d35ee4c4dfab513226a9d14ea37a55962ad3644a08e2ce551b4495beabb9b09e688c7b92eba18acc")

        let address = CoinType.ethereum.deriveAddressFromPublicKey(publicKey: pubkey2)
        XCTAssertEqual(address, "0x996891c410FB76C19DBA72C6f6cEFF2d9DD069b1")
    }

    func testSpanishMnemonic() throws {
        let mnemonic = "llanto radical atraer riesgo actuar masa fondo cielo dieta archivo sonrisa mamut"
        XCTAssertNil(HDWallet(mnemonic: mnemonic, passphrase: ""))

        let wallet = HDWallet(mnemonic: mnemonic, passphrase: "", check: false)!
        let btcXpub = wallet.getExtendedPublicKey(purpose: .bip44, coin: .bitcoin, version: .xpub)
        let ethXpub = wallet.getExtendedPublicKey(purpose: .bip44, coin: .ethereum, version: .xpub)

        XCTAssertEqual(btcXpub, "xpub6Cq43Vqyvb2DwXzjzNeMpPuxXRCN1WnmRCmYLPaaSv2XZXM2yCwUHpWEyB3zQ3FGCQsvY21gecMaQR7b2zhhgiHnjzDYpKCE2LACueaSMuR")
        XCTAssertEqual(ethXpub, "xpub6Bgma7boPVudhExmB97iySvatGfnXkfBxYZYNTFYJvVzigUPk1X2iE8VhJPPxVuzjH8wBuTqRBMKCbwMYQNLrFCwYzMugYw4RM5VGNeVDpp")

        let ethAddress = wallet.getAddressForCoin(coin: .ethereum)
        let btcAddress = wallet.getAddressForCoin(coin: .bitcoin)

        XCTAssertEqual(ethAddress, "0xa4531dE99E22B2166d340E7221669DF565c52024")
        XCTAssertEqual(btcAddress, "bc1q97jc0jdgsyvvhxydxxd6np8sa920c39l3qpscf")
    }
    
    func testMessageAndVerifySignerImmutableX() {
        let privateKey = PrivateKey(data: Data(hexString: "3b0a61f46fdae924007146eacb6db6642de7a5603ad843ec58e10331d89d4b84")!)!
        let msg = "Only sign this request if you’ve initiated an action with Immutable X.\n\nFor internal use:\nbd717ba31dca6e0f3f136f7c4197babce5f09a9f25176044c0b3112b1b6017a3"
        let signature = EthereumMessageSigner.signMessageImmutableX(privateKey: privateKey, message: msg)
        XCTAssertEqual(signature, "32cd5a58f3419fc5db672e3d57f76199b853eda0856d491b38f557b629b0a0814ace689412bf354a1af81126d2749207dffae8ae8845160f33948a6b787e17ee01")
        let pubKey = privateKey.getPublicKey(coinType: .ethereum)
        XCTAssertTrue(EthereumMessageSigner.verifyMessage(pubKey: pubKey, message: msg, signature: signature))
    }
    
    func testMessageAndVerifySignerLegacy() {
        let privateKey = PrivateKey(data: Data(hexString: "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d")!)!
        let msg = "Foo"
        let signature = EthereumMessageSigner.signMessage(privateKey: privateKey, message: msg)
        XCTAssertEqual(signature, "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be71101b")
        let pubKey = privateKey.getPublicKey(coinType: .ethereum)
        XCTAssertTrue(EthereumMessageSigner.verifyMessage(pubKey: pubKey, message: msg, signature: signature))
    }
    
    func testMessageAndVerifySignerEip155() {
        let privateKey = PrivateKey(data: Data(hexString: "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d")!)!
        let msg = "Foo"
        let signature = EthereumMessageSigner.signMessageEip155(privateKey: privateKey, message: msg, chainId: 0)
        XCTAssertEqual(signature, "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be711023")
        let pubKey = privateKey.getPublicKey(coinType: .ethereum)
        XCTAssertTrue(EthereumMessageSigner.verifyMessage(pubKey: pubKey, message: msg, signature: signature))
    }
    
    func testMessageAndVerifySigner712Legacy() {
        let privateKey = PrivateKey(data: Data(hexString: "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d")!)!
        let msg = """
         {
                        "types": {
                            "EIP712Domain": [
                                {"name": "name", "type": "string"},
                                {"name": "version", "type": "string"},
                                {"name": "chainId", "type": "uint256"},
                                {"name": "verifyingContract", "type": "address"}
                            ],
                            "Person": [
                                {"name": "name", "type": "string"},
                                {"name": "wallet", "type": "address"}
                            ]
                        },
                        "primaryType": "Person",
                        "domain": {
                            "name": "Ether Person",
                            "version": "1",
                            "chainId": 0,
                            "verifyingContract": "0xCcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC"
                        },
                        "message": {
                            "name": "Cow",
                            "wallet": "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"
                        }
                    }
        """
        let signature = EthereumMessageSigner.signTypedMessage(privateKey: privateKey, messageJson: msg)
        XCTAssertEqual(signature, "446434e4c34d6b7456e5f07a1b994b88bf85c057234c68d1e10c936b1c85706c4e19147c0ac3a983bc2d56ebfd7146f8b62bcea6114900fe8e7d7351f44bf3761c")
        let pubKey = privateKey.getPublicKey(coinType: .ethereum)
        XCTAssertTrue(EthereumMessageSigner.verifyMessage(pubKey: pubKey, message: msg, signature: signature))
    }
    
    func testMessageAndVerifySigner712Eip155() {
        let privateKey = PrivateKey(data: Data(hexString: "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d")!)!
        let msg = """
         {
                        "types": {
                            "EIP712Domain": [
                                {"name": "name", "type": "string"},
                                {"name": "version", "type": "string"},
                                {"name": "chainId", "type": "uint256"},
                                {"name": "verifyingContract", "type": "address"}
                            ],
                            "Person": [
                                {"name": "name", "type": "string"},
                                {"name": "wallet", "type": "address"}
                            ]
                        },
                        "primaryType": "Person",
                        "domain": {
                            "name": "Ether Person",
                            "version": "1",
                            "chainId": 0,
                            "verifyingContract": "0xCcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC"
                        },
                        "message": {
                            "name": "Cow",
                            "wallet": "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"
                        }
                    }
        """
        let signature = EthereumMessageSigner.signTypedMessageEip155(privateKey: privateKey, messageJson: msg, chainId: 0)
        XCTAssertEqual(signature, "446434e4c34d6b7456e5f07a1b994b88bf85c057234c68d1e10c936b1c85706c4e19147c0ac3a983bc2d56ebfd7146f8b62bcea6114900fe8e7d7351f44bf37624")
        let pubKey = privateKey.getPublicKey(coinType: .ethereum)
        XCTAssertTrue(EthereumMessageSigner.verifyMessage(pubKey: pubKey, message: msg, signature: signature))
    }

    // EIP4337

    func testEIP4337DeploymentAddress() {
        let factoryAddress = "0xd9145CCE52D386f254917e481eB44e9943F39138"
        let logicAddress = "0x5C9eb5D6a6C2c1B3EFc52255C0b356f116f6f66D"
        let ownerAddress = "0xA5a1dddEF094095AfB7b6e322dE72961DF2e1988"
        let result = Ethereum.eip4337GetDeploymentAddress(factoryAddress: factoryAddress, logicAddress: logicAddress, ownerAddress: ownerAddress)
        XCTAssertEqual(result, "0xbEaA87cEEaC906C21aaacd258FbFB87CfA3c90a8")
    }

    func testEIP4337NativeTransferAccountNotDeployed() throws {
        let input = EthereumSigningInput.with {
            $0.txMode = .userOp
            $0.chainID = Data(hexString: "05")!
            $0.nonce = Data(hexString: "00")!
            $0.toAddress = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a"

            $0.gasLimit = Data(hexString: "0x5580")!
            $0.maxFeePerGas = Data(hexString: "0x01952f1f85")!
            $0.maxInclusionFeePerGas = Data(hexString: "0x0f")!

            $0.userOperation = EthereumUserOperation.with {
                $0.entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53"
                $0.accountFactory = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92"
                $0.accountLogic = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620"
                $0.owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238"
                $0.isAccountDeployed = false

                $0.preVerificationGas = Data(hexString: "0xbc18")!
                $0.verificationGasLimit = Data(hexString: "0x073272")!
            }

            $0.privateKey = Data(hexString: "0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8")!
            $0.transaction = EthereumTransaction.with {
                $0.transfer = EthereumTransaction.Transfer.with {
                    $0.amount = Data(hexString: "0x2386f26fc10000")!
                }
            }
        }

        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)
        let json = String(data: output.encoded, encoding: .utf8)

        XCTAssertEqual(try input.serializedData().hexString, "0a010512010018022a02558032010f3a0501952f1f85422a3078636536343233353546613535336634303843333461323635304164324634413136333464303333614a20f9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8520b0a090a072386f26fc100005ab9010a2a307831333036623031624333653441443230323631324433383433333837653934373337363733463533122a3078354138373230396237353537383163463635664565456464333835356164653033313766346139321a2a307832316363323764376462346661313938353761333730323635336137613637656533306361363230222a3078373864394333326239364262383732443636443531383138323237353633663434653637453233383202bc183a03073272")
        XCTAssertEqual(json, "{\"callData\":\"0xb61d27f6000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a000000000000000000000000000000000000000000000000002386f26fc1000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"21888\",\"initCode\":\"0x5a87209b755781cf65feeedd3855ade0317f4a925fbfb9cf00000000000000000000000078d9c32b96bb872d66d51818227563f44e67e2380000000000000000000000000000000000000000000000000000000000000000\",\"maxFeePerGas\":\"6797860741\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"0\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"48152\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0x1560b19d17613ec8580cb0feaf7ac2953771404c5bd7830f585e5062e6ddd4b82ae3bb8dbddb659c0300e8009857b5c77501e1cfd5bbab48d03de0ea7207d07c1b\",\"verificationGasLimit\":\"471666\"}");
    }

    func testEIP4337NativeTransferAccountDeployed() throws {
        let input = EthereumSigningInput.with {
            $0.txMode = .userOp
            $0.chainID = Data(hexString: "05")!
            $0.nonce = Data(hexString: "01")!
            $0.toAddress = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a"

            $0.gasLimit = Data(hexString: "0x9d55")!
            $0.maxFeePerGas = Data(hexString: "0x01a339c9e9")!
            $0.maxInclusionFeePerGas = Data(hexString: "0x0f")!

            $0.userOperation = EthereumUserOperation.with {
                $0.entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53"
                $0.accountFactory = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92"
                $0.accountLogic = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620"
                $0.owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238"
                $0.isAccountDeployed = true

                $0.preVerificationGas = Data(hexString: "0xb708")!
                $0.verificationGasLimit = Data(hexString: "0x0186a0")!
            }

            $0.privateKey = Data(hexString: "0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8")!
            $0.transaction = EthereumTransaction.with {
                $0.transfer = EthereumTransaction.Transfer.with {
                    $0.amount = Data(hexString: "0x2386f26fc10000")!
                }
            }
        }

        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)
        let json = String(data: output.encoded, encoding: .utf8)

        XCTAssertEqual(try input.serializedData().hexString, "0a010512010118022a029d5532010f3a0501a339c9e9422a3078636536343233353546613535336634303843333461323635304164324634413136333464303333614a20f9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8520b0a090a072386f26fc100005abb010a2a307831333036623031624333653441443230323631324433383433333837653934373337363733463533122a3078354138373230396237353537383163463635664565456464333835356164653033313766346139321a2a307832316363323764376462346661313938353761333730323635336137613637656533306361363230222a30783738643943333262393642623837324436364435313831383232373536336634346536374532333828013202b7083a030186a0")
        XCTAssertEqual(json, "{\"callData\":\"0xb61d27f6000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a000000000000000000000000000000000000000000000000002386f26fc1000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"40277\",\"initCode\":\"0x\",\"maxFeePerGas\":\"7033440745\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"1\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"46856\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0xaed2011e5cf267de495b38ecf86ad6f1d4c05217a99e59f47e8d52ba3d41c10144785893fa3e7c116a054999e3902fc2771064d0545148bc49f6d7c827fc7a9a1c\",\"verificationGasLimit\":\"100000\"}");
    }

    func testEIP4337ERC20TransferAccountDeployed() throws {
        let input = EthereumSigningInput.with {
            $0.txMode = .userOp
            $0.chainID = Data(hexString: "05")!
            $0.nonce = Data(hexString: "06")!
            $0.toAddress = "0x98339d8c260052b7ad81c28c16c0b98420f2b46a"

            $0.gasLimit = Data(hexString: "0xf78e")!
            $0.maxFeePerGas = Data(hexString: "0x168ad5950f")!
            $0.maxInclusionFeePerGas = Data(hexString: "0x0f")!


            $0.userOperation = EthereumUserOperation.with {
                $0.entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53"
                $0.accountFactory = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92"
                $0.accountLogic = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620"
                $0.owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238"
                $0.isAccountDeployed = true

                $0.preVerificationGas = Data(hexString: "0xbb10")!
                $0.verificationGasLimit = Data(hexString: "0x0186a0")!
            }

            $0.privateKey = Data(hexString: "0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8")!
            $0.transaction = EthereumTransaction.with {
                $0.erc20Transfer = EthereumTransaction.ERC20Transfer.with {
                    $0.to = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a"
                    $0.amount = Data(hexString: "0x0186a0")!
                }
            }
        }

        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)
        let json = String(data: output.encoded, encoding: .utf8)

        XCTAssertEqual(json, "{\"callData\":\"0xb61d27f600000000000000000000000098339d8c260052b7ad81c28c16c0b98420f2b46a000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044a9059cbb000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a00000000000000000000000000000000000000000000000000000000000186a000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"63374\",\"initCode\":\"0x\",\"maxFeePerGas\":\"96818533647\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"6\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"47888\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0xd006c93d6a8753b5e7c1e6349de0dea34eab2e7a533106e0f2e1a3a3b013c8e97b007546dab9d7b8fc471ad14ff2e8aa351dc4f1ecb63bf20f33858dc7366cbe1c\",\"verificationGasLimit\":\"100000\"}");
    }

    func testEIP4337ERC20ApproveAccountDeployed() throws {
        let input = EthereumSigningInput.with {
            $0.txMode = .userOp
            $0.chainID = Data(hexString: "05")!
            $0.nonce = Data(hexString: "09")!
            $0.toAddress = "0x98339d8c260052b7ad81c28c16c0b98420f2b46a"

            $0.gasLimit = Data(hexString: "0xf78e")!
            $0.maxFeePerGas = Data(hexString: "0x168ad5950f")!
            $0.maxInclusionFeePerGas = Data(hexString: "0x0f")!

            $0.userOperation = EthereumUserOperation.with {
                $0.entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53"
                $0.accountFactory = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92"
                $0.accountLogic = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620"
                $0.owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238"
                $0.isAccountDeployed = true

                $0.preVerificationGas = Data(hexString: "0xbb10")!
                $0.verificationGasLimit = Data(hexString: "0x0186a0")!
            }

            $0.privateKey = Data(hexString: "0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8")!
            $0.transaction = EthereumTransaction.with {
                $0.erc20Approve = EthereumTransaction.ERC20Approve.with {
                    $0.spender = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a"
                    $0.amount = Data(hexString: "0x0186a0")!
                }
            }
        }

        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)
        let json = String(data: output.encoded, encoding: .utf8)

        XCTAssertEqual(json, "{\"callData\":\"0xb61d27f600000000000000000000000098339d8c260052b7ad81c28c16c0b98420f2b46a000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044095ea7b3000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a00000000000000000000000000000000000000000000000000000000000186a000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"63374\",\"initCode\":\"0x\",\"maxFeePerGas\":\"96818533647\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"9\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"47888\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0x262a67dd8cf3d16a72b7809b3b5ed55e9f4c2b93eedd5a3c6be035fbbd7111164464ec933d0fdfa359e266e318f3ac22702ae428ce14fc142e4475603e6ec15e1c\",\"verificationGasLimit\":\"100000\"}");
    }

    func testEIP4337ERC721TransferAccountDeployed() throws {
        let input = EthereumSigningInput.with {
            $0.txMode = .userOp
            $0.chainID = Data(hexString: "05")!
            $0.nonce = Data(hexString: "0x0C")!
            $0.toAddress = "0xf5de760f2e916647fd766b4ad9e85ff943ce3a2b"

            $0.gasLimit = Data(hexString: "0x60B378")!
            $0.maxFeePerGas = Data(hexString: "0x168ad5950f")!
            $0.maxInclusionFeePerGas = Data(hexString: "0x0f")!

            $0.userOperation = EthereumUserOperation.with {
                $0.entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53"
                $0.accountFactory = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92"
                $0.accountLogic = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620"
                $0.owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238"
                $0.isAccountDeployed = true

                $0.preVerificationGas = Data(hexString: "0xC34F")!
                $0.verificationGasLimit = Data(hexString: "0x16E360")!
            }

            $0.privateKey = Data(hexString: "0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8")!
            $0.transaction = EthereumTransaction.with {
                $0.erc721Transfer = EthereumTransaction.ERC721Transfer.with {
                    $0.from = "0x8cE23B8769ac01d0df0d5f47Be1A38FeA97F3879"
                    $0.to = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a"
                    $0.tokenID = Data(hexString: "0x2A8E57")!
                }
            }
        }

        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)
        let json = String(data: output.encoded, encoding: .utf8)

        XCTAssertEqual(json, "{\"callData\":\"0xb61d27f6000000000000000000000000f5de760f2e916647fd766b4ad9e85ff943ce3a2b00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000000000000000000006423b872dd0000000000000000000000008ce23b8769ac01d0df0d5f47be1a38fea97f3879000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a00000000000000000000000000000000000000000000000000000000002a8e5700000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"6337400\",\"initCode\":\"0x\",\"maxFeePerGas\":\"96818533647\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"12\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"49999\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0x5951cc161a4d60d6b59503efb93e446f5d1a2e3a41d4503ba6393bcf2a2637340d0a865ed5d4d7650a68cbb95915eaa7ed54fd2c42b4bf7c83376f5c5d70691d1b\",\"verificationGasLimit\":\"1500000\"}");
    }

    func testEIP4337ERC1155TransferAccountDeployed() throws {
        let input = EthereumSigningInput.with {
            $0.txMode = .userOp
            $0.chainID = Data(hexString: "05")!
            $0.nonce = Data(hexString: "0x")!
            $0.toAddress = "0x428ce4b916332e1afccfddce08baecc97cb40b12"

            $0.gasLimit = Data(hexString: "0x60B378")!
            $0.maxFeePerGas = Data(hexString: "0x168ad5950f")!
            $0.maxInclusionFeePerGas = Data(hexString: "0x0f")!

            $0.userOperation = EthereumUserOperation.with {
                $0.entryPoint = "0x1306b01bC3e4AD202612D3843387e94737673F53"
                $0.accountFactory = "0x76627b8D1E01fAF0C73B69625BC1fCb8FA19a2AD"
                $0.accountLogic = "0x510ab68bd111ce7115df797118b0334d727d564b"
                $0.owner = "0x78d9C32b96Bb872D66D51818227563f44e67E238"
                $0.isAccountDeployed = true

                $0.preVerificationGas = Data(hexString: "0xC738")!
                $0.verificationGasLimit = Data(hexString: "0x16E360")!
            }

            $0.privateKey = Data(hexString: "0xf9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8")!
            $0.transaction = EthereumTransaction.with {
                $0.erc1155Transfer = EthereumTransaction.ERC1155Transfer.with {
                    $0.from = "0x8c560E00680b973645900528EDe71a99b8d4dca8"
                    $0.to = "0xce642355Fa553f408C34a2650Ad2F4A1634d033a"
                    $0.tokenID = Data(hexString: "0x01")!
                    $0.value = Data(hexString: "0x")!
                    $0.data = Data(hexString: "0x")!
                }
            }
        }

        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)
        let json = String(data: output.encoded, encoding: .utf8)

        XCTAssertEqual(json, "{\"callData\":\"0xb61d27f6000000000000000000000000428ce4b916332e1afccfddce08baecc97cb40b120000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000c4f242432a0000000000000000000000008c560e00680b973645900528ede71a99b8d4dca8000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a0000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"6337400\",\"initCode\":\"0x\",\"maxFeePerGas\":\"96818533647\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"0\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"51000\",\"sender\":\"0x8c560e00680b973645900528ede71a99b8d4dca8\",\"signature\":\"0xaae38bcf9f946921541b44c2a66596968beecb9420471e2c9c531f758a2d652930ffdeeab95742e57e8520fb5c8ca4fee6a8e47e37336d4201fe104103f85e111c\",\"verificationGasLimit\":\"1500000\"}");
    }
}
