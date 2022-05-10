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
        }

        let output: NULSSigningOutput = AnySigner.sign(input: input, coin: .nuls)

        XCTAssertEqual(output.encoded.hexString, "0200f885885d0000d202170100012c177a01a7afbe98e094007b99476534fb7926b709000100809698000000000000000000000000000000000000000000000000000000000008000000000000000000170100012c177a01a7afbe98e094007b99476534fb7926b701000100a086010000000000000000000000000000000000000000000000000000000000080000000000000000000117010001f05e7878971f3374515eabb6f16d75219d88731209000100809698000000000000000000000000000000000000000000000000000000000000000000000000006921033c87a3d9b812556b3034b6471cad5131a01e210c1d7ca06dd53b7d0aff0ee04546304402205ddea604c6cdfcf6cbe32f5873937641676ee5f9aee3c40aa9857c59aefedff202205b77429cf62307d43a6a79b4c106123e6232e3981032573770fe2726bf9fc07c")
    }
}
