// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation
import SwiftProtobuf

public typealias SigningInput = Message
public typealias SigningOutput = Message

public final class AnySigner {
    public static func sign<SigningOutput: Message>(input: SigningInput, coin: CoinType) -> SigningOutput {
        do {
            let outputData = nativeSign(data: try input.serializedData(), coin: coin)
            return try SigningOutput(serializedData: outputData)
        } catch let error {
            fatalError(error.localizedDescription)
        }
    }

    public static func nativeSign(data: Data, coin: CoinType) -> Data {
        let inputData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(inputData)
        }
        return TWDataNSData(TWAnySignerSign(inputData, TWCoinType(rawValue: coin.rawValue)))
    }

    public static func supportsJSON(coin: CoinType) -> Bool {
        return TWAnySignerSupportsJSON(TWCoinType(rawValue: coin.rawValue))
    }

    public static func signJSON(_ json: String, key: Data, coin: CoinType) -> String {
        let jsonString = TWStringCreateWithNSString(json)
        let keyData = TWDataCreateWithNSData(key)
        defer {
            TWDataDelete(keyData)
        }
        return TWStringNSString(TWAnySignerSignJSON(jsonString, keyData, TWCoinType(rawValue: coin.rawValue)))
    }

    public static func plan<TransactionPlan: Message>(input: SigningInput, coin: CoinType) -> TransactionPlan {
        do {
            let outputData = nativePlan(data: try input.serializedData(), coin: coin)
            return try TransactionPlan(serializedData: outputData)
        } catch let error {
            fatalError(error.localizedDescription)
        }
    }

    public static func nativePlan(data: Data, coin: CoinType) -> Data {
        let inputData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(inputData)
        }
        return TWDataNSData(TWAnySignerPlan(inputData, TWCoinType(rawValue: coin.rawValue)))
    }
}
