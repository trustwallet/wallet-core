// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation
import WalletCoreSwiftProtobuf

public typealias SigningInput = Message
public typealias SigningOutput = Message

/// Represents a signer to sign transactions for any blockchain.
public final class AnySigner {

    /// Signs a transaction by SigningInput message and coin type
    ///
    /// - Parameters:
    /// - input: The generic SigningInput SwiftProtobuf message
    /// - coin: CoinType
    /// - Returns: The generic SigningOutput SwiftProtobuf message
    public static func sign<SigningOutput: Message>(input: SigningInput, coin: CoinType) -> SigningOutput {
        do {
            let outputData = nativeSign(data: try input.serializedData(), coin: coin)
            return try SigningOutput(serializedBytes: outputData)
        } catch let error {
            fatalError(error.localizedDescription)
        }
    }

    /// Signs a transaction by serialized data of a SigningInput and coin type
    ///
    /// - Parameters:
    /// - data: The serialized data of a SigningInput
    /// - coin: CoinType
    /// - Returns: The serialized data of a SigningOutput
    public static func nativeSign(data: Data, coin: CoinType) -> Data {
        let inputData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(inputData)
        }
        return TWDataNSData(TWAnySignerSign(inputData, TWCoinType(rawValue: coin.rawValue)))
    }

    /// Check if AnySigner supports signing JSON representation of SigningInput for a given coin.
    public static func supportsJSON(coin: CoinType) -> Bool {
        return TWAnySignerSupportsJSON(TWCoinType(rawValue: coin.rawValue))
    }

    /// Signs a transaction specified by the JSON representation of a SigningInput, coin type and a private key
    ///
    /// - Parameters:
    /// - json: JSON representation of a SigningInput
    /// - key: The private key data
    /// - coin: CoinType
    /// - Returns: The JSON representation of a SigningOutput.
    public static func signJSON(_ json: String, key: Data, coin: CoinType) -> String {
        let jsonString = TWStringCreateWithNSString(json)
        let keyData = TWDataCreateWithNSData(key)
        defer {
            TWDataDelete(keyData)
        }
        return TWStringNSString(TWAnySignerSignJSON(jsonString, keyData, TWCoinType(rawValue: coin.rawValue)))
    }

    /// Plans a transaction (for UTXO chains only).
    ///
    /// - Parameters:
    /// - input: The generic SigningInput SwiftProtobuf message
    /// - coin: CoinType
    /// - Returns: TransactionPlan SwiftProtobuf message
    public static func plan<TransactionPlan: Message>(input: SigningInput, coin: CoinType) -> TransactionPlan {
        do {
            let outputData = nativePlan(data: try input.serializedData(), coin: coin)
            return try TransactionPlan(serializedBytes: outputData)
        } catch let error {
            fatalError(error.localizedDescription)
        }
    }

    /// Plans a transaction (for UTXO chains only).
    ///
    /// - Parameters:
    /// - input: The serialized data of a SigningInput
    /// - coin: CoinType
    /// - Returns: The serialized data of a TransactionPlan
    public static func nativePlan(data: Data, coin: CoinType) -> Data {
        let inputData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(inputData)
        }
        return TWDataNSData(TWAnySignerPlan(inputData, TWCoinType(rawValue: coin.rawValue)))
    }
}
