//
//  Generated code. Do not modify.
//  source: Ethereum.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'Common.pbenum.dart' as $0;
import 'Ethereum.pbenum.dart';

export 'Ethereum.pbenum.dart';

/// Native coin transfer transaction
class Transaction_Transfer extends $pb.GeneratedMessage {
  factory Transaction_Transfer({
    $core.List<$core.int>? amount,
    $core.List<$core.int>? data,
  }) {
    final $result = create();
    if (amount != null) {
      $result.amount = amount;
    }
    if (data != null) {
      $result.data = data;
    }
    return $result;
  }
  Transaction_Transfer._() : super();
  factory Transaction_Transfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction_Transfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction.Transfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'data', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transaction_Transfer clone() => Transaction_Transfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transaction_Transfer copyWith(void Function(Transaction_Transfer) updates) => super.copyWith((message) => updates(message as Transaction_Transfer)) as Transaction_Transfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Transaction_Transfer create() => Transaction_Transfer._();
  Transaction_Transfer createEmptyInstance() => create();
  static $pb.PbList<Transaction_Transfer> createRepeated() => $pb.PbList<Transaction_Transfer>();
  @$core.pragma('dart2js:noInline')
  static Transaction_Transfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transaction_Transfer>(create);
  static Transaction_Transfer? _defaultInstance;

