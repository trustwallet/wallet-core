// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public extension CoinType {
    /// Converts a string to an address for this coin type.
    func address(string: String) -> Address? {
        switch self {
        case .binance, .cosmos:
            if let addr = CosmosAddress(string: string), addr.hrp == hrp { return addr }
        case .bitcoin, .litecoin, .viacoin, .qtum:
            if let addr = SegwitAddress(string: string), addr.hrp == hrp {
                return addr
            } else if let addr = BitcoinAddress(string: string), prefixSet.contains(addr.prefix) { return addr }
        case .bitcoinCash:
            if let addr = BitcoinCashAddress(string: string) {
                return addr
            } else if let addr = BitcoinAddress(string: string), prefixSet.contains(addr.prefix) { return addr }
        case .dash, .dogecoin, .zcoin, .lux:
            if let addr = BitcoinAddress(string: string), prefixSet.contains(addr.prefix) { return addr }
        case .callisto,
             .ellaism,
             .ethereum,
             .ethereumClassic,
             .ethersocial,
             .goChain,
             .poanetwork,
             .theta,
             .thunderToken,
             .tomoChain,
             .veChain,
             .xdai,
             .dexon:
            return EthereumAddress(string: string)
        case .wanchain:
            return WanchainAddress(string: string)
        case .icon:
            return IconAddress(string: string)
        case .ontology:
            return OntologyAddress(string: string)
        case .xrp:
            return RippleAddress(string: string)
        case .tezos:
            return TezosAddress(string: string)
        case .tron:
            return TronAddress(string: string)
        case .zelcash,
             .zcash:
            return ZcashTAddress(string: string)
        case .nimiq:
            return NimiqAddress(string: string)
        case .stellar, .kin:
            return StellarAddress(string: string)
        case .aion:
            return AionAddress(string: string)
        case .nano:
            return NanoAddress(string: string)
        case .neo:
            return NEOAddress(string: string)
        case .decred:
            return DecredAddress(string: string)
        case .iocoin:
            return IocoinAddress(string: string)
        case .groestlcoin:
            if let addr = SegwitAddress(string: string), addr.hrp == hrp {
                return addr
            } else {
                return GroestlcoinAddress(string: string)
            }
        case .iost:
            return IOSTAccount(string: string)
        case .nuls:
            return NULSAddress(string: string)
        case .bravoCoin:
            return BravoAddress(string: string)
        case .steem:
            return SteemAddress(string: string)
        case .eos:
            return EOSAddress(string: string)
        case .ioTeX:
            return IoTeXAddress(string: string)
        case .zilliqa:
            return ZilliqaAddress(string: string)
        case .semux:
            return SemuxAddress(string: string)
        case .ark:
            return ARKAddress(string: string)
        }
        return .none
    }

    /// Set of valid prefixes for this coin type.
    var prefixSet: Set<UInt8> {
        switch self {
        case .bitcoin,
             .bitcoinCash:
            return Set([P2SHPrefix.bitcoin.rawValue, P2PKHPrefix.bitcoin.rawValue])
        case .litecoin:
            return Set([P2SHPrefix.litecoin.rawValue, P2PKHPrefix.litecoin.rawValue])
        case .lux:
            return Set([P2SHPrefix.lux.rawValue, P2PKHPrefix.litecoin.rawValue])
        case .groestlcoin:
            return Set([P2SHPrefix.bitcoin.rawValue, P2PKHPrefix.groestlcoin.rawValue])
        case .dash:
            return Set([P2SHPrefix.dash.rawValue, P2PKHPrefix.dash.rawValue])
        case .zcoin:
            return Set([P2SHPrefix.zcoin.rawValue, P2PKHPrefix.zcoin.rawValue])
        case .zelcash,
             .zcash:
            return Set([P2SHPrefix.zcashT.rawValue, P2PKHPrefix.zcashT.rawValue])
        case .qtum:
            return Set([P2PKHPrefix.qtum.rawValue])
        case .dogecoin:
            return Set([P2SHPrefix.dogecoin.rawValue, P2PKHPrefix.dogecoin.rawValue])
        case .viacoin:
            return Set([P2SHPrefix.viacoin.rawValue, P2PKHPrefix.viacoin.rawValue])
        default:
            return Set()
        }
    }

    /// HRP for this coin type.
    var hrp: HRP {
        switch self {
        case .bitcoin:
            return .bitcoin
        case .bitcoinCash:
            return .bitcoinCash
        case .binance:
            return .binance
        case .cosmos:
            return .cosmos
        case .litecoin:
            return .litecoin
        case .groestlcoin:
            return .groestlcoin
        case .viacoin:
            return .viacoin
        case .qtum:
            return .qtum
        default:
            return HRP.unknown
        }
    }
}

extension IOSTAccount: Address {}
