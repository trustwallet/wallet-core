// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class NULSTests: XCTestCase {

    func testAddress() {
        let privateKey = PrivateKey(data: Data(hexString: "0xa1269039e4ffdf43687852d7247a295f0b5bc55e6dda031cffaa3295ca0a9d7a")!)!
        let pubkey = privateKey.getPublicKeySecp256k1(compressed: true)
        let address = AnyAddress(publicKey: pubkey, coin: .nuls)
        let addressFromString = AnyAddress(string: "NULSd6HghWa4CN5qdxqMwYVikQxRZyj57Jn4L", coin: .nuls)!

        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        let input = NULSSigningInput.with {
            $0.privateKey = Data(hexString: "0x9ce21dad67e0f0af2599b41b515a7f7018059418bab892a7b68f283d489abc4b")!
            $0.from = "NULSd6Hgj7ZoVgsPN9ybB4C1N2TbvkgLc8Z9H"
            $0.to = "NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe"
            $0.amount = Data(hexString: "0x989680")!
            $0.chainID = 1
            $0.idassetsID = 1
            $0.nonce = "0000000000000000".data(using: .utf8)!
            $0.remark = ""
            $0.balance = Data(hexString: "0x05f5e100")!
            $0.timestamp = 1569228280
        }

        let output: NULSSigningOutput = AnySigner.sign(input: input, coin: .nuls)

        XCTAssertEqual(output.encoded.hexString, "0200f885885d00008c0117010001f7ec6473df12e751d64cf20a8baa7edd50810f8101000100201d9a0000000000000000000000000000000000000000000000000000000000080000000000000000000117010001f05e7878971f3374515eabb6f16d75219d8873120100010080969800000000000000000000000000000000000000000000000000000000000000000000000000692103958b790c331954ed367d37bac901de5c2f06ac8368b37d7bd6cd5ae143c1d7e3463044022028019c0099e2233c7adb84bb03a9a5666ece4a5b65a026a090fa460f3679654702204df0fcb8762b5944b3aba033fa1a287ccb098150035dd8b66f52dc58d3d0843a")
    }

    func testTokenSign() {
        let input = NULSSigningInput.with {
            $0.privateKey = Data(hexString: "0x9ce21dad67e0f0af2599b41b515a7f7018059418bab892a7b68f283d489abc4b")!
            $0.from = "NULSd6Hgj7ZoVgsPN9ybB4C1N2TbvkgLc8Z9H"
            $0.to = "NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe"
            $0.amount = Data(hexString: "0x989680")!
            $0.chainID = 9
            $0.idassetsID = 1
            $0.nonce = "0000000000000000".data(using: .utf8)!
            $0.remark = ""
            $0.balance = Data(hexString: "0x05f5e100")!
            $0.timestamp = 1569228280
            $0.feePayer = "NULSd6Hgj7ZoVgsPN9ybB4C1N2TbvkgLc8Z9H"
            $0.feePayerBalance = Data(hexString: "0x0f4240")!
            $0.feePayerNonce = "0000000000000000".data(using: .utf8)!

        }

        let output: NULSSigningOutput = AnySigner.sign(input: input, coin: .nuls)

        XCTAssertEqual(output.encoded.hexString, "0200f885885d0000d20217010001f7ec6473df12e751d64cf20a8baa7edd50810f810900010080969800000000000000000000000000000000000000000000000000000000000800000000000000000017010001f7ec6473df12e751d64cf20a8baa7edd50810f8101000100a086010000000000000000000000000000000000000000000000000000000000080000000000000000000117010001f05e7878971f3374515eabb6f16d75219d8873120900010080969800000000000000000000000000000000000000000000000000000000000000000000000000692103958b790c331954ed367d37bac901de5c2f06ac8368b37d7bd6cd5ae143c1d7e346304402204629e7e39708468a405f8d8dd208d0133a686beb5d3ae829b7a2f5867c74480902203d0dffac8189b1caa9087e480fd57581e8c382cc4e17034b492dd2178dac851d")
    }

    func testSignWithFeePayer() {
        let input = NULSSigningInput.with {
            $0.privateKey = Data(hexString: "0x48c91cd24a27a1cdc791022ff39316444229db1c466b3b1841b40c919dee3002")!
            $0.from = "NULSd6HgYx7bdWWv7PxYhYeTRBhD6kZs1o5Ac"
            $0.to = "NULSd6HgaHjzhMEUjd4T5DFnLz9EvV4TntrdV"
            $0.amount = Data(hexString: "0x0186a0")!
            $0.chainID = 1
            $0.idassetsID = 1
            $0.nonce = "0000000000000000".data(using: .utf8)!
            $0.remark = ""
            $0.balance = Data(hexString: "0x0f4240")!
            $0.timestamp = 1660632991
            $0.feePayer = "NULSd6HgYj81NrQBFZYXvyQhHCJCkGYWDTNeA"
            $0.feePayerNonce = "0000000000000000".data(using: .utf8)!
            $0.feePayerBalance = Data(hexString: "0x0f4240")!
            $0.feePayerPrivateKey = Data(hexString: "0x9401fd554cb700777e57b05338f9ff47597add8b23ce9f1c8e041e9b4e2116b6")!
        }

        let output: NULSSigningOutput = AnySigner.sign(input: input, coin: .nuls)

        XCTAssertEqual(output.encoded.hexString, "02009f3ffb620000d202170100014f019a4227bff89d51590fbf53fbd98d994485f801000100a086010000000000000000000000000000000000000000000000000000000000080000000000000000001701000152a6388c8bf54e8fcd73cc824813bfef0912299b01000100a086010000000000000000000000000000000000000000000000000000000000080000000000000000000117010001686a3c9cd2ac45aee0ef825b0443d1eb209368b701000100a0860100000000000000000000000000000000000000000000000000000000000000000000000000d22102764370693450d654d6fc061d1d4dbfbe0c95715fd3cde7e15df073ab0983b8c8463044022040b5820b93fd5e272f2a00518af45a722571834934ba20d9a866de8e6d6409ab022003c610c647648444c1e2193634b2c51817a5a6ac3fe781da1a9ea773506afd8221025910df09ca768909cce9224d182401044c7b5bd44b0adee2ec5f2e64446573ff4630440220140e46b260942a8475f38df39bf54a2eea56c77199fc7ba775aa4b7f147d0d2102206c82705cba509f37ba0e35520a97bccb71a9e35cadcb8d95dd7fde5c8aa9e428")
    }

    func testTokenSignWithFeePayer() {
        let input = NULSSigningInput.with {
            $0.privateKey = Data(hexString: "0x48c91cd24a27a1cdc791022ff39316444229db1c466b3b1841b40c919dee3002")!
            $0.from = "NULSd6HgYx7bdWWv7PxYhYeTRBhD6kZs1o5Ac"
            $0.to = "NULSd6HgaHjzhMEUjd4T5DFnLz9EvV4TntrdV"
            $0.amount = Data(hexString: "0x0186a0")!
            $0.chainID = 9
            $0.idassetsID = 1
            $0.nonce = "0000000000000000".data(using: .utf8)!
            $0.remark = ""
            $0.balance = Data(hexString: "0x0dbba0")!
            $0.timestamp = 1660636748
            $0.feePayer = "NULSd6HgYj81NrQBFZYXvyQhHCJCkGYWDTNeA"
            $0.feePayerNonce = "e05d03df6ede0e22".data(using: .utf8)!
            $0.feePayerBalance = Data(hexString: "0x0f4240")!
            $0.feePayerPrivateKey = Data(hexString: "0x9401fd554cb700777e57b05338f9ff47597add8b23ce9f1c8e041e9b4e2116b6")!
        }

        let output: NULSSigningOutput = AnySigner.sign(input: input, coin: .nuls)

        XCTAssertEqual(output.encoded.hexString, "02004c4efb620000d2021701000152a6388c8bf54e8fcd73cc824813bfef0912299b09000100a08601000000000000000000000000000000000000000000000000000000000008000000000000000000170100014f019a4227bff89d51590fbf53fbd98d994485f801000100a08601000000000000000000000000000000000000000000000000000000000008e05d03df6ede0e22000117010001686a3c9cd2ac45aee0ef825b0443d1eb209368b709000100a0860100000000000000000000000000000000000000000000000000000000000000000000000000d32102764370693450d654d6fc061d1d4dbfbe0c95715fd3cde7e15df073ab0983b8c8463044022025cb5b40bda4e6fc0ba719bb0c1a907b2a0faa91316ef2c836310d49f906b6a802207d530a56a6c56d974036c86125da06d6e47f9d8ba1544ac3e620cebd883a707821025910df09ca768909cce9224d182401044c7b5bd44b0adee2ec5f2e64446573ff473045022100ff6f45a1c3856f9ea954baca6b2988295bbb22c958f87f0d3baf9989930549530220426ecb1520513710b99ab50e1f6c7e21b0175adef08aa05070bb9bfca8a001d8")
    }
}
