// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

/// Tezos blockchain.
open class Tezos: Blockchain {
//  /// Chain identifier.
//  open var chainID: Int {
//    return 1
//  }
//
  /// SLIP-044 coin type.
  open override var coinType: CoinType {
    return .tezos
  }

//  open override func address(for publicKey: PublicKey) -> Address {
//    return EthereumAddress(publicKey: publicKey)
//  }
//
//  open override func address(string: String) -> Address? {
//    return EthereumAddress(string: string)
//  }
//
//  open override func address(data: Data) -> Address? {
//    return EthereumAddress(keyHash: data)
//  }
//}
}
