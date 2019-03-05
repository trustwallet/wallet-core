// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

public struct TronAddress: Address, Hashable {
    public static let size = 20
    public static let prefix: UInt8 = 0x41

    static public func isValid(data: Data) -> Bool {
        if data.count != TronAddress.size + 1 {
            return false
        }
        return true
    }

    static public func isValidString(string: String) -> Bool {
        guard let decoded = Base58.decode(string: string) else {
            return false
        }
        if decoded.count != 1 + TronAddress.size {
            return false
        }
        return string.hasPrefix("T")
    }

    public let data: Data

    public init(publicKey: PublicKeySecp256k1) {
        assert(publicKey.isCompressed == false)
        let keyhash = Hash.keccak256(data: publicKey.data.dropFirst()).suffix(20)
        data = Data([TronAddress.prefix]) + keyhash
    }

    public init?(data: Data) {
        if !TronAddress.isValid(data: data) {
            return nil
        }
        self.data = data
    }

    public init?(string: String) {
        guard let decoded = Base58.decode(string: string) else {
            return nil
        }
        self.init(data: decoded)
    }

    public var base58String: String {
        return Base58.encode(data: data)
    }

    public var description: String {
        return base58String
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(data)
    }

    public static func == (lhs: TronAddress, rhs: TronAddress) -> Bool {
        return lhs.data == rhs.data
    }
}