  /// Amount to send in wei (uint256, serialized big endian)
  @$pb.TagNumber(1)
  $core.List<$core.int> get amount => $_getN(0);
  @$pb.TagNumber(1)
  set amount($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAmount() => $_has(0);
  @$pb.TagNumber(1)
  void clearAmount() => clearField(1);

  /// Optional payload data
  @$pb.TagNumber(2)
  $core.List<$core.int> get data => $_getN(1);
  @$pb.TagNumber(2)
  set data($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasData() => $_has(1);
  @$pb.TagNumber(2)
  void clearData() => clearField(2);
}

/// ERC20 token transfer transaction
class Transaction_ERC20Transfer extends $pb.GeneratedMessage {
  factory Transaction_ERC20Transfer({
    $core.String? to,
    $core.List<$core.int>? amount,
  }) {
    final $result = create();
    if (to != null) {
      $result.to = to;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    return $result;
  }
  Transaction_ERC20Transfer._() : super();
  factory Transaction_ERC20Transfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction_ERC20Transfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction.ERC20Transfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'to')
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transaction_ERC20Transfer clone() => Transaction_ERC20Transfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transaction_ERC20Transfer copyWith(void Function(Transaction_ERC20Transfer) updates) => super.copyWith((message) => updates(message as Transaction_ERC20Transfer)) as Transaction_ERC20Transfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Transaction_ERC20Transfer create() => Transaction_ERC20Transfer._();
  Transaction_ERC20Transfer createEmptyInstance() => create();
  static $pb.PbList<Transaction_ERC20Transfer> createRepeated() => $pb.PbList<Transaction_ERC20Transfer>();
  @$core.pragma('dart2js:noInline')
  static Transaction_ERC20Transfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transaction_ERC20Transfer>(create);
  static Transaction_ERC20Transfer? _defaultInstance;

  /// destination address (string)
  @$pb.TagNumber(1)
  $core.String get to => $_getSZ(0);
  @$pb.TagNumber(1)
  set to($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasTo() => $_has(0);
  @$pb.TagNumber(1)
  void clearTo() => clearField(1);

  /// Amount to send (uint256, serialized big endian)
  @$pb.TagNumber(2)
  $core.List<$core.int> get amount => $_getN(1);
  @$pb.TagNumber(2)
  set amount($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);
}

/// ERC20 approve transaction
class Transaction_ERC20Approve extends $pb.GeneratedMessage {
  factory Transaction_ERC20Approve({
    $core.String? spender,
    $core.List<$core.int>? amount,
  }) {
    final $result = create();
    if (spender != null) {
      $result.spender = spender;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    return $result;
  }
  Transaction_ERC20Approve._() : super();
  factory Transaction_ERC20Approve.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction_ERC20Approve.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction.ERC20Approve', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'spender')
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transaction_ERC20Approve clone() => Transaction_ERC20Approve()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transaction_ERC20Approve copyWith(void Function(Transaction_ERC20Approve) updates) => super.copyWith((message) => updates(message as Transaction_ERC20Approve)) as Transaction_ERC20Approve;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Transaction_ERC20Approve create() => Transaction_ERC20Approve._();
  Transaction_ERC20Approve createEmptyInstance() => create();
  static $pb.PbList<Transaction_ERC20Approve> createRepeated() => $pb.PbList<Transaction_ERC20Approve>();
  @$core.pragma('dart2js:noInline')
  static Transaction_ERC20Approve getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transaction_ERC20Approve>(create);
  static Transaction_ERC20Approve? _defaultInstance;

  /// Target of the approval
  @$pb.TagNumber(1)
  $core.String get spender => $_getSZ(0);
  @$pb.TagNumber(1)
  set spender($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSpender() => $_has(0);
  @$pb.TagNumber(1)
  void clearSpender() => clearField(1);

  /// Amount to send (uint256, serialized big endian)
  @$pb.TagNumber(2)
  $core.List<$core.int> get amount => $_getN(1);
  @$pb.TagNumber(2)
  set amount($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);
}

/// ERC721 NFT transfer transaction
class Transaction_ERC721Transfer extends $pb.GeneratedMessage {
  factory Transaction_ERC721Transfer({
    $core.String? from,
    $core.String? to,
    $core.List<$core.int>? tokenId,
  }) {
    final $result = create();
    if (from != null) {
      $result.from = from;
    }
    if (to != null) {
      $result.to = to;
    }
    if (tokenId != null) {
      $result.tokenId = tokenId;
    }
    return $result;
  }
  Transaction_ERC721Transfer._() : super();
  factory Transaction_ERC721Transfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction_ERC721Transfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction.ERC721Transfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'from')
    ..aOS(2, _omitFieldNames ? '' : 'to')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'tokenId', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transaction_ERC721Transfer clone() => Transaction_ERC721Transfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transaction_ERC721Transfer copyWith(void Function(Transaction_ERC721Transfer) updates) => super.copyWith((message) => updates(message as Transaction_ERC721Transfer)) as Transaction_ERC721Transfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Transaction_ERC721Transfer create() => Transaction_ERC721Transfer._();
  Transaction_ERC721Transfer createEmptyInstance() => create();
  static $pb.PbList<Transaction_ERC721Transfer> createRepeated() => $pb.PbList<Transaction_ERC721Transfer>();
  @$core.pragma('dart2js:noInline')
  static Transaction_ERC721Transfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transaction_ERC721Transfer>(create);
  static Transaction_ERC721Transfer? _defaultInstance;

  /// Source address
  @$pb.TagNumber(1)
  $core.String get from => $_getSZ(0);
  @$pb.TagNumber(1)
  set from($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFrom() => $_has(0);
  @$pb.TagNumber(1)
  void clearFrom() => clearField(1);

  /// Destination address
  @$pb.TagNumber(2)
  $core.String get to => $_getSZ(1);
  @$pb.TagNumber(2)
  set to($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTo() => $_has(1);
  @$pb.TagNumber(2)
  void clearTo() => clearField(2);

  /// ID of the token (uint256, serialized big endian)
  @$pb.TagNumber(3)
  $core.List<$core.int> get tokenId => $_getN(2);
  @$pb.TagNumber(3)
  set tokenId($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTokenId() => $_has(2);
  @$pb.TagNumber(3)
  void clearTokenId() => clearField(3);
}

/// ERC1155 NFT transfer transaction
class Transaction_ERC1155Transfer extends $pb.GeneratedMessage {
  factory Transaction_ERC1155Transfer({
    $core.String? from,
    $core.String? to,
    $core.List<$core.int>? tokenId,
    $core.List<$core.int>? value,
    $core.List<$core.int>? data,
  }) {
    final $result = create();
    if (from != null) {
      $result.from = from;
    }
    if (to != null) {
      $result.to = to;
    }
    if (tokenId != null) {
      $result.tokenId = tokenId;
    }
    if (value != null) {
      $result.value = value;
    }
    if (data != null) {
      $result.data = data;
    }
    return $result;
  }
  Transaction_ERC1155Transfer._() : super();
  factory Transaction_ERC1155Transfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction_ERC1155Transfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction.ERC1155Transfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'from')
    ..aOS(2, _omitFieldNames ? '' : 'to')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'tokenId', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(5, _omitFieldNames ? '' : 'data', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transaction_ERC1155Transfer clone() => Transaction_ERC1155Transfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transaction_ERC1155Transfer copyWith(void Function(Transaction_ERC1155Transfer) updates) => super.copyWith((message) => updates(message as Transaction_ERC1155Transfer)) as Transaction_ERC1155Transfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Transaction_ERC1155Transfer create() => Transaction_ERC1155Transfer._();
  Transaction_ERC1155Transfer createEmptyInstance() => create();
  static $pb.PbList<Transaction_ERC1155Transfer> createRepeated() => $pb.PbList<Transaction_ERC1155Transfer>();
  @$core.pragma('dart2js:noInline')
  static Transaction_ERC1155Transfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transaction_ERC1155Transfer>(create);
  static Transaction_ERC1155Transfer? _defaultInstance;

  /// Source address
  @$pb.TagNumber(1)
  $core.String get from => $_getSZ(0);
  @$pb.TagNumber(1)
  set from($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFrom() => $_has(0);
  @$pb.TagNumber(1)
  void clearFrom() => clearField(1);

  /// Destination address
  @$pb.TagNumber(2)
  $core.String get to => $_getSZ(1);
  @$pb.TagNumber(2)
  set to($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTo() => $_has(1);
  @$pb.TagNumber(2)
  void clearTo() => clearField(2);

  /// ID of the token (uint256, serialized big endian)
  @$pb.TagNumber(3)
  $core.List<$core.int> get tokenId => $_getN(2);
  @$pb.TagNumber(3)
  set tokenId($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTokenId() => $_has(2);
  @$pb.TagNumber(3)
  void clearTokenId() => clearField(3);

  /// The amount of tokens being transferred (uint256, serialized big endian)
  @$pb.TagNumber(4)
  $core.List<$core.int> get value => $_getN(3);
  @$pb.TagNumber(4)
  set value($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasValue() => $_has(3);
  @$pb.TagNumber(4)
  void clearValue() => clearField(4);

  @$pb.TagNumber(5)
  $core.List<$core.int> get data => $_getN(4);
  @$pb.TagNumber(5)
  set data($core.List<$core.int> v) { $_setBytes(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasData() => $_has(4);
  @$pb.TagNumber(5)
  void clearData() => clearField(5);
}

/// Generic smart contract transaction
class Transaction_ContractGeneric extends $pb.GeneratedMessage {
  factory Transaction_ContractGeneric({
    $core.List<$core.int>? amount,
    $core.List<$core.int>? data,
  }) {
    final $result = create();
    if (amount != null) {
      $result.amount = amount;
    }
    if (data != null) {
      $result.data = data;
    }
    return $result;
  }
  Transaction_ContractGeneric._() : super();
  factory Transaction_ContractGeneric.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction_ContractGeneric.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction.ContractGeneric', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'data', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transaction_ContractGeneric clone() => Transaction_ContractGeneric()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transaction_ContractGeneric copyWith(void Function(Transaction_ContractGeneric) updates) => super.copyWith((message) => updates(message as Transaction_ContractGeneric)) as Transaction_ContractGeneric;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Transaction_ContractGeneric create() => Transaction_ContractGeneric._();
  Transaction_ContractGeneric createEmptyInstance() => create();
  static $pb.PbList<Transaction_ContractGeneric> createRepeated() => $pb.PbList<Transaction_ContractGeneric>();
  @$core.pragma('dart2js:noInline')
  static Transaction_ContractGeneric getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transaction_ContractGeneric>(create);
  static Transaction_ContractGeneric? _defaultInstance;

  /// Amount to send in wei (uint256, serialized big endian)
  @$pb.TagNumber(1)
  $core.List<$core.int> get amount => $_getN(0);
  @$pb.TagNumber(1)
  set amount($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAmount() => $_has(0);
  @$pb.TagNumber(1)
  void clearAmount() => clearField(1);

  /// Contract call payload data
  @$pb.TagNumber(2)
  $core.List<$core.int> get data => $_getN(1);
  @$pb.TagNumber(2)
  set data($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasData() => $_has(1);
  @$pb.TagNumber(2)
  void clearData() => clearField(2);
}

class Transaction_Batch_BatchedCall extends $pb.GeneratedMessage {
  factory Transaction_Batch_BatchedCall({
    $core.String? address,
    $core.List<$core.int>? amount,
    $core.List<$core.int>? payload,
  }) {
    final $result = create();
    if (address != null) {
      $result.address = address;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (payload != null) {
      $result.payload = payload;
    }
    return $result;
  }
  Transaction_Batch_BatchedCall._() : super();
  factory Transaction_Batch_BatchedCall.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction_Batch_BatchedCall.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction.Batch.BatchedCall', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'address')
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'payload', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transaction_Batch_BatchedCall clone() => Transaction_Batch_BatchedCall()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transaction_Batch_BatchedCall copyWith(void Function(Transaction_Batch_BatchedCall) updates) => super.copyWith((message) => updates(message as Transaction_Batch_BatchedCall)) as Transaction_Batch_BatchedCall;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Transaction_Batch_BatchedCall create() => Transaction_Batch_BatchedCall._();
  Transaction_Batch_BatchedCall createEmptyInstance() => create();
  static $pb.PbList<Transaction_Batch_BatchedCall> createRepeated() => $pb.PbList<Transaction_Batch_BatchedCall>();
  @$core.pragma('dart2js:noInline')
  static Transaction_Batch_BatchedCall getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transaction_Batch_BatchedCall>(create);
  static Transaction_Batch_BatchedCall? _defaultInstance;

  /// Recipient addresses.
  @$pb.TagNumber(1)
  $core.String get address => $_getSZ(0);
  @$pb.TagNumber(1)
  set address($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearAddress() => clearField(1);

  /// Amounts to send in wei (uint256, serialized big endian)
  @$pb.TagNumber(2)
  $core.List<$core.int> get amount => $_getN(1);
  @$pb.TagNumber(2)
  set amount($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);

  /// Contract call payloads data
  @$pb.TagNumber(3)
  $core.List<$core.int> get payload => $_getN(2);
  @$pb.TagNumber(3)
  set payload($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasPayload() => $_has(2);
  @$pb.TagNumber(3)
  void clearPayload() => clearField(3);
}

/// Batched transaction for ERC-4337 wallets
class Transaction_Batch extends $pb.GeneratedMessage {
  factory Transaction_Batch({
    $core.Iterable<Transaction_Batch_BatchedCall>? calls,
  }) {
    final $result = create();
    if (calls != null) {
      $result.calls.addAll(calls);
    }
    return $result;
  }
  Transaction_Batch._() : super();
  factory Transaction_Batch.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction_Batch.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction.Batch', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..pc<Transaction_Batch_BatchedCall>(1, _omitFieldNames ? '' : 'calls', $pb.PbFieldType.PM, subBuilder: Transaction_Batch_BatchedCall.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transaction_Batch clone() => Transaction_Batch()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transaction_Batch copyWith(void Function(Transaction_Batch) updates) => super.copyWith((message) => updates(message as Transaction_Batch)) as Transaction_Batch;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Transaction_Batch create() => Transaction_Batch._();
  Transaction_Batch createEmptyInstance() => create();
  static $pb.PbList<Transaction_Batch> createRepeated() => $pb.PbList<Transaction_Batch>();
  @$core.pragma('dart2js:noInline')
  static Transaction_Batch getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transaction_Batch>(create);
  static Transaction_Batch? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<Transaction_Batch_BatchedCall> get calls => $_getList(0);
}

enum Transaction_TransactionOneof {
  transfer, 
  erc20Transfer, 
  erc20Approve, 
  erc721Transfer, 
  erc1155Transfer, 
  contractGeneric, 
  batch, 
  notSet
}

/// Transaction (transfer, smart contract call, ...)
class Transaction extends $pb.GeneratedMessage {
  factory Transaction({
    Transaction_Transfer? transfer,
    Transaction_ERC20Transfer? erc20Transfer,
    Transaction_ERC20Approve? erc20Approve,
    Transaction_ERC721Transfer? erc721Transfer,
    Transaction_ERC1155Transfer? erc1155Transfer,
    Transaction_ContractGeneric? contractGeneric,
    Transaction_Batch? batch,
  }) {
    final $result = create();
    if (transfer != null) {
      $result.transfer = transfer;
    }
    if (erc20Transfer != null) {
      $result.erc20Transfer = erc20Transfer;
    }
    if (erc20Approve != null) {
      $result.erc20Approve = erc20Approve;
    }
    if (erc721Transfer != null) {
      $result.erc721Transfer = erc721Transfer;
    }
    if (erc1155Transfer != null) {
      $result.erc1155Transfer = erc1155Transfer;
    }
    if (contractGeneric != null) {
      $result.contractGeneric = contractGeneric;
    }
    if (batch != null) {
      $result.batch = batch;
    }
    return $result;
  }
  Transaction._() : super();
  factory Transaction.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Transaction_TransactionOneof> _Transaction_TransactionOneofByTag = {
    1 : Transaction_TransactionOneof.transfer,
    2 : Transaction_TransactionOneof.erc20Transfer,
    3 : Transaction_TransactionOneof.erc20Approve,
    4 : Transaction_TransactionOneof.erc721Transfer,
    5 : Transaction_TransactionOneof.erc1155Transfer,
    6 : Transaction_TransactionOneof.contractGeneric,
    7 : Transaction_TransactionOneof.batch,
    0 : Transaction_TransactionOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2, 3, 4, 5, 6, 7])
    ..aOM<Transaction_Transfer>(1, _omitFieldNames ? '' : 'transfer', subBuilder: Transaction_Transfer.create)
    ..aOM<Transaction_ERC20Transfer>(2, _omitFieldNames ? '' : 'erc20Transfer', subBuilder: Transaction_ERC20Transfer.create)
    ..aOM<Transaction_ERC20Approve>(3, _omitFieldNames ? '' : 'erc20Approve', subBuilder: Transaction_ERC20Approve.create)
    ..aOM<Transaction_ERC721Transfer>(4, _omitFieldNames ? '' : 'erc721Transfer', subBuilder: Transaction_ERC721Transfer.create)
    ..aOM<Transaction_ERC1155Transfer>(5, _omitFieldNames ? '' : 'erc1155Transfer', subBuilder: Transaction_ERC1155Transfer.create)
    ..aOM<Transaction_ContractGeneric>(6, _omitFieldNames ? '' : 'contractGeneric', subBuilder: Transaction_ContractGeneric.create)
    ..aOM<Transaction_Batch>(7, _omitFieldNames ? '' : 'batch', subBuilder: Transaction_Batch.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transaction clone() => Transaction()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transaction copyWith(void Function(Transaction) updates) => super.copyWith((message) => updates(message as Transaction)) as Transaction;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Transaction create() => Transaction._();
  Transaction createEmptyInstance() => create();
  static $pb.PbList<Transaction> createRepeated() => $pb.PbList<Transaction>();
  @$core.pragma('dart2js:noInline')
  static Transaction getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transaction>(create);
  static Transaction? _defaultInstance;

  Transaction_TransactionOneof whichTransactionOneof() => _Transaction_TransactionOneofByTag[$_whichOneof(0)]!;
  void clearTransactionOneof() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  Transaction_Transfer get transfer => $_getN(0);
  @$pb.TagNumber(1)
  set transfer(Transaction_Transfer v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasTransfer() => $_has(0);
  @$pb.TagNumber(1)
  void clearTransfer() => clearField(1);
  @$pb.TagNumber(1)
  Transaction_Transfer ensureTransfer() => $_ensure(0);

  @$pb.TagNumber(2)
  Transaction_ERC20Transfer get erc20Transfer => $_getN(1);
  @$pb.TagNumber(2)
  set erc20Transfer(Transaction_ERC20Transfer v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasErc20Transfer() => $_has(1);
  @$pb.TagNumber(2)
  void clearErc20Transfer() => clearField(2);
  @$pb.TagNumber(2)
  Transaction_ERC20Transfer ensureErc20Transfer() => $_ensure(1);

  @$pb.TagNumber(3)
  Transaction_ERC20Approve get erc20Approve => $_getN(2);
  @$pb.TagNumber(3)
  set erc20Approve(Transaction_ERC20Approve v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasErc20Approve() => $_has(2);
  @$pb.TagNumber(3)
  void clearErc20Approve() => clearField(3);
  @$pb.TagNumber(3)
  Transaction_ERC20Approve ensureErc20Approve() => $_ensure(2);

  @$pb.TagNumber(4)
  Transaction_ERC721Transfer get erc721Transfer => $_getN(3);
  @$pb.TagNumber(4)
  set erc721Transfer(Transaction_ERC721Transfer v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasErc721Transfer() => $_has(3);
  @$pb.TagNumber(4)
  void clearErc721Transfer() => clearField(4);
  @$pb.TagNumber(4)
  Transaction_ERC721Transfer ensureErc721Transfer() => $_ensure(3);

  @$pb.TagNumber(5)
  Transaction_ERC1155Transfer get erc1155Transfer => $_getN(4);
  @$pb.TagNumber(5)
  set erc1155Transfer(Transaction_ERC1155Transfer v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasErc1155Transfer() => $_has(4);
  @$pb.TagNumber(5)
  void clearErc1155Transfer() => clearField(5);
  @$pb.TagNumber(5)
  Transaction_ERC1155Transfer ensureErc1155Transfer() => $_ensure(4);

  @$pb.TagNumber(6)
  Transaction_ContractGeneric get contractGeneric => $_getN(5);
  @$pb.TagNumber(6)
  set contractGeneric(Transaction_ContractGeneric v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasContractGeneric() => $_has(5);
  @$pb.TagNumber(6)
  void clearContractGeneric() => clearField(6);
  @$pb.TagNumber(6)
  Transaction_ContractGeneric ensureContractGeneric() => $_ensure(5);

  @$pb.TagNumber(7)
  Transaction_Batch get batch => $_getN(6);
  @$pb.TagNumber(7)
  set batch(Transaction_Batch v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasBatch() => $_has(6);
  @$pb.TagNumber(7)
  void clearBatch() => clearField(7);
  @$pb.TagNumber(7)
  Transaction_Batch ensureBatch() => $_ensure(6);
}

/// ERC-4337 structure that describes a transaction to be sent on behalf of a user
class UserOperation extends $pb.GeneratedMessage {
  factory UserOperation({
    $core.String? entryPoint,
    $core.List<$core.int>? initCode,
    $core.String? sender,
    $core.List<$core.int>? preVerificationGas,
    $core.List<$core.int>? verificationGasLimit,
    $core.List<$core.int>? paymasterAndData,
  }) {
    final $result = create();
    if (entryPoint != null) {
      $result.entryPoint = entryPoint;
    }
    if (initCode != null) {
      $result.initCode = initCode;
    }
    if (sender != null) {
      $result.sender = sender;
    }
    if (preVerificationGas != null) {
      $result.preVerificationGas = preVerificationGas;
    }
    if (verificationGasLimit != null) {
      $result.verificationGasLimit = verificationGasLimit;
    }
    if (paymasterAndData != null) {
      $result.paymasterAndData = paymasterAndData;
    }
    return $result;
  }
  UserOperation._() : super();
  factory UserOperation.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory UserOperation.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'UserOperation', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'entryPoint')
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'initCode', $pb.PbFieldType.OY)
    ..aOS(3, _omitFieldNames ? '' : 'sender')
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'preVerificationGas', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(5, _omitFieldNames ? '' : 'verificationGasLimit', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(6, _omitFieldNames ? '' : 'paymasterAndData', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  UserOperation clone() => UserOperation()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  UserOperation copyWith(void Function(UserOperation) updates) => super.copyWith((message) => updates(message as UserOperation)) as UserOperation;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static UserOperation create() => UserOperation._();
  UserOperation createEmptyInstance() => create();
  static $pb.PbList<UserOperation> createRepeated() => $pb.PbList<UserOperation>();
  @$core.pragma('dart2js:noInline')
  static UserOperation getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<UserOperation>(create);
  static UserOperation? _defaultInstance;

  /// Entry point contract address
  @$pb.TagNumber(1)
  $core.String get entryPoint => $_getSZ(0);
  @$pb.TagNumber(1)
  set entryPoint($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasEntryPoint() => $_has(0);
  @$pb.TagNumber(1)
  void clearEntryPoint() => clearField(1);

  /// Account factory contract address
  @$pb.TagNumber(2)
  $core.List<$core.int> get initCode => $_getN(1);
  @$pb.TagNumber(2)
  set initCode($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasInitCode() => $_has(1);
  @$pb.TagNumber(2)
  void clearInitCode() => clearField(2);

  /// Account logic contract address
  @$pb.TagNumber(3)
  $core.String get sender => $_getSZ(2);
  @$pb.TagNumber(3)
  set sender($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSender() => $_has(2);
  @$pb.TagNumber(3)
  void clearSender() => clearField(3);

  /// The amount of gas to pay for to compensate the bundler for pre-verification execution and calldata
  @$pb.TagNumber(4)
  $core.List<$core.int> get preVerificationGas => $_getN(3);
  @$pb.TagNumber(4)
  set preVerificationGas($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasPreVerificationGas() => $_has(3);
  @$pb.TagNumber(4)
  void clearPreVerificationGas() => clearField(4);

  /// The amount of gas to allocate for the verification step
  @$pb.TagNumber(5)
  $core.List<$core.int> get verificationGasLimit => $_getN(4);
  @$pb.TagNumber(5)
  set verificationGasLimit($core.List<$core.int> v) { $_setBytes(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasVerificationGasLimit() => $_has(4);
  @$pb.TagNumber(5)
  void clearVerificationGasLimit() => clearField(5);

  /// Address of paymaster sponsoring the transaction, followed by extra data to send to the paymaster (empty for self-sponsored transaction)
  @$pb.TagNumber(6)
  $core.List<$core.int> get paymasterAndData => $_getN(5);
  @$pb.TagNumber(6)
  set paymasterAndData($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasPaymasterAndData() => $_has(5);
  @$pb.TagNumber(6)
  void clearPaymasterAndData() => clearField(6);
}

/// Input data necessary to create a signed transaction.
/// Legacy and EIP2718/EIP1559 transactions supported, see TransactionMode.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.List<$core.int>? chainId,
    $core.List<$core.int>? nonce,
    TransactionMode? txMode,
    $core.List<$core.int>? gasPrice,
    $core.List<$core.int>? gasLimit,
    $core.List<$core.int>? maxInclusionFeePerGas,
    $core.List<$core.int>? maxFeePerGas,
    $core.String? toAddress,
    $core.List<$core.int>? privateKey,
    Transaction? transaction,
    UserOperation? userOperation,
  }) {
    final $result = create();
    if (chainId != null) {
      $result.chainId = chainId;
    }
    if (nonce != null) {
      $result.nonce = nonce;
    }
    if (txMode != null) {
      $result.txMode = txMode;
    }
    if (gasPrice != null) {
      $result.gasPrice = gasPrice;
    }
    if (gasLimit != null) {
      $result.gasLimit = gasLimit;
    }
    if (maxInclusionFeePerGas != null) {
      $result.maxInclusionFeePerGas = maxInclusionFeePerGas;
    }
    if (maxFeePerGas != null) {
      $result.maxFeePerGas = maxFeePerGas;
    }
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (transaction != null) {
      $result.transaction = transaction;
    }
    if (userOperation != null) {
      $result.userOperation = userOperation;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'chainId', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'nonce', $pb.PbFieldType.OY)
    ..e<TransactionMode>(3, _omitFieldNames ? '' : 'txMode', $pb.PbFieldType.OE, defaultOrMaker: TransactionMode.Legacy, valueOf: TransactionMode.valueOf, enumValues: TransactionMode.values)
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'gasPrice', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(5, _omitFieldNames ? '' : 'gasLimit', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(6, _omitFieldNames ? '' : 'maxInclusionFeePerGas', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(7, _omitFieldNames ? '' : 'maxFeePerGas', $pb.PbFieldType.OY)
    ..aOS(8, _omitFieldNames ? '' : 'toAddress')
    ..a<$core.List<$core.int>>(9, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..aOM<Transaction>(10, _omitFieldNames ? '' : 'transaction', subBuilder: Transaction.create)
    ..aOM<UserOperation>(11, _omitFieldNames ? '' : 'userOperation', subBuilder: UserOperation.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SigningInput clone() => SigningInput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SigningInput copyWith(void Function(SigningInput) updates) => super.copyWith((message) => updates(message as SigningInput)) as SigningInput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SigningInput create() => SigningInput._();
  SigningInput createEmptyInstance() => create();
  static $pb.PbList<SigningInput> createRepeated() => $pb.PbList<SigningInput>();
  @$core.pragma('dart2js:noInline')
  static SigningInput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SigningInput>(create);
  static SigningInput? _defaultInstance;

  /// Chain identifier (uint256, serialized big endian)
  @$pb.TagNumber(1)
  $core.List<$core.int> get chainId => $_getN(0);
  @$pb.TagNumber(1)
  set chainId($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasChainId() => $_has(0);
  @$pb.TagNumber(1)
  void clearChainId() => clearField(1);

  /// Nonce (uint256, serialized big endian)
  @$pb.TagNumber(2)
  $core.List<$core.int> get nonce => $_getN(1);
  @$pb.TagNumber(2)
  set nonce($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasNonce() => $_has(1);
  @$pb.TagNumber(2)
  void clearNonce() => clearField(2);

  /// Transaction version selector: Legacy or enveloped, has impact on fee structure.
  /// Default is Legacy (value 0)
  @$pb.TagNumber(3)
  TransactionMode get txMode => $_getN(2);
  @$pb.TagNumber(3)
  set txMode(TransactionMode v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasTxMode() => $_has(2);
  @$pb.TagNumber(3)
  void clearTxMode() => clearField(3);

  /// Gas price (uint256, serialized big endian)
  /// Relevant for legacy transactions only (disregarded for enveloped/EIP1559)
  @$pb.TagNumber(4)
  $core.List<$core.int> get gasPrice => $_getN(3);
  @$pb.TagNumber(4)
  set gasPrice($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasGasPrice() => $_has(3);
  @$pb.TagNumber(4)
  void clearGasPrice() => clearField(4);

  /// Gas limit (uint256, serialized big endian)
  @$pb.TagNumber(5)
  $core.List<$core.int> get gasLimit => $_getN(4);
  @$pb.TagNumber(5)
  set gasLimit($core.List<$core.int> v) { $_setBytes(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasGasLimit() => $_has(4);
  @$pb.TagNumber(5)
  void clearGasLimit() => clearField(5);

  /// Maximum optional inclusion fee (aka tip) (uint256, serialized big endian)
  /// Relevant for enveloped/EIP1559 transactions only, tx_mode=Enveloped, (disregarded for legacy)
  @$pb.TagNumber(6)
  $core.List<$core.int> get maxInclusionFeePerGas => $_getN(5);
  @$pb.TagNumber(6)
  set maxInclusionFeePerGas($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasMaxInclusionFeePerGas() => $_has(5);
  @$pb.TagNumber(6)
  void clearMaxInclusionFeePerGas() => clearField(6);

  /// Maximum fee (uint256, serialized big endian)
  /// Relevant for enveloped/EIP1559 transactions only, tx_mode=Enveloped, (disregarded for legacy)
  @$pb.TagNumber(7)
  $core.List<$core.int> get maxFeePerGas => $_getN(6);
  @$pb.TagNumber(7)
  set maxFeePerGas($core.List<$core.int> v) { $_setBytes(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasMaxFeePerGas() => $_has(6);
  @$pb.TagNumber(7)
  void clearMaxFeePerGas() => clearField(7);

  /// Recipient's address.
  @$pb.TagNumber(8)
  $core.String get toAddress => $_getSZ(7);
  @$pb.TagNumber(8)
  set toAddress($core.String v) { $_setString(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasToAddress() => $_has(7);
  @$pb.TagNumber(8)
  void clearToAddress() => clearField(8);

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(9)
  $core.List<$core.int> get privateKey => $_getN(8);
  @$pb.TagNumber(9)
  set privateKey($core.List<$core.int> v) { $_setBytes(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasPrivateKey() => $_has(8);
  @$pb.TagNumber(9)
  void clearPrivateKey() => clearField(9);

  /// The payload transaction
  @$pb.TagNumber(10)
  Transaction get transaction => $_getN(9);
  @$pb.TagNumber(10)
  set transaction(Transaction v) { setField(10, v); }
  @$pb.TagNumber(10)
  $core.bool hasTransaction() => $_has(9);
  @$pb.TagNumber(10)
  void clearTransaction() => clearField(10);
  @$pb.TagNumber(10)
  Transaction ensureTransaction() => $_ensure(9);

  /// UserOperation for ERC-4337 wallets
  @$pb.TagNumber(11)
  UserOperation get userOperation => $_getN(10);
  @$pb.TagNumber(11)
  set userOperation(UserOperation v) { setField(11, v); }
  @$pb.TagNumber(11)
  $core.bool hasUserOperation() => $_has(10);
  @$pb.TagNumber(11)
  void clearUserOperation() => clearField(11);
  @$pb.TagNumber(11)
  UserOperation ensureUserOperation() => $_ensure(10);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.List<$core.int>? encoded,
    $core.List<$core.int>? v,
    $core.List<$core.int>? r,
    $core.List<$core.int>? s,
    $core.List<$core.int>? data,
    $0.SigningError? error,
    $core.String? errorMessage,
    $core.List<$core.int>? preHash,
  }) {
    final $result = create();
    if (encoded != null) {
      $result.encoded = encoded;
    }
    if (v != null) {
      $result.v = v;
    }
    if (r != null) {
      $result.r = r;
    }
    if (s != null) {
      $result.s = s;
    }
    if (data != null) {
      $result.data = data;
    }
    if (error != null) {
      $result.error = error;
    }
    if (errorMessage != null) {
      $result.errorMessage = errorMessage;
    }
    if (preHash != null) {
      $result.preHash = preHash;
    }
    return $result;
  }
  SigningOutput._() : super();
  factory SigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'encoded', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'v', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'r', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 's', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(5, _omitFieldNames ? '' : 'data', $pb.PbFieldType.OY)
    ..e<$0.SigningError>(6, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(7, _omitFieldNames ? '' : 'errorMessage')
    ..a<$core.List<$core.int>>(8, _omitFieldNames ? '' : 'preHash', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SigningOutput clone() => SigningOutput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SigningOutput copyWith(void Function(SigningOutput) updates) => super.copyWith((message) => updates(message as SigningOutput)) as SigningOutput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SigningOutput create() => SigningOutput._();
  SigningOutput createEmptyInstance() => create();
  static $pb.PbList<SigningOutput> createRepeated() => $pb.PbList<SigningOutput>();
  @$core.pragma('dart2js:noInline')
  static SigningOutput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SigningOutput>(create);
  static SigningOutput? _defaultInstance;

  /// Signed and encoded transaction bytes.
  @$pb.TagNumber(1)
  $core.List<$core.int> get encoded => $_getN(0);
  @$pb.TagNumber(1)
  set encoded($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasEncoded() => $_has(0);
  @$pb.TagNumber(1)
  void clearEncoded() => clearField(1);

  /// The V, R, S components of the resulting signature, (each uint256, serialized big endian)
  @$pb.TagNumber(2)
  $core.List<$core.int> get v => $_getN(1);
  @$pb.TagNumber(2)
  set v($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasV() => $_has(1);
  @$pb.TagNumber(2)
  void clearV() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<$core.int> get r => $_getN(2);
  @$pb.TagNumber(3)
  set r($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasR() => $_has(2);
  @$pb.TagNumber(3)
  void clearR() => clearField(3);

  @$pb.TagNumber(4)
  $core.List<$core.int> get s => $_getN(3);
  @$pb.TagNumber(4)
  set s($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasS() => $_has(3);
  @$pb.TagNumber(4)
  void clearS() => clearField(4);

  /// The payload part, supplied in the input or assembled from input parameters
  @$pb.TagNumber(5)
  $core.List<$core.int> get data => $_getN(4);
  @$pb.TagNumber(5)
  set data($core.List<$core.int> v) { $_setBytes(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasData() => $_has(4);
  @$pb.TagNumber(5)
  void clearData() => clearField(5);

  /// error code, 0 is ok, other codes will be treated as errors
  @$pb.TagNumber(6)
  $0.SigningError get error => $_getN(5);
  @$pb.TagNumber(6)
  set error($0.SigningError v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasError() => $_has(5);
  @$pb.TagNumber(6)
  void clearError() => clearField(6);

  /// error code description
  @$pb.TagNumber(7)
  $core.String get errorMessage => $_getSZ(6);
  @$pb.TagNumber(7)
  set errorMessage($core.String v) { $_setString(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasErrorMessage() => $_has(6);
  @$pb.TagNumber(7)
  void clearErrorMessage() => clearField(7);

  /// Encoded transaction bytes.
  @$pb.TagNumber(8)
  $core.List<$core.int> get preHash => $_getN(7);
  @$pb.TagNumber(8)
  set preHash($core.List<$core.int> v) { $_setBytes(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasPreHash() => $_has(7);
  @$pb.TagNumber(8)
  void clearPreHash() => clearField(8);
}

class MaybeChainId extends $pb.GeneratedMessage {
  factory MaybeChainId({
    $fixnum.Int64? chainId,
  }) {
    final $result = create();
    if (chainId != null) {
      $result.chainId = chainId;
    }
    return $result;
  }
  MaybeChainId._() : super();
  factory MaybeChainId.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory MaybeChainId.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'MaybeChainId', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'chainId', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  MaybeChainId clone() => MaybeChainId()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  MaybeChainId copyWith(void Function(MaybeChainId) updates) => super.copyWith((message) => updates(message as MaybeChainId)) as MaybeChainId;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static MaybeChainId create() => MaybeChainId._();
  MaybeChainId createEmptyInstance() => create();
  static $pb.PbList<MaybeChainId> createRepeated() => $pb.PbList<MaybeChainId>();
  @$core.pragma('dart2js:noInline')
  static MaybeChainId getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<MaybeChainId>(create);
  static MaybeChainId? _defaultInstance;

  /// Chain ID.
  @$pb.TagNumber(3)
  $fixnum.Int64 get chainId => $_getI64(0);
  @$pb.TagNumber(3)
  set chainId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(3)
  $core.bool hasChainId() => $_has(0);
  @$pb.TagNumber(3)
  void clearChainId() => clearField(3);
}

class MessageSigningInput extends $pb.GeneratedMessage {
  factory MessageSigningInput({
    $core.List<$core.int>? privateKey,
    $core.String? message,
    MaybeChainId? chainId,
    MessageType? messageType,
  }) {
    final $result = create();
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (message != null) {
      $result.message = message;
    }
    if (chainId != null) {
      $result.chainId = chainId;
    }
    if (messageType != null) {
      $result.messageType = messageType;
    }
    return $result;
  }
  MessageSigningInput._() : super();
  factory MessageSigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory MessageSigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'MessageSigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..aOS(2, _omitFieldNames ? '' : 'message')
    ..aOM<MaybeChainId>(3, _omitFieldNames ? '' : 'chainId', subBuilder: MaybeChainId.create)
    ..e<MessageType>(4, _omitFieldNames ? '' : 'messageType', $pb.PbFieldType.OE, defaultOrMaker: MessageType.MessageType_legacy, valueOf: MessageType.valueOf, enumValues: MessageType.values)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  MessageSigningInput clone() => MessageSigningInput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  MessageSigningInput copyWith(void Function(MessageSigningInput) updates) => super.copyWith((message) => updates(message as MessageSigningInput)) as MessageSigningInput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static MessageSigningInput create() => MessageSigningInput._();
  MessageSigningInput createEmptyInstance() => create();
  static $pb.PbList<MessageSigningInput> createRepeated() => $pb.PbList<MessageSigningInput>();
  @$core.pragma('dart2js:noInline')
  static MessageSigningInput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<MessageSigningInput>(create);
  static MessageSigningInput? _defaultInstance;

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(1)
  $core.List<$core.int> get privateKey => $_getN(0);
  @$pb.TagNumber(1)
  set privateKey($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPrivateKey() => $_has(0);
  @$pb.TagNumber(1)
  void clearPrivateKey() => clearField(1);

  /// Message to sign. Either a regular message or a typed data structured message in JSON format.
  /// Message type should be declared at `message_type`.
  @$pb.TagNumber(2)
  $core.String get message => $_getSZ(1);
  @$pb.TagNumber(2)
  set message($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasMessage() => $_has(1);
  @$pb.TagNumber(2)
  void clearMessage() => clearField(2);

  /// Optional. Used in replay protection and to check Typed Structured Data input.
  /// Eg. should be set if `message_type` is `MessageType_eip155`, or MessageType_typed, or `MessageType_typed_eip155`.
  @$pb.TagNumber(3)
  MaybeChainId get chainId => $_getN(2);
  @$pb.TagNumber(3)
  set chainId(MaybeChainId v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasChainId() => $_has(2);
  @$pb.TagNumber(3)
  void clearChainId() => clearField(3);
  @$pb.TagNumber(3)
  MaybeChainId ensureChainId() => $_ensure(2);

  /// Message type.
  @$pb.TagNumber(4)
  MessageType get messageType => $_getN(3);
  @$pb.TagNumber(4)
  set messageType(MessageType v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasMessageType() => $_has(3);
  @$pb.TagNumber(4)
  void clearMessageType() => clearField(4);
}

class MessageSigningOutput extends $pb.GeneratedMessage {
  factory MessageSigningOutput({
    $core.String? signature,
    $0.SigningError? error,
    $core.String? errorMessage,
  }) {
    final $result = create();
    if (signature != null) {
      $result.signature = signature;
    }
    if (error != null) {
      $result.error = error;
    }
    if (errorMessage != null) {
      $result.errorMessage = errorMessage;
    }
    return $result;
  }
  MessageSigningOutput._() : super();
  factory MessageSigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory MessageSigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'MessageSigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'signature')
    ..e<$0.SigningError>(2, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(3, _omitFieldNames ? '' : 'errorMessage')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  MessageSigningOutput clone() => MessageSigningOutput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  MessageSigningOutput copyWith(void Function(MessageSigningOutput) updates) => super.copyWith((message) => updates(message as MessageSigningOutput)) as MessageSigningOutput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static MessageSigningOutput create() => MessageSigningOutput._();
  MessageSigningOutput createEmptyInstance() => create();
  static $pb.PbList<MessageSigningOutput> createRepeated() => $pb.PbList<MessageSigningOutput>();
  @$core.pragma('dart2js:noInline')
  static MessageSigningOutput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<MessageSigningOutput>(create);
  static MessageSigningOutput? _defaultInstance;

  /// The signature, Hex-encoded.
  @$pb.TagNumber(1)
  $core.String get signature => $_getSZ(0);
  @$pb.TagNumber(1)
  set signature($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSignature() => $_has(0);
  @$pb.TagNumber(1)
  void clearSignature() => clearField(1);

  /// error code, 0 is ok, other codes will be treated as errors
  @$pb.TagNumber(2)
  $0.SigningError get error => $_getN(1);
  @$pb.TagNumber(2)
  set error($0.SigningError v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasError() => $_has(1);
  @$pb.TagNumber(2)
  void clearError() => clearField(2);

  /// error code description
  @$pb.TagNumber(3)
  $core.String get errorMessage => $_getSZ(2);
  @$pb.TagNumber(3)
  set errorMessage($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasErrorMessage() => $_has(2);
  @$pb.TagNumber(3)
  void clearErrorMessage() => clearField(3);
}

class MessageVerifyingInput extends $pb.GeneratedMessage {
  factory MessageVerifyingInput({
    $core.String? message,
    $core.List<$core.int>? publicKey,
    $core.String? signature,
  }) {
    final $result = create();
    if (message != null) {
      $result.message = message;
    }
    if (publicKey != null) {
      $result.publicKey = publicKey;
    }
    if (signature != null) {
      $result.signature = signature;
    }
    return $result;
  }
  MessageVerifyingInput._() : super();
  factory MessageVerifyingInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory MessageVerifyingInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'MessageVerifyingInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ethereum.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'message')
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'publicKey', $pb.PbFieldType.OY)
    ..aOS(3, _omitFieldNames ? '' : 'signature')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  MessageVerifyingInput clone() => MessageVerifyingInput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  MessageVerifyingInput copyWith(void Function(MessageVerifyingInput) updates) => super.copyWith((message) => updates(message as MessageVerifyingInput)) as MessageVerifyingInput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static MessageVerifyingInput create() => MessageVerifyingInput._();
  MessageVerifyingInput createEmptyInstance() => create();
  static $pb.PbList<MessageVerifyingInput> createRepeated() => $pb.PbList<MessageVerifyingInput>();
  @$core.pragma('dart2js:noInline')
  static MessageVerifyingInput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<MessageVerifyingInput>(create);
  static MessageVerifyingInput? _defaultInstance;

  /// The message signed.
  @$pb.TagNumber(1)
  $core.String get message => $_getSZ(0);
  @$pb.TagNumber(1)
  set message($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasMessage() => $_has(0);
  @$pb.TagNumber(1)
  void clearMessage() => clearField(1);

  /// Public key that will verify and recover the message from the signature.
  @$pb.TagNumber(2)
  $core.List<$core.int> get publicKey => $_getN(1);
  @$pb.TagNumber(2)
  set publicKey($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPublicKey() => $_has(1);
  @$pb.TagNumber(2)
  void clearPublicKey() => clearField(2);

  /// The signature, Hex-encoded.
  @$pb.TagNumber(3)
  $core.String get signature => $_getSZ(2);
  @$pb.TagNumber(3)
  set signature($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSignature() => $_has(2);
  @$pb.TagNumber(3)
  void clearSignature() => clearField(3);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
