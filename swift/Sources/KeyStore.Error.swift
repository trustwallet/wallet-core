// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

extension KeyStore {
    public enum Error: Swift.Error, LocalizedError {
        case accountNotFound
        case invalidMnemonic
        case invalidJSON
        case invalidKey
        case invalidPassword
        case storageFailed

        public var errorDescription: String? {
            switch self {
            case .accountNotFound:
                return NSLocalizedString("Account not found", comment: "Error message when trying to access an account that does not exist")
            case .invalidMnemonic:
                return NSLocalizedString("Invalid mnemonic phrase", comment: "Error message when trying to import an invalid mnemonic phrase")
            case .invalidJSON:
                return NSLocalizedString("Invalid Keystore/JSON format", comment: "Error message when trying to import an invalid json phrase")
            case .invalidKey:
                return NSLocalizedString("Invalid private key", comment: "Error message when trying to import an invalid private key")
            case .invalidPassword:
                return NSLocalizedString("Invalid password", comment: "Error message when trying to export a private key")
            case .storageFailed:
                return NSLocalizedString("Failed to save keystore file", comment: "Error message when the keystore file cannot be written to disk. Please check available disk space and directory permissions")
            }
        }
    }
}
