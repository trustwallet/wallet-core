// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

extension Bech32Address: Address, Equatable {
    public static func isValid(data: Data) -> Bool {
        return data.count >= 2 && data.count <= 40
    }
}
