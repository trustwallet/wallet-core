//
//  Generated code. Do not modify.
//  source: Stellar.proto
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
import 'Stellar.pbenum.dart';

export 'Stellar.pbenum.dart';

/// Represents an asset
/// Note: alphanum12 currently not supported
class Asset extends $pb.GeneratedMessage {
  factory Asset({
    $core.String? issuer,
    $core.String? alphanum4,
  }) {
    final $result = create();
    if (issuer != null) {
      $result.issuer = issuer;
    }
    if (alphanum4 != null) {
      $result.alphanum4 = alphanum4;
    }
    return $result;
  }
  Asset._() : super();
  factory Asset.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Asset.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Asset', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Stellar.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'issuer')
    ..aOS(2, _omitFieldNames ? '' : 'alphanum4')
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

  /// Optional in case of non-native asset; the asset issuer address
  @$pb.TagNumber(1)
  $core.String get issuer => $_getSZ(0);
  @$pb.TagNumber(1)
  set issuer($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasIssuer() => $_has(0);
  @$pb.TagNumber(1)
  void clearIssuer() => clearField(1);

  /// Optional in case of non-native asset; the asset alphanum4 code.
  @$pb.TagNumber(2)
  $core.String get alphanum4 => $_getSZ(1);
  @$pb.TagNumber(2)
  set alphanum4($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAlphanum4() => $_has(1);
  @$pb.TagNumber(2)
  void clearAlphanum4() => clearField(2);
}

/// Create a new account
class OperationCreateAccount extends $pb.GeneratedMessage {
  factory OperationCreateAccount({
    $core.String? destination,
    $fixnum.Int64? amount,
  }) {
    final $result = create();
    if (destination != null) {
      $result.destination = destination;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    return $result;
  }
  OperationCreateAccount._() : super();
  factory OperationCreateAccount.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory OperationCreateAccount.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'OperationCreateAccount', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Stellar.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'destination')
    ..aInt64(2, _omitFieldNames ? '' : 'amount')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  OperationCreateAccount clone() => OperationCreateAccount()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  OperationCreateAccount copyWith(void Function(OperationCreateAccount) updates) => super.copyWith((message) => updates(message as OperationCreateAccount)) as OperationCreateAccount;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static OperationCreateAccount create() => OperationCreateAccount._();
  OperationCreateAccount createEmptyInstance() => create();
  static $pb.PbList<OperationCreateAccount> createRepeated() => $pb.PbList<OperationCreateAccount>();
  @$core.pragma('dart2js:noInline')
  static OperationCreateAccount getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<OperationCreateAccount>(create);
  static OperationCreateAccount? _defaultInstance;

  /// address
  @$pb.TagNumber(1)
  $core.String get destination => $_getSZ(0);
  @$pb.TagNumber(1)
  set destination($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDestination() => $_has(0);
  @$pb.TagNumber(1)
  void clearDestination() => clearField(1);

  /// Amount (*10^7)
  @$pb.TagNumber(2)
  $fixnum.Int64 get amount => $_getI64(1);
  @$pb.TagNumber(2)
  set amount($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);
}

/// Perform payment
class OperationPayment extends $pb.GeneratedMessage {
  factory OperationPayment({
    $core.String? destination,
    Asset? asset,
    $fixnum.Int64? amount,
  }) {
    final $result = create();
    if (destination != null) {
      $result.destination = destination;
    }
    if (asset != null) {
      $result.asset = asset;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    return $result;
  }
  OperationPayment._() : super();
  factory OperationPayment.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory OperationPayment.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'OperationPayment', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Stellar.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'destination')
    ..aOM<Asset>(2, _omitFieldNames ? '' : 'asset', subBuilder: Asset.create)
    ..aInt64(3, _omitFieldNames ? '' : 'amount')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  OperationPayment clone() => OperationPayment()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  OperationPayment copyWith(void Function(OperationPayment) updates) => super.copyWith((message) => updates(message as OperationPayment)) as OperationPayment;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static OperationPayment create() => OperationPayment._();
  OperationPayment createEmptyInstance() => create();
  static $pb.PbList<OperationPayment> createRepeated() => $pb.PbList<OperationPayment>();
  @$core.pragma('dart2js:noInline')
  static OperationPayment getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<OperationPayment>(create);
  static OperationPayment? _defaultInstance;

  /// Destination address
  @$pb.TagNumber(1)
  $core.String get destination => $_getSZ(0);
  @$pb.TagNumber(1)
  set destination($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDestination() => $_has(0);
  @$pb.TagNumber(1)
  void clearDestination() => clearField(1);

  /// Optional, can be left empty for native asset
  @$pb.TagNumber(2)
  Asset get asset => $_getN(1);
  @$pb.TagNumber(2)
  set asset(Asset v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasAsset() => $_has(1);
  @$pb.TagNumber(2)
  void clearAsset() => clearField(2);
  @$pb.TagNumber(2)
  Asset ensureAsset() => $_ensure(1);

  /// Amount (*10^7)
  @$pb.TagNumber(3)
  $fixnum.Int64 get amount => $_getI64(2);
  @$pb.TagNumber(3)
  set amount($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);
}

/// Change trust
class OperationChangeTrust extends $pb.GeneratedMessage {
  factory OperationChangeTrust({
    Asset? asset,
    $fixnum.Int64? validBefore,
  }) {
    final $result = create();
    if (asset != null) {
      $result.asset = asset;
    }
    if (validBefore != null) {
      $result.validBefore = validBefore;
    }
    return $result;
  }
  OperationChangeTrust._() : super();
  factory OperationChangeTrust.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory OperationChangeTrust.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'OperationChangeTrust', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Stellar.Proto'), createEmptyInstance: create)
    ..aOM<Asset>(1, _omitFieldNames ? '' : 'asset', subBuilder: Asset.create)
    ..aInt64(2, _omitFieldNames ? '' : 'validBefore')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  OperationChangeTrust clone() => OperationChangeTrust()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  OperationChangeTrust copyWith(void Function(OperationChangeTrust) updates) => super.copyWith((message) => updates(message as OperationChangeTrust)) as OperationChangeTrust;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static OperationChangeTrust create() => OperationChangeTrust._();
  OperationChangeTrust createEmptyInstance() => create();
  static $pb.PbList<OperationChangeTrust> createRepeated() => $pb.PbList<OperationChangeTrust>();
  @$core.pragma('dart2js:noInline')
  static OperationChangeTrust getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<OperationChangeTrust>(create);
  static OperationChangeTrust? _defaultInstance;

  /// The asset
  @$pb.TagNumber(1)
  Asset get asset => $_getN(0);
  @$pb.TagNumber(1)
  set asset(Asset v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasAsset() => $_has(0);
  @$pb.TagNumber(1)
  void clearAsset() => clearField(1);
  @$pb.TagNumber(1)
  Asset ensureAsset() => $_ensure(0);

  /// Validity (time bound to), unix time.  Set to (now() + 2 * 365 * 86400) for 2 years; set to 0 for missing.
  @$pb.TagNumber(2)
  $fixnum.Int64 get validBefore => $_getI64(1);
  @$pb.TagNumber(2)
  set validBefore($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValidBefore() => $_has(1);
  @$pb.TagNumber(2)
  void clearValidBefore() => clearField(2);
}

/// Claimant: account & predicate
class Claimant extends $pb.GeneratedMessage {
  factory Claimant({
    $core.String? account,
    ClaimPredicate? predicate,
  }) {
    final $result = create();
    if (account != null) {
      $result.account = account;
    }
    if (predicate != null) {
      $result.predicate = predicate;
    }
    return $result;
  }
  Claimant._() : super();
  factory Claimant.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Claimant.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Claimant', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Stellar.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'account')
    ..e<ClaimPredicate>(2, _omitFieldNames ? '' : 'predicate', $pb.PbFieldType.OE, defaultOrMaker: ClaimPredicate.Predicate_unconditional, valueOf: ClaimPredicate.valueOf, enumValues: ClaimPredicate.values)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Claimant clone() => Claimant()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Claimant copyWith(void Function(Claimant) updates) => super.copyWith((message) => updates(message as Claimant)) as Claimant;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Claimant create() => Claimant._();
  Claimant createEmptyInstance() => create();
  static $pb.PbList<Claimant> createRepeated() => $pb.PbList<Claimant>();
  @$core.pragma('dart2js:noInline')
  static Claimant getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Claimant>(create);
  static Claimant? _defaultInstance;

  /// Claimant account
  @$pb.TagNumber(1)
  $core.String get account => $_getSZ(0);
  @$pb.TagNumber(1)
  set account($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAccount() => $_has(0);
  @$pb.TagNumber(1)
  void clearAccount() => clearField(1);

  /// predicate
  @$pb.TagNumber(2)
  ClaimPredicate get predicate => $_getN(1);
  @$pb.TagNumber(2)
  set predicate(ClaimPredicate v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasPredicate() => $_has(1);
  @$pb.TagNumber(2)
  void clearPredicate() => clearField(2);
}

/// Create a claimable balance (2-phase transfer)
class OperationCreateClaimableBalance extends $pb.GeneratedMessage {
  factory OperationCreateClaimableBalance({
    Asset? asset,
    $fixnum.Int64? amount,
    $core.Iterable<Claimant>? claimants,
  }) {
    final $result = create();
    if (asset != null) {
      $result.asset = asset;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (claimants != null) {
      $result.claimants.addAll(claimants);
    }
    return $result;
  }
  OperationCreateClaimableBalance._() : super();
  factory OperationCreateClaimableBalance.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory OperationCreateClaimableBalance.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'OperationCreateClaimableBalance', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Stellar.Proto'), createEmptyInstance: create)
    ..aOM<Asset>(1, _omitFieldNames ? '' : 'asset', subBuilder: Asset.create)
    ..aInt64(2, _omitFieldNames ? '' : 'amount')
    ..pc<Claimant>(3, _omitFieldNames ? '' : 'claimants', $pb.PbFieldType.PM, subBuilder: Claimant.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  OperationCreateClaimableBalance clone() => OperationCreateClaimableBalance()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  OperationCreateClaimableBalance copyWith(void Function(OperationCreateClaimableBalance) updates) => super.copyWith((message) => updates(message as OperationCreateClaimableBalance)) as OperationCreateClaimableBalance;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static OperationCreateClaimableBalance create() => OperationCreateClaimableBalance._();
  OperationCreateClaimableBalance createEmptyInstance() => create();
  static $pb.PbList<OperationCreateClaimableBalance> createRepeated() => $pb.PbList<OperationCreateClaimableBalance>();
  @$core.pragma('dart2js:noInline')
  static OperationCreateClaimableBalance getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<OperationCreateClaimableBalance>(create);
  static OperationCreateClaimableBalance? _defaultInstance;

  /// Optional, can be left empty for native asset
  @$pb.TagNumber(1)
  Asset get asset => $_getN(0);
  @$pb.TagNumber(1)
  set asset(Asset v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasAsset() => $_has(0);
  @$pb.TagNumber(1)
  void clearAsset() => clearField(1);
  @$pb.TagNumber(1)
  Asset ensureAsset() => $_ensure(0);

  /// Amount (*10^7)
  @$pb.TagNumber(2)
  $fixnum.Int64 get amount => $_getI64(1);
  @$pb.TagNumber(2)
  set amount($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);

  /// One or more claimants
  @$pb.TagNumber(3)
  $core.List<Claimant> get claimants => $_getList(2);
}

/// Claim a claimable balance
class OperationClaimClaimableBalance extends $pb.GeneratedMessage {
  factory OperationClaimClaimableBalance({
    $core.List<$core.int>? balanceId,
  }) {
    final $result = create();
    if (balanceId != null) {
      $result.balanceId = balanceId;
    }
    return $result;
  }
  OperationClaimClaimableBalance._() : super();
  factory OperationClaimClaimableBalance.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory OperationClaimClaimableBalance.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'OperationClaimClaimableBalance', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Stellar.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'balanceId', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  OperationClaimClaimableBalance clone() => OperationClaimClaimableBalance()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  OperationClaimClaimableBalance copyWith(void Function(OperationClaimClaimableBalance) updates) => super.copyWith((message) => updates(message as OperationClaimClaimableBalance)) as OperationClaimClaimableBalance;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static OperationClaimClaimableBalance create() => OperationClaimClaimableBalance._();
  OperationClaimClaimableBalance createEmptyInstance() => create();
  static $pb.PbList<OperationClaimClaimableBalance> createRepeated() => $pb.PbList<OperationClaimClaimableBalance>();
  @$core.pragma('dart2js:noInline')
  static OperationClaimClaimableBalance getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<OperationClaimClaimableBalance>(create);
  static OperationClaimClaimableBalance? _defaultInstance;

  /// 32-byte balance ID hash
  @$pb.TagNumber(1)
  $core.List<$core.int> get balanceId => $_getN(0);
  @$pb.TagNumber(1)
  set balanceId($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasBalanceId() => $_has(0);
  @$pb.TagNumber(1)
  void clearBalanceId() => clearField(1);
}

/// Empty memo (placeholder)
class MemoVoid extends $pb.GeneratedMessage {
  factory MemoVoid() => create();
  MemoVoid._() : super();
  factory MemoVoid.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory MemoVoid.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'MemoVoid', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Stellar.Proto'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  MemoVoid clone() => MemoVoid()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  MemoVoid copyWith(void Function(MemoVoid) updates) => super.copyWith((message) => updates(message as MemoVoid)) as MemoVoid;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static MemoVoid create() => MemoVoid._();
  MemoVoid createEmptyInstance() => create();
  static $pb.PbList<MemoVoid> createRepeated() => $pb.PbList<MemoVoid>();
  @$core.pragma('dart2js:noInline')
  static MemoVoid getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<MemoVoid>(create);
  static MemoVoid? _defaultInstance;
}

/// Memo with text
class MemoText extends $pb.GeneratedMessage {
  factory MemoText({
    $core.String? text,
  }) {
    final $result = create();
    if (text != null) {
      $result.text = text;
    }
    return $result;
  }
  MemoText._() : super();
  factory MemoText.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory MemoText.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'MemoText', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Stellar.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'text')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  MemoText clone() => MemoText()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  MemoText copyWith(void Function(MemoText) updates) => super.copyWith((message) => updates(message as MemoText)) as MemoText;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static MemoText create() => MemoText._();
  MemoText createEmptyInstance() => create();
  static $pb.PbList<MemoText> createRepeated() => $pb.PbList<MemoText>();
  @$core.pragma('dart2js:noInline')
  static MemoText getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<MemoText>(create);
  static MemoText? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get text => $_getSZ(0);
  @$pb.TagNumber(1)
  set text($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasText() => $_has(0);
  @$pb.TagNumber(1)
  void clearText() => clearField(1);
}

/// Memo with an ID
class MemoId extends $pb.GeneratedMessage {
  factory MemoId({
    $fixnum.Int64? id,
  }) {
    final $result = create();
    if (id != null) {
      $result.id = id;
    }
    return $result;
  }
  MemoId._() : super();
  factory MemoId.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory MemoId.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'MemoId', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Stellar.Proto'), createEmptyInstance: create)
    ..aInt64(1, _omitFieldNames ? '' : 'id')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  MemoId clone() => MemoId()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  MemoId copyWith(void Function(MemoId) updates) => super.copyWith((message) => updates(message as MemoId)) as MemoId;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static MemoId create() => MemoId._();
  MemoId createEmptyInstance() => create();
  static $pb.PbList<MemoId> createRepeated() => $pb.PbList<MemoId>();
  @$core.pragma('dart2js:noInline')
  static MemoId getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<MemoId>(create);
  static MemoId? _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get id => $_getI64(0);
  @$pb.TagNumber(1)
  set id($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasId() => $_has(0);
  @$pb.TagNumber(1)
  void clearId() => clearField(1);
}

/// Memo with a hash
class MemoHash extends $pb.GeneratedMessage {
  factory MemoHash({
    $core.List<$core.int>? hash,
  }) {
    final $result = create();
    if (hash != null) {
      $result.hash = hash;
    }
    return $result;
  }
  MemoHash._() : super();
  factory MemoHash.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory MemoHash.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'MemoHash', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Stellar.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'hash', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  MemoHash clone() => MemoHash()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  MemoHash copyWith(void Function(MemoHash) updates) => super.copyWith((message) => updates(message as MemoHash)) as MemoHash;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static MemoHash create() => MemoHash._();
  MemoHash createEmptyInstance() => create();
  static $pb.PbList<MemoHash> createRepeated() => $pb.PbList<MemoHash>();
  @$core.pragma('dart2js:noInline')
  static MemoHash getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<MemoHash>(create);
  static MemoHash? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.int> get hash => $_getN(0);
  @$pb.TagNumber(1)
  set hash($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasHash() => $_has(0);
  @$pb.TagNumber(1)
  void clearHash() => clearField(1);
}

enum SigningInput_OperationOneof {
  opCreateAccount, 
  opPayment, 
  opChangeTrust, 
  opCreateClaimableBalance, 
  opClaimClaimableBalance, 
  notSet
}

enum SigningInput_MemoTypeOneof {
  memoVoid, 
  memoText, 
  memoId, 
  memoHash, 
  memoReturnHash, 
  notSet
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.int? fee,
    $fixnum.Int64? sequence,
    $core.String? account,
    $core.List<$core.int>? privateKey,
    $core.String? passphrase,
    OperationCreateAccount? opCreateAccount,
    OperationPayment? opPayment,
    OperationChangeTrust? opChangeTrust,
    MemoVoid? memoVoid,
    MemoText? memoText,
    MemoId? memoId,
    MemoHash? memoHash,
    MemoHash? memoReturnHash,
    OperationCreateClaimableBalance? opCreateClaimableBalance,
    OperationClaimClaimableBalance? opClaimClaimableBalance,
    $fixnum.Int64? timeBounds,
  }) {
    final $result = create();
    if (fee != null) {
      $result.fee = fee;
    }
    if (sequence != null) {
      $result.sequence = sequence;
    }
    if (account != null) {
      $result.account = account;
    }
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (passphrase != null) {
      $result.passphrase = passphrase;
    }
    if (opCreateAccount != null) {
      $result.opCreateAccount = opCreateAccount;
    }
    if (opPayment != null) {
      $result.opPayment = opPayment;
    }
    if (opChangeTrust != null) {
      $result.opChangeTrust = opChangeTrust;
    }
    if (memoVoid != null) {
      $result.memoVoid = memoVoid;
    }
    if (memoText != null) {
      $result.memoText = memoText;
    }
    if (memoId != null) {
      $result.memoId = memoId;
    }
    if (memoHash != null) {
      $result.memoHash = memoHash;
    }
    if (memoReturnHash != null) {
      $result.memoReturnHash = memoReturnHash;
    }
    if (opCreateClaimableBalance != null) {
      $result.opCreateClaimableBalance = opCreateClaimableBalance;
    }
    if (opClaimClaimableBalance != null) {
      $result.opClaimClaimableBalance = opClaimClaimableBalance;
    }
    if (timeBounds != null) {
      $result.timeBounds = timeBounds;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, SigningInput_OperationOneof> _SigningInput_OperationOneofByTag = {
    6 : SigningInput_OperationOneof.opCreateAccount,
    7 : SigningInput_OperationOneof.opPayment,
    8 : SigningInput_OperationOneof.opChangeTrust,
    14 : SigningInput_OperationOneof.opCreateClaimableBalance,
    15 : SigningInput_OperationOneof.opClaimClaimableBalance,
    0 : SigningInput_OperationOneof.notSet
  };
  static const $core.Map<$core.int, SigningInput_MemoTypeOneof> _SigningInput_MemoTypeOneofByTag = {
    9 : SigningInput_MemoTypeOneof.memoVoid,
    10 : SigningInput_MemoTypeOneof.memoText,
    11 : SigningInput_MemoTypeOneof.memoId,
    12 : SigningInput_MemoTypeOneof.memoHash,
    13 : SigningInput_MemoTypeOneof.memoReturnHash,
    0 : SigningInput_MemoTypeOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Stellar.Proto'), createEmptyInstance: create)
    ..oo(0, [6, 7, 8, 14, 15])
    ..oo(1, [9, 10, 11, 12, 13])
    ..a<$core.int>(1, _omitFieldNames ? '' : 'fee', $pb.PbFieldType.O3)
    ..aInt64(2, _omitFieldNames ? '' : 'sequence')
    ..aOS(3, _omitFieldNames ? '' : 'account')
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..aOS(5, _omitFieldNames ? '' : 'passphrase')
    ..aOM<OperationCreateAccount>(6, _omitFieldNames ? '' : 'opCreateAccount', subBuilder: OperationCreateAccount.create)
    ..aOM<OperationPayment>(7, _omitFieldNames ? '' : 'opPayment', subBuilder: OperationPayment.create)
    ..aOM<OperationChangeTrust>(8, _omitFieldNames ? '' : 'opChangeTrust', subBuilder: OperationChangeTrust.create)
    ..aOM<MemoVoid>(9, _omitFieldNames ? '' : 'memoVoid', subBuilder: MemoVoid.create)
    ..aOM<MemoText>(10, _omitFieldNames ? '' : 'memoText', subBuilder: MemoText.create)
    ..aOM<MemoId>(11, _omitFieldNames ? '' : 'memoId', subBuilder: MemoId.create)
    ..aOM<MemoHash>(12, _omitFieldNames ? '' : 'memoHash', subBuilder: MemoHash.create)
    ..aOM<MemoHash>(13, _omitFieldNames ? '' : 'memoReturnHash', subBuilder: MemoHash.create)
    ..aOM<OperationCreateClaimableBalance>(14, _omitFieldNames ? '' : 'opCreateClaimableBalance', subBuilder: OperationCreateClaimableBalance.create)
    ..aOM<OperationClaimClaimableBalance>(15, _omitFieldNames ? '' : 'opClaimClaimableBalance', subBuilder: OperationClaimClaimableBalance.create)
    ..aInt64(16, _omitFieldNames ? '' : 'timeBounds')
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

  SigningInput_OperationOneof whichOperationOneof() => _SigningInput_OperationOneofByTag[$_whichOneof(0)]!;
  void clearOperationOneof() => clearField($_whichOneof(0));

  SigningInput_MemoTypeOneof whichMemoTypeOneof() => _SigningInput_MemoTypeOneofByTag[$_whichOneof(1)]!;
  void clearMemoTypeOneof() => clearField($_whichOneof(1));

  /// Transaction fee
  @$pb.TagNumber(1)
  $core.int get fee => $_getIZ(0);
  @$pb.TagNumber(1)
  set fee($core.int v) { $_setSignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFee() => $_has(0);
  @$pb.TagNumber(1)
  void clearFee() => clearField(1);

  /// Account sequence
  @$pb.TagNumber(2)
  $fixnum.Int64 get sequence => $_getI64(1);
  @$pb.TagNumber(2)
  set sequence($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasSequence() => $_has(1);
  @$pb.TagNumber(2)
  void clearSequence() => clearField(2);

  /// Source account
  @$pb.TagNumber(3)
  $core.String get account => $_getSZ(2);
  @$pb.TagNumber(3)
  set account($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAccount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAccount() => clearField(3);

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(4)
  $core.List<$core.int> get privateKey => $_getN(3);
  @$pb.TagNumber(4)
  set privateKey($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasPrivateKey() => $_has(3);
  @$pb.TagNumber(4)
  void clearPrivateKey() => clearField(4);

  /// Wellknown passphrase, specific to the chain
  @$pb.TagNumber(5)
  $core.String get passphrase => $_getSZ(4);
  @$pb.TagNumber(5)
  set passphrase($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasPassphrase() => $_has(4);
  @$pb.TagNumber(5)
  void clearPassphrase() => clearField(5);

  @$pb.TagNumber(6)
  OperationCreateAccount get opCreateAccount => $_getN(5);
  @$pb.TagNumber(6)
  set opCreateAccount(OperationCreateAccount v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasOpCreateAccount() => $_has(5);
  @$pb.TagNumber(6)
  void clearOpCreateAccount() => clearField(6);
  @$pb.TagNumber(6)
  OperationCreateAccount ensureOpCreateAccount() => $_ensure(5);

  @$pb.TagNumber(7)
  OperationPayment get opPayment => $_getN(6);
  @$pb.TagNumber(7)
  set opPayment(OperationPayment v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasOpPayment() => $_has(6);
  @$pb.TagNumber(7)
  void clearOpPayment() => clearField(7);
  @$pb.TagNumber(7)
  OperationPayment ensureOpPayment() => $_ensure(6);

  @$pb.TagNumber(8)
  OperationChangeTrust get opChangeTrust => $_getN(7);
  @$pb.TagNumber(8)
  set opChangeTrust(OperationChangeTrust v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasOpChangeTrust() => $_has(7);
  @$pb.TagNumber(8)
  void clearOpChangeTrust() => clearField(8);
  @$pb.TagNumber(8)
  OperationChangeTrust ensureOpChangeTrust() => $_ensure(7);

  @$pb.TagNumber(9)
  MemoVoid get memoVoid => $_getN(8);
  @$pb.TagNumber(9)
  set memoVoid(MemoVoid v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasMemoVoid() => $_has(8);
  @$pb.TagNumber(9)
  void clearMemoVoid() => clearField(9);
  @$pb.TagNumber(9)
  MemoVoid ensureMemoVoid() => $_ensure(8);

  @$pb.TagNumber(10)
  MemoText get memoText => $_getN(9);
  @$pb.TagNumber(10)
  set memoText(MemoText v) { setField(10, v); }
  @$pb.TagNumber(10)
  $core.bool hasMemoText() => $_has(9);
  @$pb.TagNumber(10)
  void clearMemoText() => clearField(10);
  @$pb.TagNumber(10)
  MemoText ensureMemoText() => $_ensure(9);

  @$pb.TagNumber(11)
  MemoId get memoId => $_getN(10);
  @$pb.TagNumber(11)
  set memoId(MemoId v) { setField(11, v); }
  @$pb.TagNumber(11)
  $core.bool hasMemoId() => $_has(10);
  @$pb.TagNumber(11)
  void clearMemoId() => clearField(11);
  @$pb.TagNumber(11)
  MemoId ensureMemoId() => $_ensure(10);

  @$pb.TagNumber(12)
  MemoHash get memoHash => $_getN(11);
  @$pb.TagNumber(12)
  set memoHash(MemoHash v) { setField(12, v); }
  @$pb.TagNumber(12)
  $core.bool hasMemoHash() => $_has(11);
  @$pb.TagNumber(12)
  void clearMemoHash() => clearField(12);
  @$pb.TagNumber(12)
  MemoHash ensureMemoHash() => $_ensure(11);

  @$pb.TagNumber(13)
  MemoHash get memoReturnHash => $_getN(12);
  @$pb.TagNumber(13)
  set memoReturnHash(MemoHash v) { setField(13, v); }
  @$pb.TagNumber(13)
  $core.bool hasMemoReturnHash() => $_has(12);
  @$pb.TagNumber(13)
  void clearMemoReturnHash() => clearField(13);
  @$pb.TagNumber(13)
  MemoHash ensureMemoReturnHash() => $_ensure(12);

  @$pb.TagNumber(14)
  OperationCreateClaimableBalance get opCreateClaimableBalance => $_getN(13);
  @$pb.TagNumber(14)
  set opCreateClaimableBalance(OperationCreateClaimableBalance v) { setField(14, v); }
  @$pb.TagNumber(14)
  $core.bool hasOpCreateClaimableBalance() => $_has(13);
  @$pb.TagNumber(14)
  void clearOpCreateClaimableBalance() => clearField(14);
  @$pb.TagNumber(14)
  OperationCreateClaimableBalance ensureOpCreateClaimableBalance() => $_ensure(13);

  @$pb.TagNumber(15)
  OperationClaimClaimableBalance get opClaimClaimableBalance => $_getN(14);
  @$pb.TagNumber(15)
  set opClaimClaimableBalance(OperationClaimClaimableBalance v) { setField(15, v); }
  @$pb.TagNumber(15)
  $core.bool hasOpClaimClaimableBalance() => $_has(14);
  @$pb.TagNumber(15)
  void clearOpClaimClaimableBalance() => clearField(15);
  @$pb.TagNumber(15)
  OperationClaimClaimableBalance ensureOpClaimClaimableBalance() => $_ensure(14);

  @$pb.TagNumber(16)
  $fixnum.Int64 get timeBounds => $_getI64(15);
  @$pb.TagNumber(16)
  set timeBounds($fixnum.Int64 v) { $_setInt64(15, v); }
  @$pb.TagNumber(16)
  $core.bool hasTimeBounds() => $_has(15);
  @$pb.TagNumber(16)
  void clearTimeBounds() => clearField(16);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
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
  SigningOutput._() : super();
  factory SigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Stellar.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'signature')
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

  /// Signature.
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


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
