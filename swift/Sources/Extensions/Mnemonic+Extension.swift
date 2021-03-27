// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
