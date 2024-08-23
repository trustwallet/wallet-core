//
//  Generated code. Do not modify.
//  source: IOST.proto
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
import 'IOST.pbenum.dart';

export 'IOST.pbenum.dart';

/// The message defines transaction action struct.
class Action extends $pb.GeneratedMessage {
  factory Action({
    $core.String? contract,
    $core.String? actionName,
    $core.String? data,
  }) {
    final $result = create();
    if (contract != null) {
      $result.contract = contract;
    }
    if (actionName != null) {
      $result.actionName = actionName;
    }
    if (data != null) {
      $result.data = data;
    }
    return $result;
  }
  Action._() : super();
  factory Action.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Action.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Action', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IOST.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'contract')
    ..aOS(2, _omitFieldNames ? '' : 'actionName')
    ..aOS(3, _omitFieldNames ? '' : 'data')
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

  /// contract name
  @$pb.TagNumber(1)
  $core.String get contract => $_getSZ(0);
  @$pb.TagNumber(1)
  set contract($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasContract() => $_has(0);
  @$pb.TagNumber(1)
  void clearContract() => clearField(1);

  /// action name
  @$pb.TagNumber(2)
  $core.String get actionName => $_getSZ(1);
  @$pb.TagNumber(2)
  set actionName($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasActionName() => $_has(1);
  @$pb.TagNumber(2)
  void clearActionName() => clearField(2);

  /// data
  @$pb.TagNumber(3)
  $core.String get data => $_getSZ(2);
  @$pb.TagNumber(3)
  set data($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasData() => $_has(2);
  @$pb.TagNumber(3)
  void clearData() => clearField(3);
}

/// The message defines transaction amount limit struct.
class AmountLimit extends $pb.GeneratedMessage {
  factory AmountLimit({
    $core.String? token,
    $core.String? value,
  }) {
    final $result = create();
    if (token != null) {
      $result.token = token;
    }
    if (value != null) {
      $result.value = value;
    }
    return $result;
  }
  AmountLimit._() : super();
  factory AmountLimit.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AmountLimit.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'AmountLimit', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IOST.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'token')
    ..aOS(2, _omitFieldNames ? '' : 'value')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  AmountLimit clone() => AmountLimit()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  AmountLimit copyWith(void Function(AmountLimit) updates) => super.copyWith((message) => updates(message as AmountLimit)) as AmountLimit;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static AmountLimit create() => AmountLimit._();
  AmountLimit createEmptyInstance() => create();
  static $pb.PbList<AmountLimit> createRepeated() => $pb.PbList<AmountLimit>();
  @$core.pragma('dart2js:noInline')
  static AmountLimit getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AmountLimit>(create);
  static AmountLimit? _defaultInstance;

  /// token name
  @$pb.TagNumber(1)
  $core.String get token => $_getSZ(0);
  @$pb.TagNumber(1)
  set token($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasToken() => $_has(0);
  @$pb.TagNumber(1)
  void clearToken() => clearField(1);

  /// limit value
  @$pb.TagNumber(2)
  $core.String get value => $_getSZ(1);
  @$pb.TagNumber(2)
  set value($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValue() => $_has(1);
  @$pb.TagNumber(2)
  void clearValue() => clearField(2);
}

/// The message defines signature struct.
class Signature extends $pb.GeneratedMessage {
  factory Signature({
    Algorithm? algorithm,
    $core.List<$core.int>? signature,
    $core.List<$core.int>? publicKey,
  }) {
    final $result = create();
    if (algorithm != null) {
      $result.algorithm = algorithm;
    }
    if (signature != null) {
      $result.signature = signature;
    }
    if (publicKey != null) {
      $result.publicKey = publicKey;
    }
    return $result;
  }
  Signature._() : super();
  factory Signature.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Signature.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Signature', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IOST.Proto'), createEmptyInstance: create)
    ..e<Algorithm>(1, _omitFieldNames ? '' : 'algorithm', $pb.PbFieldType.OE, defaultOrMaker: Algorithm.UNKNOWN, valueOf: Algorithm.valueOf, enumValues: Algorithm.values)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'signature', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'publicKey', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Signature clone() => Signature()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Signature copyWith(void Function(Signature) updates) => super.copyWith((message) => updates(message as Signature)) as Signature;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Signature create() => Signature._();
  Signature createEmptyInstance() => create();
  static $pb.PbList<Signature> createRepeated() => $pb.PbList<Signature>();
  @$core.pragma('dart2js:noInline')
  static Signature getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Signature>(create);
  static Signature? _defaultInstance;

  /// signature algorithm
  @$pb.TagNumber(1)
  Algorithm get algorithm => $_getN(0);
  @$pb.TagNumber(1)
  set algorithm(Algorithm v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasAlgorithm() => $_has(0);
  @$pb.TagNumber(1)
  void clearAlgorithm() => clearField(1);

  /// signature bytes
  @$pb.TagNumber(2)
  $core.List<$core.int> get signature => $_getN(1);
  @$pb.TagNumber(2)
  set signature($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasSignature() => $_has(1);
  @$pb.TagNumber(2)
  void clearSignature() => clearField(2);

  /// public key
  @$pb.TagNumber(3)
  $core.List<$core.int> get publicKey => $_getN(2);
  @$pb.TagNumber(3)
  set publicKey($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasPublicKey() => $_has(2);
  @$pb.TagNumber(3)
  void clearPublicKey() => clearField(3);
}

/// The message defines the transaction request.
class Transaction extends $pb.GeneratedMessage {
  factory Transaction({
    $fixnum.Int64? time,
    $fixnum.Int64? expiration,
    $core.double? gasRatio,
    $core.double? gasLimit,
    $fixnum.Int64? delay,
    $core.int? chainId,
    $core.Iterable<Action>? actions,
    $core.Iterable<AmountLimit>? amountLimit,
    $core.Iterable<$core.String>? signers,
    $core.Iterable<Signature>? signatures,
    $core.String? publisher,
    $core.Iterable<Signature>? publisherSigs,
  }) {
    final $result = create();
    if (time != null) {
      $result.time = time;
    }
    if (expiration != null) {
      $result.expiration = expiration;
    }
    if (gasRatio != null) {
      $result.gasRatio = gasRatio;
    }
    if (gasLimit != null) {
      $result.gasLimit = gasLimit;
    }
    if (delay != null) {
      $result.delay = delay;
    }
    if (chainId != null) {
      $result.chainId = chainId;
    }
    if (actions != null) {
      $result.actions.addAll(actions);
    }
    if (amountLimit != null) {
      $result.amountLimit.addAll(amountLimit);
    }
    if (signers != null) {
      $result.signers.addAll(signers);
    }
    if (signatures != null) {
      $result.signatures.addAll(signatures);
    }
    if (publisher != null) {
      $result.publisher = publisher;
    }
    if (publisherSigs != null) {
      $result.publisherSigs.addAll(publisherSigs);
    }
    return $result;
  }
  Transaction._() : super();
  factory Transaction.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IOST.Proto'), createEmptyInstance: create)
    ..aInt64(1, _omitFieldNames ? '' : 'time')
    ..aInt64(2, _omitFieldNames ? '' : 'expiration')
    ..a<$core.double>(3, _omitFieldNames ? '' : 'gasRatio', $pb.PbFieldType.OD)
    ..a<$core.double>(4, _omitFieldNames ? '' : 'gasLimit', $pb.PbFieldType.OD)
    ..aInt64(5, _omitFieldNames ? '' : 'delay')
    ..a<$core.int>(6, _omitFieldNames ? '' : 'chainId', $pb.PbFieldType.OU3)
    ..pc<Action>(7, _omitFieldNames ? '' : 'actions', $pb.PbFieldType.PM, subBuilder: Action.create)
    ..pc<AmountLimit>(8, _omitFieldNames ? '' : 'amountLimit', $pb.PbFieldType.PM, subBuilder: AmountLimit.create)
    ..pPS(9, _omitFieldNames ? '' : 'signers')
    ..pc<Signature>(10, _omitFieldNames ? '' : 'signatures', $pb.PbFieldType.PM, subBuilder: Signature.create)
    ..aOS(11, _omitFieldNames ? '' : 'publisher')
    ..pc<Signature>(12, _omitFieldNames ? '' : 'publisherSigs', $pb.PbFieldType.PM, subBuilder: Signature.create)
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

  /// transaction timestamp
  @$pb.TagNumber(1)
  $fixnum.Int64 get time => $_getI64(0);
  @$pb.TagNumber(1)
  set time($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasTime() => $_has(0);
  @$pb.TagNumber(1)
  void clearTime() => clearField(1);

  /// expiration timestamp
  @$pb.TagNumber(2)
  $fixnum.Int64 get expiration => $_getI64(1);
  @$pb.TagNumber(2)
  set expiration($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasExpiration() => $_has(1);
  @$pb.TagNumber(2)
  void clearExpiration() => clearField(2);

  /// gas price
  @$pb.TagNumber(3)
  $core.double get gasRatio => $_getN(2);
  @$pb.TagNumber(3)
  set gasRatio($core.double v) { $_setDouble(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasGasRatio() => $_has(2);
  @$pb.TagNumber(3)
  void clearGasRatio() => clearField(3);

  /// gas limit
  @$pb.TagNumber(4)
  $core.double get gasLimit => $_getN(3);
  @$pb.TagNumber(4)
  set gasLimit($core.double v) { $_setDouble(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasGasLimit() => $_has(3);
  @$pb.TagNumber(4)
  void clearGasLimit() => clearField(4);

  /// delay nanoseconds
  @$pb.TagNumber(5)
  $fixnum.Int64 get delay => $_getI64(4);
  @$pb.TagNumber(5)
  set delay($fixnum.Int64 v) { $_setInt64(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasDelay() => $_has(4);
  @$pb.TagNumber(5)
  void clearDelay() => clearField(5);

  /// chain id
  @$pb.TagNumber(6)
  $core.int get chainId => $_getIZ(5);
  @$pb.TagNumber(6)
  set chainId($core.int v) { $_setUnsignedInt32(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasChainId() => $_has(5);
  @$pb.TagNumber(6)
  void clearChainId() => clearField(6);

  /// action list
  @$pb.TagNumber(7)
  $core.List<Action> get actions => $_getList(6);

  /// amount limit
  @$pb.TagNumber(8)
  $core.List<AmountLimit> get amountLimit => $_getList(7);

  /// signer list
  @$pb.TagNumber(9)
  $core.List<$core.String> get signers => $_getList(8);

  /// signatures of signers
  @$pb.TagNumber(10)
  $core.List<Signature> get signatures => $_getList(9);

  /// publisher
  @$pb.TagNumber(11)
  $core.String get publisher => $_getSZ(10);
  @$pb.TagNumber(11)
  set publisher($core.String v) { $_setString(10, v); }
  @$pb.TagNumber(11)
  $core.bool hasPublisher() => $_has(10);
  @$pb.TagNumber(11)
  void clearPublisher() => clearField(11);

  /// signatures of publisher
  @$pb.TagNumber(12)
  $core.List<Signature> get publisherSigs => $_getList(11);
}

class AccountInfo extends $pb.GeneratedMessage {
  factory AccountInfo({
    $core.String? name,
    $core.List<$core.int>? activeKey,
    $core.List<$core.int>? ownerKey,
  }) {
    final $result = create();
    if (name != null) {
      $result.name = name;
    }
    if (activeKey != null) {
      $result.activeKey = activeKey;
    }
    if (ownerKey != null) {
      $result.ownerKey = ownerKey;
    }
    return $result;
  }
  AccountInfo._() : super();
  factory AccountInfo.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AccountInfo.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'AccountInfo', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IOST.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'name')
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'activeKey', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'ownerKey', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  AccountInfo clone() => AccountInfo()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  AccountInfo copyWith(void Function(AccountInfo) updates) => super.copyWith((message) => updates(message as AccountInfo)) as AccountInfo;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static AccountInfo create() => AccountInfo._();
  AccountInfo createEmptyInstance() => create();
  static $pb.PbList<AccountInfo> createRepeated() => $pb.PbList<AccountInfo>();
  @$core.pragma('dart2js:noInline')
  static AccountInfo getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AccountInfo>(create);
  static AccountInfo? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get name => $_getSZ(0);
  @$pb.TagNumber(1)
  set name($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasName() => $_has(0);
  @$pb.TagNumber(1)
  void clearName() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<$core.int> get activeKey => $_getN(1);
  @$pb.TagNumber(2)
  set activeKey($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasActiveKey() => $_has(1);
  @$pb.TagNumber(2)
  void clearActiveKey() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<$core.int> get ownerKey => $_getN(2);
  @$pb.TagNumber(3)
  set ownerKey($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasOwnerKey() => $_has(2);
  @$pb.TagNumber(3)
  void clearOwnerKey() => clearField(3);
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    AccountInfo? account,
    Transaction? transactionTemplate,
    $core.String? transferDestination,
    $core.String? transferAmount,
    $core.String? transferMemo,
  }) {
    final $result = create();
    if (account != null) {
      $result.account = account;
    }
    if (transactionTemplate != null) {
      $result.transactionTemplate = transactionTemplate;
    }
    if (transferDestination != null) {
      $result.transferDestination = transferDestination;
    }
    if (transferAmount != null) {
      $result.transferAmount = transferAmount;
    }
    if (transferMemo != null) {
      $result.transferMemo = transferMemo;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IOST.Proto'), createEmptyInstance: create)
    ..aOM<AccountInfo>(1, _omitFieldNames ? '' : 'account', subBuilder: AccountInfo.create)
    ..aOM<Transaction>(2, _omitFieldNames ? '' : 'transactionTemplate', subBuilder: Transaction.create)
    ..aOS(3, _omitFieldNames ? '' : 'transferDestination')
    ..aOS(4, _omitFieldNames ? '' : 'transferAmount')
    ..aOS(5, _omitFieldNames ? '' : 'transferMemo')
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

  @$pb.TagNumber(1)
  AccountInfo get account => $_getN(0);
  @$pb.TagNumber(1)
  set account(AccountInfo v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasAccount() => $_has(0);
  @$pb.TagNumber(1)
  void clearAccount() => clearField(1);
  @$pb.TagNumber(1)
  AccountInfo ensureAccount() => $_ensure(0);

  @$pb.TagNumber(2)
  Transaction get transactionTemplate => $_getN(1);
  @$pb.TagNumber(2)
  set transactionTemplate(Transaction v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasTransactionTemplate() => $_has(1);
  @$pb.TagNumber(2)
  void clearTransactionTemplate() => clearField(2);
  @$pb.TagNumber(2)
  Transaction ensureTransactionTemplate() => $_ensure(1);

  @$pb.TagNumber(3)
  $core.String get transferDestination => $_getSZ(2);
  @$pb.TagNumber(3)
  set transferDestination($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTransferDestination() => $_has(2);
  @$pb.TagNumber(3)
  void clearTransferDestination() => clearField(3);

  @$pb.TagNumber(4)
  $core.String get transferAmount => $_getSZ(3);
  @$pb.TagNumber(4)
  set transferAmount($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasTransferAmount() => $_has(3);
  @$pb.TagNumber(4)
  void clearTransferAmount() => clearField(4);

  @$pb.TagNumber(5)
  $core.String get transferMemo => $_getSZ(4);
  @$pb.TagNumber(5)
  set transferMemo($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasTransferMemo() => $_has(4);
  @$pb.TagNumber(5)
  void clearTransferMemo() => clearField(5);
}

/// Transaction signing output.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    Transaction? transaction,
    $core.List<$core.int>? encoded,
    $0.SigningError? error,
    $core.String? errorMessage,
  }) {
    final $result = create();
    if (transaction != null) {
      $result.transaction = transaction;
    }
    if (encoded != null) {
      $result.encoded = encoded;
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

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IOST.Proto'), createEmptyInstance: create)
    ..aOM<Transaction>(1, _omitFieldNames ? '' : 'transaction', subBuilder: Transaction.create)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'encoded', $pb.PbFieldType.OY)
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

  /// Signed transaction
  @$pb.TagNumber(1)
  Transaction get transaction => $_getN(0);
  @$pb.TagNumber(1)
  set transaction(Transaction v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasTransaction() => $_has(0);
  @$pb.TagNumber(1)
  void clearTransaction() => clearField(1);
  @$pb.TagNumber(1)
  Transaction ensureTransaction() => $_ensure(0);

  /// Signed and encoded transaction bytes.
  @$pb.TagNumber(2)
  $core.List<$core.int> get encoded => $_getN(1);
  @$pb.TagNumber(2)
  set encoded($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasEncoded() => $_has(1);
  @$pb.TagNumber(2)
  void clearEncoded() => clearField(2);

  /// error code, 0 is ok, other codes will be treated as errors
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
