// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public typealias TWTronSigningInput = TW_Tron_Proto_SigningInput
public typealias TWTronSigningOutput = TW_Tron_Proto_SigningOutput
public typealias TWTronTransaction = TW_Tron_Proto_Transaction
public typealias TWTronTransferContract = TW_Tron_Proto_TransferContract
public typealias TWTronTransferAssetContract = TW_Tron_Proto_TransferAssetContract
public typealias TWTronBlockHeader = TW_Tron_Proto_BlockHeader

public final class Tron: Blockchain {
    override public var coinType: CoinType {
        return .tron
    }

    override public func address(for publicKey: PublicKey) -> Address {
        assert(publicKey.isCompressed == false)
        let keyhash = Hash.keccak256(data: publicKey.data.dropFirst()).suffix(20)
        let hash = Data([TronAddress.prefix]) + keyhash
        return TronAddress(data: hash)!
    }

    public override func address(string: String) -> Address? {
        return TronAddress(string: string)
    }

    public override func address(data: Data) -> Address? {
        return TronAddress(data: data)
    }

    override public init(purpose: Purpose = .bip44) {
        super.init(purpose: purpose)
    }
}
