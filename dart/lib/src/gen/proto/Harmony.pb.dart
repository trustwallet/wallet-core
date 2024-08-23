//
//  Generated code. Do not modify.
//  source: Harmony.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

import 'Common.pbenum.dart' as $0;

enum SigningInput_MessageOneof {
  transactionMessage, 
  stakingMessage, 
  notSet
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.List<$core.int>? chainId,
    $core.List<$core.int>? privateKey,
    TransactionMessage? transactionMessage,
    StakingMessage? stakingMessage,
  }) {
    final $result = create();
    if (chainId != null) {
      $result.chainId = chainId;
    }
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (transactionMessage != null) {
      $result.transactionMessage = transactionMessage;
    }
    if (stakingMessage != null) {
      $result.stakingMessage = stakingMessage;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, SigningInput_MessageOneof> _SigningInput_MessageOneofByTag = {
    3 : SigningInput_MessageOneof.transactionMessage,
    4 : SigningInput_MessageOneof.stakingMessage,
    0 : SigningInput_MessageOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Harmony.Proto'), createEmptyInstance: create)
    ..oo(0, [3, 4])
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'chainId', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..aOM<TransactionMessage>(3, _omitFieldNames ? '' : 'transactionMessage', subBuilder: TransactionMessage.create)
    ..aOM<StakingMessage>(4, _omitFieldNames ? '' : 'stakingMessage', subBuilder: StakingMessage.create)
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

  SigningInput_MessageOneof whichMessageOneof() => _SigningInput_MessageOneofByTag[$_whichOneof(0)]!;
  void clearMessageOneof() => clearField($_whichOneof(0));

  /// Chain identifier (uint256, serialized big endian)
  @$pb.TagNumber(1)
  $core.List<$core.int> get chainId => $_getN(0);
  @$pb.TagNumber(1)
  set chainId($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasChainId() => $_has(0);
  @$pb.TagNumber(1)
  void clearChainId() => clearField(1);

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(2)
  $core.List<$core.int> get privateKey => $_getN(1);
  @$pb.TagNumber(2)
  set privateKey($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPrivateKey() => $_has(1);
  @$pb.TagNumber(2)
  void clearPrivateKey() => clearField(2);

  @$pb.TagNumber(3)
  TransactionMessage get transactionMessage => $_getN(2);
  @$pb.TagNumber(3)
  set transactionMessage(TransactionMessage v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasTransactionMessage() => $_has(2);
  @$pb.TagNumber(3)
  void clearTransactionMessage() => clearField(3);
  @$pb.TagNumber(3)
  TransactionMessage ensureTransactionMessage() => $_ensure(2);

  @$pb.TagNumber(4)
  StakingMessage get stakingMessage => $_getN(3);
  @$pb.TagNumber(4)
  set stakingMessage(StakingMessage v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasStakingMessage() => $_has(3);
  @$pb.TagNumber(4)
  void clearStakingMessage() => clearField(4);
  @$pb.TagNumber(4)
  StakingMessage ensureStakingMessage() => $_ensure(3);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.List<$core.int>? encoded,
    $core.List<$core.int>? v,
    $core.List<$core.int>? r,
    $core.List<$core.int>? s,
    $0.SigningError? error,
    $core.String? errorMessage,
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

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Harmony.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'encoded', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'v', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'r', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 's', $pb.PbFieldType.OY)
    ..e<$0.SigningError>(5, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(6, _omitFieldNames ? '' : 'errorMessage')
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

  /// THE V,R,S components of the signature
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

  /// error code, 0 is ok, other codes will be treated as errors
  @$pb.TagNumber(5)
  $0.SigningError get error => $_getN(4);
  @$pb.TagNumber(5)
  set error($0.SigningError v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasError() => $_has(4);
  @$pb.TagNumber(5)
  void clearError() => clearField(5);

  /// error code description
  @$pb.TagNumber(6)
  $core.String get errorMessage => $_getSZ(5);
  @$pb.TagNumber(6)
  set errorMessage($core.String v) { $_setString(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasErrorMessage() => $_has(5);
  @$pb.TagNumber(6)
  void clearErrorMessage() => clearField(6);
}

/// A Transfer message
class TransactionMessage extends $pb.GeneratedMessage {
  factory TransactionMessage({
    $core.List<$core.int>? nonce,
    $core.List<$core.int>? gasPrice,
    $core.List<$core.int>? gasLimit,
    $core.String? toAddress,
    $core.List<$core.int>? amount,
    $core.List<$core.int>? payload,
    $core.List<$core.int>? fromShardId,
    $core.List<$core.int>? toShardId,
  }) {
    final $result = create();
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
    if (payload != null) {
      $result.payload = payload;
    }
    if (fromShardId != null) {
      $result.fromShardId = fromShardId;
    }
    if (toShardId != null) {
      $result.toShardId = toShardId;
    }
    return $result;
  }
  TransactionMessage._() : super();
  factory TransactionMessage.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TransactionMessage.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TransactionMessage', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Harmony.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'nonce', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'gasPrice', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'gasLimit', $pb.PbFieldType.OY)
    ..aOS(4, _omitFieldNames ? '' : 'toAddress')
    ..a<$core.List<$core.int>>(5, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(6, _omitFieldNames ? '' : 'payload', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(7, _omitFieldNames ? '' : 'fromShardId', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(8, _omitFieldNames ? '' : 'toShardId', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TransactionMessage clone() => TransactionMessage()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TransactionMessage copyWith(void Function(TransactionMessage) updates) => super.copyWith((message) => updates(message as TransactionMessage)) as TransactionMessage;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TransactionMessage create() => TransactionMessage._();
  TransactionMessage createEmptyInstance() => create();
  static $pb.PbList<TransactionMessage> createRepeated() => $pb.PbList<TransactionMessage>();
  @$core.pragma('dart2js:noInline')
  static TransactionMessage getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TransactionMessage>(create);
  static TransactionMessage? _defaultInstance;

  /// Nonce (uint256, serialized big endian)
  @$pb.TagNumber(1)
  $core.List<$core.int> get nonce => $_getN(0);
  @$pb.TagNumber(1)
  set nonce($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasNonce() => $_has(0);
  @$pb.TagNumber(1)
  void clearNonce() => clearField(1);

  /// Gas price (uint256, serialized big endian)
  @$pb.TagNumber(2)
  $core.List<$core.int> get gasPrice => $_getN(1);
  @$pb.TagNumber(2)
  set gasPrice($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasGasPrice() => $_has(1);
  @$pb.TagNumber(2)
  void clearGasPrice() => clearField(2);

  /// Gas limit (uint256, serialized big endian)
  @$pb.TagNumber(3)
  $core.List<$core.int> get gasLimit => $_getN(2);
  @$pb.TagNumber(3)
  set gasLimit($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasGasLimit() => $_has(2);
  @$pb.TagNumber(3)
  void clearGasLimit() => clearField(3);

  /// Recipient's address.
  @$pb.TagNumber(4)
  $core.String get toAddress => $_getSZ(3);
  @$pb.TagNumber(4)
  set toAddress($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasToAddress() => $_has(3);
  @$pb.TagNumber(4)
  void clearToAddress() => clearField(4);

  /// Amount to send in wei (uint256, serialized big endian)
  @$pb.TagNumber(5)
  $core.List<$core.int> get amount => $_getN(4);
  @$pb.TagNumber(5)
  set amount($core.List<$core.int> v) { $_setBytes(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasAmount() => $_has(4);
  @$pb.TagNumber(5)
  void clearAmount() => clearField(5);

  /// Optional payload
  @$pb.TagNumber(6)
  $core.List<$core.int> get payload => $_getN(5);
  @$pb.TagNumber(6)
  set payload($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasPayload() => $_has(5);
  @$pb.TagNumber(6)
  void clearPayload() => clearField(6);

  /// From shard ID (uint256, serialized big endian)
  @$pb.TagNumber(7)
  $core.List<$core.int> get fromShardId => $_getN(6);
  @$pb.TagNumber(7)
  set fromShardId($core.List<$core.int> v) { $_setBytes(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasFromShardId() => $_has(6);
  @$pb.TagNumber(7)
  void clearFromShardId() => clearField(7);

  /// To Shard ID (uint256, serialized big endian)
  @$pb.TagNumber(8)
  $core.List<$core.int> get toShardId => $_getN(7);
  @$pb.TagNumber(8)
  set toShardId($core.List<$core.int> v) { $_setBytes(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasToShardId() => $_has(7);
  @$pb.TagNumber(8)
  void clearToShardId() => clearField(8);
}

enum StakingMessage_StakeMsg {
  createValidatorMessage, 
  editValidatorMessage, 
  delegateMessage, 
  undelegateMessage, 
  collectRewards, 
  notSet
}

/// A Staking message.
class StakingMessage extends $pb.GeneratedMessage {
  factory StakingMessage({
    DirectiveCreateValidator? createValidatorMessage,
    DirectiveEditValidator? editValidatorMessage,
    DirectiveDelegate? delegateMessage,
    DirectiveUndelegate? undelegateMessage,
    DirectiveCollectRewards? collectRewards,
    $core.List<$core.int>? nonce,
    $core.List<$core.int>? gasPrice,
    $core.List<$core.int>? gasLimit,
  }) {
    final $result = create();
    if (createValidatorMessage != null) {
      $result.createValidatorMessage = createValidatorMessage;
    }
    if (editValidatorMessage != null) {
      $result.editValidatorMessage = editValidatorMessage;
    }
    if (delegateMessage != null) {
      $result.delegateMessage = delegateMessage;
    }
    if (undelegateMessage != null) {
      $result.undelegateMessage = undelegateMessage;
    }
    if (collectRewards != null) {
      $result.collectRewards = collectRewards;
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
    return $result;
  }
  StakingMessage._() : super();
  factory StakingMessage.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory StakingMessage.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, StakingMessage_StakeMsg> _StakingMessage_StakeMsgByTag = {
    1 : StakingMessage_StakeMsg.createValidatorMessage,
    2 : StakingMessage_StakeMsg.editValidatorMessage,
    3 : StakingMessage_StakeMsg.delegateMessage,
    4 : StakingMessage_StakeMsg.undelegateMessage,
    5 : StakingMessage_StakeMsg.collectRewards,
    0 : StakingMessage_StakeMsg.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'StakingMessage', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Harmony.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2, 3, 4, 5])
    ..aOM<DirectiveCreateValidator>(1, _omitFieldNames ? '' : 'createValidatorMessage', subBuilder: DirectiveCreateValidator.create)
    ..aOM<DirectiveEditValidator>(2, _omitFieldNames ? '' : 'editValidatorMessage', subBuilder: DirectiveEditValidator.create)
    ..aOM<DirectiveDelegate>(3, _omitFieldNames ? '' : 'delegateMessage', subBuilder: DirectiveDelegate.create)
    ..aOM<DirectiveUndelegate>(4, _omitFieldNames ? '' : 'undelegateMessage', subBuilder: DirectiveUndelegate.create)
    ..aOM<DirectiveCollectRewards>(5, _omitFieldNames ? '' : 'collectRewards', subBuilder: DirectiveCollectRewards.create)
    ..a<$core.List<$core.int>>(6, _omitFieldNames ? '' : 'nonce', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(7, _omitFieldNames ? '' : 'gasPrice', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(8, _omitFieldNames ? '' : 'gasLimit', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  StakingMessage clone() => StakingMessage()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  StakingMessage copyWith(void Function(StakingMessage) updates) => super.copyWith((message) => updates(message as StakingMessage)) as StakingMessage;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static StakingMessage create() => StakingMessage._();
  StakingMessage createEmptyInstance() => create();
  static $pb.PbList<StakingMessage> createRepeated() => $pb.PbList<StakingMessage>();
  @$core.pragma('dart2js:noInline')
  static StakingMessage getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<StakingMessage>(create);
  static StakingMessage? _defaultInstance;

  StakingMessage_StakeMsg whichStakeMsg() => _StakingMessage_StakeMsgByTag[$_whichOneof(0)]!;
  void clearStakeMsg() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  DirectiveCreateValidator get createValidatorMessage => $_getN(0);
  @$pb.TagNumber(1)
  set createValidatorMessage(DirectiveCreateValidator v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasCreateValidatorMessage() => $_has(0);
  @$pb.TagNumber(1)
  void clearCreateValidatorMessage() => clearField(1);
  @$pb.TagNumber(1)
  DirectiveCreateValidator ensureCreateValidatorMessage() => $_ensure(0);

  @$pb.TagNumber(2)
  DirectiveEditValidator get editValidatorMessage => $_getN(1);
  @$pb.TagNumber(2)
  set editValidatorMessage(DirectiveEditValidator v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasEditValidatorMessage() => $_has(1);
  @$pb.TagNumber(2)
  void clearEditValidatorMessage() => clearField(2);
  @$pb.TagNumber(2)
  DirectiveEditValidator ensureEditValidatorMessage() => $_ensure(1);

  @$pb.TagNumber(3)
  DirectiveDelegate get delegateMessage => $_getN(2);
  @$pb.TagNumber(3)
  set delegateMessage(DirectiveDelegate v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasDelegateMessage() => $_has(2);
  @$pb.TagNumber(3)
  void clearDelegateMessage() => clearField(3);
  @$pb.TagNumber(3)
  DirectiveDelegate ensureDelegateMessage() => $_ensure(2);

  @$pb.TagNumber(4)
  DirectiveUndelegate get undelegateMessage => $_getN(3);
  @$pb.TagNumber(4)
  set undelegateMessage(DirectiveUndelegate v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasUndelegateMessage() => $_has(3);
  @$pb.TagNumber(4)
  void clearUndelegateMessage() => clearField(4);
  @$pb.TagNumber(4)
  DirectiveUndelegate ensureUndelegateMessage() => $_ensure(3);

  @$pb.TagNumber(5)
  DirectiveCollectRewards get collectRewards => $_getN(4);
  @$pb.TagNumber(5)
  set collectRewards(DirectiveCollectRewards v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasCollectRewards() => $_has(4);
  @$pb.TagNumber(5)
  void clearCollectRewards() => clearField(5);
  @$pb.TagNumber(5)
  DirectiveCollectRewards ensureCollectRewards() => $_ensure(4);

  /// Nonce (uint256, serialized big endian)
  @$pb.TagNumber(6)
  $core.List<$core.int> get nonce => $_getN(5);
  @$pb.TagNumber(6)
  set nonce($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasNonce() => $_has(5);
  @$pb.TagNumber(6)
  void clearNonce() => clearField(6);

  /// Gas price (uint256, serialized big endian)
  @$pb.TagNumber(7)
  $core.List<$core.int> get gasPrice => $_getN(6);
  @$pb.TagNumber(7)
  set gasPrice($core.List<$core.int> v) { $_setBytes(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasGasPrice() => $_has(6);
  @$pb.TagNumber(7)
  void clearGasPrice() => clearField(7);

  /// Gas limit (uint256, serialized big endian)
  @$pb.TagNumber(8)
  $core.List<$core.int> get gasLimit => $_getN(7);
  @$pb.TagNumber(8)
  set gasLimit($core.List<$core.int> v) { $_setBytes(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasGasLimit() => $_has(7);
  @$pb.TagNumber(8)
  void clearGasLimit() => clearField(8);
}

/// Description for a validator
class Description extends $pb.GeneratedMessage {
  factory Description({
    $core.String? name,
    $core.String? identity,
    $core.String? website,
    $core.String? securityContact,
    $core.String? details,
  }) {
    final $result = create();
    if (name != null) {
      $result.name = name;
    }
    if (identity != null) {
      $result.identity = identity;
    }
    if (website != null) {
      $result.website = website;
    }
    if (securityContact != null) {
      $result.securityContact = securityContact;
    }
    if (details != null) {
      $result.details = details;
    }
    return $result;
  }
  Description._() : super();
  factory Description.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Description.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Description', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Harmony.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'name')
    ..aOS(2, _omitFieldNames ? '' : 'identity')
    ..aOS(3, _omitFieldNames ? '' : 'website')
    ..aOS(4, _omitFieldNames ? '' : 'securityContact')
    ..aOS(5, _omitFieldNames ? '' : 'details')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Description clone() => Description()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Description copyWith(void Function(Description) updates) => super.copyWith((message) => updates(message as Description)) as Description;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Description create() => Description._();
  Description createEmptyInstance() => create();
  static $pb.PbList<Description> createRepeated() => $pb.PbList<Description>();
  @$core.pragma('dart2js:noInline')
  static Description getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Description>(create);
  static Description? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get name => $_getSZ(0);
  @$pb.TagNumber(1)
  set name($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasName() => $_has(0);
  @$pb.TagNumber(1)
  void clearName() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get identity => $_getSZ(1);
  @$pb.TagNumber(2)
  set identity($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasIdentity() => $_has(1);
  @$pb.TagNumber(2)
  void clearIdentity() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get website => $_getSZ(2);
  @$pb.TagNumber(3)
  set website($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasWebsite() => $_has(2);
  @$pb.TagNumber(3)
  void clearWebsite() => clearField(3);

  @$pb.TagNumber(4)
  $core.String get securityContact => $_getSZ(3);
  @$pb.TagNumber(4)
  set securityContact($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasSecurityContact() => $_has(3);
  @$pb.TagNumber(4)
  void clearSecurityContact() => clearField(4);

  @$pb.TagNumber(5)
  $core.String get details => $_getSZ(4);
  @$pb.TagNumber(5)
  set details($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasDetails() => $_has(4);
  @$pb.TagNumber(5)
  void clearDetails() => clearField(5);
}

/// A variable precision number
class Decimal extends $pb.GeneratedMessage {
  factory Decimal({
    $core.List<$core.int>? value,
    $core.List<$core.int>? precision,
  }) {
    final $result = create();
    if (value != null) {
      $result.value = value;
    }
    if (precision != null) {
      $result.precision = precision;
    }
    return $result;
  }
  Decimal._() : super();
  factory Decimal.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Decimal.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Decimal', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Harmony.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'precision', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Decimal clone() => Decimal()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Decimal copyWith(void Function(Decimal) updates) => super.copyWith((message) => updates(message as Decimal)) as Decimal;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Decimal create() => Decimal._();
  Decimal createEmptyInstance() => create();
  static $pb.PbList<Decimal> createRepeated() => $pb.PbList<Decimal>();
  @$core.pragma('dart2js:noInline')
  static Decimal getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Decimal>(create);
  static Decimal? _defaultInstance;

  /// The 'raw' value
  @$pb.TagNumber(1)
  $core.List<$core.int> get value => $_getN(0);
  @$pb.TagNumber(1)
  set value($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasValue() => $_has(0);
  @$pb.TagNumber(1)
  void clearValue() => clearField(1);

  /// The precision (number of decimals)
  @$pb.TagNumber(2)
  $core.List<$core.int> get precision => $_getN(1);
  @$pb.TagNumber(2)
  set precision($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPrecision() => $_has(1);
  @$pb.TagNumber(2)
  void clearPrecision() => clearField(2);
}

/// Represents validator commission rule
class CommissionRate extends $pb.GeneratedMessage {
  factory CommissionRate({
    Decimal? rate,
    Decimal? maxRate,
    Decimal? maxChangeRate,
  }) {
    final $result = create();
    if (rate != null) {
      $result.rate = rate;
    }
    if (maxRate != null) {
      $result.maxRate = maxRate;
    }
    if (maxChangeRate != null) {
      $result.maxChangeRate = maxChangeRate;
    }
    return $result;
  }
  CommissionRate._() : super();
  factory CommissionRate.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory CommissionRate.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'CommissionRate', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Harmony.Proto'), createEmptyInstance: create)
    ..aOM<Decimal>(1, _omitFieldNames ? '' : 'rate', subBuilder: Decimal.create)
    ..aOM<Decimal>(2, _omitFieldNames ? '' : 'maxRate', subBuilder: Decimal.create)
    ..aOM<Decimal>(3, _omitFieldNames ? '' : 'maxChangeRate', subBuilder: Decimal.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  CommissionRate clone() => CommissionRate()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  CommissionRate copyWith(void Function(CommissionRate) updates) => super.copyWith((message) => updates(message as CommissionRate)) as CommissionRate;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static CommissionRate create() => CommissionRate._();
  CommissionRate createEmptyInstance() => create();
  static $pb.PbList<CommissionRate> createRepeated() => $pb.PbList<CommissionRate>();
  @$core.pragma('dart2js:noInline')
  static CommissionRate getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<CommissionRate>(create);
  static CommissionRate? _defaultInstance;

  /// The rate
  @$pb.TagNumber(1)
  Decimal get rate => $_getN(0);
  @$pb.TagNumber(1)
  set rate(Decimal v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasRate() => $_has(0);
  @$pb.TagNumber(1)
  void clearRate() => clearField(1);
  @$pb.TagNumber(1)
  Decimal ensureRate() => $_ensure(0);

  /// Maximum rate
  @$pb.TagNumber(2)
  Decimal get maxRate => $_getN(1);
  @$pb.TagNumber(2)
  set maxRate(Decimal v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasMaxRate() => $_has(1);
  @$pb.TagNumber(2)
  void clearMaxRate() => clearField(2);
  @$pb.TagNumber(2)
  Decimal ensureMaxRate() => $_ensure(1);

  /// Maximum of rate change
  @$pb.TagNumber(3)
  Decimal get maxChangeRate => $_getN(2);
  @$pb.TagNumber(3)
  set maxChangeRate(Decimal v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasMaxChangeRate() => $_has(2);
  @$pb.TagNumber(3)
  void clearMaxChangeRate() => clearField(3);
  @$pb.TagNumber(3)
  Decimal ensureMaxChangeRate() => $_ensure(2);
}

/// Create Validator directive
class DirectiveCreateValidator extends $pb.GeneratedMessage {
  factory DirectiveCreateValidator({
    $core.String? validatorAddress,
    Description? description,
    CommissionRate? commissionRates,
    $core.List<$core.int>? minSelfDelegation,
    $core.List<$core.int>? maxTotalDelegation,
    $core.Iterable<$core.List<$core.int>>? slotPubKeys,
    $core.Iterable<$core.List<$core.int>>? slotKeySigs,
    $core.List<$core.int>? amount,
  }) {
    final $result = create();
    if (validatorAddress != null) {
      $result.validatorAddress = validatorAddress;
    }
    if (description != null) {
      $result.description = description;
    }
    if (commissionRates != null) {
      $result.commissionRates = commissionRates;
    }
    if (minSelfDelegation != null) {
      $result.minSelfDelegation = minSelfDelegation;
    }
    if (maxTotalDelegation != null) {
      $result.maxTotalDelegation = maxTotalDelegation;
    }
    if (slotPubKeys != null) {
      $result.slotPubKeys.addAll(slotPubKeys);
    }
    if (slotKeySigs != null) {
      $result.slotKeySigs.addAll(slotKeySigs);
    }
    if (amount != null) {
      $result.amount = amount;
    }
    return $result;
  }
  DirectiveCreateValidator._() : super();
  factory DirectiveCreateValidator.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DirectiveCreateValidator.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DirectiveCreateValidator', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Harmony.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'validatorAddress')
    ..aOM<Description>(2, _omitFieldNames ? '' : 'description', subBuilder: Description.create)
    ..aOM<CommissionRate>(3, _omitFieldNames ? '' : 'commissionRates', subBuilder: CommissionRate.create)
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'minSelfDelegation', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(5, _omitFieldNames ? '' : 'maxTotalDelegation', $pb.PbFieldType.OY)
    ..p<$core.List<$core.int>>(6, _omitFieldNames ? '' : 'slotPubKeys', $pb.PbFieldType.PY)
    ..p<$core.List<$core.int>>(7, _omitFieldNames ? '' : 'slotKeySigs', $pb.PbFieldType.PY)
    ..a<$core.List<$core.int>>(8, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DirectiveCreateValidator clone() => DirectiveCreateValidator()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DirectiveCreateValidator copyWith(void Function(DirectiveCreateValidator) updates) => super.copyWith((message) => updates(message as DirectiveCreateValidator)) as DirectiveCreateValidator;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DirectiveCreateValidator create() => DirectiveCreateValidator._();
  DirectiveCreateValidator createEmptyInstance() => create();
  static $pb.PbList<DirectiveCreateValidator> createRepeated() => $pb.PbList<DirectiveCreateValidator>();
  @$core.pragma('dart2js:noInline')
  static DirectiveCreateValidator getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DirectiveCreateValidator>(create);
  static DirectiveCreateValidator? _defaultInstance;

  /// Address of validator
  @$pb.TagNumber(1)
  $core.String get validatorAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set validatorAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasValidatorAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearValidatorAddress() => clearField(1);

  /// Description, name etc.
  @$pb.TagNumber(2)
  Description get description => $_getN(1);
  @$pb.TagNumber(2)
  set description(Description v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasDescription() => $_has(1);
  @$pb.TagNumber(2)
  void clearDescription() => clearField(2);
  @$pb.TagNumber(2)
  Description ensureDescription() => $_ensure(1);

  /// Rates
  @$pb.TagNumber(3)
  CommissionRate get commissionRates => $_getN(2);
  @$pb.TagNumber(3)
  set commissionRates(CommissionRate v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasCommissionRates() => $_has(2);
  @$pb.TagNumber(3)
  void clearCommissionRates() => clearField(3);
  @$pb.TagNumber(3)
  CommissionRate ensureCommissionRates() => $_ensure(2);

  @$pb.TagNumber(4)
  $core.List<$core.int> get minSelfDelegation => $_getN(3);
  @$pb.TagNumber(4)
  set minSelfDelegation($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasMinSelfDelegation() => $_has(3);
  @$pb.TagNumber(4)
  void clearMinSelfDelegation() => clearField(4);

  @$pb.TagNumber(5)
  $core.List<$core.int> get maxTotalDelegation => $_getN(4);
  @$pb.TagNumber(5)
  set maxTotalDelegation($core.List<$core.int> v) { $_setBytes(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasMaxTotalDelegation() => $_has(4);
  @$pb.TagNumber(5)
  void clearMaxTotalDelegation() => clearField(5);

  @$pb.TagNumber(6)
  $core.List<$core.List<$core.int>> get slotPubKeys => $_getList(5);

  @$pb.TagNumber(7)
  $core.List<$core.List<$core.int>> get slotKeySigs => $_getList(6);

  @$pb.TagNumber(8)
  $core.List<$core.int> get amount => $_getN(7);
  @$pb.TagNumber(8)
  set amount($core.List<$core.int> v) { $_setBytes(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasAmount() => $_has(7);
  @$pb.TagNumber(8)
  void clearAmount() => clearField(8);
}

/// Edit Validator directive
class DirectiveEditValidator extends $pb.GeneratedMessage {
  factory DirectiveEditValidator({
    $core.String? validatorAddress,
    Description? description,
    Decimal? commissionRate,
    $core.List<$core.int>? minSelfDelegation,
    $core.List<$core.int>? maxTotalDelegation,
    $core.List<$core.int>? slotKeyToRemove,
    $core.List<$core.int>? slotKeyToAdd,
    $core.List<$core.int>? slotKeyToAddSig,
    $core.List<$core.int>? active,
  }) {
    final $result = create();
    if (validatorAddress != null) {
      $result.validatorAddress = validatorAddress;
    }
    if (description != null) {
      $result.description = description;
    }
    if (commissionRate != null) {
      $result.commissionRate = commissionRate;
    }
    if (minSelfDelegation != null) {
      $result.minSelfDelegation = minSelfDelegation;
    }
    if (maxTotalDelegation != null) {
      $result.maxTotalDelegation = maxTotalDelegation;
    }
    if (slotKeyToRemove != null) {
      $result.slotKeyToRemove = slotKeyToRemove;
    }
    if (slotKeyToAdd != null) {
      $result.slotKeyToAdd = slotKeyToAdd;
    }
    if (slotKeyToAddSig != null) {
      $result.slotKeyToAddSig = slotKeyToAddSig;
    }
    if (active != null) {
      $result.active = active;
    }
    return $result;
  }
  DirectiveEditValidator._() : super();
  factory DirectiveEditValidator.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DirectiveEditValidator.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DirectiveEditValidator', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Harmony.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'validatorAddress')
    ..aOM<Description>(2, _omitFieldNames ? '' : 'description', subBuilder: Description.create)
    ..aOM<Decimal>(3, _omitFieldNames ? '' : 'commissionRate', subBuilder: Decimal.create)
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'minSelfDelegation', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(5, _omitFieldNames ? '' : 'maxTotalDelegation', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(6, _omitFieldNames ? '' : 'slotKeyToRemove', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(7, _omitFieldNames ? '' : 'slotKeyToAdd', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(8, _omitFieldNames ? '' : 'slotKeyToAddSig', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(9, _omitFieldNames ? '' : 'active', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DirectiveEditValidator clone() => DirectiveEditValidator()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DirectiveEditValidator copyWith(void Function(DirectiveEditValidator) updates) => super.copyWith((message) => updates(message as DirectiveEditValidator)) as DirectiveEditValidator;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DirectiveEditValidator create() => DirectiveEditValidator._();
  DirectiveEditValidator createEmptyInstance() => create();
  static $pb.PbList<DirectiveEditValidator> createRepeated() => $pb.PbList<DirectiveEditValidator>();
  @$core.pragma('dart2js:noInline')
  static DirectiveEditValidator getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DirectiveEditValidator>(create);
  static DirectiveEditValidator? _defaultInstance;

  /// Validator address
  @$pb.TagNumber(1)
  $core.String get validatorAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set validatorAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasValidatorAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearValidatorAddress() => clearField(1);

  @$pb.TagNumber(2)
  Description get description => $_getN(1);
  @$pb.TagNumber(2)
  set description(Description v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasDescription() => $_has(1);
  @$pb.TagNumber(2)
  void clearDescription() => clearField(2);
  @$pb.TagNumber(2)
  Description ensureDescription() => $_ensure(1);

  @$pb.TagNumber(3)
  Decimal get commissionRate => $_getN(2);
  @$pb.TagNumber(3)
  set commissionRate(Decimal v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasCommissionRate() => $_has(2);
  @$pb.TagNumber(3)
  void clearCommissionRate() => clearField(3);
  @$pb.TagNumber(3)
  Decimal ensureCommissionRate() => $_ensure(2);

  @$pb.TagNumber(4)
  $core.List<$core.int> get minSelfDelegation => $_getN(3);
  @$pb.TagNumber(4)
  set minSelfDelegation($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasMinSelfDelegation() => $_has(3);
  @$pb.TagNumber(4)
  void clearMinSelfDelegation() => clearField(4);

  @$pb.TagNumber(5)
  $core.List<$core.int> get maxTotalDelegation => $_getN(4);
  @$pb.TagNumber(5)
  set maxTotalDelegation($core.List<$core.int> v) { $_setBytes(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasMaxTotalDelegation() => $_has(4);
  @$pb.TagNumber(5)
  void clearMaxTotalDelegation() => clearField(5);

  @$pb.TagNumber(6)
  $core.List<$core.int> get slotKeyToRemove => $_getN(5);
  @$pb.TagNumber(6)
  set slotKeyToRemove($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasSlotKeyToRemove() => $_has(5);
  @$pb.TagNumber(6)
  void clearSlotKeyToRemove() => clearField(6);

  @$pb.TagNumber(7)
  $core.List<$core.int> get slotKeyToAdd => $_getN(6);
  @$pb.TagNumber(7)
  set slotKeyToAdd($core.List<$core.int> v) { $_setBytes(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasSlotKeyToAdd() => $_has(6);
  @$pb.TagNumber(7)
  void clearSlotKeyToAdd() => clearField(7);

  @$pb.TagNumber(8)
  $core.List<$core.int> get slotKeyToAddSig => $_getN(7);
  @$pb.TagNumber(8)
  set slotKeyToAddSig($core.List<$core.int> v) { $_setBytes(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasSlotKeyToAddSig() => $_has(7);
  @$pb.TagNumber(8)
  void clearSlotKeyToAddSig() => clearField(8);

  @$pb.TagNumber(9)
  $core.List<$core.int> get active => $_getN(8);
  @$pb.TagNumber(9)
  set active($core.List<$core.int> v) { $_setBytes(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasActive() => $_has(8);
  @$pb.TagNumber(9)
  void clearActive() => clearField(9);
}

/// Delegate directive
class DirectiveDelegate extends $pb.GeneratedMessage {
  factory DirectiveDelegate({
    $core.String? delegatorAddress,
    $core.String? validatorAddress,
    $core.List<$core.int>? amount,
  }) {
    final $result = create();
    if (delegatorAddress != null) {
      $result.delegatorAddress = delegatorAddress;
    }
    if (validatorAddress != null) {
      $result.validatorAddress = validatorAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    return $result;
  }
  DirectiveDelegate._() : super();
  factory DirectiveDelegate.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DirectiveDelegate.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DirectiveDelegate', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Harmony.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'delegatorAddress')
    ..aOS(2, _omitFieldNames ? '' : 'validatorAddress')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DirectiveDelegate clone() => DirectiveDelegate()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DirectiveDelegate copyWith(void Function(DirectiveDelegate) updates) => super.copyWith((message) => updates(message as DirectiveDelegate)) as DirectiveDelegate;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DirectiveDelegate create() => DirectiveDelegate._();
  DirectiveDelegate createEmptyInstance() => create();
  static $pb.PbList<DirectiveDelegate> createRepeated() => $pb.PbList<DirectiveDelegate>();
  @$core.pragma('dart2js:noInline')
  static DirectiveDelegate getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DirectiveDelegate>(create);
  static DirectiveDelegate? _defaultInstance;

  /// Delegator address
  @$pb.TagNumber(1)
  $core.String get delegatorAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set delegatorAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDelegatorAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearDelegatorAddress() => clearField(1);

  /// Validator address
  @$pb.TagNumber(2)
  $core.String get validatorAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set validatorAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValidatorAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearValidatorAddress() => clearField(2);

  /// Delegate amount (uint256, serialized big endian)
  @$pb.TagNumber(3)
  $core.List<$core.int> get amount => $_getN(2);
  @$pb.TagNumber(3)
  set amount($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);
}

/// Undelegate directive
class DirectiveUndelegate extends $pb.GeneratedMessage {
  factory DirectiveUndelegate({
    $core.String? delegatorAddress,
    $core.String? validatorAddress,
    $core.List<$core.int>? amount,
  }) {
    final $result = create();
    if (delegatorAddress != null) {
      $result.delegatorAddress = delegatorAddress;
    }
    if (validatorAddress != null) {
      $result.validatorAddress = validatorAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    return $result;
  }
  DirectiveUndelegate._() : super();
  factory DirectiveUndelegate.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DirectiveUndelegate.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DirectiveUndelegate', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Harmony.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'delegatorAddress')
    ..aOS(2, _omitFieldNames ? '' : 'validatorAddress')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DirectiveUndelegate clone() => DirectiveUndelegate()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DirectiveUndelegate copyWith(void Function(DirectiveUndelegate) updates) => super.copyWith((message) => updates(message as DirectiveUndelegate)) as DirectiveUndelegate;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DirectiveUndelegate create() => DirectiveUndelegate._();
  DirectiveUndelegate createEmptyInstance() => create();
  static $pb.PbList<DirectiveUndelegate> createRepeated() => $pb.PbList<DirectiveUndelegate>();
  @$core.pragma('dart2js:noInline')
  static DirectiveUndelegate getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DirectiveUndelegate>(create);
  static DirectiveUndelegate? _defaultInstance;

  /// Delegator address
  @$pb.TagNumber(1)
  $core.String get delegatorAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set delegatorAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDelegatorAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearDelegatorAddress() => clearField(1);

  /// Validator address
  @$pb.TagNumber(2)
  $core.String get validatorAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set validatorAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValidatorAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearValidatorAddress() => clearField(2);

  /// Undelegate amount (uint256, serialized big endian)
  @$pb.TagNumber(3)
  $core.List<$core.int> get amount => $_getN(2);
  @$pb.TagNumber(3)
  set amount($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);
}

/// Collect reward
class DirectiveCollectRewards extends $pb.GeneratedMessage {
  factory DirectiveCollectRewards({
    $core.String? delegatorAddress,
  }) {
    final $result = create();
    if (delegatorAddress != null) {
      $result.delegatorAddress = delegatorAddress;
    }
    return $result;
  }
  DirectiveCollectRewards._() : super();
  factory DirectiveCollectRewards.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DirectiveCollectRewards.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DirectiveCollectRewards', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Harmony.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'delegatorAddress')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DirectiveCollectRewards clone() => DirectiveCollectRewards()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DirectiveCollectRewards copyWith(void Function(DirectiveCollectRewards) updates) => super.copyWith((message) => updates(message as DirectiveCollectRewards)) as DirectiveCollectRewards;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DirectiveCollectRewards create() => DirectiveCollectRewards._();
  DirectiveCollectRewards createEmptyInstance() => create();
  static $pb.PbList<DirectiveCollectRewards> createRepeated() => $pb.PbList<DirectiveCollectRewards>();
  @$core.pragma('dart2js:noInline')
  static DirectiveCollectRewards getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DirectiveCollectRewards>(create);
  static DirectiveCollectRewards? _defaultInstance;

  /// Delegator address
  @$pb.TagNumber(1)
  $core.String get delegatorAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set delegatorAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDelegatorAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearDelegatorAddress() => clearField(1);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
