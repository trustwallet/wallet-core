//
//  Generated code. Do not modify.
//  source: Cardano.proto
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

/// A transaction output that can be used as input
class OutPoint extends $pb.GeneratedMessage {
  factory OutPoint({
    $core.List<$core.int>? txHash,
    $fixnum.Int64? outputIndex,
  }) {
    final $result = create();
    if (txHash != null) {
      $result.txHash = txHash;
    }
    if (outputIndex != null) {
      $result.outputIndex = outputIndex;
    }
    return $result;
  }
  OutPoint._() : super();
  factory OutPoint.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory OutPoint.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'OutPoint', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cardano.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'txHash', $pb.PbFieldType.OY)
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'outputIndex', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  OutPoint clone() => OutPoint()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  OutPoint copyWith(void Function(OutPoint) updates) => super.copyWith((message) => updates(message as OutPoint)) as OutPoint;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static OutPoint create() => OutPoint._();
  OutPoint createEmptyInstance() => create();
  static $pb.PbList<OutPoint> createRepeated() => $pb.PbList<OutPoint>();
  @$core.pragma('dart2js:noInline')
  static OutPoint getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<OutPoint>(create);
  static OutPoint? _defaultInstance;

  /// The transaction ID
  @$pb.TagNumber(1)
  $core.List<$core.int> get txHash => $_getN(0);
  @$pb.TagNumber(1)
  set txHash($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasTxHash() => $_has(0);
  @$pb.TagNumber(1)
  void clearTxHash() => clearField(1);

  /// The index of this output within the transaction
  @$pb.TagNumber(2)
  $fixnum.Int64 get outputIndex => $_getI64(1);
  @$pb.TagNumber(2)
  set outputIndex($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasOutputIndex() => $_has(1);
  @$pb.TagNumber(2)
  void clearOutputIndex() => clearField(2);
}

/// Represents a token and an amount. Token is identified by PolicyID and name.
class TokenAmount extends $pb.GeneratedMessage {
  factory TokenAmount({
    $core.String? policyId,
    $core.String? assetName,
    $core.List<$core.int>? amount,
    $core.String? assetNameHex,
  }) {
    final $result = create();
    if (policyId != null) {
      $result.policyId = policyId;
    }
    if (assetName != null) {
      $result.assetName = assetName;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (assetNameHex != null) {
      $result.assetNameHex = assetNameHex;
    }
    return $result;
  }
  TokenAmount._() : super();
  factory TokenAmount.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TokenAmount.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TokenAmount', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cardano.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'policyId')
    ..aOS(2, _omitFieldNames ? '' : 'assetName')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..aOS(4, _omitFieldNames ? '' : 'assetNameHex')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TokenAmount clone() => TokenAmount()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TokenAmount copyWith(void Function(TokenAmount) updates) => super.copyWith((message) => updates(message as TokenAmount)) as TokenAmount;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TokenAmount create() => TokenAmount._();
  TokenAmount createEmptyInstance() => create();
  static $pb.PbList<TokenAmount> createRepeated() => $pb.PbList<TokenAmount>();
  @$core.pragma('dart2js:noInline')
  static TokenAmount getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TokenAmount>(create);
  static TokenAmount? _defaultInstance;

  /// Policy ID of the token, as hex string (28x2 digits)
  @$pb.TagNumber(1)
  $core.String get policyId => $_getSZ(0);
  @$pb.TagNumber(1)
  set policyId($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPolicyId() => $_has(0);
  @$pb.TagNumber(1)
  void clearPolicyId() => clearField(1);

  /// The name of the asset (within the policy)
  @$pb.TagNumber(2)
  $core.String get assetName => $_getSZ(1);
  @$pb.TagNumber(2)
  set assetName($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAssetName() => $_has(1);
  @$pb.TagNumber(2)
  void clearAssetName() => clearField(2);

  /// The amount (uint256, serialized big endian)
  @$pb.TagNumber(3)
  $core.List<$core.int> get amount => $_getN(2);
  @$pb.TagNumber(3)
  set amount($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);

  /// The name of the asset (hex encoded). Ignored if `asset_name` is set
  @$pb.TagNumber(4)
  $core.String get assetNameHex => $_getSZ(3);
  @$pb.TagNumber(4)
  set assetNameHex($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasAssetNameHex() => $_has(3);
  @$pb.TagNumber(4)
  void clearAssetNameHex() => clearField(4);
}

/// One input for a transaction
class TxInput extends $pb.GeneratedMessage {
  factory TxInput({
    OutPoint? outPoint,
    $core.String? address,
    $fixnum.Int64? amount,
    $core.Iterable<TokenAmount>? tokenAmount,
  }) {
    final $result = create();
    if (outPoint != null) {
      $result.outPoint = outPoint;
    }
    if (address != null) {
      $result.address = address;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (tokenAmount != null) {
      $result.tokenAmount.addAll(tokenAmount);
    }
    return $result;
  }
  TxInput._() : super();
  factory TxInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TxInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TxInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cardano.Proto'), createEmptyInstance: create)
    ..aOM<OutPoint>(1, _omitFieldNames ? '' : 'outPoint', subBuilder: OutPoint.create)
    ..aOS(2, _omitFieldNames ? '' : 'address')
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..pc<TokenAmount>(4, _omitFieldNames ? '' : 'tokenAmount', $pb.PbFieldType.PM, subBuilder: TokenAmount.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TxInput clone() => TxInput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TxInput copyWith(void Function(TxInput) updates) => super.copyWith((message) => updates(message as TxInput)) as TxInput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TxInput create() => TxInput._();
  TxInput createEmptyInstance() => create();
  static $pb.PbList<TxInput> createRepeated() => $pb.PbList<TxInput>();
  @$core.pragma('dart2js:noInline')
  static TxInput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TxInput>(create);
  static TxInput? _defaultInstance;

  /// The UTXO
  @$pb.TagNumber(1)
  OutPoint get outPoint => $_getN(0);
  @$pb.TagNumber(1)
  set outPoint(OutPoint v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasOutPoint() => $_has(0);
  @$pb.TagNumber(1)
  void clearOutPoint() => clearField(1);
  @$pb.TagNumber(1)
  OutPoint ensureOutPoint() => $_ensure(0);

  /// The owner address (string)
  @$pb.TagNumber(2)
  $core.String get address => $_getSZ(1);
  @$pb.TagNumber(2)
  set address($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearAddress() => clearField(2);

  /// ADA amount in the UTXO
  @$pb.TagNumber(3)
  $fixnum.Int64 get amount => $_getI64(2);
  @$pb.TagNumber(3)
  set amount($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);

  /// optional token amounts in the UTXO
  @$pb.TagNumber(4)
  $core.List<TokenAmount> get tokenAmount => $_getList(3);
}

/// One output for a transaction
class TxOutput extends $pb.GeneratedMessage {
  factory TxOutput({
    $core.String? address,
    $fixnum.Int64? amount,
    $core.Iterable<TokenAmount>? tokenAmount,
  }) {
    final $result = create();
    if (address != null) {
      $result.address = address;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (tokenAmount != null) {
      $result.tokenAmount.addAll(tokenAmount);
    }
    return $result;
  }
  TxOutput._() : super();
  factory TxOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TxOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TxOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cardano.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'address')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..pc<TokenAmount>(3, _omitFieldNames ? '' : 'tokenAmount', $pb.PbFieldType.PM, subBuilder: TokenAmount.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TxOutput clone() => TxOutput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TxOutput copyWith(void Function(TxOutput) updates) => super.copyWith((message) => updates(message as TxOutput)) as TxOutput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TxOutput create() => TxOutput._();
  TxOutput createEmptyInstance() => create();
  static $pb.PbList<TxOutput> createRepeated() => $pb.PbList<TxOutput>();
  @$core.pragma('dart2js:noInline')
  static TxOutput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TxOutput>(create);
  static TxOutput? _defaultInstance;

  /// Destination address (string)
  @$pb.TagNumber(1)
  $core.String get address => $_getSZ(0);
  @$pb.TagNumber(1)
  set address($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearAddress() => clearField(1);

  /// ADA amount
  @$pb.TagNumber(2)
  $fixnum.Int64 get amount => $_getI64(1);
  @$pb.TagNumber(2)
  set amount($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);

  /// optional token amounts
  @$pb.TagNumber(3)
  $core.List<TokenAmount> get tokenAmount => $_getList(2);
}

/// Collection of tokens with amount
class TokenBundle extends $pb.GeneratedMessage {
  factory TokenBundle({
    $core.Iterable<TokenAmount>? token,
  }) {
    final $result = create();
    if (token != null) {
      $result.token.addAll(token);
    }
    return $result;
  }
  TokenBundle._() : super();
  factory TokenBundle.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TokenBundle.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TokenBundle', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cardano.Proto'), createEmptyInstance: create)
    ..pc<TokenAmount>(1, _omitFieldNames ? '' : 'token', $pb.PbFieldType.PM, subBuilder: TokenAmount.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TokenBundle clone() => TokenBundle()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TokenBundle copyWith(void Function(TokenBundle) updates) => super.copyWith((message) => updates(message as TokenBundle)) as TokenBundle;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TokenBundle create() => TokenBundle._();
  TokenBundle createEmptyInstance() => create();
  static $pb.PbList<TokenBundle> createRepeated() => $pb.PbList<TokenBundle>();
  @$core.pragma('dart2js:noInline')
  static TokenBundle getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TokenBundle>(create);
  static TokenBundle? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<TokenAmount> get token => $_getList(0);
}

/// Message for simple Transfer tx
class Transfer extends $pb.GeneratedMessage {
  factory Transfer({
    $core.String? toAddress,
    $core.String? changeAddress,
    $fixnum.Int64? amount,
    TokenBundle? tokenAmount,
    $core.bool? useMaxAmount,
    $fixnum.Int64? forceFee,
  }) {
    final $result = create();
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (changeAddress != null) {
      $result.changeAddress = changeAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (tokenAmount != null) {
      $result.tokenAmount = tokenAmount;
    }
    if (useMaxAmount != null) {
      $result.useMaxAmount = useMaxAmount;
    }
    if (forceFee != null) {
      $result.forceFee = forceFee;
    }
    return $result;
  }
  Transfer._() : super();
  factory Transfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cardano.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'toAddress')
    ..aOS(2, _omitFieldNames ? '' : 'changeAddress')
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOM<TokenBundle>(4, _omitFieldNames ? '' : 'tokenAmount', subBuilder: TokenBundle.create)
    ..aOB(5, _omitFieldNames ? '' : 'useMaxAmount')
    ..a<$fixnum.Int64>(6, _omitFieldNames ? '' : 'forceFee', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transfer clone() => Transfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transfer copyWith(void Function(Transfer) updates) => super.copyWith((message) => updates(message as Transfer)) as Transfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Transfer create() => Transfer._();
  Transfer createEmptyInstance() => create();
  static $pb.PbList<Transfer> createRepeated() => $pb.PbList<Transfer>();
  @$core.pragma('dart2js:noInline')
  static Transfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transfer>(create);
  static Transfer? _defaultInstance;

  /// Destination address as string
  @$pb.TagNumber(1)
  $core.String get toAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set toAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasToAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearToAddress() => clearField(1);

  /// Change address
  @$pb.TagNumber(2)
  $core.String get changeAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set changeAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasChangeAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearChangeAddress() => clearField(2);

  /// Requested ADA amount to be transferred. Output can be different only in use_max_amount case.
  /// Note that Cardano has a minimum amount per UTXO, see TWCardanoMinAdaAmount.
  @$pb.TagNumber(3)
  $fixnum.Int64 get amount => $_getI64(2);
  @$pb.TagNumber(3)
  set amount($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);

  /// Optional token(s) to be transferred
  /// Currently only one token type to be transferred per transaction is supported
  @$pb.TagNumber(4)
  TokenBundle get tokenAmount => $_getN(3);
  @$pb.TagNumber(4)
  set tokenAmount(TokenBundle v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasTokenAmount() => $_has(3);
  @$pb.TagNumber(4)
  void clearTokenAmount() => clearField(4);
  @$pb.TagNumber(4)
  TokenBundle ensureTokenAmount() => $_ensure(3);

  /// Request max amount option. If set, tx will send all possible amounts from all inputs, including all tokens; there will be no change; requested amount and token_amount is disregarded in this case.
  @$pb.TagNumber(5)
  $core.bool get useMaxAmount => $_getBF(4);
  @$pb.TagNumber(5)
  set useMaxAmount($core.bool v) { $_setBool(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasUseMaxAmount() => $_has(4);
  @$pb.TagNumber(5)
  void clearUseMaxAmount() => clearField(5);

  /// Optional fee overriding. If left to 0, optimal fee will be calculated. If set, exactly this value will be used as fee.
  /// Use it with care, it may result in underfunded or wasteful fee.
  @$pb.TagNumber(6)
  $fixnum.Int64 get forceFee => $_getI64(5);
  @$pb.TagNumber(6)
  set forceFee($fixnum.Int64 v) { $_setInt64(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasForceFee() => $_has(5);
  @$pb.TagNumber(6)
  void clearForceFee() => clearField(6);
}

/// Register a staking key for the account, prerequisite for Staking.
/// Note: staking messages are typically used with a 1-output-to-self transaction.
class RegisterStakingKey extends $pb.GeneratedMessage {
  factory RegisterStakingKey({
    $core.String? stakingAddress,
    $fixnum.Int64? depositAmount,
  }) {
    final $result = create();
    if (stakingAddress != null) {
      $result.stakingAddress = stakingAddress;
    }
    if (depositAmount != null) {
      $result.depositAmount = depositAmount;
    }
    return $result;
  }
  RegisterStakingKey._() : super();
  factory RegisterStakingKey.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RegisterStakingKey.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'RegisterStakingKey', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cardano.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'stakingAddress')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'depositAmount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  RegisterStakingKey clone() => RegisterStakingKey()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  RegisterStakingKey copyWith(void Function(RegisterStakingKey) updates) => super.copyWith((message) => updates(message as RegisterStakingKey)) as RegisterStakingKey;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static RegisterStakingKey create() => RegisterStakingKey._();
  RegisterStakingKey createEmptyInstance() => create();
  static $pb.PbList<RegisterStakingKey> createRepeated() => $pb.PbList<RegisterStakingKey>();
  @$core.pragma('dart2js:noInline')
  static RegisterStakingKey getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RegisterStakingKey>(create);
  static RegisterStakingKey? _defaultInstance;

  /// Staking address (as string)
  @$pb.TagNumber(1)
  $core.String get stakingAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set stakingAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasStakingAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearStakingAddress() => clearField(1);

  /// Amount deposited in this TX. Should be 2 ADA (2000000). If not set correctly, TX will be rejected. See also Delegate.deposit_amount.
  @$pb.TagNumber(2)
  $fixnum.Int64 get depositAmount => $_getI64(1);
  @$pb.TagNumber(2)
  set depositAmount($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasDepositAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearDepositAmount() => clearField(2);
}

/// Deregister staking key. can be done when staking is stopped completely. The Staking deposit is returned at this time.
class DeregisterStakingKey extends $pb.GeneratedMessage {
  factory DeregisterStakingKey({
    $core.String? stakingAddress,
    $fixnum.Int64? undepositAmount,
  }) {
    final $result = create();
    if (stakingAddress != null) {
      $result.stakingAddress = stakingAddress;
    }
    if (undepositAmount != null) {
      $result.undepositAmount = undepositAmount;
    }
    return $result;
  }
  DeregisterStakingKey._() : super();
  factory DeregisterStakingKey.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeregisterStakingKey.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DeregisterStakingKey', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cardano.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'stakingAddress')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'undepositAmount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DeregisterStakingKey clone() => DeregisterStakingKey()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DeregisterStakingKey copyWith(void Function(DeregisterStakingKey) updates) => super.copyWith((message) => updates(message as DeregisterStakingKey)) as DeregisterStakingKey;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DeregisterStakingKey create() => DeregisterStakingKey._();
  DeregisterStakingKey createEmptyInstance() => create();
  static $pb.PbList<DeregisterStakingKey> createRepeated() => $pb.PbList<DeregisterStakingKey>();
  @$core.pragma('dart2js:noInline')
  static DeregisterStakingKey getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeregisterStakingKey>(create);
  static DeregisterStakingKey? _defaultInstance;

  /// Staking address (as string)
  @$pb.TagNumber(1)
  $core.String get stakingAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set stakingAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasStakingAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearStakingAddress() => clearField(1);

  /// Amount undeposited in this TX. Should be 2 ADA (2000000). If not set correctly, TX will be rejected. See also RegisterStakingKey.deposit_amount.
  @$pb.TagNumber(2)
  $fixnum.Int64 get undepositAmount => $_getI64(1);
  @$pb.TagNumber(2)
  set undepositAmount($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasUndepositAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearUndepositAmount() => clearField(2);
}

/// Delegate funds in this account to a specified staking pool.
class Delegate extends $pb.GeneratedMessage {
  factory Delegate({
    $core.String? stakingAddress,
    $core.List<$core.int>? poolId,
    $fixnum.Int64? depositAmount,
  }) {
    final $result = create();
    if (stakingAddress != null) {
      $result.stakingAddress = stakingAddress;
    }
    if (poolId != null) {
      $result.poolId = poolId;
    }
    if (depositAmount != null) {
      $result.depositAmount = depositAmount;
    }
    return $result;
  }
  Delegate._() : super();
  factory Delegate.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Delegate.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Delegate', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cardano.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'stakingAddress')
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'poolId', $pb.PbFieldType.OY)
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'depositAmount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Delegate clone() => Delegate()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Delegate copyWith(void Function(Delegate) updates) => super.copyWith((message) => updates(message as Delegate)) as Delegate;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Delegate create() => Delegate._();
  Delegate createEmptyInstance() => create();
  static $pb.PbList<Delegate> createRepeated() => $pb.PbList<Delegate>();
  @$core.pragma('dart2js:noInline')
  static Delegate getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Delegate>(create);
  static Delegate? _defaultInstance;

  /// Staking address (as string)
  @$pb.TagNumber(1)
  $core.String get stakingAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set stakingAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasStakingAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearStakingAddress() => clearField(1);

  /// PoolID of staking pool
  @$pb.TagNumber(2)
  $core.List<$core.int> get poolId => $_getN(1);
  @$pb.TagNumber(2)
  set poolId($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPoolId() => $_has(1);
  @$pb.TagNumber(2)
  void clearPoolId() => clearField(2);

  /// Amount deposited in this TX. Should be 0. If not set correctly, TX will be rejected. See also RegisterStakingKey.deposit_amount.
  @$pb.TagNumber(3)
  $fixnum.Int64 get depositAmount => $_getI64(2);
  @$pb.TagNumber(3)
  set depositAmount($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasDepositAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearDepositAmount() => clearField(3);
}

/// Withdraw earned staking reward.
class Withdraw extends $pb.GeneratedMessage {
  factory Withdraw({
    $core.String? stakingAddress,
    $fixnum.Int64? withdrawAmount,
  }) {
    final $result = create();
    if (stakingAddress != null) {
      $result.stakingAddress = stakingAddress;
    }
    if (withdrawAmount != null) {
      $result.withdrawAmount = withdrawAmount;
    }
    return $result;
  }
  Withdraw._() : super();
  factory Withdraw.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Withdraw.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Withdraw', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cardano.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'stakingAddress')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'withdrawAmount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Withdraw clone() => Withdraw()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Withdraw copyWith(void Function(Withdraw) updates) => super.copyWith((message) => updates(message as Withdraw)) as Withdraw;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Withdraw create() => Withdraw._();
  Withdraw createEmptyInstance() => create();
  static $pb.PbList<Withdraw> createRepeated() => $pb.PbList<Withdraw>();
  @$core.pragma('dart2js:noInline')
  static Withdraw getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Withdraw>(create);
  static Withdraw? _defaultInstance;

  /// Staking address (as string)
  @$pb.TagNumber(1)
  $core.String get stakingAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set stakingAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasStakingAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearStakingAddress() => clearField(1);

  /// Withdrawal amount. Should match the real value of the earned reward.
  @$pb.TagNumber(2)
  $fixnum.Int64 get withdrawAmount => $_getI64(1);
  @$pb.TagNumber(2)
  set withdrawAmount($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasWithdrawAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearWithdrawAmount() => clearField(2);
}

/// Describes a preliminary transaction plan.
class TransactionPlan extends $pb.GeneratedMessage {
  factory TransactionPlan({
    $fixnum.Int64? availableAmount,
    $fixnum.Int64? amount,
    $fixnum.Int64? fee,
    $fixnum.Int64? change,
    $core.Iterable<TokenAmount>? availableTokens,
    $core.Iterable<TokenAmount>? outputTokens,
    $core.Iterable<TokenAmount>? changeTokens,
    $core.Iterable<TxInput>? utxos,
    $0.SigningError? error,
    $fixnum.Int64? deposit,
    $fixnum.Int64? undeposit,
    $core.Iterable<TxOutput>? extraOutputs,
  }) {
    final $result = create();
    if (availableAmount != null) {
      $result.availableAmount = availableAmount;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (fee != null) {
      $result.fee = fee;
    }
    if (change != null) {
      $result.change = change;
    }
    if (availableTokens != null) {
      $result.availableTokens.addAll(availableTokens);
    }
    if (outputTokens != null) {
      $result.outputTokens.addAll(outputTokens);
    }
    if (changeTokens != null) {
      $result.changeTokens.addAll(changeTokens);
    }
    if (utxos != null) {
      $result.utxos.addAll(utxos);
    }
    if (error != null) {
      $result.error = error;
    }
    if (deposit != null) {
      $result.deposit = deposit;
    }
    if (undeposit != null) {
      $result.undeposit = undeposit;
    }
    if (extraOutputs != null) {
      $result.extraOutputs.addAll(extraOutputs);
    }
    return $result;
  }
  TransactionPlan._() : super();
  factory TransactionPlan.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TransactionPlan.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TransactionPlan', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cardano.Proto'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'availableAmount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'fee', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(4, _omitFieldNames ? '' : 'change', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..pc<TokenAmount>(5, _omitFieldNames ? '' : 'availableTokens', $pb.PbFieldType.PM, subBuilder: TokenAmount.create)
    ..pc<TokenAmount>(6, _omitFieldNames ? '' : 'outputTokens', $pb.PbFieldType.PM, subBuilder: TokenAmount.create)
    ..pc<TokenAmount>(7, _omitFieldNames ? '' : 'changeTokens', $pb.PbFieldType.PM, subBuilder: TokenAmount.create)
    ..pc<TxInput>(8, _omitFieldNames ? '' : 'utxos', $pb.PbFieldType.PM, subBuilder: TxInput.create)
    ..e<$0.SigningError>(9, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..a<$fixnum.Int64>(10, _omitFieldNames ? '' : 'deposit', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(11, _omitFieldNames ? '' : 'undeposit', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..pc<TxOutput>(12, _omitFieldNames ? '' : 'extraOutputs', $pb.PbFieldType.PM, subBuilder: TxOutput.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TransactionPlan clone() => TransactionPlan()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TransactionPlan copyWith(void Function(TransactionPlan) updates) => super.copyWith((message) => updates(message as TransactionPlan)) as TransactionPlan;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TransactionPlan create() => TransactionPlan._();
  TransactionPlan createEmptyInstance() => create();
  static $pb.PbList<TransactionPlan> createRepeated() => $pb.PbList<TransactionPlan>();
  @$core.pragma('dart2js:noInline')
  static TransactionPlan getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TransactionPlan>(create);
  static TransactionPlan? _defaultInstance;

  /// total coins in the utxos
  @$pb.TagNumber(1)
  $fixnum.Int64 get availableAmount => $_getI64(0);
  @$pb.TagNumber(1)
  set availableAmount($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAvailableAmount() => $_has(0);
  @$pb.TagNumber(1)
  void clearAvailableAmount() => clearField(1);

  /// coins in the output UTXO
  @$pb.TagNumber(2)
  $fixnum.Int64 get amount => $_getI64(1);
  @$pb.TagNumber(2)
  set amount($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);

  /// coin amount deducted as fee
  @$pb.TagNumber(3)
  $fixnum.Int64 get fee => $_getI64(2);
  @$pb.TagNumber(3)
  set fee($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasFee() => $_has(2);
  @$pb.TagNumber(3)
  void clearFee() => clearField(3);

  /// coins in the change UTXO
  @$pb.TagNumber(4)
  $fixnum.Int64 get change => $_getI64(3);
  @$pb.TagNumber(4)
  set change($fixnum.Int64 v) { $_setInt64(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasChange() => $_has(3);
  @$pb.TagNumber(4)
  void clearChange() => clearField(4);

  /// total tokens in the utxos (optional)
  @$pb.TagNumber(5)
  $core.List<TokenAmount> get availableTokens => $_getList(4);

  /// tokens in the output (optional)
  @$pb.TagNumber(6)
  $core.List<TokenAmount> get outputTokens => $_getList(5);

  /// tokens in the change (optional)
  @$pb.TagNumber(7)
  $core.List<TokenAmount> get changeTokens => $_getList(6);

  /// The selected UTXOs, subset ot the input UTXOs
  @$pb.TagNumber(8)
  $core.List<TxInput> get utxos => $_getList(7);

  /// Optional error
  @$pb.TagNumber(9)
  $0.SigningError get error => $_getN(8);
  @$pb.TagNumber(9)
  set error($0.SigningError v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasError() => $_has(8);
  @$pb.TagNumber(9)
  void clearError() => clearField(9);

  /// coins deposited (going to deposit) in this TX
  @$pb.TagNumber(10)
  $fixnum.Int64 get deposit => $_getI64(9);
  @$pb.TagNumber(10)
  set deposit($fixnum.Int64 v) { $_setInt64(9, v); }
  @$pb.TagNumber(10)
  $core.bool hasDeposit() => $_has(9);
  @$pb.TagNumber(10)
  void clearDeposit() => clearField(10);

  /// coins undeposited (coming from deposit) in this TX
  @$pb.TagNumber(11)
  $fixnum.Int64 get undeposit => $_getI64(10);
  @$pb.TagNumber(11)
  set undeposit($fixnum.Int64 v) { $_setInt64(10, v); }
  @$pb.TagNumber(11)
  $core.bool hasUndeposit() => $_has(10);
  @$pb.TagNumber(11)
  void clearUndeposit() => clearField(11);

  /// Optional additional destination addresses, additional to first to_address output
  @$pb.TagNumber(12)
  $core.List<TxOutput> get extraOutputs => $_getList(11);
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.Iterable<TxInput>? utxos,
    $core.Iterable<$core.List<$core.int>>? privateKey,
    Transfer? transferMessage,
    $fixnum.Int64? ttl,
    TransactionPlan? plan,
    RegisterStakingKey? registerStakingKey,
    Delegate? delegate,
    Withdraw? withdraw,
    DeregisterStakingKey? deregisterStakingKey,
    $core.Iterable<TxOutput>? extraOutputs,
  }) {
    final $result = create();
    if (utxos != null) {
      $result.utxos.addAll(utxos);
    }
    if (privateKey != null) {
      $result.privateKey.addAll(privateKey);
    }
    if (transferMessage != null) {
      $result.transferMessage = transferMessage;
    }
    if (ttl != null) {
      $result.ttl = ttl;
    }
    if (plan != null) {
      $result.plan = plan;
    }
    if (registerStakingKey != null) {
      $result.registerStakingKey = registerStakingKey;
    }
    if (delegate != null) {
      $result.delegate = delegate;
    }
    if (withdraw != null) {
      $result.withdraw = withdraw;
    }
    if (deregisterStakingKey != null) {
      $result.deregisterStakingKey = deregisterStakingKey;
    }
    if (extraOutputs != null) {
      $result.extraOutputs.addAll(extraOutputs);
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cardano.Proto'), createEmptyInstance: create)
    ..pc<TxInput>(1, _omitFieldNames ? '' : 'utxos', $pb.PbFieldType.PM, subBuilder: TxInput.create)
    ..p<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.PY)
    ..aOM<Transfer>(3, _omitFieldNames ? '' : 'transferMessage', subBuilder: Transfer.create)
    ..a<$fixnum.Int64>(4, _omitFieldNames ? '' : 'ttl', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOM<TransactionPlan>(5, _omitFieldNames ? '' : 'plan', subBuilder: TransactionPlan.create)
    ..aOM<RegisterStakingKey>(6, _omitFieldNames ? '' : 'registerStakingKey', subBuilder: RegisterStakingKey.create)
    ..aOM<Delegate>(7, _omitFieldNames ? '' : 'delegate', subBuilder: Delegate.create)
    ..aOM<Withdraw>(8, _omitFieldNames ? '' : 'withdraw', subBuilder: Withdraw.create)
    ..aOM<DeregisterStakingKey>(9, _omitFieldNames ? '' : 'deregisterStakingKey', subBuilder: DeregisterStakingKey.create)
    ..pc<TxOutput>(10, _omitFieldNames ? '' : 'extraOutputs', $pb.PbFieldType.PM, subBuilder: TxOutput.create)
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

  /// Available input UTXOs
  @$pb.TagNumber(1)
  $core.List<TxInput> get utxos => $_getList(0);

  /// Available private keys (double extended keys); every input UTXO address should be covered
  /// In case of Plan only, keys should be present, in correct number
  @$pb.TagNumber(2)
  $core.List<$core.List<$core.int>> get privateKey => $_getList(1);

  /// Later this can be made oneof if more message types are supported
  @$pb.TagNumber(3)
  Transfer get transferMessage => $_getN(2);
  @$pb.TagNumber(3)
  set transferMessage(Transfer v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasTransferMessage() => $_has(2);
  @$pb.TagNumber(3)
  void clearTransferMessage() => clearField(3);
  @$pb.TagNumber(3)
  Transfer ensureTransferMessage() => $_ensure(2);

  /// Time-to-live time of the TX
  @$pb.TagNumber(4)
  $fixnum.Int64 get ttl => $_getI64(3);
  @$pb.TagNumber(4)
  set ttl($fixnum.Int64 v) { $_setInt64(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasTtl() => $_has(3);
  @$pb.TagNumber(4)
  void clearTtl() => clearField(4);

  /// Optional plan, if missing it will be computed
  @$pb.TagNumber(5)
  TransactionPlan get plan => $_getN(4);
  @$pb.TagNumber(5)
  set plan(TransactionPlan v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasPlan() => $_has(4);
  @$pb.TagNumber(5)
  void clearPlan() => clearField(5);
  @$pb.TagNumber(5)
  TransactionPlan ensurePlan() => $_ensure(4);

  /// Optional, used in case of Staking Key registration (prerequisite for Staking)
  @$pb.TagNumber(6)
  RegisterStakingKey get registerStakingKey => $_getN(5);
  @$pb.TagNumber(6)
  set registerStakingKey(RegisterStakingKey v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasRegisterStakingKey() => $_has(5);
  @$pb.TagNumber(6)
  void clearRegisterStakingKey() => clearField(6);
  @$pb.TagNumber(6)
  RegisterStakingKey ensureRegisterStakingKey() => $_ensure(5);

  /// Optional, used in case of (re)delegation
  @$pb.TagNumber(7)
  Delegate get delegate => $_getN(6);
  @$pb.TagNumber(7)
  set delegate(Delegate v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasDelegate() => $_has(6);
  @$pb.TagNumber(7)
  void clearDelegate() => clearField(7);
  @$pb.TagNumber(7)
  Delegate ensureDelegate() => $_ensure(6);

  /// Optional, used in case of withdraw
  @$pb.TagNumber(8)
  Withdraw get withdraw => $_getN(7);
  @$pb.TagNumber(8)
  set withdraw(Withdraw v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasWithdraw() => $_has(7);
  @$pb.TagNumber(8)
  void clearWithdraw() => clearField(8);
  @$pb.TagNumber(8)
  Withdraw ensureWithdraw() => $_ensure(7);

  /// Optional
  @$pb.TagNumber(9)
  DeregisterStakingKey get deregisterStakingKey => $_getN(8);
  @$pb.TagNumber(9)
  set deregisterStakingKey(DeregisterStakingKey v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasDeregisterStakingKey() => $_has(8);
  @$pb.TagNumber(9)
  void clearDeregisterStakingKey() => clearField(9);
  @$pb.TagNumber(9)
  DeregisterStakingKey ensureDeregisterStakingKey() => $_ensure(8);

  /// extra output UTXOs
  @$pb.TagNumber(10)
  $core.List<TxOutput> get extraOutputs => $_getList(9);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.List<$core.int>? encoded,
    $core.List<$core.int>? txId,
    $0.SigningError? error,
    $core.String? errorMessage,
  }) {
    final $result = create();
    if (encoded != null) {
      $result.encoded = encoded;
    }
    if (txId != null) {
      $result.txId = txId;
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

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cardano.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'encoded', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'txId', $pb.PbFieldType.OY)
    ..e<$0.SigningError>(3, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(4, _omitFieldNames ? '' : 'errorMessage')
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

  /// Encoded transaction bytes
  @$pb.TagNumber(1)
  $core.List<$core.int> get encoded => $_getN(0);
  @$pb.TagNumber(1)
  set encoded($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasEncoded() => $_has(0);
  @$pb.TagNumber(1)
  void clearEncoded() => clearField(1);

  /// TxID, derived from transaction data, also needed for submission
  @$pb.TagNumber(2)
  $core.List<$core.int> get txId => $_getN(1);
  @$pb.TagNumber(2)
  set txId($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTxId() => $_has(1);
  @$pb.TagNumber(2)
  void clearTxId() => clearField(2);

  /// Optional error
  @$pb.TagNumber(3)
  $0.SigningError get error => $_getN(2);
  @$pb.TagNumber(3)
  set error($0.SigningError v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasError() => $_has(2);
  @$pb.TagNumber(3)
  void clearError() => clearField(3);

  /// error code description
  @$pb.TagNumber(4)
  $core.String get errorMessage => $_getSZ(3);
  @$pb.TagNumber(4)
  set errorMessage($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasErrorMessage() => $_has(3);
  @$pb.TagNumber(4)
  void clearErrorMessage() => clearField(4);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
