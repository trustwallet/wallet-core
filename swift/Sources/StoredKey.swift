// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public final class StoredKey {

    public static func load(path: String) -> StoredKey? {
        let pathString = TWStringCreateWithNSString(path)
        defer {
            TWStringDelete(pathString)
        }
        guard let value = TWStoredKeyLoad(pathString) else {
            return nil
        }
        return StoredKey(rawValue: value)
    }

    public static func importPrivateKey(privateKey: Data, password: String, coin: CoinType) -> StoredKey {
        let privateKeyData = TWDataCreateWithNSData(privateKey)
        defer {
            TWDataDelete(privateKeyData)
        }
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        return StoredKey(rawValue: TWStoredKeyImportPrivateKey(privateKeyData, passwordString, TWCoinType(rawValue: coin.rawValue)))
    }

    public static func importHDWallet(mnemonic: String, password: String, coin: CoinType) -> StoredKey {
        let mnemonicString = TWStringCreateWithNSString(mnemonic)
        defer {
            TWStringDelete(mnemonicString)
        }
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        return StoredKey(rawValue: TWStoredKeyImportHDWallet(mnemonicString, passwordString, TWCoinType(rawValue: coin.rawValue)))
    }

    public static func importJSON(json: Data) -> StoredKey? {
        let jsonData = TWDataCreateWithNSData(json)
        defer {
            TWDataDelete(jsonData)
        }
        guard let value = TWStoredKeyImportJSON(jsonData) else {
            return nil
        }
        return StoredKey(rawValue: value)
    }

    public var identifier: String? {
        guard let result = TWStoredKeyIdentifier(rawValue) else {
            return nil
        }
        return TWStringNSString(result)
    }

    public var isMnemonic: Bool {
        return TWStoredKeyIsMnemonic(rawValue)
    }

    public var accountCount: Int {
        return TWStoredKeyAccountCount(rawValue)
    }

    let rawValue: OpaquePointer

    init(rawValue: OpaquePointer) {
        self.rawValue = rawValue
    }

    public init(password: String) {
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        rawValue = TWStoredKeyCreate(passwordString)
    }

    deinit {
        TWStoredKeyDelete(rawValue)
    }

    public func account(index: Int) -> Account? {
        guard let value = TWStoredKeyAccount(rawValue, index) else {
            return nil
        }
        return Account(rawValue: value)
    }

    public func accountForCoin(coin: CoinType, wallet: HDWallet?) -> Account? {
        guard let value = TWStoredKeyAccountForCoin(rawValue, TWCoinType(rawValue: coin.rawValue), wallet?.rawValue) else {
            return nil
        }
        return Account(rawValue: value)
    }

    public func addAccount(address: String, derivationPath: String, extetndedPublicKey: String) -> Void {
        let addressString = TWStringCreateWithNSString(address)
        defer {
            TWStringDelete(addressString)
        }
        let derivationPathString = TWStringCreateWithNSString(derivationPath)
        defer {
            TWStringDelete(derivationPathString)
        }
        let extetndedPublicKeyString = TWStringCreateWithNSString(extetndedPublicKey)
        defer {
            TWStringDelete(extetndedPublicKeyString)
        }
        return TWStoredKeyAddAccount(rawValue, addressString, derivationPathString, extetndedPublicKeyString)
    }

    public func store(path: String) -> Bool {
        let pathString = TWStringCreateWithNSString(path)
        defer {
            TWStringDelete(pathString)
        }
        return TWStoredKeyStore(rawValue, pathString)
    }

    public func decryptPrivateKey(password: String) -> Data? {
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        guard let result = TWStoredKeyDecryptPrivateKey(rawValue, passwordString) else {
            return nil
        }
        return TWDataNSData(result)
    }

    public func decryptMnemonic(password: String) -> String? {
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        guard let result = TWStoredKeyDecryptMnemonic(rawValue, passwordString) else {
            return nil
        }
        return TWStringNSString(result)
    }

    public func privateKey(coin: CoinType, password: String) -> PrivateKey? {
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        guard let value = TWStoredKeyPrivateKey(rawValue, TWCoinType(rawValue: coin.rawValue), passwordString) else {
            return nil
        }
        return PrivateKey(rawValue: value)
    }

    public func wallet(password: String) -> HDWallet? {
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        guard let value = TWStoredKeyWallet(rawValue, passwordString) else {
            return nil
        }
        return HDWallet(rawValue: value)
    }

    public func exportJSON() -> Data? {
        guard let result = TWStoredKeyExportJSON(rawValue) else {
            return nil
        }
        return TWDataNSData(result)
    }

    public func fixAddresses(password: String) -> Bool {
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        return TWStoredKeyFixAddresses(rawValue, passwordString)
    }

}
