//
//  Generated code. Do not modify.
//  source: Nebulas.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.String? fromAddress,
    $core.List<$core.int>? chainId,
    $core.List<$core.int>? nonce,
    $core.List<$core.int>? gasPrice,
    $core.List<$core.int>? gasLimit,
    $core.String? toAddress,
    $core.List<$core.int>? amount,
    $core.List<$core.int>? timestamp,
    $core.String? payload,
    $core.List<$core.int>? privateKey,
  }) {
    final $result = create();
    if (fromAddress != null) {
      $result.fromAddress = fromAddress;
    }
    if (chainId != null) {
      $result.chainId = chainId;
    }
    if (nonce != null) {
      $result.nonce = nonce;
    }
    if (gasPrice != null) {
      $result.gasPrice = gasPrice;
    }
    if (gasLimit != null) {
      $result.gasLimit = gasLimit;
    }
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (timestamp != null) {
      $result.timestamp = timestamp;
    }
    if (payload != null) {
      $result.payload = payload;
    }
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nebulas.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'fromAddress')
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'chainId', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'nonce', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'gasPrice', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(5, _omitFieldNames ? '' : 'gasLimit', $pb.PbFieldType.OY)
    ..aOS(6, _omitFieldNames ? '' : 'toAddress')
    ..a<$core.List<$core.int>>(7, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(8, _omitFieldNames ? '' : 'timestamp', $pb.PbFieldType.OY)
    ..aOS(9, _omitFieldNames ? '' : 'payload')
    ..a<$core.List<$core.int>>(10, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
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

  /// sender's address.
  @$pb.TagNumber(1)
  $core.String get fromAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set fromAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFromAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearFromAddress() => clearField(1);

  /// Chain identifier (uint256, serialized big endian)
  @$pb.TagNumber(2)
  $core.List<$core.int> get chainId => $_getN(1);
  @$pb.TagNumber(2)
  set chainId($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasChainId() => $_has(1);
  @$pb.TagNumber(2)
  void clearChainId() => clearField(2);

  /// Nonce (uint256, serialized big endian)
  @$pb.TagNumber(3)
  $core.List<$core.int> get nonce => $_getN(2);
  @$pb.TagNumber(3)
  set nonce($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasNonce() => $_has(2);
  @$pb.TagNumber(3)
  void clearNonce() => clearField(3);

  /// Gas price (uint256, serialized big endian)
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

  /// Recipient's address.
  @$pb.TagNumber(6)
  $core.String get toAddress => $_getSZ(5);
  @$pb.TagNumber(6)
  set toAddress($core.String v) { $_setString(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasToAddress() => $_has(5);
  @$pb.TagNumber(6)
  void clearToAddress() => clearField(6);

  /// Amount to send in wei, 1 NAS = 10^18 Wei (uint256, serialized big endian)
  @$pb.TagNumber(7)
  $core.List<$core.int> get amount => $_getN(6);
  @$pb.TagNumber(7)
  set amount($core.List<$core.int> v) { $_setBytes(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasAmount() => $_has(6);
  @$pb.TagNumber(7)
  void clearAmount() => clearField(7);

  /// Timestamp to create transaction (uint256, serialized big endian)
  @$pb.TagNumber(8)
  $core.List<$core.int> get timestamp => $_getN(7);
  @$pb.TagNumber(8)
  set timestamp($core.List<$core.int> v) { $_setBytes(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasTimestamp() => $_has(7);
  @$pb.TagNumber(8)
  void clearTimestamp() => clearField(8);

  /// Optional payload
  @$pb.TagNumber(9)
  $core.String get payload => $_getSZ(8);
  @$pb.TagNumber(9)
  set payload($core.String v) { $_setString(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasPayload() => $_has(8);
  @$pb.TagNumber(9)
  void clearPayload() => clearField(9);

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(10)
  $core.List<$core.int> get privateKey => $_getN(9);
  @$pb.TagNumber(10)
  set privateKey($core.List<$core.int> v) { $_setBytes(9, v); }
  @$pb.TagNumber(10)
  $core.bool hasPrivateKey() => $_has(9);
  @$pb.TagNumber(10)
  void clearPrivateKey() => clearField(10);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.int? algorithm,
    $core.List<$core.int>? signature,
    $core.String? raw,
  }) {
    final $result = create();
    if (algorithm != null) {
      $result.algorithm = algorithm;
    }
    if (signature != null) {
      $result.signature = signature;
    }
    if (raw != null) {
      $result.raw = raw;
    }
    return $result;
  }
  SigningOutput._() : super();
  factory SigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nebulas.Proto'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'algorithm', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'signature', $pb.PbFieldType.OY)
    ..aOS(3, _omitFieldNames ? '' : 'raw')
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

  /// Algorithm code
  @$pb.TagNumber(1)
  $core.int get algorithm => $_getIZ(0);
  @$pb.TagNumber(1)
  set algorithm($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAlgorithm() => $_has(0);
  @$pb.TagNumber(1)
  void clearAlgorithm() => clearField(1);

  /// The signature
  @$pb.TagNumber(2)
  $core.List<$core.int> get signature => $_getN(1);
  @$pb.TagNumber(2)
  set signature($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasSignature() => $_has(1);
  @$pb.TagNumber(2)
  void clearSignature() => clearField(2);

  /// Encoded transaction
  @$pb.TagNumber(3)
  $core.String get raw => $_getSZ(2);
  @$pb.TagNumber(3)
  set raw($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasRaw() => $_has(2);
  @$pb.TagNumber(3)
  void clearRaw() => clearField(3);
}

/// Generic data
class Data extends $pb.GeneratedMessage {
  factory Data({
    $core.String? type,
    $core.List<$core.int>? payload,
  }) {
    final $result = create();
    if (type != null) {
      $result.type = type;
    }
    if (payload != null) {
      $result.payload = payload;
    }
    return $result;
  }
  Data._() : super();
  factory Data.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Data.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Data', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nebulas.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'type')
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'payload', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Data clone() => Data()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Data copyWith(void Function(Data) updates) => super.copyWith((message) => updates(message as Data)) as Data;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Data create() => Data._();
  Data createEmptyInstance() => create();
  static $pb.PbList<Data> createRepeated() => $pb.PbList<Data>();
  @$core.pragma('dart2js:noInline')
  static Data getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Data>(create);
  static Data? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get type => $_getSZ(0);
  @$pb.TagNumber(1)
  set type($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasType() => $_has(0);
  @$pb.TagNumber(1)
  void clearType() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<$core.int> get payload => $_getN(1);
  @$pb.TagNumber(2)
  set payload($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPayload() => $_has(1);
  @$pb.TagNumber(2)
  void clearPayload() => clearField(2);
}

/// Raw transaction data
class RawTransaction extends $pb.GeneratedMessage {
  factory RawTransaction({
    $core.List<$core.int>? hash,
    $core.List<$core.int>? from,
    $core.List<$core.int>? to,
    $core.List<$core.int>? value,
    $fixnum.Int64? nonce,
    $fixnum.Int64? timestamp,
    Data? data,
    $core.int? chainId,
    $core.List<$core.int>? gasPrice,
    $core.List<$core.int>? gasLimit,
    $core.int? alg,
    $core.List<$core.int>? sign,
  }) {
    final $result = create();
    if (hash != null) {
      $result.hash = hash;
    }
    if (from != null) {
      $result.from = from;
    }
    if (to != null) {
      $result.to = to;
    }
    if (value != null) {
      $result.value = value;
    }
    if (nonce != null) {
      $result.nonce = nonce;
    }
    if (timestamp != null) {
      $result.timestamp = timestamp;
    }
    if (data != null) {
      $result.data = data;
    }
    if (chainId != null) {
      $result.chainId = chainId;
    }
    if (gasPrice != null) {
      $result.gasPrice = gasPrice;
    }
    if (gasLimit != null) {
      $result.gasLimit = gasLimit;
    }
    if (alg != null) {
      $result.alg = alg;
    }
    if (sign != null) {
      $result.sign = sign;
    }
    return $result;
  }
  RawTransaction._() : super();
  factory RawTransaction.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RawTransaction.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'RawTransaction', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nebulas.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'hash', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'from', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'to', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OY)
    ..a<$fixnum.Int64>(5, _omitFieldNames ? '' : 'nonce', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aInt64(6, _omitFieldNames ? '' : 'timestamp')
    ..aOM<Data>(7, _omitFieldNames ? '' : 'data', subBuilder: Data.create)
    ..a<$core.int>(8, _omitFieldNames ? '' : 'chainId', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(9, _omitFieldNames ? '' : 'gasPrice', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(10, _omitFieldNames ? '' : 'gasLimit', $pb.PbFieldType.OY)
    ..a<$core.int>(11, _omitFieldNames ? '' : 'alg', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(12, _omitFieldNames ? '' : 'sign', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  RawTransaction clone() => RawTransaction()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  RawTransaction copyWith(void Function(RawTransaction) updates) => super.copyWith((message) => updates(message as RawTransaction)) as RawTransaction;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static RawTransaction create() => RawTransaction._();
  RawTransaction createEmptyInstance() => create();
  static $pb.PbList<RawTransaction> createRepeated() => $pb.PbList<RawTransaction>();
  @$core.pragma('dart2js:noInline')
  static RawTransaction getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RawTransaction>(create);
  static RawTransaction? _defaultInstance;

  /// tx hash
  @$pb.TagNumber(1)
  $core.List<$core.int> get hash => $_getN(0);
  @$pb.TagNumber(1)
  set hash($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasHash() => $_has(0);
  @$pb.TagNumber(1)
  void clearHash() => clearField(1);

  /// source address
  @$pb.TagNumber(2)
  $core.List<$core.int> get from => $_getN(1);
  @$pb.TagNumber(2)
  set from($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasFrom() => $_has(1);
  @$pb.TagNumber(2)
  void clearFrom() => clearField(2);

  /// destination address
  @$pb.TagNumber(3)
  $core.List<$core.int> get to => $_getN(2);
  @$pb.TagNumber(3)
  set to($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTo() => $_has(2);
  @$pb.TagNumber(3)
  void clearTo() => clearField(3);

  /// amount (uint256, serialized big endian)
  @$pb.TagNumber(4)
  $core.List<$core.int> get value => $_getN(3);
  @$pb.TagNumber(4)
  set value($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasValue() => $_has(3);
  @$pb.TagNumber(4)
  void clearValue() => clearField(4);

  /// Nonce (should be larger than in the last transaction of the account)
  @$pb.TagNumber(5)
  $fixnum.Int64 get nonce => $_getI64(4);
  @$pb.TagNumber(5)
  set nonce($fixnum.Int64 v) { $_setInt64(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasNonce() => $_has(4);
  @$pb.TagNumber(5)
  void clearNonce() => clearField(5);

  /// transaction timestamp
  @$pb.TagNumber(6)
  $fixnum.Int64 get timestamp => $_getI64(5);
  @$pb.TagNumber(6)
  set timestamp($fixnum.Int64 v) { $_setInt64(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasTimestamp() => $_has(5);
  @$pb.TagNumber(6)
  void clearTimestamp() => clearField(6);

  /// generic data
  @$pb.TagNumber(7)
  Data get data => $_getN(6);
  @$pb.TagNumber(7)
  set data(Data v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasData() => $_has(6);
  @$pb.TagNumber(7)
  void clearData() => clearField(7);
  @$pb.TagNumber(7)
  Data ensureData() => $_ensure(6);

  /// chain ID (4 bytes)
  @$pb.TagNumber(8)
  $core.int get chainId => $_getIZ(7);
  @$pb.TagNumber(8)
  set chainId($core.int v) { $_setUnsignedInt32(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasChainId() => $_has(7);
  @$pb.TagNumber(8)
  void clearChainId() => clearField(8);

  /// gas price (uint256, serialized big endian)
  @$pb.TagNumber(9)
  $core.List<$core.int> get gasPrice => $_getN(8);
  @$pb.TagNumber(9)
  set gasPrice($core.List<$core.int> v) { $_setBytes(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasGasPrice() => $_has(8);
  @$pb.TagNumber(9)
  void clearGasPrice() => clearField(9);

  /// gas limit (uint256, serialized big endian)
  @$pb.TagNumber(10)
  $core.List<$core.int> get gasLimit => $_getN(9);
  @$pb.TagNumber(10)
  set gasLimit($core.List<$core.int> v) { $_setBytes(9, v); }
  @$pb.TagNumber(10)
  $core.bool hasGasLimit() => $_has(9);
  @$pb.TagNumber(10)
  void clearGasLimit() => clearField(10);

  /// algorithm
  @$pb.TagNumber(11)
  $core.int get alg => $_getIZ(10);
  @$pb.TagNumber(11)
  set alg($core.int v) { $_setUnsignedInt32(10, v); }
  @$pb.TagNumber(11)
  $core.bool hasAlg() => $_has(10);
  @$pb.TagNumber(11)
  void clearAlg() => clearField(11);

  /// signature
  @$pb.TagNumber(12)
  $core.List<$core.int> get sign => $_getN(11);
  @$pb.TagNumber(12)
  set sign($core.List<$core.int> v) { $_setBytes(11, v); }
  @$pb.TagNumber(12)
  $core.bool hasSign() => $_has(11);
  @$pb.TagNumber(12)
  void clearSign() => clearField(12);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
