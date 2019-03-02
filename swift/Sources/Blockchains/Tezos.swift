// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

/// Tezos blockchain.
open class Tezos: Blockchain {
  open override var coinType: CoinType {
    return .tezos
  }

  /// Returns the address associated with a public key.
  open override func address(for publicKey: PublicKey) -> Address {
    fatalError("TODO(keefertaylor): Implement.")
  }

  /// Returns the address given its string representation.
  open override func address(string: String) -> Address? {
    fatalError("TODO(keefertaylor): Implement.")
  }

  /// Returns the address given its raw representation.
  open override func address(data: Data) -> Address? {
    fatalError("TODO(keefertaylor): Implement.")
  }
}
