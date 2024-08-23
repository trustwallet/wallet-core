//
//  Generated code. Do not modify.
//  source: Cosmos.proto
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
import 'Cosmos.pbenum.dart';

export 'Cosmos.pbenum.dart';

/// A denomination and an amount
class Amount extends $pb.GeneratedMessage {
  factory Amount({
    $core.String? denom,
    $core.String? amount,
  }) {
    final $result = create();
    if (denom != null) {
      $result.denom = denom;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    return $result;
  }
  Amount._() : super();
  factory Amount.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Amount.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Amount', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'denom')
    ..aOS(2, _omitFieldNames ? '' : 'amount')
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

  /// name of the denomination
  @$pb.TagNumber(1)
  $core.String get denom => $_getSZ(0);
  @$pb.TagNumber(1)
  set denom($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDenom() => $_has(0);
  @$pb.TagNumber(1)
  void clearDenom() => clearField(1);

  /// amount, number as string
  @$pb.TagNumber(2)
  $core.String get amount => $_getSZ(1);
  @$pb.TagNumber(2)
  set amount($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);
}

/// Fee incl. gas
class Fee extends $pb.GeneratedMessage {
  factory Fee({
    $core.Iterable<Amount>? amounts,
    $fixnum.Int64? gas,
  }) {
    final $result = create();
    if (amounts != null) {
      $result.amounts.addAll(amounts);
    }
    if (gas != null) {
      $result.gas = gas;
    }
    return $result;
  }
  Fee._() : super();
  factory Fee.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Fee.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Fee', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..pc<Amount>(1, _omitFieldNames ? '' : 'amounts', $pb.PbFieldType.PM, subBuilder: Amount.create)
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'gas', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Fee clone() => Fee()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Fee copyWith(void Function(Fee) updates) => super.copyWith((message) => updates(message as Fee)) as Fee;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Fee create() => Fee._();
  Fee createEmptyInstance() => create();
  static $pb.PbList<Fee> createRepeated() => $pb.PbList<Fee>();
  @$core.pragma('dart2js:noInline')
  static Fee getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Fee>(create);
  static Fee? _defaultInstance;

  /// Fee amount(s)
  @$pb.TagNumber(1)
  $core.List<Amount> get amounts => $_getList(0);

  /// Gas price
  @$pb.TagNumber(2)
  $fixnum.Int64 get gas => $_getI64(1);
  @$pb.TagNumber(2)
  set gas($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasGas() => $_has(1);
  @$pb.TagNumber(2)
  void clearGas() => clearField(2);
}

/// Block height, a revision and block height tuple.
/// A height can be compared against another Height for the purposes of updating and freezing clients
class Height extends $pb.GeneratedMessage {
  factory Height({
    $fixnum.Int64? revisionNumber,
    $fixnum.Int64? revisionHeight,
  }) {
    final $result = create();
    if (revisionNumber != null) {
      $result.revisionNumber = revisionNumber;
    }
    if (revisionHeight != null) {
      $result.revisionHeight = revisionHeight;
    }
    return $result;
  }
  Height._() : super();
  factory Height.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Height.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Height', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'revisionNumber', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'revisionHeight', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Height clone() => Height()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Height copyWith(void Function(Height) updates) => super.copyWith((message) => updates(message as Height)) as Height;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Height create() => Height._();
  Height createEmptyInstance() => create();
  static $pb.PbList<Height> createRepeated() => $pb.PbList<Height>();
  @$core.pragma('dart2js:noInline')
  static Height getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Height>(create);
  static Height? _defaultInstance;

  /// the revision that the client is currently on
  @$pb.TagNumber(1)
  $fixnum.Int64 get revisionNumber => $_getI64(0);
  @$pb.TagNumber(1)
  set revisionNumber($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasRevisionNumber() => $_has(0);
  @$pb.TagNumber(1)
  void clearRevisionNumber() => clearField(1);

  /// the height within the given revision
  @$pb.TagNumber(2)
  $fixnum.Int64 get revisionHeight => $_getI64(1);
  @$pb.TagNumber(2)
  set revisionHeight($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasRevisionHeight() => $_has(1);
  @$pb.TagNumber(2)
  void clearRevisionHeight() => clearField(2);
}

class THORChainAsset extends $pb.GeneratedMessage {
  factory THORChainAsset({
    $core.String? chain,
    $core.String? symbol,
    $core.String? ticker,
    $core.bool? synth,
  }) {
    final $result = create();
    if (chain != null) {
      $result.chain = chain;
    }
    if (symbol != null) {
      $result.symbol = symbol;
    }
    if (ticker != null) {
      $result.ticker = ticker;
    }
    if (synth != null) {
      $result.synth = synth;
    }
    return $result;
  }
  THORChainAsset._() : super();
  factory THORChainAsset.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory THORChainAsset.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'THORChainAsset', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'chain')
    ..aOS(2, _omitFieldNames ? '' : 'symbol')
    ..aOS(3, _omitFieldNames ? '' : 'ticker')
    ..aOB(4, _omitFieldNames ? '' : 'synth')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  THORChainAsset clone() => THORChainAsset()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  THORChainAsset copyWith(void Function(THORChainAsset) updates) => super.copyWith((message) => updates(message as THORChainAsset)) as THORChainAsset;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static THORChainAsset create() => THORChainAsset._();
  THORChainAsset createEmptyInstance() => create();
  static $pb.PbList<THORChainAsset> createRepeated() => $pb.PbList<THORChainAsset>();
  @$core.pragma('dart2js:noInline')
  static THORChainAsset getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<THORChainAsset>(create);
  static THORChainAsset? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get chain => $_getSZ(0);
  @$pb.TagNumber(1)
  set chain($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasChain() => $_has(0);
  @$pb.TagNumber(1)
  void clearChain() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get symbol => $_getSZ(1);
  @$pb.TagNumber(2)
  set symbol($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasSymbol() => $_has(1);
  @$pb.TagNumber(2)
  void clearSymbol() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get ticker => $_getSZ(2);
  @$pb.TagNumber(3)
  set ticker($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTicker() => $_has(2);
  @$pb.TagNumber(3)
  void clearTicker() => clearField(3);

  @$pb.TagNumber(4)
  $core.bool get synth => $_getBF(3);
  @$pb.TagNumber(4)
  set synth($core.bool v) { $_setBool(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasSynth() => $_has(3);
  @$pb.TagNumber(4)
  void clearSynth() => clearField(4);
}

class THORChainCoin extends $pb.GeneratedMessage {
  factory THORChainCoin({
    THORChainAsset? asset,
    $core.String? amount,
    $fixnum.Int64? decimals,
  }) {
    final $result = create();
    if (asset != null) {
      $result.asset = asset;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (decimals != null) {
      $result.decimals = decimals;
    }
    return $result;
  }
  THORChainCoin._() : super();
  factory THORChainCoin.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory THORChainCoin.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'THORChainCoin', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOM<THORChainAsset>(1, _omitFieldNames ? '' : 'asset', subBuilder: THORChainAsset.create)
    ..aOS(2, _omitFieldNames ? '' : 'amount')
    ..aInt64(3, _omitFieldNames ? '' : 'decimals')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  THORChainCoin clone() => THORChainCoin()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  THORChainCoin copyWith(void Function(THORChainCoin) updates) => super.copyWith((message) => updates(message as THORChainCoin)) as THORChainCoin;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static THORChainCoin create() => THORChainCoin._();
  THORChainCoin createEmptyInstance() => create();
  static $pb.PbList<THORChainCoin> createRepeated() => $pb.PbList<THORChainCoin>();
  @$core.pragma('dart2js:noInline')
  static THORChainCoin getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<THORChainCoin>(create);
  static THORChainCoin? _defaultInstance;

  @$pb.TagNumber(1)
  THORChainAsset get asset => $_getN(0);
  @$pb.TagNumber(1)
  set asset(THORChainAsset v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasAsset() => $_has(0);
  @$pb.TagNumber(1)
  void clearAsset() => clearField(1);
  @$pb.TagNumber(1)
  THORChainAsset ensureAsset() => $_ensure(0);

  @$pb.TagNumber(2)
  $core.String get amount => $_getSZ(1);
  @$pb.TagNumber(2)
  set amount($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);

  @$pb.TagNumber(3)
  $fixnum.Int64 get decimals => $_getI64(2);
  @$pb.TagNumber(3)
  set decimals($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasDecimals() => $_has(2);
  @$pb.TagNumber(3)
  void clearDecimals() => clearField(3);
}

/// cosmos-sdk/MsgSend
class Message_Send extends $pb.GeneratedMessage {
  factory Message_Send({
    $core.String? fromAddress,
    $core.String? toAddress,
    $core.Iterable<Amount>? amounts,
    $core.String? typePrefix,
  }) {
    final $result = create();
    if (fromAddress != null) {
      $result.fromAddress = fromAddress;
    }
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (amounts != null) {
      $result.amounts.addAll(amounts);
    }
    if (typePrefix != null) {
      $result.typePrefix = typePrefix;
    }
    return $result;
  }
  Message_Send._() : super();
  factory Message_Send.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_Send.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.Send', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'fromAddress')
    ..aOS(2, _omitFieldNames ? '' : 'toAddress')
    ..pc<Amount>(3, _omitFieldNames ? '' : 'amounts', $pb.PbFieldType.PM, subBuilder: Amount.create)
    ..aOS(4, _omitFieldNames ? '' : 'typePrefix')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_Send clone() => Message_Send()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_Send copyWith(void Function(Message_Send) updates) => super.copyWith((message) => updates(message as Message_Send)) as Message_Send;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_Send create() => Message_Send._();
  Message_Send createEmptyInstance() => create();
  static $pb.PbList<Message_Send> createRepeated() => $pb.PbList<Message_Send>();
  @$core.pragma('dart2js:noInline')
  static Message_Send getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_Send>(create);
  static Message_Send? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get fromAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set fromAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFromAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearFromAddress() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get toAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set toAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasToAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearToAddress() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<Amount> get amounts => $_getList(2);

  @$pb.TagNumber(4)
  $core.String get typePrefix => $_getSZ(3);
  @$pb.TagNumber(4)
  set typePrefix($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasTypePrefix() => $_has(3);
  @$pb.TagNumber(4)
  void clearTypePrefix() => clearField(4);
}

/// cosmos-sdk/MsgTransfer, IBC transfer
class Message_Transfer extends $pb.GeneratedMessage {
  factory Message_Transfer({
    $core.String? sourcePort,
    $core.String? sourceChannel,
    Amount? token,
    $core.String? sender,
    $core.String? receiver,
    Height? timeoutHeight,
    $fixnum.Int64? timeoutTimestamp,
  }) {
    final $result = create();
    if (sourcePort != null) {
      $result.sourcePort = sourcePort;
    }
    if (sourceChannel != null) {
      $result.sourceChannel = sourceChannel;
    }
    if (token != null) {
      $result.token = token;
    }
    if (sender != null) {
      $result.sender = sender;
    }
    if (receiver != null) {
      $result.receiver = receiver;
    }
    if (timeoutHeight != null) {
      $result.timeoutHeight = timeoutHeight;
    }
    if (timeoutTimestamp != null) {
      $result.timeoutTimestamp = timeoutTimestamp;
    }
    return $result;
  }
  Message_Transfer._() : super();
  factory Message_Transfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_Transfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.Transfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'sourcePort')
    ..aOS(2, _omitFieldNames ? '' : 'sourceChannel')
    ..aOM<Amount>(3, _omitFieldNames ? '' : 'token', subBuilder: Amount.create)
    ..aOS(4, _omitFieldNames ? '' : 'sender')
    ..aOS(5, _omitFieldNames ? '' : 'receiver')
    ..aOM<Height>(6, _omitFieldNames ? '' : 'timeoutHeight', subBuilder: Height.create)
    ..a<$fixnum.Int64>(7, _omitFieldNames ? '' : 'timeoutTimestamp', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_Transfer clone() => Message_Transfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_Transfer copyWith(void Function(Message_Transfer) updates) => super.copyWith((message) => updates(message as Message_Transfer)) as Message_Transfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_Transfer create() => Message_Transfer._();
  Message_Transfer createEmptyInstance() => create();
  static $pb.PbList<Message_Transfer> createRepeated() => $pb.PbList<Message_Transfer>();
  @$core.pragma('dart2js:noInline')
  static Message_Transfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_Transfer>(create);
  static Message_Transfer? _defaultInstance;

  /// IBC port, e.g. "transfer"
  @$pb.TagNumber(1)
  $core.String get sourcePort => $_getSZ(0);
  @$pb.TagNumber(1)
  set sourcePort($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSourcePort() => $_has(0);
  @$pb.TagNumber(1)
  void clearSourcePort() => clearField(1);

  /// IBC connection channel, e.g. "channel-141", see apis /ibc/applications/transfer/v1beta1/denom_traces (connections) or /node_info (own channel)
  @$pb.TagNumber(2)
  $core.String get sourceChannel => $_getSZ(1);
  @$pb.TagNumber(2)
  set sourceChannel($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasSourceChannel() => $_has(1);
  @$pb.TagNumber(2)
  void clearSourceChannel() => clearField(2);

  @$pb.TagNumber(3)
  Amount get token => $_getN(2);
  @$pb.TagNumber(3)
  set token(Amount v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasToken() => $_has(2);
  @$pb.TagNumber(3)
  void clearToken() => clearField(3);
  @$pb.TagNumber(3)
  Amount ensureToken() => $_ensure(2);

  @$pb.TagNumber(4)
  $core.String get sender => $_getSZ(3);
  @$pb.TagNumber(4)
  set sender($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasSender() => $_has(3);
  @$pb.TagNumber(4)
  void clearSender() => clearField(4);

  @$pb.TagNumber(5)
  $core.String get receiver => $_getSZ(4);
  @$pb.TagNumber(5)
  set receiver($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasReceiver() => $_has(4);
  @$pb.TagNumber(5)
  void clearReceiver() => clearField(5);

  /// Timeout block height. Either timeout height or timestamp should be set.
  /// Recommendation is to set height, to rev. 1 and block current + 1000 (see api /blocks/latest)
  @$pb.TagNumber(6)
  Height get timeoutHeight => $_getN(5);
  @$pb.TagNumber(6)
  set timeoutHeight(Height v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasTimeoutHeight() => $_has(5);
  @$pb.TagNumber(6)
  void clearTimeoutHeight() => clearField(6);
  @$pb.TagNumber(6)
  Height ensureTimeoutHeight() => $_ensure(5);

  /// Timeout timestamp (in nanoseconds) relative to the current block timestamp.  Either timeout height or timestamp should be set.
  @$pb.TagNumber(7)
  $fixnum.Int64 get timeoutTimestamp => $_getI64(6);
  @$pb.TagNumber(7)
  set timeoutTimestamp($fixnum.Int64 v) { $_setInt64(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasTimeoutTimestamp() => $_has(6);
  @$pb.TagNumber(7)
  void clearTimeoutTimestamp() => clearField(7);
}

/// cosmos-sdk/MsgDelegate to stake
class Message_Delegate extends $pb.GeneratedMessage {
  factory Message_Delegate({
    $core.String? delegatorAddress,
    $core.String? validatorAddress,
    Amount? amount,
    $core.String? typePrefix,
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
    if (typePrefix != null) {
      $result.typePrefix = typePrefix;
    }
    return $result;
  }
  Message_Delegate._() : super();
  factory Message_Delegate.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_Delegate.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.Delegate', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'delegatorAddress')
    ..aOS(2, _omitFieldNames ? '' : 'validatorAddress')
    ..aOM<Amount>(3, _omitFieldNames ? '' : 'amount', subBuilder: Amount.create)
    ..aOS(4, _omitFieldNames ? '' : 'typePrefix')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_Delegate clone() => Message_Delegate()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_Delegate copyWith(void Function(Message_Delegate) updates) => super.copyWith((message) => updates(message as Message_Delegate)) as Message_Delegate;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_Delegate create() => Message_Delegate._();
  Message_Delegate createEmptyInstance() => create();
  static $pb.PbList<Message_Delegate> createRepeated() => $pb.PbList<Message_Delegate>();
  @$core.pragma('dart2js:noInline')
  static Message_Delegate getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_Delegate>(create);
  static Message_Delegate? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get delegatorAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set delegatorAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDelegatorAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearDelegatorAddress() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get validatorAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set validatorAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValidatorAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearValidatorAddress() => clearField(2);

  @$pb.TagNumber(3)
  Amount get amount => $_getN(2);
  @$pb.TagNumber(3)
  set amount(Amount v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);
  @$pb.TagNumber(3)
  Amount ensureAmount() => $_ensure(2);

  @$pb.TagNumber(4)
  $core.String get typePrefix => $_getSZ(3);
  @$pb.TagNumber(4)
  set typePrefix($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasTypePrefix() => $_has(3);
  @$pb.TagNumber(4)
  void clearTypePrefix() => clearField(4);
}

/// cosmos-sdk/MsgUndelegate to unstake
class Message_Undelegate extends $pb.GeneratedMessage {
  factory Message_Undelegate({
    $core.String? delegatorAddress,
    $core.String? validatorAddress,
    Amount? amount,
    $core.String? typePrefix,
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
    if (typePrefix != null) {
      $result.typePrefix = typePrefix;
    }
    return $result;
  }
  Message_Undelegate._() : super();
  factory Message_Undelegate.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_Undelegate.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.Undelegate', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'delegatorAddress')
    ..aOS(2, _omitFieldNames ? '' : 'validatorAddress')
    ..aOM<Amount>(3, _omitFieldNames ? '' : 'amount', subBuilder: Amount.create)
    ..aOS(4, _omitFieldNames ? '' : 'typePrefix')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_Undelegate clone() => Message_Undelegate()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_Undelegate copyWith(void Function(Message_Undelegate) updates) => super.copyWith((message) => updates(message as Message_Undelegate)) as Message_Undelegate;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_Undelegate create() => Message_Undelegate._();
  Message_Undelegate createEmptyInstance() => create();
  static $pb.PbList<Message_Undelegate> createRepeated() => $pb.PbList<Message_Undelegate>();
  @$core.pragma('dart2js:noInline')
  static Message_Undelegate getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_Undelegate>(create);
  static Message_Undelegate? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get delegatorAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set delegatorAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDelegatorAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearDelegatorAddress() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get validatorAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set validatorAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValidatorAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearValidatorAddress() => clearField(2);

  @$pb.TagNumber(3)
  Amount get amount => $_getN(2);
  @$pb.TagNumber(3)
  set amount(Amount v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);
  @$pb.TagNumber(3)
  Amount ensureAmount() => $_ensure(2);

  @$pb.TagNumber(4)
  $core.String get typePrefix => $_getSZ(3);
  @$pb.TagNumber(4)
  set typePrefix($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasTypePrefix() => $_has(3);
  @$pb.TagNumber(4)
  void clearTypePrefix() => clearField(4);
}

/// cosmos-sdk/MsgBeginRedelegate
class Message_BeginRedelegate extends $pb.GeneratedMessage {
  factory Message_BeginRedelegate({
    $core.String? delegatorAddress,
    $core.String? validatorSrcAddress,
    $core.String? validatorDstAddress,
    Amount? amount,
    $core.String? typePrefix,
  }) {
    final $result = create();
    if (delegatorAddress != null) {
      $result.delegatorAddress = delegatorAddress;
    }
    if (validatorSrcAddress != null) {
      $result.validatorSrcAddress = validatorSrcAddress;
    }
    if (validatorDstAddress != null) {
      $result.validatorDstAddress = validatorDstAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (typePrefix != null) {
      $result.typePrefix = typePrefix;
    }
    return $result;
  }
  Message_BeginRedelegate._() : super();
  factory Message_BeginRedelegate.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_BeginRedelegate.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.BeginRedelegate', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'delegatorAddress')
    ..aOS(2, _omitFieldNames ? '' : 'validatorSrcAddress')
    ..aOS(3, _omitFieldNames ? '' : 'validatorDstAddress')
    ..aOM<Amount>(4, _omitFieldNames ? '' : 'amount', subBuilder: Amount.create)
    ..aOS(5, _omitFieldNames ? '' : 'typePrefix')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_BeginRedelegate clone() => Message_BeginRedelegate()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_BeginRedelegate copyWith(void Function(Message_BeginRedelegate) updates) => super.copyWith((message) => updates(message as Message_BeginRedelegate)) as Message_BeginRedelegate;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_BeginRedelegate create() => Message_BeginRedelegate._();
  Message_BeginRedelegate createEmptyInstance() => create();
  static $pb.PbList<Message_BeginRedelegate> createRepeated() => $pb.PbList<Message_BeginRedelegate>();
  @$core.pragma('dart2js:noInline')
  static Message_BeginRedelegate getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_BeginRedelegate>(create);
  static Message_BeginRedelegate? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get delegatorAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set delegatorAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDelegatorAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearDelegatorAddress() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get validatorSrcAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set validatorSrcAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValidatorSrcAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearValidatorSrcAddress() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get validatorDstAddress => $_getSZ(2);
  @$pb.TagNumber(3)
  set validatorDstAddress($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasValidatorDstAddress() => $_has(2);
  @$pb.TagNumber(3)
  void clearValidatorDstAddress() => clearField(3);

  @$pb.TagNumber(4)
  Amount get amount => $_getN(3);
  @$pb.TagNumber(4)
  set amount(Amount v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasAmount() => $_has(3);
  @$pb.TagNumber(4)
  void clearAmount() => clearField(4);
  @$pb.TagNumber(4)
  Amount ensureAmount() => $_ensure(3);

  @$pb.TagNumber(5)
  $core.String get typePrefix => $_getSZ(4);
  @$pb.TagNumber(5)
  set typePrefix($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasTypePrefix() => $_has(4);
  @$pb.TagNumber(5)
  void clearTypePrefix() => clearField(5);
}

/// cosmos-sdk/MsgSetWithdrawAddress
class Message_SetWithdrawAddress extends $pb.GeneratedMessage {
  factory Message_SetWithdrawAddress({
    $core.String? delegatorAddress,
    $core.String? withdrawAddress,
    $core.String? typePrefix,
  }) {
    final $result = create();
    if (delegatorAddress != null) {
      $result.delegatorAddress = delegatorAddress;
    }
    if (withdrawAddress != null) {
      $result.withdrawAddress = withdrawAddress;
    }
    if (typePrefix != null) {
      $result.typePrefix = typePrefix;
    }
    return $result;
  }
  Message_SetWithdrawAddress._() : super();
  factory Message_SetWithdrawAddress.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_SetWithdrawAddress.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.SetWithdrawAddress', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'delegatorAddress')
    ..aOS(2, _omitFieldNames ? '' : 'withdrawAddress')
    ..aOS(3, _omitFieldNames ? '' : 'typePrefix')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_SetWithdrawAddress clone() => Message_SetWithdrawAddress()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_SetWithdrawAddress copyWith(void Function(Message_SetWithdrawAddress) updates) => super.copyWith((message) => updates(message as Message_SetWithdrawAddress)) as Message_SetWithdrawAddress;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_SetWithdrawAddress create() => Message_SetWithdrawAddress._();
  Message_SetWithdrawAddress createEmptyInstance() => create();
  static $pb.PbList<Message_SetWithdrawAddress> createRepeated() => $pb.PbList<Message_SetWithdrawAddress>();
  @$core.pragma('dart2js:noInline')
  static Message_SetWithdrawAddress getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_SetWithdrawAddress>(create);
  static Message_SetWithdrawAddress? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get delegatorAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set delegatorAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDelegatorAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearDelegatorAddress() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get withdrawAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set withdrawAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasWithdrawAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearWithdrawAddress() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get typePrefix => $_getSZ(2);
  @$pb.TagNumber(3)
  set typePrefix($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTypePrefix() => $_has(2);
  @$pb.TagNumber(3)
  void clearTypePrefix() => clearField(3);
}

/// cosmos-sdk/MsgWithdrawDelegationReward
class Message_WithdrawDelegationReward extends $pb.GeneratedMessage {
  factory Message_WithdrawDelegationReward({
    $core.String? delegatorAddress,
    $core.String? validatorAddress,
    $core.String? typePrefix,
  }) {
    final $result = create();
    if (delegatorAddress != null) {
      $result.delegatorAddress = delegatorAddress;
    }
    if (validatorAddress != null) {
      $result.validatorAddress = validatorAddress;
    }
    if (typePrefix != null) {
      $result.typePrefix = typePrefix;
    }
    return $result;
  }
  Message_WithdrawDelegationReward._() : super();
  factory Message_WithdrawDelegationReward.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_WithdrawDelegationReward.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.WithdrawDelegationReward', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'delegatorAddress')
    ..aOS(2, _omitFieldNames ? '' : 'validatorAddress')
    ..aOS(3, _omitFieldNames ? '' : 'typePrefix')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_WithdrawDelegationReward clone() => Message_WithdrawDelegationReward()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_WithdrawDelegationReward copyWith(void Function(Message_WithdrawDelegationReward) updates) => super.copyWith((message) => updates(message as Message_WithdrawDelegationReward)) as Message_WithdrawDelegationReward;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_WithdrawDelegationReward create() => Message_WithdrawDelegationReward._();
  Message_WithdrawDelegationReward createEmptyInstance() => create();
  static $pb.PbList<Message_WithdrawDelegationReward> createRepeated() => $pb.PbList<Message_WithdrawDelegationReward>();
  @$core.pragma('dart2js:noInline')
  static Message_WithdrawDelegationReward getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_WithdrawDelegationReward>(create);
  static Message_WithdrawDelegationReward? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get delegatorAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set delegatorAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDelegatorAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearDelegatorAddress() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get validatorAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set validatorAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValidatorAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearValidatorAddress() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get typePrefix => $_getSZ(2);
  @$pb.TagNumber(3)
  set typePrefix($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTypePrefix() => $_has(2);
  @$pb.TagNumber(3)
  void clearTypePrefix() => clearField(3);
}

/// transfer within wasm/MsgExecuteContract, used by Terra Classic
class Message_WasmTerraExecuteContractTransfer extends $pb.GeneratedMessage {
  factory Message_WasmTerraExecuteContractTransfer({
    $core.String? senderAddress,
    $core.String? contractAddress,
    $core.List<$core.int>? amount,
    $core.String? recipientAddress,
  }) {
    final $result = create();
    if (senderAddress != null) {
      $result.senderAddress = senderAddress;
    }
    if (contractAddress != null) {
      $result.contractAddress = contractAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (recipientAddress != null) {
      $result.recipientAddress = recipientAddress;
    }
    return $result;
  }
  Message_WasmTerraExecuteContractTransfer._() : super();
  factory Message_WasmTerraExecuteContractTransfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_WasmTerraExecuteContractTransfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.WasmTerraExecuteContractTransfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'senderAddress')
    ..aOS(2, _omitFieldNames ? '' : 'contractAddress')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..aOS(4, _omitFieldNames ? '' : 'recipientAddress')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_WasmTerraExecuteContractTransfer clone() => Message_WasmTerraExecuteContractTransfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_WasmTerraExecuteContractTransfer copyWith(void Function(Message_WasmTerraExecuteContractTransfer) updates) => super.copyWith((message) => updates(message as Message_WasmTerraExecuteContractTransfer)) as Message_WasmTerraExecuteContractTransfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_WasmTerraExecuteContractTransfer create() => Message_WasmTerraExecuteContractTransfer._();
  Message_WasmTerraExecuteContractTransfer createEmptyInstance() => create();
  static $pb.PbList<Message_WasmTerraExecuteContractTransfer> createRepeated() => $pb.PbList<Message_WasmTerraExecuteContractTransfer>();
  @$core.pragma('dart2js:noInline')
  static Message_WasmTerraExecuteContractTransfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_WasmTerraExecuteContractTransfer>(create);
  static Message_WasmTerraExecuteContractTransfer? _defaultInstance;

  /// sender address
  @$pb.TagNumber(1)
  $core.String get senderAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set senderAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSenderAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearSenderAddress() => clearField(1);

  /// token contract address
  @$pb.TagNumber(2)
  $core.String get contractAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set contractAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasContractAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearContractAddress() => clearField(2);

  /// size is uint128, as bigint
  @$pb.TagNumber(3)
  $core.List<$core.int> get amount => $_getN(2);
  @$pb.TagNumber(3)
  set amount($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);

  @$pb.TagNumber(4)
  $core.String get recipientAddress => $_getSZ(3);
  @$pb.TagNumber(4)
  set recipientAddress($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasRecipientAddress() => $_has(3);
  @$pb.TagNumber(4)
  void clearRecipientAddress() => clearField(4);
}

/// send within wasm/MsgExecuteContract, used by Terra Classic
class Message_WasmTerraExecuteContractSend extends $pb.GeneratedMessage {
  factory Message_WasmTerraExecuteContractSend({
    $core.String? senderAddress,
    $core.String? contractAddress,
    $core.List<$core.int>? amount,
    $core.String? recipientContractAddress,
    $core.String? msg,
    $core.Iterable<$core.String>? coin,
  }) {
    final $result = create();
    if (senderAddress != null) {
      $result.senderAddress = senderAddress;
    }
    if (contractAddress != null) {
      $result.contractAddress = contractAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (recipientContractAddress != null) {
      $result.recipientContractAddress = recipientContractAddress;
    }
    if (msg != null) {
      $result.msg = msg;
    }
    if (coin != null) {
      $result.coin.addAll(coin);
    }
    return $result;
  }
  Message_WasmTerraExecuteContractSend._() : super();
  factory Message_WasmTerraExecuteContractSend.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_WasmTerraExecuteContractSend.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.WasmTerraExecuteContractSend', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'senderAddress')
    ..aOS(2, _omitFieldNames ? '' : 'contractAddress')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..aOS(4, _omitFieldNames ? '' : 'recipientContractAddress')
    ..aOS(5, _omitFieldNames ? '' : 'msg')
    ..pPS(6, _omitFieldNames ? '' : 'coin')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_WasmTerraExecuteContractSend clone() => Message_WasmTerraExecuteContractSend()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_WasmTerraExecuteContractSend copyWith(void Function(Message_WasmTerraExecuteContractSend) updates) => super.copyWith((message) => updates(message as Message_WasmTerraExecuteContractSend)) as Message_WasmTerraExecuteContractSend;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_WasmTerraExecuteContractSend create() => Message_WasmTerraExecuteContractSend._();
  Message_WasmTerraExecuteContractSend createEmptyInstance() => create();
  static $pb.PbList<Message_WasmTerraExecuteContractSend> createRepeated() => $pb.PbList<Message_WasmTerraExecuteContractSend>();
  @$core.pragma('dart2js:noInline')
  static Message_WasmTerraExecuteContractSend getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_WasmTerraExecuteContractSend>(create);
  static Message_WasmTerraExecuteContractSend? _defaultInstance;

  /// sender address
  @$pb.TagNumber(1)
  $core.String get senderAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set senderAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSenderAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearSenderAddress() => clearField(1);

  /// token contract address
  @$pb.TagNumber(2)
  $core.String get contractAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set contractAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasContractAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearContractAddress() => clearField(2);

  /// size is uint128, as bigint
  @$pb.TagNumber(3)
  $core.List<$core.int> get amount => $_getN(2);
  @$pb.TagNumber(3)
  set amount($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);

  @$pb.TagNumber(4)
  $core.String get recipientContractAddress => $_getSZ(3);
  @$pb.TagNumber(4)
  set recipientContractAddress($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasRecipientContractAddress() => $_has(3);
  @$pb.TagNumber(4)
  void clearRecipientContractAddress() => clearField(4);

  /// execute_msg to be executed in the context of recipient contract
  @$pb.TagNumber(5)
  $core.String get msg => $_getSZ(4);
  @$pb.TagNumber(5)
  set msg($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasMsg() => $_has(4);
  @$pb.TagNumber(5)
  void clearMsg() => clearField(5);

  /// used in case you are sending native tokens along with this message
  @$pb.TagNumber(6)
  $core.List<$core.String> get coin => $_getList(5);
}

/// thorchain/MsgSend
class Message_THORChainSend extends $pb.GeneratedMessage {
  factory Message_THORChainSend({
    $core.List<$core.int>? fromAddress,
    $core.List<$core.int>? toAddress,
    $core.Iterable<Amount>? amounts,
  }) {
    final $result = create();
    if (fromAddress != null) {
      $result.fromAddress = fromAddress;
    }
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (amounts != null) {
      $result.amounts.addAll(amounts);
    }
    return $result;
  }
  Message_THORChainSend._() : super();
  factory Message_THORChainSend.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_THORChainSend.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.THORChainSend', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'fromAddress', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'toAddress', $pb.PbFieldType.OY)
    ..pc<Amount>(3, _omitFieldNames ? '' : 'amounts', $pb.PbFieldType.PM, subBuilder: Amount.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_THORChainSend clone() => Message_THORChainSend()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_THORChainSend copyWith(void Function(Message_THORChainSend) updates) => super.copyWith((message) => updates(message as Message_THORChainSend)) as Message_THORChainSend;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_THORChainSend create() => Message_THORChainSend._();
  Message_THORChainSend createEmptyInstance() => create();
  static $pb.PbList<Message_THORChainSend> createRepeated() => $pb.PbList<Message_THORChainSend>();
  @$core.pragma('dart2js:noInline')
  static Message_THORChainSend getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_THORChainSend>(create);
  static Message_THORChainSend? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.int> get fromAddress => $_getN(0);
  @$pb.TagNumber(1)
  set fromAddress($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFromAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearFromAddress() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<$core.int> get toAddress => $_getN(1);
  @$pb.TagNumber(2)
  set toAddress($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasToAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearToAddress() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<Amount> get amounts => $_getList(2);
}

/// thorchain/MsgDeposit
class Message_THORChainDeposit extends $pb.GeneratedMessage {
  factory Message_THORChainDeposit({
    $core.Iterable<THORChainCoin>? coins,
    $core.String? memo,
    $core.List<$core.int>? signer,
  }) {
    final $result = create();
    if (coins != null) {
      $result.coins.addAll(coins);
    }
    if (memo != null) {
      $result.memo = memo;
    }
    if (signer != null) {
      $result.signer = signer;
    }
    return $result;
  }
  Message_THORChainDeposit._() : super();
  factory Message_THORChainDeposit.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_THORChainDeposit.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.THORChainDeposit', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..pc<THORChainCoin>(1, _omitFieldNames ? '' : 'coins', $pb.PbFieldType.PM, subBuilder: THORChainCoin.create)
    ..aOS(2, _omitFieldNames ? '' : 'memo')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'signer', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_THORChainDeposit clone() => Message_THORChainDeposit()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_THORChainDeposit copyWith(void Function(Message_THORChainDeposit) updates) => super.copyWith((message) => updates(message as Message_THORChainDeposit)) as Message_THORChainDeposit;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_THORChainDeposit create() => Message_THORChainDeposit._();
  Message_THORChainDeposit createEmptyInstance() => create();
  static $pb.PbList<Message_THORChainDeposit> createRepeated() => $pb.PbList<Message_THORChainDeposit>();
  @$core.pragma('dart2js:noInline')
  static Message_THORChainDeposit getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_THORChainDeposit>(create);
  static Message_THORChainDeposit? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<THORChainCoin> get coins => $_getList(0);

  @$pb.TagNumber(2)
  $core.String get memo => $_getSZ(1);
  @$pb.TagNumber(2)
  set memo($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasMemo() => $_has(1);
  @$pb.TagNumber(2)
  void clearMemo() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<$core.int> get signer => $_getN(2);
  @$pb.TagNumber(3)
  set signer($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSigner() => $_has(2);
  @$pb.TagNumber(3)
  void clearSigner() => clearField(3);
}

/// execute within wasm/MsgExecuteContract, used by Terra Classic
class Message_WasmTerraExecuteContractGeneric extends $pb.GeneratedMessage {
  factory Message_WasmTerraExecuteContractGeneric({
    $core.String? senderAddress,
    $core.String? contractAddress,
    $core.String? executeMsg,
    $core.Iterable<Amount>? coins,
  }) {
    final $result = create();
    if (senderAddress != null) {
      $result.senderAddress = senderAddress;
    }
    if (contractAddress != null) {
      $result.contractAddress = contractAddress;
    }
    if (executeMsg != null) {
      $result.executeMsg = executeMsg;
    }
    if (coins != null) {
      $result.coins.addAll(coins);
    }
    return $result;
  }
  Message_WasmTerraExecuteContractGeneric._() : super();
  factory Message_WasmTerraExecuteContractGeneric.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_WasmTerraExecuteContractGeneric.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.WasmTerraExecuteContractGeneric', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'senderAddress')
    ..aOS(2, _omitFieldNames ? '' : 'contractAddress')
    ..aOS(3, _omitFieldNames ? '' : 'executeMsg')
    ..pc<Amount>(5, _omitFieldNames ? '' : 'coins', $pb.PbFieldType.PM, subBuilder: Amount.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_WasmTerraExecuteContractGeneric clone() => Message_WasmTerraExecuteContractGeneric()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_WasmTerraExecuteContractGeneric copyWith(void Function(Message_WasmTerraExecuteContractGeneric) updates) => super.copyWith((message) => updates(message as Message_WasmTerraExecuteContractGeneric)) as Message_WasmTerraExecuteContractGeneric;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_WasmTerraExecuteContractGeneric create() => Message_WasmTerraExecuteContractGeneric._();
  Message_WasmTerraExecuteContractGeneric createEmptyInstance() => create();
  static $pb.PbList<Message_WasmTerraExecuteContractGeneric> createRepeated() => $pb.PbList<Message_WasmTerraExecuteContractGeneric>();
  @$core.pragma('dart2js:noInline')
  static Message_WasmTerraExecuteContractGeneric getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_WasmTerraExecuteContractGeneric>(create);
  static Message_WasmTerraExecuteContractGeneric? _defaultInstance;

  /// sender address
  @$pb.TagNumber(1)
  $core.String get senderAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set senderAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSenderAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearSenderAddress() => clearField(1);

  /// token contract address
  @$pb.TagNumber(2)
  $core.String get contractAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set contractAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasContractAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearContractAddress() => clearField(2);

  /// execute_msg to be executed in the context of recipient contract
  @$pb.TagNumber(3)
  $core.String get executeMsg => $_getSZ(2);
  @$pb.TagNumber(3)
  set executeMsg($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasExecuteMsg() => $_has(2);
  @$pb.TagNumber(3)
  void clearExecuteMsg() => clearField(3);

  /// used in case you are sending native tokens along with this message
  /// Gap in field numbering is intentional
  @$pb.TagNumber(5)
  $core.List<Amount> get coins => $_getList(3);
}

/// transfer within wasm/MsgExecuteContract
class Message_WasmExecuteContractTransfer extends $pb.GeneratedMessage {
  factory Message_WasmExecuteContractTransfer({
    $core.String? senderAddress,
    $core.String? contractAddress,
    $core.List<$core.int>? amount,
    $core.String? recipientAddress,
  }) {
    final $result = create();
    if (senderAddress != null) {
      $result.senderAddress = senderAddress;
    }
    if (contractAddress != null) {
      $result.contractAddress = contractAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (recipientAddress != null) {
      $result.recipientAddress = recipientAddress;
    }
    return $result;
  }
  Message_WasmExecuteContractTransfer._() : super();
  factory Message_WasmExecuteContractTransfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_WasmExecuteContractTransfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.WasmExecuteContractTransfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'senderAddress')
    ..aOS(2, _omitFieldNames ? '' : 'contractAddress')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..aOS(4, _omitFieldNames ? '' : 'recipientAddress')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_WasmExecuteContractTransfer clone() => Message_WasmExecuteContractTransfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_WasmExecuteContractTransfer copyWith(void Function(Message_WasmExecuteContractTransfer) updates) => super.copyWith((message) => updates(message as Message_WasmExecuteContractTransfer)) as Message_WasmExecuteContractTransfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_WasmExecuteContractTransfer create() => Message_WasmExecuteContractTransfer._();
  Message_WasmExecuteContractTransfer createEmptyInstance() => create();
  static $pb.PbList<Message_WasmExecuteContractTransfer> createRepeated() => $pb.PbList<Message_WasmExecuteContractTransfer>();
  @$core.pragma('dart2js:noInline')
  static Message_WasmExecuteContractTransfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_WasmExecuteContractTransfer>(create);
  static Message_WasmExecuteContractTransfer? _defaultInstance;

  /// sender address
  @$pb.TagNumber(1)
  $core.String get senderAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set senderAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSenderAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearSenderAddress() => clearField(1);

  /// token contract address
  @$pb.TagNumber(2)
  $core.String get contractAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set contractAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasContractAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearContractAddress() => clearField(2);

  /// size is uint128, as bigint
  @$pb.TagNumber(3)
  $core.List<$core.int> get amount => $_getN(2);
  @$pb.TagNumber(3)
  set amount($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);

  @$pb.TagNumber(4)
  $core.String get recipientAddress => $_getSZ(3);
  @$pb.TagNumber(4)
  set recipientAddress($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasRecipientAddress() => $_has(3);
  @$pb.TagNumber(4)
  void clearRecipientAddress() => clearField(4);
}

/// send within wasm/MsgExecuteContract
class Message_WasmExecuteContractSend extends $pb.GeneratedMessage {
  factory Message_WasmExecuteContractSend({
    $core.String? senderAddress,
    $core.String? contractAddress,
    $core.List<$core.int>? amount,
    $core.String? recipientContractAddress,
    $core.String? msg,
    $core.Iterable<$core.String>? coin,
  }) {
    final $result = create();
    if (senderAddress != null) {
      $result.senderAddress = senderAddress;
    }
    if (contractAddress != null) {
      $result.contractAddress = contractAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (recipientContractAddress != null) {
      $result.recipientContractAddress = recipientContractAddress;
    }
    if (msg != null) {
      $result.msg = msg;
    }
    if (coin != null) {
      $result.coin.addAll(coin);
    }
    return $result;
  }
  Message_WasmExecuteContractSend._() : super();
  factory Message_WasmExecuteContractSend.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_WasmExecuteContractSend.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.WasmExecuteContractSend', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'senderAddress')
    ..aOS(2, _omitFieldNames ? '' : 'contractAddress')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..aOS(4, _omitFieldNames ? '' : 'recipientContractAddress')
    ..aOS(5, _omitFieldNames ? '' : 'msg')
    ..pPS(6, _omitFieldNames ? '' : 'coin')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_WasmExecuteContractSend clone() => Message_WasmExecuteContractSend()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_WasmExecuteContractSend copyWith(void Function(Message_WasmExecuteContractSend) updates) => super.copyWith((message) => updates(message as Message_WasmExecuteContractSend)) as Message_WasmExecuteContractSend;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_WasmExecuteContractSend create() => Message_WasmExecuteContractSend._();
  Message_WasmExecuteContractSend createEmptyInstance() => create();
  static $pb.PbList<Message_WasmExecuteContractSend> createRepeated() => $pb.PbList<Message_WasmExecuteContractSend>();
  @$core.pragma('dart2js:noInline')
  static Message_WasmExecuteContractSend getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_WasmExecuteContractSend>(create);
  static Message_WasmExecuteContractSend? _defaultInstance;

  /// sender address
  @$pb.TagNumber(1)
  $core.String get senderAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set senderAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSenderAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearSenderAddress() => clearField(1);

  /// token contract address
  @$pb.TagNumber(2)
  $core.String get contractAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set contractAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasContractAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearContractAddress() => clearField(2);

  /// size is uint128, as bigint
  @$pb.TagNumber(3)
  $core.List<$core.int> get amount => $_getN(2);
  @$pb.TagNumber(3)
  set amount($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);

  @$pb.TagNumber(4)
  $core.String get recipientContractAddress => $_getSZ(3);
  @$pb.TagNumber(4)
  set recipientContractAddress($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasRecipientContractAddress() => $_has(3);
  @$pb.TagNumber(4)
  void clearRecipientContractAddress() => clearField(4);

  /// execute_msg to be executed in the context of recipient contract
  @$pb.TagNumber(5)
  $core.String get msg => $_getSZ(4);
  @$pb.TagNumber(5)
  set msg($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasMsg() => $_has(4);
  @$pb.TagNumber(5)
  void clearMsg() => clearField(5);

  /// used in case you are sending native tokens along with this message
  @$pb.TagNumber(6)
  $core.List<$core.String> get coin => $_getList(5);
}

/// execute within wasm/MsgExecuteContract
/// TODO replaces `ExecuteContract`.
class Message_WasmExecuteContractGeneric extends $pb.GeneratedMessage {
  factory Message_WasmExecuteContractGeneric({
    $core.String? senderAddress,
    $core.String? contractAddress,
    $core.String? executeMsg,
    $core.Iterable<Amount>? coins,
  }) {
    final $result = create();
    if (senderAddress != null) {
      $result.senderAddress = senderAddress;
    }
    if (contractAddress != null) {
      $result.contractAddress = contractAddress;
    }
    if (executeMsg != null) {
      $result.executeMsg = executeMsg;
    }
    if (coins != null) {
      $result.coins.addAll(coins);
    }
    return $result;
  }
  Message_WasmExecuteContractGeneric._() : super();
  factory Message_WasmExecuteContractGeneric.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_WasmExecuteContractGeneric.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.WasmExecuteContractGeneric', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'senderAddress')
    ..aOS(2, _omitFieldNames ? '' : 'contractAddress')
    ..aOS(3, _omitFieldNames ? '' : 'executeMsg')
    ..pc<Amount>(5, _omitFieldNames ? '' : 'coins', $pb.PbFieldType.PM, subBuilder: Amount.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_WasmExecuteContractGeneric clone() => Message_WasmExecuteContractGeneric()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_WasmExecuteContractGeneric copyWith(void Function(Message_WasmExecuteContractGeneric) updates) => super.copyWith((message) => updates(message as Message_WasmExecuteContractGeneric)) as Message_WasmExecuteContractGeneric;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_WasmExecuteContractGeneric create() => Message_WasmExecuteContractGeneric._();
  Message_WasmExecuteContractGeneric createEmptyInstance() => create();
  static $pb.PbList<Message_WasmExecuteContractGeneric> createRepeated() => $pb.PbList<Message_WasmExecuteContractGeneric>();
  @$core.pragma('dart2js:noInline')
  static Message_WasmExecuteContractGeneric getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_WasmExecuteContractGeneric>(create);
  static Message_WasmExecuteContractGeneric? _defaultInstance;

  /// sender address
  @$pb.TagNumber(1)
  $core.String get senderAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set senderAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSenderAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearSenderAddress() => clearField(1);

  /// token contract address
  @$pb.TagNumber(2)
  $core.String get contractAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set contractAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasContractAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearContractAddress() => clearField(2);

  /// execute_msg to be executed in the context of recipient contract
  @$pb.TagNumber(3)
  $core.String get executeMsg => $_getSZ(2);
  @$pb.TagNumber(3)
  set executeMsg($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasExecuteMsg() => $_has(2);
  @$pb.TagNumber(3)
  void clearExecuteMsg() => clearField(3);

  /// used in case you are sending native tokens along with this message
  /// Gap in field numbering is intentional
  @$pb.TagNumber(5)
  $core.List<Amount> get coins => $_getList(3);
}

class Message_RawJSON extends $pb.GeneratedMessage {
  factory Message_RawJSON({
    $core.String? type,
    $core.String? value,
  }) {
    final $result = create();
    if (type != null) {
      $result.type = type;
    }
    if (value != null) {
      $result.value = value;
    }
    return $result;
  }
  Message_RawJSON._() : super();
  factory Message_RawJSON.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_RawJSON.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.RawJSON', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'type')
    ..aOS(2, _omitFieldNames ? '' : 'value')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_RawJSON clone() => Message_RawJSON()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_RawJSON copyWith(void Function(Message_RawJSON) updates) => super.copyWith((message) => updates(message as Message_RawJSON)) as Message_RawJSON;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_RawJSON create() => Message_RawJSON._();
  Message_RawJSON createEmptyInstance() => create();
  static $pb.PbList<Message_RawJSON> createRepeated() => $pb.PbList<Message_RawJSON>();
  @$core.pragma('dart2js:noInline')
  static Message_RawJSON getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_RawJSON>(create);
  static Message_RawJSON? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get type => $_getSZ(0);
  @$pb.TagNumber(1)
  set type($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasType() => $_has(0);
  @$pb.TagNumber(1)
  void clearType() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get value => $_getSZ(1);
  @$pb.TagNumber(2)
  set value($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValue() => $_has(1);
  @$pb.TagNumber(2)
  void clearValue() => clearField(2);
}

/// For signing an already serialized transaction. Account number and chain ID must be set outside.
class Message_SignDirect extends $pb.GeneratedMessage {
  factory Message_SignDirect({
    $core.List<$core.int>? bodyBytes,
    $core.List<$core.int>? authInfoBytes,
  }) {
    final $result = create();
    if (bodyBytes != null) {
      $result.bodyBytes = bodyBytes;
    }
    if (authInfoBytes != null) {
      $result.authInfoBytes = authInfoBytes;
    }
    return $result;
  }
  Message_SignDirect._() : super();
  factory Message_SignDirect.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_SignDirect.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.SignDirect', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'bodyBytes', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'authInfoBytes', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_SignDirect clone() => Message_SignDirect()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_SignDirect copyWith(void Function(Message_SignDirect) updates) => super.copyWith((message) => updates(message as Message_SignDirect)) as Message_SignDirect;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_SignDirect create() => Message_SignDirect._();
  Message_SignDirect createEmptyInstance() => create();
  static $pb.PbList<Message_SignDirect> createRepeated() => $pb.PbList<Message_SignDirect>();
  @$core.pragma('dart2js:noInline')
  static Message_SignDirect getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_SignDirect>(create);
  static Message_SignDirect? _defaultInstance;

  /// The prepared serialized TxBody
  @$pb.TagNumber(1)
  $core.List<$core.int> get bodyBytes => $_getN(0);
  @$pb.TagNumber(1)
  set bodyBytes($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasBodyBytes() => $_has(0);
  @$pb.TagNumber(1)
  void clearBodyBytes() => clearField(1);

  /// The prepared serialized AuthInfo
  @$pb.TagNumber(2)
  $core.List<$core.int> get authInfoBytes => $_getN(1);
  @$pb.TagNumber(2)
  set authInfoBytes($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAuthInfoBytes() => $_has(1);
  @$pb.TagNumber(2)
  void clearAuthInfoBytes() => clearField(2);
}

/// Validators defines list of validator addresses.
class Message_StakeAuthorization_Validators extends $pb.GeneratedMessage {
  factory Message_StakeAuthorization_Validators({
    $core.Iterable<$core.String>? address,
  }) {
    final $result = create();
    if (address != null) {
      $result.address.addAll(address);
    }
    return $result;
  }
  Message_StakeAuthorization_Validators._() : super();
  factory Message_StakeAuthorization_Validators.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_StakeAuthorization_Validators.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.StakeAuthorization.Validators', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..pPS(1, _omitFieldNames ? '' : 'address')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_StakeAuthorization_Validators clone() => Message_StakeAuthorization_Validators()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_StakeAuthorization_Validators copyWith(void Function(Message_StakeAuthorization_Validators) updates) => super.copyWith((message) => updates(message as Message_StakeAuthorization_Validators)) as Message_StakeAuthorization_Validators;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_StakeAuthorization_Validators create() => Message_StakeAuthorization_Validators._();
  Message_StakeAuthorization_Validators createEmptyInstance() => create();
  static $pb.PbList<Message_StakeAuthorization_Validators> createRepeated() => $pb.PbList<Message_StakeAuthorization_Validators>();
  @$core.pragma('dart2js:noInline')
  static Message_StakeAuthorization_Validators getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_StakeAuthorization_Validators>(create);
  static Message_StakeAuthorization_Validators? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.String> get address => $_getList(0);
}

enum Message_StakeAuthorization_Validators_ {
  allowList, 
  denyList, 
  notSet
}

///  StakeAuthorization defines authorization for delegate/undelegate/redelegate.
///
///  Since: cosmos-sdk 0.43
class Message_StakeAuthorization extends $pb.GeneratedMessage {
  factory Message_StakeAuthorization({
    Amount? maxTokens,
    Message_StakeAuthorization_Validators? allowList,
    Message_StakeAuthorization_Validators? denyList,
    Message_AuthorizationType? authorizationType,
  }) {
    final $result = create();
    if (maxTokens != null) {
      $result.maxTokens = maxTokens;
    }
    if (allowList != null) {
      $result.allowList = allowList;
    }
    if (denyList != null) {
      $result.denyList = denyList;
    }
    if (authorizationType != null) {
      $result.authorizationType = authorizationType;
    }
    return $result;
  }
  Message_StakeAuthorization._() : super();
  factory Message_StakeAuthorization.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_StakeAuthorization.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Message_StakeAuthorization_Validators_> _Message_StakeAuthorization_Validators_ByTag = {
    2 : Message_StakeAuthorization_Validators_.allowList,
    3 : Message_StakeAuthorization_Validators_.denyList,
    0 : Message_StakeAuthorization_Validators_.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.StakeAuthorization', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..oo(0, [2, 3])
    ..aOM<Amount>(1, _omitFieldNames ? '' : 'maxTokens', subBuilder: Amount.create)
    ..aOM<Message_StakeAuthorization_Validators>(2, _omitFieldNames ? '' : 'allowList', subBuilder: Message_StakeAuthorization_Validators.create)
    ..aOM<Message_StakeAuthorization_Validators>(3, _omitFieldNames ? '' : 'denyList', subBuilder: Message_StakeAuthorization_Validators.create)
    ..e<Message_AuthorizationType>(4, _omitFieldNames ? '' : 'authorizationType', $pb.PbFieldType.OE, defaultOrMaker: Message_AuthorizationType.UNSPECIFIED, valueOf: Message_AuthorizationType.valueOf, enumValues: Message_AuthorizationType.values)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_StakeAuthorization clone() => Message_StakeAuthorization()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_StakeAuthorization copyWith(void Function(Message_StakeAuthorization) updates) => super.copyWith((message) => updates(message as Message_StakeAuthorization)) as Message_StakeAuthorization;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_StakeAuthorization create() => Message_StakeAuthorization._();
  Message_StakeAuthorization createEmptyInstance() => create();
  static $pb.PbList<Message_StakeAuthorization> createRepeated() => $pb.PbList<Message_StakeAuthorization>();
  @$core.pragma('dart2js:noInline')
  static Message_StakeAuthorization getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_StakeAuthorization>(create);
  static Message_StakeAuthorization? _defaultInstance;

  Message_StakeAuthorization_Validators_ whichValidators() => _Message_StakeAuthorization_Validators_ByTag[$_whichOneof(0)]!;
  void clearValidators() => clearField($_whichOneof(0));

  /// max_tokens specifies the maximum amount of tokens can be delegate to a validator. If it is
  /// empty, there is no spend limit and any amount of coins can be delegated.
  @$pb.TagNumber(1)
  Amount get maxTokens => $_getN(0);
  @$pb.TagNumber(1)
  set maxTokens(Amount v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasMaxTokens() => $_has(0);
  @$pb.TagNumber(1)
  void clearMaxTokens() => clearField(1);
  @$pb.TagNumber(1)
  Amount ensureMaxTokens() => $_ensure(0);

  /// allow_list specifies list of validator addresses to whom grantee can delegate tokens on behalf of granter's
  /// account.
  @$pb.TagNumber(2)
  Message_StakeAuthorization_Validators get allowList => $_getN(1);
  @$pb.TagNumber(2)
  set allowList(Message_StakeAuthorization_Validators v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasAllowList() => $_has(1);
  @$pb.TagNumber(2)
  void clearAllowList() => clearField(2);
  @$pb.TagNumber(2)
  Message_StakeAuthorization_Validators ensureAllowList() => $_ensure(1);

  /// deny_list specifies list of validator addresses to whom grantee can not delegate tokens.
  @$pb.TagNumber(3)
  Message_StakeAuthorization_Validators get denyList => $_getN(2);
  @$pb.TagNumber(3)
  set denyList(Message_StakeAuthorization_Validators v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasDenyList() => $_has(2);
  @$pb.TagNumber(3)
  void clearDenyList() => clearField(3);
  @$pb.TagNumber(3)
  Message_StakeAuthorization_Validators ensureDenyList() => $_ensure(2);

  /// authorization_type defines one of AuthorizationType.
  @$pb.TagNumber(4)
  Message_AuthorizationType get authorizationType => $_getN(3);
  @$pb.TagNumber(4)
  set authorizationType(Message_AuthorizationType v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasAuthorizationType() => $_has(3);
  @$pb.TagNumber(4)
  void clearAuthorizationType() => clearField(4);
}

enum Message_AuthGrant_GrantType {
  grantStake, 
  notSet
}

/// cosmos-sdk/MsgGrant
class Message_AuthGrant extends $pb.GeneratedMessage {
  factory Message_AuthGrant({
    $core.String? granter,
    $core.String? grantee,
    Message_StakeAuthorization? grantStake,
    $fixnum.Int64? expiration,
  }) {
    final $result = create();
    if (granter != null) {
      $result.granter = granter;
    }
    if (grantee != null) {
      $result.grantee = grantee;
    }
    if (grantStake != null) {
      $result.grantStake = grantStake;
    }
    if (expiration != null) {
      $result.expiration = expiration;
    }
    return $result;
  }
  Message_AuthGrant._() : super();
  factory Message_AuthGrant.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_AuthGrant.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Message_AuthGrant_GrantType> _Message_AuthGrant_GrantTypeByTag = {
    3 : Message_AuthGrant_GrantType.grantStake,
    0 : Message_AuthGrant_GrantType.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.AuthGrant', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..oo(0, [3])
    ..aOS(1, _omitFieldNames ? '' : 'granter')
    ..aOS(2, _omitFieldNames ? '' : 'grantee')
    ..aOM<Message_StakeAuthorization>(3, _omitFieldNames ? '' : 'grantStake', subBuilder: Message_StakeAuthorization.create)
    ..aInt64(4, _omitFieldNames ? '' : 'expiration')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_AuthGrant clone() => Message_AuthGrant()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_AuthGrant copyWith(void Function(Message_AuthGrant) updates) => super.copyWith((message) => updates(message as Message_AuthGrant)) as Message_AuthGrant;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_AuthGrant create() => Message_AuthGrant._();
  Message_AuthGrant createEmptyInstance() => create();
  static $pb.PbList<Message_AuthGrant> createRepeated() => $pb.PbList<Message_AuthGrant>();
  @$core.pragma('dart2js:noInline')
  static Message_AuthGrant getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_AuthGrant>(create);
  static Message_AuthGrant? _defaultInstance;

  Message_AuthGrant_GrantType whichGrantType() => _Message_AuthGrant_GrantTypeByTag[$_whichOneof(0)]!;
  void clearGrantType() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  $core.String get granter => $_getSZ(0);
  @$pb.TagNumber(1)
  set granter($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasGranter() => $_has(0);
  @$pb.TagNumber(1)
  void clearGranter() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get grantee => $_getSZ(1);
  @$pb.TagNumber(2)
  set grantee($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasGrantee() => $_has(1);
  @$pb.TagNumber(2)
  void clearGrantee() => clearField(2);

  @$pb.TagNumber(3)
  Message_StakeAuthorization get grantStake => $_getN(2);
  @$pb.TagNumber(3)
  set grantStake(Message_StakeAuthorization v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasGrantStake() => $_has(2);
  @$pb.TagNumber(3)
  void clearGrantStake() => clearField(3);
  @$pb.TagNumber(3)
  Message_StakeAuthorization ensureGrantStake() => $_ensure(2);

  @$pb.TagNumber(4)
  $fixnum.Int64 get expiration => $_getI64(3);
  @$pb.TagNumber(4)
  set expiration($fixnum.Int64 v) { $_setInt64(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasExpiration() => $_has(3);
  @$pb.TagNumber(4)
  void clearExpiration() => clearField(4);
}

/// cosmos-sdk/MsgRevoke
class Message_AuthRevoke extends $pb.GeneratedMessage {
  factory Message_AuthRevoke({
    $core.String? granter,
    $core.String? grantee,
    $core.String? msgTypeUrl,
  }) {
    final $result = create();
    if (granter != null) {
      $result.granter = granter;
    }
    if (grantee != null) {
      $result.grantee = grantee;
    }
    if (msgTypeUrl != null) {
      $result.msgTypeUrl = msgTypeUrl;
    }
    return $result;
  }
  Message_AuthRevoke._() : super();
  factory Message_AuthRevoke.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_AuthRevoke.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.AuthRevoke', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'granter')
    ..aOS(2, _omitFieldNames ? '' : 'grantee')
    ..aOS(3, _omitFieldNames ? '' : 'msgTypeUrl')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_AuthRevoke clone() => Message_AuthRevoke()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_AuthRevoke copyWith(void Function(Message_AuthRevoke) updates) => super.copyWith((message) => updates(message as Message_AuthRevoke)) as Message_AuthRevoke;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_AuthRevoke create() => Message_AuthRevoke._();
  Message_AuthRevoke createEmptyInstance() => create();
  static $pb.PbList<Message_AuthRevoke> createRepeated() => $pb.PbList<Message_AuthRevoke>();
  @$core.pragma('dart2js:noInline')
  static Message_AuthRevoke getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_AuthRevoke>(create);
  static Message_AuthRevoke? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get granter => $_getSZ(0);
  @$pb.TagNumber(1)
  set granter($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasGranter() => $_has(0);
  @$pb.TagNumber(1)
  void clearGranter() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get grantee => $_getSZ(1);
  @$pb.TagNumber(2)
  set grantee($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasGrantee() => $_has(1);
  @$pb.TagNumber(2)
  void clearGrantee() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get msgTypeUrl => $_getSZ(2);
  @$pb.TagNumber(3)
  set msgTypeUrl($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasMsgTypeUrl() => $_has(2);
  @$pb.TagNumber(3)
  void clearMsgTypeUrl() => clearField(3);
}

/// cosmos-sdk/MsgVote defines a message to cast a vote.
class Message_MsgVote extends $pb.GeneratedMessage {
  factory Message_MsgVote({
    $fixnum.Int64? proposalId,
    $core.String? voter,
    Message_VoteOption? option,
  }) {
    final $result = create();
    if (proposalId != null) {
      $result.proposalId = proposalId;
    }
    if (voter != null) {
      $result.voter = voter;
    }
    if (option != null) {
      $result.option = option;
    }
    return $result;
  }
  Message_MsgVote._() : super();
  factory Message_MsgVote.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_MsgVote.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.MsgVote', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'proposalId', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(2, _omitFieldNames ? '' : 'voter')
    ..e<Message_VoteOption>(3, _omitFieldNames ? '' : 'option', $pb.PbFieldType.OE, defaultOrMaker: Message_VoteOption.UNSPECIFIED_, valueOf: Message_VoteOption.valueOf, enumValues: Message_VoteOption.values)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_MsgVote clone() => Message_MsgVote()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_MsgVote copyWith(void Function(Message_MsgVote) updates) => super.copyWith((message) => updates(message as Message_MsgVote)) as Message_MsgVote;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_MsgVote create() => Message_MsgVote._();
  Message_MsgVote createEmptyInstance() => create();
  static $pb.PbList<Message_MsgVote> createRepeated() => $pb.PbList<Message_MsgVote>();
  @$core.pragma('dart2js:noInline')
  static Message_MsgVote getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_MsgVote>(create);
  static Message_MsgVote? _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get proposalId => $_getI64(0);
  @$pb.TagNumber(1)
  set proposalId($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasProposalId() => $_has(0);
  @$pb.TagNumber(1)
  void clearProposalId() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get voter => $_getSZ(1);
  @$pb.TagNumber(2)
  set voter($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasVoter() => $_has(1);
  @$pb.TagNumber(2)
  void clearVoter() => clearField(2);

  @$pb.TagNumber(3)
  Message_VoteOption get option => $_getN(2);
  @$pb.TagNumber(3)
  set option(Message_VoteOption v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasOption() => $_has(2);
  @$pb.TagNumber(3)
  void clearOption() => clearField(3);
}

class Message_MsgStrideLiquidStakingStake extends $pb.GeneratedMessage {
  factory Message_MsgStrideLiquidStakingStake({
    $core.String? creator,
    $core.String? amount,
    $core.String? hostDenom,
  }) {
    final $result = create();
    if (creator != null) {
      $result.creator = creator;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (hostDenom != null) {
      $result.hostDenom = hostDenom;
    }
    return $result;
  }
  Message_MsgStrideLiquidStakingStake._() : super();
  factory Message_MsgStrideLiquidStakingStake.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_MsgStrideLiquidStakingStake.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.MsgStrideLiquidStakingStake', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'creator')
    ..aOS(2, _omitFieldNames ? '' : 'amount')
    ..aOS(3, _omitFieldNames ? '' : 'hostDenom')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_MsgStrideLiquidStakingStake clone() => Message_MsgStrideLiquidStakingStake()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_MsgStrideLiquidStakingStake copyWith(void Function(Message_MsgStrideLiquidStakingStake) updates) => super.copyWith((message) => updates(message as Message_MsgStrideLiquidStakingStake)) as Message_MsgStrideLiquidStakingStake;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_MsgStrideLiquidStakingStake create() => Message_MsgStrideLiquidStakingStake._();
  Message_MsgStrideLiquidStakingStake createEmptyInstance() => create();
  static $pb.PbList<Message_MsgStrideLiquidStakingStake> createRepeated() => $pb.PbList<Message_MsgStrideLiquidStakingStake>();
  @$core.pragma('dart2js:noInline')
  static Message_MsgStrideLiquidStakingStake getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_MsgStrideLiquidStakingStake>(create);
  static Message_MsgStrideLiquidStakingStake? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get creator => $_getSZ(0);
  @$pb.TagNumber(1)
  set creator($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasCreator() => $_has(0);
  @$pb.TagNumber(1)
  void clearCreator() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get amount => $_getSZ(1);
  @$pb.TagNumber(2)
  set amount($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get hostDenom => $_getSZ(2);
  @$pb.TagNumber(3)
  set hostDenom($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasHostDenom() => $_has(2);
  @$pb.TagNumber(3)
  void clearHostDenom() => clearField(3);
}

class Message_MsgStrideLiquidStakingRedeem extends $pb.GeneratedMessage {
  factory Message_MsgStrideLiquidStakingRedeem({
    $core.String? creator,
    $core.String? amount,
    $core.String? hostZone,
    $core.String? receiver,
  }) {
    final $result = create();
    if (creator != null) {
      $result.creator = creator;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (hostZone != null) {
      $result.hostZone = hostZone;
    }
    if (receiver != null) {
      $result.receiver = receiver;
    }
    return $result;
  }
  Message_MsgStrideLiquidStakingRedeem._() : super();
  factory Message_MsgStrideLiquidStakingRedeem.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_MsgStrideLiquidStakingRedeem.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.MsgStrideLiquidStakingRedeem', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'creator')
    ..aOS(2, _omitFieldNames ? '' : 'amount')
    ..aOS(3, _omitFieldNames ? '' : 'hostZone')
    ..aOS(4, _omitFieldNames ? '' : 'receiver')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_MsgStrideLiquidStakingRedeem clone() => Message_MsgStrideLiquidStakingRedeem()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_MsgStrideLiquidStakingRedeem copyWith(void Function(Message_MsgStrideLiquidStakingRedeem) updates) => super.copyWith((message) => updates(message as Message_MsgStrideLiquidStakingRedeem)) as Message_MsgStrideLiquidStakingRedeem;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_MsgStrideLiquidStakingRedeem create() => Message_MsgStrideLiquidStakingRedeem._();
  Message_MsgStrideLiquidStakingRedeem createEmptyInstance() => create();
  static $pb.PbList<Message_MsgStrideLiquidStakingRedeem> createRepeated() => $pb.PbList<Message_MsgStrideLiquidStakingRedeem>();
  @$core.pragma('dart2js:noInline')
  static Message_MsgStrideLiquidStakingRedeem getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_MsgStrideLiquidStakingRedeem>(create);
  static Message_MsgStrideLiquidStakingRedeem? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get creator => $_getSZ(0);
  @$pb.TagNumber(1)
  set creator($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasCreator() => $_has(0);
  @$pb.TagNumber(1)
  void clearCreator() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get amount => $_getSZ(1);
  @$pb.TagNumber(2)
  set amount($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get hostZone => $_getSZ(2);
  @$pb.TagNumber(3)
  set hostZone($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasHostZone() => $_has(2);
  @$pb.TagNumber(3)
  void clearHostZone() => clearField(3);

  @$pb.TagNumber(4)
  $core.String get receiver => $_getSZ(3);
  @$pb.TagNumber(4)
  set receiver($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasReceiver() => $_has(3);
  @$pb.TagNumber(4)
  void clearReceiver() => clearField(4);
}

enum Message_MessageOneof {
  sendCoinsMessage, 
  transferTokensMessage, 
  stakeMessage, 
  unstakeMessage, 
  restakeMessage, 
  withdrawStakeRewardMessage, 
  rawJsonMessage, 
  wasmTerraExecuteContractTransferMessage, 
  wasmTerraExecuteContractSendMessage, 
  thorchainSendMessage, 
  wasmTerraExecuteContractGeneric, 
  wasmExecuteContractTransferMessage, 
  wasmExecuteContractSendMessage, 
  wasmExecuteContractGeneric, 
  signDirectMessage, 
  authGrant, 
  authRevoke, 
  setWithdrawAddressMessage, 
  msgVote, 
  msgStrideLiquidStakingStake, 
  msgStrideLiquidStakingRedeem, 
  thorchainDepositMessage, 
  notSet
}

/// A transaction payload message
class Message extends $pb.GeneratedMessage {
  factory Message({
    Message_Send? sendCoinsMessage,
    Message_Transfer? transferTokensMessage,
    Message_Delegate? stakeMessage,
    Message_Undelegate? unstakeMessage,
    Message_BeginRedelegate? restakeMessage,
    Message_WithdrawDelegationReward? withdrawStakeRewardMessage,
    Message_RawJSON? rawJsonMessage,
    Message_WasmTerraExecuteContractTransfer? wasmTerraExecuteContractTransferMessage,
    Message_WasmTerraExecuteContractSend? wasmTerraExecuteContractSendMessage,
    Message_THORChainSend? thorchainSendMessage,
    Message_WasmTerraExecuteContractGeneric? wasmTerraExecuteContractGeneric,
    Message_WasmExecuteContractTransfer? wasmExecuteContractTransferMessage,
    Message_WasmExecuteContractSend? wasmExecuteContractSendMessage,
    Message_WasmExecuteContractGeneric? wasmExecuteContractGeneric,
    Message_SignDirect? signDirectMessage,
    Message_AuthGrant? authGrant,
    Message_AuthRevoke? authRevoke,
    Message_SetWithdrawAddress? setWithdrawAddressMessage,
    Message_MsgVote? msgVote,
    Message_MsgStrideLiquidStakingStake? msgStrideLiquidStakingStake,
    Message_MsgStrideLiquidStakingRedeem? msgStrideLiquidStakingRedeem,
    Message_THORChainDeposit? thorchainDepositMessage,
  }) {
    final $result = create();
    if (sendCoinsMessage != null) {
      $result.sendCoinsMessage = sendCoinsMessage;
    }
    if (transferTokensMessage != null) {
      $result.transferTokensMessage = transferTokensMessage;
    }
    if (stakeMessage != null) {
      $result.stakeMessage = stakeMessage;
    }
    if (unstakeMessage != null) {
      $result.unstakeMessage = unstakeMessage;
    }
    if (restakeMessage != null) {
      $result.restakeMessage = restakeMessage;
    }
    if (withdrawStakeRewardMessage != null) {
      $result.withdrawStakeRewardMessage = withdrawStakeRewardMessage;
    }
    if (rawJsonMessage != null) {
      $result.rawJsonMessage = rawJsonMessage;
    }
    if (wasmTerraExecuteContractTransferMessage != null) {
      $result.wasmTerraExecuteContractTransferMessage = wasmTerraExecuteContractTransferMessage;
    }
    if (wasmTerraExecuteContractSendMessage != null) {
      $result.wasmTerraExecuteContractSendMessage = wasmTerraExecuteContractSendMessage;
    }
    if (thorchainSendMessage != null) {
      $result.thorchainSendMessage = thorchainSendMessage;
    }
    if (wasmTerraExecuteContractGeneric != null) {
      $result.wasmTerraExecuteContractGeneric = wasmTerraExecuteContractGeneric;
    }
    if (wasmExecuteContractTransferMessage != null) {
      $result.wasmExecuteContractTransferMessage = wasmExecuteContractTransferMessage;
    }
    if (wasmExecuteContractSendMessage != null) {
      $result.wasmExecuteContractSendMessage = wasmExecuteContractSendMessage;
    }
    if (wasmExecuteContractGeneric != null) {
      $result.wasmExecuteContractGeneric = wasmExecuteContractGeneric;
    }
    if (signDirectMessage != null) {
      $result.signDirectMessage = signDirectMessage;
    }
    if (authGrant != null) {
      $result.authGrant = authGrant;
    }
    if (authRevoke != null) {
      $result.authRevoke = authRevoke;
    }
    if (setWithdrawAddressMessage != null) {
      $result.setWithdrawAddressMessage = setWithdrawAddressMessage;
    }
    if (msgVote != null) {
      $result.msgVote = msgVote;
    }
    if (msgStrideLiquidStakingStake != null) {
      $result.msgStrideLiquidStakingStake = msgStrideLiquidStakingStake;
    }
    if (msgStrideLiquidStakingRedeem != null) {
      $result.msgStrideLiquidStakingRedeem = msgStrideLiquidStakingRedeem;
    }
    if (thorchainDepositMessage != null) {
      $result.thorchainDepositMessage = thorchainDepositMessage;
    }
    return $result;
  }
  Message._() : super();
  factory Message.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Message_MessageOneof> _Message_MessageOneofByTag = {
    1 : Message_MessageOneof.sendCoinsMessage,
    2 : Message_MessageOneof.transferTokensMessage,
    3 : Message_MessageOneof.stakeMessage,
    4 : Message_MessageOneof.unstakeMessage,
    5 : Message_MessageOneof.restakeMessage,
    6 : Message_MessageOneof.withdrawStakeRewardMessage,
    7 : Message_MessageOneof.rawJsonMessage,
    8 : Message_MessageOneof.wasmTerraExecuteContractTransferMessage,
    9 : Message_MessageOneof.wasmTerraExecuteContractSendMessage,
    10 : Message_MessageOneof.thorchainSendMessage,
    12 : Message_MessageOneof.wasmTerraExecuteContractGeneric,
    13 : Message_MessageOneof.wasmExecuteContractTransferMessage,
    14 : Message_MessageOneof.wasmExecuteContractSendMessage,
    15 : Message_MessageOneof.wasmExecuteContractGeneric,
    16 : Message_MessageOneof.signDirectMessage,
    17 : Message_MessageOneof.authGrant,
    18 : Message_MessageOneof.authRevoke,
    19 : Message_MessageOneof.setWithdrawAddressMessage,
    20 : Message_MessageOneof.msgVote,
    21 : Message_MessageOneof.msgStrideLiquidStakingStake,
    22 : Message_MessageOneof.msgStrideLiquidStakingRedeem,
    23 : Message_MessageOneof.thorchainDepositMessage,
    0 : Message_MessageOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23])
    ..aOM<Message_Send>(1, _omitFieldNames ? '' : 'sendCoinsMessage', subBuilder: Message_Send.create)
    ..aOM<Message_Transfer>(2, _omitFieldNames ? '' : 'transferTokensMessage', subBuilder: Message_Transfer.create)
    ..aOM<Message_Delegate>(3, _omitFieldNames ? '' : 'stakeMessage', subBuilder: Message_Delegate.create)
    ..aOM<Message_Undelegate>(4, _omitFieldNames ? '' : 'unstakeMessage', subBuilder: Message_Undelegate.create)
    ..aOM<Message_BeginRedelegate>(5, _omitFieldNames ? '' : 'restakeMessage', subBuilder: Message_BeginRedelegate.create)
    ..aOM<Message_WithdrawDelegationReward>(6, _omitFieldNames ? '' : 'withdrawStakeRewardMessage', subBuilder: Message_WithdrawDelegationReward.create)
    ..aOM<Message_RawJSON>(7, _omitFieldNames ? '' : 'rawJsonMessage', subBuilder: Message_RawJSON.create)
    ..aOM<Message_WasmTerraExecuteContractTransfer>(8, _omitFieldNames ? '' : 'wasmTerraExecuteContractTransferMessage', subBuilder: Message_WasmTerraExecuteContractTransfer.create)
    ..aOM<Message_WasmTerraExecuteContractSend>(9, _omitFieldNames ? '' : 'wasmTerraExecuteContractSendMessage', subBuilder: Message_WasmTerraExecuteContractSend.create)
    ..aOM<Message_THORChainSend>(10, _omitFieldNames ? '' : 'thorchainSendMessage', subBuilder: Message_THORChainSend.create)
    ..aOM<Message_WasmTerraExecuteContractGeneric>(12, _omitFieldNames ? '' : 'wasmTerraExecuteContractGeneric', subBuilder: Message_WasmTerraExecuteContractGeneric.create)
    ..aOM<Message_WasmExecuteContractTransfer>(13, _omitFieldNames ? '' : 'wasmExecuteContractTransferMessage', subBuilder: Message_WasmExecuteContractTransfer.create)
    ..aOM<Message_WasmExecuteContractSend>(14, _omitFieldNames ? '' : 'wasmExecuteContractSendMessage', subBuilder: Message_WasmExecuteContractSend.create)
    ..aOM<Message_WasmExecuteContractGeneric>(15, _omitFieldNames ? '' : 'wasmExecuteContractGeneric', subBuilder: Message_WasmExecuteContractGeneric.create)
    ..aOM<Message_SignDirect>(16, _omitFieldNames ? '' : 'signDirectMessage', subBuilder: Message_SignDirect.create)
    ..aOM<Message_AuthGrant>(17, _omitFieldNames ? '' : 'authGrant', subBuilder: Message_AuthGrant.create)
    ..aOM<Message_AuthRevoke>(18, _omitFieldNames ? '' : 'authRevoke', subBuilder: Message_AuthRevoke.create)
    ..aOM<Message_SetWithdrawAddress>(19, _omitFieldNames ? '' : 'setWithdrawAddressMessage', subBuilder: Message_SetWithdrawAddress.create)
    ..aOM<Message_MsgVote>(20, _omitFieldNames ? '' : 'msgVote', subBuilder: Message_MsgVote.create)
    ..aOM<Message_MsgStrideLiquidStakingStake>(21, _omitFieldNames ? '' : 'msgStrideLiquidStakingStake', subBuilder: Message_MsgStrideLiquidStakingStake.create)
    ..aOM<Message_MsgStrideLiquidStakingRedeem>(22, _omitFieldNames ? '' : 'msgStrideLiquidStakingRedeem', subBuilder: Message_MsgStrideLiquidStakingRedeem.create)
    ..aOM<Message_THORChainDeposit>(23, _omitFieldNames ? '' : 'thorchainDepositMessage', subBuilder: Message_THORChainDeposit.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message clone() => Message()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message copyWith(void Function(Message) updates) => super.copyWith((message) => updates(message as Message)) as Message;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message create() => Message._();
  Message createEmptyInstance() => create();
  static $pb.PbList<Message> createRepeated() => $pb.PbList<Message>();
  @$core.pragma('dart2js:noInline')
  static Message getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message>(create);
  static Message? _defaultInstance;

  Message_MessageOneof whichMessageOneof() => _Message_MessageOneofByTag[$_whichOneof(0)]!;
  void clearMessageOneof() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  Message_Send get sendCoinsMessage => $_getN(0);
  @$pb.TagNumber(1)
  set sendCoinsMessage(Message_Send v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasSendCoinsMessage() => $_has(0);
  @$pb.TagNumber(1)
  void clearSendCoinsMessage() => clearField(1);
  @$pb.TagNumber(1)
  Message_Send ensureSendCoinsMessage() => $_ensure(0);

  @$pb.TagNumber(2)
  Message_Transfer get transferTokensMessage => $_getN(1);
  @$pb.TagNumber(2)
  set transferTokensMessage(Message_Transfer v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasTransferTokensMessage() => $_has(1);
  @$pb.TagNumber(2)
  void clearTransferTokensMessage() => clearField(2);
  @$pb.TagNumber(2)
  Message_Transfer ensureTransferTokensMessage() => $_ensure(1);

  @$pb.TagNumber(3)
  Message_Delegate get stakeMessage => $_getN(2);
  @$pb.TagNumber(3)
  set stakeMessage(Message_Delegate v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasStakeMessage() => $_has(2);
  @$pb.TagNumber(3)
  void clearStakeMessage() => clearField(3);
  @$pb.TagNumber(3)
  Message_Delegate ensureStakeMessage() => $_ensure(2);

  @$pb.TagNumber(4)
  Message_Undelegate get unstakeMessage => $_getN(3);
  @$pb.TagNumber(4)
  set unstakeMessage(Message_Undelegate v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasUnstakeMessage() => $_has(3);
  @$pb.TagNumber(4)
  void clearUnstakeMessage() => clearField(4);
  @$pb.TagNumber(4)
  Message_Undelegate ensureUnstakeMessage() => $_ensure(3);

  @$pb.TagNumber(5)
  Message_BeginRedelegate get restakeMessage => $_getN(4);
  @$pb.TagNumber(5)
  set restakeMessage(Message_BeginRedelegate v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasRestakeMessage() => $_has(4);
  @$pb.TagNumber(5)
  void clearRestakeMessage() => clearField(5);
  @$pb.TagNumber(5)
  Message_BeginRedelegate ensureRestakeMessage() => $_ensure(4);

  @$pb.TagNumber(6)
  Message_WithdrawDelegationReward get withdrawStakeRewardMessage => $_getN(5);
  @$pb.TagNumber(6)
  set withdrawStakeRewardMessage(Message_WithdrawDelegationReward v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasWithdrawStakeRewardMessage() => $_has(5);
  @$pb.TagNumber(6)
  void clearWithdrawStakeRewardMessage() => clearField(6);
  @$pb.TagNumber(6)
  Message_WithdrawDelegationReward ensureWithdrawStakeRewardMessage() => $_ensure(5);

  @$pb.TagNumber(7)
  Message_RawJSON get rawJsonMessage => $_getN(6);
  @$pb.TagNumber(7)
  set rawJsonMessage(Message_RawJSON v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasRawJsonMessage() => $_has(6);
  @$pb.TagNumber(7)
  void clearRawJsonMessage() => clearField(7);
  @$pb.TagNumber(7)
  Message_RawJSON ensureRawJsonMessage() => $_ensure(6);

  @$pb.TagNumber(8)
  Message_WasmTerraExecuteContractTransfer get wasmTerraExecuteContractTransferMessage => $_getN(7);
  @$pb.TagNumber(8)
  set wasmTerraExecuteContractTransferMessage(Message_WasmTerraExecuteContractTransfer v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasWasmTerraExecuteContractTransferMessage() => $_has(7);
  @$pb.TagNumber(8)
  void clearWasmTerraExecuteContractTransferMessage() => clearField(8);
  @$pb.TagNumber(8)
  Message_WasmTerraExecuteContractTransfer ensureWasmTerraExecuteContractTransferMessage() => $_ensure(7);

  @$pb.TagNumber(9)
  Message_WasmTerraExecuteContractSend get wasmTerraExecuteContractSendMessage => $_getN(8);
  @$pb.TagNumber(9)
  set wasmTerraExecuteContractSendMessage(Message_WasmTerraExecuteContractSend v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasWasmTerraExecuteContractSendMessage() => $_has(8);
  @$pb.TagNumber(9)
  void clearWasmTerraExecuteContractSendMessage() => clearField(9);
  @$pb.TagNumber(9)
  Message_WasmTerraExecuteContractSend ensureWasmTerraExecuteContractSendMessage() => $_ensure(8);

  @$pb.TagNumber(10)
  Message_THORChainSend get thorchainSendMessage => $_getN(9);
  @$pb.TagNumber(10)
  set thorchainSendMessage(Message_THORChainSend v) { setField(10, v); }
  @$pb.TagNumber(10)
  $core.bool hasThorchainSendMessage() => $_has(9);
  @$pb.TagNumber(10)
  void clearThorchainSendMessage() => clearField(10);
  @$pb.TagNumber(10)
  Message_THORChainSend ensureThorchainSendMessage() => $_ensure(9);

  @$pb.TagNumber(12)
  Message_WasmTerraExecuteContractGeneric get wasmTerraExecuteContractGeneric => $_getN(10);
  @$pb.TagNumber(12)
  set wasmTerraExecuteContractGeneric(Message_WasmTerraExecuteContractGeneric v) { setField(12, v); }
  @$pb.TagNumber(12)
  $core.bool hasWasmTerraExecuteContractGeneric() => $_has(10);
  @$pb.TagNumber(12)
  void clearWasmTerraExecuteContractGeneric() => clearField(12);
  @$pb.TagNumber(12)
  Message_WasmTerraExecuteContractGeneric ensureWasmTerraExecuteContractGeneric() => $_ensure(10);

  @$pb.TagNumber(13)
  Message_WasmExecuteContractTransfer get wasmExecuteContractTransferMessage => $_getN(11);
  @$pb.TagNumber(13)
  set wasmExecuteContractTransferMessage(Message_WasmExecuteContractTransfer v) { setField(13, v); }
  @$pb.TagNumber(13)
  $core.bool hasWasmExecuteContractTransferMessage() => $_has(11);
  @$pb.TagNumber(13)
  void clearWasmExecuteContractTransferMessage() => clearField(13);
  @$pb.TagNumber(13)
  Message_WasmExecuteContractTransfer ensureWasmExecuteContractTransferMessage() => $_ensure(11);

  @$pb.TagNumber(14)
  Message_WasmExecuteContractSend get wasmExecuteContractSendMessage => $_getN(12);
  @$pb.TagNumber(14)
  set wasmExecuteContractSendMessage(Message_WasmExecuteContractSend v) { setField(14, v); }
  @$pb.TagNumber(14)
  $core.bool hasWasmExecuteContractSendMessage() => $_has(12);
  @$pb.TagNumber(14)
  void clearWasmExecuteContractSendMessage() => clearField(14);
  @$pb.TagNumber(14)
  Message_WasmExecuteContractSend ensureWasmExecuteContractSendMessage() => $_ensure(12);

  @$pb.TagNumber(15)
  Message_WasmExecuteContractGeneric get wasmExecuteContractGeneric => $_getN(13);
  @$pb.TagNumber(15)
  set wasmExecuteContractGeneric(Message_WasmExecuteContractGeneric v) { setField(15, v); }
  @$pb.TagNumber(15)
  $core.bool hasWasmExecuteContractGeneric() => $_has(13);
  @$pb.TagNumber(15)
  void clearWasmExecuteContractGeneric() => clearField(15);
  @$pb.TagNumber(15)
  Message_WasmExecuteContractGeneric ensureWasmExecuteContractGeneric() => $_ensure(13);

  @$pb.TagNumber(16)
  Message_SignDirect get signDirectMessage => $_getN(14);
  @$pb.TagNumber(16)
  set signDirectMessage(Message_SignDirect v) { setField(16, v); }
  @$pb.TagNumber(16)
  $core.bool hasSignDirectMessage() => $_has(14);
  @$pb.TagNumber(16)
  void clearSignDirectMessage() => clearField(16);
  @$pb.TagNumber(16)
  Message_SignDirect ensureSignDirectMessage() => $_ensure(14);

  @$pb.TagNumber(17)
  Message_AuthGrant get authGrant => $_getN(15);
  @$pb.TagNumber(17)
  set authGrant(Message_AuthGrant v) { setField(17, v); }
  @$pb.TagNumber(17)
  $core.bool hasAuthGrant() => $_has(15);
  @$pb.TagNumber(17)
  void clearAuthGrant() => clearField(17);
  @$pb.TagNumber(17)
  Message_AuthGrant ensureAuthGrant() => $_ensure(15);

  @$pb.TagNumber(18)
  Message_AuthRevoke get authRevoke => $_getN(16);
  @$pb.TagNumber(18)
  set authRevoke(Message_AuthRevoke v) { setField(18, v); }
  @$pb.TagNumber(18)
  $core.bool hasAuthRevoke() => $_has(16);
  @$pb.TagNumber(18)
  void clearAuthRevoke() => clearField(18);
  @$pb.TagNumber(18)
  Message_AuthRevoke ensureAuthRevoke() => $_ensure(16);

  @$pb.TagNumber(19)
  Message_SetWithdrawAddress get setWithdrawAddressMessage => $_getN(17);
  @$pb.TagNumber(19)
  set setWithdrawAddressMessage(Message_SetWithdrawAddress v) { setField(19, v); }
  @$pb.TagNumber(19)
  $core.bool hasSetWithdrawAddressMessage() => $_has(17);
  @$pb.TagNumber(19)
  void clearSetWithdrawAddressMessage() => clearField(19);
  @$pb.TagNumber(19)
  Message_SetWithdrawAddress ensureSetWithdrawAddressMessage() => $_ensure(17);

  @$pb.TagNumber(20)
  Message_MsgVote get msgVote => $_getN(18);
  @$pb.TagNumber(20)
  set msgVote(Message_MsgVote v) { setField(20, v); }
  @$pb.TagNumber(20)
  $core.bool hasMsgVote() => $_has(18);
  @$pb.TagNumber(20)
  void clearMsgVote() => clearField(20);
  @$pb.TagNumber(20)
  Message_MsgVote ensureMsgVote() => $_ensure(18);

  @$pb.TagNumber(21)
  Message_MsgStrideLiquidStakingStake get msgStrideLiquidStakingStake => $_getN(19);
  @$pb.TagNumber(21)
  set msgStrideLiquidStakingStake(Message_MsgStrideLiquidStakingStake v) { setField(21, v); }
  @$pb.TagNumber(21)
  $core.bool hasMsgStrideLiquidStakingStake() => $_has(19);
  @$pb.TagNumber(21)
  void clearMsgStrideLiquidStakingStake() => clearField(21);
  @$pb.TagNumber(21)
  Message_MsgStrideLiquidStakingStake ensureMsgStrideLiquidStakingStake() => $_ensure(19);

  @$pb.TagNumber(22)
  Message_MsgStrideLiquidStakingRedeem get msgStrideLiquidStakingRedeem => $_getN(20);
  @$pb.TagNumber(22)
  set msgStrideLiquidStakingRedeem(Message_MsgStrideLiquidStakingRedeem v) { setField(22, v); }
  @$pb.TagNumber(22)
  $core.bool hasMsgStrideLiquidStakingRedeem() => $_has(20);
  @$pb.TagNumber(22)
  void clearMsgStrideLiquidStakingRedeem() => clearField(22);
  @$pb.TagNumber(22)
  Message_MsgStrideLiquidStakingRedeem ensureMsgStrideLiquidStakingRedeem() => $_ensure(20);

  @$pb.TagNumber(23)
  Message_THORChainDeposit get thorchainDepositMessage => $_getN(21);
  @$pb.TagNumber(23)
  set thorchainDepositMessage(Message_THORChainDeposit v) { setField(23, v); }
  @$pb.TagNumber(23)
  $core.bool hasThorchainDepositMessage() => $_has(21);
  @$pb.TagNumber(23)
  void clearThorchainDepositMessage() => clearField(23);
  @$pb.TagNumber(23)
  Message_THORChainDeposit ensureThorchainDepositMessage() => $_ensure(21);
}

/// Custom Signer info required to sign a transaction and generate a broadcast JSON message.
class SignerInfo extends $pb.GeneratedMessage {
  factory SignerInfo({
    SignerPublicKeyType? publicKeyType,
    $core.String? jsonType,
    $core.String? protobufType,
  }) {
    final $result = create();
    if (publicKeyType != null) {
      $result.publicKeyType = publicKeyType;
    }
    if (jsonType != null) {
      $result.jsonType = jsonType;
    }
    if (protobufType != null) {
      $result.protobufType = protobufType;
    }
    return $result;
  }
  SignerInfo._() : super();
  factory SignerInfo.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SignerInfo.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SignerInfo', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..e<SignerPublicKeyType>(1, _omitFieldNames ? '' : 'publicKeyType', $pb.PbFieldType.OE, defaultOrMaker: SignerPublicKeyType.Secp256k1, valueOf: SignerPublicKeyType.valueOf, enumValues: SignerPublicKeyType.values)
    ..aOS(2, _omitFieldNames ? '' : 'jsonType')
    ..aOS(3, _omitFieldNames ? '' : 'protobufType')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SignerInfo clone() => SignerInfo()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SignerInfo copyWith(void Function(SignerInfo) updates) => super.copyWith((message) => updates(message as SignerInfo)) as SignerInfo;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SignerInfo create() => SignerInfo._();
  SignerInfo createEmptyInstance() => create();
  static $pb.PbList<SignerInfo> createRepeated() => $pb.PbList<SignerInfo>();
  @$core.pragma('dart2js:noInline')
  static SignerInfo getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SignerInfo>(create);
  static SignerInfo? _defaultInstance;

  /// Public key type used to sign a transaction.
  /// It can be different from the value from `registry.json`.
  @$pb.TagNumber(1)
  SignerPublicKeyType get publicKeyType => $_getN(0);
  @$pb.TagNumber(1)
  set publicKeyType(SignerPublicKeyType v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasPublicKeyType() => $_has(0);
  @$pb.TagNumber(1)
  void clearPublicKeyType() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get jsonType => $_getSZ(1);
  @$pb.TagNumber(2)
  set jsonType($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasJsonType() => $_has(1);
  @$pb.TagNumber(2)
  void clearJsonType() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get protobufType => $_getSZ(2);
  @$pb.TagNumber(3)
  set protobufType($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasProtobufType() => $_has(2);
  @$pb.TagNumber(3)
  void clearProtobufType() => clearField(3);
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    SigningMode? signingMode,
    $fixnum.Int64? accountNumber,
    $core.String? chainId,
    Fee? fee,
    $core.String? memo,
    $fixnum.Int64? sequence,
    $core.List<$core.int>? privateKey,
    $core.Iterable<Message>? messages,
    BroadcastMode? mode,
    $core.List<$core.int>? publicKey,
    TxHasher? txHasher,
    SignerInfo? signerInfo,
    $fixnum.Int64? timeoutHeight,
  }) {
    final $result = create();
    if (signingMode != null) {
      $result.signingMode = signingMode;
    }
    if (accountNumber != null) {
      $result.accountNumber = accountNumber;
    }
    if (chainId != null) {
      $result.chainId = chainId;
    }
    if (fee != null) {
      $result.fee = fee;
    }
    if (memo != null) {
      $result.memo = memo;
    }
    if (sequence != null) {
      $result.sequence = sequence;
    }
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (messages != null) {
      $result.messages.addAll(messages);
    }
    if (mode != null) {
      $result.mode = mode;
    }
    if (publicKey != null) {
      $result.publicKey = publicKey;
    }
    if (txHasher != null) {
      $result.txHasher = txHasher;
    }
    if (signerInfo != null) {
      $result.signerInfo = signerInfo;
    }
    if (timeoutHeight != null) {
      $result.timeoutHeight = timeoutHeight;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..e<SigningMode>(1, _omitFieldNames ? '' : 'signingMode', $pb.PbFieldType.OE, defaultOrMaker: SigningMode.JSON, valueOf: SigningMode.valueOf, enumValues: SigningMode.values)
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'accountNumber', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(3, _omitFieldNames ? '' : 'chainId')
    ..aOM<Fee>(4, _omitFieldNames ? '' : 'fee', subBuilder: Fee.create)
    ..aOS(5, _omitFieldNames ? '' : 'memo')
    ..a<$fixnum.Int64>(6, _omitFieldNames ? '' : 'sequence', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.List<$core.int>>(7, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..pc<Message>(8, _omitFieldNames ? '' : 'messages', $pb.PbFieldType.PM, subBuilder: Message.create)
    ..e<BroadcastMode>(9, _omitFieldNames ? '' : 'mode', $pb.PbFieldType.OE, defaultOrMaker: BroadcastMode.BLOCK, valueOf: BroadcastMode.valueOf, enumValues: BroadcastMode.values)
    ..a<$core.List<$core.int>>(10, _omitFieldNames ? '' : 'publicKey', $pb.PbFieldType.OY)
    ..e<TxHasher>(11, _omitFieldNames ? '' : 'txHasher', $pb.PbFieldType.OE, defaultOrMaker: TxHasher.UseDefault, valueOf: TxHasher.valueOf, enumValues: TxHasher.values)
    ..aOM<SignerInfo>(12, _omitFieldNames ? '' : 'signerInfo', subBuilder: SignerInfo.create)
    ..a<$fixnum.Int64>(13, _omitFieldNames ? '' : 'timeoutHeight', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
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

  /// Specify if protobuf (a.k.a. Stargate) or earlier JSON serialization is used
  @$pb.TagNumber(1)
  SigningMode get signingMode => $_getN(0);
  @$pb.TagNumber(1)
  set signingMode(SigningMode v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasSigningMode() => $_has(0);
  @$pb.TagNumber(1)
  void clearSigningMode() => clearField(1);

  /// Source account number
  @$pb.TagNumber(2)
  $fixnum.Int64 get accountNumber => $_getI64(1);
  @$pb.TagNumber(2)
  set accountNumber($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAccountNumber() => $_has(1);
  @$pb.TagNumber(2)
  void clearAccountNumber() => clearField(2);

  /// Chain ID (string)
  @$pb.TagNumber(3)
  $core.String get chainId => $_getSZ(2);
  @$pb.TagNumber(3)
  set chainId($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasChainId() => $_has(2);
  @$pb.TagNumber(3)
  void clearChainId() => clearField(3);

  /// Transaction fee
  @$pb.TagNumber(4)
  Fee get fee => $_getN(3);
  @$pb.TagNumber(4)
  set fee(Fee v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasFee() => $_has(3);
  @$pb.TagNumber(4)
  void clearFee() => clearField(4);
  @$pb.TagNumber(4)
  Fee ensureFee() => $_ensure(3);

  /// Optional memo
  @$pb.TagNumber(5)
  $core.String get memo => $_getSZ(4);
  @$pb.TagNumber(5)
  set memo($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasMemo() => $_has(4);
  @$pb.TagNumber(5)
  void clearMemo() => clearField(5);

  /// Sequence number (account specific)
  @$pb.TagNumber(6)
  $fixnum.Int64 get sequence => $_getI64(5);
  @$pb.TagNumber(6)
  set sequence($fixnum.Int64 v) { $_setInt64(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasSequence() => $_has(5);
  @$pb.TagNumber(6)
  void clearSequence() => clearField(6);

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(7)
  $core.List<$core.int> get privateKey => $_getN(6);
  @$pb.TagNumber(7)
  set privateKey($core.List<$core.int> v) { $_setBytes(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasPrivateKey() => $_has(6);
  @$pb.TagNumber(7)
  void clearPrivateKey() => clearField(7);

  /// Payload message(s)
  @$pb.TagNumber(8)
  $core.List<Message> get messages => $_getList(7);

  /// Broadcast mode (included in output, relevant when broadcasting)
  @$pb.TagNumber(9)
  BroadcastMode get mode => $_getN(8);
  @$pb.TagNumber(9)
  set mode(BroadcastMode v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasMode() => $_has(8);
  @$pb.TagNumber(9)
  void clearMode() => clearField(9);

  @$pb.TagNumber(10)
  $core.List<$core.int> get publicKey => $_getN(9);
  @$pb.TagNumber(10)
  set publicKey($core.List<$core.int> v) { $_setBytes(9, v); }
  @$pb.TagNumber(10)
  $core.bool hasPublicKey() => $_has(9);
  @$pb.TagNumber(10)
  void clearPublicKey() => clearField(10);

  @$pb.TagNumber(11)
  TxHasher get txHasher => $_getN(10);
  @$pb.TagNumber(11)
  set txHasher(TxHasher v) { setField(11, v); }
  @$pb.TagNumber(11)
  $core.bool hasTxHasher() => $_has(10);
  @$pb.TagNumber(11)
  void clearTxHasher() => clearField(11);

  /// Optional. If set, use a different Signer info when signing the transaction.
  @$pb.TagNumber(12)
  SignerInfo get signerInfo => $_getN(11);
  @$pb.TagNumber(12)
  set signerInfo(SignerInfo v) { setField(12, v); }
  @$pb.TagNumber(12)
  $core.bool hasSignerInfo() => $_has(11);
  @$pb.TagNumber(12)
  void clearSignerInfo() => clearField(12);
  @$pb.TagNumber(12)
  SignerInfo ensureSignerInfo() => $_ensure(11);

  /// Optional timeout_height
  @$pb.TagNumber(13)
  $fixnum.Int64 get timeoutHeight => $_getI64(12);
  @$pb.TagNumber(13)
  set timeoutHeight($fixnum.Int64 v) { $_setInt64(12, v); }
  @$pb.TagNumber(13)
  $core.bool hasTimeoutHeight() => $_has(12);
  @$pb.TagNumber(13)
  void clearTimeoutHeight() => clearField(13);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.List<$core.int>? signature,
    $core.String? json,
    $core.String? serialized,
    $core.String? signatureJson,
    $core.String? errorMessage,
    $0.SigningError? error,
  }) {
    final $result = create();
    if (signature != null) {
      $result.signature = signature;
    }
    if (json != null) {
      $result.json = json;
    }
    if (serialized != null) {
      $result.serialized = serialized;
    }
    if (signatureJson != null) {
      $result.signatureJson = signatureJson;
    }
    if (errorMessage != null) {
      $result.errorMessage = errorMessage;
    }
    if (error != null) {
      $result.error = error;
    }
    return $result;
  }
  SigningOutput._() : super();
  factory SigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Cosmos.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'signature', $pb.PbFieldType.OY)
    ..aOS(2, _omitFieldNames ? '' : 'json')
    ..aOS(3, _omitFieldNames ? '' : 'serialized')
    ..aOS(4, _omitFieldNames ? '' : 'signatureJson')
    ..aOS(5, _omitFieldNames ? '' : 'errorMessage')
    ..e<$0.SigningError>(6, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
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

  /// Signature
  @$pb.TagNumber(1)
  $core.List<$core.int> get signature => $_getN(0);
  @$pb.TagNumber(1)
  set signature($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSignature() => $_has(0);
  @$pb.TagNumber(1)
  void clearSignature() => clearField(1);

  /// Signed transaction in JSON (pre-Stargate case)
  @$pb.TagNumber(2)
  $core.String get json => $_getSZ(1);
  @$pb.TagNumber(2)
  set json($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasJson() => $_has(1);
  @$pb.TagNumber(2)
  void clearJson() => clearField(2);

  /// Signed transaction containing protobuf encoded, Base64-encoded form (Stargate case),
  /// wrapped in a ready-to-broadcast json.
  @$pb.TagNumber(3)
  $core.String get serialized => $_getSZ(2);
  @$pb.TagNumber(3)
  set serialized($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSerialized() => $_has(2);
  @$pb.TagNumber(3)
  void clearSerialized() => clearField(3);

  /// signatures array json string
  @$pb.TagNumber(4)
  $core.String get signatureJson => $_getSZ(3);
  @$pb.TagNumber(4)
  set signatureJson($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasSignatureJson() => $_has(3);
  @$pb.TagNumber(4)
  void clearSignatureJson() => clearField(4);

  /// error description
  @$pb.TagNumber(5)
  $core.String get errorMessage => $_getSZ(4);
  @$pb.TagNumber(5)
  set errorMessage($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasErrorMessage() => $_has(4);
  @$pb.TagNumber(5)
  void clearErrorMessage() => clearField(5);

  @$pb.TagNumber(6)
  $0.SigningError get error => $_getN(5);
  @$pb.TagNumber(6)
  set error($0.SigningError v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasError() => $_has(5);
  @$pb.TagNumber(6)
  void clearError() => clearField(6);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
