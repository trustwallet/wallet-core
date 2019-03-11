// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

/// Creates a blockchain for a specific SLIP-0044 coin type.
public func blockchain(coin: CoinType) -> Blockchain {
    switch coin {
    case .bitcoin:
        return Bitcoin()
    case .litecoin:
        return Litecoin()
    case .ethereum:
        return Ethereum()
    case .ethereumClassic:
        return EthereumClassic()
    case .go:
        return Go()
    case .thunderToken:
        return ThunderToken()
    case .poa:
        return POA()
    case .tron:
        return Tron()
    case .veChain:
        return Vechain()
    case .callisto:
        return Callisto()
    case .wanChain:
        return Wanchain()
    case .xdai:
        return XDai();
    case .dash:
        return Dash()
    case .icon:
        return Icon()
    case .bitcoinCash:
        return BitcoinCash()
    case .tomoChain:
        return TomoChain()
    case .zcoin:
        return Zcoin()
    case .binance:
        return BinanceChainTestnet()
    case .ripple:
        return Ripple()
    case .zcash:
        return Zcash()
    case .tezos:
        return Tezos()
    case .nimiq:
        return Nimq()
    case .stellar:
        return Stellar()
    case .aion:
        return Aion()
    }
}
