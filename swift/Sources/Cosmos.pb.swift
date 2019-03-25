// DO NOT EDIT.
//
// Generated by the Swift generator plugin for the protocol buffer compiler.
// Source: Cosmos.proto
//
// For information on using the generated types, please see the documenation:
//   https://github.com/apple/swift-protobuf/

import Foundation
import SwiftProtobuf

// If the compiler emits an error on this type, it is because this file
// was generated by a version of the `protoc` Swift plug-in that is
// incompatible with the version of SwiftProtobuf to which you are linking.
// Please ensure that your are building against the same version of the API
// that was used to generate this file.
fileprivate struct _GeneratedWithProtocGenSwiftVersion: SwiftProtobuf.ProtobufAPIVersionCheck {
  struct _2: SwiftProtobuf.ProtobufAPIVersion_2 {}
  typealias Version = _2
}

/// Message for sending tokens.
public struct TW_Cosmos_Proto_SendTokensMessage {
  // SwiftProtobuf.Message conformance is added in an extension below. See the
  // `Message` and `Message+*Additions` files in the SwiftProtobuf library for
  // methods supported on all messages.

  public var fromAddress: Data = SwiftProtobuf.Internal.emptyData

  public var toAddress: Data = SwiftProtobuf.Internal.emptyData

  public var amount: [TW_Cosmos_Proto_SendTokensMessage.Token] = []

  public var unknownFields = SwiftProtobuf.UnknownStorage()

  public struct Token {
    // SwiftProtobuf.Message conformance is added in an extension below. See the
    // `Message` and `Message+*Additions` files in the SwiftProtobuf library for
    // methods supported on all messages.

    public var denom: String = String()

    public var amount: Int64 = 0

    public var unknownFields = SwiftProtobuf.UnknownStorage()

    public init() {}
  }

  public init() {}
}

/// Input data necessary to create a signed order.
public struct TW_Cosmos_Proto_SigningInput {
  // SwiftProtobuf.Message conformance is added in an extension below. See the
  // `Message` and `Message+*Additions` files in the SwiftProtobuf library for
  // methods supported on all messages.

  public var accountNumber: UInt64 {
    get {return _storage._accountNumber}
    set {_uniqueStorage()._accountNumber = newValue}
  }

  public var chainID: String {
    get {return _storage._chainID}
    set {_uniqueStorage()._chainID = newValue}
  }

  public var memo: String {
    get {return _storage._memo}
    set {_uniqueStorage()._memo = newValue}
  }

  public var sequence: UInt64 {
    get {return _storage._sequence}
    set {_uniqueStorage()._sequence = newValue}
  }

  public var privateKey: Data {
    get {return _storage._privateKey}
    set {_uniqueStorage()._privateKey = newValue}
  }

  public var messageOneof: OneOf_MessageOneof? {
    get {return _storage._messageOneof}
    set {_uniqueStorage()._messageOneof = newValue}
  }

  public var message: TW_Cosmos_Proto_SendTokensMessage {
    get {
      if case .message(let v)? = _storage._messageOneof {return v}
      return TW_Cosmos_Proto_SendTokensMessage()
    }
    set {_uniqueStorage()._messageOneof = .message(newValue)}
  }

  public var unknownFields = SwiftProtobuf.UnknownStorage()

  public enum OneOf_MessageOneof: Equatable {
    case message(TW_Cosmos_Proto_SendTokensMessage)

  #if !swift(>=4.1)
    public static func ==(lhs: TW_Cosmos_Proto_SigningInput.OneOf_MessageOneof, rhs: TW_Cosmos_Proto_SigningInput.OneOf_MessageOneof) -> Bool {
      switch (lhs, rhs) {
      case (.message(let l), .message(let r)): return l == r
      }
    }
  #endif
  }

  public init() {}

  fileprivate var _storage = _StorageClass.defaultInstance
}

// MARK: - Code below here is support for the SwiftProtobuf runtime.

fileprivate let _protobuf_package = "TW.Cosmos.Proto"

