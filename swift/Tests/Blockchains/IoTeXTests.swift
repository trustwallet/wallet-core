// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class IoTeXTests: XCTestCase {
    func testSign() {
        let privateKey = PrivateKey(data: Data(hexString: "0x68ffa8ec149ce50da647166036555f73d57f662eb420e154621e5f24f6cf9748")!)!

        let input = IoTeXSigningInput.with {
            $0.version = 1
            $0.nonce = 1
            $0.gasLimit = 1
            $0.gasPrice = "1"
            $0.transfer = IoTeXTransfer.with {
                $0.amount = "1"
                $0.recipient = AnyAddress(string: "io1e2nqsyt7fkpzs5x7zf2uk0jj72teu5n6aku3tr", coin: .ioTeX)!.description
            }
            $0.privateKey = privateKey.data
        }

        let output: IoTeXSigningOutput = AnySigner.sign(input: input, coin: .ioTeX)

        XCTAssertEqual(output.encoded.hexString, "0a39080110011801220131522e0a01311229696f3165326e7173797437666b707a733578377a6632756b306a6a3732746575356e36616b75337472124104fb30b196ce3e976593ecc2da220dca9cdea8c84d2373770042a930b892ac0f5cf762f20459c9100eb9d4d7597f5817bf21e10b53a0120b9ec1ba5cddfdcb669b1a41ec9757ae6c9009315830faaab250b6db0e9535b00843277f596ae0b2b9efc0bd4e14138c056fc4cdfa285d13dd618052b3d1cb7a3f554722005a2941bfede96601")
    }

    func testSignStakingCreate() {
        var input = IoTeXSigningInput.with {
            $0.version = 1
            $0.nonce = 0
            $0.gasLimit = 1000000
            $0.gasPrice = "10"
            $0.privateKey = Data(hexString: "cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")!
        }
        input.stakeCreate = IoTeXStaking.Create.with {
            $0.candidateName = "io19d0p3ah4g8ww9d7kcxfq87yxe7fnr8rpth5shj"
            $0.stakedAmount = "100"
            $0.stakedDuration = 10000
            $0.autoStake = true
            $0.payload = "payload".data(using: .utf8)!
        }
        let output: IoTeXSigningOutput = AnySigner.sign(input: input, coin: .ioTeX)

        XCTAssertEqual(output.encoded.hexString, "0a4b080118c0843d22023130c2023e0a29696f313964307033616834673877773964376b63786671383779786537666e7238727074683573686a120331303018904e20012a077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a412e8bac421bab88dcd99c26ac8ffbf27f11ee57a41e7d2537891bfed5aed8e2e026d46e55d1b856787bc1cd7c1216a6e2534c5b5d1097c3afe8e657aa27cbbb0801")
        XCTAssertEqual(output.hash.hexString, "f1785e47b4200c752bb6518bd18097a41e075438b8c18c9cb00e1ae2f38ce767")
    }

    func testSignStakingAddDeposit() {
        var input = IoTeXSigningInput.with {
            $0.version = 1
            $0.nonce = 0
            $0.gasLimit = 1000000
            $0.gasPrice = "10"
            $0.privateKey = Data(hexString: "cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")!
        }
        input.stakeAddDeposit = IoTeXStaking.AddDeposit.with {
            $0.bucketIndex = 10
            $0.amount = "10"
            $0.payload = "payload".data(using: .utf8)!
        }
        let output: IoTeXSigningOutput = AnySigner.sign(input: input, coin: .ioTeX)

        XCTAssertEqual(output.encoded.hexString, "0a1c080118c0843d22023130da020f080a120231301a077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a41a48ab1feba8181d760de946aefed7d815a89fd9b1ab503d2392bb55e1bb75eec42dddc8bd642f89accc3a37b3cf15a103a95d66695fdf0647b202869fdd66bcb01")
        XCTAssertEqual(output.hash.hexString, "ca8937d6f224a4e4bf93cb5605581de2d26fb0481e1dfc1eef384ee7ccf94b73")
    }
    
    func testSignStakingUnstake() {
        var input = IoTeXSigningInput.with {
            $0.version = 1
            $0.nonce = 0
            $0.gasLimit = 1000000
            $0.gasPrice = "10"
            $0.privateKey = Data(hexString: "cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")!
        }
        input.stakeUnstake = IoTeXStaking.Reclaim.with {
            $0.bucketIndex = 10
            $0.payload = "payload".data(using: .utf8)!
        }
        let output: IoTeXSigningOutput = AnySigner.sign(input: input, coin: .ioTeX)

        XCTAssertEqual(output.encoded.hexString, "0a18080118c0843d22023130ca020b080a12077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a4100adee39b48e1d3dbbd65298a57c7889709fc4df39987130da306f6997374a184b7e7c232a42f21e89b06e6e7ceab81303c6b7483152d08d19ac829b22eb81e601")
        XCTAssertEqual(output.hash.hexString, "bed58b64a6c4e959eca60a86f0b2149ce0e1dd527ac5fd26aef725ebf7c22a7d")
    }

    func testSignStakingWithdraw() {
        var input = IoTeXSigningInput.with {
            $0.version = 1
            $0.nonce = 0
            $0.gasLimit = 1000000
            $0.gasPrice = "10"
            $0.privateKey = Data(hexString: "cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")!
        }
        input.stakeWithdraw = IoTeXStaking.Reclaim.with {
            $0.bucketIndex = 10
            $0.payload = "payload".data(using: .utf8)!
        }
        let output: IoTeXSigningOutput = AnySigner.sign(input: input, coin: .ioTeX)

        XCTAssertEqual(output.encoded.hexString, "0a18080118c0843d22023130d2020b080a12077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a4152644d102186be6640d46b517331f3402e24424b0d85129595421d28503d75340b2922f5a0d4f667bbd6f576d9816770286b2ce032ba22eaec3952e24da4756b00")
        XCTAssertEqual(output.hash.hexString, "28049348cf34f1aa927caa250e7a1b08778c44efaf73b565b6fa9abe843871b4")
    }

    func testSignStakingRestake() {
        var input = IoTeXSigningInput.with {
            $0.version = 1
            $0.nonce = 0
            $0.gasLimit = 1000000
            $0.gasPrice = "10"
            $0.privateKey = Data(hexString: "cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")!
        }
        input.stakeRestake = IoTeXStaking.Restake.with {
            $0.bucketIndex = 10
            $0.stakedDuration = 1000
            $0.autoStake = true
            $0.payload = "payload".data(using: .utf8)!
        }
        let output: IoTeXSigningOutput = AnySigner.sign(input: input, coin: .ioTeX)

        XCTAssertEqual(output.encoded.hexString, "0a1d080118c0843d22023130e20210080a10e807180122077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a41e2e763aed5b1fd1a8601de0f0ae34eb05162e34b0389ae3418eedbf762f64959634a968313a6516dba3a97b34efba4753bbed3a33d409ecbd45ac75007cd8e9101")
        XCTAssertEqual(output.hash.hexString, "8816e8f784a1fce40b54d1cd172bb6976fd9552f1570c73d1d9fcdc5635424a9")
    }

    func testSignStakingChangeCandidate() {
        var input = IoTeXSigningInput.with {
            $0.version = 1
            $0.nonce = 0
            $0.gasLimit = 1000000
            $0.gasPrice = "10"
            $0.privateKey = Data(hexString: "cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")!
        }
        input.stakeChangeCandidate = IoTeXStaking.ChangeCandidate.with {
            $0.bucketIndex = 10    
            $0.candidateName = "io1xpq62aw85uqzrccg9y5hnryv8ld2nkpycc3gza"
            $0.payload = "payload".data(using: .utf8)!
        }
        let output: IoTeXSigningOutput = AnySigner.sign(input: input, coin: .ioTeX)

        XCTAssertEqual(output.encoded.hexString, "0a43080118c0843d22023130ea0236080a1229696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611a077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a41d519eb3747163b945b862989b7e82a7f8468001e9683757cb88d5ddd95f81895047429e858bd48f7d59a88bfec92de231d216293aeba1e4fbe11461d9c9fc99801")
        XCTAssertEqual(output.hash.hexString, "186526b5b9fe74e25beb52c83c41780a69108160bef2ddaf3bffb9f1f1e5e73a")
    }

    func testSignStakingTransfer() {
        var input = IoTeXSigningInput.with {
            $0.version = 1
            $0.nonce = 0
            $0.gasLimit = 1000000
            $0.gasPrice = "10"
            $0.privateKey = Data(hexString: "cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")!
        }
        input.stakeTransferOwnership = IoTeXStaking.TransferOwnership.with {
            $0.bucketIndex = 10    
            $0.voterAddress = "io1xpq62aw85uqzrccg9y5hnryv8ld2nkpycc3gza"
            $0.payload = "payload".data(using: .utf8)!
        }
        let output: IoTeXSigningOutput = AnySigner.sign(input: input, coin: .ioTeX)

        XCTAssertEqual(output.encoded.hexString, "0a43080118c0843d22023130f20236080a1229696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611a077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a41fa26db427ab87a56a129196c1604f2e22c4dd2a1f99b2217bc916260757d00093d9e6dccdf53e3b0b64e41a69d71c238fbf9281625164694a74dfbeba075d0ce01")
        XCTAssertEqual(output.hash.hexString, "74b2e1d6a09ba5d1298fa422d5850991ae516865077282196295a38f93c78b85")
    }

    func testSignCandidateRegister() {
        var input = IoTeXSigningInput.with {
            $0.version = 1
            $0.nonce = 0
            $0.gasLimit = 1000000
            $0.gasPrice = "1000"
            $0.privateKey = Data(hexString: "cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")!
        }
        input.candidateRegister = IoTeXStaking.CandidateRegister.with {
            $0.candidate = IoTeXStaking.CandidateBasicInfo.with {
                $0.name = "test"
                $0.operatorAddress = "io10a298zmzvrt4guq79a9f4x7qedj59y7ery84he"
                $0.rewardAddress = "io13sj9mzpewn25ymheukte4v39hvjdtrfp00mlyv"
            }
            $0.stakedAmount = "100"
            $0.stakedDuration = 10000
            $0.autoStake = false
            $0.ownerAddress = "io19d0p3ah4g8ww9d7kcxfq87yxe7fnr8rpth5shj"
            $0.payload = "payload".data(using: .utf8)!
        }
        let output: IoTeXSigningOutput = AnySigner.sign(input: input, coin: .ioTeX)

        XCTAssertEqual(output.encoded.hexString, "0aaa01080118c0843d220431303030fa029a010a5c0a04746573741229696f3130613239387a6d7a7672743467757137396139663478377165646a35397937657279383468651a29696f3133736a396d7a7065776e3235796d6865756b74653476333968766a647472667030306d6c7976120331303018904e2a29696f313964307033616834673877773964376b63786671383779786537666e7238727074683573686a32077061796c6f6164124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a417819b5bcb635e3577acc8ca757f2c3d6afa451c2b6ff8a9179b141ac68e2c50305679e5d09d288da6f0fb52876a86c74deab6a5247edc6d371de5c2f121e159400")
        XCTAssertEqual(output.hash.hexString, "35f53a536e014b32b85df50483ef04849b80ad60635b3b1979c5ba1096b65237")
    }

    func testSignCandidateUpdate() {
        var input = IoTeXSigningInput.with {
            $0.version = 1
            $0.nonce = 0
            $0.gasLimit = 1000000
            $0.gasPrice = "10"
            $0.privateKey = Data(hexString: "cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1")!
        }
        input.candidateUpdate = IoTeXStaking.CandidateBasicInfo.with {
            $0.name = "test"
            $0.operatorAddress = "io1cl6rl2ev5dfa988qmgzg2x4hfazmp9vn2g66ng"
            $0.rewardAddress = "io1juvx5g063eu4ts832nukp4vgcwk2gnc5cu9ayd"
        }
        let output: IoTeXSigningOutput = AnySigner.sign(input: input, coin: .ioTeX)

        XCTAssertEqual(output.encoded.hexString, "0a69080118c0843d2202313082035c0a04746573741229696f31636c36726c32657635646661393838716d677a673278346866617a6d7039766e326736366e671a29696f316a757678356730363365753474733833326e756b7034766763776b32676e6335637539617964124104755ce6d8903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a1038ed9da8daf331a4101885c9c6684a4a8f2f5bf11f8326f27be48658f292e8f55ec8a11a604bb0c563a11ebf12d995ca1c152e00f8e0f0edf288db711aa10dbdfd5b7d73b4a28e1f701")
        XCTAssertEqual(output.hash.hexString, "ca1a28f0e9a58ffc67037cc75066dbdd8e024aa2b2e416e4d6ce16c3d86282e5")
    }
}
