// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public class Tezos: Blockchain {
  public override var coinType: CoinType {
    return .tezos
  }

  public override func address(for publicKey: PublicKey) -> Address {
    fatalError("Not implemented.")
  }

  public override func address(string: String) -> Address? {
    fatalError("Not implemented.")
  }

  public override func address(data: Data) -> Address? {
    fatalError("Not implemented.")
  }
}
