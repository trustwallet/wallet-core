// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

/// Ethereum blockchain.
///
/// Ethereum-based blockchains should inherit from this class.
open class Ethereum: Blockchain {
    /// Chain identifier.
    open var chainID: Int {
        return 1
    }

    /// SLIP-044 coin type.
    open override var coinType: CoinType {
        return .ethereum
    }

    open override func address(for publicKey: PublicKey) -> Address {
        return EthereumAddress(publicKey: publicKey)
    }

    open override func address(string: String) -> Address? {
        return EthereumAddress(string: string)
    }

    open override func address(data: Data) -> Address? {
        return EthereumAddress(keyHash: data)
    }
}

public final class Go: Ethereum {
    public override var chainID: Int {
        return 60
    }

    public override var coinType: CoinType {
        return .go
    }
}

public final class POA: Ethereum {
    public override var chainID: Int {
        return 99
    }

    public override var coinType: CoinType {
        return .poa
    }
}

public final class Callisto: Ethereum {
    public override var chainID: Int {
        return 820
    }

    public override var coinType: CoinType {
        return .callisto
    }
}

public final class EthereumClassic: Ethereum {
    public override var chainID: Int {
        return 61
    }

    public override var coinType: CoinType {
        return .ethereumClassic
    }
}

public final class Vechain: Ethereum {
    public override var chainID: Int {
        return 74
    }

    public override var coinType: CoinType {
        return .veChain
    }
}

public final class ThunderToken: Ethereum {
    public override var chainID: Int {
        return 18
    }

    public override var coinType: CoinType {
        return .thunderToken
    }
}

public final class TomoChain: Ethereum {
    public override var chainID: Int {
        return 88
    }

    public override var coinType: CoinType {
        return .tomoChain
    }
}
