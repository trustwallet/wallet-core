// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class BitsharesTests: XCTestCase {
    override func setUp() {
        continueAfterFailure = false
    }

    func testAddresses() {
        XCTAssertTrue(CoinType.bitshares.validate(address: "BTS52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa"))
        for addr in [
            "52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa",
            "BTS52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRd",
            "BTS52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaaaa",
            "BITS52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa",
            ] {
            XCTAssertFalse(CoinType.steem.validate(address: addr))
        }
    }
    
    
    let signingInput: BitsharesSigningInput = {
        let asset = BitsharesAsset.with {
            $0.amount = 511;
            $0.assetID = 0;
        }
        
        let fee = BitsharesAsset.with {
            $0.amount = 2;
            $0.assetID = 0;
        }

        return BitsharesSigningInput.with {
            $0.chainID = Data(repeating: 0, count: 32);
            $0.senderID = 12;
            $0.recipientID = 16;
            $0.asset = asset;
            $0.fee = fee;
            $0.referenceBlockID = Data(hexString: "0000086bf9e7704509aa41311a66fa0a1b479c6b")!;
            $0.referenceBlockTime = 1552464180;
            $0.privateKey = Hash.sha256(data: "A".data(using: .utf8)!)
        }
        
    }()

    // ensure valid input is signed
    func testSigning() throws {
        let result = BitsharesSigner.sign(input: signingInput);
        XCTAssertTrue(result.success, "Error signing: \(result.error)")
        XCTAssertEqual(result.objects.count, 1)

        let signingOutput = try BitsharesSigningOutput(unpackingAny: result.objects[0])
        
        let signedJSON = signingOutput.jsonEncoded
        let data = signedJSON.data(using: .utf8)
        XCTAssertNotNil(data)
        
        let jsonArray = try JSONSerialization.jsonObject(with: data!, options : .allowFragments) as? [String: Any]
        XCTAssertNotNil(jsonArray, "Error parsing JSON result")
        let signature = (jsonArray!["signatures"] as? [Any])?.first as? String
        XCTAssertNotNil(signature, "Error parsing JSON result")
        XCTAssertEqual(signature!, "1f0ee91e5f9cdd04629d4db71a6d5f0d75c282669bbaf84c184b6c18f04fe75dfb560969a9d8360f31bdd93b90c40dfe5fed601094433962205b4c49e925b51b24")
    }
    
    // ensure invalid inputs are not signed
    func testSigningFailures() throws {
        var badInput = signingInput
        badInput.asset.amount = 0
        var result = BitsharesSigner.sign(input: badInput);
        XCTAssertFalse(result.success, "Expected error but signing suceeded!")
        
        badInput = signingInput
        badInput.fee.amount = -1;
        result = BitsharesSigner.sign(input: badInput);
        XCTAssertFalse(result.success, "Expected error but signing suceeded!")

        badInput = signingInput
        badInput.senderID = badInput.recipientID
        result = BitsharesSigner.sign(input: badInput);
        XCTAssertFalse(result.success, "Expected error but signing suceeded!")

        badInput = signingInput
        badInput.referenceBlockID = Data(hexString: "0000086bf9e7704509aa41311a66fa0a1b479c")!;
        result = BitsharesSigner.sign(input: badInput);
        XCTAssertFalse(result.success, "Expected error but signing suceeded!")
    }
    
    func testMemos() throws {
        let message = "Hello, world!"
        var inputWithMemo = signingInput
        let senderPublicKey = PrivateKey(data: Hash.sha256(data: "A".data(using: .utf8)!))!.getPublicKeySecp256k1(compressed: true)
        let recipientPublicKey = PrivateKey(data: Hash.sha256(data: "B".data(using: .utf8)!))!.getPublicKeySecp256k1(compressed: true)
        inputWithMemo.memo = message
        inputWithMemo.recipientPublicKey = recipientPublicKey.data
        
        let result = BitsharesSigner.sign(input: inputWithMemo);
        XCTAssertTrue(result.success, "Error signing: \(result.error)")
        XCTAssertEqual(result.objects.count, 1)
        
        let signingOutput = try BitsharesSigningOutput(unpackingAny: result.objects[0])
        
        let signedJSON = signingOutput.jsonEncoded
        let data = signedJSON.data(using: .utf8)
        XCTAssertNotNil(data)
        
        let jsonArray = try JSONSerialization.jsonObject(with: data!, options : .allowFragments) as? [String: Any]
        XCTAssertNotNil(jsonArray, "Error parsing JSON result")
        let memoJSON = (((jsonArray!["operations"] as? [Any])?.first as? [Any])?[1] as? [String: Any])?["memo"] as? [String: Any]
        XCTAssertNotNil(memoJSON, "Error parsing JSON result")
        
        XCTAssertEqual(memoJSON!["to"] as! String, BitsharesAddress(publicKey: recipientPublicKey).description)
        XCTAssertEqual(memoJSON!["from"] as! String, BitsharesAddress(publicKey: senderPublicKey).description)
        
        guard let encryptedHex = memoJSON!["message"] as? String, let encryptedData = Data(hexString: encryptedHex), let nonce = memoJSON!["nonce"] as? UInt64 else {
            XCTFail("Failed to extract encrypted message")
            return
        }
        
        let sharedSecretHex = "05c160e88379eabeaac0ac98afd22486a1e1ec7982cebad6073d68647f8cdea8e61cb714f314a2ad2d9d6eb74cde6f1e7150823b361032c8b598dcd68957f474"
        let keyPlusIV = Hash.sha512(data: ("\(nonce)" + sharedSecretHex).data(using: .utf8)!)
        let decrypted = AES.cbcdecrypt(key: keyPlusIV[0..<32], data: encryptedData, iv: keyPlusIV[32..<48])
        XCTAssertNotNil(decrypted)
        let messageData = message.data(using: .utf8)!
        
        // check decrypted bytes against the original message's bytes
        // skip the checksum bytes (first 4) and the padding bytes
        XCTAssertEqual(decrypted![4..<messageData.count + 4], messageData)
    }
}
