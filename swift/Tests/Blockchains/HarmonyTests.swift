// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class HarmonyTests: XCTestCase {
    let localNet = "0x02"

    func testSigner() {
        let transaction = HarmonyTransactionMessage.with {
            $0.nonce = Data(hexString: "0x9")!
            $0.gasPrice = Data(hexString: "0x")!
            $0.gasLimit = Data(hexString: "0x5208")!
            $0.fromShardID = Data(hexString: "0x1")!
            $0.toShardID = Data(hexString: "0x0")!
            $0.payload = Data(hexString: "0x")!
            $0.toAddress = "one1d2rngmem4x2c6zxsjjz29dlah0jzkr0k2n88wc"
            $0.amount = Data(hexString: "0x4c53ecdc18a60000")!
        }
        let input = HarmonySigningInput.with {
            $0.transactionMessage = transaction
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = Data(hexString: "0xb578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e")!
        }
        let output = HarmonySigner.sign(input: input)
        let expected = "f86909808252080180946a87346f3ba9958d08d09484a2b7fdbbe42b0df6884c53ecdc18a600008028a0325aed6caa01a5235b"
            + "7a508c8ab67f0c43946b05a1ea6a3e0628de4033fe372da06c19085d3376c30f6dc47cec795991cd37d6d0ebddfa633b0a8f494bc19cd01b"
        XCTAssertEqual(output.encoded.hexString, expected)
        XCTAssertEqual(output.v.hexString, "28")
        XCTAssertEqual(output.r.hexString, "325aed6caa01a5235b7a508c8ab67f0c43946b05a1ea6a3e0628de4033fe372d")
        XCTAssertEqual(output.s.hexString, "6c19085d3376c30f6dc47cec795991cd37d6d0ebddfa633b0a8f494bc19cd01b")
    }

    let oneAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
    let privateKeyData = Data(hexString: "4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48")!
    let pubKeyData = Data(hexString: "b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b62247608611")!

    func testStakingCreateValidator() {
        let description = HarmonyDescription.with {
            $0.name = "Alice"
            $0.identity = "alice"
            $0.website = "alice.harmony.one"
            $0.securityContact = "Bob"
            $0.details = "Don't mess with me!!!"
        }
        let rate = HarmonyDecimal.with {
            $0.value = Data(hexString: "0x1")!
            $0.precision = Data(hexString: "0x1")!
        }
        let maxRate = HarmonyDecimal.with {
            $0.value = Data(hexString: "0x9")!
            $0.precision = Data(hexString: "0x1")!
        }
        let maxChangeRate = HarmonyDecimal.with {
            $0.value = Data(hexString: "0x5")!
            $0.precision = Data(hexString: "0x2")!
        }
        let commission = HarmonyCommissionRate.with {
            $0.rate = rate
            $0.maxRate = maxRate
            $0.maxChangeRate = maxChangeRate
        }
        let pubKey = pubKeyData
        let createValidator = HarmonyDirectiveCreateValidator.with {
            $0.validatorAddress = oneAddress
            $0.description_p = description
            $0.commissionRates = commission
            $0.minSelfDelegation = Data(hexString: "0xa")!
            $0.maxTotalDelegation = Data(hexString: "0x0bb8")!
            $0.slotPubKeys = [pubKey]
            $0.amount = Data(hexString: "0x64")!
        }
        let staking = HarmonyStakingMessage.with {
            $0.createValidatorMessage = createValidator
            $0.nonce = Data(hexString: "0x2")!
            $0.gasPrice = Data(hexString: "0x")!
            $0.gasLimit = Data(hexString: "0x64")!
        }
        let input = HarmonySigningInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = privateKeyData
            $0.stakingMessage = staking
        }
        let output = HarmonySigner.sign(input: input)

        let expected = "f8ed80f8a494ebcd16e8c1d8f493ba04e99a56474122d81a9c58f83885416c69636585616c69636591616c6963"
            + "652e6861726d6f6e792e6f6e6583426f6295446f6e2774206d6573732077697468206d65212121ddc988016345"
            + "785d8a0000c9880c7d713b49da0000c887b1a2bc2ec500000a820bb8f1b0b9486167ab9087ab818dc4ce026edb"
            + "5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b622476086116402806428a0476e8a0fe478"
            + "e0d03ff10222d4d590bca8cee3ec51b830f4fc4a8bee5d0e9d28a03b2be18e73b2f99d7e2691485a0e166f28e6"
            + "2815079c126e68f876dc97339f8f"

        XCTAssertEqual(output.encoded.hexString, expected)
        XCTAssertEqual(output.v.hexString, "28")
        XCTAssertEqual(output.r.hexString, "476e8a0fe478e0d03ff10222d4d590bca8cee3ec51b830f4fc4a8bee5d0e9d28")
        XCTAssertEqual(output.s.hexString, "3b2be18e73b2f99d7e2691485a0e166f28e62815079c126e68f876dc97339f8f")
    }

    func testStakingEditValidator() {
        let desc = HarmonyDescription.with {
            $0.name = "Alice"
            $0.identity = "alice"
            $0.website = "alice.harmony.one"
            $0.securityContact = "Bob"
            $0.details = "Don't mess with me!!!"
        }
        let commissionRate = HarmonyDecimal.with {
            $0.value = Data(hexString: "0x1")!
            $0.precision = Data(hexString: "0x1")!
        }
        let editValidator = HarmonyDirectiveEditValidator.with {
            $0.validatorAddress = oneAddress
            $0.description_p = desc
            $0.commissionRate = commissionRate
            $0.minSelfDelegation = Data(hexString: "0xa")!
            $0.maxTotalDelegation = Data(hexString: "0x0bb8")!
            $0.slotKeyToRemove = pubKeyData
            $0.slotKeyToAdd = pubKeyData
        }
        let staking = HarmonyStakingMessage.with {
            $0.editValidatorMessage = editValidator
            $0.nonce = Data(hexString: "0x2")!
            $0.gasPrice = Data(hexString: "0x")!
            $0.gasLimit = Data(hexString: "0x64")!
        }
        let input = HarmonySigningInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = privateKeyData
            $0.stakingMessage = staking
        }
        let output = HarmonySigner.sign(input: input)

        let expected = "f9010801f8bf94ebcd16e8c1d8f493ba04e99a56474122d81a9c58f83885416c69636585616c69636591616c"
            + "6963652e6861726d6f6e792e6f6e6583426f6295446f6e2774206d6573732077697468206d65212121c9880163"
            + "45785d8a00000a820bb8b0b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7"
            + "d12f0e6dd5307a73b62247608611b0b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced"
            + "1ad181e7d12f0e6dd5307a73b6224760861102806427a05e54b55272f6bf5ffeca10d85976749d6b844cc9f30b"
            + "a3285b9ab8a82d53e3e3a03ce04d9a9f834e20b22aa918ead346c84a04b1504fe3ff9e38f21c5e5712f013"

        XCTAssertEqual(output.encoded.hexString, expected)
        XCTAssertEqual(output.v.hexString, "27")
        XCTAssertEqual(output.r.hexString, "5e54b55272f6bf5ffeca10d85976749d6b844cc9f30ba3285b9ab8a82d53e3e3")
        XCTAssertEqual(output.s.hexString, "3ce04d9a9f834e20b22aa918ead346c84a04b1504fe3ff9e38f21c5e5712f013")
    }

    func testStakingDelegate() {
        let delegate = HarmonyDirectiveDelegate.with {
            $0.delegatorAddress = oneAddress
            $0.validatorAddress = oneAddress
            $0.amount = Data(hexString: "0xa")!
        }
        let staking = HarmonyStakingMessage.with {
            $0.delegateMessage = delegate
            $0.nonce = Data(hexString: "0x2")!
            $0.gasPrice = Data(hexString: "0x")!
            $0.gasLimit = Data(hexString: "0x64")!
        }
        let input = HarmonySigningInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = privateKeyData
            $0.stakingMessage = staking
        }
        let output = HarmonySigner.sign(input: input)

        let e1 = "f87302eb94ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a"
        let e2 = "9c580a02806428a0ada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80a05c28dbc4"
        let e3 = "1763dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54"

        XCTAssertEqual(output.encoded.hexString, e1 + e2 + e3)
        XCTAssertEqual(output.v.hexString, "28")
        XCTAssertEqual(output.r.hexString, "ada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80")
        XCTAssertEqual(output.s.hexString, "5c28dbc41763dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54")
    }

    func testStakingUndelegate() {
        let undelegate = HarmonyDirectiveUndelegate.with {
            $0.delegatorAddress = oneAddress
            $0.validatorAddress = oneAddress
            $0.amount = Data(hexString: "0xa")!
        }
        let staking = HarmonyStakingMessage.with {
            $0.undelegateMessage = undelegate
            $0.nonce = Data(hexString: "0x2")!
            $0.gasPrice = Data(hexString: "0x")!
            $0.gasLimit = Data(hexString: "0x64")!
        }
        let input = HarmonySigningInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = privateKeyData
            $0.stakingMessage = staking
        }
        let output = HarmonySigner.sign(input: input)

        let expected = "f87303eb94ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a9c"
            + "580a02806428a05bf8c653567defe2c3728732bc9d67dd099a977df91c740a883fd89e03abb6e2a05202c4b516"
            + "52d5144c6a30d14d1a7a316b5a4a6b49be985b4bc6980e49f7acb7"

        XCTAssertEqual(output.encoded.hexString, expected)
        XCTAssertEqual(output.v.hexString, "28")
        XCTAssertEqual(output.r.hexString, "5bf8c653567defe2c3728732bc9d67dd099a977df91c740a883fd89e03abb6e2")
        XCTAssertEqual(output.s.hexString, "5202c4b51652d5144c6a30d14d1a7a316b5a4a6b49be985b4bc6980e49f7acb7")
    }

    func testStakingCollectRewards() {
        let collectRewards = HarmonyDirectiveCollectRewards.with {
            $0.delegatorAddress = oneAddress
        }
        let staking = HarmonyStakingMessage.with {
            $0.collectRewards = collectRewards
            $0.nonce = Data(hexString: "0x2")!
            $0.gasPrice = Data(hexString: "0x")!
            $0.gasLimit = Data(hexString: "0x64")!
        }
        let input = HarmonySigningInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = privateKeyData
            $0.stakingMessage = staking
        }
        let output = HarmonySigner.sign(input: input)

        let expected = "f85d04d594ebcd16e8c1d8f493ba04e99a56474122d81a9c5802806428a04c15c72f425"
            + "77001083a9c7ff9d9724077aec704a524e53dc7c9afe97ca4e625a055c13ea17c3efd1cd9"
            + "1f2988c7e7673950bac5a08c174f2d0af27a82039f1e3d"

        XCTAssertEqual(output.encoded.hexString, expected)
        XCTAssertEqual(output.v.hexString, "28")
        XCTAssertEqual(output.r.hexString, "4c15c72f42577001083a9c7ff9d9724077aec704a524e53dc7c9afe97ca4e625")
        XCTAssertEqual(output.s.hexString, "55c13ea17c3efd1cd91f2988c7e7673950bac5a08c174f2d0af27a82039f1e3d")
    }
}
