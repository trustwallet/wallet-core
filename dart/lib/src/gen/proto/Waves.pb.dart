//
//  Generated code. Do not modify.
//  source: Waves.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

/// Transfer transaction
class TransferMessage extends $pb.GeneratedMessage {
  factory TransferMessage({
    $fixnum.Int64? amount,
    $core.String? asset,
    $fixnum.Int64? fee,
    $core.String? feeAsset,
    $core.String? to,
    $core.List<$core.int>? attachment,
  }) {
    final $result = create();
    if (amount != null) {
      $result.amount = amount;
    }
    if (asset != null) {
      $result.asset = asset;
    }
    if (fee != null) {
      $result.fee = fee;
    }
    if (feeAsset != null) {
      $result.feeAsset = feeAsset;
    }
    if (to != null) {
      $result.to = to;
    }
    if (attachment != null) {
      $result.attachment = attachment;
    }
    return $result;
  }
  TransferMessage._() : super();
  factory TransferMessage.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TransferMessage.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TransferMessage', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Waves.Proto'), createEmptyInstance: create)
    ..aInt64(1, _omitFieldNames ? '' : 'amount')
    ..aOS(2, _omitFieldNames ? '' : 'asset')
    ..aInt64(3, _omitFieldNames ? '' : 'fee')
    ..aOS(4, _omitFieldNames ? '' : 'feeAsset')
    ..aOS(5, _omitFieldNames ? '' : 'to')
    ..a<$core.List<$core.int>>(6, _omitFieldNames ? '' : 'attachment', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TransferMessage clone() => TransferMessage()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TransferMessage copyWith(void Function(TransferMessage) updates) => super.copyWith((message) => updates(message as TransferMessage)) as TransferMessage;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TransferMessage create() => TransferMessage._();
  TransferMessage createEmptyInstance() => create();
  static $pb.PbList<TransferMessage> createRepeated() => $pb.PbList<TransferMessage>();
  @$core.pragma('dart2js:noInline')
  static TransferMessage getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TransferMessage>(create);
  static TransferMessage? _defaultInstance;

  /// amount
  @$pb.TagNumber(1)
  $fixnum.Int64 get amount => $_getI64(0);
  @$pb.TagNumber(1)
  set amount($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAmount() => $_has(0);
  @$pb.TagNumber(1)
  void clearAmount() => clearField(1);

  /// asset ID
  @$pb.TagNumber(2)
  $core.String get asset => $_getSZ(1);
  @$pb.TagNumber(2)
  set asset($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAsset() => $_has(1);
  @$pb.TagNumber(2)
  void clearAsset() => clearField(2);

  /// minimum 0.001 Waves (100000 Wavelets) for now
  @$pb.TagNumber(3)
  $fixnum.Int64 get fee => $_getI64(2);
  @$pb.TagNumber(3)
  set fee($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasFee() => $_has(2);
  @$pb.TagNumber(3)
  void clearFee() => clearField(3);

  /// asset of the fee
  @$pb.TagNumber(4)
  $core.String get feeAsset => $_getSZ(3);
  @$pb.TagNumber(4)
  set feeAsset($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasFeeAsset() => $_has(3);
  @$pb.TagNumber(4)
  void clearFeeAsset() => clearField(4);

  /// destination address
  @$pb.TagNumber(5)
  $core.String get to => $_getSZ(4);
  @$pb.TagNumber(5)
  set to($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasTo() => $_has(4);
  @$pb.TagNumber(5)
  void clearTo() => clearField(5);

  /// any 140 bytes payload, will be displayed to the client as utf-8 string
  @$pb.TagNumber(6)
  $core.List<$core.int> get attachment => $_getN(5);
  @$pb.TagNumber(6)
  set attachment($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasAttachment() => $_has(5);
  @$pb.TagNumber(6)
  void clearAttachment() => clearField(6);
}

/// Lease transaction
class LeaseMessage extends $pb.GeneratedMessage {
  factory LeaseMessage({
    $fixnum.Int64? amount,
    $core.String? to,
    $fixnum.Int64? fee,
  }) {
    final $result = create();
    if (amount != null) {
      $result.amount = amount;
    }
    if (to != null) {
      $result.to = to;
    }
    if (fee != null) {
      $result.fee = fee;
    }
    return $result;
  }
  LeaseMessage._() : super();
  factory LeaseMessage.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory LeaseMessage.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'LeaseMessage', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Waves.Proto'), createEmptyInstance: create)
    ..aInt64(1, _omitFieldNames ? '' : 'amount')
    ..aOS(2, _omitFieldNames ? '' : 'to')
    ..aInt64(3, _omitFieldNames ? '' : 'fee')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  LeaseMessage clone() => LeaseMessage()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  LeaseMessage copyWith(void Function(LeaseMessage) updates) => super.copyWith((message) => updates(message as LeaseMessage)) as LeaseMessage;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static LeaseMessage create() => LeaseMessage._();
  LeaseMessage createEmptyInstance() => create();
  static $pb.PbList<LeaseMessage> createRepeated() => $pb.PbList<LeaseMessage>();
  @$core.pragma('dart2js:noInline')
  static LeaseMessage getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<LeaseMessage>(create);
  static LeaseMessage? _defaultInstance;

  /// amount
  @$pb.TagNumber(1)
  $fixnum.Int64 get amount => $_getI64(0);
  @$pb.TagNumber(1)
  set amount($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAmount() => $_has(0);
  @$pb.TagNumber(1)
  void clearAmount() => clearField(1);

  /// destination
  @$pb.TagNumber(2)
  $core.String get to => $_getSZ(1);
  @$pb.TagNumber(2)
  set to($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTo() => $_has(1);
  @$pb.TagNumber(2)
  void clearTo() => clearField(2);

  /// minimum 0.001 Waves (100000 Wavelets) for now
  @$pb.TagNumber(3)
  $fixnum.Int64 get fee => $_getI64(2);
  @$pb.TagNumber(3)
  set fee($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasFee() => $_has(2);
  @$pb.TagNumber(3)
  void clearFee() => clearField(3);
}

/// Cancel Lease transaction
class CancelLeaseMessage extends $pb.GeneratedMessage {
  factory CancelLeaseMessage({
    $core.String? leaseId,
    $fixnum.Int64? fee,
  }) {
    final $result = create();
    if (leaseId != null) {
      $result.leaseId = leaseId;
    }
    if (fee != null) {
      $result.fee = fee;
    }
    return $result;
  }
  CancelLeaseMessage._() : super();
  factory CancelLeaseMessage.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory CancelLeaseMessage.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'CancelLeaseMessage', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Waves.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'leaseId')
    ..aInt64(2, _omitFieldNames ? '' : 'fee')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  CancelLeaseMessage clone() => CancelLeaseMessage()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  CancelLeaseMessage copyWith(void Function(CancelLeaseMessage) updates) => super.copyWith((message) => updates(message as CancelLeaseMessage)) as CancelLeaseMessage;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static CancelLeaseMessage create() => CancelLeaseMessage._();
  CancelLeaseMessage createEmptyInstance() => create();
  static $pb.PbList<CancelLeaseMessage> createRepeated() => $pb.PbList<CancelLeaseMessage>();
  @$core.pragma('dart2js:noInline')
  static CancelLeaseMessage getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<CancelLeaseMessage>(create);
  static CancelLeaseMessage? _defaultInstance;

  /// Lease ID to cancel
  @$pb.TagNumber(1)
  $core.String get leaseId => $_getSZ(0);
  @$pb.TagNumber(1)
  set leaseId($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasLeaseId() => $_has(0);
  @$pb.TagNumber(1)
  void clearLeaseId() => clearField(1);

  /// Fee used
  @$pb.TagNumber(2)
  $fixnum.Int64 get fee => $_getI64(1);
  @$pb.TagNumber(2)
  set fee($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasFee() => $_has(1);
  @$pb.TagNumber(2)
  void clearFee() => clearField(2);
}

enum SigningInput_MessageOneof {
  transferMessage, 
  leaseMessage, 
  cancelLeaseMessage, 
  notSet
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $fixnum.Int64? timestamp,
    $core.List<$core.int>? privateKey,
    TransferMessage? transferMessage,
    LeaseMessage? leaseMessage,
    CancelLeaseMessage? cancelLeaseMessage,
  }) {
    final $result = create();
    if (timestamp != null) {
      $result.timestamp = timestamp;
    }
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (transferMessage != null) {
      $result.transferMessage = transferMessage;
    }
    if (leaseMessage != null) {
      $result.leaseMessage = leaseMessage;
    }
    if (cancelLeaseMessage != null) {
      $result.cancelLeaseMessage = cancelLeaseMessage;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, SigningInput_MessageOneof> _SigningInput_MessageOneofByTag = {
    3 : SigningInput_MessageOneof.transferMessage,
    4 : SigningInput_MessageOneof.leaseMessage,
    5 : SigningInput_MessageOneof.cancelLeaseMessage,
    0 : SigningInput_MessageOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Waves.Proto'), createEmptyInstance: create)
    ..oo(0, [3, 4, 5])
    ..aInt64(1, _omitFieldNames ? '' : 'timestamp')
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..aOM<TransferMessage>(3, _omitFieldNames ? '' : 'transferMessage', subBuilder: TransferMessage.create)
    ..aOM<LeaseMessage>(4, _omitFieldNames ? '' : 'leaseMessage', subBuilder: LeaseMessage.create)
    ..aOM<CancelLeaseMessage>(5, _omitFieldNames ? '' : 'cancelLeaseMessage', subBuilder: CancelLeaseMessage.create)
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

  /// in millis
  @$pb.TagNumber(1)
  $fixnum.Int64 get timestamp => $_getI64(0);
  @$pb.TagNumber(1)
  set timestamp($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasTimestamp() => $_has(0);
  @$pb.TagNumber(1)
  void clearTimestamp() => clearField(1);

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
  TransferMessage get transferMessage => $_getN(2);
  @$pb.TagNumber(3)
  set transferMessage(TransferMessage v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasTransferMessage() => $_has(2);
  @$pb.TagNumber(3)
  void clearTransferMessage() => clearField(3);
  @$pb.TagNumber(3)
  TransferMessage ensureTransferMessage() => $_ensure(2);

  @$pb.TagNumber(4)
  LeaseMessage get leaseMessage => $_getN(3);
  @$pb.TagNumber(4)
  set leaseMessage(LeaseMessage v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasLeaseMessage() => $_has(3);
  @$pb.TagNumber(4)
  void clearLeaseMessage() => clearField(4);
  @$pb.TagNumber(4)
  LeaseMessage ensureLeaseMessage() => $_ensure(3);

  @$pb.TagNumber(5)
  CancelLeaseMessage get cancelLeaseMessage => $_getN(4);
  @$pb.TagNumber(5)
  set cancelLeaseMessage(CancelLeaseMessage v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasCancelLeaseMessage() => $_has(4);
  @$pb.TagNumber(5)
  void clearCancelLeaseMessage() => clearField(5);
  @$pb.TagNumber(5)
  CancelLeaseMessage ensureCancelLeaseMessage() => $_ensure(4);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.List<$core.int>? signature,
    $core.String? json,
  }) {
    final $result = create();
    if (signature != null) {
      $result.signature = signature;
    }
    if (json != null) {
      $result.json = json;
    }
    return $result;
  }
  SigningOutput._() : super();
  factory SigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Waves.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'signature', $pb.PbFieldType.OY)
    ..aOS(2, _omitFieldNames ? '' : 'json')
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

  /// signature data
  @$pb.TagNumber(1)
  $core.List<$core.int> get signature => $_getN(0);
  @$pb.TagNumber(1)
  set signature($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSignature() => $_has(0);
  @$pb.TagNumber(1)
  void clearSignature() => clearField(1);

  /// transaction in JSON format
  @$pb.TagNumber(2)
  $core.String get json => $_getSZ(1);
  @$pb.TagNumber(2)
  set json($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasJson() => $_has(1);
  @$pb.TagNumber(2)
  void clearJson() => clearField(2);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
