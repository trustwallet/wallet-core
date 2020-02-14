// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation
import SwiftProtobuf

public typealias SigningInput = Message

public final class AnySigner {
    public static func sign<SigningOutput: Message>(input: SigningInput, coin: CoinType) -> SigningOutput {
        do {
            let inputData = TWDataCreateWithNSData(try input.serializedData())
            defer {
                TWDataDelete(inputData)
            }
            let outputData = TWDataNSData(TWAnySignerSign(inputData, TWCoinType(rawValue: coin.rawValue)))
            return try SigningOutput(serializedData: outputData)
        } catch let error {
            fatalError(error.localizedDescription)
        }
    }
}
