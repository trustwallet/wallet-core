// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public final class BitcoinScript {

    public static func == (lhs: BitcoinScript, rhs: BitcoinScript) -> Bool {
        return TWBitcoinScriptEqual(lhs.rawValue, rhs.rawValue)
    }

    public static func buildPayToPublicKeyHash(hash: Data) -> BitcoinScript {
        let hashData = TWDataCreateWithNSData(hash)
        defer {
            TWDataDelete(hashData)
        }
        return BitcoinScript(rawValue: TWBitcoinScriptBuildPayToPublicKeyHash(hashData))
    }

    public static func buildPayToScriptHash(scriptHash: Data) -> BitcoinScript {
        let scriptHashData = TWDataCreateWithNSData(scriptHash)
        defer {
            TWDataDelete(scriptHashData)
        }
        return BitcoinScript(rawValue: TWBitcoinScriptBuildPayToScriptHash(scriptHashData))
    }

    public static func buildPayToWitnessPubkeyHash(hash: Data) -> BitcoinScript {
        let hashData = TWDataCreateWithNSData(hash)
        defer {
            TWDataDelete(hashData)
        }
        return BitcoinScript(rawValue: TWBitcoinScriptBuildPayToWitnessPubkeyHash(hashData))
    }

    public static func buildPayToWitnessScriptHash(scriptHash: Data) -> BitcoinScript {
        let scriptHashData = TWDataCreateWithNSData(scriptHash)
        defer {
            TWDataDelete(scriptHashData)
        }
        return BitcoinScript(rawValue: TWBitcoinScriptBuildPayToWitnessScriptHash(scriptHashData))
    }

    public static func buildForAddress(address: String) -> BitcoinScript {
        let addressString = TWStringCreateWithNSString(address)
        defer {
            TWStringDelete(addressString)
        }
        return BitcoinScript(rawValue: TWBitcoinScriptBuildForAddress(addressString))
    }

    public var size: Int {
        return TWBitcoinScriptSize(rawValue)
    }

    public var data: Data {
        return TWDataNSData(TWBitcoinScriptData(rawValue))
    }

    public var scriptHash: Data {
        return TWDataNSData(TWBitcoinScriptScriptHash(rawValue))
    }

    public var isPayToScriptHash: Bool {
        return TWBitcoinScriptIsPayToScriptHash(rawValue)
    }

    public var isPayToWitnessScriptHash: Bool {
        return TWBitcoinScriptIsPayToWitnessScriptHash(rawValue)
    }

    public var isWitnessProgram: Bool {
        return TWBitcoinScriptIsWitnessProgram(rawValue)
    }

    let rawValue: OpaquePointer

    init(rawValue: OpaquePointer) {
        self.rawValue = rawValue
    }

    public init() {
        rawValue = TWBitcoinScriptCreate()
    }

    public init(data: Data) {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        rawValue = TWBitcoinScriptCreateWithData(dataData)
    }

    public init(script: BitcoinScript) {
        rawValue = TWBitcoinScriptCreateCopy(script.rawValue)
    }

    deinit {
        TWBitcoinScriptDelete(rawValue)
    }

    public func matchPayToPubkey() -> Data? {
        guard let result = TWBitcoinScriptMatchPayToPubkey(rawValue) else {
            return nil
        }
        return TWDataNSData(result)
    }

    public func matchPayToPubkeyHash() -> Data? {
        guard let result = TWBitcoinScriptMatchPayToPubkeyHash(rawValue) else {
            return nil
        }
        return TWDataNSData(result)
    }

    public func matchPayToScriptHash() -> Data? {
        guard let result = TWBitcoinScriptMatchPayToScriptHash(rawValue) else {
            return nil
        }
        return TWDataNSData(result)
    }

    public func matchPayToWitnessPublicKeyHash() -> Data? {
        guard let result = TWBitcoinScriptMatchPayToWitnessPublicKeyHash(rawValue) else {
            return nil
        }
        return TWDataNSData(result)
    }

    public func matchPayToWitnessScriptHash() -> Data? {
        guard let result = TWBitcoinScriptMatchPayToWitnessScriptHash(rawValue) else {
            return nil
        }
        return TWDataNSData(result)
    }

    public func encode() -> Data {
        return TWDataNSData(TWBitcoinScriptEncode(rawValue))
    }

}
