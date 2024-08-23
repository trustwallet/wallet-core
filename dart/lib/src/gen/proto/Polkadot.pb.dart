//
//  Generated code. Do not modify.
//  source: Polkadot.proto
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
import 'Polkadot.pbenum.dart';

export 'Polkadot.pbenum.dart';

/// An era, a period defined by a starting block and length
class Era extends $pb.GeneratedMessage {
  factory Era({
    $fixnum.Int64? blockNumber,
    $fixnum.Int64? period,
  }) {
    final $result = create();
    if (blockNumber != null) {
      $result.blockNumber = blockNumber;
    }
    if (period != null) {
      $result.period = period;
    }
    return $result;
  }
  Era._() : super();
  factory Era.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Era.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Era', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'blockNumber', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'period', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Era clone() => Era()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Era copyWith(void Function(Era) updates) => super.copyWith((message) => updates(message as Era)) as Era;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Era create() => Era._();
  Era createEmptyInstance() => create();
  static $pb.PbList<Era> createRepeated() => $pb.PbList<Era>();
  @$core.pragma('dart2js:noInline')
  static Era getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Era>(create);
  static Era? _defaultInstance;

  /// recent block number (called phase in polkadot code), should match block hash
  @$pb.TagNumber(1)
  $fixnum.Int64 get blockNumber => $_getI64(0);
  @$pb.TagNumber(1)
  set blockNumber($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasBlockNumber() => $_has(0);
  @$pb.TagNumber(1)
  void clearBlockNumber() => clearField(1);

  /// length of period, calculated from block number, e.g. 64
  @$pb.TagNumber(2)
  $fixnum.Int64 get period => $_getI64(1);
  @$pb.TagNumber(2)
  set period($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPeriod() => $_has(1);
  @$pb.TagNumber(2)
  void clearPeriod() => clearField(2);
}

/// Readable decoded call indices can be found at https://www.subscan.io/
class CustomCallIndices extends $pb.GeneratedMessage {
  factory CustomCallIndices({
    $core.int? moduleIndex,
    $core.int? methodIndex,
  }) {
    final $result = create();
    if (moduleIndex != null) {
      $result.moduleIndex = moduleIndex;
    }
    if (methodIndex != null) {
      $result.methodIndex = methodIndex;
    }
    return $result;
  }
  CustomCallIndices._() : super();
  factory CustomCallIndices.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory CustomCallIndices.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'CustomCallIndices', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..a<$core.int>(4, _omitFieldNames ? '' : 'moduleIndex', $pb.PbFieldType.O3)
    ..a<$core.int>(5, _omitFieldNames ? '' : 'methodIndex', $pb.PbFieldType.O3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  CustomCallIndices clone() => CustomCallIndices()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  CustomCallIndices copyWith(void Function(CustomCallIndices) updates) => super.copyWith((message) => updates(message as CustomCallIndices)) as CustomCallIndices;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static CustomCallIndices create() => CustomCallIndices._();
  CustomCallIndices createEmptyInstance() => create();
  static $pb.PbList<CustomCallIndices> createRepeated() => $pb.PbList<CustomCallIndices>();
  @$core.pragma('dart2js:noInline')
  static CustomCallIndices getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<CustomCallIndices>(create);
  static CustomCallIndices? _defaultInstance;

  /// Module index.
  @$pb.TagNumber(4)
  $core.int get moduleIndex => $_getIZ(0);
  @$pb.TagNumber(4)
  set moduleIndex($core.int v) { $_setSignedInt32(0, v); }
  @$pb.TagNumber(4)
  $core.bool hasModuleIndex() => $_has(0);
  @$pb.TagNumber(4)
  void clearModuleIndex() => clearField(4);

  /// Method index.
  @$pb.TagNumber(5)
  $core.int get methodIndex => $_getIZ(1);
  @$pb.TagNumber(5)
  set methodIndex($core.int v) { $_setSignedInt32(1, v); }
  @$pb.TagNumber(5)
  $core.bool hasMethodIndex() => $_has(1);
  @$pb.TagNumber(5)
  void clearMethodIndex() => clearField(5);
}

enum CallIndices_Variant {
  custom, 
  notSet
}

/// Optional call indices.
/// Must be set if `SigningInput::network` is different from `Polkadot` and `Kusama`.
class CallIndices extends $pb.GeneratedMessage {
  factory CallIndices({
    CustomCallIndices? custom,
  }) {
    final $result = create();
    if (custom != null) {
      $result.custom = custom;
    }
    return $result;
  }
  CallIndices._() : super();
  factory CallIndices.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory CallIndices.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, CallIndices_Variant> _CallIndices_VariantByTag = {
    1 : CallIndices_Variant.custom,
    0 : CallIndices_Variant.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'CallIndices', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..oo(0, [1])
    ..aOM<CustomCallIndices>(1, _omitFieldNames ? '' : 'custom', subBuilder: CustomCallIndices.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  CallIndices clone() => CallIndices()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  CallIndices copyWith(void Function(CallIndices) updates) => super.copyWith((message) => updates(message as CallIndices)) as CallIndices;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static CallIndices create() => CallIndices._();
  CallIndices createEmptyInstance() => create();
  static $pb.PbList<CallIndices> createRepeated() => $pb.PbList<CallIndices>();
  @$core.pragma('dart2js:noInline')
  static CallIndices getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<CallIndices>(create);
  static CallIndices? _defaultInstance;

  CallIndices_Variant whichVariant() => _CallIndices_VariantByTag[$_whichOneof(0)]!;
  void clearVariant() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  CustomCallIndices get custom => $_getN(0);
  @$pb.TagNumber(1)
  set custom(CustomCallIndices v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasCustom() => $_has(0);
  @$pb.TagNumber(1)
  void clearCustom() => clearField(1);
  @$pb.TagNumber(1)
  CustomCallIndices ensureCustom() => $_ensure(0);
}

/// transfer
class Balance_Transfer extends $pb.GeneratedMessage {
  factory Balance_Transfer({
    $core.String? toAddress,
    $core.List<$core.int>? value,
    $core.String? memo,
    CallIndices? callIndices,
  }) {
    final $result = create();
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (value != null) {
      $result.value = value;
    }
    if (memo != null) {
      $result.memo = memo;
    }
    if (callIndices != null) {
      $result.callIndices = callIndices;
    }
    return $result;
  }
  Balance_Transfer._() : super();
  factory Balance_Transfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Balance_Transfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Balance.Transfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'toAddress')
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OY)
    ..aOS(3, _omitFieldNames ? '' : 'memo')
    ..aOM<CallIndices>(4, _omitFieldNames ? '' : 'callIndices', subBuilder: CallIndices.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Balance_Transfer clone() => Balance_Transfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Balance_Transfer copyWith(void Function(Balance_Transfer) updates) => super.copyWith((message) => updates(message as Balance_Transfer)) as Balance_Transfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Balance_Transfer create() => Balance_Transfer._();
  Balance_Transfer createEmptyInstance() => create();
  static $pb.PbList<Balance_Transfer> createRepeated() => $pb.PbList<Balance_Transfer>();
  @$core.pragma('dart2js:noInline')
  static Balance_Transfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Balance_Transfer>(create);
  static Balance_Transfer? _defaultInstance;

  /// destination address
  @$pb.TagNumber(1)
  $core.String get toAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set toAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasToAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearToAddress() => clearField(1);

  /// amount (uint256, serialized big endian)
  @$pb.TagNumber(2)
  $core.List<$core.int> get value => $_getN(1);
  @$pb.TagNumber(2)
  set value($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValue() => $_has(1);
  @$pb.TagNumber(2)
  void clearValue() => clearField(2);

  /// max 32 chars
  @$pb.TagNumber(3)
  $core.String get memo => $_getSZ(2);
  @$pb.TagNumber(3)
  set memo($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasMemo() => $_has(2);
  @$pb.TagNumber(3)
  void clearMemo() => clearField(3);

  /// call indices
  @$pb.TagNumber(4)
  CallIndices get callIndices => $_getN(3);
  @$pb.TagNumber(4)
  set callIndices(CallIndices v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasCallIndices() => $_has(3);
  @$pb.TagNumber(4)
  void clearCallIndices() => clearField(4);
  @$pb.TagNumber(4)
  CallIndices ensureCallIndices() => $_ensure(3);
}

/// batch tranfer
class Balance_BatchTransfer extends $pb.GeneratedMessage {
  factory Balance_BatchTransfer({
    CallIndices? callIndices,
    $core.Iterable<Balance_Transfer>? transfers,
  }) {
    final $result = create();
    if (callIndices != null) {
      $result.callIndices = callIndices;
    }
    if (transfers != null) {
      $result.transfers.addAll(transfers);
    }
    return $result;
  }
  Balance_BatchTransfer._() : super();
  factory Balance_BatchTransfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Balance_BatchTransfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Balance.BatchTransfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..aOM<CallIndices>(1, _omitFieldNames ? '' : 'callIndices', subBuilder: CallIndices.create)
    ..pc<Balance_Transfer>(2, _omitFieldNames ? '' : 'transfers', $pb.PbFieldType.PM, subBuilder: Balance_Transfer.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Balance_BatchTransfer clone() => Balance_BatchTransfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Balance_BatchTransfer copyWith(void Function(Balance_BatchTransfer) updates) => super.copyWith((message) => updates(message as Balance_BatchTransfer)) as Balance_BatchTransfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Balance_BatchTransfer create() => Balance_BatchTransfer._();
  Balance_BatchTransfer createEmptyInstance() => create();
  static $pb.PbList<Balance_BatchTransfer> createRepeated() => $pb.PbList<Balance_BatchTransfer>();
  @$core.pragma('dart2js:noInline')
  static Balance_BatchTransfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Balance_BatchTransfer>(create);
  static Balance_BatchTransfer? _defaultInstance;

  /// call indices
  @$pb.TagNumber(1)
  CallIndices get callIndices => $_getN(0);
  @$pb.TagNumber(1)
  set callIndices(CallIndices v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasCallIndices() => $_has(0);
  @$pb.TagNumber(1)
  void clearCallIndices() => clearField(1);
  @$pb.TagNumber(1)
  CallIndices ensureCallIndices() => $_ensure(0);

  @$pb.TagNumber(2)
  $core.List<Balance_Transfer> get transfers => $_getList(1);
}

/// asset transfer
class Balance_AssetTransfer extends $pb.GeneratedMessage {
  factory Balance_AssetTransfer({
    CallIndices? callIndices,
    $core.String? toAddress,
    $core.List<$core.int>? value,
    $core.int? assetId,
    $core.int? feeAssetId,
  }) {
    final $result = create();
    if (callIndices != null) {
      $result.callIndices = callIndices;
    }
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (value != null) {
      $result.value = value;
    }
    if (assetId != null) {
      $result.assetId = assetId;
    }
    if (feeAssetId != null) {
      $result.feeAssetId = feeAssetId;
    }
    return $result;
  }
  Balance_AssetTransfer._() : super();
  factory Balance_AssetTransfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Balance_AssetTransfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Balance.AssetTransfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..aOM<CallIndices>(1, _omitFieldNames ? '' : 'callIndices', subBuilder: CallIndices.create)
    ..aOS(2, _omitFieldNames ? '' : 'toAddress')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OY)
    ..a<$core.int>(4, _omitFieldNames ? '' : 'assetId', $pb.PbFieldType.OU3)
    ..a<$core.int>(5, _omitFieldNames ? '' : 'feeAssetId', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Balance_AssetTransfer clone() => Balance_AssetTransfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Balance_AssetTransfer copyWith(void Function(Balance_AssetTransfer) updates) => super.copyWith((message) => updates(message as Balance_AssetTransfer)) as Balance_AssetTransfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Balance_AssetTransfer create() => Balance_AssetTransfer._();
  Balance_AssetTransfer createEmptyInstance() => create();
  static $pb.PbList<Balance_AssetTransfer> createRepeated() => $pb.PbList<Balance_AssetTransfer>();
  @$core.pragma('dart2js:noInline')
  static Balance_AssetTransfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Balance_AssetTransfer>(create);
  static Balance_AssetTransfer? _defaultInstance;

  /// call indices
  @$pb.TagNumber(1)
  CallIndices get callIndices => $_getN(0);
  @$pb.TagNumber(1)
  set callIndices(CallIndices v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasCallIndices() => $_has(0);
  @$pb.TagNumber(1)
  void clearCallIndices() => clearField(1);
  @$pb.TagNumber(1)
  CallIndices ensureCallIndices() => $_ensure(0);

  /// destination
  @$pb.TagNumber(2)
  $core.String get toAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set toAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasToAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearToAddress() => clearField(2);

  /// value - BigInteger
  @$pb.TagNumber(3)
  $core.List<$core.int> get value => $_getN(2);
  @$pb.TagNumber(3)
  set value($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasValue() => $_has(2);
  @$pb.TagNumber(3)
  void clearValue() => clearField(3);

  /// asset identifier
  @$pb.TagNumber(4)
  $core.int get assetId => $_getIZ(3);
  @$pb.TagNumber(4)
  set assetId($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasAssetId() => $_has(3);
  @$pb.TagNumber(4)
  void clearAssetId() => clearField(4);

  /// fee asset identifier
  @$pb.TagNumber(5)
  $core.int get feeAssetId => $_getIZ(4);
  @$pb.TagNumber(5)
  set feeAssetId($core.int v) { $_setUnsignedInt32(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasFeeAssetId() => $_has(4);
  @$pb.TagNumber(5)
  void clearFeeAssetId() => clearField(5);
}

/// batch asset transfer
class Balance_BatchAssetTransfer extends $pb.GeneratedMessage {
  factory Balance_BatchAssetTransfer({
    CallIndices? callIndices,
    $core.int? feeAssetId,
    $core.Iterable<Balance_AssetTransfer>? transfers,
  }) {
    final $result = create();
    if (callIndices != null) {
      $result.callIndices = callIndices;
    }
    if (feeAssetId != null) {
      $result.feeAssetId = feeAssetId;
    }
    if (transfers != null) {
      $result.transfers.addAll(transfers);
    }
    return $result;
  }
  Balance_BatchAssetTransfer._() : super();
  factory Balance_BatchAssetTransfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Balance_BatchAssetTransfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Balance.BatchAssetTransfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..aOM<CallIndices>(1, _omitFieldNames ? '' : 'callIndices', subBuilder: CallIndices.create)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'feeAssetId', $pb.PbFieldType.OU3)
    ..pc<Balance_AssetTransfer>(3, _omitFieldNames ? '' : 'transfers', $pb.PbFieldType.PM, subBuilder: Balance_AssetTransfer.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Balance_BatchAssetTransfer clone() => Balance_BatchAssetTransfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Balance_BatchAssetTransfer copyWith(void Function(Balance_BatchAssetTransfer) updates) => super.copyWith((message) => updates(message as Balance_BatchAssetTransfer)) as Balance_BatchAssetTransfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Balance_BatchAssetTransfer create() => Balance_BatchAssetTransfer._();
  Balance_BatchAssetTransfer createEmptyInstance() => create();
  static $pb.PbList<Balance_BatchAssetTransfer> createRepeated() => $pb.PbList<Balance_BatchAssetTransfer>();
  @$core.pragma('dart2js:noInline')
  static Balance_BatchAssetTransfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Balance_BatchAssetTransfer>(create);
  static Balance_BatchAssetTransfer? _defaultInstance;

  /// call indices
  @$pb.TagNumber(1)
  CallIndices get callIndices => $_getN(0);
  @$pb.TagNumber(1)
  set callIndices(CallIndices v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasCallIndices() => $_has(0);
  @$pb.TagNumber(1)
  void clearCallIndices() => clearField(1);
  @$pb.TagNumber(1)
  CallIndices ensureCallIndices() => $_ensure(0);

  /// fee asset identifier
  @$pb.TagNumber(2)
  $core.int get feeAssetId => $_getIZ(1);
  @$pb.TagNumber(2)
  set feeAssetId($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasFeeAssetId() => $_has(1);
  @$pb.TagNumber(2)
  void clearFeeAssetId() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<Balance_AssetTransfer> get transfers => $_getList(2);
}

enum Balance_MessageOneof {
  transfer, 
  batchTransfer, 
  assetTransfer, 
  batchAssetTransfer, 
  notSet
}

/// Balance transfer transaction
class Balance extends $pb.GeneratedMessage {
  factory Balance({
    Balance_Transfer? transfer,
    Balance_BatchTransfer? batchTransfer,
    Balance_AssetTransfer? assetTransfer,
    Balance_BatchAssetTransfer? batchAssetTransfer,
  }) {
    final $result = create();
    if (transfer != null) {
      $result.transfer = transfer;
    }
    if (batchTransfer != null) {
      $result.batchTransfer = batchTransfer;
    }
    if (assetTransfer != null) {
      $result.assetTransfer = assetTransfer;
    }
    if (batchAssetTransfer != null) {
      $result.batchAssetTransfer = batchAssetTransfer;
    }
    return $result;
  }
  Balance._() : super();
  factory Balance.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Balance.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Balance_MessageOneof> _Balance_MessageOneofByTag = {
    1 : Balance_MessageOneof.transfer,
    2 : Balance_MessageOneof.batchTransfer,
    3 : Balance_MessageOneof.assetTransfer,
    4 : Balance_MessageOneof.batchAssetTransfer,
    0 : Balance_MessageOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Balance', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2, 3, 4])
    ..aOM<Balance_Transfer>(1, _omitFieldNames ? '' : 'transfer', subBuilder: Balance_Transfer.create)
    ..aOM<Balance_BatchTransfer>(2, _omitFieldNames ? '' : 'batchTransfer', protoName: 'batchTransfer', subBuilder: Balance_BatchTransfer.create)
    ..aOM<Balance_AssetTransfer>(3, _omitFieldNames ? '' : 'assetTransfer', subBuilder: Balance_AssetTransfer.create)
    ..aOM<Balance_BatchAssetTransfer>(4, _omitFieldNames ? '' : 'batchAssetTransfer', subBuilder: Balance_BatchAssetTransfer.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Balance clone() => Balance()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Balance copyWith(void Function(Balance) updates) => super.copyWith((message) => updates(message as Balance)) as Balance;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Balance create() => Balance._();
  Balance createEmptyInstance() => create();
  static $pb.PbList<Balance> createRepeated() => $pb.PbList<Balance>();
  @$core.pragma('dart2js:noInline')
  static Balance getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Balance>(create);
  static Balance? _defaultInstance;

  Balance_MessageOneof whichMessageOneof() => _Balance_MessageOneofByTag[$_whichOneof(0)]!;
  void clearMessageOneof() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  Balance_Transfer get transfer => $_getN(0);
  @$pb.TagNumber(1)
  set transfer(Balance_Transfer v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasTransfer() => $_has(0);
  @$pb.TagNumber(1)
  void clearTransfer() => clearField(1);
  @$pb.TagNumber(1)
  Balance_Transfer ensureTransfer() => $_ensure(0);

  @$pb.TagNumber(2)
  Balance_BatchTransfer get batchTransfer => $_getN(1);
  @$pb.TagNumber(2)
  set batchTransfer(Balance_BatchTransfer v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasBatchTransfer() => $_has(1);
  @$pb.TagNumber(2)
  void clearBatchTransfer() => clearField(2);
  @$pb.TagNumber(2)
  Balance_BatchTransfer ensureBatchTransfer() => $_ensure(1);

  @$pb.TagNumber(3)
  Balance_AssetTransfer get assetTransfer => $_getN(2);
  @$pb.TagNumber(3)
  set assetTransfer(Balance_AssetTransfer v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasAssetTransfer() => $_has(2);
  @$pb.TagNumber(3)
  void clearAssetTransfer() => clearField(3);
  @$pb.TagNumber(3)
  Balance_AssetTransfer ensureAssetTransfer() => $_ensure(2);

  @$pb.TagNumber(4)
  Balance_BatchAssetTransfer get batchAssetTransfer => $_getN(3);
  @$pb.TagNumber(4)
  set batchAssetTransfer(Balance_BatchAssetTransfer v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasBatchAssetTransfer() => $_has(3);
  @$pb.TagNumber(4)
  void clearBatchAssetTransfer() => clearField(4);
  @$pb.TagNumber(4)
  Balance_BatchAssetTransfer ensureBatchAssetTransfer() => $_ensure(3);
}

/// Bond to a controller
class Staking_Bond extends $pb.GeneratedMessage {
  factory Staking_Bond({
    $core.String? controller,
    $core.List<$core.int>? value,
    RewardDestination? rewardDestination,
    CallIndices? callIndices,
  }) {
    final $result = create();
    if (controller != null) {
      $result.controller = controller;
    }
    if (value != null) {
      $result.value = value;
    }
    if (rewardDestination != null) {
      $result.rewardDestination = rewardDestination;
    }
    if (callIndices != null) {
      $result.callIndices = callIndices;
    }
    return $result;
  }
  Staking_Bond._() : super();
  factory Staking_Bond.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_Bond.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.Bond', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'controller')
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OY)
    ..e<RewardDestination>(3, _omitFieldNames ? '' : 'rewardDestination', $pb.PbFieldType.OE, defaultOrMaker: RewardDestination.STAKED, valueOf: RewardDestination.valueOf, enumValues: RewardDestination.values)
    ..aOM<CallIndices>(4, _omitFieldNames ? '' : 'callIndices', subBuilder: CallIndices.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_Bond clone() => Staking_Bond()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_Bond copyWith(void Function(Staking_Bond) updates) => super.copyWith((message) => updates(message as Staking_Bond)) as Staking_Bond;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_Bond create() => Staking_Bond._();
  Staking_Bond createEmptyInstance() => create();
  static $pb.PbList<Staking_Bond> createRepeated() => $pb.PbList<Staking_Bond>();
  @$core.pragma('dart2js:noInline')
  static Staking_Bond getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_Bond>(create);
  static Staking_Bond? _defaultInstance;

  /// controller ID (optional)
  @$pb.TagNumber(1)
  $core.String get controller => $_getSZ(0);
  @$pb.TagNumber(1)
  set controller($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasController() => $_has(0);
  @$pb.TagNumber(1)
  void clearController() => clearField(1);

  /// amount (uint256, serialized big endian)
  @$pb.TagNumber(2)
  $core.List<$core.int> get value => $_getN(1);
  @$pb.TagNumber(2)
  set value($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValue() => $_has(1);
  @$pb.TagNumber(2)
  void clearValue() => clearField(2);

  /// destination for rewards
  @$pb.TagNumber(3)
  RewardDestination get rewardDestination => $_getN(2);
  @$pb.TagNumber(3)
  set rewardDestination(RewardDestination v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasRewardDestination() => $_has(2);
  @$pb.TagNumber(3)
  void clearRewardDestination() => clearField(3);

  /// call indices
  @$pb.TagNumber(4)
  CallIndices get callIndices => $_getN(3);
  @$pb.TagNumber(4)
  set callIndices(CallIndices v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasCallIndices() => $_has(3);
  @$pb.TagNumber(4)
  void clearCallIndices() => clearField(4);
  @$pb.TagNumber(4)
  CallIndices ensureCallIndices() => $_ensure(3);
}

/// Bond to a controller, with nominators
class Staking_BondAndNominate extends $pb.GeneratedMessage {
  factory Staking_BondAndNominate({
    $core.String? controller,
    $core.List<$core.int>? value,
    RewardDestination? rewardDestination,
    $core.Iterable<$core.String>? nominators,
    CallIndices? callIndices,
  }) {
    final $result = create();
    if (controller != null) {
      $result.controller = controller;
    }
    if (value != null) {
      $result.value = value;
    }
    if (rewardDestination != null) {
      $result.rewardDestination = rewardDestination;
    }
    if (nominators != null) {
      $result.nominators.addAll(nominators);
    }
    if (callIndices != null) {
      $result.callIndices = callIndices;
    }
    return $result;
  }
  Staking_BondAndNominate._() : super();
  factory Staking_BondAndNominate.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_BondAndNominate.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.BondAndNominate', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'controller')
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OY)
    ..e<RewardDestination>(3, _omitFieldNames ? '' : 'rewardDestination', $pb.PbFieldType.OE, defaultOrMaker: RewardDestination.STAKED, valueOf: RewardDestination.valueOf, enumValues: RewardDestination.values)
    ..pPS(4, _omitFieldNames ? '' : 'nominators')
    ..aOM<CallIndices>(5, _omitFieldNames ? '' : 'callIndices', subBuilder: CallIndices.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_BondAndNominate clone() => Staking_BondAndNominate()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_BondAndNominate copyWith(void Function(Staking_BondAndNominate) updates) => super.copyWith((message) => updates(message as Staking_BondAndNominate)) as Staking_BondAndNominate;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_BondAndNominate create() => Staking_BondAndNominate._();
  Staking_BondAndNominate createEmptyInstance() => create();
  static $pb.PbList<Staking_BondAndNominate> createRepeated() => $pb.PbList<Staking_BondAndNominate>();
  @$core.pragma('dart2js:noInline')
  static Staking_BondAndNominate getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_BondAndNominate>(create);
  static Staking_BondAndNominate? _defaultInstance;

  /// controller ID (optional)
  @$pb.TagNumber(1)
  $core.String get controller => $_getSZ(0);
  @$pb.TagNumber(1)
  set controller($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasController() => $_has(0);
  @$pb.TagNumber(1)
  void clearController() => clearField(1);

  /// amount (uint256, serialized big endian)
  @$pb.TagNumber(2)
  $core.List<$core.int> get value => $_getN(1);
  @$pb.TagNumber(2)
  set value($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValue() => $_has(1);
  @$pb.TagNumber(2)
  void clearValue() => clearField(2);

  /// destination for rewards
  @$pb.TagNumber(3)
  RewardDestination get rewardDestination => $_getN(2);
  @$pb.TagNumber(3)
  set rewardDestination(RewardDestination v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasRewardDestination() => $_has(2);
  @$pb.TagNumber(3)
  void clearRewardDestination() => clearField(3);

  /// list of nominators
  @$pb.TagNumber(4)
  $core.List<$core.String> get nominators => $_getList(3);

  /// call indices
  @$pb.TagNumber(5)
  CallIndices get callIndices => $_getN(4);
  @$pb.TagNumber(5)
  set callIndices(CallIndices v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasCallIndices() => $_has(4);
  @$pb.TagNumber(5)
  void clearCallIndices() => clearField(5);
  @$pb.TagNumber(5)
  CallIndices ensureCallIndices() => $_ensure(4);
}

/// Bond extra amount
class Staking_BondExtra extends $pb.GeneratedMessage {
  factory Staking_BondExtra({
    $core.List<$core.int>? value,
    CallIndices? callIndices,
  }) {
    final $result = create();
    if (value != null) {
      $result.value = value;
    }
    if (callIndices != null) {
      $result.callIndices = callIndices;
    }
    return $result;
  }
  Staking_BondExtra._() : super();
  factory Staking_BondExtra.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_BondExtra.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.BondExtra', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OY)
    ..aOM<CallIndices>(2, _omitFieldNames ? '' : 'callIndices', subBuilder: CallIndices.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_BondExtra clone() => Staking_BondExtra()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_BondExtra copyWith(void Function(Staking_BondExtra) updates) => super.copyWith((message) => updates(message as Staking_BondExtra)) as Staking_BondExtra;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_BondExtra create() => Staking_BondExtra._();
  Staking_BondExtra createEmptyInstance() => create();
  static $pb.PbList<Staking_BondExtra> createRepeated() => $pb.PbList<Staking_BondExtra>();
  @$core.pragma('dart2js:noInline')
  static Staking_BondExtra getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_BondExtra>(create);
  static Staking_BondExtra? _defaultInstance;

  /// amount (uint256, serialized big endian)
  @$pb.TagNumber(1)
  $core.List<$core.int> get value => $_getN(0);
  @$pb.TagNumber(1)
  set value($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasValue() => $_has(0);
  @$pb.TagNumber(1)
  void clearValue() => clearField(1);

  /// call indices
  @$pb.TagNumber(2)
  CallIndices get callIndices => $_getN(1);
  @$pb.TagNumber(2)
  set callIndices(CallIndices v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasCallIndices() => $_has(1);
  @$pb.TagNumber(2)
  void clearCallIndices() => clearField(2);
  @$pb.TagNumber(2)
  CallIndices ensureCallIndices() => $_ensure(1);
}

/// Unbond
class Staking_Unbond extends $pb.GeneratedMessage {
  factory Staking_Unbond({
    $core.List<$core.int>? value,
    CallIndices? callIndices,
  }) {
    final $result = create();
    if (value != null) {
      $result.value = value;
    }
    if (callIndices != null) {
      $result.callIndices = callIndices;
    }
    return $result;
  }
  Staking_Unbond._() : super();
  factory Staking_Unbond.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_Unbond.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.Unbond', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OY)
    ..aOM<CallIndices>(2, _omitFieldNames ? '' : 'callIndices', subBuilder: CallIndices.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_Unbond clone() => Staking_Unbond()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_Unbond copyWith(void Function(Staking_Unbond) updates) => super.copyWith((message) => updates(message as Staking_Unbond)) as Staking_Unbond;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_Unbond create() => Staking_Unbond._();
  Staking_Unbond createEmptyInstance() => create();
  static $pb.PbList<Staking_Unbond> createRepeated() => $pb.PbList<Staking_Unbond>();
  @$core.pragma('dart2js:noInline')
  static Staking_Unbond getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_Unbond>(create);
  static Staking_Unbond? _defaultInstance;

  /// amount (uint256, serialized big endian)
  @$pb.TagNumber(1)
  $core.List<$core.int> get value => $_getN(0);
  @$pb.TagNumber(1)
  set value($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasValue() => $_has(0);
  @$pb.TagNumber(1)
  void clearValue() => clearField(1);

  /// call indices
  @$pb.TagNumber(2)
  CallIndices get callIndices => $_getN(1);
  @$pb.TagNumber(2)
  set callIndices(CallIndices v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasCallIndices() => $_has(1);
  @$pb.TagNumber(2)
  void clearCallIndices() => clearField(2);
  @$pb.TagNumber(2)
  CallIndices ensureCallIndices() => $_ensure(1);
}

/// Rebond
class Staking_Rebond extends $pb.GeneratedMessage {
  factory Staking_Rebond({
    $core.List<$core.int>? value,
    CallIndices? callIndices,
  }) {
    final $result = create();
    if (value != null) {
      $result.value = value;
    }
    if (callIndices != null) {
      $result.callIndices = callIndices;
    }
    return $result;
  }
  Staking_Rebond._() : super();
  factory Staking_Rebond.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_Rebond.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.Rebond', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OY)
    ..aOM<CallIndices>(2, _omitFieldNames ? '' : 'callIndices', subBuilder: CallIndices.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_Rebond clone() => Staking_Rebond()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_Rebond copyWith(void Function(Staking_Rebond) updates) => super.copyWith((message) => updates(message as Staking_Rebond)) as Staking_Rebond;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_Rebond create() => Staking_Rebond._();
  Staking_Rebond createEmptyInstance() => create();
  static $pb.PbList<Staking_Rebond> createRepeated() => $pb.PbList<Staking_Rebond>();
  @$core.pragma('dart2js:noInline')
  static Staking_Rebond getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_Rebond>(create);
  static Staking_Rebond? _defaultInstance;

  /// amount (uint256, serialized big endian)
  @$pb.TagNumber(1)
  $core.List<$core.int> get value => $_getN(0);
  @$pb.TagNumber(1)
  set value($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasValue() => $_has(0);
  @$pb.TagNumber(1)
  void clearValue() => clearField(1);

  /// call indices
  @$pb.TagNumber(2)
  CallIndices get callIndices => $_getN(1);
  @$pb.TagNumber(2)
  set callIndices(CallIndices v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasCallIndices() => $_has(1);
  @$pb.TagNumber(2)
  void clearCallIndices() => clearField(2);
  @$pb.TagNumber(2)
  CallIndices ensureCallIndices() => $_ensure(1);
}

/// Withdraw unbonded amounts
class Staking_WithdrawUnbonded extends $pb.GeneratedMessage {
  factory Staking_WithdrawUnbonded({
    $core.int? slashingSpans,
    CallIndices? callIndices,
  }) {
    final $result = create();
    if (slashingSpans != null) {
      $result.slashingSpans = slashingSpans;
    }
    if (callIndices != null) {
      $result.callIndices = callIndices;
    }
    return $result;
  }
  Staking_WithdrawUnbonded._() : super();
  factory Staking_WithdrawUnbonded.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_WithdrawUnbonded.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.WithdrawUnbonded', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'slashingSpans', $pb.PbFieldType.O3)
    ..aOM<CallIndices>(2, _omitFieldNames ? '' : 'callIndices', subBuilder: CallIndices.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_WithdrawUnbonded clone() => Staking_WithdrawUnbonded()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_WithdrawUnbonded copyWith(void Function(Staking_WithdrawUnbonded) updates) => super.copyWith((message) => updates(message as Staking_WithdrawUnbonded)) as Staking_WithdrawUnbonded;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_WithdrawUnbonded create() => Staking_WithdrawUnbonded._();
  Staking_WithdrawUnbonded createEmptyInstance() => create();
  static $pb.PbList<Staking_WithdrawUnbonded> createRepeated() => $pb.PbList<Staking_WithdrawUnbonded>();
  @$core.pragma('dart2js:noInline')
  static Staking_WithdrawUnbonded getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_WithdrawUnbonded>(create);
  static Staking_WithdrawUnbonded? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get slashingSpans => $_getIZ(0);
  @$pb.TagNumber(1)
  set slashingSpans($core.int v) { $_setSignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSlashingSpans() => $_has(0);
  @$pb.TagNumber(1)
  void clearSlashingSpans() => clearField(1);

  /// call indices
  @$pb.TagNumber(2)
  CallIndices get callIndices => $_getN(1);
  @$pb.TagNumber(2)
  set callIndices(CallIndices v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasCallIndices() => $_has(1);
  @$pb.TagNumber(2)
  void clearCallIndices() => clearField(2);
  @$pb.TagNumber(2)
  CallIndices ensureCallIndices() => $_ensure(1);
}

/// Nominate
class Staking_Nominate extends $pb.GeneratedMessage {
  factory Staking_Nominate({
    $core.Iterable<$core.String>? nominators,
    CallIndices? callIndices,
  }) {
    final $result = create();
    if (nominators != null) {
      $result.nominators.addAll(nominators);
    }
    if (callIndices != null) {
      $result.callIndices = callIndices;
    }
    return $result;
  }
  Staking_Nominate._() : super();
  factory Staking_Nominate.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_Nominate.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.Nominate', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..pPS(1, _omitFieldNames ? '' : 'nominators')
    ..aOM<CallIndices>(2, _omitFieldNames ? '' : 'callIndices', subBuilder: CallIndices.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_Nominate clone() => Staking_Nominate()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_Nominate copyWith(void Function(Staking_Nominate) updates) => super.copyWith((message) => updates(message as Staking_Nominate)) as Staking_Nominate;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_Nominate create() => Staking_Nominate._();
  Staking_Nominate createEmptyInstance() => create();
  static $pb.PbList<Staking_Nominate> createRepeated() => $pb.PbList<Staking_Nominate>();
  @$core.pragma('dart2js:noInline')
  static Staking_Nominate getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_Nominate>(create);
  static Staking_Nominate? _defaultInstance;

  /// list of nominators
  @$pb.TagNumber(1)
  $core.List<$core.String> get nominators => $_getList(0);

  /// call indices
  @$pb.TagNumber(2)
  CallIndices get callIndices => $_getN(1);
  @$pb.TagNumber(2)
  set callIndices(CallIndices v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasCallIndices() => $_has(1);
  @$pb.TagNumber(2)
  void clearCallIndices() => clearField(2);
  @$pb.TagNumber(2)
  CallIndices ensureCallIndices() => $_ensure(1);
}

/// Chill and unbound
class Staking_ChillAndUnbond extends $pb.GeneratedMessage {
  factory Staking_ChillAndUnbond({
    $core.List<$core.int>? value,
    CallIndices? callIndices,
  }) {
    final $result = create();
    if (value != null) {
      $result.value = value;
    }
    if (callIndices != null) {
      $result.callIndices = callIndices;
    }
    return $result;
  }
  Staking_ChillAndUnbond._() : super();
  factory Staking_ChillAndUnbond.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_ChillAndUnbond.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.ChillAndUnbond', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OY)
    ..aOM<CallIndices>(2, _omitFieldNames ? '' : 'callIndices', subBuilder: CallIndices.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_ChillAndUnbond clone() => Staking_ChillAndUnbond()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_ChillAndUnbond copyWith(void Function(Staking_ChillAndUnbond) updates) => super.copyWith((message) => updates(message as Staking_ChillAndUnbond)) as Staking_ChillAndUnbond;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_ChillAndUnbond create() => Staking_ChillAndUnbond._();
  Staking_ChillAndUnbond createEmptyInstance() => create();
  static $pb.PbList<Staking_ChillAndUnbond> createRepeated() => $pb.PbList<Staking_ChillAndUnbond>();
  @$core.pragma('dart2js:noInline')
  static Staking_ChillAndUnbond getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_ChillAndUnbond>(create);
  static Staking_ChillAndUnbond? _defaultInstance;

  /// amount (uint256, serialized big endian)
  @$pb.TagNumber(1)
  $core.List<$core.int> get value => $_getN(0);
  @$pb.TagNumber(1)
  set value($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasValue() => $_has(0);
  @$pb.TagNumber(1)
  void clearValue() => clearField(1);

  /// call indices
  @$pb.TagNumber(2)
  CallIndices get callIndices => $_getN(1);
  @$pb.TagNumber(2)
  set callIndices(CallIndices v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasCallIndices() => $_has(1);
  @$pb.TagNumber(2)
  void clearCallIndices() => clearField(2);
  @$pb.TagNumber(2)
  CallIndices ensureCallIndices() => $_ensure(1);
}

/// Chill
class Staking_Chill extends $pb.GeneratedMessage {
  factory Staking_Chill({
    CallIndices? callIndices,
  }) {
    final $result = create();
    if (callIndices != null) {
      $result.callIndices = callIndices;
    }
    return $result;
  }
  Staking_Chill._() : super();
  factory Staking_Chill.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_Chill.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.Chill', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..aOM<CallIndices>(1, _omitFieldNames ? '' : 'callIndices', subBuilder: CallIndices.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_Chill clone() => Staking_Chill()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_Chill copyWith(void Function(Staking_Chill) updates) => super.copyWith((message) => updates(message as Staking_Chill)) as Staking_Chill;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_Chill create() => Staking_Chill._();
  Staking_Chill createEmptyInstance() => create();
  static $pb.PbList<Staking_Chill> createRepeated() => $pb.PbList<Staking_Chill>();
  @$core.pragma('dart2js:noInline')
  static Staking_Chill getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_Chill>(create);
  static Staking_Chill? _defaultInstance;

  /// call indices
  @$pb.TagNumber(1)
  CallIndices get callIndices => $_getN(0);
  @$pb.TagNumber(1)
  set callIndices(CallIndices v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasCallIndices() => $_has(0);
  @$pb.TagNumber(1)
  void clearCallIndices() => clearField(1);
  @$pb.TagNumber(1)
  CallIndices ensureCallIndices() => $_ensure(0);
}

enum Staking_MessageOneof {
  bond, 
  bondAndNominate, 
  bondExtra, 
  unbond, 
  withdrawUnbonded, 
  nominate, 
  chill, 
  chillAndUnbond, 
  rebond, 
  notSet
}

/// Staking transaction
class Staking extends $pb.GeneratedMessage {
  factory Staking({
    Staking_Bond? bond,
    Staking_BondAndNominate? bondAndNominate,
    Staking_BondExtra? bondExtra,
    Staking_Unbond? unbond,
    Staking_WithdrawUnbonded? withdrawUnbonded,
    Staking_Nominate? nominate,
    Staking_Chill? chill,
    Staking_ChillAndUnbond? chillAndUnbond,
    Staking_Rebond? rebond,
  }) {
    final $result = create();
    if (bond != null) {
      $result.bond = bond;
    }
    if (bondAndNominate != null) {
      $result.bondAndNominate = bondAndNominate;
    }
    if (bondExtra != null) {
      $result.bondExtra = bondExtra;
    }
    if (unbond != null) {
      $result.unbond = unbond;
    }
    if (withdrawUnbonded != null) {
      $result.withdrawUnbonded = withdrawUnbonded;
    }
    if (nominate != null) {
      $result.nominate = nominate;
    }
    if (chill != null) {
      $result.chill = chill;
    }
    if (chillAndUnbond != null) {
      $result.chillAndUnbond = chillAndUnbond;
    }
    if (rebond != null) {
      $result.rebond = rebond;
    }
    return $result;
  }
  Staking._() : super();
  factory Staking.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Staking_MessageOneof> _Staking_MessageOneofByTag = {
    1 : Staking_MessageOneof.bond,
    2 : Staking_MessageOneof.bondAndNominate,
    3 : Staking_MessageOneof.bondExtra,
    4 : Staking_MessageOneof.unbond,
    5 : Staking_MessageOneof.withdrawUnbonded,
    6 : Staking_MessageOneof.nominate,
    7 : Staking_MessageOneof.chill,
    8 : Staking_MessageOneof.chillAndUnbond,
    9 : Staking_MessageOneof.rebond,
    0 : Staking_MessageOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2, 3, 4, 5, 6, 7, 8, 9])
    ..aOM<Staking_Bond>(1, _omitFieldNames ? '' : 'bond', subBuilder: Staking_Bond.create)
    ..aOM<Staking_BondAndNominate>(2, _omitFieldNames ? '' : 'bondAndNominate', subBuilder: Staking_BondAndNominate.create)
    ..aOM<Staking_BondExtra>(3, _omitFieldNames ? '' : 'bondExtra', subBuilder: Staking_BondExtra.create)
    ..aOM<Staking_Unbond>(4, _omitFieldNames ? '' : 'unbond', subBuilder: Staking_Unbond.create)
    ..aOM<Staking_WithdrawUnbonded>(5, _omitFieldNames ? '' : 'withdrawUnbonded', subBuilder: Staking_WithdrawUnbonded.create)
    ..aOM<Staking_Nominate>(6, _omitFieldNames ? '' : 'nominate', subBuilder: Staking_Nominate.create)
    ..aOM<Staking_Chill>(7, _omitFieldNames ? '' : 'chill', subBuilder: Staking_Chill.create)
    ..aOM<Staking_ChillAndUnbond>(8, _omitFieldNames ? '' : 'chillAndUnbond', subBuilder: Staking_ChillAndUnbond.create)
    ..aOM<Staking_Rebond>(9, _omitFieldNames ? '' : 'rebond', subBuilder: Staking_Rebond.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking clone() => Staking()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking copyWith(void Function(Staking) updates) => super.copyWith((message) => updates(message as Staking)) as Staking;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking create() => Staking._();
  Staking createEmptyInstance() => create();
  static $pb.PbList<Staking> createRepeated() => $pb.PbList<Staking>();
  @$core.pragma('dart2js:noInline')
  static Staking getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking>(create);
  static Staking? _defaultInstance;

  Staking_MessageOneof whichMessageOneof() => _Staking_MessageOneofByTag[$_whichOneof(0)]!;
  void clearMessageOneof() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  Staking_Bond get bond => $_getN(0);
  @$pb.TagNumber(1)
  set bond(Staking_Bond v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasBond() => $_has(0);
  @$pb.TagNumber(1)
  void clearBond() => clearField(1);
  @$pb.TagNumber(1)
  Staking_Bond ensureBond() => $_ensure(0);

  @$pb.TagNumber(2)
  Staking_BondAndNominate get bondAndNominate => $_getN(1);
  @$pb.TagNumber(2)
  set bondAndNominate(Staking_BondAndNominate v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasBondAndNominate() => $_has(1);
  @$pb.TagNumber(2)
  void clearBondAndNominate() => clearField(2);
  @$pb.TagNumber(2)
  Staking_BondAndNominate ensureBondAndNominate() => $_ensure(1);

  @$pb.TagNumber(3)
  Staking_BondExtra get bondExtra => $_getN(2);
  @$pb.TagNumber(3)
  set bondExtra(Staking_BondExtra v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasBondExtra() => $_has(2);
  @$pb.TagNumber(3)
  void clearBondExtra() => clearField(3);
  @$pb.TagNumber(3)
  Staking_BondExtra ensureBondExtra() => $_ensure(2);

  @$pb.TagNumber(4)
  Staking_Unbond get unbond => $_getN(3);
  @$pb.TagNumber(4)
  set unbond(Staking_Unbond v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasUnbond() => $_has(3);
  @$pb.TagNumber(4)
  void clearUnbond() => clearField(4);
  @$pb.TagNumber(4)
  Staking_Unbond ensureUnbond() => $_ensure(3);

  @$pb.TagNumber(5)
  Staking_WithdrawUnbonded get withdrawUnbonded => $_getN(4);
  @$pb.TagNumber(5)
  set withdrawUnbonded(Staking_WithdrawUnbonded v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasWithdrawUnbonded() => $_has(4);
  @$pb.TagNumber(5)
  void clearWithdrawUnbonded() => clearField(5);
  @$pb.TagNumber(5)
  Staking_WithdrawUnbonded ensureWithdrawUnbonded() => $_ensure(4);

  @$pb.TagNumber(6)
  Staking_Nominate get nominate => $_getN(5);
  @$pb.TagNumber(6)
  set nominate(Staking_Nominate v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasNominate() => $_has(5);
  @$pb.TagNumber(6)
  void clearNominate() => clearField(6);
  @$pb.TagNumber(6)
  Staking_Nominate ensureNominate() => $_ensure(5);

  @$pb.TagNumber(7)
  Staking_Chill get chill => $_getN(6);
  @$pb.TagNumber(7)
  set chill(Staking_Chill v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasChill() => $_has(6);
  @$pb.TagNumber(7)
  void clearChill() => clearField(7);
  @$pb.TagNumber(7)
  Staking_Chill ensureChill() => $_ensure(6);

  @$pb.TagNumber(8)
  Staking_ChillAndUnbond get chillAndUnbond => $_getN(7);
  @$pb.TagNumber(8)
  set chillAndUnbond(Staking_ChillAndUnbond v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasChillAndUnbond() => $_has(7);
  @$pb.TagNumber(8)
  void clearChillAndUnbond() => clearField(8);
  @$pb.TagNumber(8)
  Staking_ChillAndUnbond ensureChillAndUnbond() => $_ensure(7);

  @$pb.TagNumber(9)
  Staking_Rebond get rebond => $_getN(8);
  @$pb.TagNumber(9)
  set rebond(Staking_Rebond v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasRebond() => $_has(8);
  @$pb.TagNumber(9)
  void clearRebond() => clearField(9);
  @$pb.TagNumber(9)
  Staking_Rebond ensureRebond() => $_ensure(8);
}

/// Identity::join_identity_as_key call
class Identity_JoinIdentityAsKey extends $pb.GeneratedMessage {
  factory Identity_JoinIdentityAsKey({
    CallIndices? callIndices,
    $fixnum.Int64? authId,
  }) {
    final $result = create();
    if (callIndices != null) {
      $result.callIndices = callIndices;
    }
    if (authId != null) {
      $result.authId = authId;
    }
    return $result;
  }
  Identity_JoinIdentityAsKey._() : super();
  factory Identity_JoinIdentityAsKey.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Identity_JoinIdentityAsKey.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Identity.JoinIdentityAsKey', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..aOM<CallIndices>(1, _omitFieldNames ? '' : 'callIndices', subBuilder: CallIndices.create)
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'authId', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Identity_JoinIdentityAsKey clone() => Identity_JoinIdentityAsKey()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Identity_JoinIdentityAsKey copyWith(void Function(Identity_JoinIdentityAsKey) updates) => super.copyWith((message) => updates(message as Identity_JoinIdentityAsKey)) as Identity_JoinIdentityAsKey;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Identity_JoinIdentityAsKey create() => Identity_JoinIdentityAsKey._();
  Identity_JoinIdentityAsKey createEmptyInstance() => create();
  static $pb.PbList<Identity_JoinIdentityAsKey> createRepeated() => $pb.PbList<Identity_JoinIdentityAsKey>();
  @$core.pragma('dart2js:noInline')
  static Identity_JoinIdentityAsKey getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Identity_JoinIdentityAsKey>(create);
  static Identity_JoinIdentityAsKey? _defaultInstance;

  /// call indices
  @$pb.TagNumber(1)
  CallIndices get callIndices => $_getN(0);
  @$pb.TagNumber(1)
  set callIndices(CallIndices v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasCallIndices() => $_has(0);
  @$pb.TagNumber(1)
  void clearCallIndices() => clearField(1);
  @$pb.TagNumber(1)
  CallIndices ensureCallIndices() => $_ensure(0);

  /// auth id
  @$pb.TagNumber(2)
  $fixnum.Int64 get authId => $_getI64(1);
  @$pb.TagNumber(2)
  set authId($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAuthId() => $_has(1);
  @$pb.TagNumber(2)
  void clearAuthId() => clearField(2);
}

class Identity_AddAuthorization_Data extends $pb.GeneratedMessage {
  factory Identity_AddAuthorization_Data({
    $core.List<$core.int>? data,
  }) {
    final $result = create();
    if (data != null) {
      $result.data = data;
    }
    return $result;
  }
  Identity_AddAuthorization_Data._() : super();
  factory Identity_AddAuthorization_Data.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Identity_AddAuthorization_Data.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Identity.AddAuthorization.Data', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'data', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Identity_AddAuthorization_Data clone() => Identity_AddAuthorization_Data()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Identity_AddAuthorization_Data copyWith(void Function(Identity_AddAuthorization_Data) updates) => super.copyWith((message) => updates(message as Identity_AddAuthorization_Data)) as Identity_AddAuthorization_Data;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Identity_AddAuthorization_Data create() => Identity_AddAuthorization_Data._();
  Identity_AddAuthorization_Data createEmptyInstance() => create();
  static $pb.PbList<Identity_AddAuthorization_Data> createRepeated() => $pb.PbList<Identity_AddAuthorization_Data>();
  @$core.pragma('dart2js:noInline')
  static Identity_AddAuthorization_Data getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Identity_AddAuthorization_Data>(create);
  static Identity_AddAuthorization_Data? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.int> get data => $_getN(0);
  @$pb.TagNumber(1)
  set data($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasData() => $_has(0);
  @$pb.TagNumber(1)
  void clearData() => clearField(1);
}

class Identity_AddAuthorization_AuthData extends $pb.GeneratedMessage {
  factory Identity_AddAuthorization_AuthData({
    Identity_AddAuthorization_Data? asset,
    Identity_AddAuthorization_Data? extrinsic,
    Identity_AddAuthorization_Data? portfolio,
  }) {
    final $result = create();
    if (asset != null) {
      $result.asset = asset;
    }
    if (extrinsic != null) {
      $result.extrinsic = extrinsic;
    }
    if (portfolio != null) {
      $result.portfolio = portfolio;
    }
    return $result;
  }
  Identity_AddAuthorization_AuthData._() : super();
  factory Identity_AddAuthorization_AuthData.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Identity_AddAuthorization_AuthData.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Identity.AddAuthorization.AuthData', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..aOM<Identity_AddAuthorization_Data>(1, _omitFieldNames ? '' : 'asset', subBuilder: Identity_AddAuthorization_Data.create)
    ..aOM<Identity_AddAuthorization_Data>(2, _omitFieldNames ? '' : 'extrinsic', subBuilder: Identity_AddAuthorization_Data.create)
    ..aOM<Identity_AddAuthorization_Data>(3, _omitFieldNames ? '' : 'portfolio', subBuilder: Identity_AddAuthorization_Data.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Identity_AddAuthorization_AuthData clone() => Identity_AddAuthorization_AuthData()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Identity_AddAuthorization_AuthData copyWith(void Function(Identity_AddAuthorization_AuthData) updates) => super.copyWith((message) => updates(message as Identity_AddAuthorization_AuthData)) as Identity_AddAuthorization_AuthData;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Identity_AddAuthorization_AuthData create() => Identity_AddAuthorization_AuthData._();
  Identity_AddAuthorization_AuthData createEmptyInstance() => create();
  static $pb.PbList<Identity_AddAuthorization_AuthData> createRepeated() => $pb.PbList<Identity_AddAuthorization_AuthData>();
  @$core.pragma('dart2js:noInline')
  static Identity_AddAuthorization_AuthData getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Identity_AddAuthorization_AuthData>(create);
  static Identity_AddAuthorization_AuthData? _defaultInstance;

  /// authorization data, empty means all permissions, null means no permissions
  @$pb.TagNumber(1)
  Identity_AddAuthorization_Data get asset => $_getN(0);
  @$pb.TagNumber(1)
  set asset(Identity_AddAuthorization_Data v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasAsset() => $_has(0);
  @$pb.TagNumber(1)
  void clearAsset() => clearField(1);
  @$pb.TagNumber(1)
  Identity_AddAuthorization_Data ensureAsset() => $_ensure(0);

  /// authorization data, empty means all permissions, null means no permissions
  @$pb.TagNumber(2)
  Identity_AddAuthorization_Data get extrinsic => $_getN(1);
  @$pb.TagNumber(2)
  set extrinsic(Identity_AddAuthorization_Data v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasExtrinsic() => $_has(1);
  @$pb.TagNumber(2)
  void clearExtrinsic() => clearField(2);
  @$pb.TagNumber(2)
  Identity_AddAuthorization_Data ensureExtrinsic() => $_ensure(1);

  /// authorization data, empty means all permissions, null means no permissions
  @$pb.TagNumber(3)
  Identity_AddAuthorization_Data get portfolio => $_getN(2);
  @$pb.TagNumber(3)
  set portfolio(Identity_AddAuthorization_Data v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasPortfolio() => $_has(2);
  @$pb.TagNumber(3)
  void clearPortfolio() => clearField(3);
  @$pb.TagNumber(3)
  Identity_AddAuthorization_Data ensurePortfolio() => $_ensure(2);
}

/// Identity::add_authorization call
class Identity_AddAuthorization extends $pb.GeneratedMessage {
  factory Identity_AddAuthorization({
    CallIndices? callIndices,
    $core.String? target,
    Identity_AddAuthorization_AuthData? data,
    $fixnum.Int64? expiry,
  }) {
    final $result = create();
    if (callIndices != null) {
      $result.callIndices = callIndices;
    }
    if (target != null) {
      $result.target = target;
    }
    if (data != null) {
      $result.data = data;
    }
    if (expiry != null) {
      $result.expiry = expiry;
    }
    return $result;
  }
  Identity_AddAuthorization._() : super();
  factory Identity_AddAuthorization.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Identity_AddAuthorization.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Identity.AddAuthorization', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..aOM<CallIndices>(1, _omitFieldNames ? '' : 'callIndices', subBuilder: CallIndices.create)
    ..aOS(2, _omitFieldNames ? '' : 'target')
    ..aOM<Identity_AddAuthorization_AuthData>(3, _omitFieldNames ? '' : 'data', subBuilder: Identity_AddAuthorization_AuthData.create)
    ..a<$fixnum.Int64>(4, _omitFieldNames ? '' : 'expiry', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Identity_AddAuthorization clone() => Identity_AddAuthorization()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Identity_AddAuthorization copyWith(void Function(Identity_AddAuthorization) updates) => super.copyWith((message) => updates(message as Identity_AddAuthorization)) as Identity_AddAuthorization;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Identity_AddAuthorization create() => Identity_AddAuthorization._();
  Identity_AddAuthorization createEmptyInstance() => create();
  static $pb.PbList<Identity_AddAuthorization> createRepeated() => $pb.PbList<Identity_AddAuthorization>();
  @$core.pragma('dart2js:noInline')
  static Identity_AddAuthorization getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Identity_AddAuthorization>(create);
  static Identity_AddAuthorization? _defaultInstance;

  /// call indices
  @$pb.TagNumber(1)
  CallIndices get callIndices => $_getN(0);
  @$pb.TagNumber(1)
  set callIndices(CallIndices v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasCallIndices() => $_has(0);
  @$pb.TagNumber(1)
  void clearCallIndices() => clearField(1);
  @$pb.TagNumber(1)
  CallIndices ensureCallIndices() => $_ensure(0);

  /// address that will be added to the Identity
  @$pb.TagNumber(2)
  $core.String get target => $_getSZ(1);
  @$pb.TagNumber(2)
  set target($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTarget() => $_has(1);
  @$pb.TagNumber(2)
  void clearTarget() => clearField(2);

  /// authorization data, null means all permissions
  @$pb.TagNumber(3)
  Identity_AddAuthorization_AuthData get data => $_getN(2);
  @$pb.TagNumber(3)
  set data(Identity_AddAuthorization_AuthData v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasData() => $_has(2);
  @$pb.TagNumber(3)
  void clearData() => clearField(3);
  @$pb.TagNumber(3)
  Identity_AddAuthorization_AuthData ensureData() => $_ensure(2);

  /// expire time, unix seconds
  @$pb.TagNumber(4)
  $fixnum.Int64 get expiry => $_getI64(3);
  @$pb.TagNumber(4)
  set expiry($fixnum.Int64 v) { $_setInt64(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasExpiry() => $_has(3);
  @$pb.TagNumber(4)
  void clearExpiry() => clearField(4);
}

enum Identity_MessageOneof {
  joinIdentityAsKey, 
  addAuthorization, 
  notSet
}

/// Identity module
class Identity extends $pb.GeneratedMessage {
  factory Identity({
    Identity_JoinIdentityAsKey? joinIdentityAsKey,
    Identity_AddAuthorization? addAuthorization,
  }) {
    final $result = create();
    if (joinIdentityAsKey != null) {
      $result.joinIdentityAsKey = joinIdentityAsKey;
    }
    if (addAuthorization != null) {
      $result.addAuthorization = addAuthorization;
    }
    return $result;
  }
  Identity._() : super();
  factory Identity.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Identity.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Identity_MessageOneof> _Identity_MessageOneofByTag = {
    1 : Identity_MessageOneof.joinIdentityAsKey,
    2 : Identity_MessageOneof.addAuthorization,
    0 : Identity_MessageOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Identity', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2])
    ..aOM<Identity_JoinIdentityAsKey>(1, _omitFieldNames ? '' : 'joinIdentityAsKey', subBuilder: Identity_JoinIdentityAsKey.create)
    ..aOM<Identity_AddAuthorization>(2, _omitFieldNames ? '' : 'addAuthorization', subBuilder: Identity_AddAuthorization.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Identity clone() => Identity()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Identity copyWith(void Function(Identity) updates) => super.copyWith((message) => updates(message as Identity)) as Identity;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Identity create() => Identity._();
  Identity createEmptyInstance() => create();
  static $pb.PbList<Identity> createRepeated() => $pb.PbList<Identity>();
  @$core.pragma('dart2js:noInline')
  static Identity getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Identity>(create);
  static Identity? _defaultInstance;

  Identity_MessageOneof whichMessageOneof() => _Identity_MessageOneofByTag[$_whichOneof(0)]!;
  void clearMessageOneof() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  Identity_JoinIdentityAsKey get joinIdentityAsKey => $_getN(0);
  @$pb.TagNumber(1)
  set joinIdentityAsKey(Identity_JoinIdentityAsKey v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasJoinIdentityAsKey() => $_has(0);
  @$pb.TagNumber(1)
  void clearJoinIdentityAsKey() => clearField(1);
  @$pb.TagNumber(1)
  Identity_JoinIdentityAsKey ensureJoinIdentityAsKey() => $_ensure(0);

  @$pb.TagNumber(2)
  Identity_AddAuthorization get addAuthorization => $_getN(1);
  @$pb.TagNumber(2)
  set addAuthorization(Identity_AddAuthorization v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasAddAuthorization() => $_has(1);
  @$pb.TagNumber(2)
  void clearAddAuthorization() => clearField(2);
  @$pb.TagNumber(2)
  Identity_AddAuthorization ensureAddAuthorization() => $_ensure(1);
}

enum PolymeshCall_MessageOneof {
  identityCall, 
  notSet
}

/// Polymesh call
class PolymeshCall extends $pb.GeneratedMessage {
  factory PolymeshCall({
    Identity? identityCall,
  }) {
    final $result = create();
    if (identityCall != null) {
      $result.identityCall = identityCall;
    }
    return $result;
  }
  PolymeshCall._() : super();
  factory PolymeshCall.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PolymeshCall.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, PolymeshCall_MessageOneof> _PolymeshCall_MessageOneofByTag = {
    2 : PolymeshCall_MessageOneof.identityCall,
    0 : PolymeshCall_MessageOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'PolymeshCall', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..oo(0, [2])
    ..aOM<Identity>(2, _omitFieldNames ? '' : 'identityCall', subBuilder: Identity.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  PolymeshCall clone() => PolymeshCall()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  PolymeshCall copyWith(void Function(PolymeshCall) updates) => super.copyWith((message) => updates(message as PolymeshCall)) as PolymeshCall;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static PolymeshCall create() => PolymeshCall._();
  PolymeshCall createEmptyInstance() => create();
  static $pb.PbList<PolymeshCall> createRepeated() => $pb.PbList<PolymeshCall>();
  @$core.pragma('dart2js:noInline')
  static PolymeshCall getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PolymeshCall>(create);
  static PolymeshCall? _defaultInstance;

  PolymeshCall_MessageOneof whichMessageOneof() => _PolymeshCall_MessageOneofByTag[$_whichOneof(0)]!;
  void clearMessageOneof() => clearField($_whichOneof(0));

  @$pb.TagNumber(2)
  Identity get identityCall => $_getN(0);
  @$pb.TagNumber(2)
  set identityCall(Identity v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasIdentityCall() => $_has(0);
  @$pb.TagNumber(2)
  void clearIdentityCall() => clearField(2);
  @$pb.TagNumber(2)
  Identity ensureIdentityCall() => $_ensure(0);
}

enum SigningInput_MessageOneof {
  balanceCall, 
  stakingCall, 
  polymeshCall, 
  notSet
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.List<$core.int>? blockHash,
    $core.List<$core.int>? genesisHash,
    $fixnum.Int64? nonce,
    $core.int? specVersion,
    $core.int? transactionVersion,
    $core.List<$core.int>? tip,
    Era? era,
    $core.List<$core.int>? privateKey,
    $core.int? network,
    $core.bool? multiAddress,
    Balance? balanceCall,
    Staking? stakingCall,
    PolymeshCall? polymeshCall,
  }) {
    final $result = create();
    if (blockHash != null) {
      $result.blockHash = blockHash;
    }
    if (genesisHash != null) {
      $result.genesisHash = genesisHash;
    }
    if (nonce != null) {
      $result.nonce = nonce;
    }
    if (specVersion != null) {
      $result.specVersion = specVersion;
    }
    if (transactionVersion != null) {
      $result.transactionVersion = transactionVersion;
    }
    if (tip != null) {
      $result.tip = tip;
    }
    if (era != null) {
      $result.era = era;
    }
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (network != null) {
      $result.network = network;
    }
    if (multiAddress != null) {
      $result.multiAddress = multiAddress;
    }
    if (balanceCall != null) {
      $result.balanceCall = balanceCall;
    }
    if (stakingCall != null) {
      $result.stakingCall = stakingCall;
    }
    if (polymeshCall != null) {
      $result.polymeshCall = polymeshCall;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, SigningInput_MessageOneof> _SigningInput_MessageOneofByTag = {
    11 : SigningInput_MessageOneof.balanceCall,
    12 : SigningInput_MessageOneof.stakingCall,
    13 : SigningInput_MessageOneof.polymeshCall,
    0 : SigningInput_MessageOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..oo(0, [11, 12, 13])
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'blockHash', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'genesisHash', $pb.PbFieldType.OY)
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'nonce', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.int>(4, _omitFieldNames ? '' : 'specVersion', $pb.PbFieldType.OU3)
    ..a<$core.int>(5, _omitFieldNames ? '' : 'transactionVersion', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(6, _omitFieldNames ? '' : 'tip', $pb.PbFieldType.OY)
    ..aOM<Era>(7, _omitFieldNames ? '' : 'era', subBuilder: Era.create)
    ..a<$core.List<$core.int>>(8, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..a<$core.int>(9, _omitFieldNames ? '' : 'network', $pb.PbFieldType.OU3)
    ..aOB(10, _omitFieldNames ? '' : 'multiAddress')
    ..aOM<Balance>(11, _omitFieldNames ? '' : 'balanceCall', subBuilder: Balance.create)
    ..aOM<Staking>(12, _omitFieldNames ? '' : 'stakingCall', subBuilder: Staking.create)
    ..aOM<PolymeshCall>(13, _omitFieldNames ? '' : 'polymeshCall', subBuilder: PolymeshCall.create)
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

  /// Recent block hash, or genesis hash if era is not set
  @$pb.TagNumber(1)
  $core.List<$core.int> get blockHash => $_getN(0);
  @$pb.TagNumber(1)
  set blockHash($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasBlockHash() => $_has(0);
  @$pb.TagNumber(1)
  void clearBlockHash() => clearField(1);

  /// Genesis block hash (identifies the chain)
  @$pb.TagNumber(2)
  $core.List<$core.int> get genesisHash => $_getN(1);
  @$pb.TagNumber(2)
  set genesisHash($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasGenesisHash() => $_has(1);
  @$pb.TagNumber(2)
  void clearGenesisHash() => clearField(2);

  /// Current account nonce
  @$pb.TagNumber(3)
  $fixnum.Int64 get nonce => $_getI64(2);
  @$pb.TagNumber(3)
  set nonce($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasNonce() => $_has(2);
  @$pb.TagNumber(3)
  void clearNonce() => clearField(3);

  /// Specification version, e.g. 26.
  @$pb.TagNumber(4)
  $core.int get specVersion => $_getIZ(3);
  @$pb.TagNumber(4)
  set specVersion($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasSpecVersion() => $_has(3);
  @$pb.TagNumber(4)
  void clearSpecVersion() => clearField(4);

  /// Transaction version, e.g. 5.
  @$pb.TagNumber(5)
  $core.int get transactionVersion => $_getIZ(4);
  @$pb.TagNumber(5)
  set transactionVersion($core.int v) { $_setUnsignedInt32(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasTransactionVersion() => $_has(4);
  @$pb.TagNumber(5)
  void clearTransactionVersion() => clearField(5);

  /// Optional tip to pay, big integer
  @$pb.TagNumber(6)
  $core.List<$core.int> get tip => $_getN(5);
  @$pb.TagNumber(6)
  set tip($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasTip() => $_has(5);
  @$pb.TagNumber(6)
  void clearTip() => clearField(6);

  /// Optional time validity limit, recommended, for replay-protection.  Empty means Immortal.
  @$pb.TagNumber(7)
  Era get era => $_getN(6);
  @$pb.TagNumber(7)
  set era(Era v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasEra() => $_has(6);
  @$pb.TagNumber(7)
  void clearEra() => clearField(7);
  @$pb.TagNumber(7)
  Era ensureEra() => $_ensure(6);

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(8)
  $core.List<$core.int> get privateKey => $_getN(7);
  @$pb.TagNumber(8)
  set privateKey($core.List<$core.int> v) { $_setBytes(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasPrivateKey() => $_has(7);
  @$pb.TagNumber(8)
  void clearPrivateKey() => clearField(8);

  /// Network type
  @$pb.TagNumber(9)
  $core.int get network => $_getIZ(8);
  @$pb.TagNumber(9)
  set network($core.int v) { $_setUnsignedInt32(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasNetwork() => $_has(8);
  @$pb.TagNumber(9)
  void clearNetwork() => clearField(9);

  /// Whether enable MultiAddress
  @$pb.TagNumber(10)
  $core.bool get multiAddress => $_getBF(9);
  @$pb.TagNumber(10)
  set multiAddress($core.bool v) { $_setBool(9, v); }
  @$pb.TagNumber(10)
  $core.bool hasMultiAddress() => $_has(9);
  @$pb.TagNumber(10)
  void clearMultiAddress() => clearField(10);

  @$pb.TagNumber(11)
  Balance get balanceCall => $_getN(10);
  @$pb.TagNumber(11)
  set balanceCall(Balance v) { setField(11, v); }
  @$pb.TagNumber(11)
  $core.bool hasBalanceCall() => $_has(10);
  @$pb.TagNumber(11)
  void clearBalanceCall() => clearField(11);
  @$pb.TagNumber(11)
  Balance ensureBalanceCall() => $_ensure(10);

  @$pb.TagNumber(12)
  Staking get stakingCall => $_getN(11);
  @$pb.TagNumber(12)
  set stakingCall(Staking v) { setField(12, v); }
  @$pb.TagNumber(12)
  $core.bool hasStakingCall() => $_has(11);
  @$pb.TagNumber(12)
  void clearStakingCall() => clearField(12);
  @$pb.TagNumber(12)
  Staking ensureStakingCall() => $_ensure(11);

  @$pb.TagNumber(13)
  PolymeshCall get polymeshCall => $_getN(12);
  @$pb.TagNumber(13)
  set polymeshCall(PolymeshCall v) { setField(13, v); }
  @$pb.TagNumber(13)
  $core.bool hasPolymeshCall() => $_has(12);
  @$pb.TagNumber(13)
  void clearPolymeshCall() => clearField(13);
  @$pb.TagNumber(13)
  PolymeshCall ensurePolymeshCall() => $_ensure(12);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.List<$core.int>? encoded,
    $0.SigningError? error,
    $core.String? errorMessage,
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
    return $result;
  }
  SigningOutput._() : super();
  factory SigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Polkadot.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'encoded', $pb.PbFieldType.OY)
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

  /// Signed and encoded transaction bytes.
  @$pb.TagNumber(1)
  $core.List<$core.int> get encoded => $_getN(0);
  @$pb.TagNumber(1)
  set encoded($core.List<$core.int> v) { $_setBytes(0, v); }
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
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
