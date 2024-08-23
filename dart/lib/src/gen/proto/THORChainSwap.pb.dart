//
//  Generated code. Do not modify.
//  source: THORChainSwap.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'Binance.pb.dart' as $6;
import 'Bitcoin.pb.dart' as $2;
import 'Cosmos.pb.dart' as $4;
import 'Ethereum.pb.dart' as $3;
import 'THORChainSwap.pbenum.dart';

export 'THORChainSwap.pbenum.dart';

/// An error code + a free text
class Error extends $pb.GeneratedMessage {
  factory Error({
    ErrorCode? code,
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
  Error._() : super();
  factory Error.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Error.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Error', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.THORChainSwap.Proto'), createEmptyInstance: create)
    ..e<ErrorCode>(1, _omitFieldNames ? '' : 'code', $pb.PbFieldType.OE, defaultOrMaker: ErrorCode.OK, valueOf: ErrorCode.valueOf, enumValues: ErrorCode.values)
    ..aOS(2, _omitFieldNames ? '' : 'message')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Error clone() => Error()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Error copyWith(void Function(Error) updates) => super.copyWith((message) => updates(message as Error)) as Error;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Error create() => Error._();
  Error createEmptyInstance() => create();
  static $pb.PbList<Error> createRepeated() => $pb.PbList<Error>();
  @$core.pragma('dart2js:noInline')
  static Error getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Error>(create);
  static Error? _defaultInstance;

  /// code of the error
  @$pb.TagNumber(1)
  ErrorCode get code => $_getN(0);
  @$pb.TagNumber(1)
  set code(ErrorCode v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasCode() => $_has(0);
  @$pb.TagNumber(1)
  void clearCode() => clearField(1);

  /// optional error message
  @$pb.TagNumber(2)
  $core.String get message => $_getSZ(1);
  @$pb.TagNumber(2)
  set message($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasMessage() => $_has(1);
  @$pb.TagNumber(2)
  void clearMessage() => clearField(2);
}

/// Represents an asset.  Examples: BNB.BNB, RUNE.RUNE, BNB.RUNE-67C
class Asset extends $pb.GeneratedMessage {
  factory Asset({
    Chain? chain,
    $core.String? symbol,
    $core.String? tokenId,
  }) {
    final $result = create();
    if (chain != null) {
      $result.chain = chain;
    }
    if (symbol != null) {
      $result.symbol = symbol;
    }
    if (tokenId != null) {
      $result.tokenId = tokenId;
    }
    return $result;
  }
  Asset._() : super();
  factory Asset.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Asset.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Asset', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.THORChainSwap.Proto'), createEmptyInstance: create)
    ..e<Chain>(1, _omitFieldNames ? '' : 'chain', $pb.PbFieldType.OE, defaultOrMaker: Chain.THOR, valueOf: Chain.valueOf, enumValues: Chain.values)
    ..aOS(2, _omitFieldNames ? '' : 'symbol')
    ..aOS(3, _omitFieldNames ? '' : 'tokenId')
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