extension TW_Cosmos_Proto_SendTokensMessage: SwiftProtobuf.Message, SwiftProtobuf._MessageImplementationBase, SwiftProtobuf._ProtoNameProviding {
  public static let protoMessageName: String = _protobuf_package + ".SendTokensMessage"
  public static let _protobuf_nameMap: SwiftProtobuf._NameMap = [
    1: .standard(proto: "from_address"),
    2: .standard(proto: "to_address"),
    3: .same(proto: "amount"),
  ]

  public mutating func decodeMessage<D: SwiftProtobuf.Decoder>(decoder: inout D) throws {
    while let fieldNumber = try decoder.nextFieldNumber() {
      switch fieldNumber {
      case 1: try decoder.decodeSingularBytesField(value: &self.fromAddress)
      case 2: try decoder.decodeSingularBytesField(value: &self.toAddress)
      case 3: try decoder.decodeRepeatedMessageField(value: &self.amount)
      default: break
      }
    }
  }

  public func traverse<V: SwiftProtobuf.Visitor>(visitor: inout V) throws {
    if !self.fromAddress.isEmpty {
      try visitor.visitSingularBytesField(value: self.fromAddress, fieldNumber: 1)
    }
    if !self.toAddress.isEmpty {
      try visitor.visitSingularBytesField(value: self.toAddress, fieldNumber: 2)
    }
    if !self.amount.isEmpty {
      try visitor.visitRepeatedMessageField(value: self.amount, fieldNumber: 3)
    }
    try unknownFields.traverse(visitor: &visitor)
  }

  public static func ==(lhs: TW_Cosmos_Proto_SendTokensMessage, rhs: TW_Cosmos_Proto_SendTokensMessage) -> Bool {
    if lhs.fromAddress != rhs.fromAddress {return false}
    if lhs.toAddress != rhs.toAddress {return false}
    if lhs.amount != rhs.amount {return false}
    if lhs.unknownFields != rhs.unknownFields {return false}
    return true
  }
}

extension TW_Cosmos_Proto_SendTokensMessage.Token: SwiftProtobuf.Message, SwiftProtobuf._MessageImplementationBase, SwiftProtobuf._ProtoNameProviding {
  public static let protoMessageName: String = TW_Cosmos_Proto_SendTokensMessage.protoMessageName + ".Token"
  public static let _protobuf_nameMap: SwiftProtobuf._NameMap = [
    1: .same(proto: "denom"),
    2: .same(proto: "amount"),
  ]

  public mutating func decodeMessage<D: SwiftProtobuf.Decoder>(decoder: inout D) throws {
    while let fieldNumber = try decoder.nextFieldNumber() {
      switch fieldNumber {
      case 1: try decoder.decodeSingularStringField(value: &self.denom)
      case 2: try decoder.decodeSingularInt64Field(value: &self.amount)
      default: break
      }
    }
  }

  public func traverse<V: SwiftProtobuf.Visitor>(visitor: inout V) throws {
    if !self.denom.isEmpty {
      try visitor.visitSingularStringField(value: self.denom, fieldNumber: 1)
    }
    if self.amount != 0 {
      try visitor.visitSingularInt64Field(value: self.amount, fieldNumber: 2)
    }
    try unknownFields.traverse(visitor: &visitor)
  }

  public static func ==(lhs: TW_Cosmos_Proto_SendTokensMessage.Token, rhs: TW_Cosmos_Proto_SendTokensMessage.Token) -> Bool {
    if lhs.denom != rhs.denom {return false}
    if lhs.amount != rhs.amount {return false}
    if lhs.unknownFields != rhs.unknownFields {return false}
    return true
  }
}

extension TW_Cosmos_Proto_SigningInput: SwiftProtobuf.Message, SwiftProtobuf._MessageImplementationBase, SwiftProtobuf._ProtoNameProviding {
  public static let protoMessageName: String = _protobuf_package + ".SigningInput"
  public static let _protobuf_nameMap: SwiftProtobuf._NameMap = [
    1: .standard(proto: "account_number"),
    2: .standard(proto: "chain_id"),
    3: .same(proto: "memo"),
    4: .same(proto: "sequence"),
    5: .standard(proto: "private_Key"),
    6: .same(proto: "message"),
  ]

