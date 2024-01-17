// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class HarmonyTests: XCTestCase {
    let localNet = "0x02"

    func testAddressData() {
        let address = AnyAddress(string: "one1c8dpswxg2p50znzecnq0peuxlxtcm9je7q7yje", coin: .harmony)!

        XCTAssertEqual(address.data.hexString, "c1da1838c85068f14c59c4c0f0e786f9978d9659")
    }

    func testSigner() {
        let transaction = HarmonyTransactionMessage.with {
            $0.nonce = Data(hexString: "0x09")!
            $0.gasPrice = Data()
            $0.gasLimit = Data(hexString: "0x5208")!
            $0.fromShardID = Data(hexString: "0x01")!
            $0.toShardID = Data()
            $0.payload = Data()
            $0.toAddress = "one1d2rngmem4x2c6zxsjjz29dlah0jzkr0k2n88wc"
            $0.amount = Data(hexString: "0x4c53ecdc18a60000")!
        }
        let input = HarmonySigningInput.with {
            $0.transactionMessage = transaction
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = Data(hexString: "0xb578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e")!
        }
        let output: HarmonySigningOutput = AnySigner.sign(input: input, coin: .harmony)
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
    let blsSigData = Data(hexString: "4252b0f1210efb0d5061e8a706a7ea9d62292a7947a975472fb77e1af7278a1c3c2e6eeba73c0581ece398613829940df129f3071c9a24b4b448bb1e880dc5872a58cb07eed94294c4e01a5c864771cafef7b96be541cb3c521a98f01838dd94")!

    func testStakingCreateValidator() {
        let description = HarmonyDescription.with {
            $0.name = "Alice"
            $0.identity = "alice"
            $0.website = "alice.harmony.one"
            $0.securityContact = "Bob"
            $0.details = "Don't mess with me!!!"
        }
        let rate = HarmonyDecimal.with {
            $0.value = Data(hexString: "0x01")!
            $0.precision = Data(hexString: "0x01")!
        }
        let maxRate = HarmonyDecimal.with {
            $0.value = Data(hexString: "0x09")!
            $0.precision = Data(hexString: "0x01")!
        }
        let maxChangeRate = HarmonyDecimal.with {
            $0.value = Data(hexString: "0x05")!
            $0.precision = Data(hexString: "0x02")!
        }
        let commission = HarmonyCommissionRate.with {
            $0.rate = rate
            $0.maxRate = maxRate
            $0.maxChangeRate = maxChangeRate
        }
        let pubKey = pubKeyData
        let blsSig = blsSigData
        let createValidator = HarmonyDirectiveCreateValidator.with {
            $0.validatorAddress = oneAddress
            $0.description_p = description
            $0.commissionRates = commission
            $0.minSelfDelegation = Data(hexString: "0x0a")!
            $0.maxTotalDelegation = Data(hexString: "0x0bb8")!
            $0.slotPubKeys = [pubKey]
            $0.slotKeySigs = [blsSig]
            $0.amount = Data(hexString: "0x64")!
        }
        let staking = HarmonyStakingMessage.with {
            $0.createValidatorMessage = createValidator
            $0.nonce = Data(hexString: "0x02")!
            $0.gasPrice = Data(hexString: "0x00")!
            $0.gasLimit = Data(hexString: "0x64")!
        }
        let input = HarmonySigningInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = privateKeyData
            $0.stakingMessage = staking
        }
        let output: HarmonySigningOutput = AnySigner.sign(input: input, coin: .harmony)

        let expected = "f9015280f9010894ebcd16e8c1d8f493ba04e99a56474122d81a9c58f83885416c69636585616c69636591616c6963652e6861726d6f6e792e6f6e6583426f6295446f6e2774206d6573732077697468206d65212121ddc988016345785d8a0000c9880c7d713b49da0000c887b1a2bc2ec500000a820bb8f1b0b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b62247608611f862b8604252b0f1210efb0d5061e8a706a7ea9d62292a7947a975472fb77e1af7278a1c3c2e6eeba73c0581ece398613829940df129f3071c9a24b4b448bb1e880dc5872a58cb07eed94294c4e01a5c864771cafef7b96be541cb3c521a98f01838dd946402806428a00d8437f81be3481b01542e9baef0445f3758cf084c5e1fba93d087ccce084cb1a0404c1a42442c2d39f84582353a1c67012451ff83ef6d3622f684041df9bf0072"

        XCTAssertEqual(output.encoded.hexString, expected)
        XCTAssertEqual(output.v.hexString, "28")
        XCTAssertEqual(output.r.hexString, "0d8437f81be3481b01542e9baef0445f3758cf084c5e1fba93d087ccce084cb1")
        XCTAssertEqual(output.s.hexString, "404c1a42442c2d39f84582353a1c67012451ff83ef6d3622f684041df9bf0072")
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
            $0.value = Data(hexString: "0x01")!
            $0.precision = Data(hexString: "0x01")!
        }
        let editValidator = HarmonyDirectiveEditValidator.with {
            $0.validatorAddress = oneAddress
            $0.description_p = desc
            $0.commissionRate = commissionRate
            $0.minSelfDelegation = Data(hexString: "0x0a")!
            $0.maxTotalDelegation = Data(hexString: "0x0bb8")!
            $0.slotKeyToRemove = pubKeyData
            $0.slotKeyToAdd = pubKeyData
            $0.slotKeyToAddSig = blsSigData
            $0.active = Data(hexString: "0x01")!
        }
        let staking = HarmonyStakingMessage.with {
            $0.editValidatorMessage = editValidator
            $0.nonce = Data(hexString: "0x02")!
            $0.gasPrice = Data()
            $0.gasLimit = Data(hexString: "0x64")!
        }
        let input = HarmonySigningInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = privateKeyData
            $0.stakingMessage = staking
        }
        let output: HarmonySigningOutput = AnySigner.sign(input: input, coin: .harmony)

        let expected = "f9016c01f9012294ebcd16e8c1d8f493ba04e99a56474122d81a9c58f83885416c69636585616c69636591616c6963652e6861726d6f6e792e6f6e6583426f6295446f6e2774206d6573732077697468206d65212121c988016345785d8a00000a820bb8b0b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b62247608611b0b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b62247608611b8604252b0f1210efb0d5061e8a706a7ea9d62292a7947a975472fb77e1af7278a1c3c2e6eeba73c0581ece398613829940df129f3071c9a24b4b448bb1e880dc5872a58cb07eed94294c4e01a5c864771cafef7b96be541cb3c521a98f01838dd940102806427a089d6f87855619c31e933d5f00638ca58737dfffdfdf8b66a048a2e45f103e05da04aafc5c51a95412760c089371b411a5ab8f235b456291a9754d544b162df4eef"

        XCTAssertEqual(output.encoded.hexString, expected)
        XCTAssertEqual(output.v.hexString, "27")
        XCTAssertEqual(output.r.hexString, "89d6f87855619c31e933d5f00638ca58737dfffdfdf8b66a048a2e45f103e05d")
        XCTAssertEqual(output.s.hexString, "4aafc5c51a95412760c089371b411a5ab8f235b456291a9754d544b162df4eef")
    }

    func testStakingEditValidator2() {
        let desc = HarmonyDescription.with {
            $0.name = "Alice"
            $0.identity = "alice"
            $0.website = "alice.harmony.one"
            $0.securityContact = "Bob"
            $0.details = "Don't mess with me!!!"
        }
        let commissionRate = HarmonyDecimal.with {
            $0.value = Data(hexString: "0x01")!
            $0.precision = Data(hexString: "0x01")!
        }
        let editValidator = HarmonyDirectiveEditValidator.with {
            $0.validatorAddress = oneAddress
            $0.description_p = desc
            $0.commissionRate = commissionRate
            $0.minSelfDelegation = Data(hexString: "0x0a")!
            $0.maxTotalDelegation = Data(hexString: "0x0bb8")!
            $0.slotKeyToRemove = pubKeyData
            $0.slotKeyToAdd = pubKeyData
            $0.slotKeyToAddSig = blsSigData
            $0.active = Data(hexString: "0x02")!
        }
        let staking = HarmonyStakingMessage.with {
            $0.editValidatorMessage = editValidator
            $0.nonce = Data(hexString: "0x02")!
            $0.gasPrice = Data()
            $0.gasLimit = Data(hexString: "0x64")!
        }
        let input = HarmonySigningInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = privateKeyData
            $0.stakingMessage = staking
        }
        let output: HarmonySigningOutput = AnySigner.sign(input: input, coin: .harmony)

        let expected = "f9016c01f9012294ebcd16e8c1d8f493ba04e99a56474122d81a9c58f83885416c69636585616c69636591616c6963652e6861726d6f6e792e6f6e6583426f6295446f6e2774206d6573732077697468206d65212121c988016345785d8a00000a820bb8b0b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b62247608611b0b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b62247608611b8604252b0f1210efb0d5061e8a706a7ea9d62292a7947a975472fb77e1af7278a1c3c2e6eeba73c0581ece398613829940df129f3071c9a24b4b448bb1e880dc5872a58cb07eed94294c4e01a5c864771cafef7b96be541cb3c521a98f01838dd940202806428a09d1212ab7f54b05a4f89506ea1fe3bebecf5126544bc990b53b27f1fccb69d52a01d13704dce7399f154c2352d61ad1fc208880c267695cb423c57cd52b9821a79"
        XCTAssertEqual(output.encoded.hexString, expected)
        XCTAssertEqual(output.v.hexString, "28")
        XCTAssertEqual(output.r.hexString, "9d1212ab7f54b05a4f89506ea1fe3bebecf5126544bc990b53b27f1fccb69d52")
        XCTAssertEqual(output.s.hexString, "1d13704dce7399f154c2352d61ad1fc208880c267695cb423c57cd52b9821a79")
    }

    func testStakingEditValidator3() {
        let desc = HarmonyDescription.with {
            $0.name = "Alice"
            $0.identity = "alice"
            $0.website = "alice.harmony.one"
            $0.securityContact = "Bob"
            $0.details = "Don't mess with me!!!"
        }
        let commissionRate = HarmonyDecimal.with {
            $0.value = Data(hexString: "0x01")!
            $0.precision = Data(hexString: "0x01")!
        }
        let editValidator = HarmonyDirectiveEditValidator.with {
            $0.validatorAddress = oneAddress
            $0.description_p = desc
            $0.commissionRate = commissionRate
            $0.minSelfDelegation = Data(hexString: "0x0a")!
            $0.maxTotalDelegation = Data(hexString: "0x0bb8")!
            $0.slotKeyToRemove = pubKeyData
            $0.slotKeyToAdd = pubKeyData
            $0.slotKeyToAddSig = blsSigData
            $0.active = Data()
        }
        let staking = HarmonyStakingMessage.with {
            $0.editValidatorMessage = editValidator
            $0.nonce = Data(hexString: "0x02")!
            $0.gasPrice = Data()
            $0.gasLimit = Data(hexString: "0x64")!
        }
        let input = HarmonySigningInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = privateKeyData
            $0.stakingMessage = staking
        }
        let output: HarmonySigningOutput = AnySigner.sign(input: input, coin: .harmony)

        let expected = "f9016c01f9012294ebcd16e8c1d8f493ba04e99a56474122d81a9c58f83885416c69636585616c69636591616c6963652e6861726d6f6e792e6f6e6583426f6295446f6e2774206d6573732077697468206d65212121c988016345785d8a00000a820bb8b0b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b62247608611b0b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b62247608611b8604252b0f1210efb0d5061e8a706a7ea9d62292a7947a975472fb77e1af7278a1c3c2e6eeba73c0581ece398613829940df129f3071c9a24b4b448bb1e880dc5872a58cb07eed94294c4e01a5c864771cafef7b96be541cb3c521a98f01838dd948002806427a02f160fbf125b614764f9d45dc20acc63da41b02f380e20135e72bc7e74ab3205a011a1d8824a871361e2817145e87148b072c378c38d037df482214a3e4e3f2205"
        XCTAssertEqual(output.encoded.hexString, expected)
        XCTAssertEqual(output.v.hexString, "27")
        XCTAssertEqual(output.r.hexString, "2f160fbf125b614764f9d45dc20acc63da41b02f380e20135e72bc7e74ab3205")
        XCTAssertEqual(output.s.hexString, "11a1d8824a871361e2817145e87148b072c378c38d037df482214a3e4e3f2205")
    }


    func testStakingDelegate() {
        let delegate = HarmonyDirectiveDelegate.with {
            $0.delegatorAddress = oneAddress
            $0.validatorAddress = oneAddress
            $0.amount = Data(hexString: "0x0a")!
        }
        let staking = HarmonyStakingMessage.with {
            $0.delegateMessage = delegate
            $0.nonce = Data(hexString: "0x02")!
            $0.gasPrice = Data()
            $0.gasLimit = Data(hexString: "0x64")!
        }
        let input = HarmonySigningInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = privateKeyData
            $0.stakingMessage = staking
        }
        let output: HarmonySigningOutput = AnySigner.sign(input: input, coin: .harmony)

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
            $0.amount = Data(hexString: "0x0a")!
        }
        let staking = HarmonyStakingMessage.with {
            $0.undelegateMessage = undelegate
            $0.nonce = Data(hexString: "0x02")!
            $0.gasPrice = Data()
            $0.gasLimit = Data(hexString: "0x64")!
        }
        let input = HarmonySigningInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = privateKeyData
            $0.stakingMessage = staking
        }
        let output: HarmonySigningOutput = AnySigner.sign(input: input, coin: .harmony)

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
            $0.nonce = Data(hexString: "0x02")!
            $0.gasPrice = Data()
            $0.gasLimit = Data(hexString: "0x64")!
        }
        let input = HarmonySigningInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = privateKeyData
            $0.stakingMessage = staking
        }
        let output: HarmonySigningOutput = AnySigner.sign(input: input, coin: .harmony)

        let expected = "f85d04d594ebcd16e8c1d8f493ba04e99a56474122d81a9c5802806428a04c15c72f425"
            + "77001083a9c7ff9d9724077aec704a524e53dc7c9afe97ca4e625a055c13ea17c3efd1cd9"
            + "1f2988c7e7673950bac5a08c174f2d0af27a82039f1e3d"

        XCTAssertEqual(output.encoded.hexString, expected)
        XCTAssertEqual(output.v.hexString, "28")
        XCTAssertEqual(output.r.hexString, "4c15c72f42577001083a9c7ff9d9724077aec704a524e53dc7c9afe97ca4e625")
        XCTAssertEqual(output.s.hexString, "55c13ea17c3efd1cd91f2988c7e7673950bac5a08c174f2d0af27a82039f1e3d")
    }

    func testSignJSON() {
        let json = """
            {"chainId":"Ag==","transactionMessage":{"nonce":"AQ==","gasPrice":"AA==","gasLimit":"Ugg=","toAddress":"one129r9pj3sk0re76f7zs3qz92rggmdgjhtwge62k","amount":"Br/I2l7oIgAA","fromShardId":"AQ==","toShardId":"AA=="}}
        """
        let key = Data(hexString: "4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48")!
        let result = AnySigner.signJSON(json, key: key, coin: .harmony)

        XCTAssertEqual(result, "f86a0180825208018094514650ca30b3c79f693e14220115434236d44aeb8906bfc8da5ee82200008028a084cc200aab11f5e1b2f7ece0d56ec67385ac50cefb6e3dc2a2f3e036ed575a5ca0643f18005b790cac8d8e7dc90e6147df0b83874b52db198864694ea28a79e6fc")
    }
}
