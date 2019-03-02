// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public struct TezosAddress: Address {
  private static let prefixBytes = Data([6, 161, 159]) // tz1
  private static let checksumLength = 4
  private static let outputLength = 20

  public var data: Data

  public var description: String {
    return Base58.encode(data: data)
  }

  /// Validates that the raw data is a valid address.
  public static func isValid(data: Data) -> Bool {
    let bytes = [UInt8](data)

    // Check that the prefix is correct.
    for (i, byte) in prefixBytes.enumerated() where bytes[i] != byte {
      return false
    }

    // Validate that the checksum is correct.
    let expectedChecksum = bytes.suffix(checksumLength)
    let bytesToChecksum = bytes.prefix(upTo: bytes.count - checksumLength)
    let calculatedChecksum = calculateChecksum(Data(bytesToChecksum))
    guard expectedChecksum.elementsEqual(calculatedChecksum, by: { $0 == $1 }) else {
      return false
    }

    return true
  }

  /// Validates that the string is a valid address.
  public static func isValidString(string: String) -> Bool {
    guard let decodedData = Base58.decodeNoCheck(string: string) else {
      return false
    }
    return isValid(data: decodedData)
  }

  /// Calculate an expected checksum from the given data.
  private static func calculateChecksum(_ input: Data) -> [UInt8] {
    let hashedData = Hash.sha256(data: input)
    let doubleHashedData =  Hash.sha256(data: hashedData)
    let doubleHashedArray = Array(doubleHashedData)
    return Array(doubleHashedArray.prefix(checksumLength))
  }

  /// Creates an address from a string representation.
  public init?(string: String) {
    guard TezosAddress.isValidString(string: string),
          let data = Base58.decode(string: string) else {
      return nil
    }
    self.data = data
  }

  /// Creates an address from a PublicKey.
  public init(publicKey: PublicKey) {
    self.data = TezosAddress.prefixBytes + Hash.blake2b(data: publicKey.data, size: TezosAddress.outputLength)
  }
}
