//
//  Generated code. Do not modify.
//  source: EOS.proto
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
import 'EOS.pbenum.dart';

export 'EOS.pbenum.dart';

/// Values for an Asset object.
class Asset extends $pb.GeneratedMessage {
  factory Asset({
    $fixnum.Int64? amount,
    $core.int? decimals,
    $core.String? symbol,
  }) {
    final $result = create();
    if (amount != null) {
      $result.amount = amount;
    }
    if (decimals != null) {
      $result.decimals = decimals;
    }
    if (symbol != null) {
      $result.symbol = symbol;
    }
    return $result;
  }
  Asset._() : super();
  factory Asset.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Asset.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Asset', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.EOS.Proto'), createEmptyInstance: create)
    ..aInt64(1, _omitFieldNames ? '' : 'amount')
    ..a<$core.int>(2, _omitFieldNames ? '' : 'decimals', $pb.PbFieldType.OU3)
    ..aOS(3, _omitFieldNames ? '' : 'symbol')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Asset clone() => Asset()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Asset copyWith(void Function(Asset) updates) => super.copyWith((message) => updates(message as Asset)) as Asset;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Asset create() => Asset._();
  Asset createEmptyInstance() => create();
  static $pb.PbList<Asset> createRepeated() => $pb.PbList<Asset>();
  @$core.pragma('dart2js:noInline')
  static Asset getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Asset>(create);
  static Asset? _defaultInstance;

  /// Total amount
  @$pb.TagNumber(1)
  $fixnum.Int64 get amount => $_getI64(0);
  @$pb.TagNumber(1)
  set amount($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAmount() => $_has(0);
  @$pb.TagNumber(1)
  void clearAmount() => clearField(1);

  /// Number of decimals defined
  @$pb.TagNumber(2)
  $core.int get decimals => $_getIZ(1);
  @$pb.TagNumber(2)
  set decimals($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasDecimals() => $_has(1);
  @$pb.TagNumber(2)
  void clearDecimals() => clearField(2);

  /// Asset symbol
  @$pb.TagNumber(3)
  $core.String get symbol => $_getSZ(2);
  @$pb.TagNumber(3)
  set symbol($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSymbol() => $_has(2);
  @$pb.TagNumber(3)
  void clearSymbol() => clearField(3);
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.List<$core.int>? chainId,
    $core.List<$core.int>? referenceBlockId,
    $core.int? referenceBlockTime,
    $core.String? currency,
    $core.String? sender,
    $core.String? recipient,
    $core.String? memo,
    Asset? asset,
    $core.List<$core.int>? privateKey,
    KeyType? privateKeyType,
    $core.int? expiration,
  }) {
    final $result = create();
    if (chainId != null) {
      $result.chainId = chainId;
    }
    if (referenceBlockId != null) {
      $result.referenceBlockId = referenceBlockId;
    }
    if (referenceBlockTime != null) {
      $result.referenceBlockTime = referenceBlockTime;
    }
    if (currency != null) {
      $result.currency = currency;
    }
    if (sender != null) {
      $result.sender = sender;
    }
    if (recipient != null) {
      $result.recipient = recipient;
    }
    if (memo != null) {
      $result.memo = memo;
    }
    if (asset != null) {
      $result.asset = asset;
    }
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (privateKeyType != null) {
      $result.privateKeyType = privateKeyType;
    }
    if (expiration != null) {
      $result.expiration = expiration;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.EOS.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'chainId', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'referenceBlockId', $pb.PbFieldType.OY)
    ..a<$core.int>(3, _omitFieldNames ? '' : 'referenceBlockTime', $pb.PbFieldType.OSF3)
    ..aOS(4, _omitFieldNames ? '' : 'currency')
    ..aOS(5, _omitFieldNames ? '' : 'sender')
    ..aOS(6, _omitFieldNames ? '' : 'recipient')
    ..aOS(7, _omitFieldNames ? '' : 'memo')
    ..aOM<Asset>(8, _omitFieldNames ? '' : 'asset', subBuilder: Asset.create)
    ..a<$core.List<$core.int>>(9, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..e<KeyType>(10, _omitFieldNames ? '' : 'privateKeyType', $pb.PbFieldType.OE, defaultOrMaker: KeyType.LEGACY, valueOf: KeyType.valueOf, enumValues: KeyType.values)
    ..a<$core.int>(11, _omitFieldNames ? '' : 'expiration', $pb.PbFieldType.OSF3)
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

  /// Chain id (uint256, serialized big endian)
  @$pb.TagNumber(1)
  $core.List<$core.int> get chainId => $_getN(0);
  @$pb.TagNumber(1)
  set chainId($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasChainId() => $_has(0);
  @$pb.TagNumber(1)
  void clearChainId() => clearField(1);

  /// Reference Block Id (uint256, serialized big endian)
  @$pb.TagNumber(2)
  $core.List<$core.int> get referenceBlockId => $_getN(1);
  @$pb.TagNumber(2)
  set referenceBlockId($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasReferenceBlockId() => $_has(1);
  @$pb.TagNumber(2)
  void clearReferenceBlockId() => clearField(2);

  /// Timestamp on the reference block
  @$pb.TagNumber(3)
  $core.int get referenceBlockTime => $_getIZ(2);
  @$pb.TagNumber(3)
  set referenceBlockTime($core.int v) { $_setSignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasReferenceBlockTime() => $_has(2);
  @$pb.TagNumber(3)
  void clearReferenceBlockTime() => clearField(3);

  /// Currency (e.g. "eosio.token")
  @$pb.TagNumber(4)
  $core.String get currency => $_getSZ(3);
  @$pb.TagNumber(4)
  set currency($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasCurrency() => $_has(3);
  @$pb.TagNumber(4)
  void clearCurrency() => clearField(4);

  /// Sender's username
  @$pb.TagNumber(5)
  $core.String get sender => $_getSZ(4);
  @$pb.TagNumber(5)
  set sender($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasSender() => $_has(4);
  @$pb.TagNumber(5)
  void clearSender() => clearField(5);

  /// Recipient's username
  @$pb.TagNumber(6)
  $core.String get recipient => $_getSZ(5);
  @$pb.TagNumber(6)
  set recipient($core.String v) { $_setString(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasRecipient() => $_has(5);
  @$pb.TagNumber(6)
  void clearRecipient() => clearField(6);

  /// Memo attached to the transaction
  @$pb.TagNumber(7)
  $core.String get memo => $_getSZ(6);
  @$pb.TagNumber(7)
  set memo($core.String v) { $_setString(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasMemo() => $_has(6);
  @$pb.TagNumber(7)
  void clearMemo() => clearField(7);

  /// Asset details and amount
  @$pb.TagNumber(8)
  Asset get asset => $_getN(7);
  @$pb.TagNumber(8)
  set asset(Asset v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasAsset() => $_has(7);
  @$pb.TagNumber(8)
  void clearAsset() => clearField(8);
  @$pb.TagNumber(8)
  Asset ensureAsset() => $_ensure(7);

  /// Sender's secret private key used for signing (32 bytes).
  @$pb.TagNumber(9)
  $core.List<$core.int> get privateKey => $_getN(8);
  @$pb.TagNumber(9)
  set privateKey($core.List<$core.int> v) { $_setBytes(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasPrivateKey() => $_has(8);
  @$pb.TagNumber(9)
  void clearPrivateKey() => clearField(9);

  /// Type of the private key
  @$pb.TagNumber(10)
  KeyType get privateKeyType => $_getN(9);
  @$pb.TagNumber(10)
  set privateKeyType(KeyType v) { setField(10, v); }
  @$pb.TagNumber(10)
  $core.bool hasPrivateKeyType() => $_has(9);
  @$pb.TagNumber(10)
  void clearPrivateKeyType() => clearField(10);

  /// Expiration of the transaction, if not set, default is reference_block_time + 3600 seconds
  @$pb.TagNumber(11)
  $core.int get expiration => $_getIZ(10);
  @$pb.TagNumber(11)
  set expiration($core.int v) { $_setSignedInt32(10, v); }
  @$pb.TagNumber(11)
  $core.bool hasExpiration() => $_has(10);
  @$pb.TagNumber(11)
  void clearExpiration() => clearField(11);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.String? jsonEncoded,
    $0.SigningError? error,
    $core.String? errorMessage,
  }) {
    final $result = create();
    if (jsonEncoded != null) {
      $result.jsonEncoded = jsonEncoded;
    }
    if (error != null) {
      $result.error = error;
    }
    if (errorMessage != null) {
      $result.errorMessage = errorMessage;
    }
    return $result;
  }
  SigningOutput._() : super();
  factory SigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.EOS.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'jsonEncoded')
    ..e<$0.SigningError>(2, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(3, _omitFieldNames ? '' : 'errorMessage')
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

  /// JSON of the packed transaction.
  @$pb.TagNumber(1)
  $core.String get jsonEncoded => $_getSZ(0);
  @$pb.TagNumber(1)
  set jsonEncoded($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasJsonEncoded() => $_has(0);
  @$pb.TagNumber(1)
  void clearJsonEncoded() => clearField(1);

  /// Optional error
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


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
