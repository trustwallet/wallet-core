// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class StellarTests: XCTestCase {

    func testAddressFromPrivateKey() {
        let key = PrivateKey(data: Data(hexString: "59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .stellar)

        XCTAssertEqual(pubkey.data.hexString, "09A966BCAACC103E38896BAAE3F8C2F06C21FD47DD4F864FF0D33F9819DF5CA2".lowercased())
        XCTAssertEqual(address.description, "GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI")
    }

    func testAddressFromPublicKey() {
        let pubkey = PublicKey(data: Data(hexString: "0103E20EC6B4A39A629815AE02C0A1393B9225E3B890CAE45B59F42FA29BE9668D")!, type: .ed25519)!
        let address = AnyAddress(publicKey: pubkey, coin: .stellar)

        XCTAssertEqual(address.description, "GAB6EDWGWSRZUYUYCWXAFQFBHE5ZEJPDXCIMVZC3LH2C7IU35FTI2NOQ")
    }

    let account = "GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI"
    let destination = "GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52"
    let privateKeyData = Data(hexString: "59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722")!

    func testSigner() {
        let operation = StellarOperationPayment.with {
            $0.destination = destination
            $0.amount = 10_000_000
        }
        let input = StellarSigningInput.with {
            $0.passphrase = StellarPassphrase.stellar.description
            $0.fee = 1000
            $0.sequence = 2 // from account info api
            $0.account = account
            $0.privateKey = privateKeyData
            $0.opPayment = operation
        }

        let output: StellarSigningOutput = AnySigner.sign(input: input, coin: .stellar)
        XCTAssertEqual(output.signature, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAAAAAABAAAAAAAAAAEAAAAAxYC2MXoOs5v3/NT6PBn9q0uJu6u/YQle5FBa9uzteq4AAAAAAAAAAACYloAAAAAAAAAAARnfXKIAAABAocQZwTnVvGMQlpdGacWvgenxN5ku8YB8yhEGrDfEV48yDqcj6QaePAitDj/N2gxfYD9Q2pJ+ZpkQMsZZG4ACAg==")
    }

    func testSignWithMemoHash() {
        let operation = StellarOperationPayment.with {
            $0.destination = destination
            $0.amount = 10_000_000
        }
        let input = StellarSigningInput.with {
            $0.passphrase = StellarPassphrase.stellar.description
            $0.fee = 1000
            $0.sequence = 2 // from account info api
            $0.account = account
            $0.privateKey = privateKeyData
            $0.memoHash = StellarMemoHash.with {
                $0.hash = Data(hexString: "315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3")!
            }
            $0.opPayment = operation
        }

        let output: StellarSigningOutput = AnySigner.sign(input: input, coin: .stellar)
        XCTAssertEqual(output.signature, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAMxX1vbdtB4xDuKwAZOSgFkYSsfznfIaTRb/JTHWJTt0wAAAAEAAAAAAAAAAQAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAECIyh1BG+hER5W+dgHDKe49X6VEYRWIjajM4Ufq3DUG/yw7Xv1MMF4eax3U0TRi7Qwj2fio/DRD3+/Ljtvip2MD")
    }

    func testSignWithMemoReturn() {
        let operation = StellarOperationPayment.with {
            $0.destination = destination
            $0.amount = 10_000_000
        }
        let input = StellarSigningInput.with {
            $0.passphrase = StellarPassphrase.stellar.description
            $0.fee = 1000
            $0.sequence = 2 // from account info api
            $0.account = account
            $0.privateKey = privateKeyData
            $0.memoReturnHash = StellarMemoHash.with {
                $0.hash = Data(hexString: "315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3")!
            }
            $0.opPayment = operation
        }

        let output: StellarSigningOutput = AnySigner.sign(input: input, coin: .stellar)
        XCTAssertEqual(output.signature, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAQxX1vbdtB4xDuKwAZOSgFkYSsfznfIaTRb/JTHWJTt0wAAAAEAAAAAAAAAAQAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAEBd77iui04quoaoWMfeJO06nRfn3Z9bptbAj7Ol44j3ApU8c9dJwVhJbQ7La4mKgIkYviEhGx3AIulFYCkokb8M")
    }

    func testSignWithMemoId() {
        let operation = StellarOperationPayment.with {
            $0.destination = destination
            $0.amount = 10_000_000
        }
        let input = StellarSigningInput.with {
            $0.passphrase = StellarPassphrase.stellar.description
            $0.fee = 1000
            $0.sequence = 2 // from account info api
            $0.account = account
            $0.privateKey = privateKeyData
            $0.memoID = StellarMemoId.with {
                $0.id = 1234567890
            }
            $0.opPayment = operation
        }

        let output: StellarSigningOutput = AnySigner.sign(input: input, coin: .stellar)
        XCTAssertEqual(output.signature, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAIAAAAASZYC0gAAAAEAAAAAAAAAAQAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAEAOJ8wwCizQPf6JmkCsCNZolQeqet2qN7fgLUUQlwx3TNzM0+/GJ6Qc2faTybjKy111rE60IlnfaPeMl/nyxKIB")
    }

    func testSignCreateAccount() {
        let operation = StellarOperationCreateAccount.with {
            $0.destination = destination
            $0.amount = 10_000_000
        }
        let input = StellarSigningInput.with {
            $0.passphrase = StellarPassphrase.stellar.description
            $0.fee = 1000
            $0.sequence = 2 // from account info api
            $0.account = account
            $0.privateKey = privateKeyData
            $0.memoID = StellarMemoId.with {
                $0.id = 1234567890
            }
            $0.opCreateAccount = operation
        }

        let output: StellarSigningOutput = AnySigner.sign(input: input, coin: .stellar)
        XCTAssertEqual(output.signature, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAIAAAAASZYC0gAAAAEAAAAAAAAAAAAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAmJaAAAAAAAAAAAEZ31yiAAAAQNgqNDqbe0X60gyH+1xf2Tv2RndFiJmyfbrvVjsTfjZAVRrS2zE9hHlqPQKpZkGKEFka7+1ElOS+/m/1JDnauQg=")
    }

    func testSignChangeTrust() {
        let assetMobi = StellarAsset.with {
            $0.issuer = "GA6HCMBLTZS5VYYBCATRBRZ3BZJMAFUDKYYF6AH6MVCMGWMRDNSWJPIH"
            $0.alphanum4 = "MOBI"
        }
        let operation = StellarOperationChangeTrust.with {
            $0.asset = assetMobi
            $0.validBefore = 1613336576
        }
        let input = StellarSigningInput.with {
            $0.passphrase = StellarPassphrase.stellar.description
            $0.fee = 10000
            $0.sequence = 144098454883270659
            $0.account = "GDFEKJIFKUZP26SESUHZONAUJZMBSODVN2XBYN4KAGNHB7LX2OIXLPUL"
            $0.privateKey = Data(hexString: "3c0635f8638605aed6e461cf3fa2d508dd895df1a1655ff92c79bfbeaf88d4b9")!
            $0.opChangeTrust = operation
        }

        let output: StellarSigningOutput = AnySigner.sign(input: input, coin: .stellar)
        XCTAssertEqual(output.signature, "AAAAAMpFJQVVMv16RJUPlzQUTlgZOHVurhw3igGacP1305F1AAAnEAH/8MgAAAADAAAAAQAAAAAAAAAAAAAAAGApkAAAAAAAAAAAAQAAAAAAAAAGAAAAAU1PQkkAAAAAPHEwK55l2uMBECcQxzsOUsAWg1YwXwD+ZUTDWZEbZWR//////////wAAAAAAAAABd9ORdQAAAEAnfyXyaNQX5Bq3AEQVBIaYd+cLib+y2sNY7DF/NYVSE51dZ6swGGElz094ObsPefmVmeRrkGsSc/fF5pmth+wJ")
    }
}
