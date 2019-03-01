// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public class Zcash: Bitcoin {

    public var tAddrPrefix: UInt8 {
        return 0x1c
    }

    override public var coinType: CoinType {
        return .zcash
    }

    override public var p2pkhPrefix: UInt8 {
        return P2PKHPrefix.zcashT.rawValue
    }

    override public var p2shPrefix: UInt8 {
        return P2SHPrefix.zcashT.rawValue
    }

    override public var xpubVersion: HDVersion? {
        return .xpub
    }

    override public var xprvVersion: HDVersion? {
        return .xprv
    }

    override public init(purpose: Purpose = .bip44) {
        super.init(purpose: purpose)
    }

    override public func address(for publicKey: PublicKey) -> Address {
        return ZcashTAddress(publicKey: publicKey.compressed, prefix: p2pkhPrefix)!
    }

    override public func address(string: String) -> Address? {
        return ZcashTAddress(string: string)
    }

    override public func address(data: Data) -> Address? {
        return ZcashTAddress(data: data)
    }

    override public func validate(address: String) -> Bool {
        if let data = Base58.decode(string: address),
            data.count == TWZcashTAddressSize,
            data[0] == tAddrPrefix {
            return data[1] == p2pkhPrefix || data[1] == p2shPrefix
        } else {
            return false
        }
    }
}

extension ZcashTAddress: Address {}
