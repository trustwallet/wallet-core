//
//  Generated code. Do not modify.
//  source: Nervos.proto
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

/// Nervos transaction plan
class TransactionPlan extends $pb.GeneratedMessage {
  factory TransactionPlan({
    $core.Iterable<CellDep>? cellDeps,
    $core.Iterable<$core.List<$core.int>>? headerDeps,
    $core.Iterable<Cell>? selectedCells,
    $core.Iterable<CellOutput>? outputs,
    $core.Iterable<$core.List<$core.int>>? outputsData,
    $0.SigningError? error,
  }) {
    final $result = create();
    if (cellDeps != null) {
      $result.cellDeps.addAll(cellDeps);
    }
    if (headerDeps != null) {
      $result.headerDeps.addAll(headerDeps);
    }
    if (selectedCells != null) {
      $result.selectedCells.addAll(selectedCells);
    }
    if (outputs != null) {
      $result.outputs.addAll(outputs);
    }
    if (outputsData != null) {
      $result.outputsData.addAll(outputsData);
    }
    if (error != null) {
      $result.error = error;
    }
    return $result;
  }
  TransactionPlan._() : super();
  factory TransactionPlan.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TransactionPlan.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TransactionPlan', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nervos.Proto'), createEmptyInstance: create)
    ..pc<CellDep>(1, _omitFieldNames ? '' : 'cellDeps', $pb.PbFieldType.PM, subBuilder: CellDep.create)
    ..p<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'headerDeps', $pb.PbFieldType.PY)
    ..pc<Cell>(3, _omitFieldNames ? '' : 'selectedCells', $pb.PbFieldType.PM, subBuilder: Cell.create)
    ..pc<CellOutput>(4, _omitFieldNames ? '' : 'outputs', $pb.PbFieldType.PM, subBuilder: CellOutput.create)
    ..p<$core.List<$core.int>>(5, _omitFieldNames ? '' : 'outputsData', $pb.PbFieldType.PY)
    ..e<$0.SigningError>(6, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TransactionPlan clone() => TransactionPlan()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TransactionPlan copyWith(void Function(TransactionPlan) updates) => super.copyWith((message) => updates(message as TransactionPlan)) as TransactionPlan;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TransactionPlan create() => TransactionPlan._();
  TransactionPlan createEmptyInstance() => create();
  static $pb.PbList<TransactionPlan> createRepeated() => $pb.PbList<TransactionPlan>();
  @$core.pragma('dart2js:noInline')
  static TransactionPlan getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TransactionPlan>(create);
  static TransactionPlan? _defaultInstance;

  /// A list of cell deps.
  @$pb.TagNumber(1)
  $core.List<CellDep> get cellDeps => $_getList(0);

  /// A list of header deps.
  @$pb.TagNumber(2)
  $core.List<$core.List<$core.int>> get headerDeps => $_getList(1);

  /// A list of 1 or more selected cells for this transaction
  @$pb.TagNumber(3)
  $core.List<Cell> get selectedCells => $_getList(2);

  /// A list of 1 or more outputs by this transaction
  @$pb.TagNumber(4)
  $core.List<CellOutput> get outputs => $_getList(3);

  /// A list of outputs data.
  @$pb.TagNumber(5)
  $core.List<$core.List<$core.int>> get outputsData => $_getList(4);

  /// Optional error
  @$pb.TagNumber(6)
  $0.SigningError get error => $_getN(5);
  @$pb.TagNumber(6)
  set error($0.SigningError v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasError() => $_has(5);
  @$pb.TagNumber(6)
  void clearError() => clearField(6);
}

/// Nervos cell dep.
class CellDep extends $pb.GeneratedMessage {
  factory CellDep({
    $core.String? depType,
    OutPoint? outPoint,
  }) {
    final $result = create();
    if (depType != null) {
      $result.depType = depType;
    }
    if (outPoint != null) {
      $result.outPoint = outPoint;
    }
    return $result;
  }
  CellDep._() : super();
  factory CellDep.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory CellDep.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'CellDep', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nervos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'depType')
    ..aOM<OutPoint>(2, _omitFieldNames ? '' : 'outPoint', subBuilder: OutPoint.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  CellDep clone() => CellDep()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  CellDep copyWith(void Function(CellDep) updates) => super.copyWith((message) => updates(message as CellDep)) as CellDep;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static CellDep create() => CellDep._();
  CellDep createEmptyInstance() => create();
  static $pb.PbList<CellDep> createRepeated() => $pb.PbList<CellDep>();
  @$core.pragma('dart2js:noInline')
  static CellDep getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<CellDep>(create);
  static CellDep? _defaultInstance;

  /// Prevents the transaction to be mined before an absolute or relative time
  @$pb.TagNumber(1)
  $core.String get depType => $_getSZ(0);
  @$pb.TagNumber(1)
  set depType($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDepType() => $_has(0);
  @$pb.TagNumber(1)
  void clearDepType() => clearField(1);

  /// Reference to the previous transaction's output.
  @$pb.TagNumber(2)
  OutPoint get outPoint => $_getN(1);
  @$pb.TagNumber(2)
  set outPoint(OutPoint v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasOutPoint() => $_has(1);
  @$pb.TagNumber(2)
  void clearOutPoint() => clearField(2);
  @$pb.TagNumber(2)
  OutPoint ensureOutPoint() => $_ensure(1);
}

/// Nervos transaction out-point reference.
class OutPoint extends $pb.GeneratedMessage {
  factory OutPoint({
    $core.List<$core.int>? txHash,
    $core.int? index,
  }) {
    final $result = create();
    if (txHash != null) {
      $result.txHash = txHash;
    }
    if (index != null) {
      $result.index = index;
    }
    return $result;
  }
  OutPoint._() : super();
  factory OutPoint.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory OutPoint.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'OutPoint', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nervos.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'txHash', $pb.PbFieldType.OY)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'index', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  OutPoint clone() => OutPoint()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  OutPoint copyWith(void Function(OutPoint) updates) => super.copyWith((message) => updates(message as OutPoint)) as OutPoint;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static OutPoint create() => OutPoint._();
  OutPoint createEmptyInstance() => create();
  static $pb.PbList<OutPoint> createRepeated() => $pb.PbList<OutPoint>();
  @$core.pragma('dart2js:noInline')
  static OutPoint getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<OutPoint>(create);
  static OutPoint? _defaultInstance;

  /// The hash of the referenced transaction.
  @$pb.TagNumber(1)
  $core.List<$core.int> get txHash => $_getN(0);
  @$pb.TagNumber(1)
  set txHash($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasTxHash() => $_has(0);
  @$pb.TagNumber(1)
  void clearTxHash() => clearField(1);

  /// The index of the specific output in the transaction.
  @$pb.TagNumber(2)
  $core.int get index => $_getIZ(1);
  @$pb.TagNumber(2)
  set index($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasIndex() => $_has(1);
  @$pb.TagNumber(2)
  void clearIndex() => clearField(2);
}

/// Nervos cell output.
class CellOutput extends $pb.GeneratedMessage {
  factory CellOutput({
    $fixnum.Int64? capacity,
    Script? lock,
    Script? type,
  }) {
    final $result = create();
    if (capacity != null) {
      $result.capacity = capacity;
    }
    if (lock != null) {
      $result.lock = lock;
    }
    if (type != null) {
      $result.type = type;
    }
    return $result;
  }
  CellOutput._() : super();
  factory CellOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory CellOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'CellOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nervos.Proto'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'capacity', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOM<Script>(2, _omitFieldNames ? '' : 'lock', subBuilder: Script.create)
    ..aOM<Script>(3, _omitFieldNames ? '' : 'type', subBuilder: Script.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  CellOutput clone() => CellOutput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  CellOutput copyWith(void Function(CellOutput) updates) => super.copyWith((message) => updates(message as CellOutput)) as CellOutput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static CellOutput create() => CellOutput._();
  CellOutput createEmptyInstance() => create();
  static $pb.PbList<CellOutput> createRepeated() => $pb.PbList<CellOutput>();
  @$core.pragma('dart2js:noInline')
  static CellOutput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<CellOutput>(create);
  static CellOutput? _defaultInstance;

  /// Transaction amount.
  @$pb.TagNumber(1)
  $fixnum.Int64 get capacity => $_getI64(0);
  @$pb.TagNumber(1)
  set capacity($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasCapacity() => $_has(0);
  @$pb.TagNumber(1)
  void clearCapacity() => clearField(1);

  /// Lock script
  @$pb.TagNumber(2)
  Script get lock => $_getN(1);
  @$pb.TagNumber(2)
  set lock(Script v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasLock() => $_has(1);
  @$pb.TagNumber(2)
  void clearLock() => clearField(2);
  @$pb.TagNumber(2)
  Script ensureLock() => $_ensure(1);

  /// Type script
  @$pb.TagNumber(3)
  Script get type => $_getN(2);
  @$pb.TagNumber(3)
  set type(Script v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasType() => $_has(2);
  @$pb.TagNumber(3)
  void clearType() => clearField(3);
  @$pb.TagNumber(3)
  Script ensureType() => $_ensure(2);
}

/// Nervos script
class Script extends $pb.GeneratedMessage {
  factory Script({
    $core.List<$core.int>? codeHash,
    $core.String? hashType,
    $core.List<$core.int>? args,
  }) {
    final $result = create();
    if (codeHash != null) {
      $result.codeHash = codeHash;
    }
    if (hashType != null) {
      $result.hashType = hashType;
    }
    if (args != null) {
      $result.args = args;
    }
    return $result;
  }
  Script._() : super();
  factory Script.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Script.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Script', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nervos.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'codeHash', $pb.PbFieldType.OY)
    ..aOS(2, _omitFieldNames ? '' : 'hashType')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'args', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Script clone() => Script()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Script copyWith(void Function(Script) updates) => super.copyWith((message) => updates(message as Script)) as Script;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Script create() => Script._();
  Script createEmptyInstance() => create();
  static $pb.PbList<Script> createRepeated() => $pb.PbList<Script>();
  @$core.pragma('dart2js:noInline')
  static Script getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Script>(create);
  static Script? _defaultInstance;

  /// Code hash
  @$pb.TagNumber(1)
  $core.List<$core.int> get codeHash => $_getN(0);
  @$pb.TagNumber(1)
  set codeHash($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasCodeHash() => $_has(0);
  @$pb.TagNumber(1)
  void clearCodeHash() => clearField(1);

  /// Hash type
  @$pb.TagNumber(2)
  $core.String get hashType => $_getSZ(1);
  @$pb.TagNumber(2)
  set hashType($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasHashType() => $_has(1);
  @$pb.TagNumber(2)
  void clearHashType() => clearField(2);

  /// args
  @$pb.TagNumber(3)
  $core.List<$core.int> get args => $_getN(2);
  @$pb.TagNumber(3)
  set args($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasArgs() => $_has(2);
  @$pb.TagNumber(3)
  void clearArgs() => clearField(3);
}

/// Transfer of native asset
class NativeTransfer extends $pb.GeneratedMessage {
  factory NativeTransfer({
    $core.String? toAddress,
    $core.String? changeAddress,
    $fixnum.Int64? amount,
    $core.bool? useMaxAmount,
  }) {
    final $result = create();
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (changeAddress != null) {
      $result.changeAddress = changeAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (useMaxAmount != null) {
      $result.useMaxAmount = useMaxAmount;
    }
    return $result;
  }
  NativeTransfer._() : super();
  factory NativeTransfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory NativeTransfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'NativeTransfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nervos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'toAddress')
    ..aOS(2, _omitFieldNames ? '' : 'changeAddress')
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOB(4, _omitFieldNames ? '' : 'useMaxAmount')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  NativeTransfer clone() => NativeTransfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  NativeTransfer copyWith(void Function(NativeTransfer) updates) => super.copyWith((message) => updates(message as NativeTransfer)) as NativeTransfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static NativeTransfer create() => NativeTransfer._();
  NativeTransfer createEmptyInstance() => create();
  static $pb.PbList<NativeTransfer> createRepeated() => $pb.PbList<NativeTransfer>();
  @$core.pragma('dart2js:noInline')
  static NativeTransfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<NativeTransfer>(create);
  static NativeTransfer? _defaultInstance;

  /// Recipient's address.
  @$pb.TagNumber(1)
  $core.String get toAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set toAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasToAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearToAddress() => clearField(1);

  /// Change address.
  @$pb.TagNumber(2)
  $core.String get changeAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set changeAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasChangeAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearChangeAddress() => clearField(2);

  /// Amount to send.
  @$pb.TagNumber(3)
  $fixnum.Int64 get amount => $_getI64(2);
  @$pb.TagNumber(3)
  set amount($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);

  /// If sending max amount.
  @$pb.TagNumber(4)
  $core.bool get useMaxAmount => $_getBF(3);
  @$pb.TagNumber(4)
  set useMaxAmount($core.bool v) { $_setBool(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasUseMaxAmount() => $_has(3);
  @$pb.TagNumber(4)
  void clearUseMaxAmount() => clearField(4);
}

/// Token transfer (SUDT)
class SudtTransfer extends $pb.GeneratedMessage {
  factory SudtTransfer({
    $core.String? toAddress,
    $core.String? changeAddress,
    $core.List<$core.int>? sudtAddress,
    $core.String? amount,
    $core.bool? useMaxAmount,
  }) {
    final $result = create();
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (changeAddress != null) {
      $result.changeAddress = changeAddress;
    }
    if (sudtAddress != null) {
      $result.sudtAddress = sudtAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (useMaxAmount != null) {
      $result.useMaxAmount = useMaxAmount;
    }
    return $result;
  }
  SudtTransfer._() : super();
  factory SudtTransfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SudtTransfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SudtTransfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nervos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'toAddress')
    ..aOS(2, _omitFieldNames ? '' : 'changeAddress')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'sudtAddress', $pb.PbFieldType.OY)
    ..aOS(4, _omitFieldNames ? '' : 'amount')
    ..aOB(5, _omitFieldNames ? '' : 'useMaxAmount')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SudtTransfer clone() => SudtTransfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SudtTransfer copyWith(void Function(SudtTransfer) updates) => super.copyWith((message) => updates(message as SudtTransfer)) as SudtTransfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SudtTransfer create() => SudtTransfer._();
  SudtTransfer createEmptyInstance() => create();
  static $pb.PbList<SudtTransfer> createRepeated() => $pb.PbList<SudtTransfer>();
  @$core.pragma('dart2js:noInline')
  static SudtTransfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SudtTransfer>(create);
  static SudtTransfer? _defaultInstance;

  /// Recipient's address.
  @$pb.TagNumber(1)
  $core.String get toAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set toAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasToAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearToAddress() => clearField(1);

  /// Change address.
  @$pb.TagNumber(2)
  $core.String get changeAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set changeAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasChangeAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearChangeAddress() => clearField(2);

  /// SUDT (Simple User Defined Token) address
  @$pb.TagNumber(3)
  $core.List<$core.int> get sudtAddress => $_getN(2);
  @$pb.TagNumber(3)
  set sudtAddress($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSudtAddress() => $_has(2);
  @$pb.TagNumber(3)
  void clearSudtAddress() => clearField(3);

  /// Amount to send.
  @$pb.TagNumber(4)
  $core.String get amount => $_getSZ(3);
  @$pb.TagNumber(4)
  set amount($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasAmount() => $_has(3);
  @$pb.TagNumber(4)
  void clearAmount() => clearField(4);

  /// If sending max amount.
  @$pb.TagNumber(5)
  $core.bool get useMaxAmount => $_getBF(4);
  @$pb.TagNumber(5)
  set useMaxAmount($core.bool v) { $_setBool(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasUseMaxAmount() => $_has(4);
  @$pb.TagNumber(5)
  void clearUseMaxAmount() => clearField(5);
}

/// Deposit
class DaoDeposit extends $pb.GeneratedMessage {
  factory DaoDeposit({
    $core.String? toAddress,
    $core.String? changeAddress,
    $fixnum.Int64? amount,
  }) {
    final $result = create();
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (changeAddress != null) {
      $result.changeAddress = changeAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    return $result;
  }
  DaoDeposit._() : super();
  factory DaoDeposit.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DaoDeposit.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DaoDeposit', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nervos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'toAddress')
    ..aOS(2, _omitFieldNames ? '' : 'changeAddress')
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DaoDeposit clone() => DaoDeposit()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DaoDeposit copyWith(void Function(DaoDeposit) updates) => super.copyWith((message) => updates(message as DaoDeposit)) as DaoDeposit;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DaoDeposit create() => DaoDeposit._();
  DaoDeposit createEmptyInstance() => create();
  static $pb.PbList<DaoDeposit> createRepeated() => $pb.PbList<DaoDeposit>();
  @$core.pragma('dart2js:noInline')
  static DaoDeposit getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DaoDeposit>(create);
  static DaoDeposit? _defaultInstance;

  /// Recipient's address.
  @$pb.TagNumber(1)
  $core.String get toAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set toAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasToAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearToAddress() => clearField(1);

  /// Change address.
  @$pb.TagNumber(2)
  $core.String get changeAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set changeAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasChangeAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearChangeAddress() => clearField(2);

  /// Amount to deposit.
  @$pb.TagNumber(3)
  $fixnum.Int64 get amount => $_getI64(2);
  @$pb.TagNumber(3)
  set amount($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);
}

class DaoWithdrawPhase1 extends $pb.GeneratedMessage {
  factory DaoWithdrawPhase1({
    Cell? depositCell,
    $core.String? changeAddress,
  }) {
    final $result = create();
    if (depositCell != null) {
      $result.depositCell = depositCell;
    }
    if (changeAddress != null) {
      $result.changeAddress = changeAddress;
    }
    return $result;
  }
  DaoWithdrawPhase1._() : super();
  factory DaoWithdrawPhase1.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DaoWithdrawPhase1.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DaoWithdrawPhase1', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nervos.Proto'), createEmptyInstance: create)
    ..aOM<Cell>(1, _omitFieldNames ? '' : 'depositCell', subBuilder: Cell.create)
    ..aOS(2, _omitFieldNames ? '' : 'changeAddress')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DaoWithdrawPhase1 clone() => DaoWithdrawPhase1()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DaoWithdrawPhase1 copyWith(void Function(DaoWithdrawPhase1) updates) => super.copyWith((message) => updates(message as DaoWithdrawPhase1)) as DaoWithdrawPhase1;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DaoWithdrawPhase1 create() => DaoWithdrawPhase1._();
  DaoWithdrawPhase1 createEmptyInstance() => create();
  static $pb.PbList<DaoWithdrawPhase1> createRepeated() => $pb.PbList<DaoWithdrawPhase1>();
  @$core.pragma('dart2js:noInline')
  static DaoWithdrawPhase1 getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DaoWithdrawPhase1>(create);
  static DaoWithdrawPhase1? _defaultInstance;

  /// Deposit cell
  @$pb.TagNumber(1)
  Cell get depositCell => $_getN(0);
  @$pb.TagNumber(1)
  set depositCell(Cell v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasDepositCell() => $_has(0);
  @$pb.TagNumber(1)
  void clearDepositCell() => clearField(1);
  @$pb.TagNumber(1)
  Cell ensureDepositCell() => $_ensure(0);

  /// Change address.
  @$pb.TagNumber(2)
  $core.String get changeAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set changeAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasChangeAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearChangeAddress() => clearField(2);
}

class DaoWithdrawPhase2 extends $pb.GeneratedMessage {
  factory DaoWithdrawPhase2({
    Cell? depositCell,
    Cell? withdrawingCell,
    $fixnum.Int64? amount,
  }) {
    final $result = create();
    if (depositCell != null) {
      $result.depositCell = depositCell;
    }
    if (withdrawingCell != null) {
      $result.withdrawingCell = withdrawingCell;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    return $result;
  }
  DaoWithdrawPhase2._() : super();
  factory DaoWithdrawPhase2.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DaoWithdrawPhase2.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DaoWithdrawPhase2', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nervos.Proto'), createEmptyInstance: create)
    ..aOM<Cell>(1, _omitFieldNames ? '' : 'depositCell', subBuilder: Cell.create)
    ..aOM<Cell>(2, _omitFieldNames ? '' : 'withdrawingCell', subBuilder: Cell.create)
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DaoWithdrawPhase2 clone() => DaoWithdrawPhase2()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DaoWithdrawPhase2 copyWith(void Function(DaoWithdrawPhase2) updates) => super.copyWith((message) => updates(message as DaoWithdrawPhase2)) as DaoWithdrawPhase2;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DaoWithdrawPhase2 create() => DaoWithdrawPhase2._();
  DaoWithdrawPhase2 createEmptyInstance() => create();
  static $pb.PbList<DaoWithdrawPhase2> createRepeated() => $pb.PbList<DaoWithdrawPhase2>();
  @$core.pragma('dart2js:noInline')
  static DaoWithdrawPhase2 getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DaoWithdrawPhase2>(create);
  static DaoWithdrawPhase2? _defaultInstance;

  /// Deposit cell
  @$pb.TagNumber(1)
  Cell get depositCell => $_getN(0);
  @$pb.TagNumber(1)
  set depositCell(Cell v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasDepositCell() => $_has(0);
  @$pb.TagNumber(1)
  void clearDepositCell() => clearField(1);
  @$pb.TagNumber(1)
  Cell ensureDepositCell() => $_ensure(0);

  /// Withdrawing cell
  @$pb.TagNumber(2)
  Cell get withdrawingCell => $_getN(1);
  @$pb.TagNumber(2)
  set withdrawingCell(Cell v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasWithdrawingCell() => $_has(1);
  @$pb.TagNumber(2)
  void clearWithdrawingCell() => clearField(2);
  @$pb.TagNumber(2)
  Cell ensureWithdrawingCell() => $_ensure(1);

  /// Amount
  @$pb.TagNumber(3)
  $fixnum.Int64 get amount => $_getI64(2);
  @$pb.TagNumber(3)
  set amount($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);
}

enum SigningInput_OperationOneof {
  nativeTransfer, 
  sudtTransfer, 
  daoDeposit, 
  daoWithdrawPhase1, 
  daoWithdrawPhase2, 
  notSet
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $fixnum.Int64? byteFee,
    $core.Iterable<$core.List<$core.int>>? privateKey,
    $core.Iterable<Cell>? cell,
    TransactionPlan? plan,
    NativeTransfer? nativeTransfer,
    SudtTransfer? sudtTransfer,
    DaoDeposit? daoDeposit,
    DaoWithdrawPhase1? daoWithdrawPhase1,
    DaoWithdrawPhase2? daoWithdrawPhase2,
  }) {
    final $result = create();
    if (byteFee != null) {
      $result.byteFee = byteFee;
    }
    if (privateKey != null) {
      $result.privateKey.addAll(privateKey);
    }
    if (cell != null) {
      $result.cell.addAll(cell);
    }
    if (plan != null) {
      $result.plan = plan;
    }
    if (nativeTransfer != null) {
      $result.nativeTransfer = nativeTransfer;
    }
    if (sudtTransfer != null) {
      $result.sudtTransfer = sudtTransfer;
    }
    if (daoDeposit != null) {
      $result.daoDeposit = daoDeposit;
    }
    if (daoWithdrawPhase1 != null) {
      $result.daoWithdrawPhase1 = daoWithdrawPhase1;
    }
    if (daoWithdrawPhase2 != null) {
      $result.daoWithdrawPhase2 = daoWithdrawPhase2;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, SigningInput_OperationOneof> _SigningInput_OperationOneofByTag = {
    5 : SigningInput_OperationOneof.nativeTransfer,
    6 : SigningInput_OperationOneof.sudtTransfer,
    7 : SigningInput_OperationOneof.daoDeposit,
    8 : SigningInput_OperationOneof.daoWithdrawPhase1,
    9 : SigningInput_OperationOneof.daoWithdrawPhase2,
    0 : SigningInput_OperationOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nervos.Proto'), createEmptyInstance: create)
    ..oo(0, [5, 6, 7, 8, 9])
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'byteFee', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..p<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.PY)
    ..pc<Cell>(3, _omitFieldNames ? '' : 'cell', $pb.PbFieldType.PM, subBuilder: Cell.create)
    ..aOM<TransactionPlan>(4, _omitFieldNames ? '' : 'plan', subBuilder: TransactionPlan.create)
    ..aOM<NativeTransfer>(5, _omitFieldNames ? '' : 'nativeTransfer', subBuilder: NativeTransfer.create)
    ..aOM<SudtTransfer>(6, _omitFieldNames ? '' : 'sudtTransfer', subBuilder: SudtTransfer.create)
    ..aOM<DaoDeposit>(7, _omitFieldNames ? '' : 'daoDeposit', subBuilder: DaoDeposit.create)
    ..aOM<DaoWithdrawPhase1>(8, _omitFieldNames ? '' : 'daoWithdrawPhase1', subBuilder: DaoWithdrawPhase1.create)
    ..aOM<DaoWithdrawPhase2>(9, _omitFieldNames ? '' : 'daoWithdrawPhase2', subBuilder: DaoWithdrawPhase2.create)
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

  /// Transaction fee per byte.
  @$pb.TagNumber(1)
  $fixnum.Int64 get byteFee => $_getI64(0);
  @$pb.TagNumber(1)
  set byteFee($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasByteFee() => $_has(0);
  @$pb.TagNumber(1)
  void clearByteFee() => clearField(1);

  /// The available secret private keys used for signing (32 bytes each).
  @$pb.TagNumber(2)
  $core.List<$core.List<$core.int>> get privateKey => $_getList(1);

  /// Available unspent cell outputs.
  @$pb.TagNumber(3)
  $core.List<Cell> get cell => $_getList(2);

  /// Optional transaction plan
  @$pb.TagNumber(4)
  TransactionPlan get plan => $_getN(3);
  @$pb.TagNumber(4)
  set plan(TransactionPlan v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasPlan() => $_has(3);
  @$pb.TagNumber(4)
  void clearPlan() => clearField(4);
  @$pb.TagNumber(4)
  TransactionPlan ensurePlan() => $_ensure(3);

  @$pb.TagNumber(5)
  NativeTransfer get nativeTransfer => $_getN(4);
  @$pb.TagNumber(5)
  set nativeTransfer(NativeTransfer v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasNativeTransfer() => $_has(4);
  @$pb.TagNumber(5)
  void clearNativeTransfer() => clearField(5);
  @$pb.TagNumber(5)
  NativeTransfer ensureNativeTransfer() => $_ensure(4);

  @$pb.TagNumber(6)
  SudtTransfer get sudtTransfer => $_getN(5);
  @$pb.TagNumber(6)
  set sudtTransfer(SudtTransfer v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasSudtTransfer() => $_has(5);
  @$pb.TagNumber(6)
  void clearSudtTransfer() => clearField(6);
  @$pb.TagNumber(6)
  SudtTransfer ensureSudtTransfer() => $_ensure(5);

  @$pb.TagNumber(7)
  DaoDeposit get daoDeposit => $_getN(6);
  @$pb.TagNumber(7)
  set daoDeposit(DaoDeposit v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasDaoDeposit() => $_has(6);
  @$pb.TagNumber(7)
  void clearDaoDeposit() => clearField(7);
  @$pb.TagNumber(7)
  DaoDeposit ensureDaoDeposit() => $_ensure(6);

  @$pb.TagNumber(8)
  DaoWithdrawPhase1 get daoWithdrawPhase1 => $_getN(7);
  @$pb.TagNumber(8)
  set daoWithdrawPhase1(DaoWithdrawPhase1 v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasDaoWithdrawPhase1() => $_has(7);
  @$pb.TagNumber(8)
  void clearDaoWithdrawPhase1() => clearField(8);
  @$pb.TagNumber(8)
  DaoWithdrawPhase1 ensureDaoWithdrawPhase1() => $_ensure(7);

  @$pb.TagNumber(9)
  DaoWithdrawPhase2 get daoWithdrawPhase2 => $_getN(8);
  @$pb.TagNumber(9)
  set daoWithdrawPhase2(DaoWithdrawPhase2 v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasDaoWithdrawPhase2() => $_has(8);
  @$pb.TagNumber(9)
  void clearDaoWithdrawPhase2() => clearField(9);
  @$pb.TagNumber(9)
  DaoWithdrawPhase2 ensureDaoWithdrawPhase2() => $_ensure(8);
}

/// An unspent cell output, that can serve as input to a transaction
class Cell extends $pb.GeneratedMessage {
  factory Cell({
    OutPoint? outPoint,
    $fixnum.Int64? capacity,
    Script? lock,
    Script? type,
    $core.List<$core.int>? data,
    $fixnum.Int64? blockNumber,
    $core.List<$core.int>? blockHash,
    $fixnum.Int64? since,
    $core.List<$core.int>? inputType,
    $core.List<$core.int>? outputType,
  }) {
    final $result = create();
    if (outPoint != null) {
      $result.outPoint = outPoint;
    }
    if (capacity != null) {
      $result.capacity = capacity;
    }
    if (lock != null) {
      $result.lock = lock;
    }
    if (type != null) {
      $result.type = type;
    }
    if (data != null) {
      $result.data = data;
    }
    if (blockNumber != null) {
      $result.blockNumber = blockNumber;
    }
    if (blockHash != null) {
      $result.blockHash = blockHash;
    }
    if (since != null) {
      $result.since = since;
    }
    if (inputType != null) {
      $result.inputType = inputType;
    }
    if (outputType != null) {
      $result.outputType = outputType;
    }
    return $result;
  }
  Cell._() : super();
  factory Cell.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Cell.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Cell', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nervos.Proto'), createEmptyInstance: create)
    ..aOM<OutPoint>(1, _omitFieldNames ? '' : 'outPoint', subBuilder: OutPoint.create)
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'capacity', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOM<Script>(3, _omitFieldNames ? '' : 'lock', subBuilder: Script.create)
    ..aOM<Script>(4, _omitFieldNames ? '' : 'type', subBuilder: Script.create)
    ..a<$core.List<$core.int>>(5, _omitFieldNames ? '' : 'data', $pb.PbFieldType.OY)
    ..a<$fixnum.Int64>(6, _omitFieldNames ? '' : 'blockNumber', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.List<$core.int>>(7, _omitFieldNames ? '' : 'blockHash', $pb.PbFieldType.OY)
    ..a<$fixnum.Int64>(8, _omitFieldNames ? '' : 'since', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.List<$core.int>>(9, _omitFieldNames ? '' : 'inputType', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(10, _omitFieldNames ? '' : 'outputType', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Cell clone() => Cell()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Cell copyWith(void Function(Cell) updates) => super.copyWith((message) => updates(message as Cell)) as Cell;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Cell create() => Cell._();
  Cell createEmptyInstance() => create();
  static $pb.PbList<Cell> createRepeated() => $pb.PbList<Cell>();
  @$core.pragma('dart2js:noInline')
  static Cell getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Cell>(create);
  static Cell? _defaultInstance;

  /// The unspent output
  @$pb.TagNumber(1)
  OutPoint get outPoint => $_getN(0);
  @$pb.TagNumber(1)
  set outPoint(OutPoint v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasOutPoint() => $_has(0);
  @$pb.TagNumber(1)
  void clearOutPoint() => clearField(1);
  @$pb.TagNumber(1)
  OutPoint ensureOutPoint() => $_ensure(0);

  /// Amount of the cell
  @$pb.TagNumber(2)
  $fixnum.Int64 get capacity => $_getI64(1);
  @$pb.TagNumber(2)
  set capacity($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasCapacity() => $_has(1);
  @$pb.TagNumber(2)
  void clearCapacity() => clearField(2);

  /// Lock script
  @$pb.TagNumber(3)
  Script get lock => $_getN(2);
  @$pb.TagNumber(3)
  set lock(Script v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasLock() => $_has(2);
  @$pb.TagNumber(3)
  void clearLock() => clearField(3);
  @$pb.TagNumber(3)
  Script ensureLock() => $_ensure(2);

  /// Type script
  @$pb.TagNumber(4)
  Script get type => $_getN(3);
  @$pb.TagNumber(4)
  set type(Script v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasType() => $_has(3);
  @$pb.TagNumber(4)
  void clearType() => clearField(4);
  @$pb.TagNumber(4)
  Script ensureType() => $_ensure(3);

  /// Data
  @$pb.TagNumber(5)
  $core.List<$core.int> get data => $_getN(4);
  @$pb.TagNumber(5)
  set data($core.List<$core.int> v) { $_setBytes(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasData() => $_has(4);
  @$pb.TagNumber(5)
  void clearData() => clearField(5);

  /// Optional block number
  @$pb.TagNumber(6)
  $fixnum.Int64 get blockNumber => $_getI64(5);
  @$pb.TagNumber(6)
  set blockNumber($fixnum.Int64 v) { $_setInt64(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasBlockNumber() => $_has(5);
  @$pb.TagNumber(6)
  void clearBlockNumber() => clearField(6);

  /// Optional block hash
  @$pb.TagNumber(7)
  $core.List<$core.int> get blockHash => $_getN(6);
  @$pb.TagNumber(7)
  set blockHash($core.List<$core.int> v) { $_setBytes(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasBlockHash() => $_has(6);
  @$pb.TagNumber(7)
  void clearBlockHash() => clearField(7);

  /// Optional since the cell is available to spend
  @$pb.TagNumber(8)
  $fixnum.Int64 get since => $_getI64(7);
  @$pb.TagNumber(8)
  set since($fixnum.Int64 v) { $_setInt64(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasSince() => $_has(7);
  @$pb.TagNumber(8)
  void clearSince() => clearField(8);

  /// Optional input type data to be included in witness
  @$pb.TagNumber(9)
  $core.List<$core.int> get inputType => $_getN(8);
  @$pb.TagNumber(9)
  set inputType($core.List<$core.int> v) { $_setBytes(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasInputType() => $_has(8);
  @$pb.TagNumber(9)
  void clearInputType() => clearField(9);

  /// Optional output type data to be included in witness
  @$pb.TagNumber(10)
  $core.List<$core.int> get outputType => $_getN(9);
  @$pb.TagNumber(10)
  set outputType($core.List<$core.int> v) { $_setBytes(9, v); }
  @$pb.TagNumber(10)
  $core.bool hasOutputType() => $_has(9);
  @$pb.TagNumber(10)
  void clearOutputType() => clearField(10);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.String? transactionJson,
    $core.String? transactionId,
    $0.SigningError? error,
  }) {
    final $result = create();
    if (transactionJson != null) {
      $result.transactionJson = transactionJson;
    }
    if (transactionId != null) {
      $result.transactionId = transactionId;
    }
    if (error != null) {
      $result.error = error;
    }
    return $result;
  }
  SigningOutput._() : super();
  factory SigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nervos.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'transactionJson')
    ..aOS(2, _omitFieldNames ? '' : 'transactionId')
    ..e<$0.SigningError>(3, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
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

  /// Resulting transaction. Note that the amount may be different than the requested amount to account for fees and available funds.
  @$pb.TagNumber(1)
  $core.String get transactionJson => $_getSZ(0);
  @$pb.TagNumber(1)
  set transactionJson($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasTransactionJson() => $_has(0);
  @$pb.TagNumber(1)
  void clearTransactionJson() => clearField(1);

  /// Transaction id
  @$pb.TagNumber(2)
  $core.String get transactionId => $_getSZ(1);
  @$pb.TagNumber(2)
  set transactionId($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTransactionId() => $_has(1);
  @$pb.TagNumber(2)
  void clearTransactionId() => clearField(2);

  /// Optional error
  @$pb.TagNumber(3)
  $0.SigningError get error => $_getN(2);
  @$pb.TagNumber(3)
  set error($0.SigningError v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasError() => $_has(2);
  @$pb.TagNumber(3)
  void clearError() => clearField(3);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
