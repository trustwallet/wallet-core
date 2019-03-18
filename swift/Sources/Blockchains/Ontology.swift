// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public final class Ontology: Blockchain {
    /// Chain identifier.
    public var chainID: Int {
        return 1
    }
    
    /// SLIP-044 coin type.
    public override var coinType: CoinType {
        return .ontology
    }
    
    public override func address(for publicKey: PublicKey) -> Address {
        return OntologyAddress(publicKey: publicKey)
    }
    
    public override func address(string: String) -> Address? {
        return OntologyAddress(string: string)
    }
    
    public override func address(data: Data) -> Address? {
        return OntologyAddress(data: data)
    }
}
