// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

/// Generic Address protocol for AnyAddress / SegwitAddress / SolanaAddress
public protocol Address: CustomStringConvertible {}

extension AnyAddress: Equatable {}
