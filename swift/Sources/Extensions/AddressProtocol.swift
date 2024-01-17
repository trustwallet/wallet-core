// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import Foundation

/// Generic Address protocol for AnyAddress / SegwitAddress / SolanaAddress
public protocol Address: CustomStringConvertible {}

extension AnyAddress: Equatable {}