  /// Chain ID
  @$pb.TagNumber(1)
  Chain get chain => $_getN(0);
  @$pb.TagNumber(1)
  set chain(Chain v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasChain() => $_has(0);
  @$pb.TagNumber(1)
  void clearChain() => clearField(1);

  /// Symbol
  @$pb.TagNumber(2)
  $core.String get symbol => $_getSZ(1);
  @$pb.TagNumber(2)
  set symbol($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasSymbol() => $_has(1);
  @$pb.TagNumber(2)
  void clearSymbol() => clearField(2);

  /// The ID of the token (blockchain-specific format)
  @$pb.TagNumber(3)
  $core.String get tokenId => $_getSZ(2);
  @$pb.TagNumber(3)
  set tokenId($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTokenId() => $_has(2);
  @$pb.TagNumber(3)
  void clearTokenId() => clearField(3);
}

class StreamParams extends $pb.GeneratedMessage {
  factory StreamParams({
    $core.String? interval,
    $core.String? quantity,
  }) {
    final $result = create();
    if (interval != null) {
      $result.interval = interval;
    }
    if (quantity != null) {
      $result.quantity = quantity;
    }
    return $result;
  }
  StreamParams._() : super();
  factory StreamParams.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory StreamParams.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'StreamParams', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.THORChainSwap.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'interval')
    ..aOS(2, _omitFieldNames ? '' : 'quantity')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  StreamParams clone() => StreamParams()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  StreamParams copyWith(void Function(StreamParams) updates) => super.copyWith((message) => updates(message as StreamParams)) as StreamParams;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static StreamParams create() => StreamParams._();
  StreamParams createEmptyInstance() => create();
  static $pb.PbList<StreamParams> createRepeated() => $pb.PbList<StreamParams>();
  @$core.pragma('dart2js:noInline')
  static StreamParams getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<StreamParams>(create);
  static StreamParams? _defaultInstance;

  /// Optional Swap Interval ncy in blocks.
  /// The default is 1 - time-optimised means getting the trade done quickly, regardless of the cost.
  @$pb.TagNumber(1)
  $core.String get interval => $_getSZ(0);
  @$pb.TagNumber(1)
  set interval($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasInterval() => $_has(0);
  @$pb.TagNumber(1)
  void clearInterval() => clearField(1);

  /// Optional Swap Quantity. Swap interval times every Interval blocks.
  /// The default is 0 - network will determine the number of swaps.
  @$pb.TagNumber(2)
  $core.String get quantity => $_getSZ(1);
  @$pb.TagNumber(2)
  set quantity($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasQuantity() => $_has(1);
  @$pb.TagNumber(2)
  void clearQuantity() => clearField(2);
}

/// Input for a swap between source and destination chains; for creating a TX on the source chain.
class SwapInput extends $pb.GeneratedMessage {
  factory SwapInput({
    Asset? fromAsset,
    $core.String? fromAddress,
    Asset? toAsset,
    $core.String? toAddress,
    $core.String? vaultAddress,
    $core.String? routerAddress,
    $core.String? fromAmount,
    $core.String? toAmountLimit,
    $core.String? affiliateFeeAddress,
    $core.String? affiliateFeeRateBp,
    $core.String? extraMemo,
    $fixnum.Int64? expirationTime,
    StreamParams? streamParams,
  }) {
    final $result = create();
    if (fromAsset != null) {
      $result.fromAsset = fromAsset;
    }
    if (fromAddress != null) {
      $result.fromAddress = fromAddress;
    }
    if (toAsset != null) {
      $result.toAsset = toAsset;
    }
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (vaultAddress != null) {
      $result.vaultAddress = vaultAddress;
    }
    if (routerAddress != null) {
      $result.routerAddress = routerAddress;
    }
    if (fromAmount != null) {
      $result.fromAmount = fromAmount;
    }
    if (toAmountLimit != null) {
      $result.toAmountLimit = toAmountLimit;
    }
    if (affiliateFeeAddress != null) {
      $result.affiliateFeeAddress = affiliateFeeAddress;
    }
    if (affiliateFeeRateBp != null) {
      $result.affiliateFeeRateBp = affiliateFeeRateBp;
    }
    if (extraMemo != null) {
      $result.extraMemo = extraMemo;
    }
    if (expirationTime != null) {
      $result.expirationTime = expirationTime;
    }
    if (streamParams != null) {
      $result.streamParams = streamParams;
    }
    return $result;
  }
  SwapInput._() : super();
  factory SwapInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SwapInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SwapInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.THORChainSwap.Proto'), createEmptyInstance: create)
    ..aOM<Asset>(1, _omitFieldNames ? '' : 'fromAsset', subBuilder: Asset.create)
    ..aOS(2, _omitFieldNames ? '' : 'fromAddress')
    ..aOM<Asset>(3, _omitFieldNames ? '' : 'toAsset', subBuilder: Asset.create)
    ..aOS(4, _omitFieldNames ? '' : 'toAddress')
    ..aOS(5, _omitFieldNames ? '' : 'vaultAddress')
    ..aOS(6, _omitFieldNames ? '' : 'routerAddress')
    ..aOS(7, _omitFieldNames ? '' : 'fromAmount')
    ..aOS(8, _omitFieldNames ? '' : 'toAmountLimit')
    ..aOS(9, _omitFieldNames ? '' : 'affiliateFeeAddress')
    ..aOS(10, _omitFieldNames ? '' : 'affiliateFeeRateBp')
    ..aOS(11, _omitFieldNames ? '' : 'extraMemo')
    ..a<$fixnum.Int64>(12, _omitFieldNames ? '' : 'expirationTime', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOM<StreamParams>(13, _omitFieldNames ? '' : 'streamParams', subBuilder: StreamParams.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SwapInput clone() => SwapInput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SwapInput copyWith(void Function(SwapInput) updates) => super.copyWith((message) => updates(message as SwapInput)) as SwapInput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SwapInput create() => SwapInput._();
  SwapInput createEmptyInstance() => create();
  static $pb.PbList<SwapInput> createRepeated() => $pb.PbList<SwapInput>();
  @$core.pragma('dart2js:noInline')
  static SwapInput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SwapInput>(create);
  static SwapInput? _defaultInstance;

  /// Source chain
  @$pb.TagNumber(1)
  Asset get fromAsset => $_getN(0);
  @$pb.TagNumber(1)
  set fromAsset(Asset v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasFromAsset() => $_has(0);
  @$pb.TagNumber(1)
  void clearFromAsset() => clearField(1);
  @$pb.TagNumber(1)
  Asset ensureFromAsset() => $_ensure(0);

  /// Source address, on source chain
  @$pb.TagNumber(2)
  $core.String get fromAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set fromAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasFromAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearFromAddress() => clearField(2);

  /// Destination chain+asset, on destination chain
  @$pb.TagNumber(3)
  Asset get toAsset => $_getN(2);
  @$pb.TagNumber(3)
  set toAsset(Asset v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasToAsset() => $_has(2);
  @$pb.TagNumber(3)
  void clearToAsset() => clearField(3);
  @$pb.TagNumber(3)
  Asset ensureToAsset() => $_ensure(2);

  /// Destination address, on destination chain
  @$pb.TagNumber(4)
  $core.String get toAddress => $_getSZ(3);
  @$pb.TagNumber(4)
  set toAddress($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasToAddress() => $_has(3);
  @$pb.TagNumber(4)
  void clearToAddress() => clearField(4);

  /// ThorChainSwap vault, on the source chain. Should be queried afresh, as it may change
  @$pb.TagNumber(5)
  $core.String get vaultAddress => $_getSZ(4);
  @$pb.TagNumber(5)
  set vaultAddress($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasVaultAddress() => $_has(4);
  @$pb.TagNumber(5)
  void clearVaultAddress() => clearField(5);

  /// ThorChain router, only in case of Ethereum source network
  @$pb.TagNumber(6)
  $core.String get routerAddress => $_getSZ(5);
  @$pb.TagNumber(6)
  set routerAddress($core.String v) { $_setString(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasRouterAddress() => $_has(5);
  @$pb.TagNumber(6)
  void clearRouterAddress() => clearField(6);

  /// The source amount, integer as string, in the smallest native unit of the chain
  @$pb.TagNumber(7)
  $core.String get fromAmount => $_getSZ(6);
  @$pb.TagNumber(7)
  set fromAmount($core.String v) { $_setString(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasFromAmount() => $_has(6);
  @$pb.TagNumber(7)
  void clearFromAmount() => clearField(7);

  /// Optional minimum accepted destination amount.  Actual destination amount will depend on current rates, limit amount can be used to prevent using very unfavorable rates.
  /// The default is 0 - no price limit.
  @$pb.TagNumber(8)
  $core.String get toAmountLimit => $_getSZ(7);
  @$pb.TagNumber(8)
  set toAmountLimit($core.String v) { $_setString(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasToAmountLimit() => $_has(7);
  @$pb.TagNumber(8)
  void clearToAmountLimit() => clearField(8);

  /// Optional affiliate fee destination address.  A Rune address.
  @$pb.TagNumber(9)
  $core.String get affiliateFeeAddress => $_getSZ(8);
  @$pb.TagNumber(9)
  set affiliateFeeAddress($core.String v) { $_setString(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasAffiliateFeeAddress() => $_has(8);
  @$pb.TagNumber(9)
  void clearAffiliateFeeAddress() => clearField(9);

  /// Optional affiliate fee, percentage base points, e.g. 100 means 1%, 0 - 1000, as string. Empty means to ignore it.
  @$pb.TagNumber(10)
  $core.String get affiliateFeeRateBp => $_getSZ(9);
  @$pb.TagNumber(10)
  set affiliateFeeRateBp($core.String v) { $_setString(9, v); }
  @$pb.TagNumber(10)
  $core.bool hasAffiliateFeeRateBp() => $_has(9);
  @$pb.TagNumber(10)
  void clearAffiliateFeeRateBp() => clearField(10);

  /// Optional extra custom memo, reserved for later use.
  @$pb.TagNumber(11)
  $core.String get extraMemo => $_getSZ(10);
  @$pb.TagNumber(11)
  set extraMemo($core.String v) { $_setString(10, v); }
  @$pb.TagNumber(11)
  $core.bool hasExtraMemo() => $_has(10);
  @$pb.TagNumber(11)
  void clearExtraMemo() => clearField(11);

  /// Optional expirationTime, will be now() + 15 min if not set
  @$pb.TagNumber(12)
  $fixnum.Int64 get expirationTime => $_getI64(11);
  @$pb.TagNumber(12)
  set expirationTime($fixnum.Int64 v) { $_setInt64(11, v); }
  @$pb.TagNumber(12)
  $core.bool hasExpirationTime() => $_has(11);
  @$pb.TagNumber(12)
  void clearExpirationTime() => clearField(12);

  /// Optional streaming parameters. Use Streaming Swaps and Swap Optimisation strategy if set.
  /// https://docs.thorchain.org/thorchain-finance/continuous-liquidity-pools#streaming-swaps-and-swap-optimisation
  @$pb.TagNumber(13)
  StreamParams get streamParams => $_getN(12);
  @$pb.TagNumber(13)
  set streamParams(StreamParams v) { setField(13, v); }
  @$pb.TagNumber(13)
  $core.bool hasStreamParams() => $_has(12);
  @$pb.TagNumber(13)
  void clearStreamParams() => clearField(13);
  @$pb.TagNumber(13)
  StreamParams ensureStreamParams() => $_ensure(12);
}

enum SwapOutput_SigningInputOneof {
  bitcoin, 
  ethereum, 
  binance, 
  cosmos, 
  notSet
}

/// Result of the swap, a SigningInput struct for the specific chain
class SwapOutput extends $pb.GeneratedMessage {
  factory SwapOutput({
    Chain? fromChain,
    Chain? toChain,
    Error? error,
    $2.SigningInput? bitcoin,
    $3.SigningInput? ethereum,
    $6.SigningInput? binance,
    $4.SigningInput? cosmos,
  }) {
    final $result = create();
    if (fromChain != null) {
      $result.fromChain = fromChain;
    }
    if (toChain != null) {
      $result.toChain = toChain;
    }
    if (error != null) {
      $result.error = error;
    }
    if (bitcoin != null) {
      $result.bitcoin = bitcoin;
    }
    if (ethereum != null) {
      $result.ethereum = ethereum;
    }
    if (binance != null) {
      $result.binance = binance;
    }
    if (cosmos != null) {
      $result.cosmos = cosmos;
    }
    return $result;
  }
  SwapOutput._() : super();
  factory SwapOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SwapOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, SwapOutput_SigningInputOneof> _SwapOutput_SigningInputOneofByTag = {
    4 : SwapOutput_SigningInputOneof.bitcoin,
    5 : SwapOutput_SigningInputOneof.ethereum,
    6 : SwapOutput_SigningInputOneof.binance,
    7 : SwapOutput_SigningInputOneof.cosmos,
    0 : SwapOutput_SigningInputOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SwapOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.THORChainSwap.Proto'), createEmptyInstance: create)
    ..oo(0, [4, 5, 6, 7])
    ..e<Chain>(1, _omitFieldNames ? '' : 'fromChain', $pb.PbFieldType.OE, defaultOrMaker: Chain.THOR, valueOf: Chain.valueOf, enumValues: Chain.values)
    ..e<Chain>(2, _omitFieldNames ? '' : 'toChain', $pb.PbFieldType.OE, defaultOrMaker: Chain.THOR, valueOf: Chain.valueOf, enumValues: Chain.values)
    ..aOM<Error>(3, _omitFieldNames ? '' : 'error', subBuilder: Error.create)
    ..aOM<$2.SigningInput>(4, _omitFieldNames ? '' : 'bitcoin', subBuilder: $2.SigningInput.create)
    ..aOM<$3.SigningInput>(5, _omitFieldNames ? '' : 'ethereum', subBuilder: $3.SigningInput.create)
    ..aOM<$6.SigningInput>(6, _omitFieldNames ? '' : 'binance', subBuilder: $6.SigningInput.create)
    ..aOM<$4.SigningInput>(7, _omitFieldNames ? '' : 'cosmos', subBuilder: $4.SigningInput.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SwapOutput clone() => SwapOutput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SwapOutput copyWith(void Function(SwapOutput) updates) => super.copyWith((message) => updates(message as SwapOutput)) as SwapOutput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SwapOutput create() => SwapOutput._();
  SwapOutput createEmptyInstance() => create();
  static $pb.PbList<SwapOutput> createRepeated() => $pb.PbList<SwapOutput>();
  @$core.pragma('dart2js:noInline')
  static SwapOutput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SwapOutput>(create);
  static SwapOutput? _defaultInstance;

  SwapOutput_SigningInputOneof whichSigningInputOneof() => _SwapOutput_SigningInputOneofByTag[$_whichOneof(0)]!;
  void clearSigningInputOneof() => clearField($_whichOneof(0));

  /// Source chain
  @$pb.TagNumber(1)
  Chain get fromChain => $_getN(0);
  @$pb.TagNumber(1)
  set fromChain(Chain v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasFromChain() => $_has(0);
  @$pb.TagNumber(1)
  void clearFromChain() => clearField(1);

  /// Destination chain
  @$pb.TagNumber(2)
  Chain get toChain => $_getN(1);
  @$pb.TagNumber(2)
  set toChain(Chain v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasToChain() => $_has(1);
  @$pb.TagNumber(2)
  void clearToChain() => clearField(2);

  /// Error code, filled in case of error, OK/empty on success
  @$pb.TagNumber(3)
  Error get error => $_getN(2);
  @$pb.TagNumber(3)
  set error(Error v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasError() => $_has(2);
  @$pb.TagNumber(3)
  void clearError() => clearField(3);
  @$pb.TagNumber(3)
  Error ensureError() => $_ensure(2);

  @$pb.TagNumber(4)
  $2.SigningInput get bitcoin => $_getN(3);
  @$pb.TagNumber(4)
  set bitcoin($2.SigningInput v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasBitcoin() => $_has(3);
  @$pb.TagNumber(4)
  void clearBitcoin() => clearField(4);
  @$pb.TagNumber(4)
  $2.SigningInput ensureBitcoin() => $_ensure(3);

  @$pb.TagNumber(5)
  $3.SigningInput get ethereum => $_getN(4);
  @$pb.TagNumber(5)
  set ethereum($3.SigningInput v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasEthereum() => $_has(4);
  @$pb.TagNumber(5)
  void clearEthereum() => clearField(5);
  @$pb.TagNumber(5)
  $3.SigningInput ensureEthereum() => $_ensure(4);

  @$pb.TagNumber(6)
  $6.SigningInput get binance => $_getN(5);
  @$pb.TagNumber(6)
  set binance($6.SigningInput v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasBinance() => $_has(5);
  @$pb.TagNumber(6)
  void clearBinance() => clearField(6);
  @$pb.TagNumber(6)
  $6.SigningInput ensureBinance() => $_ensure(5);

  @$pb.TagNumber(7)
  $4.SigningInput get cosmos => $_getN(6);
  @$pb.TagNumber(7)
  set cosmos($4.SigningInput v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasCosmos() => $_has(6);
  @$pb.TagNumber(7)
  void clearCosmos() => clearField(7);
  @$pb.TagNumber(7)
  $4.SigningInput ensureCosmos() => $_ensure(6);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
