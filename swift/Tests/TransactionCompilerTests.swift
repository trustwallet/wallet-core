// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class TransactionCompilerTests: XCTestCase {
    override func setUp() {
        continueAfterFailure = false
    }

    func testBitcoinCompileWithSignatures() throws {
        // Test external signining with a Bitcoin transaction with 3 input UTXOs, all used, but only using 2 public keys.
        // Three signatures are neeeded.  This illustrates that order of UTXOs/hashes is not always the same.

        let revUtxoHash0 = Data(hexString: "07c42b969286be06fae38528c85f0a1ce508d4df837eb5ac4cf5f2a7a9d65fa8")!
        let revUtxoHash1 = Data(hexString: "d6892a5aa54e3b8fe430efd23f49a8950733aaa9d7c915d9989179f48dd1905e")!
        let revUtxoHash2 = Data(hexString: "6021efcf7555f90627364339fc921139dd40a06ccb2cb2a2a4f8f4ea7a2dc74d")!
        let inPubKey0 = Data(hexString: "024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382")!
        let inPubKey1 = Data(hexString: "0217142f69535e4dad0dc7060df645c55a174cc1bfa5b9eb2e59aad2ae96072dfc")!
        let inPubKeyHash0 = Data(hexString: "bd92088bb7e82d611a9b94fbb74a0908152b784f")!
        let inPubKeyHash1 = Data(hexString: "6641abedacf9483b793afe1718689cc9420bbb1c")!

        // Test data: Input UTXO infos
        struct UtxoInfo {
            let revUtxoHash: Data
            let publicKey: Data
            let amount: Int64
            let index: UInt32
        }
        let utxoInfos: [UtxoInfo] = [
            // first
            UtxoInfo(revUtxoHash: revUtxoHash0, publicKey: inPubKey0, amount: 600000, index: 0),
            // second UTXO, with same pubkey
            UtxoInfo(revUtxoHash: revUtxoHash1, publicKey: inPubKey0, amount: 500000, index: 1),
            // third UTXO, with different pubkey
            UtxoInfo(revUtxoHash: revUtxoHash2, publicKey: inPubKey1, amount: 400000, index: 0),
        ]
        
        // Signature infos, indexed by pubkeyhash+hash
        struct SignatureInfo {
            let signature: Data
            let publicKey: Data
        }
        let signatureInfos: [String: SignatureInfo] = [
            inPubKeyHash0.hexString + "+" + "a296bead4172007be69b21971a790e076388666c162a9505698415f1b003ebd7":
            SignatureInfo(signature: Data(hexString: "304402201857bc6e6e48b46046a4bd204136fc77e24c240943fb5a1f0e86387aae59b34902200a7f31478784e51c49f46ef072745a4f263d7efdbc9c6784aa2571ff4f6f2a40")!, publicKey: inPubKey0),
            inPubKeyHash1.hexString + "+" + "505f527f00e15fcc5a2d2416c9970beb57dfdfaca99e572a01f143b24dd8fab6":
            SignatureInfo(signature: Data(hexString: "3044022041294880caa09bb1b653775310fcdd1458da6b8e7d7fae34e37966414fe115820220646397c9d2513edc5974ecc336e9b287de0cdf071c366f3b3dc3ff309213e4e4")!, publicKey: inPubKey1),
            inPubKeyHash0.hexString + "+" + "60ed6e9371e5ddc72fd88e46a12cb2f68516ebd307c0fd31b1b55cf767272101":
            SignatureInfo(signature: Data(hexString: "30440220764e3d5b3971c4b3e70b23fb700a7462a6fe519d9830e863a1f8388c402ad0b102207e777f7972c636961f92375a2774af3b7a2a04190251bbcb31d19c70927952dc")!, publicKey: inPubKey0),
        ]

        let coin = CoinType.bitcoin
        let ownAddress = "bc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z00ppggv"

        // Setup input for Plan
        var signingInput = BitcoinSigningInput.with {
            $0.coinType = coin.rawValue
            $0.hashType = BitcoinSigHashType.all.rawValue
            $0.amount = 1200000
            $0.useMaxAmount = false
            $0.byteFee = 1
            $0.toAddress = "bc1q2dsdlq3343vk29runkgv4yc292hmq53jedfjmp"
            $0.changeAddress = ownAddress
        }
        
        // process UTXOs
        var count: UInt32 = 0        
        for u in utxoInfos {
            let publicKey = PublicKey(data: u.publicKey, type: PublicKeyType.secp256k1)
            let address = SegwitAddress(hrp: .bitcoin, publicKey: publicKey!)
            if (count == 0) { XCTAssertEqual(address.description, ownAddress) }
            if (count == 1) { XCTAssertEqual(address.description, ownAddress) }
            if (count == 2) { XCTAssertEqual(address.description, "bc1qveq6hmdvl9yrk7f6lct3s6yue9pqhwcuxedggg") }

            let utxoScript = BitcoinScript.lockScriptForAddress(address: address.description, coin: coin)
            if (count == 0) { XCTAssertEqual(utxoScript.data.hexString, "0014bd92088bb7e82d611a9b94fbb74a0908152b784f") }
            if (count == 1) { XCTAssertEqual(utxoScript.data.hexString, "0014bd92088bb7e82d611a9b94fbb74a0908152b784f") }
            if (count == 2) { XCTAssertEqual(utxoScript.data.hexString, "00146641abedacf9483b793afe1718689cc9420bbb1c") }
            
            let keyHash: Data = utxoScript.matchPayToWitnessPublicKeyHash()!
            if (count == 0) { XCTAssertEqual(keyHash.hexString, inPubKeyHash0.hexString) }
            if (count == 1) { XCTAssertEqual(keyHash.hexString, inPubKeyHash0.hexString) }
            if (count == 2) { XCTAssertEqual(keyHash.hexString, inPubKeyHash1.hexString) }
            
            let redeemScript = BitcoinScript.buildPayToPublicKeyHash(hash: keyHash)
            if (count == 0) { XCTAssertEqual(redeemScript.data.hexString, "76a914bd92088bb7e82d611a9b94fbb74a0908152b784f88ac") }
            if (count == 1) { XCTAssertEqual(redeemScript.data.hexString, "76a914bd92088bb7e82d611a9b94fbb74a0908152b784f88ac") }
            if (count == 2) { XCTAssertEqual(redeemScript.data.hexString, "76a9146641abedacf9483b793afe1718689cc9420bbb1c88ac") }
            signingInput.scripts[keyHash.hexString] = redeemScript.data
            
            let outPoint = BitcoinOutPoint.with {
                $0.hash = u.revUtxoHash
                $0.index = u.index
                $0.sequence = UInt32.max
            }
            let utxo = BitcoinUnspentTransaction.with {
                $0.script = utxoScript.data
                $0.amount = u.amount
                $0.outPoint = outPoint
            }
            signingInput.utxo.append(utxo)
            
            count += 1
        }
        XCTAssertEqual(count, 3)
        XCTAssertEqual(signingInput.utxo.count, 3)

        // Plan
        let plan: BitcoinTransactionPlan = AnySigner.plan(input: signingInput, coin: coin)

        // At this point plan can be checked, assume it is accepted unmodified
        XCTAssertEqual(plan.amount, 1200000)
        XCTAssertEqual(plan.fee, 277)
        XCTAssertEqual(plan.change, 299723)
        XCTAssertEqual(plan.utxos.count, 3)
        // Note that UTXOs happen to be in reverse order compared to the input
        XCTAssertEqual(plan.utxos[0].outPoint.hash.hexString, revUtxoHash2.hexString)
        XCTAssertEqual(plan.utxos[1].outPoint.hash.hexString, revUtxoHash1.hexString)
        XCTAssertEqual(plan.utxos[2].outPoint.hash.hexString, revUtxoHash0.hexString)

        // Extend input with accepted plan
        signingInput.plan = plan

        // Serialize input
        let txInputData = try signingInput.serializedData()
        XCTAssertEqual(txInputData.count, 692)

        /// Step 2: Obtain preimage hashes
        let preImageHashes = TransactionCompiler.preImageHashes(coinType: coin, txInputData: txInputData)
        let preSigningOutput: BitcoinPreSigningOutput = try BitcoinPreSigningOutput(serializedData: preImageHashes)

        XCTAssertEqual(preSigningOutput.error, CommonSigningError.ok)
        XCTAssertEqual(preSigningOutput.hashPublicKeys[0].dataHash.hexString, "505f527f00e15fcc5a2d2416c9970beb57dfdfaca99e572a01f143b24dd8fab6")
        XCTAssertEqual(preSigningOutput.hashPublicKeys[1].dataHash.hexString, "a296bead4172007be69b21971a790e076388666c162a9505698415f1b003ebd7")
        XCTAssertEqual(preSigningOutput.hashPublicKeys[2].dataHash.hexString, "60ed6e9371e5ddc72fd88e46a12cb2f68516ebd307c0fd31b1b55cf767272101")
        XCTAssertEqual(preSigningOutput.hashPublicKeys[0].publicKeyHash.hexString, inPubKeyHash1.hexString)
        XCTAssertEqual(preSigningOutput.hashPublicKeys[1].publicKeyHash.hexString, inPubKeyHash0.hexString)
        XCTAssertEqual(preSigningOutput.hashPublicKeys[2].publicKeyHash.hexString, inPubKeyHash0.hexString)

        // Simulate signatures, normally they are obtained from external source, e.g. a signature server.
        let signatureVec = DataVector()
        let pubkeyVec = DataVector()
        for h in preSigningOutput.hashPublicKeys {
            let preImageHash = h.dataHash
            let pubkeyHash = h.publicKeyHash
            
            let key: String = pubkeyHash.hexString + "+" + preImageHash.hexString
            XCTAssertTrue(signatureInfos.contains { $0.key == key })
            let sigInfo: SignatureInfo = signatureInfos[key]!
            let publicKeyData = sigInfo.publicKey
            let publicKey = PublicKey(data: publicKeyData, type: PublicKeyType.secp256k1)!
            let signature = sigInfo.signature
            
            signatureVec.add(data: signature)
            pubkeyVec.add(data: publicKeyData)

            // Verify signature (pubkey & hash & signature)
            XCTAssertTrue(publicKey.verifyAsDER(signature: signature, message: preImageHash))
        }
        
        /// Step 3: Compile transaction info
        let compileWithSignatures = TransactionCompiler.compileWithSignatures(coinType: coin, txInputData: txInputData, signatures: signatureVec, publicKeys: pubkeyVec)
        
        let ExpectedTx = "010000000001036021efcf7555f90627364339fc921139dd40a06ccb2cb2a2a4f8f4ea7a2dc74d0000000000ffffffffd6892a5aa54e3b8fe430efd23f49a8950733aaa9d7c915d9989179f48dd1905e0100000000ffffffff07c42b969286be06fae38528c85f0a1ce508d4df837eb5ac4cf5f2a7a9d65fa80000000000ffffffff02804f1200000000001600145360df8231ac5965147c9d90ca930a2aafb05232cb92040000000000160014bd92088bb7e82d611a9b94fbb74a0908152b784f02473044022041294880caa09bb1b653775310fcdd1458da6b8e7d7fae34e37966414fe115820220646397c9d2513edc5974ecc336e9b287de0cdf071c366f3b3dc3ff309213e4e401210217142f69535e4dad0dc7060df645c55a174cc1bfa5b9eb2e59aad2ae96072dfc0247304402201857bc6e6e48b46046a4bd204136fc77e24c240943fb5a1f0e86387aae59b34902200a7f31478784e51c49f46ef072745a4f263d7efdbc9c6784aa2571ff4f6f2a400121024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382024730440220764e3d5b3971c4b3e70b23fb700a7462a6fe519d9830e863a1f8388c402ad0b102207e777f7972c636961f92375a2774af3b7a2a04190251bbcb31d19c70927952dc0121024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb49338200000000"
        
        XCTAssertEqual(compileWithSignatures.count, 786)
        let output: BitcoinSigningOutput = try BitcoinSigningOutput(serializedData: compileWithSignatures)
        XCTAssertEqual(output.encoded.count, 518)
        XCTAssertEqual(output.encoded.hexString, ExpectedTx)
    }
    
    // Test if `compileWithSignaturesAndPubKeyType` binding is available in Swift.
    func testBitcoinCompileWithSignaturesAndPubKeyType() {
        let txInputData = Data()
        let signatureVec = DataVector()
        let pubkeyVec = DataVector()

        let _ = TransactionCompiler.compileWithSignaturesAndPubKeyType(coinType: .bitcoin, txInputData: txInputData, signatures: signatureVec, publicKeys: pubkeyVec, pubKeyType: .secp256k1)
    }
}
