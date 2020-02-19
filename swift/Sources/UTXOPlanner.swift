// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation
import SwiftProtobuf

public final class UTXOPlanner {
    public static func plan<TransactionPlan: Message>(input: SigningInput, coin: CoinType) -> TransactionPlan {
        do {
            let inputData = TWDataCreateWithNSData(try input.serializedData())
            defer {
                TWDataDelete(inputData)
            }
            let outputData = TWDataNSData(TWUTXOPlannerPlan(inputData, TWCoinType(rawValue: coin.rawValue)))
            return try TransactionPlan(serializedData: outputData)
        } catch let error {
            fatalError(error.localizedDescription)
        }
    }
}
