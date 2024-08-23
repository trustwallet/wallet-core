//
//  Generated code. Do not modify.
//  source: Nano.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

import 'Common.pbenum.dart' as $0;

enum SigningInput_LinkOneof {
  linkBlock, 
  linkRecipient, 
  notSet
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.List<$core.int>? privateKey,
    $core.List<$core.int>? parentBlock,
    $core.List<$core.int>? linkBlock,
    $core.String? linkRecipient,
    $core.String? representative,
    $core.String? balance,
    $core.String? work,
    $core.List<$core.int>? publicKey,
  }) {
    final $result = create();
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (parentBlock != null) {
      $result.parentBlock = parentBlock;
    }
    if (linkBlock != null) {
      $result.linkBlock = linkBlock;
    }
    if (linkRecipient != null) {
      $result.linkRecipient = linkRecipient;
    }
    if (representative != null) {
      $result.representative = representative;
    }
    if (balance != null) {
      $result.balance = balance;
    }
    if (work != null) {
      $result.work = work;
    }
    if (publicKey != null) {
      $result.publicKey = publicKey;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, SigningInput_LinkOneof> _SigningInput_LinkOneofByTag = {
    3 : SigningInput_LinkOneof.linkBlock,
    4 : SigningInput_LinkOneof.linkRecipient,
    0 : SigningInput_LinkOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nano.Proto'), createEmptyInstance: create)
    ..oo(0, [3, 4])
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'parentBlock', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'linkBlock', $pb.PbFieldType.OY)
    ..aOS(4, _omitFieldNames ? '' : 'linkRecipient')
    ..aOS(5, _omitFieldNames ? '' : 'representative')
    ..aOS(6, _omitFieldNames ? '' : 'balance')
    ..aOS(7, _omitFieldNames ? '' : 'work')
    ..a<$core.List<$core.int>>(8, _omitFieldNames ? '' : 'publicKey', $pb.PbFieldType.OY)
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

  SigningInput_LinkOneof whichLinkOneof() => _SigningInput_LinkOneofByTag[$_whichOneof(0)]!;
  void clearLinkOneof() => clearField($_whichOneof(0));

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(1)
  $core.List<$core.int> get privateKey => $_getN(0);
  @$pb.TagNumber(1)
  set privateKey($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPrivateKey() => $_has(0);
  @$pb.TagNumber(1)
  void clearPrivateKey() => clearField(1);

  /// Optional parent block hash
  @$pb.TagNumber(2)
  $core.List<$core.int> get parentBlock => $_getN(1);
  @$pb.TagNumber(2)
  set parentBlock($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasParentBlock() => $_has(1);
  @$pb.TagNumber(2)
  void clearParentBlock() => clearField(2);

  /// Hash of a block to receive from
  @$pb.TagNumber(3)
  $core.List<$core.int> get linkBlock => $_getN(2);
  @$pb.TagNumber(3)
  set linkBlock($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasLinkBlock() => $_has(2);
  @$pb.TagNumber(3)
  void clearLinkBlock() => clearField(3);

  /// Recipient address to send coins to
  @$pb.TagNumber(4)
  $core.String get linkRecipient => $_getSZ(3);
  @$pb.TagNumber(4)
  set linkRecipient($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasLinkRecipient() => $_has(3);
  @$pb.TagNumber(4)
  void clearLinkRecipient() => clearField(4);

  /// Representative address
  @$pb.TagNumber(5)
  $core.String get representative => $_getSZ(4);
  @$pb.TagNumber(5)
  set representative($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasRepresentative() => $_has(4);
  @$pb.TagNumber(5)
  void clearRepresentative() => clearField(5);

  /// Account balance (128-bit unsigned integer, as a string)
  @$pb.TagNumber(6)
  $core.String get balance => $_getSZ(5);
  @$pb.TagNumber(6)
  set balance($core.String v) { $_setString(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasBalance() => $_has(5);
  @$pb.TagNumber(6)
  void clearBalance() => clearField(6);

  /// Work
  @$pb.TagNumber(7)
  $core.String get work => $_getSZ(6);
  @$pb.TagNumber(7)
  set work($core.String v) { $_setString(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasWork() => $_has(6);
  @$pb.TagNumber(7)
  void clearWork() => clearField(7);

  /// Pulic key used for building preImage (32 bytes).
  @$pb.TagNumber(8)
  $core.List<$core.int> get publicKey => $_getN(7);
  @$pb.TagNumber(8)
  set publicKey($core.List<$core.int> v) { $_setBytes(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasPublicKey() => $_has(7);
  @$pb.TagNumber(8)
  void clearPublicKey() => clearField(8);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.List<$core.int>? signature,
    $core.List<$core.int>? blockHash,
    $core.String? json,
    $0.SigningError? error,
    $core.String? errorMessage,
  }) {
    final $result = create();
    if (signature != null) {
      $result.signature = signature;
    }
    if (blockHash != null) {
      $result.blockHash = blockHash;
    }
    if (json != null) {
      $result.json = json;
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

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Nano.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'signature', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'blockHash', $pb.PbFieldType.OY)
    ..aOS(3, _omitFieldNames ? '' : 'json')
    ..e<$0.SigningError>(4, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(5, _omitFieldNames ? '' : 'errorMessage')
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

  /// Block hash
  @$pb.TagNumber(2)
  $core.List<$core.int> get blockHash => $_getN(1);
  @$pb.TagNumber(2)
  set blockHash($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasBlockHash() => $_has(1);
  @$pb.TagNumber(2)
  void clearBlockHash() => clearField(2);

  /// JSON representation of the block
  @$pb.TagNumber(3)
  $core.String get json => $_getSZ(2);
  @$pb.TagNumber(3)
  set json($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasJson() => $_has(2);
  @$pb.TagNumber(3)
  void clearJson() => clearField(3);

  /// error code, 0 is ok, other codes will be treated as errors
  @$pb.TagNumber(4)
  $0.SigningError get error => $_getN(3);
  @$pb.TagNumber(4)
  set error($0.SigningError v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasError() => $_has(3);
  @$pb.TagNumber(4)
  void clearError() => clearField(4);

  /// error code description
  @$pb.TagNumber(5)
  $core.String get errorMessage => $_getSZ(4);
  @$pb.TagNumber(5)
  set errorMessage($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasErrorMessage() => $_has(4);
  @$pb.TagNumber(5)
  void clearErrorMessage() => clearField(5);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
