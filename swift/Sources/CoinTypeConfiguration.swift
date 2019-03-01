// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public struct CoinTypeConfiguration {

    public static func getSymbol(type: CoinType) -> String {
        return TWStringNSString(TWCoinTypeConfigurationGetSymbol(TWCoinType(rawValue: type.rawValue)))
    }

    public static func getDecimals(type: CoinType) -> Int32 {
        return TWCoinTypeConfigurationGetDecimals(TWCoinType(rawValue: type.rawValue))
    }

    public static func getTransactionURL(type: CoinType, transactionID: String) -> String {
        let transactionIDString = TWStringCreateWithNSString(transactionID)
        defer {
            TWStringDelete(transactionIDString)
        }
        return TWStringNSString(TWCoinTypeConfigurationGetTransactionURL(TWCoinType(rawValue: type.rawValue), transactionIDString))
    }

    public static func getID(type: CoinType) -> String {
        return TWStringNSString(TWCoinTypeConfigurationGetID(TWCoinType(rawValue: type.rawValue)))
    }

    public static func getName(type: CoinType) -> String {
        return TWStringNSString(TWCoinTypeConfigurationGetName(TWCoinType(rawValue: type.rawValue)))
    }

    var rawValue: TWCoinTypeConfiguration

    init(rawValue: TWCoinTypeConfiguration) {
        self.rawValue = rawValue
    }


}
