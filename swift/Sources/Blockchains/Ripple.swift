// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public typealias TWRippleSigningInput = TW_Ripple_Proto_SigningInput

extension RippleAddress: Address {
    public var data: Data {
        return keyHash
    }
}
