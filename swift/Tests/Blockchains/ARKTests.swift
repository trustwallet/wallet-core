// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
import TrustWalletCore
import XCTest

class ARKTests: XCTestCase {
    
    func testAddress() {
        let data = "this is a top secret passphrase".data(using: .utf8)!
        let hashed = Hash.sha256(data: data)
        let privateKey = PrivateKey(data: hashed)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true);
        let address = ARKAddress(publicKey: publicKey)

        XCTAssertEqual(hashed.hexString, "d8839c2432bfd0a67ef10a804ba991eabba19f154a3d707917681d45822a5712")
        XCTAssertEqual(address.description, "AGeYmgbg2LgGxRW2vNNJvQ88PknEJsYizC")
    }
    
    func testSigner() throws {
        let input = ARKSigningInput.with {
            $0.type = .transfer
            $0.amount = 123123123
            $0.fee = 10000000
            $0.privateKey = Data(hexString: "d8839c2432bfd0a67ef10a804ba991eabba19f154a3d707917681d45822a5712")!
            $0.timestamp = 67447770
            $0.toAddress = "ARkMaRcjcwRgr6vmDtAWo7bFqUgy9wG3NU"
        }
        
        let output = ARKSigner.sign(input: input)

        XCTAssertEqual(output.signature.hexString, "304402205e6365f4c3b49c28f03afd89d308736dca56671ea707dd3dd5af42272a0cc8ed02207fa7fc015fba7ae527d22a058cc4ebd8e9867c563ace7effc2dbaad2af8976c3")
        XCTAssertEqual(output.encoded, "{\"amount\":123123123,\"asset\":{},\"fee\":10000000,\"id\":\"219b1cc99ec804df02230a9e913ccb45edb7819f22328e3cd15030174a8c4167\",\"recipientId\":\"ARkMaRcjcwRgr6vmDtAWo7bFqUgy9wG3NU\",\"senderPublicKey\":\"034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192\",\"signature\":\"304402205e6365f4c3b49c28f03afd89d308736dca56671ea707dd3dd5af42272a0cc8ed02207fa7fc015fba7ae527d22a058cc4ebd8e9867c563ace7effc2dbaad2af8976c3\",\"timestamp\":67447770,\"type\":0}")
    }
    
}
