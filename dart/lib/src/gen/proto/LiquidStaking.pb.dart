//
//  Generated code. Do not modify.
//  source: LiquidStaking.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

import 'Aptos.pb.dart' as $5;
import 'Cosmos.pb.dart' as $4;
import 'Ethereum.pb.dart' as $3;
import 'LiquidStaking.pbenum.dart';

export 'LiquidStaking.pbenum.dart';

/// Message to represent the status of an operation
class Status extends $pb.GeneratedMessage {
  factory Status({
    StatusCode? code,
    $core.String? message,
  }) {
    final $result = create();
    if (code != null) {
      $result.code = code;
    }
    if (message != null) {
      $result.message = message;
    }
    return $result;
  }
  Status._() : super();
  factory Status.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Status.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Status', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.LiquidStaking.Proto'), createEmptyInstance: create)
    ..e<StatusCode>(1, _omitFieldNames ? '' : 'code', $pb.PbFieldType.OE, defaultOrMaker: StatusCode.OK, valueOf: StatusCode.valueOf, enumValues: StatusCode.values)
    ..aOS(2, _omitFieldNames ? '' : 'message')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Status clone() => Status()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Status copyWith(void Function(Status) updates) => super.copyWith((message) => updates(message as Status)) as Status;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Status create() => Status._();
  Status createEmptyInstance() => create();
  static $pb.PbList<Status> createRepeated() => $pb.PbList<Status>();
  @$core.pragma('dart2js:noInline')
  static Status getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Status>(create);
  static Status? _defaultInstance;

  /// Status code of the operation
  @$pb.TagNumber(1)
  StatusCode get code => $_getN(0);
  @$pb.TagNumber(1)
  set code(StatusCode v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasCode() => $_has(0);
  @$pb.TagNumber(1)
  void clearCode() => clearField(1);

  /// Optional error message, populated in case of error
  @$pb.TagNumber(2)
  $core.String get message => $_getSZ(1);
  @$pb.TagNumber(2)
  set message($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasMessage() => $_has(1);
  @$pb.TagNumber(2)
  void clearMessage() => clearField(2);
}

/// Message to represent the asset for staking operations
class Asset extends $pb.GeneratedMessage {
  factory Asset({
    Coin? stakingToken,
    $core.String? liquidToken,
    $core.String? denom,
    $core.String? fromAddress,
  }) {
    final $result = create();
    if (stakingToken != null) {
      $result.stakingToken = stakingToken;
    }
    if (liquidToken != null) {
      $result.liquidToken = liquidToken;
    }
    if (denom != null) {
      $result.denom = denom;
    }
    if (fromAddress != null) {
      $result.fromAddress = fromAddress;
    }
    return $result;
  }
  Asset._() : super();
  factory Asset.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Asset.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Asset', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.LiquidStaking.Proto'), createEmptyInstance: create)
    ..e<Coin>(1, _omitFieldNames ? '' : 'stakingToken', $pb.PbFieldType.OE, defaultOrMaker: Coin.MATIC, valueOf: Coin.valueOf, enumValues: Coin.values)
    ..aOS(2, _omitFieldNames ? '' : 'liquidToken')
    ..aOS(3, _omitFieldNames ? '' : 'denom')
    ..aOS(4, _omitFieldNames ? '' : 'fromAddress')
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

  /// Coin to be staked
  @$pb.TagNumber(1)
  Coin get stakingToken => $_getN(0);
  @$pb.TagNumber(1)
  set stakingToken(Coin v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasStakingToken() => $_has(0);
  @$pb.TagNumber(1)
  void clearStakingToken() => clearField(1);

  /// Optional, liquid_token to be manipulated: unstake, claim rewards
  @$pb.TagNumber(2)
  $core.String get liquidToken => $_getSZ(1);
  @$pb.TagNumber(2)
  set liquidToken($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasLiquidToken() => $_has(1);
  @$pb.TagNumber(2)
  void clearLiquidToken() => clearField(2);

  /// Denom of the asset to be manipulated, required by some liquid staking protocols
  @$pb.TagNumber(3)
  $core.String get denom => $_getSZ(2);
  @$pb.TagNumber(3)
  set denom($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasDenom() => $_has(2);
  @$pb.TagNumber(3)
  void clearDenom() => clearField(3);

  /// Address for building the appropriate input
  @$pb.TagNumber(4)
  $core.String get fromAddress => $_getSZ(3);
  @$pb.TagNumber(4)
  set fromAddress($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasFromAddress() => $_has(3);
  @$pb.TagNumber(4)
  void clearFromAddress() => clearField(4);
}

/// Message to represent a stake operation
class Stake extends $pb.GeneratedMessage {
  factory Stake({
    Asset? asset,
    $core.String? amount,
  }) {
    final $result = create();
    if (asset != null) {
      $result.asset = asset;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    return $result;
  }
  Stake._() : super();
  factory Stake.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Stake.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Stake', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.LiquidStaking.Proto'), createEmptyInstance: create)
    ..aOM<Asset>(1, _omitFieldNames ? '' : 'asset', subBuilder: Asset.create)
    ..aOS(2, _omitFieldNames ? '' : 'amount')
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

  @$pb.TagNumber(2)
  $core.String get amount => $_getSZ(1);
  @$pb.TagNumber(2)
  set amount($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);
}

/// Message to represent an unstake operation
class Unstake extends $pb.GeneratedMessage {
  factory Unstake({
    Asset? asset,
    $core.String? amount,
    $core.String? receiverAddress,
    $core.String? receiverChainId,
  }) {
    final $result = create();
    if (asset != null) {
      $result.asset = asset;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (receiverAddress != null) {
      $result.receiverAddress = receiverAddress;
    }
    if (receiverChainId != null) {
      $result.receiverChainId = receiverChainId;
    }
    return $result;
  }
  Unstake._() : super();
  factory Unstake.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Unstake.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Unstake', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.LiquidStaking.Proto'), createEmptyInstance: create)
    ..aOM<Asset>(1, _omitFieldNames ? '' : 'asset', subBuilder: Asset.create)
    ..aOS(2, _omitFieldNames ? '' : 'amount')
    ..aOS(3, _omitFieldNames ? '' : 'receiverAddress')
    ..aOS(4, _omitFieldNames ? '' : 'receiverChainId')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Unstake clone() => Unstake()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Unstake copyWith(void Function(Unstake) updates) => super.copyWith((message) => updates(message as Unstake)) as Unstake;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Unstake create() => Unstake._();
  Unstake createEmptyInstance() => create();
  static $pb.PbList<Unstake> createRepeated() => $pb.PbList<Unstake>();
  @$core.pragma('dart2js:noInline')
  static Unstake getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Unstake>(create);
  static Unstake? _defaultInstance;

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

  @$pb.TagNumber(2)
  $core.String get amount => $_getSZ(1);
  @$pb.TagNumber(2)
  set amount($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);

  /// Some cross-chain protocols propose u to setup a receiver_address
  @$pb.TagNumber(3)
  $core.String get receiverAddress => $_getSZ(2);
  @$pb.TagNumber(3)
  set receiverAddress($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasReceiverAddress() => $_has(2);
  @$pb.TagNumber(3)
  void clearReceiverAddress() => clearField(3);

  /// Some cross-chain protocols propose u to set the receiver chain_id, it allows auto-claim after probation period
  @$pb.TagNumber(4)
  $core.String get receiverChainId => $_getSZ(3);
  @$pb.TagNumber(4)
  set receiverChainId($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasReceiverChainId() => $_has(3);
  @$pb.TagNumber(4)
  void clearReceiverChainId() => clearField(4);
}

/// Message to represent a withdraw operation
class Withdraw extends $pb.GeneratedMessage {
  factory Withdraw({
    Asset? asset,
    $core.String? amount,
    $core.String? idx,
  }) {
    final $result = create();
    if (asset != null) {
      $result.asset = asset;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (idx != null) {
      $result.idx = idx;
    }
    return $result;
  }
  Withdraw._() : super();
  factory Withdraw.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Withdraw.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Withdraw', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.LiquidStaking.Proto'), createEmptyInstance: create)
    ..aOM<Asset>(1, _omitFieldNames ? '' : 'asset', subBuilder: Asset.create)
    ..aOS(2, _omitFieldNames ? '' : 'amount')
    ..aOS(3, _omitFieldNames ? '' : 'idx')
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

  @$pb.TagNumber(2)
  $core.String get amount => $_getSZ(1);
  @$pb.TagNumber(2)
  set amount($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);

  /// Sometimes withdraw is just the index of a request, amount is already known by the SC
  @$pb.TagNumber(3)
  $core.String get idx => $_getSZ(2);
  @$pb.TagNumber(3)
  set idx($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasIdx() => $_has(2);
  @$pb.TagNumber(3)
  void clearIdx() => clearField(3);
}

enum Input_Action {
  stake, 
  unstake, 
  withdraw, 
  notSet
}

/// Message to represent the input for a liquid staking operation
class Input extends $pb.GeneratedMessage {
  factory Input({
    Stake? stake,
    Unstake? unstake,
    Withdraw? withdraw,
    $core.String? smartContractAddress,
    Protocol? protocol,
    Blockchain? blockchain,
  }) {
    final $result = create();
    if (stake != null) {
      $result.stake = stake;
    }
    if (unstake != null) {
      $result.unstake = unstake;
    }
    if (withdraw != null) {
      $result.withdraw = withdraw;
    }
    if (smartContractAddress != null) {
      $result.smartContractAddress = smartContractAddress;
    }
    if (protocol != null) {
      $result.protocol = protocol;
    }
    if (blockchain != null) {
      $result.blockchain = blockchain;
    }
    return $result;
  }
  Input._() : super();
  factory Input.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Input.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Input_Action> _Input_ActionByTag = {
    1 : Input_Action.stake,
    2 : Input_Action.unstake,
    3 : Input_Action.withdraw,
    0 : Input_Action.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Input', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.LiquidStaking.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2, 3])
    ..aOM<Stake>(1, _omitFieldNames ? '' : 'stake', subBuilder: Stake.create)
    ..aOM<Unstake>(2, _omitFieldNames ? '' : 'unstake', subBuilder: Unstake.create)
    ..aOM<Withdraw>(3, _omitFieldNames ? '' : 'withdraw', subBuilder: Withdraw.create)
    ..aOS(4, _omitFieldNames ? '' : 'smartContractAddress')
    ..e<Protocol>(5, _omitFieldNames ? '' : 'protocol', $pb.PbFieldType.OE, defaultOrMaker: Protocol.Strader, valueOf: Protocol.valueOf, enumValues: Protocol.values)
    ..e<Blockchain>(6, _omitFieldNames ? '' : 'blockchain', $pb.PbFieldType.OE, defaultOrMaker: Blockchain.ETHEREUM, valueOf: Blockchain.valueOf, enumValues: Blockchain.values)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Input clone() => Input()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Input copyWith(void Function(Input) updates) => super.copyWith((message) => updates(message as Input)) as Input;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Input create() => Input._();
  Input createEmptyInstance() => create();
  static $pb.PbList<Input> createRepeated() => $pb.PbList<Input>();
  @$core.pragma('dart2js:noInline')
  static Input getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Input>(create);
  static Input? _defaultInstance;

  Input_Action whichAction() => _Input_ActionByTag[$_whichOneof(0)]!;
  void clearAction() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  Stake get stake => $_getN(0);
  @$pb.TagNumber(1)
  set stake(Stake v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasStake() => $_has(0);
  @$pb.TagNumber(1)
  void clearStake() => clearField(1);
  @$pb.TagNumber(1)
  Stake ensureStake() => $_ensure(0);

  @$pb.TagNumber(2)
  Unstake get unstake => $_getN(1);
  @$pb.TagNumber(2)
  set unstake(Unstake v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasUnstake() => $_has(1);
  @$pb.TagNumber(2)
  void clearUnstake() => clearField(2);
  @$pb.TagNumber(2)
  Unstake ensureUnstake() => $_ensure(1);

  @$pb.TagNumber(3)
  Withdraw get withdraw => $_getN(2);
  @$pb.TagNumber(3)
  set withdraw(Withdraw v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasWithdraw() => $_has(2);
  @$pb.TagNumber(3)
  void clearWithdraw() => clearField(3);
  @$pb.TagNumber(3)
  Withdraw ensureWithdraw() => $_ensure(2);

  /// Optional smart contract address for EVM-based chains
  @$pb.TagNumber(4)
  $core.String get smartContractAddress => $_getSZ(3);
  @$pb.TagNumber(4)
  set smartContractAddress($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasSmartContractAddress() => $_has(3);
  @$pb.TagNumber(4)
  void clearSmartContractAddress() => clearField(4);

  /// Protocol to be used for liquid staking
  @$pb.TagNumber(5)
  Protocol get protocol => $_getN(4);
  @$pb.TagNumber(5)
  set protocol(Protocol v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasProtocol() => $_has(4);
  @$pb.TagNumber(5)
  void clearProtocol() => clearField(5);

  /// Target blockchain for the liquid staking operation
  @$pb.TagNumber(6)
  Blockchain get blockchain => $_getN(5);
  @$pb.TagNumber(6)
  set blockchain(Blockchain v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasBlockchain() => $_has(5);
  @$pb.TagNumber(6)
  void clearBlockchain() => clearField(6);
}

enum Output_SigningInputOneof {
  ethereum, 
  cosmos, 
  aptos, 
  notSet
}

/// Message to represent the output of a liquid staking operation
class Output extends $pb.GeneratedMessage {
  factory Output({
    Status? status,
    $3.SigningInput? ethereum,
    $4.SigningInput? cosmos,
    $5.SigningInput? aptos,
  }) {
    final $result = create();
    if (status != null) {
      $result.status = status;
    }
    if (ethereum != null) {
      $result.ethereum = ethereum;
    }
    if (cosmos != null) {
      $result.cosmos = cosmos;
    }
    if (aptos != null) {
      $result.aptos = aptos;
    }
    return $result;
  }
  Output._() : super();
  factory Output.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Output.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Output_SigningInputOneof> _Output_SigningInputOneofByTag = {
    2 : Output_SigningInputOneof.ethereum,
    3 : Output_SigningInputOneof.cosmos,
    4 : Output_SigningInputOneof.aptos,
    0 : Output_SigningInputOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Output', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.LiquidStaking.Proto'), createEmptyInstance: create)
    ..oo(0, [2, 3, 4])
    ..aOM<Status>(1, _omitFieldNames ? '' : 'status', subBuilder: Status.create)
    ..aOM<$3.SigningInput>(2, _omitFieldNames ? '' : 'ethereum', subBuilder: $3.SigningInput.create)
    ..aOM<$4.SigningInput>(3, _omitFieldNames ? '' : 'cosmos', subBuilder: $4.SigningInput.create)
    ..aOM<$5.SigningInput>(4, _omitFieldNames ? '' : 'aptos', subBuilder: $5.SigningInput.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Output clone() => Output()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Output copyWith(void Function(Output) updates) => super.copyWith((message) => updates(message as Output)) as Output;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Output create() => Output._();
  Output createEmptyInstance() => create();
  static $pb.PbList<Output> createRepeated() => $pb.PbList<Output>();
  @$core.pragma('dart2js:noInline')
  static Output getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Output>(create);
  static Output? _defaultInstance;

  Output_SigningInputOneof whichSigningInputOneof() => _Output_SigningInputOneofByTag[$_whichOneof(0)]!;
  void clearSigningInputOneof() => clearField($_whichOneof(0));

  /// Status of the liquid staking operation
  @$pb.TagNumber(1)
  Status get status => $_getN(0);
  @$pb.TagNumber(1)
  set status(Status v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasStatus() => $_has(0);
  @$pb.TagNumber(1)
  void clearStatus() => clearField(1);
  @$pb.TagNumber(1)
  Status ensureStatus() => $_ensure(0);

  @$pb.TagNumber(2)
  $3.SigningInput get ethereum => $_getN(1);
  @$pb.TagNumber(2)
  set ethereum($3.SigningInput v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasEthereum() => $_has(1);
  @$pb.TagNumber(2)
  void clearEthereum() => clearField(2);
  @$pb.TagNumber(2)
  $3.SigningInput ensureEthereum() => $_ensure(1);

  @$pb.TagNumber(3)
  $4.SigningInput get cosmos => $_getN(2);
  @$pb.TagNumber(3)
  set cosmos($4.SigningInput v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasCosmos() => $_has(2);
  @$pb.TagNumber(3)
  void clearCosmos() => clearField(3);
  @$pb.TagNumber(3)
  $4.SigningInput ensureCosmos() => $_ensure(2);

  @$pb.TagNumber(4)
  $5.SigningInput get aptos => $_getN(3);
  @$pb.TagNumber(4)
  set aptos($5.SigningInput v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasAptos() => $_has(3);
  @$pb.TagNumber(4)
  void clearAptos() => clearField(4);
  @$pb.TagNumber(4)
  $5.SigningInput ensureAptos() => $_ensure(3);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
