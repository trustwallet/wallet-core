// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public class Tezos: Blockchain {
  /// Coin type for Level 2 of BIP44.
  ///
  /// - SeeAlso: https://github.com/satoshilabs/slips/blob/master/slip-0044.md
  public override var coinType: CoinType {
    return .tezos
  }

  /// Returns the address associated with a public key.
  public override func address(for publicKey: PublicKey) -> Address {
    fatalError("Not implemented.")
  }

  /// Returns the address given its string representation.
  public override func address(string: String) -> Address? {
    fatalError("Not implemented.")
  }

  /// Returns the address given its raw representation.
  public override func address(data: Data) -> Address? {
    fatalError("Not implemented.")
  }
}
