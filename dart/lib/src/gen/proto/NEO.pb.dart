//
//  Generated code. Do not modify.
//  source: NEO.proto
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

/// Input for a transaction (output of a prev tx)
class TransactionInput extends $pb.GeneratedMessage {
  factory TransactionInput({
    $core.List<$core.int>? prevHash,
    $core.int? prevIndex,
    $fixnum.Int64? value,
    $core.String? assetId,
  }) {
    final $result = create();
    if (prevHash != null) {
      $result.prevHash = prevHash;
    }
    if (prevIndex != null) {
      $result.prevIndex = prevIndex;
    }
    if (value != null) {
      $result.value = value;
    }
    if (assetId != null) {
      $result.assetId = assetId;
    }
    return $result;
  }
  TransactionInput._() : super();
  factory TransactionInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TransactionInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TransactionInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEO.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'prevHash', $pb.PbFieldType.OY)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'prevIndex', $pb.PbFieldType.OF3)
    ..aInt64(3, _omitFieldNames ? '' : 'value')
    ..aOS(4, _omitFieldNames ? '' : 'assetId')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TransactionInput clone() => TransactionInput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TransactionInput copyWith(void Function(TransactionInput) updates) => super.copyWith((message) => updates(message as TransactionInput)) as TransactionInput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TransactionInput create() => TransactionInput._();
  TransactionInput createEmptyInstance() => create();
  static $pb.PbList<TransactionInput> createRepeated() => $pb.PbList<TransactionInput>();
  @$core.pragma('dart2js:noInline')
  static TransactionInput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TransactionInput>(create);
  static TransactionInput? _defaultInstance;

  /// Previous tx hash
  @$pb.TagNumber(1)
  $core.List<$core.int> get prevHash => $_getN(0);
  @$pb.TagNumber(1)
  set prevHash($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPrevHash() => $_has(0);
  @$pb.TagNumber(1)
  void clearPrevHash() => clearField(1);

  /// Output index
  @$pb.TagNumber(2)
  $core.int get prevIndex => $_getIZ(1);
  @$pb.TagNumber(2)
  set prevIndex($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPrevIndex() => $_has(1);
  @$pb.TagNumber(2)
  void clearPrevIndex() => clearField(2);

  /// unspent value of UTXO
  @$pb.TagNumber(3)
  $fixnum.Int64 get value => $_getI64(2);
  @$pb.TagNumber(3)
  set value($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasValue() => $_has(2);
  @$pb.TagNumber(3)
  void clearValue() => clearField(3);

  /// Asset
  @$pb.TagNumber(4)
  $core.String get assetId => $_getSZ(3);
  @$pb.TagNumber(4)
  set assetId($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasAssetId() => $_has(3);
  @$pb.TagNumber(4)
  void clearAssetId() => clearField(4);
}

/// extra address of Output
class OutputAddress extends $pb.GeneratedMessage {
  factory OutputAddress({
    $fixnum.Int64? amount,
    $core.String? toAddress,
  }) {
    final $result = create();
    if (amount != null) {
      $result.amount = amount;
    }
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    return $result;
  }
  OutputAddress._() : super();
  factory OutputAddress.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory OutputAddress.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'OutputAddress', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEO.Proto'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OS6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(2, _omitFieldNames ? '' : 'toAddress')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  OutputAddress clone() => OutputAddress()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  OutputAddress copyWith(void Function(OutputAddress) updates) => super.copyWith((message) => updates(message as OutputAddress)) as OutputAddress;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static OutputAddress create() => OutputAddress._();
  OutputAddress createEmptyInstance() => create();
  static $pb.PbList<OutputAddress> createRepeated() => $pb.PbList<OutputAddress>();
  @$core.pragma('dart2js:noInline')
  static OutputAddress getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<OutputAddress>(create);
  static OutputAddress? _defaultInstance;

  /// Amount (as string)
  @$pb.TagNumber(1)
  $fixnum.Int64 get amount => $_getI64(0);
  @$pb.TagNumber(1)
  set amount($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAmount() => $_has(0);
  @$pb.TagNumber(1)
  void clearAmount() => clearField(1);

  /// destination address
  @$pb.TagNumber(2)
  $core.String get toAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set toAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasToAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearToAddress() => clearField(2);
}

/// Output of a transaction
class TransactionOutput extends $pb.GeneratedMessage {
  factory TransactionOutput({
    $core.String? assetId,
    $fixnum.Int64? amount,
    $core.String? toAddress,
    $core.String? changeAddress,
    $core.Iterable<OutputAddress>? extraOutputs,
  }) {
    final $result = create();
    if (assetId != null) {
      $result.assetId = assetId;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (changeAddress != null) {
      $result.changeAddress = changeAddress;
    }
    if (extraOutputs != null) {
      $result.extraOutputs.addAll(extraOutputs);
    }
    return $result;
  }
  TransactionOutput._() : super();
  factory TransactionOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TransactionOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TransactionOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEO.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'assetId')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OS6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(3, _omitFieldNames ? '' : 'toAddress')
    ..aOS(4, _omitFieldNames ? '' : 'changeAddress')
    ..pc<OutputAddress>(5, _omitFieldNames ? '' : 'extraOutputs', $pb.PbFieldType.PM, subBuilder: OutputAddress.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TransactionOutput clone() => TransactionOutput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TransactionOutput copyWith(void Function(TransactionOutput) updates) => super.copyWith((message) => updates(message as TransactionOutput)) as TransactionOutput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TransactionOutput create() => TransactionOutput._();
  TransactionOutput createEmptyInstance() => create();
  static $pb.PbList<TransactionOutput> createRepeated() => $pb.PbList<TransactionOutput>();
  @$core.pragma('dart2js:noInline')
  static TransactionOutput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TransactionOutput>(create);
  static TransactionOutput? _defaultInstance;

  /// Asset
  @$pb.TagNumber(1)
  $core.String get assetId => $_getSZ(0);
  @$pb.TagNumber(1)
  set assetId($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAssetId() => $_has(0);
  @$pb.TagNumber(1)
  void clearAssetId() => clearField(1);

  /// Amount (as string)
  @$pb.TagNumber(2)
  $fixnum.Int64 get amount => $_getI64(1);
  @$pb.TagNumber(2)
  set amount($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);

  /// destination address
  @$pb.TagNumber(3)
  $core.String get toAddress => $_getSZ(2);
  @$pb.TagNumber(3)
  set toAddress($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasToAddress() => $_has(2);
  @$pb.TagNumber(3)
  void clearToAddress() => clearField(3);

  /// change address
  @$pb.TagNumber(4)
  $core.String get changeAddress => $_getSZ(3);
  @$pb.TagNumber(4)
  set changeAddress($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasChangeAddress() => $_has(3);
  @$pb.TagNumber(4)
  void clearChangeAddress() => clearField(4);

  /// extra output
  @$pb.TagNumber(5)
  $core.List<OutputAddress> get extraOutputs => $_getList(4);
}

/// nep5 token transfer transaction
class Transaction_Nep5Transfer extends $pb.GeneratedMessage {
  factory Transaction_Nep5Transfer({
    $core.String? assetId,
    $core.String? from,
    $core.String? to,
    $core.List<$core.int>? amount,
    $core.bool? scriptWithRet,
  }) {
    final $result = create();
    if (assetId != null) {
      $result.assetId = assetId;
    }
    if (from != null) {
      $result.from = from;
    }
    if (to != null) {
      $result.to = to;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (scriptWithRet != null) {
      $result.scriptWithRet = scriptWithRet;
    }
    return $result;
  }
  Transaction_Nep5Transfer._() : super();
  factory Transaction_Nep5Transfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction_Nep5Transfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction.Nep5Transfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEO.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'assetId')
    ..aOS(2, _omitFieldNames ? '' : 'from')
    ..aOS(3, _omitFieldNames ? '' : 'to')
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..aOB(5, _omitFieldNames ? '' : 'scriptWithRet')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transaction_Nep5Transfer clone() => Transaction_Nep5Transfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transaction_Nep5Transfer copyWith(void Function(Transaction_Nep5Transfer) updates) => super.copyWith((message) => updates(message as Transaction_Nep5Transfer)) as Transaction_Nep5Transfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Transaction_Nep5Transfer create() => Transaction_Nep5Transfer._();
  Transaction_Nep5Transfer createEmptyInstance() => create();
  static $pb.PbList<Transaction_Nep5Transfer> createRepeated() => $pb.PbList<Transaction_Nep5Transfer>();
  @$core.pragma('dart2js:noInline')
  static Transaction_Nep5Transfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transaction_Nep5Transfer>(create);
  static Transaction_Nep5Transfer? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get assetId => $_getSZ(0);
  @$pb.TagNumber(1)
  set assetId($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAssetId() => $_has(0);
  @$pb.TagNumber(1)
  void clearAssetId() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get from => $_getSZ(1);
  @$pb.TagNumber(2)
  set from($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasFrom() => $_has(1);
  @$pb.TagNumber(2)
  void clearFrom() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get to => $_getSZ(2);
  @$pb.TagNumber(3)
  set to($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTo() => $_has(2);
  @$pb.TagNumber(3)
  void clearTo() => clearField(3);

  /// Amount to send (256-bit number)
  @$pb.TagNumber(4)
  $core.List<$core.int> get amount => $_getN(3);
  @$pb.TagNumber(4)
  set amount($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasAmount() => $_has(3);
  @$pb.TagNumber(4)
  void clearAmount() => clearField(4);

  /// determine if putting THROWIFNOT & RET instructions
  @$pb.TagNumber(5)
  $core.bool get scriptWithRet => $_getBF(4);
  @$pb.TagNumber(5)
  set scriptWithRet($core.bool v) { $_setBool(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasScriptWithRet() => $_has(4);
  @$pb.TagNumber(5)
  void clearScriptWithRet() => clearField(5);
}

/// Generic invocation transaction
class Transaction_InvocationGeneric extends $pb.GeneratedMessage {
  factory Transaction_InvocationGeneric({
    $fixnum.Int64? gas,
    $core.List<$core.int>? script,
  }) {
    final $result = create();
    if (gas != null) {
      $result.gas = gas;
    }
    if (script != null) {
      $result.script = script;
    }
    return $result;
  }
  Transaction_InvocationGeneric._() : super();
  factory Transaction_InvocationGeneric.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction_InvocationGeneric.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction.InvocationGeneric', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEO.Proto'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'gas', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'script', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transaction_InvocationGeneric clone() => Transaction_InvocationGeneric()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transaction_InvocationGeneric copyWith(void Function(Transaction_InvocationGeneric) updates) => super.copyWith((message) => updates(message as Transaction_InvocationGeneric)) as Transaction_InvocationGeneric;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Transaction_InvocationGeneric create() => Transaction_InvocationGeneric._();
  Transaction_InvocationGeneric createEmptyInstance() => create();
  static $pb.PbList<Transaction_InvocationGeneric> createRepeated() => $pb.PbList<Transaction_InvocationGeneric>();
  @$core.pragma('dart2js:noInline')
  static Transaction_InvocationGeneric getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transaction_InvocationGeneric>(create);
  static Transaction_InvocationGeneric? _defaultInstance;

  /// gas to use
  @$pb.TagNumber(1)
  $fixnum.Int64 get gas => $_getI64(0);
  @$pb.TagNumber(1)
  set gas($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasGas() => $_has(0);
  @$pb.TagNumber(1)
  void clearGas() => clearField(1);

  /// Contract call payload data
  @$pb.TagNumber(2)
  $core.List<$core.int> get script => $_getN(1);
  @$pb.TagNumber(2)
  set script($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasScript() => $_has(1);
  @$pb.TagNumber(2)
  void clearScript() => clearField(2);
}

enum Transaction_TransactionOneof {
  nep5Transfer, 
  invocationGeneric, 
  notSet
}

/// Transaction
class Transaction extends $pb.GeneratedMessage {
  factory Transaction({
    Transaction_Nep5Transfer? nep5Transfer,
    Transaction_InvocationGeneric? invocationGeneric,
  }) {
    final $result = create();
    if (nep5Transfer != null) {
      $result.nep5Transfer = nep5Transfer;
    }
    if (invocationGeneric != null) {
      $result.invocationGeneric = invocationGeneric;
    }
    return $result;
  }
  Transaction._() : super();
  factory Transaction.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Transaction_TransactionOneof> _Transaction_TransactionOneofByTag = {
    1 : Transaction_TransactionOneof.nep5Transfer,
    2 : Transaction_TransactionOneof.invocationGeneric,
    0 : Transaction_TransactionOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEO.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2])
    ..aOM<Transaction_Nep5Transfer>(1, _omitFieldNames ? '' : 'nep5Transfer', subBuilder: Transaction_Nep5Transfer.create)
    ..aOM<Transaction_InvocationGeneric>(2, _omitFieldNames ? '' : 'invocationGeneric', subBuilder: Transaction_InvocationGeneric.create)
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

  Transaction_TransactionOneof whichTransactionOneof() => _Transaction_TransactionOneofByTag[$_whichOneof(0)]!;
  void clearTransactionOneof() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  Transaction_Nep5Transfer get nep5Transfer => $_getN(0);
  @$pb.TagNumber(1)
  set nep5Transfer(Transaction_Nep5Transfer v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasNep5Transfer() => $_has(0);
  @$pb.TagNumber(1)
  void clearNep5Transfer() => clearField(1);
  @$pb.TagNumber(1)
  Transaction_Nep5Transfer ensureNep5Transfer() => $_ensure(0);

  @$pb.TagNumber(2)
  Transaction_InvocationGeneric get invocationGeneric => $_getN(1);
  @$pb.TagNumber(2)
  set invocationGeneric(Transaction_InvocationGeneric v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasInvocationGeneric() => $_has(1);
  @$pb.TagNumber(2)
  void clearInvocationGeneric() => clearField(2);
  @$pb.TagNumber(2)
  Transaction_InvocationGeneric ensureInvocationGeneric() => $_ensure(1);
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.Iterable<TransactionInput>? inputs,
    $core.Iterable<TransactionOutput>? outputs,
    $core.List<$core.int>? privateKey,
    $fixnum.Int64? fee,
    $core.String? gasAssetId,
    $core.String? gasChangeAddress,
    TransactionPlan? plan,
    Transaction? transaction,
  }) {
    final $result = create();
    if (inputs != null) {
      $result.inputs.addAll(inputs);
    }
    if (outputs != null) {
      $result.outputs.addAll(outputs);
    }
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (fee != null) {
      $result.fee = fee;
    }
    if (gasAssetId != null) {
      $result.gasAssetId = gasAssetId;
    }
    if (gasChangeAddress != null) {
      $result.gasChangeAddress = gasChangeAddress;
    }
    if (plan != null) {
      $result.plan = plan;
    }
    if (transaction != null) {
      $result.transaction = transaction;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEO.Proto'), createEmptyInstance: create)
    ..pc<TransactionInput>(1, _omitFieldNames ? '' : 'inputs', $pb.PbFieldType.PM, subBuilder: TransactionInput.create)
    ..pc<TransactionOutput>(2, _omitFieldNames ? '' : 'outputs', $pb.PbFieldType.PM, subBuilder: TransactionOutput.create)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..aInt64(4, _omitFieldNames ? '' : 'fee')
    ..aOS(5, _omitFieldNames ? '' : 'gasAssetId')
    ..aOS(6, _omitFieldNames ? '' : 'gasChangeAddress')
    ..aOM<TransactionPlan>(7, _omitFieldNames ? '' : 'plan', subBuilder: TransactionPlan.create)
    ..aOM<Transaction>(8, _omitFieldNames ? '' : 'transaction', subBuilder: Transaction.create)
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

  /// Available transaction inputs
  @$pb.TagNumber(1)
  $core.List<TransactionInput> get inputs => $_getList(0);

  /// Transaction outputs
  @$pb.TagNumber(2)
  $core.List<TransactionOutput> get outputs => $_getList(1);

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(3)
  $core.List<$core.int> get privateKey => $_getN(2);
  @$pb.TagNumber(3)
  set privateKey($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasPrivateKey() => $_has(2);
  @$pb.TagNumber(3)
  void clearPrivateKey() => clearField(3);

  /// Fee
  @$pb.TagNumber(4)
  $fixnum.Int64 get fee => $_getI64(3);
  @$pb.TagNumber(4)
  set fee($fixnum.Int64 v) { $_setInt64(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasFee() => $_has(3);
  @$pb.TagNumber(4)
  void clearFee() => clearField(4);

  /// Asset ID for gas
  @$pb.TagNumber(5)
  $core.String get gasAssetId => $_getSZ(4);
  @$pb.TagNumber(5)
  set gasAssetId($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasGasAssetId() => $_has(4);
  @$pb.TagNumber(5)
  void clearGasAssetId() => clearField(5);

  /// Address for the change
  @$pb.TagNumber(6)
  $core.String get gasChangeAddress => $_getSZ(5);
  @$pb.TagNumber(6)
  set gasChangeAddress($core.String v) { $_setString(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasGasChangeAddress() => $_has(5);
  @$pb.TagNumber(6)
  void clearGasChangeAddress() => clearField(6);

  /// Optional transaction plan (if missing it will be computed)
  @$pb.TagNumber(7)
  TransactionPlan get plan => $_getN(6);
  @$pb.TagNumber(7)
  set plan(TransactionPlan v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasPlan() => $_has(6);
  @$pb.TagNumber(7)
  void clearPlan() => clearField(7);
  @$pb.TagNumber(7)
  TransactionPlan ensurePlan() => $_ensure(6);

  @$pb.TagNumber(8)
  Transaction get transaction => $_getN(7);
  @$pb.TagNumber(8)
  set transaction(Transaction v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasTransaction() => $_has(7);
  @$pb.TagNumber(8)
  void clearTransaction() => clearField(8);
  @$pb.TagNumber(8)
  Transaction ensureTransaction() => $_ensure(7);
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

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEO.Proto'), createEmptyInstance: create)
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

  /// Optional error
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

/// Describes a preliminary transaction output plan.
class TransactionOutputPlan extends $pb.GeneratedMessage {
  factory TransactionOutputPlan({
    $fixnum.Int64? amount,
    $fixnum.Int64? availableAmount,
    $fixnum.Int64? change,
    $core.String? assetId,
    $core.String? toAddress,
    $core.String? changeAddress,
    $core.Iterable<OutputAddress>? extraOutputs,
  }) {
    final $result = create();
    if (amount != null) {
      $result.amount = amount;
    }
    if (availableAmount != null) {
      $result.availableAmount = availableAmount;
    }
    if (change != null) {
      $result.change = change;
    }
    if (assetId != null) {
      $result.assetId = assetId;
    }
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (changeAddress != null) {
      $result.changeAddress = changeAddress;
    }
    if (extraOutputs != null) {
      $result.extraOutputs.addAll(extraOutputs);
    }
    return $result;
  }
  TransactionOutputPlan._() : super();
  factory TransactionOutputPlan.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TransactionOutputPlan.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TransactionOutputPlan', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEO.Proto'), createEmptyInstance: create)
    ..aInt64(1, _omitFieldNames ? '' : 'amount')
    ..aInt64(2, _omitFieldNames ? '' : 'availableAmount')
    ..aInt64(3, _omitFieldNames ? '' : 'change')
    ..aOS(4, _omitFieldNames ? '' : 'assetId')
    ..aOS(5, _omitFieldNames ? '' : 'toAddress')
    ..aOS(6, _omitFieldNames ? '' : 'changeAddress')
    ..pc<OutputAddress>(7, _omitFieldNames ? '' : 'extraOutputs', $pb.PbFieldType.PM, subBuilder: OutputAddress.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TransactionOutputPlan clone() => TransactionOutputPlan()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TransactionOutputPlan copyWith(void Function(TransactionOutputPlan) updates) => super.copyWith((message) => updates(message as TransactionOutputPlan)) as TransactionOutputPlan;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TransactionOutputPlan create() => TransactionOutputPlan._();
  TransactionOutputPlan createEmptyInstance() => create();
  static $pb.PbList<TransactionOutputPlan> createRepeated() => $pb.PbList<TransactionOutputPlan>();
  @$core.pragma('dart2js:noInline')
  static TransactionOutputPlan getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TransactionOutputPlan>(create);
  static TransactionOutputPlan? _defaultInstance;

  /// Amount to be received at the other end.
  @$pb.TagNumber(1)
  $fixnum.Int64 get amount => $_getI64(0);
  @$pb.TagNumber(1)
  set amount($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAmount() => $_has(0);
  @$pb.TagNumber(1)
  void clearAmount() => clearField(1);

  /// Maximum available amount.
  @$pb.TagNumber(2)
  $fixnum.Int64 get availableAmount => $_getI64(1);
  @$pb.TagNumber(2)
  set availableAmount($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAvailableAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAvailableAmount() => clearField(2);

  /// Amount that is left as change
  @$pb.TagNumber(3)
  $fixnum.Int64 get change => $_getI64(2);
  @$pb.TagNumber(3)
  set change($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasChange() => $_has(2);
  @$pb.TagNumber(3)
  void clearChange() => clearField(3);

  /// Asset
  @$pb.TagNumber(4)
  $core.String get assetId => $_getSZ(3);
  @$pb.TagNumber(4)
  set assetId($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasAssetId() => $_has(3);
  @$pb.TagNumber(4)
  void clearAssetId() => clearField(4);

  /// Destination address
  @$pb.TagNumber(5)
  $core.String get toAddress => $_getSZ(4);
  @$pb.TagNumber(5)
  set toAddress($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasToAddress() => $_has(4);
  @$pb.TagNumber(5)
  void clearToAddress() => clearField(5);

  /// Address for the change
  @$pb.TagNumber(6)
  $core.String get changeAddress => $_getSZ(5);
  @$pb.TagNumber(6)
  set changeAddress($core.String v) { $_setString(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasChangeAddress() => $_has(5);
  @$pb.TagNumber(6)
  void clearChangeAddress() => clearField(6);

  /// extra output
  @$pb.TagNumber(7)
  $core.List<OutputAddress> get extraOutputs => $_getList(6);
}

class TransactionAttributePlan extends $pb.GeneratedMessage {
  factory TransactionAttributePlan({
    $core.int? usage,
    $core.List<$core.int>? data,
  }) {
    final $result = create();
    if (usage != null) {
      $result.usage = usage;
    }
    if (data != null) {
      $result.data = data;
    }
    return $result;
  }
  TransactionAttributePlan._() : super();
  factory TransactionAttributePlan.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TransactionAttributePlan.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TransactionAttributePlan', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEO.Proto'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'usage', $pb.PbFieldType.O3)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'data', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TransactionAttributePlan clone() => TransactionAttributePlan()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TransactionAttributePlan copyWith(void Function(TransactionAttributePlan) updates) => super.copyWith((message) => updates(message as TransactionAttributePlan)) as TransactionAttributePlan;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TransactionAttributePlan create() => TransactionAttributePlan._();
  TransactionAttributePlan createEmptyInstance() => create();
  static $pb.PbList<TransactionAttributePlan> createRepeated() => $pb.PbList<TransactionAttributePlan>();
  @$core.pragma('dart2js:noInline')
  static TransactionAttributePlan getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TransactionAttributePlan>(create);
  static TransactionAttributePlan? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get usage => $_getIZ(0);
  @$pb.TagNumber(1)
  set usage($core.int v) { $_setSignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasUsage() => $_has(0);
  @$pb.TagNumber(1)
  void clearUsage() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<$core.int> get data => $_getN(1);
  @$pb.TagNumber(2)
  set data($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasData() => $_has(1);
  @$pb.TagNumber(2)
  void clearData() => clearField(2);
}

/// Describes a preliminary transaction plan.
class TransactionPlan extends $pb.GeneratedMessage {
  factory TransactionPlan({
    $core.Iterable<TransactionOutputPlan>? outputs,
    $core.Iterable<TransactionInput>? inputs,
    $fixnum.Int64? fee,
    $0.SigningError? error,
    $core.Iterable<TransactionAttributePlan>? attributes,
  }) {
    final $result = create();
    if (outputs != null) {
      $result.outputs.addAll(outputs);
    }
    if (inputs != null) {
      $result.inputs.addAll(inputs);
    }
    if (fee != null) {
      $result.fee = fee;
    }
    if (error != null) {
      $result.error = error;
    }
    if (attributes != null) {
      $result.attributes.addAll(attributes);
    }
    return $result;
  }
  TransactionPlan._() : super();
  factory TransactionPlan.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TransactionPlan.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TransactionPlan', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NEO.Proto'), createEmptyInstance: create)
    ..pc<TransactionOutputPlan>(1, _omitFieldNames ? '' : 'outputs', $pb.PbFieldType.PM, subBuilder: TransactionOutputPlan.create)
    ..pc<TransactionInput>(2, _omitFieldNames ? '' : 'inputs', $pb.PbFieldType.PM, subBuilder: TransactionInput.create)
    ..aInt64(3, _omitFieldNames ? '' : 'fee')
    ..e<$0.SigningError>(4, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..pc<TransactionAttributePlan>(5, _omitFieldNames ? '' : 'attributes', $pb.PbFieldType.PM, subBuilder: TransactionAttributePlan.create)
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

  /// Used assets
  @$pb.TagNumber(1)
  $core.List<TransactionOutputPlan> get outputs => $_getList(0);

  /// Selected unspent transaction outputs.
  @$pb.TagNumber(2)
  $core.List<TransactionInput> get inputs => $_getList(1);

  /// GAS used
  @$pb.TagNumber(3)
  $fixnum.Int64 get fee => $_getI64(2);
  @$pb.TagNumber(3)
  set fee($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasFee() => $_has(2);
  @$pb.TagNumber(3)
  void clearFee() => clearField(3);

  /// Optional error
  @$pb.TagNumber(4)
  $0.SigningError get error => $_getN(3);
  @$pb.TagNumber(4)
  set error($0.SigningError v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasError() => $_has(3);
  @$pb.TagNumber(4)
  void clearError() => clearField(4);

  /// Attribute
  @$pb.TagNumber(5)
  $core.List<TransactionAttributePlan> get attributes => $_getList(4);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
