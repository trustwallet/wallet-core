// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

public enum CoinType: UInt32, CaseIterable {
    case aion = 425
    case binance = 714
    case bitcoin = 0
    case bitcoinCash = 145
    case callisto = 820
    case cosmos = 118
    case dash = 5
    case decred = 42
    case ethereum = 60
    case ethereumClassic = 61
    case go = 6060
    case icon = 74
    case kin = 2017
    case litecoin = 2
    case neo = 888
    case nimiq = 242
    case ontology = 1024
    case poa = 178
    case ripple = 144
    case stellar = 148
    case tezos = 1729
    case thunderToken = 1001
    case tomoChain = 889
    case tron = 195
    case veChain = 818
    case wanChain = 5718350
    case xdai = 700
    case zcash = 133
    case zcoin = 136
    case theta = 500

    public var purpose: Purpose {
        return Purpose(rawValue: TWCoinTypePurpose(TWCoinType(rawValue: rawValue)).rawValue)!
    }

    public var curve: Curve {
        return Curve(rawValue: TWCoinTypeCurve(TWCoinType(rawValue: rawValue)).rawValue)!
    }

    public var xpubVersion: HDVersion {
        return HDVersion(rawValue: TWCoinTypeXpubVersion(TWCoinType(rawValue: rawValue)).rawValue)!
    }

    public var xprvVersion: HDVersion {
        return HDVersion(rawValue: TWCoinTypeXprvVersion(TWCoinType(rawValue: rawValue)).rawValue)!
    }

    public func validate(address: String) -> Bool {
        let addressString = TWStringCreateWithNSString(address)
        defer {
            TWStringDelete(addressString)
        }
        return TWCoinTypeValidate(TWCoinType(rawValue: rawValue), addressString)
    }


    public func derivationPath() -> String {
        return TWStringNSString(TWCoinTypeDerivationPath(TWCoinType(rawValue: rawValue)))
    }


    public func deriveAddress(privateKey: PrivateKey) -> String {
        return TWStringNSString(TWCoinTypeDeriveAddress(TWCoinType(rawValue: rawValue), privateKey.rawValue))
    }


    public func deriveAddressFromPublicKey(publicKey: PublicKey) -> String {
        return TWStringNSString(TWCoinTypeDeriveAddressFromPublicKey(TWCoinType(rawValue: rawValue), publicKey.rawValue))
    }

}
