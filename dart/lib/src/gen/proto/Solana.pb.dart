//
//  Generated code. Do not modify.
//  source: Solana.proto
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
import 'Solana.pbenum.dart';

export 'Solana.pbenum.dart';

/// Transfer transaction
class Transfer extends $pb.GeneratedMessage {
  factory Transfer({
    $core.String? recipient,
    $fixnum.Int64? value,
    $core.String? memo,
    $core.Iterable<$core.String>? references,
  }) {
    final $result = create();
    if (recipient != null) {
      $result.recipient = recipient;
    }
    if (value != null) {
      $result.value = value;
    }
    if (memo != null) {
      $result.memo = memo;
    }
    if (references != null) {
      $result.references.addAll(references);
    }
    return $result;
  }
  Transfer._() : super();
  factory Transfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'recipient')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(3, _omitFieldNames ? '' : 'memo')
    ..pPS(4, _omitFieldNames ? '' : 'references')
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

  /// destination address
  @$pb.TagNumber(1)
  $core.String get recipient => $_getSZ(0);
  @$pb.TagNumber(1)
  set recipient($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasRecipient() => $_has(0);
  @$pb.TagNumber(1)
  void clearRecipient() => clearField(1);

  /// amount
  @$pb.TagNumber(2)
  $fixnum.Int64 get value => $_getI64(1);
  @$pb.TagNumber(2)
  set value($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValue() => $_has(1);
  @$pb.TagNumber(2)
  void clearValue() => clearField(2);

  /// optional memo
  @$pb.TagNumber(3)
  $core.String get memo => $_getSZ(2);
  @$pb.TagNumber(3)
  set memo($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasMemo() => $_has(2);
  @$pb.TagNumber(3)
  void clearMemo() => clearField(3);

  /// optional referenced public keys
  @$pb.TagNumber(4)
  $core.List<$core.String> get references => $_getList(3);
}

/// Create and initialize a stake account, and delegate amount to it.
/// Recommendation behavior is to not specify a stake account, and a new unique account will be created each time.
/// Optionally a stake account pubkey can be specified, but it should not exist on chain.
class DelegateStake extends $pb.GeneratedMessage {
  factory DelegateStake({
    $core.String? validatorPubkey,
    $fixnum.Int64? value,
    $core.String? stakeAccount,
  }) {
    final $result = create();
    if (validatorPubkey != null) {
      $result.validatorPubkey = validatorPubkey;
    }
    if (value != null) {
      $result.value = value;
    }
    if (stakeAccount != null) {
      $result.stakeAccount = stakeAccount;
    }
    return $result;
  }
  DelegateStake._() : super();
  factory DelegateStake.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DelegateStake.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DelegateStake', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'validatorPubkey')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(3, _omitFieldNames ? '' : 'stakeAccount')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DelegateStake clone() => DelegateStake()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DelegateStake copyWith(void Function(DelegateStake) updates) => super.copyWith((message) => updates(message as DelegateStake)) as DelegateStake;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DelegateStake create() => DelegateStake._();
  DelegateStake createEmptyInstance() => create();
  static $pb.PbList<DelegateStake> createRepeated() => $pb.PbList<DelegateStake>();
  @$core.pragma('dart2js:noInline')
  static DelegateStake getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DelegateStake>(create);
  static DelegateStake? _defaultInstance;

  /// Validator's public key
  @$pb.TagNumber(1)
  $core.String get validatorPubkey => $_getSZ(0);
  @$pb.TagNumber(1)
  set validatorPubkey($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasValidatorPubkey() => $_has(0);
  @$pb.TagNumber(1)
  void clearValidatorPubkey() => clearField(1);

  /// delegation amount
  @$pb.TagNumber(2)
  $fixnum.Int64 get value => $_getI64(1);
  @$pb.TagNumber(2)
  set value($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValue() => $_has(1);
  @$pb.TagNumber(2)
  void clearValue() => clearField(2);

  /// staking account
  @$pb.TagNumber(3)
  $core.String get stakeAccount => $_getSZ(2);
  @$pb.TagNumber(3)
  set stakeAccount($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasStakeAccount() => $_has(2);
  @$pb.TagNumber(3)
  void clearStakeAccount() => clearField(3);
}

/// Deactivate staking on stake account
class DeactivateStake extends $pb.GeneratedMessage {
  factory DeactivateStake({
    $core.String? stakeAccount,
  }) {
    final $result = create();
    if (stakeAccount != null) {
      $result.stakeAccount = stakeAccount;
    }
    return $result;
  }
  DeactivateStake._() : super();
  factory DeactivateStake.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeactivateStake.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DeactivateStake', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'stakeAccount')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DeactivateStake clone() => DeactivateStake()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DeactivateStake copyWith(void Function(DeactivateStake) updates) => super.copyWith((message) => updates(message as DeactivateStake)) as DeactivateStake;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DeactivateStake create() => DeactivateStake._();
  DeactivateStake createEmptyInstance() => create();
  static $pb.PbList<DeactivateStake> createRepeated() => $pb.PbList<DeactivateStake>();
  @$core.pragma('dart2js:noInline')
  static DeactivateStake getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeactivateStake>(create);
  static DeactivateStake? _defaultInstance;

  /// staking account
  @$pb.TagNumber(1)
  $core.String get stakeAccount => $_getSZ(0);
  @$pb.TagNumber(1)
  set stakeAccount($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasStakeAccount() => $_has(0);
  @$pb.TagNumber(1)
  void clearStakeAccount() => clearField(1);
}

/// Deactivate staking on multiple stake account
class DeactivateAllStake extends $pb.GeneratedMessage {
  factory DeactivateAllStake({
    $core.Iterable<$core.String>? stakeAccounts,
  }) {
    final $result = create();
    if (stakeAccounts != null) {
      $result.stakeAccounts.addAll(stakeAccounts);
    }
    return $result;
  }
  DeactivateAllStake._() : super();
  factory DeactivateAllStake.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeactivateAllStake.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DeactivateAllStake', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..pPS(1, _omitFieldNames ? '' : 'stakeAccounts')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DeactivateAllStake clone() => DeactivateAllStake()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DeactivateAllStake copyWith(void Function(DeactivateAllStake) updates) => super.copyWith((message) => updates(message as DeactivateAllStake)) as DeactivateAllStake;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DeactivateAllStake create() => DeactivateAllStake._();
  DeactivateAllStake createEmptyInstance() => create();
  static $pb.PbList<DeactivateAllStake> createRepeated() => $pb.PbList<DeactivateAllStake>();
  @$core.pragma('dart2js:noInline')
  static DeactivateAllStake getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeactivateAllStake>(create);
  static DeactivateAllStake? _defaultInstance;

  /// staking accounts
  @$pb.TagNumber(1)
  $core.List<$core.String> get stakeAccounts => $_getList(0);
}

/// Withdraw amount from stake account
class WithdrawStake extends $pb.GeneratedMessage {
  factory WithdrawStake({
    $core.String? stakeAccount,
    $fixnum.Int64? value,
  }) {
    final $result = create();
    if (stakeAccount != null) {
      $result.stakeAccount = stakeAccount;
    }
    if (value != null) {
      $result.value = value;
    }
    return $result;
  }
  WithdrawStake._() : super();
  factory WithdrawStake.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory WithdrawStake.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'WithdrawStake', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'stakeAccount')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  WithdrawStake clone() => WithdrawStake()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  WithdrawStake copyWith(void Function(WithdrawStake) updates) => super.copyWith((message) => updates(message as WithdrawStake)) as WithdrawStake;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static WithdrawStake create() => WithdrawStake._();
  WithdrawStake createEmptyInstance() => create();
  static $pb.PbList<WithdrawStake> createRepeated() => $pb.PbList<WithdrawStake>();
  @$core.pragma('dart2js:noInline')
  static WithdrawStake getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<WithdrawStake>(create);
  static WithdrawStake? _defaultInstance;

  /// staking account
  @$pb.TagNumber(1)
  $core.String get stakeAccount => $_getSZ(0);
  @$pb.TagNumber(1)
  set stakeAccount($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasStakeAccount() => $_has(0);
  @$pb.TagNumber(1)
  void clearStakeAccount() => clearField(1);

  /// withdrawal amount
  @$pb.TagNumber(2)
  $fixnum.Int64 get value => $_getI64(1);
  @$pb.TagNumber(2)
  set value($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValue() => $_has(1);
  @$pb.TagNumber(2)
  void clearValue() => clearField(2);
}

/// Technical structure to group a staking account and an amount
class StakeAccountValue extends $pb.GeneratedMessage {
  factory StakeAccountValue({
    $core.String? stakeAccount,
    $fixnum.Int64? value,
  }) {
    final $result = create();
    if (stakeAccount != null) {
      $result.stakeAccount = stakeAccount;
    }
    if (value != null) {
      $result.value = value;
    }
    return $result;
  }
  StakeAccountValue._() : super();
  factory StakeAccountValue.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory StakeAccountValue.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'StakeAccountValue', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'stakeAccount')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  StakeAccountValue clone() => StakeAccountValue()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  StakeAccountValue copyWith(void Function(StakeAccountValue) updates) => super.copyWith((message) => updates(message as StakeAccountValue)) as StakeAccountValue;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static StakeAccountValue create() => StakeAccountValue._();
  StakeAccountValue createEmptyInstance() => create();
  static $pb.PbList<StakeAccountValue> createRepeated() => $pb.PbList<StakeAccountValue>();
  @$core.pragma('dart2js:noInline')
  static StakeAccountValue getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<StakeAccountValue>(create);
  static StakeAccountValue? _defaultInstance;

  /// staking account
  @$pb.TagNumber(1)
  $core.String get stakeAccount => $_getSZ(0);
  @$pb.TagNumber(1)
  set stakeAccount($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasStakeAccount() => $_has(0);
  @$pb.TagNumber(1)
  void clearStakeAccount() => clearField(1);

  /// amount
  @$pb.TagNumber(2)
  $fixnum.Int64 get value => $_getI64(1);
  @$pb.TagNumber(2)
  set value($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValue() => $_has(1);
  @$pb.TagNumber(2)
  void clearValue() => clearField(2);
}

/// Withdraw amounts from stake accounts
class WithdrawAllStake extends $pb.GeneratedMessage {
  factory WithdrawAllStake({
    $core.Iterable<StakeAccountValue>? stakeAccounts,
  }) {
    final $result = create();
    if (stakeAccounts != null) {
      $result.stakeAccounts.addAll(stakeAccounts);
    }
    return $result;
  }
  WithdrawAllStake._() : super();
  factory WithdrawAllStake.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory WithdrawAllStake.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'WithdrawAllStake', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..pc<StakeAccountValue>(1, _omitFieldNames ? '' : 'stakeAccounts', $pb.PbFieldType.PM, subBuilder: StakeAccountValue.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  WithdrawAllStake clone() => WithdrawAllStake()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  WithdrawAllStake copyWith(void Function(WithdrawAllStake) updates) => super.copyWith((message) => updates(message as WithdrawAllStake)) as WithdrawAllStake;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static WithdrawAllStake create() => WithdrawAllStake._();
  WithdrawAllStake createEmptyInstance() => create();
  static $pb.PbList<WithdrawAllStake> createRepeated() => $pb.PbList<WithdrawAllStake>();
  @$core.pragma('dart2js:noInline')
  static WithdrawAllStake getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<WithdrawAllStake>(create);
  static WithdrawAllStake? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<StakeAccountValue> get stakeAccounts => $_getList(0);
}

/// Create a token account under a main account for a token type
class CreateTokenAccount extends $pb.GeneratedMessage {
  factory CreateTokenAccount({
    $core.String? mainAddress,
    $core.String? tokenMintAddress,
    $core.String? tokenAddress,
  }) {
    final $result = create();
    if (mainAddress != null) {
      $result.mainAddress = mainAddress;
    }
    if (tokenMintAddress != null) {
      $result.tokenMintAddress = tokenMintAddress;
    }
    if (tokenAddress != null) {
      $result.tokenAddress = tokenAddress;
    }
    return $result;
  }
  CreateTokenAccount._() : super();
  factory CreateTokenAccount.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory CreateTokenAccount.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'CreateTokenAccount', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'mainAddress')
    ..aOS(2, _omitFieldNames ? '' : 'tokenMintAddress')
    ..aOS(3, _omitFieldNames ? '' : 'tokenAddress')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  CreateTokenAccount clone() => CreateTokenAccount()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  CreateTokenAccount copyWith(void Function(CreateTokenAccount) updates) => super.copyWith((message) => updates(message as CreateTokenAccount)) as CreateTokenAccount;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static CreateTokenAccount create() => CreateTokenAccount._();
  CreateTokenAccount createEmptyInstance() => create();
  static $pb.PbList<CreateTokenAccount> createRepeated() => $pb.PbList<CreateTokenAccount>();
  @$core.pragma('dart2js:noInline')
  static CreateTokenAccount getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<CreateTokenAccount>(create);
  static CreateTokenAccount? _defaultInstance;

  /// main account -- can be same as signer, or other main account (if done on some other account's behalf)
  @$pb.TagNumber(1)
  $core.String get mainAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set mainAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasMainAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearMainAddress() => clearField(1);

  /// Token minting address
  @$pb.TagNumber(2)
  $core.String get tokenMintAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set tokenMintAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTokenMintAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearTokenMintAddress() => clearField(2);

  /// Token address
  @$pb.TagNumber(3)
  $core.String get tokenAddress => $_getSZ(2);
  @$pb.TagNumber(3)
  set tokenAddress($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTokenAddress() => $_has(2);
  @$pb.TagNumber(3)
  void clearTokenAddress() => clearField(3);
}

/// Transfer tokens
class TokenTransfer extends $pb.GeneratedMessage {
  factory TokenTransfer({
    $core.String? tokenMintAddress,
    $core.String? senderTokenAddress,
    $core.String? recipientTokenAddress,
    $fixnum.Int64? amount,
    $core.int? decimals,
    $core.String? memo,
    $core.Iterable<$core.String>? references,
  }) {
    final $result = create();
    if (tokenMintAddress != null) {
      $result.tokenMintAddress = tokenMintAddress;
    }
    if (senderTokenAddress != null) {
      $result.senderTokenAddress = senderTokenAddress;
    }
    if (recipientTokenAddress != null) {
      $result.recipientTokenAddress = recipientTokenAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (decimals != null) {
      $result.decimals = decimals;
    }
    if (memo != null) {
      $result.memo = memo;
    }
    if (references != null) {
      $result.references.addAll(references);
    }
    return $result;
  }
  TokenTransfer._() : super();
  factory TokenTransfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TokenTransfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TokenTransfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'tokenMintAddress')
    ..aOS(2, _omitFieldNames ? '' : 'senderTokenAddress')
    ..aOS(3, _omitFieldNames ? '' : 'recipientTokenAddress')
    ..a<$fixnum.Int64>(4, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.int>(5, _omitFieldNames ? '' : 'decimals', $pb.PbFieldType.OU3)
    ..aOS(6, _omitFieldNames ? '' : 'memo')
    ..pPS(7, _omitFieldNames ? '' : 'references')
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

  /// Mint address of the token
  @$pb.TagNumber(1)
  $core.String get tokenMintAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set tokenMintAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasTokenMintAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearTokenMintAddress() => clearField(1);

  /// Source address
  @$pb.TagNumber(2)
  $core.String get senderTokenAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set senderTokenAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasSenderTokenAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearSenderTokenAddress() => clearField(2);

  /// Destination address
  @$pb.TagNumber(3)
  $core.String get recipientTokenAddress => $_getSZ(2);
  @$pb.TagNumber(3)
  set recipientTokenAddress($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasRecipientTokenAddress() => $_has(2);
  @$pb.TagNumber(3)
  void clearRecipientTokenAddress() => clearField(3);

  /// Amount
  @$pb.TagNumber(4)
  $fixnum.Int64 get amount => $_getI64(3);
  @$pb.TagNumber(4)
  set amount($fixnum.Int64 v) { $_setInt64(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasAmount() => $_has(3);
  @$pb.TagNumber(4)
  void clearAmount() => clearField(4);

  /// Note: 8-bit value
  @$pb.TagNumber(5)
  $core.int get decimals => $_getIZ(4);
  @$pb.TagNumber(5)
  set decimals($core.int v) { $_setUnsignedInt32(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasDecimals() => $_has(4);
  @$pb.TagNumber(5)
  void clearDecimals() => clearField(5);

  /// optional memo§
  @$pb.TagNumber(6)
  $core.String get memo => $_getSZ(5);
  @$pb.TagNumber(6)
  set memo($core.String v) { $_setString(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasMemo() => $_has(5);
  @$pb.TagNumber(6)
  void clearMemo() => clearField(6);

  /// optional referenced public keys
  @$pb.TagNumber(7)
  $core.List<$core.String> get references => $_getList(6);
}

/// CreateTokenAccount and TokenTransfer combined
class CreateAndTransferToken extends $pb.GeneratedMessage {
  factory CreateAndTransferToken({
    $core.String? recipientMainAddress,
    $core.String? tokenMintAddress,
    $core.String? recipientTokenAddress,
    $core.String? senderTokenAddress,
    $fixnum.Int64? amount,
    $core.int? decimals,
    $core.String? memo,
    $core.Iterable<$core.String>? references,
  }) {
    final $result = create();
    if (recipientMainAddress != null) {
      $result.recipientMainAddress = recipientMainAddress;
    }
    if (tokenMintAddress != null) {
      $result.tokenMintAddress = tokenMintAddress;
    }
    if (recipientTokenAddress != null) {
      $result.recipientTokenAddress = recipientTokenAddress;
    }
    if (senderTokenAddress != null) {
      $result.senderTokenAddress = senderTokenAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (decimals != null) {
      $result.decimals = decimals;
    }
    if (memo != null) {
      $result.memo = memo;
    }
    if (references != null) {
      $result.references.addAll(references);
    }
    return $result;
  }
  CreateAndTransferToken._() : super();
  factory CreateAndTransferToken.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory CreateAndTransferToken.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'CreateAndTransferToken', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'recipientMainAddress')
    ..aOS(2, _omitFieldNames ? '' : 'tokenMintAddress')
    ..aOS(3, _omitFieldNames ? '' : 'recipientTokenAddress')
    ..aOS(4, _omitFieldNames ? '' : 'senderTokenAddress')
    ..a<$fixnum.Int64>(5, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.int>(6, _omitFieldNames ? '' : 'decimals', $pb.PbFieldType.OU3)
    ..aOS(7, _omitFieldNames ? '' : 'memo')
    ..pPS(8, _omitFieldNames ? '' : 'references')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  CreateAndTransferToken clone() => CreateAndTransferToken()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  CreateAndTransferToken copyWith(void Function(CreateAndTransferToken) updates) => super.copyWith((message) => updates(message as CreateAndTransferToken)) as CreateAndTransferToken;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static CreateAndTransferToken create() => CreateAndTransferToken._();
  CreateAndTransferToken createEmptyInstance() => create();
  static $pb.PbList<CreateAndTransferToken> createRepeated() => $pb.PbList<CreateAndTransferToken>();
  @$core.pragma('dart2js:noInline')
  static CreateAndTransferToken getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<CreateAndTransferToken>(create);
  static CreateAndTransferToken? _defaultInstance;

  /// main account -- can be same as signer, or other main account (if done on some other account's behalf)
  @$pb.TagNumber(1)
  $core.String get recipientMainAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set recipientMainAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasRecipientMainAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearRecipientMainAddress() => clearField(1);

  /// Mint address of the token
  @$pb.TagNumber(2)
  $core.String get tokenMintAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set tokenMintAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTokenMintAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearTokenMintAddress() => clearField(2);

  /// Token address for the recipient, will be created first
  @$pb.TagNumber(3)
  $core.String get recipientTokenAddress => $_getSZ(2);
  @$pb.TagNumber(3)
  set recipientTokenAddress($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasRecipientTokenAddress() => $_has(2);
  @$pb.TagNumber(3)
  void clearRecipientTokenAddress() => clearField(3);

  /// Sender's token address
  @$pb.TagNumber(4)
  $core.String get senderTokenAddress => $_getSZ(3);
  @$pb.TagNumber(4)
  set senderTokenAddress($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasSenderTokenAddress() => $_has(3);
  @$pb.TagNumber(4)
  void clearSenderTokenAddress() => clearField(4);

  /// amount
  @$pb.TagNumber(5)
  $fixnum.Int64 get amount => $_getI64(4);
  @$pb.TagNumber(5)
  set amount($fixnum.Int64 v) { $_setInt64(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasAmount() => $_has(4);
  @$pb.TagNumber(5)
  void clearAmount() => clearField(5);

  /// Note: 8-bit value
  @$pb.TagNumber(6)
  $core.int get decimals => $_getIZ(5);
  @$pb.TagNumber(6)
  set decimals($core.int v) { $_setUnsignedInt32(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasDecimals() => $_has(5);
  @$pb.TagNumber(6)
  void clearDecimals() => clearField(6);

  /// optional
  @$pb.TagNumber(7)
  $core.String get memo => $_getSZ(6);
  @$pb.TagNumber(7)
  set memo($core.String v) { $_setString(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasMemo() => $_has(6);
  @$pb.TagNumber(7)
  void clearMemo() => clearField(7);

  /// optional referenced public keys
  @$pb.TagNumber(8)
  $core.List<$core.String> get references => $_getList(7);
}

class CreateNonceAccount extends $pb.GeneratedMessage {
  factory CreateNonceAccount({
    $core.String? nonceAccount,
    $fixnum.Int64? rent,
    $core.List<$core.int>? nonceAccountPrivateKey,
  }) {
    final $result = create();
    if (nonceAccount != null) {
      $result.nonceAccount = nonceAccount;
    }
    if (rent != null) {
      $result.rent = rent;
    }
    if (nonceAccountPrivateKey != null) {
      $result.nonceAccountPrivateKey = nonceAccountPrivateKey;
    }
    return $result;
  }
  CreateNonceAccount._() : super();
  factory CreateNonceAccount.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory CreateNonceAccount.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'CreateNonceAccount', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'nonceAccount')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'rent', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'nonceAccountPrivateKey', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  CreateNonceAccount clone() => CreateNonceAccount()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  CreateNonceAccount copyWith(void Function(CreateNonceAccount) updates) => super.copyWith((message) => updates(message as CreateNonceAccount)) as CreateNonceAccount;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static CreateNonceAccount create() => CreateNonceAccount._();
  CreateNonceAccount createEmptyInstance() => create();
  static $pb.PbList<CreateNonceAccount> createRepeated() => $pb.PbList<CreateNonceAccount>();
  @$core.pragma('dart2js:noInline')
  static CreateNonceAccount getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<CreateNonceAccount>(create);
  static CreateNonceAccount? _defaultInstance;

  /// Required for building pre-signing hash of a transaction
  @$pb.TagNumber(1)
  $core.String get nonceAccount => $_getSZ(0);
  @$pb.TagNumber(1)
  set nonceAccount($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasNonceAccount() => $_has(0);
  @$pb.TagNumber(1)
  void clearNonceAccount() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get rent => $_getI64(1);
  @$pb.TagNumber(2)
  set rent($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasRent() => $_has(1);
  @$pb.TagNumber(2)
  void clearRent() => clearField(2);

  /// Optional for building pre-signing hash of a transaction
  @$pb.TagNumber(3)
  $core.List<$core.int> get nonceAccountPrivateKey => $_getN(2);
  @$pb.TagNumber(3)
  set nonceAccountPrivateKey($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasNonceAccountPrivateKey() => $_has(2);
  @$pb.TagNumber(3)
  void clearNonceAccountPrivateKey() => clearField(3);
}

class WithdrawNonceAccount extends $pb.GeneratedMessage {
  factory WithdrawNonceAccount({
    $core.String? nonceAccount,
    $core.String? recipient,
    $fixnum.Int64? value,
  }) {
    final $result = create();
    if (nonceAccount != null) {
      $result.nonceAccount = nonceAccount;
    }
    if (recipient != null) {
      $result.recipient = recipient;
    }
    if (value != null) {
      $result.value = value;
    }
    return $result;
  }
  WithdrawNonceAccount._() : super();
  factory WithdrawNonceAccount.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory WithdrawNonceAccount.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'WithdrawNonceAccount', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'nonceAccount')
    ..aOS(2, _omitFieldNames ? '' : 'recipient')
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  WithdrawNonceAccount clone() => WithdrawNonceAccount()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  WithdrawNonceAccount copyWith(void Function(WithdrawNonceAccount) updates) => super.copyWith((message) => updates(message as WithdrawNonceAccount)) as WithdrawNonceAccount;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static WithdrawNonceAccount create() => WithdrawNonceAccount._();
  WithdrawNonceAccount createEmptyInstance() => create();
  static $pb.PbList<WithdrawNonceAccount> createRepeated() => $pb.PbList<WithdrawNonceAccount>();
  @$core.pragma('dart2js:noInline')
  static WithdrawNonceAccount getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<WithdrawNonceAccount>(create);
  static WithdrawNonceAccount? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get nonceAccount => $_getSZ(0);
  @$pb.TagNumber(1)
  set nonceAccount($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasNonceAccount() => $_has(0);
  @$pb.TagNumber(1)
  void clearNonceAccount() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get recipient => $_getSZ(1);
  @$pb.TagNumber(2)
  set recipient($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasRecipient() => $_has(1);
  @$pb.TagNumber(2)
  void clearRecipient() => clearField(2);

  @$pb.TagNumber(3)
  $fixnum.Int64 get value => $_getI64(2);
  @$pb.TagNumber(3)
  set value($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasValue() => $_has(2);
  @$pb.TagNumber(3)
  void clearValue() => clearField(3);
}

class AdvanceNonceAccount extends $pb.GeneratedMessage {
  factory AdvanceNonceAccount({
    $core.String? nonceAccount,
  }) {
    final $result = create();
    if (nonceAccount != null) {
      $result.nonceAccount = nonceAccount;
    }
    return $result;
  }
  AdvanceNonceAccount._() : super();
  factory AdvanceNonceAccount.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory AdvanceNonceAccount.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'AdvanceNonceAccount', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'nonceAccount')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  AdvanceNonceAccount clone() => AdvanceNonceAccount()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  AdvanceNonceAccount copyWith(void Function(AdvanceNonceAccount) updates) => super.copyWith((message) => updates(message as AdvanceNonceAccount)) as AdvanceNonceAccount;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static AdvanceNonceAccount create() => AdvanceNonceAccount._();
  AdvanceNonceAccount createEmptyInstance() => create();
  static $pb.PbList<AdvanceNonceAccount> createRepeated() => $pb.PbList<AdvanceNonceAccount>();
  @$core.pragma('dart2js:noInline')
  static AdvanceNonceAccount getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<AdvanceNonceAccount>(create);
  static AdvanceNonceAccount? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get nonceAccount => $_getSZ(0);
  @$pb.TagNumber(1)
  set nonceAccount($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasNonceAccount() => $_has(0);
  @$pb.TagNumber(1)
  void clearNonceAccount() => clearField(1);
}

class PubkeySignature extends $pb.GeneratedMessage {
  factory PubkeySignature({
    $core.String? pubkey,
    $core.String? signature,
  }) {
    final $result = create();
    if (pubkey != null) {
      $result.pubkey = pubkey;
    }
    if (signature != null) {
      $result.signature = signature;
    }
    return $result;
  }
  PubkeySignature._() : super();
  factory PubkeySignature.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PubkeySignature.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'PubkeySignature', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'pubkey')
    ..aOS(2, _omitFieldNames ? '' : 'signature')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  PubkeySignature clone() => PubkeySignature()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  PubkeySignature copyWith(void Function(PubkeySignature) updates) => super.copyWith((message) => updates(message as PubkeySignature)) as PubkeySignature;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static PubkeySignature create() => PubkeySignature._();
  PubkeySignature createEmptyInstance() => create();
  static $pb.PbList<PubkeySignature> createRepeated() => $pb.PbList<PubkeySignature>();
  @$core.pragma('dart2js:noInline')
  static PubkeySignature getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PubkeySignature>(create);
  static PubkeySignature? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get pubkey => $_getSZ(0);
  @$pb.TagNumber(1)
  set pubkey($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPubkey() => $_has(0);
  @$pb.TagNumber(1)
  void clearPubkey() => clearField(1);

  /// base58 encoded signature.
  @$pb.TagNumber(2)
  $core.String get signature => $_getSZ(1);
  @$pb.TagNumber(2)
  set signature($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasSignature() => $_has(1);
  @$pb.TagNumber(2)
  void clearSignature() => clearField(2);
}

class RawMessage_MessageHeader extends $pb.GeneratedMessage {
  factory RawMessage_MessageHeader({
    $core.int? numRequiredSignatures,
    $core.int? numReadonlySignedAccounts,
    $core.int? numReadonlyUnsignedAccounts,
  }) {
    final $result = create();
    if (numRequiredSignatures != null) {
      $result.numRequiredSignatures = numRequiredSignatures;
    }
    if (numReadonlySignedAccounts != null) {
      $result.numReadonlySignedAccounts = numReadonlySignedAccounts;
    }
    if (numReadonlyUnsignedAccounts != null) {
      $result.numReadonlyUnsignedAccounts = numReadonlyUnsignedAccounts;
    }
    return $result;
  }
  RawMessage_MessageHeader._() : super();
  factory RawMessage_MessageHeader.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RawMessage_MessageHeader.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'RawMessage.MessageHeader', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'numRequiredSignatures', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'numReadonlySignedAccounts', $pb.PbFieldType.OU3)
    ..a<$core.int>(3, _omitFieldNames ? '' : 'numReadonlyUnsignedAccounts', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  RawMessage_MessageHeader clone() => RawMessage_MessageHeader()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  RawMessage_MessageHeader copyWith(void Function(RawMessage_MessageHeader) updates) => super.copyWith((message) => updates(message as RawMessage_MessageHeader)) as RawMessage_MessageHeader;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static RawMessage_MessageHeader create() => RawMessage_MessageHeader._();
  RawMessage_MessageHeader createEmptyInstance() => create();
  static $pb.PbList<RawMessage_MessageHeader> createRepeated() => $pb.PbList<RawMessage_MessageHeader>();
  @$core.pragma('dart2js:noInline')
  static RawMessage_MessageHeader getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RawMessage_MessageHeader>(create);
  static RawMessage_MessageHeader? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get numRequiredSignatures => $_getIZ(0);
  @$pb.TagNumber(1)
  set numRequiredSignatures($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasNumRequiredSignatures() => $_has(0);
  @$pb.TagNumber(1)
  void clearNumRequiredSignatures() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get numReadonlySignedAccounts => $_getIZ(1);
  @$pb.TagNumber(2)
  set numReadonlySignedAccounts($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasNumReadonlySignedAccounts() => $_has(1);
  @$pb.TagNumber(2)
  void clearNumReadonlySignedAccounts() => clearField(2);

  @$pb.TagNumber(3)
  $core.int get numReadonlyUnsignedAccounts => $_getIZ(2);
  @$pb.TagNumber(3)
  set numReadonlyUnsignedAccounts($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasNumReadonlyUnsignedAccounts() => $_has(2);
  @$pb.TagNumber(3)
  void clearNumReadonlyUnsignedAccounts() => clearField(3);
}

class RawMessage_Instruction extends $pb.GeneratedMessage {
  factory RawMessage_Instruction({
    $core.int? programId,
    $core.Iterable<$core.int>? accounts,
    $core.List<$core.int>? programData,
  }) {
    final $result = create();
    if (programId != null) {
      $result.programId = programId;
    }
    if (accounts != null) {
      $result.accounts.addAll(accounts);
    }
    if (programData != null) {
      $result.programData = programData;
    }
    return $result;
  }
  RawMessage_Instruction._() : super();
  factory RawMessage_Instruction.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RawMessage_Instruction.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'RawMessage.Instruction', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'programId', $pb.PbFieldType.OU3)
    ..p<$core.int>(2, _omitFieldNames ? '' : 'accounts', $pb.PbFieldType.KU3)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'programData', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  RawMessage_Instruction clone() => RawMessage_Instruction()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  RawMessage_Instruction copyWith(void Function(RawMessage_Instruction) updates) => super.copyWith((message) => updates(message as RawMessage_Instruction)) as RawMessage_Instruction;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static RawMessage_Instruction create() => RawMessage_Instruction._();
  RawMessage_Instruction createEmptyInstance() => create();
  static $pb.PbList<RawMessage_Instruction> createRepeated() => $pb.PbList<RawMessage_Instruction>();
  @$core.pragma('dart2js:noInline')
  static RawMessage_Instruction getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RawMessage_Instruction>(create);
  static RawMessage_Instruction? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get programId => $_getIZ(0);
  @$pb.TagNumber(1)
  set programId($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasProgramId() => $_has(0);
  @$pb.TagNumber(1)
  void clearProgramId() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<$core.int> get accounts => $_getList(1);

  @$pb.TagNumber(3)
  $core.List<$core.int> get programData => $_getN(2);
  @$pb.TagNumber(3)
  set programData($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasProgramData() => $_has(2);
  @$pb.TagNumber(3)
  void clearProgramData() => clearField(3);
}

class RawMessage_MessageAddressTableLookup extends $pb.GeneratedMessage {
  factory RawMessage_MessageAddressTableLookup({
    $core.String? accountKey,
    $core.Iterable<$core.int>? writableIndexes,
    $core.Iterable<$core.int>? readonlyIndexes,
  }) {
    final $result = create();
    if (accountKey != null) {
      $result.accountKey = accountKey;
    }
    if (writableIndexes != null) {
      $result.writableIndexes.addAll(writableIndexes);
    }
    if (readonlyIndexes != null) {
      $result.readonlyIndexes.addAll(readonlyIndexes);
    }
    return $result;
  }
  RawMessage_MessageAddressTableLookup._() : super();
  factory RawMessage_MessageAddressTableLookup.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RawMessage_MessageAddressTableLookup.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'RawMessage.MessageAddressTableLookup', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'accountKey')
    ..p<$core.int>(2, _omitFieldNames ? '' : 'writableIndexes', $pb.PbFieldType.KU3)
    ..p<$core.int>(3, _omitFieldNames ? '' : 'readonlyIndexes', $pb.PbFieldType.KU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  RawMessage_MessageAddressTableLookup clone() => RawMessage_MessageAddressTableLookup()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  RawMessage_MessageAddressTableLookup copyWith(void Function(RawMessage_MessageAddressTableLookup) updates) => super.copyWith((message) => updates(message as RawMessage_MessageAddressTableLookup)) as RawMessage_MessageAddressTableLookup;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static RawMessage_MessageAddressTableLookup create() => RawMessage_MessageAddressTableLookup._();
  RawMessage_MessageAddressTableLookup createEmptyInstance() => create();
  static $pb.PbList<RawMessage_MessageAddressTableLookup> createRepeated() => $pb.PbList<RawMessage_MessageAddressTableLookup>();
  @$core.pragma('dart2js:noInline')
  static RawMessage_MessageAddressTableLookup getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RawMessage_MessageAddressTableLookup>(create);
  static RawMessage_MessageAddressTableLookup? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get accountKey => $_getSZ(0);
  @$pb.TagNumber(1)
  set accountKey($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAccountKey() => $_has(0);
  @$pb.TagNumber(1)
  void clearAccountKey() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<$core.int> get writableIndexes => $_getList(1);

  @$pb.TagNumber(3)
  $core.List<$core.int> get readonlyIndexes => $_getList(2);
}

class RawMessage_MessageLegacy extends $pb.GeneratedMessage {
  factory RawMessage_MessageLegacy({
    RawMessage_MessageHeader? header,
    $core.Iterable<$core.String>? accountKeys,
    $core.String? recentBlockhash,
    $core.Iterable<RawMessage_Instruction>? instructions,
  }) {
    final $result = create();
    if (header != null) {
      $result.header = header;
    }
    if (accountKeys != null) {
      $result.accountKeys.addAll(accountKeys);
    }
    if (recentBlockhash != null) {
      $result.recentBlockhash = recentBlockhash;
    }
    if (instructions != null) {
      $result.instructions.addAll(instructions);
    }
    return $result;
  }
  RawMessage_MessageLegacy._() : super();
  factory RawMessage_MessageLegacy.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RawMessage_MessageLegacy.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'RawMessage.MessageLegacy', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOM<RawMessage_MessageHeader>(1, _omitFieldNames ? '' : 'header', subBuilder: RawMessage_MessageHeader.create)
    ..pPS(2, _omitFieldNames ? '' : 'accountKeys')
    ..aOS(3, _omitFieldNames ? '' : 'recentBlockhash')
    ..pc<RawMessage_Instruction>(4, _omitFieldNames ? '' : 'instructions', $pb.PbFieldType.PM, subBuilder: RawMessage_Instruction.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  RawMessage_MessageLegacy clone() => RawMessage_MessageLegacy()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  RawMessage_MessageLegacy copyWith(void Function(RawMessage_MessageLegacy) updates) => super.copyWith((message) => updates(message as RawMessage_MessageLegacy)) as RawMessage_MessageLegacy;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static RawMessage_MessageLegacy create() => RawMessage_MessageLegacy._();
  RawMessage_MessageLegacy createEmptyInstance() => create();
  static $pb.PbList<RawMessage_MessageLegacy> createRepeated() => $pb.PbList<RawMessage_MessageLegacy>();
  @$core.pragma('dart2js:noInline')
  static RawMessage_MessageLegacy getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RawMessage_MessageLegacy>(create);
  static RawMessage_MessageLegacy? _defaultInstance;

  @$pb.TagNumber(1)
  RawMessage_MessageHeader get header => $_getN(0);
  @$pb.TagNumber(1)
  set header(RawMessage_MessageHeader v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasHeader() => $_has(0);
  @$pb.TagNumber(1)
  void clearHeader() => clearField(1);
  @$pb.TagNumber(1)
  RawMessage_MessageHeader ensureHeader() => $_ensure(0);

  @$pb.TagNumber(2)
  $core.List<$core.String> get accountKeys => $_getList(1);

  /// Relatively recent block hash (base58 encoded).
  @$pb.TagNumber(3)
  $core.String get recentBlockhash => $_getSZ(2);
  @$pb.TagNumber(3)
  set recentBlockhash($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasRecentBlockhash() => $_has(2);
  @$pb.TagNumber(3)
  void clearRecentBlockhash() => clearField(3);

  @$pb.TagNumber(4)
  $core.List<RawMessage_Instruction> get instructions => $_getList(3);
}

class RawMessage_MessageV0 extends $pb.GeneratedMessage {
  factory RawMessage_MessageV0({
    RawMessage_MessageHeader? header,
    $core.Iterable<$core.String>? accountKeys,
    $core.String? recentBlockhash,
    $core.Iterable<RawMessage_Instruction>? instructions,
    $core.Iterable<RawMessage_MessageAddressTableLookup>? addressTableLookups,
  }) {
    final $result = create();
    if (header != null) {
      $result.header = header;
    }
    if (accountKeys != null) {
      $result.accountKeys.addAll(accountKeys);
    }
    if (recentBlockhash != null) {
      $result.recentBlockhash = recentBlockhash;
    }
    if (instructions != null) {
      $result.instructions.addAll(instructions);
    }
    if (addressTableLookups != null) {
      $result.addressTableLookups.addAll(addressTableLookups);
    }
    return $result;
  }
  RawMessage_MessageV0._() : super();
  factory RawMessage_MessageV0.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RawMessage_MessageV0.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'RawMessage.MessageV0', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOM<RawMessage_MessageHeader>(1, _omitFieldNames ? '' : 'header', subBuilder: RawMessage_MessageHeader.create)
    ..pPS(2, _omitFieldNames ? '' : 'accountKeys')
    ..aOS(3, _omitFieldNames ? '' : 'recentBlockhash')
    ..pc<RawMessage_Instruction>(4, _omitFieldNames ? '' : 'instructions', $pb.PbFieldType.PM, subBuilder: RawMessage_Instruction.create)
    ..pc<RawMessage_MessageAddressTableLookup>(5, _omitFieldNames ? '' : 'addressTableLookups', $pb.PbFieldType.PM, subBuilder: RawMessage_MessageAddressTableLookup.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  RawMessage_MessageV0 clone() => RawMessage_MessageV0()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  RawMessage_MessageV0 copyWith(void Function(RawMessage_MessageV0) updates) => super.copyWith((message) => updates(message as RawMessage_MessageV0)) as RawMessage_MessageV0;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static RawMessage_MessageV0 create() => RawMessage_MessageV0._();
  RawMessage_MessageV0 createEmptyInstance() => create();
  static $pb.PbList<RawMessage_MessageV0> createRepeated() => $pb.PbList<RawMessage_MessageV0>();
  @$core.pragma('dart2js:noInline')
  static RawMessage_MessageV0 getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RawMessage_MessageV0>(create);
  static RawMessage_MessageV0? _defaultInstance;

  @$pb.TagNumber(1)
  RawMessage_MessageHeader get header => $_getN(0);
  @$pb.TagNumber(1)
  set header(RawMessage_MessageHeader v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasHeader() => $_has(0);
  @$pb.TagNumber(1)
  void clearHeader() => clearField(1);
  @$pb.TagNumber(1)
  RawMessage_MessageHeader ensureHeader() => $_ensure(0);

  @$pb.TagNumber(2)
  $core.List<$core.String> get accountKeys => $_getList(1);

  /// Relatively recent block hash (base58 encoded).
  @$pb.TagNumber(3)
  $core.String get recentBlockhash => $_getSZ(2);
  @$pb.TagNumber(3)
  set recentBlockhash($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasRecentBlockhash() => $_has(2);
  @$pb.TagNumber(3)
  void clearRecentBlockhash() => clearField(3);

  @$pb.TagNumber(4)
  $core.List<RawMessage_Instruction> get instructions => $_getList(3);

  @$pb.TagNumber(5)
  $core.List<RawMessage_MessageAddressTableLookup> get addressTableLookups => $_getList(4);
}

enum RawMessage_Message {
  legacy, 
  v0, 
  notSet
}

class RawMessage extends $pb.GeneratedMessage {
  factory RawMessage({
    $core.Iterable<PubkeySignature>? signatures,
    RawMessage_MessageLegacy? legacy,
    RawMessage_MessageV0? v0,
  }) {
    final $result = create();
    if (signatures != null) {
      $result.signatures.addAll(signatures);
    }
    if (legacy != null) {
      $result.legacy = legacy;
    }
    if (v0 != null) {
      $result.v0 = v0;
    }
    return $result;
  }
  RawMessage._() : super();
  factory RawMessage.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RawMessage.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, RawMessage_Message> _RawMessage_MessageByTag = {
    2 : RawMessage_Message.legacy,
    3 : RawMessage_Message.v0,
    0 : RawMessage_Message.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'RawMessage', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..oo(0, [2, 3])
    ..pc<PubkeySignature>(1, _omitFieldNames ? '' : 'signatures', $pb.PbFieldType.PM, subBuilder: PubkeySignature.create)
    ..aOM<RawMessage_MessageLegacy>(2, _omitFieldNames ? '' : 'legacy', subBuilder: RawMessage_MessageLegacy.create)
    ..aOM<RawMessage_MessageV0>(3, _omitFieldNames ? '' : 'v0', subBuilder: RawMessage_MessageV0.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  RawMessage clone() => RawMessage()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  RawMessage copyWith(void Function(RawMessage) updates) => super.copyWith((message) => updates(message as RawMessage)) as RawMessage;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static RawMessage create() => RawMessage._();
  RawMessage createEmptyInstance() => create();
  static $pb.PbList<RawMessage> createRepeated() => $pb.PbList<RawMessage>();
  @$core.pragma('dart2js:noInline')
  static RawMessage getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RawMessage>(create);
  static RawMessage? _defaultInstance;

  RawMessage_Message whichMessage() => _RawMessage_MessageByTag[$_whichOneof(0)]!;
  void clearMessage() => clearField($_whichOneof(0));

  /// Transaction signatures.
  /// If private keys are set in `SigningInput`, corresponding signatures will be overriden.
  /// It's also possible some or all the signatures are be used to compile a transaction if corresponding private keys are not set.
  @$pb.TagNumber(1)
  $core.List<PubkeySignature> get signatures => $_getList(0);

  @$pb.TagNumber(2)
  RawMessage_MessageLegacy get legacy => $_getN(1);
  @$pb.TagNumber(2)
  set legacy(RawMessage_MessageLegacy v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasLegacy() => $_has(1);
  @$pb.TagNumber(2)
  void clearLegacy() => clearField(2);
  @$pb.TagNumber(2)
  RawMessage_MessageLegacy ensureLegacy() => $_ensure(1);

  @$pb.TagNumber(3)
  RawMessage_MessageV0 get v0 => $_getN(2);
  @$pb.TagNumber(3)
  set v0(RawMessage_MessageV0 v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasV0() => $_has(2);
  @$pb.TagNumber(3)
  void clearV0() => clearField(3);
  @$pb.TagNumber(3)
  RawMessage_MessageV0 ensureV0() => $_ensure(2);
}

class DecodingTransactionOutput extends $pb.GeneratedMessage {
  factory DecodingTransactionOutput({
    RawMessage? transaction,
    $0.SigningError? error,
    $core.String? errorMessage,
  }) {
    final $result = create();
    if (transaction != null) {
      $result.transaction = transaction;
    }
    if (error != null) {
      $result.error = error;
    }
    if (errorMessage != null) {
      $result.errorMessage = errorMessage;
    }
    return $result;
  }
  DecodingTransactionOutput._() : super();
  factory DecodingTransactionOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DecodingTransactionOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DecodingTransactionOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOM<RawMessage>(1, _omitFieldNames ? '' : 'transaction', subBuilder: RawMessage.create)
    ..e<$0.SigningError>(2, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(3, _omitFieldNames ? '' : 'errorMessage')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DecodingTransactionOutput clone() => DecodingTransactionOutput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DecodingTransactionOutput copyWith(void Function(DecodingTransactionOutput) updates) => super.copyWith((message) => updates(message as DecodingTransactionOutput)) as DecodingTransactionOutput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DecodingTransactionOutput create() => DecodingTransactionOutput._();
  DecodingTransactionOutput createEmptyInstance() => create();
  static $pb.PbList<DecodingTransactionOutput> createRepeated() => $pb.PbList<DecodingTransactionOutput>();
  @$core.pragma('dart2js:noInline')
  static DecodingTransactionOutput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DecodingTransactionOutput>(create);
  static DecodingTransactionOutput? _defaultInstance;

  /// Decoded transaction info.
  @$pb.TagNumber(1)
  RawMessage get transaction => $_getN(0);
  @$pb.TagNumber(1)
  set transaction(RawMessage v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasTransaction() => $_has(0);
  @$pb.TagNumber(1)
  void clearTransaction() => clearField(1);
  @$pb.TagNumber(1)
  RawMessage ensureTransaction() => $_ensure(0);

  /// Error code, 0 is ok, other codes will be treated as errors
  @$pb.TagNumber(2)
  $0.SigningError get error => $_getN(1);
  @$pb.TagNumber(2)
  set error($0.SigningError v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasError() => $_has(1);
  @$pb.TagNumber(2)
  void clearError() => clearField(2);

  /// Error code description
  @$pb.TagNumber(3)
  $core.String get errorMessage => $_getSZ(2);
  @$pb.TagNumber(3)
  set errorMessage($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasErrorMessage() => $_has(2);
  @$pb.TagNumber(3)
  void clearErrorMessage() => clearField(3);
}

/// Specific compute unit limit that the transaction is allowed to consume.
class PriorityFeePrice extends $pb.GeneratedMessage {
  factory PriorityFeePrice({
    $fixnum.Int64? price,
  }) {
    final $result = create();
    if (price != null) {
      $result.price = price;
    }
    return $result;
  }
  PriorityFeePrice._() : super();
  factory PriorityFeePrice.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PriorityFeePrice.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'PriorityFeePrice', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'price', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  PriorityFeePrice clone() => PriorityFeePrice()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  PriorityFeePrice copyWith(void Function(PriorityFeePrice) updates) => super.copyWith((message) => updates(message as PriorityFeePrice)) as PriorityFeePrice;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static PriorityFeePrice create() => PriorityFeePrice._();
  PriorityFeePrice createEmptyInstance() => create();
  static $pb.PbList<PriorityFeePrice> createRepeated() => $pb.PbList<PriorityFeePrice>();
  @$core.pragma('dart2js:noInline')
  static PriorityFeePrice getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PriorityFeePrice>(create);
  static PriorityFeePrice? _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get price => $_getI64(0);
  @$pb.TagNumber(1)
  set price($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPrice() => $_has(0);
  @$pb.TagNumber(1)
  void clearPrice() => clearField(1);
}

/// Compute unit price in "micro-lamports" to pay a higher transaction fee for higher transaction prioritization.
class PriorityFeeLimit extends $pb.GeneratedMessage {
  factory PriorityFeeLimit({
    $core.int? limit,
  }) {
    final $result = create();
    if (limit != null) {
      $result.limit = limit;
    }
    return $result;
  }
  PriorityFeeLimit._() : super();
  factory PriorityFeeLimit.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PriorityFeeLimit.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'PriorityFeeLimit', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'limit', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  PriorityFeeLimit clone() => PriorityFeeLimit()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  PriorityFeeLimit copyWith(void Function(PriorityFeeLimit) updates) => super.copyWith((message) => updates(message as PriorityFeeLimit)) as PriorityFeeLimit;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static PriorityFeeLimit create() => PriorityFeeLimit._();
  PriorityFeeLimit createEmptyInstance() => create();
  static $pb.PbList<PriorityFeeLimit> createRepeated() => $pb.PbList<PriorityFeeLimit>();
  @$core.pragma('dart2js:noInline')
  static PriorityFeeLimit getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PriorityFeeLimit>(create);
  static PriorityFeeLimit? _defaultInstance;

  @$pb.TagNumber(2)
  $core.int get limit => $_getIZ(0);
  @$pb.TagNumber(2)
  set limit($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(2)
  $core.bool hasLimit() => $_has(0);
  @$pb.TagNumber(2)
  void clearLimit() => clearField(2);
}

enum SigningInput_TransactionType {
  transferTransaction, 
  delegateStakeTransaction, 
  deactivateStakeTransaction, 
  deactivateAllStakeTransaction, 
  withdrawTransaction, 
  withdrawAllTransaction, 
  createTokenAccountTransaction, 
  tokenTransferTransaction, 
  createAndTransferTokenTransaction, 
  createNonceAccount, 
  withdrawNonceAccount, 
  advanceNonceAccount, 
  notSet
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.List<$core.int>? privateKey,
    $core.String? recentBlockhash,
    $core.bool? v0Msg,
    Transfer? transferTransaction,
    DelegateStake? delegateStakeTransaction,
    DeactivateStake? deactivateStakeTransaction,
    DeactivateAllStake? deactivateAllStakeTransaction,
    WithdrawStake? withdrawTransaction,
    WithdrawAllStake? withdrawAllTransaction,
    CreateTokenAccount? createTokenAccountTransaction,
    TokenTransfer? tokenTransferTransaction,
    CreateAndTransferToken? createAndTransferTokenTransaction,
    CreateNonceAccount? createNonceAccount,
    $core.String? sender,
    $core.String? nonceAccount,
    WithdrawNonceAccount? withdrawNonceAccount,
    $core.List<$core.int>? feePayerPrivateKey,
    $core.String? feePayer,
    AdvanceNonceAccount? advanceNonceAccount,
    RawMessage? rawMessage,
    Encoding? txEncoding,
    PriorityFeePrice? priorityFeePrice,
    PriorityFeeLimit? priorityFeeLimit,
  }) {
    final $result = create();
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (recentBlockhash != null) {
      $result.recentBlockhash = recentBlockhash;
    }
    if (v0Msg != null) {
      $result.v0Msg = v0Msg;
    }
    if (transferTransaction != null) {
      $result.transferTransaction = transferTransaction;
    }
    if (delegateStakeTransaction != null) {
      $result.delegateStakeTransaction = delegateStakeTransaction;
    }
    if (deactivateStakeTransaction != null) {
      $result.deactivateStakeTransaction = deactivateStakeTransaction;
    }
    if (deactivateAllStakeTransaction != null) {
      $result.deactivateAllStakeTransaction = deactivateAllStakeTransaction;
    }
    if (withdrawTransaction != null) {
      $result.withdrawTransaction = withdrawTransaction;
    }
    if (withdrawAllTransaction != null) {
      $result.withdrawAllTransaction = withdrawAllTransaction;
    }
    if (createTokenAccountTransaction != null) {
      $result.createTokenAccountTransaction = createTokenAccountTransaction;
    }
    if (tokenTransferTransaction != null) {
      $result.tokenTransferTransaction = tokenTransferTransaction;
    }
    if (createAndTransferTokenTransaction != null) {
      $result.createAndTransferTokenTransaction = createAndTransferTokenTransaction;
    }
    if (createNonceAccount != null) {
      $result.createNonceAccount = createNonceAccount;
    }
    if (sender != null) {
      $result.sender = sender;
    }
    if (nonceAccount != null) {
      $result.nonceAccount = nonceAccount;
    }
    if (withdrawNonceAccount != null) {
      $result.withdrawNonceAccount = withdrawNonceAccount;
    }
    if (feePayerPrivateKey != null) {
      $result.feePayerPrivateKey = feePayerPrivateKey;
    }
    if (feePayer != null) {
      $result.feePayer = feePayer;
    }
    if (advanceNonceAccount != null) {
      $result.advanceNonceAccount = advanceNonceAccount;
    }
    if (rawMessage != null) {
      $result.rawMessage = rawMessage;
    }
    if (txEncoding != null) {
      $result.txEncoding = txEncoding;
    }
    if (priorityFeePrice != null) {
      $result.priorityFeePrice = priorityFeePrice;
    }
    if (priorityFeeLimit != null) {
      $result.priorityFeeLimit = priorityFeeLimit;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, SigningInput_TransactionType> _SigningInput_TransactionTypeByTag = {
    4 : SigningInput_TransactionType.transferTransaction,
    5 : SigningInput_TransactionType.delegateStakeTransaction,
    6 : SigningInput_TransactionType.deactivateStakeTransaction,
    7 : SigningInput_TransactionType.deactivateAllStakeTransaction,
    8 : SigningInput_TransactionType.withdrawTransaction,
    9 : SigningInput_TransactionType.withdrawAllTransaction,
    10 : SigningInput_TransactionType.createTokenAccountTransaction,
    11 : SigningInput_TransactionType.tokenTransferTransaction,
    12 : SigningInput_TransactionType.createAndTransferTokenTransaction,
    13 : SigningInput_TransactionType.createNonceAccount,
    16 : SigningInput_TransactionType.withdrawNonceAccount,
    19 : SigningInput_TransactionType.advanceNonceAccount,
    0 : SigningInput_TransactionType.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..oo(0, [4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 16, 19])
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..aOS(2, _omitFieldNames ? '' : 'recentBlockhash')
    ..aOB(3, _omitFieldNames ? '' : 'v0Msg')
    ..aOM<Transfer>(4, _omitFieldNames ? '' : 'transferTransaction', subBuilder: Transfer.create)
    ..aOM<DelegateStake>(5, _omitFieldNames ? '' : 'delegateStakeTransaction', subBuilder: DelegateStake.create)
    ..aOM<DeactivateStake>(6, _omitFieldNames ? '' : 'deactivateStakeTransaction', subBuilder: DeactivateStake.create)
    ..aOM<DeactivateAllStake>(7, _omitFieldNames ? '' : 'deactivateAllStakeTransaction', subBuilder: DeactivateAllStake.create)
    ..aOM<WithdrawStake>(8, _omitFieldNames ? '' : 'withdrawTransaction', subBuilder: WithdrawStake.create)
    ..aOM<WithdrawAllStake>(9, _omitFieldNames ? '' : 'withdrawAllTransaction', subBuilder: WithdrawAllStake.create)
    ..aOM<CreateTokenAccount>(10, _omitFieldNames ? '' : 'createTokenAccountTransaction', subBuilder: CreateTokenAccount.create)
    ..aOM<TokenTransfer>(11, _omitFieldNames ? '' : 'tokenTransferTransaction', subBuilder: TokenTransfer.create)
    ..aOM<CreateAndTransferToken>(12, _omitFieldNames ? '' : 'createAndTransferTokenTransaction', subBuilder: CreateAndTransferToken.create)
    ..aOM<CreateNonceAccount>(13, _omitFieldNames ? '' : 'createNonceAccount', subBuilder: CreateNonceAccount.create)
    ..aOS(14, _omitFieldNames ? '' : 'sender')
    ..aOS(15, _omitFieldNames ? '' : 'nonceAccount')
    ..aOM<WithdrawNonceAccount>(16, _omitFieldNames ? '' : 'withdrawNonceAccount', subBuilder: WithdrawNonceAccount.create)
    ..a<$core.List<$core.int>>(17, _omitFieldNames ? '' : 'feePayerPrivateKey', $pb.PbFieldType.OY)
    ..aOS(18, _omitFieldNames ? '' : 'feePayer')
    ..aOM<AdvanceNonceAccount>(19, _omitFieldNames ? '' : 'advanceNonceAccount', subBuilder: AdvanceNonceAccount.create)
    ..aOM<RawMessage>(20, _omitFieldNames ? '' : 'rawMessage', subBuilder: RawMessage.create)
    ..e<Encoding>(21, _omitFieldNames ? '' : 'txEncoding', $pb.PbFieldType.OE, defaultOrMaker: Encoding.Base58, valueOf: Encoding.valueOf, enumValues: Encoding.values)
    ..aOM<PriorityFeePrice>(22, _omitFieldNames ? '' : 'priorityFeePrice', subBuilder: PriorityFeePrice.create)
    ..aOM<PriorityFeeLimit>(23, _omitFieldNames ? '' : 'priorityFeeLimit', subBuilder: PriorityFeeLimit.create)
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

  SigningInput_TransactionType whichTransactionType() => _SigningInput_TransactionTypeByTag[$_whichOneof(0)]!;
  void clearTransactionType() => clearField($_whichOneof(0));

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(1)
  $core.List<$core.int> get privateKey => $_getN(0);
  @$pb.TagNumber(1)
  set privateKey($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPrivateKey() => $_has(0);
  @$pb.TagNumber(1)
  void clearPrivateKey() => clearField(1);

  /// Relatively recent block hash
  @$pb.TagNumber(2)
  $core.String get recentBlockhash => $_getSZ(1);
  @$pb.TagNumber(2)
  set recentBlockhash($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasRecentBlockhash() => $_has(1);
  @$pb.TagNumber(2)
  void clearRecentBlockhash() => clearField(2);

  @$pb.TagNumber(3)
  $core.bool get v0Msg => $_getBF(2);
  @$pb.TagNumber(3)
  set v0Msg($core.bool v) { $_setBool(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasV0Msg() => $_has(2);
  @$pb.TagNumber(3)
  void clearV0Msg() => clearField(3);

  @$pb.TagNumber(4)
  Transfer get transferTransaction => $_getN(3);
  @$pb.TagNumber(4)
  set transferTransaction(Transfer v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasTransferTransaction() => $_has(3);
  @$pb.TagNumber(4)
  void clearTransferTransaction() => clearField(4);
  @$pb.TagNumber(4)
  Transfer ensureTransferTransaction() => $_ensure(3);

  @$pb.TagNumber(5)
  DelegateStake get delegateStakeTransaction => $_getN(4);
  @$pb.TagNumber(5)
  set delegateStakeTransaction(DelegateStake v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasDelegateStakeTransaction() => $_has(4);
  @$pb.TagNumber(5)
  void clearDelegateStakeTransaction() => clearField(5);
  @$pb.TagNumber(5)
  DelegateStake ensureDelegateStakeTransaction() => $_ensure(4);

  @$pb.TagNumber(6)
  DeactivateStake get deactivateStakeTransaction => $_getN(5);
  @$pb.TagNumber(6)
  set deactivateStakeTransaction(DeactivateStake v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasDeactivateStakeTransaction() => $_has(5);
  @$pb.TagNumber(6)
  void clearDeactivateStakeTransaction() => clearField(6);
  @$pb.TagNumber(6)
  DeactivateStake ensureDeactivateStakeTransaction() => $_ensure(5);

  @$pb.TagNumber(7)
  DeactivateAllStake get deactivateAllStakeTransaction => $_getN(6);
  @$pb.TagNumber(7)
  set deactivateAllStakeTransaction(DeactivateAllStake v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasDeactivateAllStakeTransaction() => $_has(6);
  @$pb.TagNumber(7)
  void clearDeactivateAllStakeTransaction() => clearField(7);
  @$pb.TagNumber(7)
  DeactivateAllStake ensureDeactivateAllStakeTransaction() => $_ensure(6);

  @$pb.TagNumber(8)
  WithdrawStake get withdrawTransaction => $_getN(7);
  @$pb.TagNumber(8)
  set withdrawTransaction(WithdrawStake v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasWithdrawTransaction() => $_has(7);
  @$pb.TagNumber(8)
  void clearWithdrawTransaction() => clearField(8);
  @$pb.TagNumber(8)
  WithdrawStake ensureWithdrawTransaction() => $_ensure(7);

  @$pb.TagNumber(9)
  WithdrawAllStake get withdrawAllTransaction => $_getN(8);
  @$pb.TagNumber(9)
  set withdrawAllTransaction(WithdrawAllStake v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasWithdrawAllTransaction() => $_has(8);
  @$pb.TagNumber(9)
  void clearWithdrawAllTransaction() => clearField(9);
  @$pb.TagNumber(9)
  WithdrawAllStake ensureWithdrawAllTransaction() => $_ensure(8);

  @$pb.TagNumber(10)
  CreateTokenAccount get createTokenAccountTransaction => $_getN(9);
  @$pb.TagNumber(10)
  set createTokenAccountTransaction(CreateTokenAccount v) { setField(10, v); }
  @$pb.TagNumber(10)
  $core.bool hasCreateTokenAccountTransaction() => $_has(9);
  @$pb.TagNumber(10)
  void clearCreateTokenAccountTransaction() => clearField(10);
  @$pb.TagNumber(10)
  CreateTokenAccount ensureCreateTokenAccountTransaction() => $_ensure(9);

  @$pb.TagNumber(11)
  TokenTransfer get tokenTransferTransaction => $_getN(10);
  @$pb.TagNumber(11)
  set tokenTransferTransaction(TokenTransfer v) { setField(11, v); }
  @$pb.TagNumber(11)
  $core.bool hasTokenTransferTransaction() => $_has(10);
  @$pb.TagNumber(11)
  void clearTokenTransferTransaction() => clearField(11);
  @$pb.TagNumber(11)
  TokenTransfer ensureTokenTransferTransaction() => $_ensure(10);

  @$pb.TagNumber(12)
  CreateAndTransferToken get createAndTransferTokenTransaction => $_getN(11);
  @$pb.TagNumber(12)
  set createAndTransferTokenTransaction(CreateAndTransferToken v) { setField(12, v); }
  @$pb.TagNumber(12)
  $core.bool hasCreateAndTransferTokenTransaction() => $_has(11);
  @$pb.TagNumber(12)
  void clearCreateAndTransferTokenTransaction() => clearField(12);
  @$pb.TagNumber(12)
  CreateAndTransferToken ensureCreateAndTransferTokenTransaction() => $_ensure(11);

  @$pb.TagNumber(13)
  CreateNonceAccount get createNonceAccount => $_getN(12);
  @$pb.TagNumber(13)
  set createNonceAccount(CreateNonceAccount v) { setField(13, v); }
  @$pb.TagNumber(13)
  $core.bool hasCreateNonceAccount() => $_has(12);
  @$pb.TagNumber(13)
  void clearCreateNonceAccount() => clearField(13);
  @$pb.TagNumber(13)
  CreateNonceAccount ensureCreateNonceAccount() => $_ensure(12);

  /// Required for building pre-signing hash of a transaction
  @$pb.TagNumber(14)
  $core.String get sender => $_getSZ(13);
  @$pb.TagNumber(14)
  set sender($core.String v) { $_setString(13, v); }
  @$pb.TagNumber(14)
  $core.bool hasSender() => $_has(13);
  @$pb.TagNumber(14)
  void clearSender() => clearField(14);

  /// Required for using durable transaction nonce
  @$pb.TagNumber(15)
  $core.String get nonceAccount => $_getSZ(14);
  @$pb.TagNumber(15)
  set nonceAccount($core.String v) { $_setString(14, v); }
  @$pb.TagNumber(15)
  $core.bool hasNonceAccount() => $_has(14);
  @$pb.TagNumber(15)
  void clearNonceAccount() => clearField(15);

  @$pb.TagNumber(16)
  WithdrawNonceAccount get withdrawNonceAccount => $_getN(15);
  @$pb.TagNumber(16)
  set withdrawNonceAccount(WithdrawNonceAccount v) { setField(16, v); }
  @$pb.TagNumber(16)
  $core.bool hasWithdrawNonceAccount() => $_has(15);
  @$pb.TagNumber(16)
  void clearWithdrawNonceAccount() => clearField(16);
  @$pb.TagNumber(16)
  WithdrawNonceAccount ensureWithdrawNonceAccount() => $_ensure(15);

  /// Optional external fee payer private key. support: TokenTransfer, CreateAndTransferToken
  @$pb.TagNumber(17)
  $core.List<$core.int> get feePayerPrivateKey => $_getN(16);
  @$pb.TagNumber(17)
  set feePayerPrivateKey($core.List<$core.int> v) { $_setBytes(16, v); }
  @$pb.TagNumber(17)
  $core.bool hasFeePayerPrivateKey() => $_has(16);
  @$pb.TagNumber(17)
  void clearFeePayerPrivateKey() => clearField(17);

  /// Optional external fee payer. support: TokenTransfer, CreateAndTransferToken
  @$pb.TagNumber(18)
  $core.String get feePayer => $_getSZ(17);
  @$pb.TagNumber(18)
  set feePayer($core.String v) { $_setString(17, v); }
  @$pb.TagNumber(18)
  $core.bool hasFeePayer() => $_has(17);
  @$pb.TagNumber(18)
  void clearFeePayer() => clearField(18);

  @$pb.TagNumber(19)
  AdvanceNonceAccount get advanceNonceAccount => $_getN(18);
  @$pb.TagNumber(19)
  set advanceNonceAccount(AdvanceNonceAccount v) { setField(19, v); }
  @$pb.TagNumber(19)
  $core.bool hasAdvanceNonceAccount() => $_has(18);
  @$pb.TagNumber(19)
  void clearAdvanceNonceAccount() => clearField(19);
  @$pb.TagNumber(19)
  AdvanceNonceAccount ensureAdvanceNonceAccount() => $_ensure(18);

  /// Optional message plan. For signing an already prepared message.
  @$pb.TagNumber(20)
  RawMessage get rawMessage => $_getN(19);
  @$pb.TagNumber(20)
  set rawMessage(RawMessage v) { setField(20, v); }
  @$pb.TagNumber(20)
  $core.bool hasRawMessage() => $_has(19);
  @$pb.TagNumber(20)
  void clearRawMessage() => clearField(20);
  @$pb.TagNumber(20)
  RawMessage ensureRawMessage() => $_ensure(19);

  /// Output transaction encoding.
  @$pb.TagNumber(21)
  Encoding get txEncoding => $_getN(20);
  @$pb.TagNumber(21)
  set txEncoding(Encoding v) { setField(21, v); }
  @$pb.TagNumber(21)
  $core.bool hasTxEncoding() => $_has(20);
  @$pb.TagNumber(21)
  void clearTxEncoding() => clearField(21);

  /// Optional. Set a specific compute unit limit that the transaction is allowed to consume.
  /// https://solana.com/docs/intro/transaction_fees#prioritization-fee
  @$pb.TagNumber(22)
  PriorityFeePrice get priorityFeePrice => $_getN(21);
  @$pb.TagNumber(22)
  set priorityFeePrice(PriorityFeePrice v) { setField(22, v); }
  @$pb.TagNumber(22)
  $core.bool hasPriorityFeePrice() => $_has(21);
  @$pb.TagNumber(22)
  void clearPriorityFeePrice() => clearField(22);
  @$pb.TagNumber(22)
  PriorityFeePrice ensurePriorityFeePrice() => $_ensure(21);

  /// Optional. Set a compute unit price in "micro-lamports" to pay a higher transaction
  /// fee for higher transaction prioritization.
  /// https://solana.com/docs/intro/transaction_fees#prioritization-fee
  @$pb.TagNumber(23)
  PriorityFeeLimit get priorityFeeLimit => $_getN(22);
  @$pb.TagNumber(23)
  set priorityFeeLimit(PriorityFeeLimit v) { setField(23, v); }
  @$pb.TagNumber(23)
  $core.bool hasPriorityFeeLimit() => $_has(22);
  @$pb.TagNumber(23)
  void clearPriorityFeeLimit() => clearField(23);
  @$pb.TagNumber(23)
  PriorityFeeLimit ensurePriorityFeeLimit() => $_ensure(22);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.String? encoded,
    $0.SigningError? error,
    $core.String? errorMessage,
    $core.String? unsignedTx,
    $core.Iterable<PubkeySignature>? signatures,
  }) {
    final $result = create();
    if (encoded != null) {
      $result.encoded = encoded;
    }
    if (error != null) {
      $result.error = error;
    }
    if (errorMessage != null) {
      $result.errorMessage = errorMessage;
    }
    if (unsignedTx != null) {
      $result.unsignedTx = unsignedTx;
    }
    if (signatures != null) {
      $result.signatures.addAll(signatures);
    }
    return $result;
  }
  SigningOutput._() : super();
  factory SigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'encoded')
    ..e<$0.SigningError>(2, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(3, _omitFieldNames ? '' : 'errorMessage')
    ..aOS(4, _omitFieldNames ? '' : 'unsignedTx')
    ..pc<PubkeySignature>(5, _omitFieldNames ? '' : 'signatures', $pb.PbFieldType.PM, subBuilder: PubkeySignature.create)
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

  /// The encoded transaction
  @$pb.TagNumber(1)
  $core.String get encoded => $_getSZ(0);
  @$pb.TagNumber(1)
  set encoded($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasEncoded() => $_has(0);
  @$pb.TagNumber(1)
  void clearEncoded() => clearField(1);

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

  /// The encoded message. Can be used to estimate a transaction fee required to execute the message.
  @$pb.TagNumber(4)
  $core.String get unsignedTx => $_getSZ(3);
  @$pb.TagNumber(4)
  set unsignedTx($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasUnsignedTx() => $_has(3);
  @$pb.TagNumber(4)
  void clearUnsignedTx() => clearField(4);

  /// Transaction signatures (may include external signatures).
  @$pb.TagNumber(5)
  $core.List<PubkeySignature> get signatures => $_getList(4);
}

/// / Transaction pre-signing output
class PreSigningOutput extends $pb.GeneratedMessage {
  factory PreSigningOutput({
    $core.Iterable<$core.List<$core.int>>? signers,
    $core.List<$core.int>? data,
    $0.SigningError? error,
    $core.String? errorMessage,
  }) {
    final $result = create();
    if (signers != null) {
      $result.signers.addAll(signers);
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
    return $result;
  }
  PreSigningOutput._() : super();
  factory PreSigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PreSigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'PreSigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Solana.Proto'), createEmptyInstance: create)
    ..p<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'signers', $pb.PbFieldType.PY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'data', $pb.PbFieldType.OY)
    ..e<$0.SigningError>(3, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(4, _omitFieldNames ? '' : 'errorMessage')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  PreSigningOutput clone() => PreSigningOutput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  PreSigningOutput copyWith(void Function(PreSigningOutput) updates) => super.copyWith((message) => updates(message as PreSigningOutput)) as PreSigningOutput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static PreSigningOutput create() => PreSigningOutput._();
  PreSigningOutput createEmptyInstance() => create();
  static $pb.PbList<PreSigningOutput> createRepeated() => $pb.PbList<PreSigningOutput>();
  @$core.pragma('dart2js:noInline')
  static PreSigningOutput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PreSigningOutput>(create);
  static PreSigningOutput? _defaultInstance;

  /// Signer list
  @$pb.TagNumber(1)
  $core.List<$core.List<$core.int>> get signers => $_getList(0);

  /// Pre-image data. There is no hashing for Solana presign image
  @$pb.TagNumber(2)
  $core.List<$core.int> get data => $_getN(1);
  @$pb.TagNumber(2)
  set data($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasData() => $_has(1);
  @$pb.TagNumber(2)
  void clearData() => clearField(2);

  /// Error code, 0 is ok, other codes will be treated as errors
  @$pb.TagNumber(3)
  $0.SigningError get error => $_getN(2);
  @$pb.TagNumber(3)
  set error($0.SigningError v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasError() => $_has(2);
  @$pb.TagNumber(3)
  void clearError() => clearField(3);

  /// Error code description
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
