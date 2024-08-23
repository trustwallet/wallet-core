//
//  Generated code. Do not modify.
//  source: Ontology.proto
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

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.String? contract,
    $core.String? method,
    $core.List<$core.int>? ownerPrivateKey,
    $core.String? toAddress,
    $fixnum.Int64? amount,
    $core.List<$core.int>? payerPrivateKey,
    $fixnum.Int64? gasPrice,
    $fixnum.Int64? gasLimit,
    $core.String? queryAddress,
    $core.int? nonce,
    $core.String? ownerAddress,
    $core.String? payerAddress,
  }) {
    final $result = create();
    if (contract != null) {
      $result.contract = contract;
    }
    if (method != null) {
      $result.method = method;
    }
    if (ownerPrivateKey != null) {
      $result.ownerPrivateKey = ownerPrivateKey;
    }
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (payerPrivateKey != null) {
      $result.payerPrivateKey = payerPrivateKey;
    }
    if (gasPrice != null) {
      $result.gasPrice = gasPrice;
    }
    if (gasLimit != null) {
      $result.gasLimit = gasLimit;
    }
    if (queryAddress != null) {
      $result.queryAddress = queryAddress;
    }
    if (nonce != null) {
      $result.nonce = nonce;
    }
    if (ownerAddress != null) {
      $result.ownerAddress = ownerAddress;
    }
    if (payerAddress != null) {
      $result.payerAddress = payerAddress;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ontology.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'contract')
    ..aOS(2, _omitFieldNames ? '' : 'method')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'ownerPrivateKey', $pb.PbFieldType.OY)
    ..aOS(4, _omitFieldNames ? '' : 'toAddress')
    ..a<$fixnum.Int64>(5, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.List<$core.int>>(6, _omitFieldNames ? '' : 'payerPrivateKey', $pb.PbFieldType.OY)
    ..a<$fixnum.Int64>(7, _omitFieldNames ? '' : 'gasPrice', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(8, _omitFieldNames ? '' : 'gasLimit', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(9, _omitFieldNames ? '' : 'queryAddress')
    ..a<$core.int>(10, _omitFieldNames ? '' : 'nonce', $pb.PbFieldType.OU3)
    ..aOS(11, _omitFieldNames ? '' : 'ownerAddress')
    ..aOS(12, _omitFieldNames ? '' : 'payerAddress')
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

  /// Contract ID, e.g. "ONT"
  @$pb.TagNumber(1)
  $core.String get contract => $_getSZ(0);
  @$pb.TagNumber(1)
  set contract($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasContract() => $_has(0);
  @$pb.TagNumber(1)
  void clearContract() => clearField(1);

  /// Method, e.g. "transfer"
  @$pb.TagNumber(2)
  $core.String get method => $_getSZ(1);
  @$pb.TagNumber(2)
  set method($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasMethod() => $_has(1);
  @$pb.TagNumber(2)
  void clearMethod() => clearField(2);

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(3)
  $core.List<$core.int> get ownerPrivateKey => $_getN(2);
  @$pb.TagNumber(3)
  set ownerPrivateKey($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasOwnerPrivateKey() => $_has(2);
  @$pb.TagNumber(3)
  void clearOwnerPrivateKey() => clearField(3);

  /// base58 encode address string (160-bit number)
  @$pb.TagNumber(4)
  $core.String get toAddress => $_getSZ(3);
  @$pb.TagNumber(4)
  set toAddress($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasToAddress() => $_has(3);
  @$pb.TagNumber(4)
  void clearToAddress() => clearField(4);

  /// Transfer amount
  @$pb.TagNumber(5)
  $fixnum.Int64 get amount => $_getI64(4);
  @$pb.TagNumber(5)
  set amount($fixnum.Int64 v) { $_setInt64(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasAmount() => $_has(4);
  @$pb.TagNumber(5)
  void clearAmount() => clearField(5);

  /// Private key of the payer
  @$pb.TagNumber(6)
  $core.List<$core.int> get payerPrivateKey => $_getN(5);
  @$pb.TagNumber(6)
  set payerPrivateKey($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasPayerPrivateKey() => $_has(5);
  @$pb.TagNumber(6)
  void clearPayerPrivateKey() => clearField(6);

  /// Gas price
  @$pb.TagNumber(7)
  $fixnum.Int64 get gasPrice => $_getI64(6);
  @$pb.TagNumber(7)
  set gasPrice($fixnum.Int64 v) { $_setInt64(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasGasPrice() => $_has(6);
  @$pb.TagNumber(7)
  void clearGasPrice() => clearField(7);

  /// Limit for gas used
  @$pb.TagNumber(8)
  $fixnum.Int64 get gasLimit => $_getI64(7);
  @$pb.TagNumber(8)
  set gasLimit($fixnum.Int64 v) { $_setInt64(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasGasLimit() => $_has(7);
  @$pb.TagNumber(8)
  void clearGasLimit() => clearField(8);

  /// base58 encode address string (160-bit number)
  @$pb.TagNumber(9)
  $core.String get queryAddress => $_getSZ(8);
  @$pb.TagNumber(9)
  set queryAddress($core.String v) { $_setString(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasQueryAddress() => $_has(8);
  @$pb.TagNumber(9)
  void clearQueryAddress() => clearField(9);

  /// Nonce (should be larger than in the last transaction of the account)
  @$pb.TagNumber(10)
  $core.int get nonce => $_getIZ(9);
  @$pb.TagNumber(10)
  set nonce($core.int v) { $_setUnsignedInt32(9, v); }
  @$pb.TagNumber(10)
  $core.bool hasNonce() => $_has(9);
  @$pb.TagNumber(10)
  void clearNonce() => clearField(10);

  /// base58 encode address string (160-bit number)
  @$pb.TagNumber(11)
  $core.String get ownerAddress => $_getSZ(10);
  @$pb.TagNumber(11)
  set ownerAddress($core.String v) { $_setString(10, v); }
  @$pb.TagNumber(11)
  $core.bool hasOwnerAddress() => $_has(10);
  @$pb.TagNumber(11)
  void clearOwnerAddress() => clearField(11);

  /// base58 encode address string (160-bit number)
  @$pb.TagNumber(12)
  $core.String get payerAddress => $_getSZ(11);
  @$pb.TagNumber(12)
  set payerAddress($core.String v) { $_setString(11, v); }
  @$pb.TagNumber(12)
  $core.bool hasPayerAddress() => $_has(11);
  @$pb.TagNumber(12)
  void clearPayerAddress() => clearField(12);
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

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Ontology.Proto'), createEmptyInstance: create)
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