  fileprivate class _StorageClass {
    var _accountNumber: UInt64 = 0
    var _chainID: String = String()
    var _memo: String = String()
    var _sequence: UInt64 = 0
    var _privateKey: Data = SwiftProtobuf.Internal.emptyData
    var _messageOneof: TW_Cosmos_Proto_SigningInput.OneOf_MessageOneof?

    static let defaultInstance = _StorageClass()

    private init() {}

    init(copying source: _StorageClass) {
      _accountNumber = source._accountNumber
      _chainID = source._chainID
      _memo = source._memo
      _sequence = source._sequence
      _privateKey = source._privateKey
      _messageOneof = source._messageOneof
    }
  }

  fileprivate mutating func _uniqueStorage() -> _StorageClass {
    if !isKnownUniquelyReferenced(&_storage) {
      _storage = _StorageClass(copying: _storage)
    }
    return _storage
  }

  public mutating func decodeMessage<D: SwiftProtobuf.Decoder>(decoder: inout D) throws {
    _ = _uniqueStorage()
    try withExtendedLifetime(_storage) { (_storage: _StorageClass) in
      while let fieldNumber = try decoder.nextFieldNumber() {
        switch fieldNumber {
        case 1: try decoder.decodeSingularUInt64Field(value: &_storage._accountNumber)
        case 2: try decoder.decodeSingularStringField(value: &_storage._chainID)
        case 3: try decoder.decodeSingularStringField(value: &_storage._memo)
        case 4: try decoder.decodeSingularUInt64Field(value: &_storage._sequence)
        case 5: try decoder.decodeSingularBytesField(value: &_storage._privateKey)
        case 6:
          var v: TW_Cosmos_Proto_SendTokensMessage?
          if let current = _storage._messageOneof {
            try decoder.handleConflictingOneOf()
            if case .message(let m) = current {v = m}
          }
          try decoder.decodeSingularMessageField(value: &v)
          if let v = v {_storage._messageOneof = .message(v)}
        default: break
        }
      }
    }
  }

  public func traverse<V: SwiftProtobuf.Visitor>(visitor: inout V) throws {
    try withExtendedLifetime(_storage) { (_storage: _StorageClass) in
      if _storage._accountNumber != 0 {
        try visitor.visitSingularUInt64Field(value: _storage._accountNumber, fieldNumber: 1)
      }
      if !_storage._chainID.isEmpty {
        try visitor.visitSingularStringField(value: _storage._chainID, fieldNumber: 2)
      }
      if !_storage._memo.isEmpty {
        try visitor.visitSingularStringField(value: _storage._memo, fieldNumber: 3)
      }
      if _storage._sequence != 0 {
        try visitor.visitSingularUInt64Field(value: _storage._sequence, fieldNumber: 4)
      }
      if !_storage._privateKey.isEmpty {
        try visitor.visitSingularBytesField(value: _storage._privateKey, fieldNumber: 5)
      }
      if case .message(let v)? = _storage._messageOneof {
        try visitor.visitSingularMessageField(value: v, fieldNumber: 6)
      }
    }
    try unknownFields.traverse(visitor: &visitor)
  }

  public static func ==(lhs: TW_Cosmos_Proto_SigningInput, rhs: TW_Cosmos_Proto_SigningInput) -> Bool {
    if lhs._storage !== rhs._storage {
      let storagesAreEqual: Bool = withExtendedLifetime((lhs._storage, rhs._storage)) { (_args: (_StorageClass, _StorageClass)) in
        let _storage = _args.0
        let rhs_storage = _args.1
        if _storage._accountNumber != rhs_storage._accountNumber {return false}
        if _storage._chainID != rhs_storage._chainID {return false}
        if _storage._memo != rhs_storage._memo {return false}
        if _storage._sequence != rhs_storage._sequence {return false}
        if _storage._privateKey != rhs_storage._privateKey {return false}
        if _storage._messageOneof != rhs_storage._messageOneof {return false}
        return true
      }
      if !storagesAreEqual {return false}
    }
    if lhs.unknownFields != rhs.unknownFields {return false}
    return true
  }
}
