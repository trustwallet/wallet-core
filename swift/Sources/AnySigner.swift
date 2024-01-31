// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation
import SwiftProtobuf

public typealias SigningInput = Message
public typealias SigningOutput = Message

// TANGEM
public protocol Signer {
    var error: Error? { get }

    var publicKey: Data { get }

    func sign(_ data: Data) -> Data
    func sign(_ data: [Data]) -> [Data]
}

// TANGEM
// For auto tests only
public struct PrivateKeySigner: Signer {
    public var error: Error?

    public var publicKey: Data {
        privateKey.getPublicKey(coinType: coin).data
    }

    private let privateKey: PrivateKey
    private let coin: CoinType

    public init(privateKey: PrivateKey, coin: CoinType) {
        self.privateKey = privateKey
        self.coin = coin
    }

    public func sign(_ data: Data) -> Data {
        return privateKey.sign(digest: data, curve: coin.curve)!
    }

    public func sign(_ data: [Data]) -> [Data] {
        return data.map { privateKey.sign(digest: $0, curve: coin.curve)! }
    }
}

// TANGEM
// We can't capture a local variable in a closure we're passing to std::function. Global variables are fine.
var externalSigner: Signer?

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
            return try SigningOutput(serializedData: outputData)
        } catch let error {
            fatalError(error.localizedDescription)
        }
    }

    // TANGEM
    public static func signExternally<SigningOutput: Message>(input: SigningInput, coin: CoinType, signer: Signer) throws -> SigningOutput {
        defer {
            externalSigner = nil
        }

        externalSigner = signer

        // It's safe to use "try!" here because the original code just catches exceptions and calls "fatalError"
        let outputData = nativeSignExternally(data: try! input.serializedData(), coin: coin, publicKey: signer.publicKey)
        // swiftlint:disable:previous force_try
        if let error = signer.error {
            throw error
        }

        // swiftlint:disable:next force_try
        return try! SigningOutput(serializedData: outputData)
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

    // TANGEM
    public static func nativeSignExternally(data: Data, coin: CoinType, publicKey: Data) -> Data {
        let inputData = TWDataCreateWithNSData(data)
        let publicKeyData = TWDataCreateWithNSData(publicKey)

        defer {
            TWDataDelete(inputData)
            TWDataDelete(publicKeyData)
        }

        return TWDataNSData(TWAnySignerSignExternally(inputData, TWCoinType(rawValue: coin.rawValue), publicKeyData, { twDataToSign in
            guard let externalSigner = externalSigner else {
                fatalError("You must set external signer to sign asynchronously")
            }

            let dataToSign = TWDataNSData(twDataToSign)
            let dataSigned = externalSigner.sign(dataToSign)
            return TWDataCreateWithNSData(dataSigned)
        }))
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
            return try TransactionPlan(serializedData: outputData)
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
