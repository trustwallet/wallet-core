//
//  Generated code. Do not modify.
//  source: NEAR.proto
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

/// Public key with type
class PublicKey extends $pb.GeneratedMessage {
  factory PublicKey({
    $core.int? keyType,
    $core.List<$core.int>? data,
  }) {
    final $result = create();
    if (keyType != null) {
      $result.keyType = keyType;
    }
    if (data != null) {
      $result.data = data;
    }
    return $result;
  }
  PublicKey._() : super();
  factory PublicKey.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PublicKey.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'PublicKey', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'keyType', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'data', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  PublicKey clone() => PublicKey()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  PublicKey copyWith(void Function(PublicKey) updates) => super.copyWith((message) => updates(message as PublicKey)) as PublicKey;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static PublicKey create() => PublicKey._();
  PublicKey createEmptyInstance() => create();
  static $pb.PbList<PublicKey> createRepeated() => $pb.PbList<PublicKey>();
  @$core.pragma('dart2js:noInline')
  static PublicKey getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PublicKey>(create);
  static PublicKey? _defaultInstance;

  /// Key type
  @$pb.TagNumber(1)
  $core.int get keyType => $_getIZ(0);
  @$pb.TagNumber(1)
  set keyType($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasKeyType() => $_has(0);
  @$pb.TagNumber(1)
  void clearKeyType() => clearField(1);

  /// The public key data
  @$pb.TagNumber(2)
  $core.List<$core.int> get data => $_getN(1);
  @$pb.TagNumber(2)
  set data($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasData() => $_has(1);
  @$pb.TagNumber(2)
  void clearData() => clearField(2);
}

/// Permissions for a function call
class FunctionCallPermission extends $pb.GeneratedMessage {
  factory FunctionCallPermission({
    $core.List<$core.int>? allowance,
    $core.String? receiverId,
    $core.Iterable<$core.String>? methodNames,
  }) {
    final $result = create();
    if (allowance != null) {
      $result.allowance = allowance;
    }
    if (receiverId != null) {
      $result.receiverId = receiverId;
    }
    if (methodNames != null) {
      $result.methodNames.addAll(methodNames);
    }
    return $result;
  }
  FunctionCallPermission._() : super();
  factory FunctionCallPermission.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory FunctionCallPermission.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'FunctionCallPermission', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'allowance', $pb.PbFieldType.OY)
    ..aOS(2, _omitFieldNames ? '' : 'receiverId')
    ..pPS(3, _omitFieldNames ? '' : 'methodNames')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  FunctionCallPermission clone() => FunctionCallPermission()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  FunctionCallPermission copyWith(void Function(FunctionCallPermission) updates) => super.copyWith((message) => updates(message as FunctionCallPermission)) as FunctionCallPermission;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static FunctionCallPermission create() => FunctionCallPermission._();
  FunctionCallPermission createEmptyInstance() => create();
  static $pb.PbList<FunctionCallPermission> createRepeated() => $pb.PbList<FunctionCallPermission>();
  @$core.pragma('dart2js:noInline')
  static FunctionCallPermission getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<FunctionCallPermission>(create);
  static FunctionCallPermission? _defaultInstance;

  /// uint128 / big endian byte order
  @$pb.TagNumber(1)
  $core.List<$core.int> get allowance => $_getN(0);
  @$pb.TagNumber(1)
  set allowance($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAllowance() => $_has(0);
  @$pb.TagNumber(1)
  void clearAllowance() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get receiverId => $_getSZ(1);
  @$pb.TagNumber(2)
  set receiverId($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasReceiverId() => $_has(1);
  @$pb.TagNumber(2)
  void clearReceiverId() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<$core.String> get methodNames => $_getList(2);
}

/// Full access
class FullAccessPermission extends $pb.GeneratedMessage {
  factory FullAccessPermission() => create();
  FullAccessPermission._() : super();
  factory FullAccessPermission.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory FullAccessPermission.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'FullAccessPermission', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  FullAccessPermission clone() => FullAccessPermission()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  FullAccessPermission copyWith(void Function(FullAccessPermission) updates) => super.copyWith((message) => updates(message as FullAccessPermission)) as FullAccessPermission;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static FullAccessPermission create() => FullAccessPermission._();
  FullAccessPermission createEmptyInstance() => create();
  static $pb.PbList<FullAccessPermission> createRepeated() => $pb.PbList<FullAccessPermission>();
  @$core.pragma('dart2js:noInline')
  static FullAccessPermission getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<FullAccessPermission>(create);
  static FullAccessPermission? _defaultInstance;
}

enum AccessKey_Permission {
  functionCall, 
  fullAccess, 
  notSet
}

/// Access key: nonce + permission
class AccessKey extends $pb.GeneratedMessage {
  factory AccessKey({
    $fixnum.Int64? nonce,
    FunctionCallPermission? functionCall,
    FullAccessPermission? fullAccess,
  }) {
    final $result = create();
    if (nonce != null) {
      $result.nonce = nonce;
    }
    if (functionCall != null) {
      $result.functionCall = functionCall;
    }
    if (fullAccess != null) {
      $result.fullAccess = fullAccess;
    }
    return $result;
  }
  AccessKey._() : super();
  factory AccessKey.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AccessKey.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, AccessKey_Permission> _AccessKey_PermissionByTag = {
    2 : AccessKey_Permission.functionCall,
    3 : AccessKey_Permission.fullAccess,
    0 : AccessKey_Permission.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'AccessKey', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..oo(0, [2, 3])
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'nonce', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOM<FunctionCallPermission>(2, _omitFieldNames ? '' : 'functionCall', subBuilder: FunctionCallPermission.create)
    ..aOM<FullAccessPermission>(3, _omitFieldNames ? '' : 'fullAccess', subBuilder: FullAccessPermission.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  AccessKey clone() => AccessKey()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  AccessKey copyWith(void Function(AccessKey) updates) => super.copyWith((message) => updates(message as AccessKey)) as AccessKey;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static AccessKey create() => AccessKey._();
  AccessKey createEmptyInstance() => create();
  static $pb.PbList<AccessKey> createRepeated() => $pb.PbList<AccessKey>();
  @$core.pragma('dart2js:noInline')
  static AccessKey getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AccessKey>(create);
  static AccessKey? _defaultInstance;

  AccessKey_Permission whichPermission() => _AccessKey_PermissionByTag[$_whichOneof(0)]!;
  void clearPermission() => clearField($_whichOneof(0));

  /// Nonce
  @$pb.TagNumber(1)
  $fixnum.Int64 get nonce => $_getI64(0);
  @$pb.TagNumber(1)
  set nonce($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasNonce() => $_has(0);
  @$pb.TagNumber(1)
  void clearNonce() => clearField(1);

  @$pb.TagNumber(2)
  FunctionCallPermission get functionCall => $_getN(1);
  @$pb.TagNumber(2)
  set functionCall(FunctionCallPermission v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasFunctionCall() => $_has(1);
  @$pb.TagNumber(2)
  void clearFunctionCall() => clearField(2);
  @$pb.TagNumber(2)
  FunctionCallPermission ensureFunctionCall() => $_ensure(1);

  @$pb.TagNumber(3)
  FullAccessPermission get fullAccess => $_getN(2);
  @$pb.TagNumber(3)
  set fullAccess(FullAccessPermission v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasFullAccess() => $_has(2);
  @$pb.TagNumber(3)
  void clearFullAccess() => clearField(3);
  @$pb.TagNumber(3)
  FullAccessPermission ensureFullAccess() => $_ensure(2);
}

/// Create Account
class CreateAccount extends $pb.GeneratedMessage {
  factory CreateAccount() => create();
  CreateAccount._() : super();
  factory CreateAccount.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory CreateAccount.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'CreateAccount', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  CreateAccount clone() => CreateAccount()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  CreateAccount copyWith(void Function(CreateAccount) updates) => super.copyWith((message) => updates(message as CreateAccount)) as CreateAccount;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static CreateAccount create() => CreateAccount._();
  CreateAccount createEmptyInstance() => create();
  static $pb.PbList<CreateAccount> createRepeated() => $pb.PbList<CreateAccount>();
  @$core.pragma('dart2js:noInline')
  static CreateAccount getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<CreateAccount>(create);
  static CreateAccount? _defaultInstance;
}

/// Deploying a contract
class DeployContract extends $pb.GeneratedMessage {
  factory DeployContract({
    $core.List<$core.int>? code,
  }) {
    final $result = create();
    if (code != null) {
      $result.code = code;
    }
    return $result;
  }
  DeployContract._() : super();
  factory DeployContract.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeployContract.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DeployContract', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'code', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DeployContract clone() => DeployContract()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DeployContract copyWith(void Function(DeployContract) updates) => super.copyWith((message) => updates(message as DeployContract)) as DeployContract;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DeployContract create() => DeployContract._();
  DeployContract createEmptyInstance() => create();
  static $pb.PbList<DeployContract> createRepeated() => $pb.PbList<DeployContract>();
  @$core.pragma('dart2js:noInline')
  static DeployContract getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeployContract>(create);
  static DeployContract? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.int> get code => $_getN(0);
  @$pb.TagNumber(1)
  set code($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasCode() => $_has(0);
  @$pb.TagNumber(1)
  void clearCode() => clearField(1);
}

/// A method/function call
class FunctionCall extends $pb.GeneratedMessage {
  factory FunctionCall({
    $core.String? methodName,
    $core.List<$core.int>? args,
    $fixnum.Int64? gas,
    $core.List<$core.int>? deposit,
  }) {
    final $result = create();
    if (methodName != null) {
      $result.methodName = methodName;
    }
    if (args != null) {
      $result.args = args;
    }
    if (gas != null) {
      $result.gas = gas;
    }
    if (deposit != null) {
      $result.deposit = deposit;
    }
    return $result;
  }
  FunctionCall._() : super();
  factory FunctionCall.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory FunctionCall.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'FunctionCall', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'methodName')
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'args', $pb.PbFieldType.OY)
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'gas', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'deposit', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  FunctionCall clone() => FunctionCall()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  FunctionCall copyWith(void Function(FunctionCall) updates) => super.copyWith((message) => updates(message as FunctionCall)) as FunctionCall;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static FunctionCall create() => FunctionCall._();
  FunctionCall createEmptyInstance() => create();
  static $pb.PbList<FunctionCall> createRepeated() => $pb.PbList<FunctionCall>();
  @$core.pragma('dart2js:noInline')
  static FunctionCall getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<FunctionCall>(create);
  static FunctionCall? _defaultInstance;

  /// Method/function name
  @$pb.TagNumber(1)
  $core.String get methodName => $_getSZ(0);
  @$pb.TagNumber(1)
  set methodName($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasMethodName() => $_has(0);
  @$pb.TagNumber(1)
  void clearMethodName() => clearField(1);

  /// input arguments
  @$pb.TagNumber(2)
  $core.List<$core.int> get args => $_getN(1);
  @$pb.TagNumber(2)
  set args($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasArgs() => $_has(1);
  @$pb.TagNumber(2)
  void clearArgs() => clearField(2);

  /// gas
  @$pb.TagNumber(3)
  $fixnum.Int64 get gas => $_getI64(2);
  @$pb.TagNumber(3)
  set gas($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasGas() => $_has(2);
  @$pb.TagNumber(3)
  void clearGas() => clearField(3);

  /// uint128 / big endian byte order
  @$pb.TagNumber(4)
  $core.List<$core.int> get deposit => $_getN(3);
  @$pb.TagNumber(4)
  set deposit($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasDeposit() => $_has(3);
  @$pb.TagNumber(4)
  void clearDeposit() => clearField(4);
}

/// Transfer
class Transfer extends $pb.GeneratedMessage {
  factory Transfer({
    $core.List<$core.int>? deposit,
  }) {
    final $result = create();
    if (deposit != null) {
      $result.deposit = deposit;
    }
    return $result;
  }
  Transfer._() : super();
  factory Transfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'deposit', $pb.PbFieldType.OY)
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

  /// amount; uint128 / big endian byte order
  @$pb.TagNumber(1)
  $core.List<$core.int> get deposit => $_getN(0);
  @$pb.TagNumber(1)
  set deposit($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDeposit() => $_has(0);
  @$pb.TagNumber(1)
  void clearDeposit() => clearField(1);
}

/// Stake
class Stake extends $pb.GeneratedMessage {
  factory Stake({
    $core.List<$core.int>? stake,
    PublicKey? publicKey,
  }) {
    final $result = create();
    if (stake != null) {
      $result.stake = stake;
    }
    if (publicKey != null) {
      $result.publicKey = publicKey;
    }
    return $result;
  }
  Stake._() : super();
  factory Stake.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Stake.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Stake', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'stake', $pb.PbFieldType.OY)
    ..aOM<PublicKey>(2, _omitFieldNames ? '' : 'publicKey', subBuilder: PublicKey.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Stake clone() => Stake()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Stake copyWith(void Function(Stake) updates) => super.copyWith((message) => updates(message as Stake)) as Stake;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Stake create() => Stake._();
  Stake createEmptyInstance() => create();
  static $pb.PbList<Stake> createRepeated() => $pb.PbList<Stake>();
  @$core.pragma('dart2js:noInline')
  static Stake getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Stake>(create);
  static Stake? _defaultInstance;

  /// amount; uint128 / big endian byte order
  @$pb.TagNumber(1)
  $core.List<$core.int> get stake => $_getN(0);
  @$pb.TagNumber(1)
  set stake($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasStake() => $_has(0);
  @$pb.TagNumber(1)
  void clearStake() => clearField(1);

  /// owner public key
  @$pb.TagNumber(2)
  PublicKey get publicKey => $_getN(1);
  @$pb.TagNumber(2)
  set publicKey(PublicKey v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasPublicKey() => $_has(1);
  @$pb.TagNumber(2)
  void clearPublicKey() => clearField(2);
  @$pb.TagNumber(2)
  PublicKey ensurePublicKey() => $_ensure(1);
}

/// Add a key
class AddKey extends $pb.GeneratedMessage {
  factory AddKey({
    PublicKey? publicKey,
    AccessKey? accessKey,
  }) {
    final $result = create();
    if (publicKey != null) {
      $result.publicKey = publicKey;
    }
    if (accessKey != null) {
      $result.accessKey = accessKey;
    }
    return $result;
  }
  AddKey._() : super();
  factory AddKey.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AddKey.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'AddKey', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..aOM<PublicKey>(1, _omitFieldNames ? '' : 'publicKey', subBuilder: PublicKey.create)
    ..aOM<AccessKey>(2, _omitFieldNames ? '' : 'accessKey', subBuilder: AccessKey.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  AddKey clone() => AddKey()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  AddKey copyWith(void Function(AddKey) updates) => super.copyWith((message) => updates(message as AddKey)) as AddKey;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static AddKey create() => AddKey._();
  AddKey createEmptyInstance() => create();
  static $pb.PbList<AddKey> createRepeated() => $pb.PbList<AddKey>();
  @$core.pragma('dart2js:noInline')
  static AddKey getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AddKey>(create);
  static AddKey? _defaultInstance;

  @$pb.TagNumber(1)
  PublicKey get publicKey => $_getN(0);
  @$pb.TagNumber(1)
  set publicKey(PublicKey v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasPublicKey() => $_has(0);
  @$pb.TagNumber(1)
  void clearPublicKey() => clearField(1);
  @$pb.TagNumber(1)
  PublicKey ensurePublicKey() => $_ensure(0);

  @$pb.TagNumber(2)
  AccessKey get accessKey => $_getN(1);
  @$pb.TagNumber(2)
  set accessKey(AccessKey v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasAccessKey() => $_has(1);
  @$pb.TagNumber(2)
  void clearAccessKey() => clearField(2);
  @$pb.TagNumber(2)
  AccessKey ensureAccessKey() => $_ensure(1);
}

/// Delete a key
class DeleteKey extends $pb.GeneratedMessage {
  factory DeleteKey({
    PublicKey? publicKey,
  }) {
    final $result = create();
    if (publicKey != null) {
      $result.publicKey = publicKey;
    }
    return $result;
  }
  DeleteKey._() : super();
  factory DeleteKey.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeleteKey.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DeleteKey', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..aOM<PublicKey>(1, _omitFieldNames ? '' : 'publicKey', subBuilder: PublicKey.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DeleteKey clone() => DeleteKey()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DeleteKey copyWith(void Function(DeleteKey) updates) => super.copyWith((message) => updates(message as DeleteKey)) as DeleteKey;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DeleteKey create() => DeleteKey._();
  DeleteKey createEmptyInstance() => create();
  static $pb.PbList<DeleteKey> createRepeated() => $pb.PbList<DeleteKey>();
  @$core.pragma('dart2js:noInline')
  static DeleteKey getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeleteKey>(create);
  static DeleteKey? _defaultInstance;

  @$pb.TagNumber(1)
  PublicKey get publicKey => $_getN(0);
  @$pb.TagNumber(1)
  set publicKey(PublicKey v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasPublicKey() => $_has(0);
  @$pb.TagNumber(1)
  void clearPublicKey() => clearField(1);
  @$pb.TagNumber(1)
  PublicKey ensurePublicKey() => $_ensure(0);
}

/// Delete account
class DeleteAccount extends $pb.GeneratedMessage {
  factory DeleteAccount({
    $core.String? beneficiaryId,
  }) {
    final $result = create();
    if (beneficiaryId != null) {
      $result.beneficiaryId = beneficiaryId;
    }
    return $result;
  }
  DeleteAccount._() : super();
  factory DeleteAccount.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeleteAccount.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DeleteAccount', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'beneficiaryId')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DeleteAccount clone() => DeleteAccount()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DeleteAccount copyWith(void Function(DeleteAccount) updates) => super.copyWith((message) => updates(message as DeleteAccount)) as DeleteAccount;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DeleteAccount create() => DeleteAccount._();
  DeleteAccount createEmptyInstance() => create();
  static $pb.PbList<DeleteAccount> createRepeated() => $pb.PbList<DeleteAccount>();
  @$core.pragma('dart2js:noInline')
  static DeleteAccount getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeleteAccount>(create);
  static DeleteAccount? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get beneficiaryId => $_getSZ(0);
  @$pb.TagNumber(1)
  set beneficiaryId($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasBeneficiaryId() => $_has(0);
  @$pb.TagNumber(1)
  void clearBeneficiaryId() => clearField(1);
}

/// Fungible token transfer
class TokenTransfer extends $pb.GeneratedMessage {
  factory TokenTransfer({
    $core.String? tokenAmount,
    $core.String? receiverId,
    $fixnum.Int64? gas,
    $core.List<$core.int>? deposit,
  }) {
    final $result = create();
    if (tokenAmount != null) {
      $result.tokenAmount = tokenAmount;
    }
    if (receiverId != null) {
      $result.receiverId = receiverId;
    }
    if (gas != null) {
      $result.gas = gas;
    }
    if (deposit != null) {
      $result.deposit = deposit;
    }
    return $result;
  }
  TokenTransfer._() : super();
  factory TokenTransfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TokenTransfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TokenTransfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'tokenAmount')
    ..aOS(2, _omitFieldNames ? '' : 'receiverId')
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'gas', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'deposit', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TokenTransfer clone() => TokenTransfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TokenTransfer copyWith(void Function(TokenTransfer) updates) => super.copyWith((message) => updates(message as TokenTransfer)) as TokenTransfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TokenTransfer create() => TokenTransfer._();
  TokenTransfer createEmptyInstance() => create();
  static $pb.PbList<TokenTransfer> createRepeated() => $pb.PbList<TokenTransfer>();
  @$core.pragma('dart2js:noInline')
  static TokenTransfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TokenTransfer>(create);
  static TokenTransfer? _defaultInstance;

  /// Token amount. Base-10 decimal string.
  @$pb.TagNumber(1)
  $core.String get tokenAmount => $_getSZ(0);
  @$pb.TagNumber(1)
  set tokenAmount($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasTokenAmount() => $_has(0);
  @$pb.TagNumber(1)
  void clearTokenAmount() => clearField(1);

  /// ID of the receiver.
  @$pb.TagNumber(2)
  $core.String get receiverId => $_getSZ(1);
  @$pb.TagNumber(2)
  set receiverId($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasReceiverId() => $_has(1);
  @$pb.TagNumber(2)
  void clearReceiverId() => clearField(2);

  /// Gas.
  @$pb.TagNumber(3)
  $fixnum.Int64 get gas => $_getI64(2);
  @$pb.TagNumber(3)
  set gas($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasGas() => $_has(2);
  @$pb.TagNumber(3)
  void clearGas() => clearField(3);

  /// NEAR deposit amount; uint128 / big endian byte order.
  @$pb.TagNumber(4)
  $core.List<$core.int> get deposit => $_getN(3);
  @$pb.TagNumber(4)
  set deposit($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasDeposit() => $_has(3);
  @$pb.TagNumber(4)
  void clearDeposit() => clearField(4);
}

enum Action_Payload {
  createAccount, 
  deployContract, 
  functionCall, 
  transfer, 
  stake, 
  addKey, 
  deleteKey, 
  deleteAccount, 
  tokenTransfer, 
  notSet
}

/// Represents an action
class Action extends $pb.GeneratedMessage {
  factory Action({
    CreateAccount? createAccount,
    DeployContract? deployContract,
    FunctionCall? functionCall,
    Transfer? transfer,
    Stake? stake,
    AddKey? addKey,
    DeleteKey? deleteKey,
    DeleteAccount? deleteAccount,
    TokenTransfer? tokenTransfer,
  }) {
    final $result = create();
    if (createAccount != null) {
      $result.createAccount = createAccount;
    }
    if (deployContract != null) {
      $result.deployContract = deployContract;
    }
    if (functionCall != null) {
      $result.functionCall = functionCall;
    }
    if (transfer != null) {
      $result.transfer = transfer;
    }
    if (stake != null) {
      $result.stake = stake;
    }
    if (addKey != null) {
      $result.addKey = addKey;
    }
    if (deleteKey != null) {
      $result.deleteKey = deleteKey;
    }
    if (deleteAccount != null) {
      $result.deleteAccount = deleteAccount;
    }
    if (tokenTransfer != null) {
      $result.tokenTransfer = tokenTransfer;
    }
    return $result;
  }
  Action._() : super();
  factory Action.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Action.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Action_Payload> _Action_PayloadByTag = {
    1 : Action_Payload.createAccount,
    2 : Action_Payload.deployContract,
    3 : Action_Payload.functionCall,
    4 : Action_Payload.transfer,
    5 : Action_Payload.stake,
    6 : Action_Payload.addKey,
    7 : Action_Payload.deleteKey,
    8 : Action_Payload.deleteAccount,
    13 : Action_Payload.tokenTransfer,
    0 : Action_Payload.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Action', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2, 3, 4, 5, 6, 7, 8, 13])
    ..aOM<CreateAccount>(1, _omitFieldNames ? '' : 'createAccount', subBuilder: CreateAccount.create)
    ..aOM<DeployContract>(2, _omitFieldNames ? '' : 'deployContract', subBuilder: DeployContract.create)
    ..aOM<FunctionCall>(3, _omitFieldNames ? '' : 'functionCall', subBuilder: FunctionCall.create)
    ..aOM<Transfer>(4, _omitFieldNames ? '' : 'transfer', subBuilder: Transfer.create)
    ..aOM<Stake>(5, _omitFieldNames ? '' : 'stake', subBuilder: Stake.create)
    ..aOM<AddKey>(6, _omitFieldNames ? '' : 'addKey', subBuilder: AddKey.create)
    ..aOM<DeleteKey>(7, _omitFieldNames ? '' : 'deleteKey', subBuilder: DeleteKey.create)
    ..aOM<DeleteAccount>(8, _omitFieldNames ? '' : 'deleteAccount', subBuilder: DeleteAccount.create)
    ..aOM<TokenTransfer>(13, _omitFieldNames ? '' : 'tokenTransfer', subBuilder: TokenTransfer.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Action clone() => Action()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Action copyWith(void Function(Action) updates) => super.copyWith((message) => updates(message as Action)) as Action;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Action create() => Action._();
  Action createEmptyInstance() => create();
  static $pb.PbList<Action> createRepeated() => $pb.PbList<Action>();
  @$core.pragma('dart2js:noInline')
  static Action getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Action>(create);
  static Action? _defaultInstance;

  Action_Payload whichPayload() => _Action_PayloadByTag[$_whichOneof(0)]!;
  void clearPayload() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  CreateAccount get createAccount => $_getN(0);
  @$pb.TagNumber(1)
  set createAccount(CreateAccount v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasCreateAccount() => $_has(0);
  @$pb.TagNumber(1)
  void clearCreateAccount() => clearField(1);
  @$pb.TagNumber(1)
  CreateAccount ensureCreateAccount() => $_ensure(0);

  @$pb.TagNumber(2)
  DeployContract get deployContract => $_getN(1);
  @$pb.TagNumber(2)
  set deployContract(DeployContract v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasDeployContract() => $_has(1);
  @$pb.TagNumber(2)
  void clearDeployContract() => clearField(2);
  @$pb.TagNumber(2)
  DeployContract ensureDeployContract() => $_ensure(1);

  @$pb.TagNumber(3)
  FunctionCall get functionCall => $_getN(2);
  @$pb.TagNumber(3)
  set functionCall(FunctionCall v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasFunctionCall() => $_has(2);
  @$pb.TagNumber(3)
  void clearFunctionCall() => clearField(3);
  @$pb.TagNumber(3)
  FunctionCall ensureFunctionCall() => $_ensure(2);

  @$pb.TagNumber(4)
  Transfer get transfer => $_getN(3);
  @$pb.TagNumber(4)
  set transfer(Transfer v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasTransfer() => $_has(3);
  @$pb.TagNumber(4)
  void clearTransfer() => clearField(4);
  @$pb.TagNumber(4)
  Transfer ensureTransfer() => $_ensure(3);

  @$pb.TagNumber(5)
  Stake get stake => $_getN(4);
  @$pb.TagNumber(5)
  set stake(Stake v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasStake() => $_has(4);
  @$pb.TagNumber(5)
  void clearStake() => clearField(5);
  @$pb.TagNumber(5)
  Stake ensureStake() => $_ensure(4);

  @$pb.TagNumber(6)
  AddKey get addKey => $_getN(5);
  @$pb.TagNumber(6)
  set addKey(AddKey v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasAddKey() => $_has(5);
  @$pb.TagNumber(6)
  void clearAddKey() => clearField(6);
  @$pb.TagNumber(6)
  AddKey ensureAddKey() => $_ensure(5);

  @$pb.TagNumber(7)
  DeleteKey get deleteKey => $_getN(6);
  @$pb.TagNumber(7)
  set deleteKey(DeleteKey v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasDeleteKey() => $_has(6);
  @$pb.TagNumber(7)
  void clearDeleteKey() => clearField(7);
  @$pb.TagNumber(7)
  DeleteKey ensureDeleteKey() => $_ensure(6);

  @$pb.TagNumber(8)
  DeleteAccount get deleteAccount => $_getN(7);
  @$pb.TagNumber(8)
  set deleteAccount(DeleteAccount v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasDeleteAccount() => $_has(7);
  @$pb.TagNumber(8)
  void clearDeleteAccount() => clearField(8);
  @$pb.TagNumber(8)
  DeleteAccount ensureDeleteAccount() => $_ensure(7);

  /// Gap in field numbering is intentional as it's not a standard NEAR action.
  @$pb.TagNumber(13)
  TokenTransfer get tokenTransfer => $_getN(8);
  @$pb.TagNumber(13)
  set tokenTransfer(TokenTransfer v) { setField(13, v); }
  @$pb.TagNumber(13)
  $core.bool hasTokenTransfer() => $_has(8);
  @$pb.TagNumber(13)
  void clearTokenTransfer() => clearField(13);
  @$pb.TagNumber(13)
  TokenTransfer ensureTokenTransfer() => $_ensure(8);
}

/// Input data necessary to create a signed order.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.String? signerId,
    $fixnum.Int64? nonce,
    $core.String? receiverId,
    $core.List<$core.int>? blockHash,
    $core.Iterable<Action>? actions,
    $core.List<$core.int>? privateKey,
    $core.List<$core.int>? publicKey,
  }) {
    final $result = create();
    if (signerId != null) {
      $result.signerId = signerId;
    }
    if (nonce != null) {
      $result.nonce = nonce;
    }
    if (receiverId != null) {
      $result.receiverId = receiverId;
    }
    if (blockHash != null) {
      $result.blockHash = blockHash;
    }
    if (actions != null) {
      $result.actions.addAll(actions);
    }
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (publicKey != null) {
      $result.publicKey = publicKey;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'signerId')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'nonce', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(3, _omitFieldNames ? '' : 'receiverId')
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'blockHash', $pb.PbFieldType.OY)
    ..pc<Action>(5, _omitFieldNames ? '' : 'actions', $pb.PbFieldType.PM, subBuilder: Action.create)
    ..a<$core.List<$core.int>>(6, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(7, _omitFieldNames ? '' : 'publicKey', $pb.PbFieldType.OY)
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

  /// ID of the sender
  @$pb.TagNumber(1)
  $core.String get signerId => $_getSZ(0);
  @$pb.TagNumber(1)
  set signerId($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSignerId() => $_has(0);
  @$pb.TagNumber(1)
  void clearSignerId() => clearField(1);

  /// Nonce (should be larger than in the last transaction of the account)
  @$pb.TagNumber(2)
  $fixnum.Int64 get nonce => $_getI64(1);
  @$pb.TagNumber(2)
  set nonce($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasNonce() => $_has(1);
  @$pb.TagNumber(2)
  void clearNonce() => clearField(2);

  /// ID of the receiver
  @$pb.TagNumber(3)
  $core.String get receiverId => $_getSZ(2);
  @$pb.TagNumber(3)
  set receiverId($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasReceiverId() => $_has(2);
  @$pb.TagNumber(3)
  void clearReceiverId() => clearField(3);

  /// Recent block hash
  @$pb.TagNumber(4)
  $core.List<$core.int> get blockHash => $_getN(3);
  @$pb.TagNumber(4)
  set blockHash($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasBlockHash() => $_has(3);
  @$pb.TagNumber(4)
  void clearBlockHash() => clearField(4);

  /// Payload action(s)
  @$pb.TagNumber(5)
  $core.List<Action> get actions => $_getList(4);

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(6)
  $core.List<$core.int> get privateKey => $_getN(5);
  @$pb.TagNumber(6)
  set privateKey($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasPrivateKey() => $_has(5);
  @$pb.TagNumber(6)
  void clearPrivateKey() => clearField(6);

  /// The public key used for compiling a transaction with a signature.
  @$pb.TagNumber(7)
  $core.List<$core.int> get publicKey => $_getN(6);
  @$pb.TagNumber(7)
  set publicKey($core.List<$core.int> v) { $_setBytes(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasPublicKey() => $_has(6);
  @$pb.TagNumber(7)
  void clearPublicKey() => clearField(7);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.List<$core.int>? signedTransaction,
    $0.SigningError? error,
    $core.String? errorMessage,
    $core.List<$core.int>? hash,
  }) {
    final $result = create();
    if (signedTransaction != null) {
      $result.signedTransaction = signedTransaction;
    }
    if (error != null) {
      $result.error = error;
    }
    if (errorMessage != null) {
      $result.errorMessage = errorMessage;
    }
    if (hash != null) {
      $result.hash = hash;
    }
    return $result;
  }
  SigningOutput._() : super();
  factory SigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEAR.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'signedTransaction', $pb.PbFieldType.OY)
    ..e<$0.SigningError>(2, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(3, _omitFieldNames ? '' : 'errorMessage')
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'hash', $pb.PbFieldType.OY)
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

  /// Signed transaction blob
  @$pb.TagNumber(1)
  $core.List<$core.int> get signedTransaction => $_getN(0);
  @$pb.TagNumber(1)
  set signedTransaction($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSignedTransaction() => $_has(0);
  @$pb.TagNumber(1)
  void clearSignedTransaction() => clearField(1);

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

  /// Hash of the transaction
  @$pb.TagNumber(4)
  $core.List<$core.int> get hash => $_getN(3);
  @$pb.TagNumber(4)
  set hash($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasHash() => $_has(3);
  @$pb.TagNumber(4)
  void clearHash() => clearField(4);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
