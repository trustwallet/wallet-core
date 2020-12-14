// Copyright © 2017-2020 Trust Wallet.
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

    func testSigner() {
        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.nonce = Data(hexString: "09")!
            $0.gasPrice = Data(hexString: "04a817c800")!
            $0.gasLimit = Data(hexString: "5208")!
            $0.toAddress = "0x3535353535353535353535353535353535353535"
            $0.privateKey = Data(hexString: "0x4646464646464646464646464646464646464646464646464646464646464646")!
            $0.transaction = EthereumTransaction.with {
                $0.transfer = TW_Ethereum_Proto_Transaction.Transfer.with {
                    $0.amount = Data(hexString: "0de0b6b3a7640000")!
                }
            }
        }

        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)
        let encoded = AnySigner.encode(input: input, coin: .ethereum)

        XCTAssertEqual(encoded, output.encoded)
        XCTAssertEqual(output.v.hexString, "25")
        XCTAssertEqual(output.r.hexString, "28ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276")
        XCTAssertEqual(output.s.hexString, "67cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83")
        XCTAssertEqual(output.encoded.hexString, "f86c098504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a028ef61340bd939bc2195fe537567866003e1a15d3c71ff63e1590620aa636276a067cbe9d8997f761aecb703304b3800ccf555c9f3dc64214b297fb1966a3b6d83")
    }

    func testSignERC20Transfer() {
        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.nonce = Data(hexString: "00")!
            $0.gasPrice = Data(hexString: "09c7652400")! // 42000000000
            $0.gasLimit = Data(hexString: "0130B9")! // 78009
            $0.toAddress = "0x6b175474e89094c44da98b954eedeac495271d0f" // DAI
            $0.privateKey = Data(hexString: "0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151")!
            $0.transaction = EthereumTransaction.with {
                $0.erc20Transfer = TW_Ethereum_Proto_Transaction.ERC20Transfer.with {
                    $0.to = "0x5322b34c88ed0691971bf52a7047448f0f4efc84"
                    $0.amount = Data(hexString: "1bc16d674ec80000")! // 2000000000000000000
                }
            }
        }
        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)
        let encoded = AnySigner.encode(input: input, coin: .ethereum)

        XCTAssertEqual(encoded, output.encoded)
        XCTAssertEqual(output.v.hexString, "25")
        XCTAssertEqual(output.r.hexString, "724c62ad4fbf47346b02de06e603e013f26f26b56fdc0be7ba3d6273401d98ce")
        XCTAssertEqual(output.s.hexString, "032131cae15da7ddcda66963e8bef51ca0d9962bfef0547d3f02597a4a58c931")
        XCTAssertEqual(output.encoded.hexString, "f8aa808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec8000025a0724c62ad4fbf47346b02de06e603e013f26f26b56fdc0be7ba3d6273401d98cea0032131cae15da7ddcda66963e8bef51ca0d9962bfef0547d3f02597a4a58c931")
    }

    func testSignERC20Approve() {
        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.nonce = Data(hexString: "00")!
            $0.gasPrice = Data(hexString: "09c7652400")! // 42000000000
            $0.gasLimit = Data(hexString: "0130B9")! // 78009
            $0.toAddress = "0x6b175474e89094c44da98b954eedeac495271d0f" // DAI
            $0.privateKey = Data(hexString: "0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151")!
            $0.transaction = EthereumTransaction.with {
                $0.erc20Approve = TW_Ethereum_Proto_Transaction.ERC20Approve.with {
                    $0.spender = "0x5322b34c88ed0691971bf52a7047448f0f4efc84"
                    $0.amount = Data(hexString: "1bc16d674ec80000")! // 2000000000000000000
                }
            }
        }
        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)
        let encoded = AnySigner.encode(input: input, coin: .ethereum)

        XCTAssertEqual(encoded, output.encoded)
        XCTAssertEqual(output.encoded.hexString, "f8aa808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844095ea7b30000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc84000000000000000000000000000000000000000000000000000000000000000026a0406bfe337306699d01f447c23a3bd927c7174ace5c1d9a39b545b49fc6fce005a049f837dd141b8fdf45dc10b415eaf974c66384c87056d8f58882896ee6fd55cb")
    }

    func testSignERC721Transfer() {
        let input = EthereumSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.nonce = Data(hexString: "00")!
            $0.gasPrice = Data(hexString: "09c7652400")! // 42000000000
            $0.gasLimit = Data(hexString: "0130B9")! // 78009
            $0.toAddress = "0x4e45e92ed38f885d39a733c14f1817217a89d425" // payload
            $0.privateKey = Data(hexString: "0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151")!
            $0.transaction = EthereumTransaction.with {
                $0.erc721Transfer = TW_Ethereum_Proto_Transaction.ERC721Transfer.with {
                    $0.from = "0x718046867b5b1782379a14eA4fc0c9b724DA94Fc"
                    $0.to = "0x5322b34c88ed0691971bf52a7047448f0f4efc84"
                    $0.tokenID = Data(hexString: "23c47ee5")!
                }
            }
        }
        let output: EthereumSigningOutput = AnySigner.sign(input: input, coin: .ethereum)
        let encoded = AnySigner.encode(input: input, coin: .ethereum)

        XCTAssertEqual(encoded, output.encoded)
        XCTAssertEqual(output.v.hexString, "26")
        XCTAssertEqual(output.r.hexString, "4f35575c8dc6d0c12fd1ae0007a1395f2baa992d5d498f5ee381cdb7d46ed43c")
        XCTAssertEqual(output.s.hexString, "0935b9ceb724ab73806e7f43da6a3079e7404e2dc28fe030fef96cd13779ac04")
        XCTAssertEqual(output.encoded.hexString, "f8b6808509c7652400830130b98080b86423b872dd000000000000000000000000718046867b5b1782379a14ea4fc0c9b724da94fc0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000000000023c47ee526a04f35575c8dc6d0c12fd1ae0007a1395f2baa992d5d498f5ee381cdb7d46ed43ca00935b9ceb724ab73806e7f43da6a3079e7404e2dc28fe030fef96cd13779ac04")
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

    func testDecode() {
        let rawTx = Data(hexString: "0xf8a86484b2d05e008277fb9400000000000c2e074ec69a0dfb2997ba6c7d2e1e80b8441896f70ae71cd96d4ba1c4b512b0c5bee30d2b6becf61e574c32a17a67156fa9ed3c4c6f0000000000000000000000004976fb03c32e5b8cfe2b6ccb31c09ba78ebaba4125a0b55e479d5872b7531437621780ead128cd25d8988fb3cda9bcfb4baeb0eda4dfa077b096cf0cb4bee6eb8c756e9cdba95a6cf62af74e05e7e4cdaa8100271a508d")!
        let decoded = AnySigner.decode(data: rawTx, coin: .ethereum)

        let expected = """
        {
          "gas": "0x77fb",
          "gasPrice": "0xb2d05e00",
          "input": "0x1896f70ae71cd96d4ba1c4b512b0c5bee30d2b6becf61e574c32a17a67156fa9ed3c4c6f0000000000000000000000004976fb03c32e5b8cfe2b6ccb31c09ba78ebaba41",
          "nonce": "0x64",
          "to": "0x00000000000c2e074ec69a0dfb2997ba6c7d2e1e",
          "value": "0x",
          "v": "0x25",
          "r": "0xb55e479d5872b7531437621780ead128cd25d8988fb3cda9bcfb4baeb0eda4df",
          "s": "0x77b096cf0cb4bee6eb8c756e9cdba95a6cf62af74e05e7e4cdaa8100271a508d"
        }
        """
        XCTAssertJSONEqual(String(data: decoded, encoding: .utf8)!, expected)
    }
}
