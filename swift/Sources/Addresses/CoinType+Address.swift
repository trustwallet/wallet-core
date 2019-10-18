// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public extension CoinType {
    /// Converts a string to an address for this coin type.
    // swiftlint:disable function_body_length
    func address(string: String) -> Address? {
        guard self.validate(address: string) else {
            return nil
        }

        switch self {
        case .binance, .cosmos, .terra:
            return CosmosAddress(string: string)
        case .bitcoin, .litecoin, .viacoin, .qtum, .digiByte, .monacoin:
            if let segwitAddress = SegwitAddress(string: string) {
                return segwitAddress
            } else {
                return BitcoinAddress(string: string)
            }
        case .bitcoinCash:
            if let bitcoinCashAddress = BitcoinCashAddress(string: string) {
                return bitcoinCashAddress
            } else {
                return BitcoinAddress(string: string)
            }
        case .dash, .dogecoin, .zcoin, .ravencoin:
            return BitcoinAddress(string: string)
        case .callisto, .ethereum, .ethereumClassic, .goChain,
             .poanetwork, .theta, .thunderToken, .tomoChain, .veChain:
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
        case .zelcash, .zcash:
            return ZcashTAddress(string: string)
        case .nimiq:
            return NimiqAddress(string: string)
        case .stellar, .kin:
            return StellarAddress(string: string)
        case .aion:
            return AionAddress(string: string)
        case .nano:
            return NanoAddress(string: string)
        case .nebulas:
            return NebulasAddress(string: string)
        case .near:
            return NEARAddress(string: string)
        case .neo:
            return NEOAddress(string: string)
        case .decred:
            return DecredAddress(string: string)
        case .groestlcoin:
            if let segwitAddress = SegwitAddress(string: string) {
                return segwitAddress
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
        case .fio:
            return FIOAddress(string: string)
        case .ioTeX:
            return IoTeXAddress(string: string)
        case .zilliqa:
            if string.starts(with: "0x"), let keyHash = Data(hexString: string) {
                return ZilliqaAddress(keyHash: keyHash)
            } else {
                return ZilliqaAddress(string: string)
            }
        case .semux:
            return SemuxAddress(string: string)
        case .ark:
            return ARKAddress(string: string)
        case .waves:
            return WavesAddress(string: string)
        case .aeternity:
            return AeternityAddress(string: string)
        case .harmony:
            return HarmonyAddress(string: string)
        case .solana:
            return SolanaAddress(string: string)
        case .algorand:
            return AlgorandAddress(string: string)
        }
    }
}

extension IOSTAccount: Address {}
