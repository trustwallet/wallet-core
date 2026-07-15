// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import Foundation

extension Account: Equatable {
    public static func == (lhs: Account, rhs: Account) -> Bool {
        return lhs.coin == rhs.coin &&
        lhs.address == rhs.address &&
        lhs.derivation == rhs.derivation &&
        lhs.derivationPath == rhs.derivationPath &&
        lhs.publicKey == rhs.publicKey &&
        lhs.extendedPublicKey == rhs.extendedPublicKey
    }
}

extension Account: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(coin)
        hasher.combine(address)
        hasher.combine(derivation)
        hasher.combine(derivationPath)
        hasher.combine(publicKey)
        hasher.combine(extendedPublicKey)
    }
}

extension Account: Codable {
    private enum CodingKeys: String, CodingKey {
        case coin
        case address
        case derivation
        case derivationPath
        case publicKey
        case extendedPublicKey
    }

    public func encode(to encoder: Encoder) throws {
        var container = encoder.container(keyedBy: CodingKeys.self)
        try container.encode(coin.rawValue, forKey: .coin)
        try container.encode(address, forKey: .address)
        try container.encode(derivation.rawValue, forKey: .derivation)
        try container.encode(derivationPath, forKey: .derivationPath)
        try container.encode(publicKey, forKey: .publicKey)
        try container.encode(extendedPublicKey, forKey: .extendedPublicKey)
    }

    public convenience init(from decoder: Decoder) throws {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        let rawCoin           = try container.decode(UInt32.self, forKey: .coin)
        let address           = try container.decode(String.self, forKey: .address)
        let rawDerivation     = try container.decode(UInt32.self, forKey: .derivation)
        let derivationPath    = try container.decode(String.self, forKey: .derivationPath)
        let publicKey         = try container.decode(String.self, forKey: .publicKey)
        let extendedPublicKey = try container.decode(String.self, forKey: .extendedPublicKey)

        guard let coin = CoinType(rawValue: rawCoin) else {
            throw DecodingError.dataCorruptedError(forKey: .coin, in: container,
                debugDescription: "Unknown coin type: \(rawCoin)")
        }
        guard let derivation = Derivation(rawValue: rawDerivation) else {
            throw DecodingError.dataCorruptedError(forKey: .derivation, in: container,
                debugDescription: "Unknown derivation: \(rawDerivation)")
        }
        // Call TWAccountCreate directly rather than using the Account failable init:
        // if we created an intermediate Account and copied its rawValue, that wrapper's
        // deinit would call TWAccountDelete before self uses the pointer (use-after-free).
        let addressString = TWStringCreateWithNSString(address)
        defer { TWStringDelete(addressString) }
        let derivationPathString = TWStringCreateWithNSString(derivationPath)
        defer { TWStringDelete(derivationPathString) }
        let publicKeyString = TWStringCreateWithNSString(publicKey)
        defer { TWStringDelete(publicKeyString) }
        let extendedPublicKeyString = TWStringCreateWithNSString(extendedPublicKey)
        defer { TWStringDelete(extendedPublicKeyString) }
        guard let rawValue = TWAccountCreate(
            addressString,
            TWCoinType(rawValue: coin.rawValue),
            TWDerivation(rawValue: derivation.rawValue),
            derivationPathString,
            publicKeyString,
            extendedPublicKeyString
        ) else {
            throw DecodingError.dataCorruptedError(forKey: .derivationPath, in: container,
                debugDescription: "Invalid derivation path: \(derivationPath)")
        }
        self.init(rawValue: rawValue)
    }
}
