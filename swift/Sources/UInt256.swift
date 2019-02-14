// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public struct UInt256 {
    public static var zero: UInt256 {
        return UInt256(rawValue: TWUInt256Zero())
    }

    public static var one: UInt256 {
        return UInt256(rawValue: TWUInt256One())
    }


    public static func == (lhs: UInt256, rhs: UInt256) -> Bool {
        return TWUInt256Equal(lhs.rawValue, rhs.rawValue)
    }

    public static func < (lhs: UInt256, rhs: UInt256) -> Bool {
        return TWUInt256Less(lhs.rawValue, rhs.rawValue)
    }

    var rawValue: TWUInt256

    public var isZero: Bool {
        return TWUInt256IsZero(rawValue)
    }

    public var uint32Value: UInt32 {
        return TWUInt256UInt32Value(rawValue)
    }

    public var uint64Value: UInt64 {
        return TWUInt256UInt64Value(rawValue)
    }

    public var data: Data {
        return TWDataNSData(TWUInt256Data(rawValue))
    }

    public var description: String {
        return TWStringNSString(TWUInt256Description(rawValue))
    }

    init(rawValue: TWUInt256) {
        self.rawValue = rawValue
    }

    public init?(data: Data) {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        rawValue = TWUInt256()
        guard TWUInt256InitWithData(&rawValue, dataData) else {
            return nil
        }
    }

    public init?(string: String) {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        rawValue = TWUInt256()
        guard TWUInt256InitWithString(&rawValue, stringString) else {
            return nil
        }
    }

    public init(_ value: UInt32) {
        rawValue = TWUInt256()
        TWUInt256InitWithUInt32(&rawValue, value)
    }

    public init(_ value: UInt64) {
        rawValue = TWUInt256()
        TWUInt256InitWithUInt64(&rawValue, value)
    }


    public func format(decimals: Int32) -> String {
        return TWStringNSString(TWUInt256Format(rawValue, Int32(decimals)))
    }

}
