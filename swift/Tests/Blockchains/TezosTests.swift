// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class TezosTests: XCTestCase {
    public let tezos = CoinType.tezos

    public func testAddressFromString_validAddress() {
        let validAddressString = "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don"
        let address = AnyAddress(string: validAddressString, coin: .tezos)

        XCTAssertNotNil(address)
        XCTAssertEqual(address?.description, validAddressString)
    }

    public func testAddressFromPublicKey() {
        let privateKey = PrivateKey(data: Data(hexString: "b177a72743f54ed4bdf51f1b55527c31bcd68c6d2cb2436d76cadd0227c99ff0")!)!
        let publicKey = privateKey.getPublicKeyEd25519()

        let address = tezos.deriveAddressFromPublicKey(publicKey: publicKey)

        XCTAssertEqual(address.description, "tz1cG2jx3W4bZFeVGBjsTxUAG8tdpTXtE8PT")
    }
    
    public func testSigningFA12() {
        let privateKeyData = Data(hexString: "363265a0b3f06661001cab8b4f3ca8fd97ae70608184979cf7300836f57ec2d6")!
        
        let branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp"
        var operationList = TezosOperationList()
        operationList.branch = branch
        
        let transactionOperationData = TezosTransactionOperationData.with {
            $0.amount = 0
            $0.destination = "KT1EwXFWoG9bYebmF4pYw72aGjwEnBWefgW5"
            $0.parameters.fa12Parameters.entrypoint = "transfer";
            $0.parameters.fa12Parameters.from = "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP";
            $0.parameters.fa12Parameters.to   = "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP";
            $0.parameters.fa12Parameters.value = "123";
        }
        
        let transactionOperation = TezosOperation.with {
            $0.source = "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP"
            $0.fee = 100000
            $0.counter = 2993172
            $0.gasLimit = 100000
            $0.storageLimit = 0
            $0.kind = .transaction
            $0.transactionOperationData = transactionOperationData
        }
        
        operationList.operations = [ transactionOperation ]

        let input = TezosSigningInput.with {
            $0.operationList = operationList
            $0.privateKey = privateKeyData
        }

        let output: TezosSigningOutput = AnySigner.sign(input: input, coin: .tezos)
        let expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016c00fe2ce0cccc0214af521ad60c140c5589b4039247a08d0694d8b601a08d0600000145bd8a65cc48159d8ea60a55df735b7c5ad45f0e00ffff087472616e736665720000005907070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555007070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555000bb012914d768155fba2df319a81136e8e3e573b9cadb1676834490c90212615d271da029b6b0531e290e9063bcdb40bea43627af048b18e036f02be2b6b22fc8b307"

        XCTAssertEqual(output.encoded.hexString, expected)
    }
    
    public func testSigningFA2() {
        let privateKeyData = Data(hexString: "363265a0b3f06661001cab8b4f3ca8fd97ae70608184979cf7300836f57ec2d6")!
        
        let branch = "BKvEAX9HXfJZWYfTQbR1C7B3ADoKY6a1aKVRF7qQqvc9hS8Rr3m"
        var operationList = TezosOperationList()
        operationList.branch = branch
        
        let transferInfos = TezosTxs.with{
            $0.to = "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP"
            $0.tokenID = "0"
            $0.amount = "10"
        }
        
        let transactionOperationData = TezosTransactionOperationData.with {
            $0.amount = 0
            $0.destination = "KT1DYk1XDzHredJq1EyNkDindiWDqZyekXGj"
            $0.parameters.fa2Parameters.entrypoint = "transfer";
            $0.parameters.fa2Parameters.txsObject = [TezosTxObject.with{
                    $0.from = "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP"
                    $0.txs = [transferInfos]
                }]
        }
        
        
        
        let transactionOperation = TezosOperation.with {
            $0.source = "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP"
            $0.fee = 100000
            $0.counter = 2993173
            $0.gasLimit = 100000
            $0.storageLimit = 0
            $0.kind = .transaction
            $0.transactionOperationData = transactionOperationData
        }
        
        operationList.operations = [ transactionOperation ]

        let input = TezosSigningInput.with {
            $0.operationList = operationList
            $0.privateKey = privateKeyData
        }

        let output: TezosSigningOutput = AnySigner.sign(input: input, coin: .tezos)
        let expected = "1b1f9345dc9f77bd24b09034d1d2f9a28f02ac837f49db54b8d68341f53dc4b76c00fe2ce0cccc0214af521ad60c140c5589b4039247a08d0695d8b601a08d0600000136767f88850bae28bfb9f46b73c5e87ede4de12700ffff087472616e7366657200000066020000006107070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b5550020000003107070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555007070000000a552d24710d6c59383286700c6c2917b25a6c1fa8b587e593c289dd47704278796792f1e522c1623845ec991e292b0935445e6994850bd03f035a006c5ed93806"

        XCTAssertEqual(output.encoded.hexString, expected)
    }
    
    public func testMessageSignerSignAndVerify() {
        let privateKey = PrivateKey(data: Data(hexString: "91b4fb8d7348db2e7de2693f58ce1cceb966fa960739adac1d9dba2cbaa0940a")!)!
        let msg = "05010000004254657a6f73205369676e6564204d6573736167653a207465737455726c20323032332d30322d30385431303a33363a31382e3435345a2048656c6c6f20576f726c64"
        let signature = TezosMessageSigner.signMessage(privateKey: privateKey, message: msg)
        XCTAssertEqual(signature, "edsigu3se2fcEJUCm1aqxjzbHdf7Wsugr4mLaA9YM2UVZ9Yy5meGv87VqHN3mmDeRwApTj1JKDaYjqmLZifSFdWCqBoghqaowwJ")
        let pubKey = privateKey.getPublicKey(coinType: .tezos)
        XCTAssertTrue(TezosMessageSigner.verifyMessage(pubKey: pubKey, message: msg, signature: signature))
    }
    
    public func testMessageSignerInputToPayload() {
        let payload = TezosMessageSigner.inputToPayload(message: "Tezos Signed Message: testUrl 2023-02-08T10:36:18.454Z Hello World");
        let expected = "05010000004254657a6f73205369676e6564204d6573736167653a207465737455726c20323032332d30322d30385431303a33363a31382e3435345a2048656c6c6f20576f726c64";
        XCTAssertEqual(payload, expected);
    }
    
    public func testMessageSignerFormatMessage() {
        let formatedMsg = TezosMessageSigner.formatMessage(message: "Hello World", url: "testUrl")
        let regex = try! NSRegularExpression(pattern: "Tezos Signed Message: \\S+ \\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}\\.\\d{3}Z .+")
        XCTAssertTrue(regex.firstMatch(in: formatedMsg, range: NSRange(location: 0, length: formatedMsg.utf16.count)) != nil)
    }

    public func testSigning() {
        let privateKeyData = Data(hexString: "c6377a4cc490dc913fc3f0d9cf67d293a32df4547c46cb7e9e33c3b7b97c64d8")!
        let privateKey = PrivateKey(data: privateKeyData)!
        let publicKey = privateKey.getPublicKeyEd25519()

        let branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp"

        var operationList = TezosOperationList()
        operationList.branch = branch

        var revealOperationData = TezosRevealOperationData()
        revealOperationData.publicKey = publicKey.data

        var transactionOperationData = TezosTransactionOperationData()
        transactionOperationData.amount = 1
        transactionOperationData.destination = "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"

        var revealOperation = TezosOperation()
        revealOperation.source = "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"
        revealOperation.fee = 1272
        revealOperation.counter = 30738
        revealOperation.gasLimit = 10100
        revealOperation.storageLimit = 257
        revealOperation.kind = .reveal
        revealOperation.revealOperationData = revealOperationData

        var transactionOperation = TezosOperation()
        transactionOperation.source = "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"
        transactionOperation.fee = 1272
        transactionOperation.counter = 30739
        transactionOperation.gasLimit = 10100
        transactionOperation.storageLimit = 257
        transactionOperation.kind = .transaction
        transactionOperation.transactionOperationData = transactionOperationData

        operationList.operations = [ revealOperation, transactionOperation ]

        let input = TezosSigningInput.with {
            $0.operationList = operationList
            $0.privateKey = Data(hexString: "2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f")!
        }

        let output: TezosSigningOutput = AnySigner.sign(input: input, coin: .tezos)
        let expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200311f002e899cdd9a52d96cb8be18ea2bbab867c505da2b44ce10906f511cff956c0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80993f001f44e810201000081faa75f741ef614b0e35fcc8c90dfa3b0b95721000217034271b815e5f0c0a881342838ce49d7b48cdf507c72b1568c69a10db70c98774cdad1a74df760763e25f760ff13afcbbf3a1f2c833a0beeb9576a579c05"

        XCTAssertEqual(output.encoded.hexString, expected)
    }

    public func testSignEncodedBytes() throws {

        let key = Data(hexString: "3caf5afaed067890cd850efd1555df351aa482badb4a541c29261f1acf261bf5")!
        let bytes = Data(hexString: "64aa7792af40de41371a72b3342daa7bf3d2b5a84511e9074341fdd52148dd9d6c00ad756cb46ba6f59efa8bd10ff544ba9d20d09542850f96c3a1079d780080ade2040155959998da7e79231e2be8ed8ff373ac1b1574b000ffff04737761700000009e070703060707020000000807070508030b000007070100000018323032332d30322d32345431333a34303a32322e3332385a07070100000024747a31625443473754415535523736356f4458694c4d63385a4537546a7376617868484807070100000024747a315377326d4641557a626b6d37646b47434472626542734a54547456374a4438457907070080dac409070700bdf892a1a291e196aa0503066c00ad756cb46ba6f59efa8bd10ff544ba9d20d09542cd0497c3a107f10f180001543aa1803f0bbe2099809ab067dfa8a4cbc1c26a00ffff07617070726f76650000002d070701000000244b5431516f64676b5974754e79664a726a72673854515a586d64544643616d373268533900006c00ad756cb46ba6f59efa8bd10ff544ba9d20d09542cd0498c3a107f70f090001543aa1803f0bbe2099809ab067dfa8a4cbc1c26a00ffff07617070726f766500000036070701000000244b5431516f64676b5974754e79664a726a72673854515a586d64544643616d373268533900bdf892a1a291e196aa056c00ad756cb46ba6f59efa8bd10ff544ba9d20d09542e71599c3a107fabb01400001b1f0d7affc39861f7f5c75f917f683d2e9f55e3100ffff04737761700000009a070700000707000007070001070700bdf892a1a291e196aa05070700a3f683c2a6d80a07070100000018323032332d30322d32345431333a34303a32322e3332385a070705090100000024747a31625443473754415535523736356f4458694c4d63385a4537546a7376617868484805090100000024747a315377326d4641557a626b6d37646b47434472626542734a54547456374a443845796c00ad756cb46ba6f59efa8bd10ff544ba9d20d09542cd049ac3a107f50f1b000193d22b59c496c94504729be1c671ec1d1d7a9cf000ffff107570646174655f6f70657261746f72730000005f020000005a050507070100000024747a31625443473754415535523736356f4458694c4d63385a4537546a73766178684848070701000000244b543147504a44546638475a73704363616e6147324b684d764775334e4a52717572617400006c00ad756cb46ba6f59efa8bd10ff544ba9d20d0954285109bc3a107a0820100000155959998da7e79231e2be8ed8ff373ac1b1574b000ffff0473776170000000a1070703060707020000000807070508030b000807070100000018323032332d30322d32345431333a34303a32322e3332385a07070100000024747a31625443473754415535523736356f4458694c4d63385a4537546a7376617868484807070100000024747a315377326d4641557a626b6d37646b47434472626542734a54547456374a44384579070700a3f683c2a6d80a070700a4f096bfbe9df6f0e00603066c00ad756cb46ba6f59efa8bd10ff544ba9d20d09542cd049cc3a107ed0f00000193d22b59c496c94504729be1c671ec1d1d7a9cf000ffff107570646174655f6f70657261746f72730000005f020000005a050807070100000024747a31625443473754415535523736356f4458694c4d63385a4537546a73766178684848070701000000244b543147504a44546638475a73704363616e6147324b684d764775334e4a5271757261740000")!
        let input = TezosSigningInput.with {
            $0.privateKey = key
            $0.encodedOperations = bytes
        }

        let output: TezosSigningOutput = AnySigner.sign(input: input, coin: .tezos)

        let expected = "64aa7792af40de41371a72b3342daa7bf3d2b5a84511e9074341fdd52148dd9d6c00ad756cb46ba6f59efa8bd10ff544ba9d20d09542850f96c3a1079d780080ade2040155959998da7e79231e2be8ed8ff373ac1b1574b000ffff04737761700000009e070703060707020000000807070508030b000007070100000018323032332d30322d32345431333a34303a32322e3332385a07070100000024747a31625443473754415535523736356f4458694c4d63385a4537546a7376617868484807070100000024747a315377326d4641557a626b6d37646b47434472626542734a54547456374a4438457907070080dac409070700bdf892a1a291e196aa0503066c00ad756cb46ba6f59efa8bd10ff544ba9d20d09542cd0497c3a107f10f180001543aa1803f0bbe2099809ab067dfa8a4cbc1c26a00ffff07617070726f76650000002d070701000000244b5431516f64676b5974754e79664a726a72673854515a586d64544643616d373268533900006c00ad756cb46ba6f59efa8bd10ff544ba9d20d09542cd0498c3a107f70f090001543aa1803f0bbe2099809ab067dfa8a4cbc1c26a00ffff07617070726f766500000036070701000000244b5431516f64676b5974754e79664a726a72673854515a586d64544643616d373268533900bdf892a1a291e196aa056c00ad756cb46ba6f59efa8bd10ff544ba9d20d09542e71599c3a107fabb01400001b1f0d7affc39861f7f5c75f917f683d2e9f55e3100ffff04737761700000009a070700000707000007070001070700bdf892a1a291e196aa05070700a3f683c2a6d80a07070100000018323032332d30322d32345431333a34303a32322e3332385a070705090100000024747a31625443473754415535523736356f4458694c4d63385a4537546a7376617868484805090100000024747a315377326d4641557a626b6d37646b47434472626542734a54547456374a443845796c00ad756cb46ba6f59efa8bd10ff544ba9d20d09542cd049ac3a107f50f1b000193d22b59c496c94504729be1c671ec1d1d7a9cf000ffff107570646174655f6f70657261746f72730000005f020000005a050507070100000024747a31625443473754415535523736356f4458694c4d63385a4537546a73766178684848070701000000244b543147504a44546638475a73704363616e6147324b684d764775334e4a52717572617400006c00ad756cb46ba6f59efa8bd10ff544ba9d20d0954285109bc3a107a0820100000155959998da7e79231e2be8ed8ff373ac1b1574b000ffff0473776170000000a1070703060707020000000807070508030b000807070100000018323032332d30322d32345431333a34303a32322e3332385a07070100000024747a31625443473754415535523736356f4458694c4d63385a4537546a7376617868484807070100000024747a315377326d4641557a626b6d37646b47434472626542734a54547456374a44384579070700a3f683c2a6d80a070700a4f096bfbe9df6f0e00603066c00ad756cb46ba6f59efa8bd10ff544ba9d20d09542cd049cc3a107ed0f00000193d22b59c496c94504729be1c671ec1d1d7a9cf000ffff107570646174655f6f70657261746f72730000005f020000005a050807070100000024747a31625443473754415535523736356f4458694c4d63385a4537546a73766178684848070701000000244b543147504a44546638475a73704363616e6147324b684d764775334e4a5271757261740000e10077fc3068aaaf1c7779e1dc2c396b3b40d73ddda04648bf4b16ac2e747c89b461771488e80da3aa30fc18c90de99fd358bfb76683f3c3ec250b1ee09b6d07"

        XCTAssertEqual(output.encoded.hexString, expected)

        // How to do it without AnySigner
        var watermark = Data([0x03])
        watermark.append(bytes)

        let hash = Hash.blake2b(data: watermark, size: 32)
        let privateKey = PrivateKey(data: key)!
        let signature = privateKey.sign(digest: hash, curve: .ed25519)!

        var signed = Data()
        signed.append(bytes)
        signed.append(signature)

        XCTAssertEqual(signed.hexString, expected)
    }
}
