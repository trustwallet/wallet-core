//
//  Generated code. Do not modify.
//  source: Everscale.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'Everscale.pbenum.dart';

export 'Everscale.pbenum.dart';

enum Transfer_AccountStateOneof {
  encodedContractData, 
  notSet
}

/// Transfer message
class Transfer extends $pb.GeneratedMessage {
  factory Transfer({
    $core.bool? bounce,
    MessageBehavior? behavior,
    $fixnum.Int64? amount,
    $core.int? expiredAt,
    $core.String? to,
    $core.String? encodedContractData,
  }) {
    final $result = create();
    if (bounce != null) {
      $result.bounce = bounce;
    }
    if (behavior != null) {
      $result.behavior = behavior;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (expiredAt != null) {
      $result.expiredAt = expiredAt;
    }
    if (to != null) {
      $result.to = to;
    }
    if (encodedContractData != null) {
      $result.encodedContractData = encodedContractData;
    }
    return $result;
  }
  Transfer._() : super();
  factory Transfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Transfer_AccountStateOneof> _Transfer_AccountStateOneofByTag = {
    6 : Transfer_AccountStateOneof.encodedContractData,
    0 : Transfer_AccountStateOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Everscale.Proto'), createEmptyInstance: create)
    ..oo(0, [6])
    ..aOB(1, _omitFieldNames ? '' : 'bounce')
    ..e<MessageBehavior>(2, _omitFieldNames ? '' : 'behavior', $pb.PbFieldType.OE, defaultOrMaker: MessageBehavior.SimpleTransfer, valueOf: MessageBehavior.valueOf, enumValues: MessageBehavior.values)
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.int>(4, _omitFieldNames ? '' : 'expiredAt', $pb.PbFieldType.OU3)
    ..aOS(5, _omitFieldNames ? '' : 'to')
    ..aOS(6, _omitFieldNames ? '' : 'encodedContractData')
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

  Transfer_AccountStateOneof whichAccountStateOneof() => _Transfer_AccountStateOneofByTag[$_whichOneof(0)]!;
  void clearAccountStateOneof() => clearField($_whichOneof(0));

  /// If set to true, then the message will be returned if there is an error on the recipient's side.
  @$pb.TagNumber(1)
  $core.bool get bounce => $_getBF(0);
  @$pb.TagNumber(1)
  set bounce($core.bool v) { $_setBool(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasBounce() => $_has(0);
  @$pb.TagNumber(1)
  void clearBounce() => clearField(1);

  /// Affect the attached amount and fees
  @$pb.TagNumber(2)
  MessageBehavior get behavior => $_getN(1);
  @$pb.TagNumber(2)
  set behavior(MessageBehavior v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasBehavior() => $_has(1);
  @$pb.TagNumber(2)
  void clearBehavior() => clearField(2);

  /// Amount to send in nano EVER
  @$pb.TagNumber(3)
  $fixnum.Int64 get amount => $_getI64(2);
  @$pb.TagNumber(3)
  set amount($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);

  /// Expiration UNIX timestamp
  @$pb.TagNumber(4)
  $core.int get expiredAt => $_getIZ(3);
  @$pb.TagNumber(4)
  set expiredAt($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasExpiredAt() => $_has(3);
  @$pb.TagNumber(4)
  void clearExpiredAt() => clearField(4);

  /// Recipient address
  @$pb.TagNumber(5)
  $core.String get to => $_getSZ(4);
  @$pb.TagNumber(5)
  set to($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasTo() => $_has(4);
  @$pb.TagNumber(5)
  void clearTo() => clearField(5);

  /// Just contract data
  @$pb.TagNumber(6)
  $core.String get encodedContractData => $_getSZ(5);
  @$pb.TagNumber(6)
  set encodedContractData($core.String v) { $_setString(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasEncodedContractData() => $_has(5);
  @$pb.TagNumber(6)
  void clearEncodedContractData() => clearField(6);
}

enum SigningInput_ActionOneof {
  transfer, 
  notSet
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    Transfer? transfer,
    $core.List<$core.int>? privateKey,
  }) {
    final $result = create();
    if (transfer != null) {
      $result.transfer = transfer;
    }
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, SigningInput_ActionOneof> _SigningInput_ActionOneofByTag = {
    1 : SigningInput_ActionOneof.transfer,
    0 : SigningInput_ActionOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Everscale.Proto'), createEmptyInstance: create)
    ..oo(0, [1])
    ..aOM<Transfer>(1, _omitFieldNames ? '' : 'transfer', subBuilder: Transfer.create)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
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

  SigningInput_ActionOneof whichActionOneof() => _SigningInput_ActionOneofByTag[$_whichOneof(0)]!;
  void clearActionOneof() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  Transfer get transfer => $_getN(0);
  @$pb.TagNumber(1)
  set transfer(Transfer v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasTransfer() => $_has(0);
  @$pb.TagNumber(1)
  void clearTransfer() => clearField(1);
  @$pb.TagNumber(1)
  Transfer ensureTransfer() => $_ensure(0);

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(2)
  $core.List<$core.int> get privateKey => $_getN(1);
  @$pb.TagNumber(2)
  set privateKey($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPrivateKey() => $_has(1);
  @$pb.TagNumber(2)
  void clearPrivateKey() => clearField(2);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.String? encoded,
  }) {
    final $result = create();
    if (encoded != null) {
      $result.encoded = encoded;
    }
    return $result;
  }
  SigningOutput._() : super();
  factory SigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Everscale.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'encoded')
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

  @$pb.TagNumber(1)
  $core.String get encoded => $_getSZ(0);
  @$pb.TagNumber(1)
  set encoded($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasEncoded() => $_has(0);
  @$pb.TagNumber(1)
  void clearEncoded() => clearField(1);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
