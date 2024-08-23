//
//  Generated code. Do not modify.
//  source: IoTeX.proto
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

/// A transfer
class Transfer extends $pb.GeneratedMessage {
  factory Transfer({
    $core.String? amount,
    $core.String? recipient,
    $core.List<$core.int>? payload,
  }) {
    final $result = create();
    if (amount != null) {
      $result.amount = amount;
    }
    if (recipient != null) {
      $result.recipient = recipient;
    }
    if (payload != null) {
      $result.payload = payload;
    }
    return $result;
  }
  Transfer._() : super();
  factory Transfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IoTeX.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'amount')
    ..aOS(2, _omitFieldNames ? '' : 'recipient')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'payload', $pb.PbFieldType.OY)
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

  /// Amount (as string)
  @$pb.TagNumber(1)
  $core.String get amount => $_getSZ(0);
  @$pb.TagNumber(1)
  set amount($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAmount() => $_has(0);
  @$pb.TagNumber(1)
  void clearAmount() => clearField(1);

  /// Destination address
  @$pb.TagNumber(2)
  $core.String get recipient => $_getSZ(1);
  @$pb.TagNumber(2)
  set recipient($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasRecipient() => $_has(1);
  @$pb.TagNumber(2)
  void clearRecipient() => clearField(2);

  /// Payload data
  @$pb.TagNumber(3)
  $core.List<$core.int> get payload => $_getN(2);
  @$pb.TagNumber(3)
  set payload($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasPayload() => $_has(2);
  @$pb.TagNumber(3)
  void clearPayload() => clearField(3);
}

/// create stake
class Staking_Create extends $pb.GeneratedMessage {
  factory Staking_Create({
    $core.String? candidateName,
    $core.String? stakedAmount,
    $core.int? stakedDuration,
    $core.bool? autoStake,
    $core.List<$core.int>? payload,
  }) {
    final $result = create();
    if (candidateName != null) {
      $result.candidateName = candidateName;
    }
    if (stakedAmount != null) {
      $result.stakedAmount = stakedAmount;
    }
    if (stakedDuration != null) {
      $result.stakedDuration = stakedDuration;
    }
    if (autoStake != null) {
      $result.autoStake = autoStake;
    }
    if (payload != null) {
      $result.payload = payload;
    }
    return $result;
  }
  Staking_Create._() : super();
  factory Staking_Create.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_Create.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.Create', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IoTeX.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'candidateName', protoName: 'candidateName')
    ..aOS(2, _omitFieldNames ? '' : 'stakedAmount', protoName: 'stakedAmount')
    ..a<$core.int>(3, _omitFieldNames ? '' : 'stakedDuration', $pb.PbFieldType.OU3, protoName: 'stakedDuration')
    ..aOB(4, _omitFieldNames ? '' : 'autoStake', protoName: 'autoStake')
    ..a<$core.List<$core.int>>(5, _omitFieldNames ? '' : 'payload', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_Create clone() => Staking_Create()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_Create copyWith(void Function(Staking_Create) updates) => super.copyWith((message) => updates(message as Staking_Create)) as Staking_Create;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_Create create() => Staking_Create._();
  Staking_Create createEmptyInstance() => create();
  static $pb.PbList<Staking_Create> createRepeated() => $pb.PbList<Staking_Create>();
  @$core.pragma('dart2js:noInline')
  static Staking_Create getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_Create>(create);
  static Staking_Create? _defaultInstance;

  /// validator name
  @$pb.TagNumber(1)
  $core.String get candidateName => $_getSZ(0);
  @$pb.TagNumber(1)
  set candidateName($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasCandidateName() => $_has(0);
  @$pb.TagNumber(1)
  void clearCandidateName() => clearField(1);

  /// amount to be staked
  @$pb.TagNumber(2)
  $core.String get stakedAmount => $_getSZ(1);
  @$pb.TagNumber(2)
  set stakedAmount($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasStakedAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearStakedAmount() => clearField(2);

  /// duration
  @$pb.TagNumber(3)
  $core.int get stakedDuration => $_getIZ(2);
  @$pb.TagNumber(3)
  set stakedDuration($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasStakedDuration() => $_has(2);
  @$pb.TagNumber(3)
  void clearStakedDuration() => clearField(3);

  /// auto-restake
  @$pb.TagNumber(4)
  $core.bool get autoStake => $_getBF(3);
  @$pb.TagNumber(4)
  set autoStake($core.bool v) { $_setBool(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasAutoStake() => $_has(3);
  @$pb.TagNumber(4)
  void clearAutoStake() => clearField(4);

  /// payload data
  @$pb.TagNumber(5)
  $core.List<$core.int> get payload => $_getN(4);
  @$pb.TagNumber(5)
  set payload($core.List<$core.int> v) { $_setBytes(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasPayload() => $_has(4);
  @$pb.TagNumber(5)
  void clearPayload() => clearField(5);
}

/// unstake or withdraw
class Staking_Reclaim extends $pb.GeneratedMessage {
  factory Staking_Reclaim({
    $fixnum.Int64? bucketIndex,
    $core.List<$core.int>? payload,
  }) {
    final $result = create();
    if (bucketIndex != null) {
      $result.bucketIndex = bucketIndex;
    }
    if (payload != null) {
      $result.payload = payload;
    }
    return $result;
  }
  Staking_Reclaim._() : super();
  factory Staking_Reclaim.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_Reclaim.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.Reclaim', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IoTeX.Proto'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'bucketIndex', $pb.PbFieldType.OU6, protoName: 'bucketIndex', defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'payload', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_Reclaim clone() => Staking_Reclaim()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_Reclaim copyWith(void Function(Staking_Reclaim) updates) => super.copyWith((message) => updates(message as Staking_Reclaim)) as Staking_Reclaim;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_Reclaim create() => Staking_Reclaim._();
  Staking_Reclaim createEmptyInstance() => create();
  static $pb.PbList<Staking_Reclaim> createRepeated() => $pb.PbList<Staking_Reclaim>();
  @$core.pragma('dart2js:noInline')
  static Staking_Reclaim getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_Reclaim>(create);
  static Staking_Reclaim? _defaultInstance;

  /// index to claim
  @$pb.TagNumber(1)
  $fixnum.Int64 get bucketIndex => $_getI64(0);
  @$pb.TagNumber(1)
  set bucketIndex($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasBucketIndex() => $_has(0);
  @$pb.TagNumber(1)
  void clearBucketIndex() => clearField(1);

  /// payload data
  @$pb.TagNumber(2)
  $core.List<$core.int> get payload => $_getN(1);
  @$pb.TagNumber(2)
  set payload($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPayload() => $_has(1);
  @$pb.TagNumber(2)
  void clearPayload() => clearField(2);
}

/// add the amount of bucket
class Staking_AddDeposit extends $pb.GeneratedMessage {
  factory Staking_AddDeposit({
    $fixnum.Int64? bucketIndex,
    $core.String? amount,
    $core.List<$core.int>? payload,
  }) {
    final $result = create();
    if (bucketIndex != null) {
      $result.bucketIndex = bucketIndex;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (payload != null) {
      $result.payload = payload;
    }
    return $result;
  }
  Staking_AddDeposit._() : super();
  factory Staking_AddDeposit.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_AddDeposit.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.AddDeposit', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IoTeX.Proto'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'bucketIndex', $pb.PbFieldType.OU6, protoName: 'bucketIndex', defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(2, _omitFieldNames ? '' : 'amount')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'payload', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_AddDeposit clone() => Staking_AddDeposit()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_AddDeposit copyWith(void Function(Staking_AddDeposit) updates) => super.copyWith((message) => updates(message as Staking_AddDeposit)) as Staking_AddDeposit;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_AddDeposit create() => Staking_AddDeposit._();
  Staking_AddDeposit createEmptyInstance() => create();
  static $pb.PbList<Staking_AddDeposit> createRepeated() => $pb.PbList<Staking_AddDeposit>();
  @$core.pragma('dart2js:noInline')
  static Staking_AddDeposit getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_AddDeposit>(create);
  static Staking_AddDeposit? _defaultInstance;

  /// index
  @$pb.TagNumber(1)
  $fixnum.Int64 get bucketIndex => $_getI64(0);
  @$pb.TagNumber(1)
  set bucketIndex($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasBucketIndex() => $_has(0);
  @$pb.TagNumber(1)
  void clearBucketIndex() => clearField(1);

  /// amount to add
  @$pb.TagNumber(2)
  $core.String get amount => $_getSZ(1);
  @$pb.TagNumber(2)
  set amount($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);

  /// payload data
  @$pb.TagNumber(3)
  $core.List<$core.int> get payload => $_getN(2);
  @$pb.TagNumber(3)
  set payload($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasPayload() => $_has(2);
  @$pb.TagNumber(3)
  void clearPayload() => clearField(3);
}

/// restake the duration and autoStake flag of bucket
class Staking_Restake extends $pb.GeneratedMessage {
  factory Staking_Restake({
    $fixnum.Int64? bucketIndex,
    $core.int? stakedDuration,
    $core.bool? autoStake,
    $core.List<$core.int>? payload,
  }) {
    final $result = create();
    if (bucketIndex != null) {
      $result.bucketIndex = bucketIndex;
    }
    if (stakedDuration != null) {
      $result.stakedDuration = stakedDuration;
    }
    if (autoStake != null) {
      $result.autoStake = autoStake;
    }
    if (payload != null) {
      $result.payload = payload;
    }
    return $result;
  }
  Staking_Restake._() : super();
  factory Staking_Restake.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_Restake.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.Restake', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IoTeX.Proto'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'bucketIndex', $pb.PbFieldType.OU6, protoName: 'bucketIndex', defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'stakedDuration', $pb.PbFieldType.OU3, protoName: 'stakedDuration')
    ..aOB(3, _omitFieldNames ? '' : 'autoStake', protoName: 'autoStake')
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'payload', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_Restake clone() => Staking_Restake()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_Restake copyWith(void Function(Staking_Restake) updates) => super.copyWith((message) => updates(message as Staking_Restake)) as Staking_Restake;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_Restake create() => Staking_Restake._();
  Staking_Restake createEmptyInstance() => create();
  static $pb.PbList<Staking_Restake> createRepeated() => $pb.PbList<Staking_Restake>();
  @$core.pragma('dart2js:noInline')
  static Staking_Restake getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_Restake>(create);
  static Staking_Restake? _defaultInstance;

  /// index
  @$pb.TagNumber(1)
  $fixnum.Int64 get bucketIndex => $_getI64(0);
  @$pb.TagNumber(1)
  set bucketIndex($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasBucketIndex() => $_has(0);
  @$pb.TagNumber(1)
  void clearBucketIndex() => clearField(1);

  /// stake duration
  @$pb.TagNumber(2)
  $core.int get stakedDuration => $_getIZ(1);
  @$pb.TagNumber(2)
  set stakedDuration($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasStakedDuration() => $_has(1);
  @$pb.TagNumber(2)
  void clearStakedDuration() => clearField(2);

  /// auto re-stake
  @$pb.TagNumber(3)
  $core.bool get autoStake => $_getBF(2);
  @$pb.TagNumber(3)
  set autoStake($core.bool v) { $_setBool(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAutoStake() => $_has(2);
  @$pb.TagNumber(3)
  void clearAutoStake() => clearField(3);

  /// payload data
  @$pb.TagNumber(4)
  $core.List<$core.int> get payload => $_getN(3);
  @$pb.TagNumber(4)
  set payload($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasPayload() => $_has(3);
  @$pb.TagNumber(4)
  void clearPayload() => clearField(4);
}

/// move the bucket to vote for another candidate or transfer the ownership of bucket to another voters
class Staking_ChangeCandidate extends $pb.GeneratedMessage {
  factory Staking_ChangeCandidate({
    $fixnum.Int64? bucketIndex,
    $core.String? candidateName,
    $core.List<$core.int>? payload,
  }) {
    final $result = create();
    if (bucketIndex != null) {
      $result.bucketIndex = bucketIndex;
    }
    if (candidateName != null) {
      $result.candidateName = candidateName;
    }
    if (payload != null) {
      $result.payload = payload;
    }
    return $result;
  }
  Staking_ChangeCandidate._() : super();
  factory Staking_ChangeCandidate.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_ChangeCandidate.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.ChangeCandidate', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IoTeX.Proto'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'bucketIndex', $pb.PbFieldType.OU6, protoName: 'bucketIndex', defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(2, _omitFieldNames ? '' : 'candidateName', protoName: 'candidateName')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'payload', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_ChangeCandidate clone() => Staking_ChangeCandidate()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_ChangeCandidate copyWith(void Function(Staking_ChangeCandidate) updates) => super.copyWith((message) => updates(message as Staking_ChangeCandidate)) as Staking_ChangeCandidate;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_ChangeCandidate create() => Staking_ChangeCandidate._();
  Staking_ChangeCandidate createEmptyInstance() => create();
  static $pb.PbList<Staking_ChangeCandidate> createRepeated() => $pb.PbList<Staking_ChangeCandidate>();
  @$core.pragma('dart2js:noInline')
  static Staking_ChangeCandidate getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_ChangeCandidate>(create);
  static Staking_ChangeCandidate? _defaultInstance;

  /// index
  @$pb.TagNumber(1)
  $fixnum.Int64 get bucketIndex => $_getI64(0);
  @$pb.TagNumber(1)
  set bucketIndex($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasBucketIndex() => $_has(0);
  @$pb.TagNumber(1)
  void clearBucketIndex() => clearField(1);

  /// validator name
  @$pb.TagNumber(2)
  $core.String get candidateName => $_getSZ(1);
  @$pb.TagNumber(2)
  set candidateName($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasCandidateName() => $_has(1);
  @$pb.TagNumber(2)
  void clearCandidateName() => clearField(2);

  /// payload data
  @$pb.TagNumber(3)
  $core.List<$core.int> get payload => $_getN(2);
  @$pb.TagNumber(3)
  set payload($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasPayload() => $_has(2);
  @$pb.TagNumber(3)
  void clearPayload() => clearField(3);
}

/// transfer ownserhip of stake
class Staking_TransferOwnership extends $pb.GeneratedMessage {
  factory Staking_TransferOwnership({
    $fixnum.Int64? bucketIndex,
    $core.String? voterAddress,
    $core.List<$core.int>? payload,
  }) {
    final $result = create();
    if (bucketIndex != null) {
      $result.bucketIndex = bucketIndex;
    }
    if (voterAddress != null) {
      $result.voterAddress = voterAddress;
    }
    if (payload != null) {
      $result.payload = payload;
    }
    return $result;
  }
  Staking_TransferOwnership._() : super();
  factory Staking_TransferOwnership.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_TransferOwnership.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.TransferOwnership', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IoTeX.Proto'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'bucketIndex', $pb.PbFieldType.OU6, protoName: 'bucketIndex', defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(2, _omitFieldNames ? '' : 'voterAddress', protoName: 'voterAddress')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'payload', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_TransferOwnership clone() => Staking_TransferOwnership()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_TransferOwnership copyWith(void Function(Staking_TransferOwnership) updates) => super.copyWith((message) => updates(message as Staking_TransferOwnership)) as Staking_TransferOwnership;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_TransferOwnership create() => Staking_TransferOwnership._();
  Staking_TransferOwnership createEmptyInstance() => create();
  static $pb.PbList<Staking_TransferOwnership> createRepeated() => $pb.PbList<Staking_TransferOwnership>();
  @$core.pragma('dart2js:noInline')
  static Staking_TransferOwnership getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_TransferOwnership>(create);
  static Staking_TransferOwnership? _defaultInstance;

  /// index
  @$pb.TagNumber(1)
  $fixnum.Int64 get bucketIndex => $_getI64(0);
  @$pb.TagNumber(1)
  set bucketIndex($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasBucketIndex() => $_has(0);
  @$pb.TagNumber(1)
  void clearBucketIndex() => clearField(1);

  /// address of voter
  @$pb.TagNumber(2)
  $core.String get voterAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set voterAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasVoterAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearVoterAddress() => clearField(2);

  /// payload data
  @$pb.TagNumber(3)
  $core.List<$core.int> get payload => $_getN(2);
  @$pb.TagNumber(3)
  set payload($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasPayload() => $_has(2);
  @$pb.TagNumber(3)
  void clearPayload() => clearField(3);
}

/// Candidate (validator) info
class Staking_CandidateBasicInfo extends $pb.GeneratedMessage {
  factory Staking_CandidateBasicInfo({
    $core.String? name,
    $core.String? operatorAddress,
    $core.String? rewardAddress,
  }) {
    final $result = create();
    if (name != null) {
      $result.name = name;
    }
    if (operatorAddress != null) {
      $result.operatorAddress = operatorAddress;
    }
    if (rewardAddress != null) {
      $result.rewardAddress = rewardAddress;
    }
    return $result;
  }
  Staking_CandidateBasicInfo._() : super();
  factory Staking_CandidateBasicInfo.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_CandidateBasicInfo.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.CandidateBasicInfo', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IoTeX.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'name')
    ..aOS(2, _omitFieldNames ? '' : 'operatorAddress', protoName: 'operatorAddress')
    ..aOS(3, _omitFieldNames ? '' : 'rewardAddress', protoName: 'rewardAddress')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_CandidateBasicInfo clone() => Staking_CandidateBasicInfo()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_CandidateBasicInfo copyWith(void Function(Staking_CandidateBasicInfo) updates) => super.copyWith((message) => updates(message as Staking_CandidateBasicInfo)) as Staking_CandidateBasicInfo;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_CandidateBasicInfo create() => Staking_CandidateBasicInfo._();
  Staking_CandidateBasicInfo createEmptyInstance() => create();
  static $pb.PbList<Staking_CandidateBasicInfo> createRepeated() => $pb.PbList<Staking_CandidateBasicInfo>();
  @$core.pragma('dart2js:noInline')
  static Staking_CandidateBasicInfo getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_CandidateBasicInfo>(create);
  static Staking_CandidateBasicInfo? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get name => $_getSZ(0);
  @$pb.TagNumber(1)
  set name($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasName() => $_has(0);
  @$pb.TagNumber(1)
  void clearName() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get operatorAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set operatorAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasOperatorAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearOperatorAddress() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get rewardAddress => $_getSZ(2);
  @$pb.TagNumber(3)
  set rewardAddress($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasRewardAddress() => $_has(2);
  @$pb.TagNumber(3)
  void clearRewardAddress() => clearField(3);
}

/// Register a Candidate
class Staking_CandidateRegister extends $pb.GeneratedMessage {
  factory Staking_CandidateRegister({
    Staking_CandidateBasicInfo? candidate,
    $core.String? stakedAmount,
    $core.int? stakedDuration,
    $core.bool? autoStake,
    $core.String? ownerAddress,
    $core.List<$core.int>? payload,
  }) {
    final $result = create();
    if (candidate != null) {
      $result.candidate = candidate;
    }
    if (stakedAmount != null) {
      $result.stakedAmount = stakedAmount;
    }
    if (stakedDuration != null) {
      $result.stakedDuration = stakedDuration;
    }
    if (autoStake != null) {
      $result.autoStake = autoStake;
    }
    if (ownerAddress != null) {
      $result.ownerAddress = ownerAddress;
    }
    if (payload != null) {
      $result.payload = payload;
    }
    return $result;
  }
  Staking_CandidateRegister._() : super();
  factory Staking_CandidateRegister.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking_CandidateRegister.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking.CandidateRegister', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IoTeX.Proto'), createEmptyInstance: create)
    ..aOM<Staking_CandidateBasicInfo>(1, _omitFieldNames ? '' : 'candidate', subBuilder: Staking_CandidateBasicInfo.create)
    ..aOS(2, _omitFieldNames ? '' : 'stakedAmount', protoName: 'stakedAmount')
    ..a<$core.int>(3, _omitFieldNames ? '' : 'stakedDuration', $pb.PbFieldType.OU3, protoName: 'stakedDuration')
    ..aOB(4, _omitFieldNames ? '' : 'autoStake', protoName: 'autoStake')
    ..aOS(5, _omitFieldNames ? '' : 'ownerAddress', protoName: 'ownerAddress')
    ..a<$core.List<$core.int>>(6, _omitFieldNames ? '' : 'payload', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Staking_CandidateRegister clone() => Staking_CandidateRegister()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Staking_CandidateRegister copyWith(void Function(Staking_CandidateRegister) updates) => super.copyWith((message) => updates(message as Staking_CandidateRegister)) as Staking_CandidateRegister;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Staking_CandidateRegister create() => Staking_CandidateRegister._();
  Staking_CandidateRegister createEmptyInstance() => create();
  static $pb.PbList<Staking_CandidateRegister> createRepeated() => $pb.PbList<Staking_CandidateRegister>();
  @$core.pragma('dart2js:noInline')
  static Staking_CandidateRegister getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Staking_CandidateRegister>(create);
  static Staking_CandidateRegister? _defaultInstance;

  @$pb.TagNumber(1)
  Staking_CandidateBasicInfo get candidate => $_getN(0);
  @$pb.TagNumber(1)
  set candidate(Staking_CandidateBasicInfo v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasCandidate() => $_has(0);
  @$pb.TagNumber(1)
  void clearCandidate() => clearField(1);
  @$pb.TagNumber(1)
  Staking_CandidateBasicInfo ensureCandidate() => $_ensure(0);

  @$pb.TagNumber(2)
  $core.String get stakedAmount => $_getSZ(1);
  @$pb.TagNumber(2)
  set stakedAmount($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasStakedAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearStakedAmount() => clearField(2);

  @$pb.TagNumber(3)
  $core.int get stakedDuration => $_getIZ(2);
  @$pb.TagNumber(3)
  set stakedDuration($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasStakedDuration() => $_has(2);
  @$pb.TagNumber(3)
  void clearStakedDuration() => clearField(3);

  @$pb.TagNumber(4)
  $core.bool get autoStake => $_getBF(3);
  @$pb.TagNumber(4)
  set autoStake($core.bool v) { $_setBool(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasAutoStake() => $_has(3);
  @$pb.TagNumber(4)
  void clearAutoStake() => clearField(4);

  @$pb.TagNumber(5)
  $core.String get ownerAddress => $_getSZ(4);
  @$pb.TagNumber(5)
  set ownerAddress($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasOwnerAddress() => $_has(4);
  @$pb.TagNumber(5)
  void clearOwnerAddress() => clearField(5);

  @$pb.TagNumber(6)
  $core.List<$core.int> get payload => $_getN(5);
  @$pb.TagNumber(6)
  set payload($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasPayload() => $_has(5);
  @$pb.TagNumber(6)
  void clearPayload() => clearField(6);
}

enum Staking_Message {
  stakeCreate, 
  stakeUnstake, 
  stakeWithdraw, 
  stakeAddDeposit, 
  stakeRestake, 
  stakeChangeCandidate, 
  stakeTransferOwnership, 
  candidateRegister, 
  candidateUpdate, 
  notSet
}

/// A Staking message
class Staking extends $pb.GeneratedMessage {
  factory Staking({
    Staking_Create? stakeCreate,
    Staking_Reclaim? stakeUnstake,
    Staking_Reclaim? stakeWithdraw,
    Staking_AddDeposit? stakeAddDeposit,
    Staking_Restake? stakeRestake,
    Staking_ChangeCandidate? stakeChangeCandidate,
    Staking_TransferOwnership? stakeTransferOwnership,
    Staking_CandidateRegister? candidateRegister,
    Staking_CandidateBasicInfo? candidateUpdate,
  }) {
    final $result = create();
    if (stakeCreate != null) {
      $result.stakeCreate = stakeCreate;
    }
    if (stakeUnstake != null) {
      $result.stakeUnstake = stakeUnstake;
    }
    if (stakeWithdraw != null) {
      $result.stakeWithdraw = stakeWithdraw;
    }
    if (stakeAddDeposit != null) {
      $result.stakeAddDeposit = stakeAddDeposit;
    }
    if (stakeRestake != null) {
      $result.stakeRestake = stakeRestake;
    }
    if (stakeChangeCandidate != null) {
      $result.stakeChangeCandidate = stakeChangeCandidate;
    }
    if (stakeTransferOwnership != null) {
      $result.stakeTransferOwnership = stakeTransferOwnership;
    }
    if (candidateRegister != null) {
      $result.candidateRegister = candidateRegister;
    }
    if (candidateUpdate != null) {
      $result.candidateUpdate = candidateUpdate;
    }
    return $result;
  }
  Staking._() : super();
  factory Staking.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Staking.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Staking_Message> _Staking_MessageByTag = {
    1 : Staking_Message.stakeCreate,
    2 : Staking_Message.stakeUnstake,
    3 : Staking_Message.stakeWithdraw,
    4 : Staking_Message.stakeAddDeposit,
    5 : Staking_Message.stakeRestake,
    6 : Staking_Message.stakeChangeCandidate,
    7 : Staking_Message.stakeTransferOwnership,
    8 : Staking_Message.candidateRegister,
    9 : Staking_Message.candidateUpdate,
    0 : Staking_Message.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Staking', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IoTeX.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2, 3, 4, 5, 6, 7, 8, 9])
    ..aOM<Staking_Create>(1, _omitFieldNames ? '' : 'stakeCreate', protoName: 'stakeCreate', subBuilder: Staking_Create.create)
    ..aOM<Staking_Reclaim>(2, _omitFieldNames ? '' : 'stakeUnstake', protoName: 'stakeUnstake', subBuilder: Staking_Reclaim.create)
    ..aOM<Staking_Reclaim>(3, _omitFieldNames ? '' : 'stakeWithdraw', protoName: 'stakeWithdraw', subBuilder: Staking_Reclaim.create)
    ..aOM<Staking_AddDeposit>(4, _omitFieldNames ? '' : 'stakeAddDeposit', protoName: 'stakeAddDeposit', subBuilder: Staking_AddDeposit.create)
    ..aOM<Staking_Restake>(5, _omitFieldNames ? '' : 'stakeRestake', protoName: 'stakeRestake', subBuilder: Staking_Restake.create)
    ..aOM<Staking_ChangeCandidate>(6, _omitFieldNames ? '' : 'stakeChangeCandidate', protoName: 'stakeChangeCandidate', subBuilder: Staking_ChangeCandidate.create)
    ..aOM<Staking_TransferOwnership>(7, _omitFieldNames ? '' : 'stakeTransferOwnership', protoName: 'stakeTransferOwnership', subBuilder: Staking_TransferOwnership.create)
    ..aOM<Staking_CandidateRegister>(8, _omitFieldNames ? '' : 'candidateRegister', protoName: 'candidateRegister', subBuilder: Staking_CandidateRegister.create)
    ..aOM<Staking_CandidateBasicInfo>(9, _omitFieldNames ? '' : 'candidateUpdate', protoName: 'candidateUpdate', subBuilder: Staking_CandidateBasicInfo.create)
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

  Staking_Message whichMessage() => _Staking_MessageByTag[$_whichOneof(0)]!;
  void clearMessage() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  Staking_Create get stakeCreate => $_getN(0);
  @$pb.TagNumber(1)
  set stakeCreate(Staking_Create v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasStakeCreate() => $_has(0);
  @$pb.TagNumber(1)
  void clearStakeCreate() => clearField(1);
  @$pb.TagNumber(1)
  Staking_Create ensureStakeCreate() => $_ensure(0);

  @$pb.TagNumber(2)
  Staking_Reclaim get stakeUnstake => $_getN(1);
  @$pb.TagNumber(2)
  set stakeUnstake(Staking_Reclaim v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasStakeUnstake() => $_has(1);
  @$pb.TagNumber(2)
  void clearStakeUnstake() => clearField(2);
  @$pb.TagNumber(2)
  Staking_Reclaim ensureStakeUnstake() => $_ensure(1);

  @$pb.TagNumber(3)
  Staking_Reclaim get stakeWithdraw => $_getN(2);
  @$pb.TagNumber(3)
  set stakeWithdraw(Staking_Reclaim v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasStakeWithdraw() => $_has(2);
  @$pb.TagNumber(3)
  void clearStakeWithdraw() => clearField(3);
  @$pb.TagNumber(3)
  Staking_Reclaim ensureStakeWithdraw() => $_ensure(2);

  @$pb.TagNumber(4)
  Staking_AddDeposit get stakeAddDeposit => $_getN(3);
  @$pb.TagNumber(4)
  set stakeAddDeposit(Staking_AddDeposit v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasStakeAddDeposit() => $_has(3);
  @$pb.TagNumber(4)
  void clearStakeAddDeposit() => clearField(4);
  @$pb.TagNumber(4)
  Staking_AddDeposit ensureStakeAddDeposit() => $_ensure(3);

  @$pb.TagNumber(5)
  Staking_Restake get stakeRestake => $_getN(4);
  @$pb.TagNumber(5)
  set stakeRestake(Staking_Restake v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasStakeRestake() => $_has(4);
  @$pb.TagNumber(5)
  void clearStakeRestake() => clearField(5);
  @$pb.TagNumber(5)
  Staking_Restake ensureStakeRestake() => $_ensure(4);

  @$pb.TagNumber(6)
  Staking_ChangeCandidate get stakeChangeCandidate => $_getN(5);
  @$pb.TagNumber(6)
  set stakeChangeCandidate(Staking_ChangeCandidate v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasStakeChangeCandidate() => $_has(5);
  @$pb.TagNumber(6)
  void clearStakeChangeCandidate() => clearField(6);
  @$pb.TagNumber(6)
  Staking_ChangeCandidate ensureStakeChangeCandidate() => $_ensure(5);

  @$pb.TagNumber(7)
  Staking_TransferOwnership get stakeTransferOwnership => $_getN(6);
  @$pb.TagNumber(7)
  set stakeTransferOwnership(Staking_TransferOwnership v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasStakeTransferOwnership() => $_has(6);
  @$pb.TagNumber(7)
  void clearStakeTransferOwnership() => clearField(7);
  @$pb.TagNumber(7)
  Staking_TransferOwnership ensureStakeTransferOwnership() => $_ensure(6);

  @$pb.TagNumber(8)
  Staking_CandidateRegister get candidateRegister => $_getN(7);
  @$pb.TagNumber(8)
  set candidateRegister(Staking_CandidateRegister v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasCandidateRegister() => $_has(7);
  @$pb.TagNumber(8)
  void clearCandidateRegister() => clearField(8);
  @$pb.TagNumber(8)
  Staking_CandidateRegister ensureCandidateRegister() => $_ensure(7);

  @$pb.TagNumber(9)
  Staking_CandidateBasicInfo get candidateUpdate => $_getN(8);
  @$pb.TagNumber(9)
  set candidateUpdate(Staking_CandidateBasicInfo v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasCandidateUpdate() => $_has(8);
  @$pb.TagNumber(9)
  void clearCandidateUpdate() => clearField(9);
  @$pb.TagNumber(9)
  Staking_CandidateBasicInfo ensureCandidateUpdate() => $_ensure(8);
}

/// Arbitrary contract call
class ContractCall extends $pb.GeneratedMessage {
  factory ContractCall({
    $core.String? amount,
    $core.String? contract,
    $core.List<$core.int>? data,
  }) {
    final $result = create();
    if (amount != null) {
      $result.amount = amount;
    }
    if (contract != null) {
      $result.contract = contract;
    }
    if (data != null) {
      $result.data = data;
    }
    return $result;
  }
  ContractCall._() : super();
  factory ContractCall.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ContractCall.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'ContractCall', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IoTeX.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'amount')
    ..aOS(2, _omitFieldNames ? '' : 'contract')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'data', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ContractCall clone() => ContractCall()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ContractCall copyWith(void Function(ContractCall) updates) => super.copyWith((message) => updates(message as ContractCall)) as ContractCall;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static ContractCall create() => ContractCall._();
  ContractCall createEmptyInstance() => create();
  static $pb.PbList<ContractCall> createRepeated() => $pb.PbList<ContractCall>();
  @$core.pragma('dart2js:noInline')
  static ContractCall getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ContractCall>(create);
  static ContractCall? _defaultInstance;

  /// amount
  @$pb.TagNumber(1)
  $core.String get amount => $_getSZ(0);
  @$pb.TagNumber(1)
  set amount($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAmount() => $_has(0);
  @$pb.TagNumber(1)
  void clearAmount() => clearField(1);

  /// contract address
  @$pb.TagNumber(2)
  $core.String get contract => $_getSZ(1);
  @$pb.TagNumber(2)
  set contract($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasContract() => $_has(1);
  @$pb.TagNumber(2)
  void clearContract() => clearField(2);

  /// payload data
  @$pb.TagNumber(3)
  $core.List<$core.int> get data => $_getN(2);
  @$pb.TagNumber(3)
  set data($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasData() => $_has(2);
  @$pb.TagNumber(3)
  void clearData() => clearField(3);
}

enum SigningInput_Action {
  transfer, 
  call, 
  stakeCreate, 
  stakeUnstake, 
  stakeWithdraw, 
  stakeAddDeposit, 
  stakeRestake, 
  stakeChangeCandidate, 
  stakeTransferOwnership, 
  candidateRegister, 
  candidateUpdate, 
  notSet
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.int? version,
    $fixnum.Int64? nonce,
    $fixnum.Int64? gasLimit,
    $core.String? gasPrice,
    $core.int? chainID,
    $core.List<$core.int>? privateKey,
    Transfer? transfer,
    ContractCall? call,
    Staking_Create? stakeCreate,
    Staking_Reclaim? stakeUnstake,
    Staking_Reclaim? stakeWithdraw,
    Staking_AddDeposit? stakeAddDeposit,
    Staking_Restake? stakeRestake,
    Staking_ChangeCandidate? stakeChangeCandidate,
    Staking_TransferOwnership? stakeTransferOwnership,
    Staking_CandidateRegister? candidateRegister,
    Staking_CandidateBasicInfo? candidateUpdate,
  }) {
    final $result = create();
    if (version != null) {
      $result.version = version;
    }
    if (nonce != null) {
      $result.nonce = nonce;
    }
    if (gasLimit != null) {
      $result.gasLimit = gasLimit;
    }
    if (gasPrice != null) {
      $result.gasPrice = gasPrice;
    }
    if (chainID != null) {
      $result.chainID = chainID;
    }
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (transfer != null) {
      $result.transfer = transfer;
    }
    if (call != null) {
      $result.call = call;
    }
    if (stakeCreate != null) {
      $result.stakeCreate = stakeCreate;
    }
    if (stakeUnstake != null) {
      $result.stakeUnstake = stakeUnstake;
    }
    if (stakeWithdraw != null) {
      $result.stakeWithdraw = stakeWithdraw;
    }
    if (stakeAddDeposit != null) {
      $result.stakeAddDeposit = stakeAddDeposit;
    }
    if (stakeRestake != null) {
      $result.stakeRestake = stakeRestake;
    }
    if (stakeChangeCandidate != null) {
      $result.stakeChangeCandidate = stakeChangeCandidate;
    }
    if (stakeTransferOwnership != null) {
      $result.stakeTransferOwnership = stakeTransferOwnership;
    }
    if (candidateRegister != null) {
      $result.candidateRegister = candidateRegister;
    }
    if (candidateUpdate != null) {
      $result.candidateUpdate = candidateUpdate;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, SigningInput_Action> _SigningInput_ActionByTag = {
    10 : SigningInput_Action.transfer,
    12 : SigningInput_Action.call,
    40 : SigningInput_Action.stakeCreate,
    41 : SigningInput_Action.stakeUnstake,
    42 : SigningInput_Action.stakeWithdraw,
    43 : SigningInput_Action.stakeAddDeposit,
    44 : SigningInput_Action.stakeRestake,
    45 : SigningInput_Action.stakeChangeCandidate,
    46 : SigningInput_Action.stakeTransferOwnership,
    47 : SigningInput_Action.candidateRegister,
    48 : SigningInput_Action.candidateUpdate,
    0 : SigningInput_Action.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IoTeX.Proto'), createEmptyInstance: create)
    ..oo(0, [10, 12, 40, 41, 42, 43, 44, 45, 46, 47, 48])
    ..a<$core.int>(1, _omitFieldNames ? '' : 'version', $pb.PbFieldType.OU3)
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'nonce', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'gasLimit', $pb.PbFieldType.OU6, protoName: 'gasLimit', defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(4, _omitFieldNames ? '' : 'gasPrice', protoName: 'gasPrice')
    ..a<$core.int>(5, _omitFieldNames ? '' : 'chainID', $pb.PbFieldType.OU3, protoName: 'chainID')
    ..a<$core.List<$core.int>>(6, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY, protoName: 'privateKey')
    ..aOM<Transfer>(10, _omitFieldNames ? '' : 'transfer', subBuilder: Transfer.create)
    ..aOM<ContractCall>(12, _omitFieldNames ? '' : 'call', subBuilder: ContractCall.create)
    ..aOM<Staking_Create>(40, _omitFieldNames ? '' : 'stakeCreate', protoName: 'stakeCreate', subBuilder: Staking_Create.create)
    ..aOM<Staking_Reclaim>(41, _omitFieldNames ? '' : 'stakeUnstake', protoName: 'stakeUnstake', subBuilder: Staking_Reclaim.create)
    ..aOM<Staking_Reclaim>(42, _omitFieldNames ? '' : 'stakeWithdraw', protoName: 'stakeWithdraw', subBuilder: Staking_Reclaim.create)
    ..aOM<Staking_AddDeposit>(43, _omitFieldNames ? '' : 'stakeAddDeposit', protoName: 'stakeAddDeposit', subBuilder: Staking_AddDeposit.create)
    ..aOM<Staking_Restake>(44, _omitFieldNames ? '' : 'stakeRestake', protoName: 'stakeRestake', subBuilder: Staking_Restake.create)
    ..aOM<Staking_ChangeCandidate>(45, _omitFieldNames ? '' : 'stakeChangeCandidate', protoName: 'stakeChangeCandidate', subBuilder: Staking_ChangeCandidate.create)
    ..aOM<Staking_TransferOwnership>(46, _omitFieldNames ? '' : 'stakeTransferOwnership', protoName: 'stakeTransferOwnership', subBuilder: Staking_TransferOwnership.create)
    ..aOM<Staking_CandidateRegister>(47, _omitFieldNames ? '' : 'candidateRegister', protoName: 'candidateRegister', subBuilder: Staking_CandidateRegister.create)
    ..aOM<Staking_CandidateBasicInfo>(48, _omitFieldNames ? '' : 'candidateUpdate', protoName: 'candidateUpdate', subBuilder: Staking_CandidateBasicInfo.create)
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

  SigningInput_Action whichAction() => _SigningInput_ActionByTag[$_whichOneof(0)]!;
  void clearAction() => clearField($_whichOneof(0));

  /// Transaction version
  @$pb.TagNumber(1)
  $core.int get version => $_getIZ(0);
  @$pb.TagNumber(1)
  set version($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasVersion() => $_has(0);
  @$pb.TagNumber(1)
  void clearVersion() => clearField(1);

  /// Nonce (should be larger than in the last transaction of the account)
  @$pb.TagNumber(2)
  $fixnum.Int64 get nonce => $_getI64(1);
  @$pb.TagNumber(2)
  set nonce($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasNonce() => $_has(1);
  @$pb.TagNumber(2)
  void clearNonce() => clearField(2);

  /// Limit for the gas used
  @$pb.TagNumber(3)
  $fixnum.Int64 get gasLimit => $_getI64(2);
  @$pb.TagNumber(3)
  set gasLimit($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasGasLimit() => $_has(2);
  @$pb.TagNumber(3)
  void clearGasLimit() => clearField(3);

  /// Gas price
  @$pb.TagNumber(4)
  $core.String get gasPrice => $_getSZ(3);
  @$pb.TagNumber(4)
  set gasPrice($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasGasPrice() => $_has(3);
  @$pb.TagNumber(4)
  void clearGasPrice() => clearField(4);

  /// The chain id of blockchain
  @$pb.TagNumber(5)
  $core.int get chainID => $_getIZ(4);
  @$pb.TagNumber(5)
  set chainID($core.int v) { $_setUnsignedInt32(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasChainID() => $_has(4);
  @$pb.TagNumber(5)
  void clearChainID() => clearField(5);

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(6)
  $core.List<$core.int> get privateKey => $_getN(5);
  @$pb.TagNumber(6)
  set privateKey($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasPrivateKey() => $_has(5);
  @$pb.TagNumber(6)
  void clearPrivateKey() => clearField(6);

  @$pb.TagNumber(10)
  Transfer get transfer => $_getN(6);
  @$pb.TagNumber(10)
  set transfer(Transfer v) { setField(10, v); }
  @$pb.TagNumber(10)
  $core.bool hasTransfer() => $_has(6);
  @$pb.TagNumber(10)
  void clearTransfer() => clearField(10);
  @$pb.TagNumber(10)
  Transfer ensureTransfer() => $_ensure(6);

  @$pb.TagNumber(12)
  ContractCall get call => $_getN(7);
  @$pb.TagNumber(12)
  set call(ContractCall v) { setField(12, v); }
  @$pb.TagNumber(12)
  $core.bool hasCall() => $_has(7);
  @$pb.TagNumber(12)
  void clearCall() => clearField(12);
  @$pb.TagNumber(12)
  ContractCall ensureCall() => $_ensure(7);

  /// Native staking
  @$pb.TagNumber(40)
  Staking_Create get stakeCreate => $_getN(8);
  @$pb.TagNumber(40)
  set stakeCreate(Staking_Create v) { setField(40, v); }
  @$pb.TagNumber(40)
  $core.bool hasStakeCreate() => $_has(8);
  @$pb.TagNumber(40)
  void clearStakeCreate() => clearField(40);
  @$pb.TagNumber(40)
  Staking_Create ensureStakeCreate() => $_ensure(8);

  @$pb.TagNumber(41)
  Staking_Reclaim get stakeUnstake => $_getN(9);
  @$pb.TagNumber(41)
  set stakeUnstake(Staking_Reclaim v) { setField(41, v); }
  @$pb.TagNumber(41)
  $core.bool hasStakeUnstake() => $_has(9);
  @$pb.TagNumber(41)
  void clearStakeUnstake() => clearField(41);
  @$pb.TagNumber(41)
  Staking_Reclaim ensureStakeUnstake() => $_ensure(9);

  @$pb.TagNumber(42)
  Staking_Reclaim get stakeWithdraw => $_getN(10);
  @$pb.TagNumber(42)
  set stakeWithdraw(Staking_Reclaim v) { setField(42, v); }
  @$pb.TagNumber(42)
  $core.bool hasStakeWithdraw() => $_has(10);
  @$pb.TagNumber(42)
  void clearStakeWithdraw() => clearField(42);
  @$pb.TagNumber(42)
  Staking_Reclaim ensureStakeWithdraw() => $_ensure(10);

  @$pb.TagNumber(43)
  Staking_AddDeposit get stakeAddDeposit => $_getN(11);
  @$pb.TagNumber(43)
  set stakeAddDeposit(Staking_AddDeposit v) { setField(43, v); }
  @$pb.TagNumber(43)
  $core.bool hasStakeAddDeposit() => $_has(11);
  @$pb.TagNumber(43)
  void clearStakeAddDeposit() => clearField(43);
  @$pb.TagNumber(43)
  Staking_AddDeposit ensureStakeAddDeposit() => $_ensure(11);

  @$pb.TagNumber(44)
  Staking_Restake get stakeRestake => $_getN(12);
  @$pb.TagNumber(44)
  set stakeRestake(Staking_Restake v) { setField(44, v); }
  @$pb.TagNumber(44)
  $core.bool hasStakeRestake() => $_has(12);
  @$pb.TagNumber(44)
  void clearStakeRestake() => clearField(44);
  @$pb.TagNumber(44)
  Staking_Restake ensureStakeRestake() => $_ensure(12);

  @$pb.TagNumber(45)
  Staking_ChangeCandidate get stakeChangeCandidate => $_getN(13);
  @$pb.TagNumber(45)
  set stakeChangeCandidate(Staking_ChangeCandidate v) { setField(45, v); }
  @$pb.TagNumber(45)
  $core.bool hasStakeChangeCandidate() => $_has(13);
  @$pb.TagNumber(45)
  void clearStakeChangeCandidate() => clearField(45);
  @$pb.TagNumber(45)
  Staking_ChangeCandidate ensureStakeChangeCandidate() => $_ensure(13);

  @$pb.TagNumber(46)
  Staking_TransferOwnership get stakeTransferOwnership => $_getN(14);
  @$pb.TagNumber(46)
  set stakeTransferOwnership(Staking_TransferOwnership v) { setField(46, v); }
  @$pb.TagNumber(46)
  $core.bool hasStakeTransferOwnership() => $_has(14);
  @$pb.TagNumber(46)
  void clearStakeTransferOwnership() => clearField(46);
  @$pb.TagNumber(46)
  Staking_TransferOwnership ensureStakeTransferOwnership() => $_ensure(14);

  @$pb.TagNumber(47)
  Staking_CandidateRegister get candidateRegister => $_getN(15);
  @$pb.TagNumber(47)
  set candidateRegister(Staking_CandidateRegister v) { setField(47, v); }
  @$pb.TagNumber(47)
  $core.bool hasCandidateRegister() => $_has(15);
  @$pb.TagNumber(47)
  void clearCandidateRegister() => clearField(47);
  @$pb.TagNumber(47)
  Staking_CandidateRegister ensureCandidateRegister() => $_ensure(15);

  @$pb.TagNumber(48)
  Staking_CandidateBasicInfo get candidateUpdate => $_getN(16);
  @$pb.TagNumber(48)
  set candidateUpdate(Staking_CandidateBasicInfo v) { setField(48, v); }
  @$pb.TagNumber(48)
  $core.bool hasCandidateUpdate() => $_has(16);
  @$pb.TagNumber(48)
  void clearCandidateUpdate() => clearField(48);
  @$pb.TagNumber(48)
  Staking_CandidateBasicInfo ensureCandidateUpdate() => $_ensure(16);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.List<$core.int>? encoded,
    $core.List<$core.int>? hash,
    $0.SigningError? error,
    $core.String? errorMessage,
  }) {
    final $result = create();
    if (encoded != null) {
      $result.encoded = encoded;
    }
    if (hash != null) {
      $result.hash = hash;
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

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IoTeX.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'encoded', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'hash', $pb.PbFieldType.OY)
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

  /// Signed and encoded Action bytes
  @$pb.TagNumber(1)
  $core.List<$core.int> get encoded => $_getN(0);
  @$pb.TagNumber(1)
  set encoded($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasEncoded() => $_has(0);
  @$pb.TagNumber(1)
  void clearEncoded() => clearField(1);

  /// Signed Action hash
  @$pb.TagNumber(2)
  $core.List<$core.int> get hash => $_getN(1);
  @$pb.TagNumber(2)
  set hash($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasHash() => $_has(1);
  @$pb.TagNumber(2)
  void clearHash() => clearField(2);

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

enum ActionCore_Action {
  transfer, 
  execution, 
  stakeCreate, 
  stakeUnstake, 
  stakeWithdraw, 
  stakeAddDeposit, 
  stakeRestake, 
  stakeChangeCandidate, 
  stakeTransferOwnership, 
  candidateRegister, 
  candidateUpdate, 
  notSet
}

/// An Action structure
/// Used internally
class ActionCore extends $pb.GeneratedMessage {
  factory ActionCore({
    $core.int? version,
    $fixnum.Int64? nonce,
    $fixnum.Int64? gasLimit,
    $core.String? gasPrice,
    $core.int? chainID,
    Transfer? transfer,
    ContractCall? execution,
    Staking_Create? stakeCreate,
    Staking_Reclaim? stakeUnstake,
    Staking_Reclaim? stakeWithdraw,
    Staking_AddDeposit? stakeAddDeposit,
    Staking_Restake? stakeRestake,
    Staking_ChangeCandidate? stakeChangeCandidate,
    Staking_TransferOwnership? stakeTransferOwnership,
    Staking_CandidateRegister? candidateRegister,
    Staking_CandidateBasicInfo? candidateUpdate,
  }) {
    final $result = create();
    if (version != null) {
      $result.version = version;
    }
    if (nonce != null) {
      $result.nonce = nonce;
    }
    if (gasLimit != null) {
      $result.gasLimit = gasLimit;
    }
    if (gasPrice != null) {
      $result.gasPrice = gasPrice;
    }
    if (chainID != null) {
      $result.chainID = chainID;
    }
    if (transfer != null) {
      $result.transfer = transfer;
    }
    if (execution != null) {
      $result.execution = execution;
    }
    if (stakeCreate != null) {
      $result.stakeCreate = stakeCreate;
    }
    if (stakeUnstake != null) {
      $result.stakeUnstake = stakeUnstake;
    }
    if (stakeWithdraw != null) {
      $result.stakeWithdraw = stakeWithdraw;
    }
    if (stakeAddDeposit != null) {
      $result.stakeAddDeposit = stakeAddDeposit;
    }
    if (stakeRestake != null) {
      $result.stakeRestake = stakeRestake;
    }
    if (stakeChangeCandidate != null) {
      $result.stakeChangeCandidate = stakeChangeCandidate;
    }
    if (stakeTransferOwnership != null) {
      $result.stakeTransferOwnership = stakeTransferOwnership;
    }
    if (candidateRegister != null) {
      $result.candidateRegister = candidateRegister;
    }
    if (candidateUpdate != null) {
      $result.candidateUpdate = candidateUpdate;
    }
    return $result;
  }
  ActionCore._() : super();
  factory ActionCore.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ActionCore.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, ActionCore_Action> _ActionCore_ActionByTag = {
    10 : ActionCore_Action.transfer,
    12 : ActionCore_Action.execution,
    40 : ActionCore_Action.stakeCreate,
    41 : ActionCore_Action.stakeUnstake,
    42 : ActionCore_Action.stakeWithdraw,
    43 : ActionCore_Action.stakeAddDeposit,
    44 : ActionCore_Action.stakeRestake,
    45 : ActionCore_Action.stakeChangeCandidate,
    46 : ActionCore_Action.stakeTransferOwnership,
    47 : ActionCore_Action.candidateRegister,
    48 : ActionCore_Action.candidateUpdate,
    0 : ActionCore_Action.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'ActionCore', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IoTeX.Proto'), createEmptyInstance: create)
    ..oo(0, [10, 12, 40, 41, 42, 43, 44, 45, 46, 47, 48])
    ..a<$core.int>(1, _omitFieldNames ? '' : 'version', $pb.PbFieldType.OU3)
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'nonce', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'gasLimit', $pb.PbFieldType.OU6, protoName: 'gasLimit', defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(4, _omitFieldNames ? '' : 'gasPrice', protoName: 'gasPrice')
    ..a<$core.int>(5, _omitFieldNames ? '' : 'chainID', $pb.PbFieldType.OU3, protoName: 'chainID')
    ..aOM<Transfer>(10, _omitFieldNames ? '' : 'transfer', subBuilder: Transfer.create)
    ..aOM<ContractCall>(12, _omitFieldNames ? '' : 'execution', subBuilder: ContractCall.create)
    ..aOM<Staking_Create>(40, _omitFieldNames ? '' : 'stakeCreate', protoName: 'stakeCreate', subBuilder: Staking_Create.create)
    ..aOM<Staking_Reclaim>(41, _omitFieldNames ? '' : 'stakeUnstake', protoName: 'stakeUnstake', subBuilder: Staking_Reclaim.create)
    ..aOM<Staking_Reclaim>(42, _omitFieldNames ? '' : 'stakeWithdraw', protoName: 'stakeWithdraw', subBuilder: Staking_Reclaim.create)
    ..aOM<Staking_AddDeposit>(43, _omitFieldNames ? '' : 'stakeAddDeposit', protoName: 'stakeAddDeposit', subBuilder: Staking_AddDeposit.create)
    ..aOM<Staking_Restake>(44, _omitFieldNames ? '' : 'stakeRestake', protoName: 'stakeRestake', subBuilder: Staking_Restake.create)
    ..aOM<Staking_ChangeCandidate>(45, _omitFieldNames ? '' : 'stakeChangeCandidate', protoName: 'stakeChangeCandidate', subBuilder: Staking_ChangeCandidate.create)
    ..aOM<Staking_TransferOwnership>(46, _omitFieldNames ? '' : 'stakeTransferOwnership', protoName: 'stakeTransferOwnership', subBuilder: Staking_TransferOwnership.create)
    ..aOM<Staking_CandidateRegister>(47, _omitFieldNames ? '' : 'candidateRegister', protoName: 'candidateRegister', subBuilder: Staking_CandidateRegister.create)
    ..aOM<Staking_CandidateBasicInfo>(48, _omitFieldNames ? '' : 'candidateUpdate', protoName: 'candidateUpdate', subBuilder: Staking_CandidateBasicInfo.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ActionCore clone() => ActionCore()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ActionCore copyWith(void Function(ActionCore) updates) => super.copyWith((message) => updates(message as ActionCore)) as ActionCore;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static ActionCore create() => ActionCore._();
  ActionCore createEmptyInstance() => create();
  static $pb.PbList<ActionCore> createRepeated() => $pb.PbList<ActionCore>();
  @$core.pragma('dart2js:noInline')
  static ActionCore getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ActionCore>(create);
  static ActionCore? _defaultInstance;

  ActionCore_Action whichAction() => _ActionCore_ActionByTag[$_whichOneof(0)]!;
  void clearAction() => clearField($_whichOneof(0));

  /// version number
  @$pb.TagNumber(1)
  $core.int get version => $_getIZ(0);
  @$pb.TagNumber(1)
  set version($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasVersion() => $_has(0);
  @$pb.TagNumber(1)
  void clearVersion() => clearField(1);

  /// Nonce (should be larger than in the last transaction of the account)
  @$pb.TagNumber(2)
  $fixnum.Int64 get nonce => $_getI64(1);
  @$pb.TagNumber(2)
  set nonce($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasNonce() => $_has(1);
  @$pb.TagNumber(2)
  void clearNonce() => clearField(2);

  /// Gas limit
  @$pb.TagNumber(3)
  $fixnum.Int64 get gasLimit => $_getI64(2);
  @$pb.TagNumber(3)
  set gasLimit($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasGasLimit() => $_has(2);
  @$pb.TagNumber(3)
  void clearGasLimit() => clearField(3);

  /// Gas price
  @$pb.TagNumber(4)
  $core.String get gasPrice => $_getSZ(3);
  @$pb.TagNumber(4)
  set gasPrice($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasGasPrice() => $_has(3);
  @$pb.TagNumber(4)
  void clearGasPrice() => clearField(4);

  /// Chain ID
  @$pb.TagNumber(5)
  $core.int get chainID => $_getIZ(4);
  @$pb.TagNumber(5)
  set chainID($core.int v) { $_setUnsignedInt32(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasChainID() => $_has(4);
  @$pb.TagNumber(5)
  void clearChainID() => clearField(5);

  @$pb.TagNumber(10)
  Transfer get transfer => $_getN(5);
  @$pb.TagNumber(10)
  set transfer(Transfer v) { setField(10, v); }
  @$pb.TagNumber(10)
  $core.bool hasTransfer() => $_has(5);
  @$pb.TagNumber(10)
  void clearTransfer() => clearField(10);
  @$pb.TagNumber(10)
  Transfer ensureTransfer() => $_ensure(5);

  @$pb.TagNumber(12)
  ContractCall get execution => $_getN(6);
  @$pb.TagNumber(12)
  set execution(ContractCall v) { setField(12, v); }
  @$pb.TagNumber(12)
  $core.bool hasExecution() => $_has(6);
  @$pb.TagNumber(12)
  void clearExecution() => clearField(12);
  @$pb.TagNumber(12)
  ContractCall ensureExecution() => $_ensure(6);

  /// Native staking
  @$pb.TagNumber(40)
  Staking_Create get stakeCreate => $_getN(7);
  @$pb.TagNumber(40)
  set stakeCreate(Staking_Create v) { setField(40, v); }
  @$pb.TagNumber(40)
  $core.bool hasStakeCreate() => $_has(7);
  @$pb.TagNumber(40)
  void clearStakeCreate() => clearField(40);
  @$pb.TagNumber(40)
  Staking_Create ensureStakeCreate() => $_ensure(7);

  @$pb.TagNumber(41)
  Staking_Reclaim get stakeUnstake => $_getN(8);
  @$pb.TagNumber(41)
  set stakeUnstake(Staking_Reclaim v) { setField(41, v); }
  @$pb.TagNumber(41)
  $core.bool hasStakeUnstake() => $_has(8);
  @$pb.TagNumber(41)
  void clearStakeUnstake() => clearField(41);
  @$pb.TagNumber(41)
  Staking_Reclaim ensureStakeUnstake() => $_ensure(8);

  @$pb.TagNumber(42)
  Staking_Reclaim get stakeWithdraw => $_getN(9);
  @$pb.TagNumber(42)
  set stakeWithdraw(Staking_Reclaim v) { setField(42, v); }
  @$pb.TagNumber(42)
  $core.bool hasStakeWithdraw() => $_has(9);
  @$pb.TagNumber(42)
  void clearStakeWithdraw() => clearField(42);
  @$pb.TagNumber(42)
  Staking_Reclaim ensureStakeWithdraw() => $_ensure(9);

  @$pb.TagNumber(43)
  Staking_AddDeposit get stakeAddDeposit => $_getN(10);
  @$pb.TagNumber(43)
  set stakeAddDeposit(Staking_AddDeposit v) { setField(43, v); }
  @$pb.TagNumber(43)
  $core.bool hasStakeAddDeposit() => $_has(10);
  @$pb.TagNumber(43)
  void clearStakeAddDeposit() => clearField(43);
  @$pb.TagNumber(43)
  Staking_AddDeposit ensureStakeAddDeposit() => $_ensure(10);

  @$pb.TagNumber(44)
  Staking_Restake get stakeRestake => $_getN(11);
  @$pb.TagNumber(44)
  set stakeRestake(Staking_Restake v) { setField(44, v); }
  @$pb.TagNumber(44)
  $core.bool hasStakeRestake() => $_has(11);
  @$pb.TagNumber(44)
  void clearStakeRestake() => clearField(44);
  @$pb.TagNumber(44)
  Staking_Restake ensureStakeRestake() => $_ensure(11);

  @$pb.TagNumber(45)
  Staking_ChangeCandidate get stakeChangeCandidate => $_getN(12);
  @$pb.TagNumber(45)
  set stakeChangeCandidate(Staking_ChangeCandidate v) { setField(45, v); }
  @$pb.TagNumber(45)
  $core.bool hasStakeChangeCandidate() => $_has(12);
  @$pb.TagNumber(45)
  void clearStakeChangeCandidate() => clearField(45);
  @$pb.TagNumber(45)
  Staking_ChangeCandidate ensureStakeChangeCandidate() => $_ensure(12);

  @$pb.TagNumber(46)
  Staking_TransferOwnership get stakeTransferOwnership => $_getN(13);
  @$pb.TagNumber(46)
  set stakeTransferOwnership(Staking_TransferOwnership v) { setField(46, v); }
  @$pb.TagNumber(46)
  $core.bool hasStakeTransferOwnership() => $_has(13);
  @$pb.TagNumber(46)
  void clearStakeTransferOwnership() => clearField(46);
  @$pb.TagNumber(46)
  Staking_TransferOwnership ensureStakeTransferOwnership() => $_ensure(13);

  @$pb.TagNumber(47)
  Staking_CandidateRegister get candidateRegister => $_getN(14);
  @$pb.TagNumber(47)
  set candidateRegister(Staking_CandidateRegister v) { setField(47, v); }
  @$pb.TagNumber(47)
  $core.bool hasCandidateRegister() => $_has(14);
  @$pb.TagNumber(47)
  void clearCandidateRegister() => clearField(47);
  @$pb.TagNumber(47)
  Staking_CandidateRegister ensureCandidateRegister() => $_ensure(14);

  @$pb.TagNumber(48)
  Staking_CandidateBasicInfo get candidateUpdate => $_getN(15);
  @$pb.TagNumber(48)
  set candidateUpdate(Staking_CandidateBasicInfo v) { setField(48, v); }
  @$pb.TagNumber(48)
  $core.bool hasCandidateUpdate() => $_has(15);
  @$pb.TagNumber(48)
  void clearCandidateUpdate() => clearField(48);
  @$pb.TagNumber(48)
  Staking_CandidateBasicInfo ensureCandidateUpdate() => $_ensure(15);
}

/// Signed Action
/// Used internally
class Action extends $pb.GeneratedMessage {
  factory Action({
    ActionCore? core,
    $core.List<$core.int>? senderPubKey,
    $core.List<$core.int>? signature,
  }) {
    final $result = create();
    if (core != null) {
      $result.core = core;
    }
    if (senderPubKey != null) {
      $result.senderPubKey = senderPubKey;
    }
    if (signature != null) {
      $result.signature = signature;
    }
    return $result;
  }
  Action._() : super();
  factory Action.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Action.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Action', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.IoTeX.Proto'), createEmptyInstance: create)
    ..aOM<ActionCore>(1, _omitFieldNames ? '' : 'core', subBuilder: ActionCore.create)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'senderPubKey', $pb.PbFieldType.OY, protoName: 'senderPubKey')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'signature', $pb.PbFieldType.OY)
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

  /// Action details
  @$pb.TagNumber(1)
  ActionCore get core => $_getN(0);
  @$pb.TagNumber(1)
  set core(ActionCore v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasCore() => $_has(0);
  @$pb.TagNumber(1)
  void clearCore() => clearField(1);
  @$pb.TagNumber(1)
  ActionCore ensureCore() => $_ensure(0);

  /// public key
  @$pb.TagNumber(2)
  $core.List<$core.int> get senderPubKey => $_getN(1);
  @$pb.TagNumber(2)
  set senderPubKey($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasSenderPubKey() => $_has(1);
  @$pb.TagNumber(2)
  void clearSenderPubKey() => clearField(2);

  /// the signature
  @$pb.TagNumber(3)
  $core.List<$core.int> get signature => $_getN(2);
  @$pb.TagNumber(3)
  set signature($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSignature() => $_has(2);
  @$pb.TagNumber(3)
  void clearSignature() => clearField(3);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
