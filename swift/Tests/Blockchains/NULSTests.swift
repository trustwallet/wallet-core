// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class NULSTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "a1269039e4ffdf43687852d7247a295f0b5bc55e6dda031cffaa3295ca0a9d7a")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = NULSAddress(publicKey: pubkey)
        let addressFromString = NULSAddress(string: "NULSd6HghWa4CN5qdxqMwYVikQxRZyj57Jn4L")!

        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        let input = NULSSigningInput.with {
            $0.private_key = "9ce21dad67e0f0af2599b41b515a7f7018059418bab892a7b68f283d489abc4b"
            $0.from_address = "NULSd6Hgj7ZoVgsPN9ybB4C1N2TbvkgLc8Z9H"
            $0.to_address = "NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe"
            $0.amount = Data(hexString: "10000000")!
            $0.chain_id = 1
            $0.idassets_id = 1
            $0.nonce = "0000000000000000"
            $0.remark = ""        
            $0.balance = Data(hexString: "100000000")!    
            $0.timestamp = 0x5d8885f8
        }

        let signer = NULSSigner(input);
        let output = signer.sign()

        XCTAssertEqual(output.encoded.hexString, 
            "0200f885885d00008c0117010001f7ec6473df12e751d64cf20a8baa7edd50810f8101000100201d9a"
            "0000000000000000000000000000000000000000000000000000000000080000000000000000000117"
            "010001f05e7878971f3374515eabb6f16d75219d887312010001008096980000000000000000000000"
            "0000000000000000000000000000000000000000000000000000692103958b790c331954ed367d37ba"
            "c901de5c2f06ac8368b37d7bd6cd5ae143c1d7e3463044022028019c0099e2233c7adb84bb03a9a566"
            "6ece4a5b65a026a090fa460f3679654702204df0fcb8762b5944b3aba033fa1a287ccb098150035dd8"
            "b66f52dc58d3d0843a")
    }
}
