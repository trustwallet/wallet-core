// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

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
                $0.recipient = IoTeXAddress(string: "io1e2nqsyt7fkpzs5x7zf2uk0jj72teu5n6aku3tr")!.description
            }
            $0.privateKey = privateKey.data
        }

        let encoded = IoTeXSigner.sign(input: input).encoded

        XCTAssertEqual(encoded.hexString, "0a39080110011801220131522e0a01311229696f3165326e7173797437666b707a733578377a6632756b306a6a3732746575356e36616b75337472124104fb30b196ce3e976593ecc2da220dca9cdea8c84d2373770042a930b892ac0f5cf762f20459c9100eb9d4d7597f5817bf21e10b53a0120b9ec1ba5cddfdcb669b1a41ec9757ae6c9009315830faaab250b6db0e9535b00843277f596ae0b2b9efc0bd4e14138c056fc4cdfa285d13dd618052b3d1cb7a3f554722005a2941bfede96601");
    }

    func stakingInput(data: Data) -> IoTeXSigningInput {
        let input = IoTeXSigningInput.with {
            $0.version = 1
            $0.nonce = 123
            $0.gasLimit = 888
            $0.gasPrice = "999"
            $0.execution = IoTeXExecution.with {
                $0.amount = "456"
                $0.contract = "io1xpq62aw85uqzrccg9y5hnryv8ld2nkpycc3gza"
                $0.data = data
            }
            $0.privateKey = Data(hexString: "0806c458b262edd333a191e92f561aff338211ee3e18ab315a074a2d82aa343f")!
        }
        return input
    }

    func testStake() {
        // candidate name is a string but hex encoded
        let candidate = Data([0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c])
        let stake = IoTeXStaking.stake(candidate: candidate, duration: 1001, nonDecay: true, data: "this is a test".data(using: .utf8)!)

        let input = stakingInput(data: stake)
        let output = IoTeXSigner.sign(input: input)

        XCTAssertEqual(output.encoded.hexString, "0a86020801107b18f806220339393962f7010a033435361229696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611ac40107c35fc00102030405060708090a0b0c000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003e900000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000000e74686973206973206120746573740000000000000000000000000000000000001241044e18306ae9ef4ec9d07bf6e705442d4d1a75e6cdf750330ca2d880f2cc54607c9c33deb9eae9c06e06e04fe9ce3d43962cc67d5aa34fbeb71270d4bad3d648d91a41a558bc9a4bfba920242ccd4d5c5da363ec534d4dd5eb67f88e9db7aaad5c50ad62dfe298c0e54e311ebba045f48cea1136e42a123a8e6b03d3e6ed82d4ec2b9401")
        XCTAssertEqual(output.hash.hexString, "41b1f8be5f6b884c06556fba2611716e8e514b507f5a653fc02ac50ba13fbd6c")
    }

    func testUnstake() {
        let unstake = IoTeXStaking.unstake(pyggIndex: 1001, data: Data())
        let input = stakingInput(data: unstake)
        let output = IoTeXSigner.sign(input: input)

        XCTAssertEqual(output.encoded.hexString, "0aa5010801107b18f80622033939396296010a033435361229696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611a64c8fd6ed000000000000000000000000000000000000000000000000000000000000003e9000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000001241044e18306ae9ef4ec9d07bf6e705442d4d1a75e6cdf750330ca2d880f2cc54607c9c33deb9eae9c06e06e04fe9ce3d43962cc67d5aa34fbeb71270d4bad3d648d91a41eeb7cb3fa7ec22a61156753d569b3f4da3c74c3c7e2f148b1a43e11d220cac5d164663ff6c785439679b088de9d7f2437545f007ca9cda4b2f5327d2c6eda5aa01")
        XCTAssertEqual(output.hash.hexString, "b93a2874a72ce4eb8a41a20c209cf3fd188671ed8be8239a57960cbed887e962")
    }

    func testWithdraw() {
        let withdraw = IoTeXStaking.withdraw(pyggIndex: 1001, data: Data())
        let input = stakingInput(data: withdraw)
        let output = IoTeXSigner.sign(input: input)

       XCTAssertEqual(output.encoded.hexString, "0aa5010801107b18f80622033939396296010a033435361229696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611a64030ba25d00000000000000000000000000000000000000000000000000000000000003e9000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000001241044e18306ae9ef4ec9d07bf6e705442d4d1a75e6cdf750330ca2d880f2cc54607c9c33deb9eae9c06e06e04fe9ce3d43962cc67d5aa34fbeb71270d4bad3d648d91a41903c79d042f6b1c05446ececb3d760ca154c539b5787e66135cd3db77638294d18dbbbbcb0de8b9a393cc7c0448cf246898e4343a2a51666e21e738ee6d8a6f700")
        XCTAssertEqual(output.hash.hexString, "2b2657247a72cb262de214b4e793c7a01fa2139fd5d12a46d43c24f87f9e2396")
    }

    func testAddStake() {
        let add = IoTeXStaking.addStake(pyggIndex: 1001, data: Data())
        let input = stakingInput(data: add)
        let output = IoTeXSigner.sign(input: input)

       XCTAssertEqual(output.encoded.hexString, "0aa5010801107b18f80622033939396296010a033435361229696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611a646e7b301700000000000000000000000000000000000000000000000000000000000003e9000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000001241044e18306ae9ef4ec9d07bf6e705442d4d1a75e6cdf750330ca2d880f2cc54607c9c33deb9eae9c06e06e04fe9ce3d43962cc67d5aa34fbeb71270d4bad3d648d91a410f0832bb7a48c9e468c0bcb2c4a35a202f9519a63c4d6474b48087ab9dc33aea18127940d5cda43710cd874cbdf7a7b26efc9c04236e14dfb4d9b6f7095b0b6c01")
        XCTAssertEqual(output.hash.hexString, "c71058812a5febe5cdcdaf9499ba0b2c895f88d1acd3203e5097b307c2a5f1d1")
    }

    func testMoveStake() {
        // candidate name is a string but hex encoded
        let candidate = Data([0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c])
        let move = IoTeXStaking.moveStake(pyggIndex: 1001, candidate: candidate, data: Data())
        let input = stakingInput(data: move)
        let output = IoTeXSigner.sign(input: input)

       XCTAssertEqual(output.encoded.hexString, "0ac6010801107b18f806220339393962b7010a033435361229696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611a8401d3e41fd200000000000000000000000000000000000000000000000000000000000003e90102030405060708090a0b0c0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000001241044e18306ae9ef4ec9d07bf6e705442d4d1a75e6cdf750330ca2d880f2cc54607c9c33deb9eae9c06e06e04fe9ce3d43962cc67d5aa34fbeb71270d4bad3d648d91a4118b71944993d1cd5362379ff64e892024a75ce697b2755cc9bbfcac24482a19b1ec2951bec7461ef9eb0723f803987cb87e3c3afb340006cd5b413c1fac10d7c01")
        XCTAssertEqual(output.hash.hexString, "33290ded342efaebf795855be73d34cbac149a2415ff9558de10303e6126f30d")
    }
}
