// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import Foundation

public extension Mnemonic {
    typealias ValidationResult = (word: String, index: Int)

    /// Returns mnemonic validation result, an array of wrong word and index tuple
    static func validate(mnemonic: [String]) -> [ValidationResult] {
        mnemonic.enumerated().compactMap { (index, word) -> ValidationResult? in
            if isValidWord(word: word) {
                return nil
            }
            return (word, index)
        }
    }

    /// Returns matched suggestion in a native array
    static func search(prefix: String) -> [String] {
        return suggest(prefix: prefix).split(separator: " ").map { String($0) }
    }
}
