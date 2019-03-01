// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public final class ZcashTransactionSigner {

    let rawValue: OpaquePointer

    init(rawValue: OpaquePointer) {
        self.rawValue = rawValue
    }

    public init(input: TW_Bitcoin_Proto_SigningInput) {
        let inputData = TWDataCreateWithNSData(try! input.serializedData())
        defer {
            TWDataDelete(inputData)
        }
        rawValue = TWZcashTransactionSignerCreate(inputData)
    }

    public init(input: TW_Bitcoin_Proto_SigningInput, plan: TW_Bitcoin_Proto_TransactionPlan) {
        let inputData = TWDataCreateWithNSData(try! input.serializedData())
        defer {
            TWDataDelete(inputData)
        }
        let planData = TWDataCreateWithNSData(try! plan.serializedData())
        defer {
            TWDataDelete(planData)
        }
        rawValue = TWZcashTransactionSignerCreateWithPlan(inputData, planData)
    }

    deinit {
        TWZcashTransactionSignerDelete(rawValue)
    }

    public func plan() -> TW_Bitcoin_Proto_TransactionPlan {
        let resultData = TWDataNSData(TWZcashTransactionSignerPlan(rawValue))
        return try! TW_Bitcoin_Proto_TransactionPlan(serializedData: resultData)
    }

    public func sign() -> TW_Proto_Result {
        let resultData = TWDataNSData(TWZcashTransactionSignerSign(rawValue))
        return try! TW_Proto_Result(serializedData: resultData)
    }

}
