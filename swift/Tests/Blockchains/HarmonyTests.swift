// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class HarmonyTests: XCTestCase {
    func testSigner() {
        let localNet = "0x02"
        let input = TW_Harmony_Proto_SigningInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.nonce = Data(hexString: "0x9")!
            $0.gasPrice = Data(hexString: "0x")!
            $0.gasLimit = Data(hexString: "0x5208")!
            $0.fromShardID = Data(hexString: "0x1")!
            $0.toShardID = Data(hexString: "0x0")!
            $0.payload = Data(hexString: "0x")!
            $0.toAddress = "one1d2rngmem4x2c6zxsjjz29dlah0jzkr0k2n88wc"
            $0.amount = Data(hexString: "0x4c53ecdc18a60000")!
            $0.privateKey = Data(hexString: "0xb578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e")!
        }
        let output = HarmonySigner.sign(input: input)
        let e1 = "f86909808252080180946a87346f3ba9958d08d09484a2b7fdbbe42b0df6884c53ecdc18a600008028a0325aed6caa01a5235b"
        let e2 = "7a508c8ab67f0c43946b05a1ea6a3e0628de4033fe372da06c19085d3376c30f6dc47cec795991cd37d6d0ebddfa633b0a8f494bc19cd01b"
        XCTAssertEqual(output.encoded.hexString, e1 + e2)
        XCTAssertEqual(output.v.hexString, "28")
        XCTAssertEqual(output.r.hexString, "325aed6caa01a5235b7a508c8ab67f0c43946b05a1ea6a3e0628de4033fe372d")
        XCTAssertEqual(output.s.hexString, "6c19085d3376c30f6dc47cec795991cd37d6d0ebddfa633b0a8f494bc19cd01b")
    }

    func testStakingNewValidator() {
        let localNet = "0x02"
        let description = TW_Harmony_Proto_Description.with {
            $0.name = "Alice"
            $0.identity = "alice"
            $0.website = "alice.harmony.one"
            $0.securityContact = "Bob"
            $0.details = "Don't mess with me!!!"
        }
        let rate = TW_Harmony_Proto_Decimal.with {
            $0.value = Data(hexString: "64")!
        }
        let maxRate = TW_Harmony_Proto_Decimal.with {
            $0.value = Data(hexString: "96")!
        }
        let maxChangeRate = TW_Harmony_Proto_Decimal.with {
            $0.value = Data(hexString: "5")!
        }
        let commission = TW_Harmony_Proto_CommissionRate.with {
            $0.rate = rate
            $0.maxRate = maxRate
            $0.maxChangeRate = maxChangeRate
        }
        let newValidator = TW_Harmony_Proto_DirectiveNewValidator.with {
            $0.description_p = description
            $0.commission = commission
            $0.minSelfDelegation = Data(hexString: "0xa")!
            $0.stakingAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            $0.pubKey = Data(hexString: "b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b62247608611")!
            $0.amount = Data(hexString: "0x64")!
        }
        let input = TW_Harmony_Proto_StakingTransactionInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = Data(hexString: "4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48")!
            $0.nonce = Data(hexString: "0x2")!
            $0.gasPrice = Data(hexString: "0x")!
            $0.gasLimit = Data(hexString: "0x64")!
            $0.newValidatorMessage = newValidator
        }
        let output = HarmonyStakingSigner.sign(input: input)

        let e1 = "f8ec80f8a3f83885416c69636585616c69636591616c6963652e6861726d6f6e792e6f6e6583426f6295446f6e"
        let e2 = "2774206d6573732077697468206d65212121e0ca89056bc75e2d63100000ca890821ab0d4414980000c9884563"
        let e3 = "918244f400000a94ebcd16e8c1d8f493ba04e99a56474122d81a9c58b0b9486167ab9087ab818dc4ce026edb5b"
        let e4 = "f216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b622476086116402806428a00b1a797d11f7b0"
        let e5 = "dad42abd66c542fab8af0f028b7159bb70e44fe68b2e4d9f2ca07b223662bdb4e1a084f8c506095886a1f5eda0"
        let e6 = "51927fab3516ab9258efc34cd7"

        XCTAssertEqual(output.encoded.hexString, e1 + e2 + e3 + e4 + e5 + e6)
        XCTAssertEqual(output.v.hexString, "28")
        XCTAssertEqual(output.r.hexString, "0b1a797d11f7b0dad42abd66c542fab8af0f028b7159bb70e44fe68b2e4d9f2c")
        XCTAssertEqual(output.s.hexString, "7b223662bdb4e1a084f8c506095886a1f5eda051927fab3516ab9258efc34cd7")
    }

    func testStakingEditValidator() {
        let localNet = "0x02"
        let desc = TW_Harmony_Proto_Description.with {
            $0.name = "Alice"
            $0.identity = "alice"
            $0.website = "alice.harmony.one"
            $0.securityContact = "Bob"
            $0.details = "Don't mess with me!!!"
        }
        let commissionRate = TW_Harmony_Proto_Decimal.with {
            $0.value = Data(hexString: "64")!
        }
        let editValidator = TW_Harmony_Proto_DirectiveEditValidator.with {
            $0.description_p = desc
            $0.stakingAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            $0.commissionRate = commissionRate
            $0.minSelfDelegation = Data(hexString: "0xa")!
        }
        let input = TW_Harmony_Proto_StakingTransactionInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = Data(hexString: "4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48")!
            $0.nonce = Data(hexString: "0x2")!
            $0.gasPrice = Data(hexString: "0x")!
            $0.gasLimit = Data(hexString: "0x64")!
            $0.editValidatorMessage = editValidator
        }
        let output = HarmonyStakingSigner.sign(input: input)

        let e1 = "f8a401f85bf83885416c69636585616c69636591616c6963652e6861726d6f6e792e6f6e6583426f6295446f6e"
        let e2 = "2774206d6573732077697468206d6521212194ebcd16e8c1d8f493ba04e99a56474122d81a9c58ca89056bc75e"
        let e3 = "2d631000000a02806428a071b68b38864e75af60bf05e52b53278e864dbf2eb4a33adeacaa6e1b31f21e59a01e"
        let e4 = "e06acb4d2bc22105454a79ef089fc0794ddba6e2849d9e4236180b47e973ed"

        XCTAssertEqual(output.encoded.hexString, e1 + e2 + e3 + e4)
        XCTAssertEqual(output.v.hexString, "28")
        XCTAssertEqual(output.r.hexString, "71b68b38864e75af60bf05e52b53278e864dbf2eb4a33adeacaa6e1b31f21e59")
        XCTAssertEqual(output.s.hexString, "1ee06acb4d2bc22105454a79ef089fc0794ddba6e2849d9e4236180b47e973ed")
    }

    func testStakingDelegate() {
        let localNet = "0x02"
        let delegate = TW_Harmony_Proto_DirectiveDelegate.with {
            $0.delegatorAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            $0.validatorAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            $0.amount = Data(hexString: "0xa")!
        }
        let input = TW_Harmony_Proto_StakingTransactionInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = Data(hexString: "4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48")!
            $0.nonce = Data(hexString: "0x2")!
            $0.gasPrice = Data(hexString: "0x")!
            $0.gasLimit = Data(hexString: "0x64")!
            $0.delegateMessage = delegate
        }
        let output = HarmonyStakingSigner.sign(input: input)

        let e1 = "f87302eb94ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a"
        let e2 = "9c580a02806428a0ada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80a05c28dbc4"
        let e3 = "1763dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54"

        XCTAssertEqual(output.encoded.hexString, e1 + e2 + e3)
        XCTAssertEqual(output.v.hexString, "28")
        XCTAssertEqual(output.r.hexString, "ada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80")
        XCTAssertEqual(output.s.hexString, "5c28dbc41763dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54")
    }

    func testStakingRedelegate() {
        let localNet = "0x02"
        let redelegate = TW_Harmony_Proto_DirectiveRedelegate.with {
            $0.delegatorAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            $0.validatorSrcAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            $0.validatorDstAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            $0.amount = Data(hexString: "0xa")!
        }
        let input = TW_Harmony_Proto_StakingTransactionInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = Data(hexString: "4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48")!
            $0.nonce = Data(hexString: "0x2")!
            $0.gasPrice = Data(hexString: "0x")!
            $0.gasLimit = Data(hexString: "0x64")!
            $0.redelegateMessage = redelegate
        }
        let output = HarmonyStakingSigner.sign(input: input)

        let e1 = "f88903f84094ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba0"
        let e2 = "4e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a9c580a02806428"
        let e3 = "a0c479121bf1ea02fa1052a4d54743703fa6eeb16e50ff002d34fcfde736c21d75a07a1b9"
        let e4 = "dac1761ab9fb38cadcdd4b0b28aafc39d1707e913f6b758e05e09b1e517"

        XCTAssertEqual(output.encoded.hexString, e1 + e2 + e3 + e4)
        XCTAssertEqual(output.v.hexString, "28")
        XCTAssertEqual(output.r.hexString, "c479121bf1ea02fa1052a4d54743703fa6eeb16e50ff002d34fcfde736c21d75")
        XCTAssertEqual(output.s.hexString, "7a1b9dac1761ab9fb38cadcdd4b0b28aafc39d1707e913f6b758e05e09b1e517")
    }

    func testStakingUndelegate() {
        let localNet = "0x02"
        let undelegate = TW_Harmony_Proto_DirectiveUndelegate.with {
            $0.delegatorAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            $0.validatorAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            $0.amount = Data(hexString: "0xa")!
        }
        let input = TW_Harmony_Proto_StakingTransactionInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = Data(hexString: "4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48")!
            $0.nonce = Data(hexString: "0x2")!
            $0.gasPrice = Data(hexString: "0x")!
            $0.gasLimit = Data(hexString: "0x64")!
            $0.undelegateMessage = undelegate
        }
        let output = HarmonyStakingSigner.sign(input: input)

        let e1 = "f87304eb94ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a"
        let e2 = "9c580a02806427a0d6af2488d3b45658f37ff6bb89f7eaa86f7c1dfce19a68697e778be28efd2320a05b9837bd"
        let e3 = "5c7041318859f9fb444a255f32f4d7e7b49f18830ba75abecdc02390"

        XCTAssertEqual(output.encoded.hexString, e1 + e2 + e3)
        XCTAssertEqual(output.v.hexString, "27")
        XCTAssertEqual(output.r.hexString, "d6af2488d3b45658f37ff6bb89f7eaa86f7c1dfce19a68697e778be28efd2320")
        XCTAssertEqual(output.s.hexString, "5b9837bd5c7041318859f9fb444a255f32f4d7e7b49f18830ba75abecdc02390")
    }
}