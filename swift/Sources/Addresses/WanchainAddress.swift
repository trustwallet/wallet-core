// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public struct WanchainAddress: Address, Hashable {

    public static let size = EthereumAddress.size

    static public func isValid(data: Data) -> Bool {
        return EthereumAddress.isValid(data: data)
    }

    static public func isValidString(string: String) -> Bool {
        return EthereumAddress.isValidString(string: string)
    }

    public let data: Data

    public init?(data: Data) {
        guard WanchainAddress.isValid(data: data) else {
            return nil
        }
        self.data = data
    }

    public init?(string: String) {
        guard let data = Data(hexString: string), WanchainAddress.isValid(data: data) else {
            return nil
        }
        self.init(data: data)
    }

    public init(publicKey: PublicKey) {
        data = EthereumAddress(publicKey: publicKey).data
    }

    public var description: String {
        return "0x" + data.hexString
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(data)
    }

    public static func == (lhs: WanchainAddress, rhs: WanchainAddress) -> Bool {
        return lhs.data == rhs.data
    }
}
