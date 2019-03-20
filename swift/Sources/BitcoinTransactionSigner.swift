// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public final class BitcoinTransactionSigner {

    let rawValue: OpaquePointer

    init(rawValue: OpaquePointer) {
        self.rawValue = rawValue
    }

    public init(input: TW_Bitcoin_Proto_SigningInput) {
        let inputData = TWDataCreateWithNSData(try! input.serializedData())
        defer {
            TWDataDelete(inputData)
        }
        rawValue = TWBitcoinTransactionSignerCreate(inputData)
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
        rawValue = TWBitcoinTransactionSignerCreateWithPlan(inputData, planData)
    }

    deinit {
        TWBitcoinTransactionSignerDelete(rawValue)
    }

    public func plan() -> TW_Bitcoin_Proto_TransactionPlan {
        let resultData = TWDataNSData(TWBitcoinTransactionSignerPlan(rawValue))
        return try! TW_Bitcoin_Proto_TransactionPlan(serializedData: resultData)
    }

    public func sign() -> TW_Proto_Result {
        let resultData = TWDataNSData(TWBitcoinTransactionSignerSign(rawValue))
        return try! TW_Proto_Result(serializedData: resultData)
    }

}
