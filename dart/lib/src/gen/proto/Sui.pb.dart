//
//  Generated code. Do not modify.
//  source: Sui.proto
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

/// Object info (including Coins).
class ObjectRef extends $pb.GeneratedMessage {
  factory ObjectRef({
    $core.String? objectId,
    $fixnum.Int64? version,
    $core.String? objectDigest,
  }) {
    final $result = create();
    if (objectId != null) {
      $result.objectId = objectId;
    }
    if (version != null) {
      $result.version = version;
    }
    if (objectDigest != null) {
      $result.objectDigest = objectDigest;
    }
    return $result;
  }
  ObjectRef._() : super();
  factory ObjectRef.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ObjectRef.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'ObjectRef', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Sui.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'objectId')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'version', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(3, _omitFieldNames ? '' : 'objectDigest')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ObjectRef clone() => ObjectRef()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ObjectRef copyWith(void Function(ObjectRef) updates) => super.copyWith((message) => updates(message as ObjectRef)) as ObjectRef;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static ObjectRef create() => ObjectRef._();
  ObjectRef createEmptyInstance() => create();
  static $pb.PbList<ObjectRef> createRepeated() => $pb.PbList<ObjectRef>();
  @$core.pragma('dart2js:noInline')
  static ObjectRef getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ObjectRef>(create);
  static ObjectRef? _defaultInstance;

  /// Hex string representing the object ID.
  @$pb.TagNumber(1)
  $core.String get objectId => $_getSZ(0);
  @$pb.TagNumber(1)
  set objectId($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasObjectId() => $_has(0);
  @$pb.TagNumber(1)
  void clearObjectId() => clearField(1);

  /// Object version.
  @$pb.TagNumber(2)
  $fixnum.Int64 get version => $_getI64(1);
  @$pb.TagNumber(2)
  set version($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasVersion() => $_has(1);
  @$pb.TagNumber(2)
  void clearVersion() => clearField(2);

  /// Base58 string representing the object digest.
  @$pb.TagNumber(3)
  $core.String get objectDigest => $_getSZ(2);
  @$pb.TagNumber(3)
  set objectDigest($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasObjectDigest() => $_has(2);
  @$pb.TagNumber(3)
  void clearObjectDigest() => clearField(3);
}

/// Optional amount.
class Amount extends $pb.GeneratedMessage {
  factory Amount({
    $fixnum.Int64? amount,
  }) {
    final $result = create();
    if (amount != null) {
      $result.amount = amount;
    }
    return $result;
  }
  Amount._() : super();
  factory Amount.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Amount.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Amount', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Sui.Proto'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Amount clone() => Amount()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Amount copyWith(void Function(Amount) updates) => super.copyWith((message) => updates(message as Amount)) as Amount;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Amount create() => Amount._();
  Amount createEmptyInstance() => create();
  static $pb.PbList<Amount> createRepeated() => $pb.PbList<Amount>();
  @$core.pragma('dart2js:noInline')
  static Amount getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Amount>(create);
  static Amount? _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get amount => $_getI64(0);
  @$pb.TagNumber(1)
  set amount($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAmount() => $_has(0);
  @$pb.TagNumber(1)
  void clearAmount() => clearField(1);
}

/// Base64 encoded msg to sign (string)
class SignDirect extends $pb.GeneratedMessage {
  factory SignDirect({
    $core.String? unsignedTxMsg,
  }) {
    final $result = create();
    if (unsignedTxMsg != null) {
      $result.unsignedTxMsg = unsignedTxMsg;
    }
    return $result;
  }
  SignDirect._() : super();
  factory SignDirect.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SignDirect.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SignDirect', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Sui.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'unsignedTxMsg')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SignDirect clone() => SignDirect()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SignDirect copyWith(void Function(SignDirect) updates) => super.copyWith((message) => updates(message as SignDirect)) as SignDirect;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SignDirect create() => SignDirect._();
  SignDirect createEmptyInstance() => create();
  static $pb.PbList<SignDirect> createRepeated() => $pb.PbList<SignDirect>();
  @$core.pragma('dart2js:noInline')
  static SignDirect getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SignDirect>(create);
  static SignDirect? _defaultInstance;

  /// Obtain by calling any write RpcJson on SUI
  @$pb.TagNumber(1)
  $core.String get unsignedTxMsg => $_getSZ(0);
  @$pb.TagNumber(1)
  set unsignedTxMsg($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasUnsignedTxMsg() => $_has(0);
  @$pb.TagNumber(1)
  void clearUnsignedTxMsg() => clearField(1);
}

/// Send `Coin<T>` to a list of addresses, where T can be any coin type, following a list of amounts.
/// The object specified in the gas field will be used to pay the gas fee for the transaction.
/// The gas object can not appear in input_coins.
/// https://docs.sui.io/sui-api-ref#unsafe_pay
class Pay extends $pb.GeneratedMessage {
  factory Pay({
    $core.Iterable<ObjectRef>? inputCoins,
    $core.Iterable<$core.String>? recipients,
    $core.Iterable<$fixnum.Int64>? amounts,
    ObjectRef? gas,
  }) {
    final $result = create();
    if (inputCoins != null) {
      $result.inputCoins.addAll(inputCoins);
    }
    if (recipients != null) {
      $result.recipients.addAll(recipients);
    }
    if (amounts != null) {
      $result.amounts.addAll(amounts);
    }
    if (gas != null) {
      $result.gas = gas;
    }
    return $result;
  }
  Pay._() : super();
  factory Pay.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Pay.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Pay', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Sui.Proto'), createEmptyInstance: create)
    ..pc<ObjectRef>(1, _omitFieldNames ? '' : 'inputCoins', $pb.PbFieldType.PM, subBuilder: ObjectRef.create)
    ..pPS(2, _omitFieldNames ? '' : 'recipients')
    ..p<$fixnum.Int64>(3, _omitFieldNames ? '' : 'amounts', $pb.PbFieldType.KU6)
    ..aOM<ObjectRef>(4, _omitFieldNames ? '' : 'gas', subBuilder: ObjectRef.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Pay clone() => Pay()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Pay copyWith(void Function(Pay) updates) => super.copyWith((message) => updates(message as Pay)) as Pay;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Pay create() => Pay._();
  Pay createEmptyInstance() => create();
  static $pb.PbList<Pay> createRepeated() => $pb.PbList<Pay>();
  @$core.pragma('dart2js:noInline')
  static Pay getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Pay>(create);
  static Pay? _defaultInstance;

  /// The Sui coins to be used in this transaction, including the coin for gas payment.
  @$pb.TagNumber(1)
  $core.List<ObjectRef> get inputCoins => $_getList(0);

  /// The recipients' addresses, the length of this vector must be the same as amounts.
  @$pb.TagNumber(2)
  $core.List<$core.String> get recipients => $_getList(1);

  /// The amounts to be transferred to recipients, following the same order.
  @$pb.TagNumber(3)
  $core.List<$fixnum.Int64> get amounts => $_getList(2);

  /// Gas object to be used in this transaction.
  @$pb.TagNumber(4)
  ObjectRef get gas => $_getN(3);
  @$pb.TagNumber(4)
  set gas(ObjectRef v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasGas() => $_has(3);
  @$pb.TagNumber(4)
  void clearGas() => clearField(4);
  @$pb.TagNumber(4)
  ObjectRef ensureGas() => $_ensure(3);
}

/// Send SUI coins to a list of addresses, following a list of amounts.
/// This is for SUI coin only and does not require a separate gas coin object.
/// https://docs.sui.io/sui-api-ref#unsafe_paysui
class PaySui extends $pb.GeneratedMessage {
  factory PaySui({
    $core.Iterable<ObjectRef>? inputCoins,
    $core.Iterable<$core.String>? recipients,
    $core.Iterable<$fixnum.Int64>? amounts,
  }) {
    final $result = create();
    if (inputCoins != null) {
      $result.inputCoins.addAll(inputCoins);
    }
    if (recipients != null) {
      $result.recipients.addAll(recipients);
    }
    if (amounts != null) {
      $result.amounts.addAll(amounts);
    }
    return $result;
  }
  PaySui._() : super();
  factory PaySui.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PaySui.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'PaySui', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Sui.Proto'), createEmptyInstance: create)
    ..pc<ObjectRef>(1, _omitFieldNames ? '' : 'inputCoins', $pb.PbFieldType.PM, subBuilder: ObjectRef.create)
    ..pPS(2, _omitFieldNames ? '' : 'recipients')
    ..p<$fixnum.Int64>(3, _omitFieldNames ? '' : 'amounts', $pb.PbFieldType.KU6)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  PaySui clone() => PaySui()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  PaySui copyWith(void Function(PaySui) updates) => super.copyWith((message) => updates(message as PaySui)) as PaySui;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static PaySui create() => PaySui._();
  PaySui createEmptyInstance() => create();
  static $pb.PbList<PaySui> createRepeated() => $pb.PbList<PaySui>();
  @$core.pragma('dart2js:noInline')
  static PaySui getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PaySui>(create);
  static PaySui? _defaultInstance;

  /// The Sui coins to be used in this transaction, including the coin for gas payment.
  @$pb.TagNumber(1)
  $core.List<ObjectRef> get inputCoins => $_getList(0);

  /// The recipients' addresses, the length of this vector must be the same as amounts.
  @$pb.TagNumber(2)
  $core.List<$core.String> get recipients => $_getList(1);

  /// The amounts to be transferred to recipients, following the same order.
  @$pb.TagNumber(3)
  $core.List<$fixnum.Int64> get amounts => $_getList(2);
}

/// Send all SUI coins to one recipient.
/// This is for SUI coin only and does not require a separate gas coin object.
/// https://docs.sui.io/sui-api-ref#unsafe_payallsui
class PayAllSui extends $pb.GeneratedMessage {
  factory PayAllSui({
    $core.Iterable<ObjectRef>? inputCoins,
    $core.String? recipient,
  }) {
    final $result = create();
    if (inputCoins != null) {
      $result.inputCoins.addAll(inputCoins);
    }
    if (recipient != null) {
      $result.recipient = recipient;
    }
    return $result;
  }
  PayAllSui._() : super();
  factory PayAllSui.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PayAllSui.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'PayAllSui', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Sui.Proto'), createEmptyInstance: create)
    ..pc<ObjectRef>(1, _omitFieldNames ? '' : 'inputCoins', $pb.PbFieldType.PM, subBuilder: ObjectRef.create)
    ..aOS(2, _omitFieldNames ? '' : 'recipient')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  PayAllSui clone() => PayAllSui()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  PayAllSui copyWith(void Function(PayAllSui) updates) => super.copyWith((message) => updates(message as PayAllSui)) as PayAllSui;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static PayAllSui create() => PayAllSui._();
  PayAllSui createEmptyInstance() => create();
  static $pb.PbList<PayAllSui> createRepeated() => $pb.PbList<PayAllSui>();
  @$core.pragma('dart2js:noInline')
  static PayAllSui getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PayAllSui>(create);
  static PayAllSui? _defaultInstance;

  /// The Sui coins to be used in this transaction, including the coin for gas payment.
  @$pb.TagNumber(1)
  $core.List<ObjectRef> get inputCoins => $_getList(0);

  /// The recipient address.
  @$pb.TagNumber(2)
  $core.String get recipient => $_getSZ(1);
  @$pb.TagNumber(2)
  set recipient($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasRecipient() => $_has(1);
  @$pb.TagNumber(2)
  void clearRecipient() => clearField(2);
}

/// Add stake to a validator's staking pool using multiple coins and amount.
/// https://docs.sui.io/sui-api-ref#unsafe_requestaddstake
class RequestAddStake extends $pb.GeneratedMessage {
  factory RequestAddStake({
    $core.Iterable<ObjectRef>? coins,
    Amount? amount,
    $core.String? validator,
    ObjectRef? gas,
  }) {
    final $result = create();
    if (coins != null) {
      $result.coins.addAll(coins);
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (validator != null) {
      $result.validator = validator;
    }
    if (gas != null) {
      $result.gas = gas;
    }
    return $result;
  }
  RequestAddStake._() : super();
  factory RequestAddStake.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RequestAddStake.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'RequestAddStake', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Sui.Proto'), createEmptyInstance: create)
    ..pc<ObjectRef>(1, _omitFieldNames ? '' : 'coins', $pb.PbFieldType.PM, subBuilder: ObjectRef.create)
    ..aOM<Amount>(2, _omitFieldNames ? '' : 'amount', subBuilder: Amount.create)
    ..aOS(3, _omitFieldNames ? '' : 'validator')
    ..aOM<ObjectRef>(4, _omitFieldNames ? '' : 'gas', subBuilder: ObjectRef.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  RequestAddStake clone() => RequestAddStake()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  RequestAddStake copyWith(void Function(RequestAddStake) updates) => super.copyWith((message) => updates(message as RequestAddStake)) as RequestAddStake;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static RequestAddStake create() => RequestAddStake._();
  RequestAddStake createEmptyInstance() => create();
  static $pb.PbList<RequestAddStake> createRepeated() => $pb.PbList<RequestAddStake>();
  @$core.pragma('dart2js:noInline')
  static RequestAddStake getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RequestAddStake>(create);
  static RequestAddStake? _defaultInstance;

  /// Coin<SUI> objects to stake.
  @$pb.TagNumber(1)
  $core.List<ObjectRef> get coins => $_getList(0);

  /// Optional stake amount.
  @$pb.TagNumber(2)
  Amount get amount => $_getN(1);
  @$pb.TagNumber(2)
  set amount(Amount v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);
  @$pb.TagNumber(2)
  Amount ensureAmount() => $_ensure(1);

  /// The validator's Sui address.
  @$pb.TagNumber(3)
  $core.String get validator => $_getSZ(2);
  @$pb.TagNumber(3)
  set validator($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasValidator() => $_has(2);
  @$pb.TagNumber(3)
  void clearValidator() => clearField(3);

  /// Gas object to be used in this transaction.
  @$pb.TagNumber(4)
  ObjectRef get gas => $_getN(3);
  @$pb.TagNumber(4)
  set gas(ObjectRef v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasGas() => $_has(3);
  @$pb.TagNumber(4)
  void clearGas() => clearField(4);
  @$pb.TagNumber(4)
  ObjectRef ensureGas() => $_ensure(3);
}

/// Withdraw stake from a validator's staking pool.
/// https://docs.sui.io/sui-api-ref#unsafe_requestwithdrawstake
class RequestWithdrawStake extends $pb.GeneratedMessage {
  factory RequestWithdrawStake({
    ObjectRef? stakedSui,
    ObjectRef? gas,
  }) {
    final $result = create();
    if (stakedSui != null) {
      $result.stakedSui = stakedSui;
    }
    if (gas != null) {
      $result.gas = gas;
    }
    return $result;
  }
  RequestWithdrawStake._() : super();
  factory RequestWithdrawStake.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RequestWithdrawStake.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'RequestWithdrawStake', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Sui.Proto'), createEmptyInstance: create)
    ..aOM<ObjectRef>(1, _omitFieldNames ? '' : 'stakedSui', subBuilder: ObjectRef.create)
    ..aOM<ObjectRef>(2, _omitFieldNames ? '' : 'gas', subBuilder: ObjectRef.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  RequestWithdrawStake clone() => RequestWithdrawStake()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  RequestWithdrawStake copyWith(void Function(RequestWithdrawStake) updates) => super.copyWith((message) => updates(message as RequestWithdrawStake)) as RequestWithdrawStake;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static RequestWithdrawStake create() => RequestWithdrawStake._();
  RequestWithdrawStake createEmptyInstance() => create();
  static $pb.PbList<RequestWithdrawStake> createRepeated() => $pb.PbList<RequestWithdrawStake>();
  @$core.pragma('dart2js:noInline')
  static RequestWithdrawStake getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RequestWithdrawStake>(create);
  static RequestWithdrawStake? _defaultInstance;

  /// StakedSui object ID.
  @$pb.TagNumber(1)
  ObjectRef get stakedSui => $_getN(0);
  @$pb.TagNumber(1)
  set stakedSui(ObjectRef v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasStakedSui() => $_has(0);
  @$pb.TagNumber(1)
  void clearStakedSui() => clearField(1);
  @$pb.TagNumber(1)
  ObjectRef ensureStakedSui() => $_ensure(0);

  /// Gas object to be used in this transaction.
  @$pb.TagNumber(2)
  ObjectRef get gas => $_getN(1);
  @$pb.TagNumber(2)
  set gas(ObjectRef v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasGas() => $_has(1);
  @$pb.TagNumber(2)
  void clearGas() => clearField(2);
  @$pb.TagNumber(2)
  ObjectRef ensureGas() => $_ensure(1);
}

/// / Transfer an object from one address to another. The object's type must allow public transfers.
/// / https://docs.sui.io/sui-api-ref#unsafe_transferobject
class TransferObject extends $pb.GeneratedMessage {
  factory TransferObject({
    ObjectRef? object,
    $core.String? recipient,
    ObjectRef? gas,
  }) {
    final $result = create();
    if (object != null) {
      $result.object = object;
    }
    if (recipient != null) {
      $result.recipient = recipient;
    }
    if (gas != null) {
      $result.gas = gas;
    }
    return $result;
  }
  TransferObject._() : super();
  factory TransferObject.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TransferObject.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TransferObject', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Sui.Proto'), createEmptyInstance: create)
    ..aOM<ObjectRef>(1, _omitFieldNames ? '' : 'object', subBuilder: ObjectRef.create)
    ..aOS(2, _omitFieldNames ? '' : 'recipient')
    ..aOM<ObjectRef>(3, _omitFieldNames ? '' : 'gas', subBuilder: ObjectRef.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TransferObject clone() => TransferObject()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TransferObject copyWith(void Function(TransferObject) updates) => super.copyWith((message) => updates(message as TransferObject)) as TransferObject;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TransferObject create() => TransferObject._();
  TransferObject createEmptyInstance() => create();
  static $pb.PbList<TransferObject> createRepeated() => $pb.PbList<TransferObject>();
  @$core.pragma('dart2js:noInline')
  static TransferObject getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TransferObject>(create);
  static TransferObject? _defaultInstance;

  /// Object ID to be transferred.
  @$pb.TagNumber(1)
  ObjectRef get object => $_getN(0);
  @$pb.TagNumber(1)
  set object(ObjectRef v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasObject() => $_has(0);
  @$pb.TagNumber(1)
  void clearObject() => clearField(1);
  @$pb.TagNumber(1)
  ObjectRef ensureObject() => $_ensure(0);

  /// The recipient address.
  @$pb.TagNumber(2)
  $core.String get recipient => $_getSZ(1);
  @$pb.TagNumber(2)
  set recipient($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasRecipient() => $_has(1);
  @$pb.TagNumber(2)
  void clearRecipient() => clearField(2);

  /// Gas object to be used in this transaction.
  @$pb.TagNumber(3)
  ObjectRef get gas => $_getN(2);
  @$pb.TagNumber(3)
  set gas(ObjectRef v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasGas() => $_has(2);
  @$pb.TagNumber(3)
  void clearGas() => clearField(3);
  @$pb.TagNumber(3)
  ObjectRef ensureGas() => $_ensure(2);
}

enum SigningInput_TransactionPayload {
  signDirectMessage, 
  pay, 
  paySui, 
  payAllSui, 
  requestAddStake, 
  requestWithdrawStake, 
  transferObject, 
  notSet
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.List<$core.int>? privateKey,
    $core.String? signer,
    SignDirect? signDirectMessage,
    Pay? pay,
    PaySui? paySui,
    PayAllSui? payAllSui,
    RequestAddStake? requestAddStake,
    RequestWithdrawStake? requestWithdrawStake,
    TransferObject? transferObject,
    $fixnum.Int64? gasBudget,
    $fixnum.Int64? referenceGasPrice,
  }) {
    final $result = create();
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (signer != null) {
      $result.signer = signer;
    }
    if (signDirectMessage != null) {
      $result.signDirectMessage = signDirectMessage;
    }
    if (pay != null) {
      $result.pay = pay;
    }
    if (paySui != null) {
      $result.paySui = paySui;
    }
    if (payAllSui != null) {
      $result.payAllSui = payAllSui;
    }
    if (requestAddStake != null) {
      $result.requestAddStake = requestAddStake;
    }
    if (requestWithdrawStake != null) {
      $result.requestWithdrawStake = requestWithdrawStake;
    }
    if (transferObject != null) {
      $result.transferObject = transferObject;
    }
    if (gasBudget != null) {
      $result.gasBudget = gasBudget;
    }
    if (referenceGasPrice != null) {
      $result.referenceGasPrice = referenceGasPrice;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, SigningInput_TransactionPayload> _SigningInput_TransactionPayloadByTag = {
    3 : SigningInput_TransactionPayload.signDirectMessage,
    4 : SigningInput_TransactionPayload.pay,
    5 : SigningInput_TransactionPayload.paySui,
    6 : SigningInput_TransactionPayload.payAllSui,
    7 : SigningInput_TransactionPayload.requestAddStake,
    8 : SigningInput_TransactionPayload.requestWithdrawStake,
    9 : SigningInput_TransactionPayload.transferObject,
    0 : SigningInput_TransactionPayload.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Sui.Proto'), createEmptyInstance: create)
    ..oo(0, [3, 4, 5, 6, 7, 8, 9])
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..aOS(2, _omitFieldNames ? '' : 'signer')
    ..aOM<SignDirect>(3, _omitFieldNames ? '' : 'signDirectMessage', subBuilder: SignDirect.create)
    ..aOM<Pay>(4, _omitFieldNames ? '' : 'pay', subBuilder: Pay.create)
    ..aOM<PaySui>(5, _omitFieldNames ? '' : 'paySui', subBuilder: PaySui.create)
    ..aOM<PayAllSui>(6, _omitFieldNames ? '' : 'payAllSui', subBuilder: PayAllSui.create)
    ..aOM<RequestAddStake>(7, _omitFieldNames ? '' : 'requestAddStake', subBuilder: RequestAddStake.create)
    ..aOM<RequestWithdrawStake>(8, _omitFieldNames ? '' : 'requestWithdrawStake', subBuilder: RequestWithdrawStake.create)
    ..aOM<TransferObject>(9, _omitFieldNames ? '' : 'transferObject', subBuilder: TransferObject.create)
    ..a<$fixnum.Int64>(12, _omitFieldNames ? '' : 'gasBudget', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(13, _omitFieldNames ? '' : 'referenceGasPrice', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
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

  SigningInput_TransactionPayload whichTransactionPayload() => _SigningInput_TransactionPayloadByTag[$_whichOneof(0)]!;
  void clearTransactionPayload() => clearField($_whichOneof(0));

  /// Private key to sign the transaction (bytes).
  @$pb.TagNumber(1)
  $core.List<$core.int> get privateKey => $_getN(0);
  @$pb.TagNumber(1)
  set privateKey($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPrivateKey() => $_has(0);
  @$pb.TagNumber(1)
  void clearPrivateKey() => clearField(1);

  /// Optional transaction signer.
  /// Needs to be set if no private key provided at `TransactionCompiler` module.
  @$pb.TagNumber(2)
  $core.String get signer => $_getSZ(1);
  @$pb.TagNumber(2)
  set signer($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasSigner() => $_has(1);
  @$pb.TagNumber(2)
  void clearSigner() => clearField(2);

  @$pb.TagNumber(3)
  SignDirect get signDirectMessage => $_getN(2);
  @$pb.TagNumber(3)
  set signDirectMessage(SignDirect v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasSignDirectMessage() => $_has(2);
  @$pb.TagNumber(3)
  void clearSignDirectMessage() => clearField(3);
  @$pb.TagNumber(3)
  SignDirect ensureSignDirectMessage() => $_ensure(2);

  @$pb.TagNumber(4)
  Pay get pay => $_getN(3);
  @$pb.TagNumber(4)
  set pay(Pay v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasPay() => $_has(3);
  @$pb.TagNumber(4)
  void clearPay() => clearField(4);
  @$pb.TagNumber(4)
  Pay ensurePay() => $_ensure(3);

  @$pb.TagNumber(5)
  PaySui get paySui => $_getN(4);
  @$pb.TagNumber(5)
  set paySui(PaySui v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasPaySui() => $_has(4);
  @$pb.TagNumber(5)
  void clearPaySui() => clearField(5);
  @$pb.TagNumber(5)
  PaySui ensurePaySui() => $_ensure(4);

  @$pb.TagNumber(6)
  PayAllSui get payAllSui => $_getN(5);
  @$pb.TagNumber(6)
  set payAllSui(PayAllSui v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasPayAllSui() => $_has(5);
  @$pb.TagNumber(6)
  void clearPayAllSui() => clearField(6);
  @$pb.TagNumber(6)
  PayAllSui ensurePayAllSui() => $_ensure(5);

  @$pb.TagNumber(7)
  RequestAddStake get requestAddStake => $_getN(6);
  @$pb.TagNumber(7)
  set requestAddStake(RequestAddStake v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasRequestAddStake() => $_has(6);
  @$pb.TagNumber(7)
  void clearRequestAddStake() => clearField(7);
  @$pb.TagNumber(7)
  RequestAddStake ensureRequestAddStake() => $_ensure(6);

  @$pb.TagNumber(8)
  RequestWithdrawStake get requestWithdrawStake => $_getN(7);
  @$pb.TagNumber(8)
  set requestWithdrawStake(RequestWithdrawStake v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasRequestWithdrawStake() => $_has(7);
  @$pb.TagNumber(8)
  void clearRequestWithdrawStake() => clearField(8);
  @$pb.TagNumber(8)
  RequestWithdrawStake ensureRequestWithdrawStake() => $_ensure(7);

  @$pb.TagNumber(9)
  TransferObject get transferObject => $_getN(8);
  @$pb.TagNumber(9)
  set transferObject(TransferObject v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasTransferObject() => $_has(8);
  @$pb.TagNumber(9)
  void clearTransferObject() => clearField(9);
  @$pb.TagNumber(9)
  TransferObject ensureTransferObject() => $_ensure(8);

  /// The gas budget, the transaction will fail if the gas cost exceed the budget.
  @$pb.TagNumber(12)
  $fixnum.Int64 get gasBudget => $_getI64(9);
  @$pb.TagNumber(12)
  set gasBudget($fixnum.Int64 v) { $_setInt64(9, v); }
  @$pb.TagNumber(12)
  $core.bool hasGasBudget() => $_has(9);
  @$pb.TagNumber(12)
  void clearGasBudget() => clearField(12);

  /// Reference gas price.
  @$pb.TagNumber(13)
  $fixnum.Int64 get referenceGasPrice => $_getI64(10);
  @$pb.TagNumber(13)
  set referenceGasPrice($fixnum.Int64 v) { $_setInt64(10, v); }
  @$pb.TagNumber(13)
  $core.bool hasReferenceGasPrice() => $_has(10);
  @$pb.TagNumber(13)
  void clearReferenceGasPrice() => clearField(13);
}

/// Transaction signing output.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.String? unsignedTx,
    $core.String? signature,
    $0.SigningError? error,
    $core.String? errorMessage,
  }) {
    final $result = create();
    if (unsignedTx != null) {
      $result.unsignedTx = unsignedTx;
    }
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

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Sui.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'unsignedTx')
    ..aOS(2, _omitFieldNames ? '' : 'signature')
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

  /// / The raw transaction without indent in base64
  @$pb.TagNumber(1)
  $core.String get unsignedTx => $_getSZ(0);
  @$pb.TagNumber(1)
  set unsignedTx($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasUnsignedTx() => $_has(0);
  @$pb.TagNumber(1)
  void clearUnsignedTx() => clearField(1);

  /// / The signature encoded in base64
  @$pb.TagNumber(2)
  $core.String get signature => $_getSZ(1);
  @$pb.TagNumber(2)
  set signature($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasSignature() => $_has(1);
  @$pb.TagNumber(2)
  void clearSignature() => clearField(2);

  /// Error code, 0 is ok, other codes will be treated as errors.
  @$pb.TagNumber(3)
  $0.SigningError get error => $_getN(2);
  @$pb.TagNumber(3)
  set error($0.SigningError v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasError() => $_has(2);
  @$pb.TagNumber(3)
  void clearError() => clearField(3);

  /// Error description.
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
