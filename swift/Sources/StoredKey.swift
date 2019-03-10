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

    public static func load(path: String, password: String) -> StoredKey? {
        let pathString = TWStringCreateWithNSString(path)
        defer {
            TWStringDelete(pathString)
        }
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        guard let value = TWStoredKeyLoad(pathString, passwordString) else {
            return nil
        }
        return StoredKey(rawValue: value)
    }

    public static func importPrivateKey(privateKey: Data, password: String, coin: CoinType) -> StoredKey? {
        let privateKeyData = TWDataCreateWithNSData(privateKey)
        defer {
            TWDataDelete(privateKeyData)
        }
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        guard let value = TWStoredKeyImportPrivateKey(privateKeyData, passwordString, TWCoinType(rawValue: coin.rawValue)) else {
            return nil
        }
        return StoredKey(rawValue: value)
    }

    public static func importHDWallet(mnemonic: String, password: String, derivationPath: String) -> StoredKey? {
        let mnemonicString = TWStringCreateWithNSString(mnemonic)
        defer {
            TWStringDelete(mnemonicString)
        }
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        let derivationPathString = TWStringCreateWithNSString(derivationPath)
        defer {
            TWStringDelete(derivationPathString)
        }
        guard let value = TWStoredKeyImportHDWallet(mnemonicString, passwordString, derivationPathString) else {
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

    public func store(path: String, password: String) -> Bool {
        let pathString = TWStringCreateWithNSString(path)
        defer {
            TWStringDelete(pathString)
        }
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        return TWStoredKeyStore(rawValue, pathString, passwordString)
    }

    public func exportPrivateKey(password: String) -> Data {
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        return TWDataNSData(TWStoredKeyExportPrivateKey(rawValue, passwordString))
    }

    public func exportMnemonic(password: String) -> String {
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        return TWStringNSString(TWStoredKeyExportMnemonic(rawValue, passwordString))
    }

}
