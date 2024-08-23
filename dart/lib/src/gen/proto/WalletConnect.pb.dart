//
//  Generated code. Do not modify.
//  source: WalletConnect.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

import 'Binance.pb.dart' as $6;
import 'Common.pbenum.dart' as $0;
import 'Solana.pb.dart' as $7;
import 'WalletConnect.pbenum.dart';

export 'WalletConnect.pbenum.dart';

class ParseRequestInput extends $pb.GeneratedMessage {
  factory ParseRequestInput({
    Protocol? protocol,
    Method? method,
    $core.String? payload,
  }) {
    final $result = create();
    if (protocol != null) {
      $result.protocol = protocol;
    }
    if (method != null) {
      $result.method = method;
    }
    if (payload != null) {
      $result.payload = payload;
    }
    return $result;
  }
  ParseRequestInput._() : super();
  factory ParseRequestInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ParseRequestInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'ParseRequestInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.WalletConnect.Proto'), createEmptyInstance: create)
    ..e<Protocol>(1, _omitFieldNames ? '' : 'protocol', $pb.PbFieldType.OE, defaultOrMaker: Protocol.V2, valueOf: Protocol.valueOf, enumValues: Protocol.values)
    ..e<Method>(2, _omitFieldNames ? '' : 'method', $pb.PbFieldType.OE, defaultOrMaker: Method.Unknown, valueOf: Method.valueOf, enumValues: Method.values)
    ..aOS(3, _omitFieldNames ? '' : 'payload')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ParseRequestInput clone() => ParseRequestInput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ParseRequestInput copyWith(void Function(ParseRequestInput) updates) => super.copyWith((message) => updates(message as ParseRequestInput)) as ParseRequestInput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static ParseRequestInput create() => ParseRequestInput._();
  ParseRequestInput createEmptyInstance() => create();
  static $pb.PbList<ParseRequestInput> createRepeated() => $pb.PbList<ParseRequestInput>();
  @$core.pragma('dart2js:noInline')
  static ParseRequestInput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ParseRequestInput>(create);
  static ParseRequestInput? _defaultInstance;

  /// A protocol version.
  @$pb.TagNumber(1)
  Protocol get protocol => $_getN(0);
  @$pb.TagNumber(1)
  set protocol(Protocol v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasProtocol() => $_has(0);
  @$pb.TagNumber(1)
  void clearProtocol() => clearField(1);

  /// A signing method like "cosmos_signAmino" or "eth_signTransaction".
  @$pb.TagNumber(2)
  Method get method => $_getN(1);
  @$pb.TagNumber(2)
  set method(Method v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasMethod() => $_has(1);
  @$pb.TagNumber(2)
  void clearMethod() => clearField(2);

  /// Transaction payload to sign.
  /// Basically, a JSON object.
  @$pb.TagNumber(3)
  $core.String get payload => $_getSZ(2);
  @$pb.TagNumber(3)
  set payload($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasPayload() => $_has(2);
  @$pb.TagNumber(3)
  void clearPayload() => clearField(3);
}

enum ParseRequestOutput_SigningInputOneof {
  binance, 
  solana, 
  notSet
}

class ParseRequestOutput extends $pb.GeneratedMessage {
  factory ParseRequestOutput({
    $0.SigningError? error,
    $core.String? errorMessage,
    $6.SigningInput? binance,
    $7.SigningInput? solana,
  }) {
    final $result = create();
    if (error != null) {
      $result.error = error;
    }
    if (errorMessage != null) {
      $result.errorMessage = errorMessage;
    }
    if (binance != null) {
      $result.binance = binance;
    }
    if (solana != null) {
      $result.solana = solana;
    }
    return $result;
  }
  ParseRequestOutput._() : super();
  factory ParseRequestOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ParseRequestOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, ParseRequestOutput_SigningInputOneof> _ParseRequestOutput_SigningInputOneofByTag = {
    3 : ParseRequestOutput_SigningInputOneof.binance,
    4 : ParseRequestOutput_SigningInputOneof.solana,
    0 : ParseRequestOutput_SigningInputOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'ParseRequestOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.WalletConnect.Proto'), createEmptyInstance: create)
    ..oo(0, [3, 4])
    ..e<$0.SigningError>(1, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(2, _omitFieldNames ? '' : 'errorMessage')
    ..aOM<$6.SigningInput>(3, _omitFieldNames ? '' : 'binance', subBuilder: $6.SigningInput.create)
    ..aOM<$7.SigningInput>(4, _omitFieldNames ? '' : 'solana', subBuilder: $7.SigningInput.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ParseRequestOutput clone() => ParseRequestOutput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ParseRequestOutput copyWith(void Function(ParseRequestOutput) updates) => super.copyWith((message) => updates(message as ParseRequestOutput)) as ParseRequestOutput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static ParseRequestOutput create() => ParseRequestOutput._();
  ParseRequestOutput createEmptyInstance() => create();
  static $pb.PbList<ParseRequestOutput> createRepeated() => $pb.PbList<ParseRequestOutput>();
  @$core.pragma('dart2js:noInline')
  static ParseRequestOutput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ParseRequestOutput>(create);
  static ParseRequestOutput? _defaultInstance;

  ParseRequestOutput_SigningInputOneof whichSigningInputOneof() => _ParseRequestOutput_SigningInputOneofByTag[$_whichOneof(0)]!;
  void clearSigningInputOneof() => clearField($_whichOneof(0));

  /// OK (=0) or other codes in case of error
  @$pb.TagNumber(1)
  $0.SigningError get error => $_getN(0);
  @$pb.TagNumber(1)
  set error($0.SigningError v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasError() => $_has(0);
  @$pb.TagNumber(1)
  void clearError() => clearField(1);

  /// error description in case of error
  @$pb.TagNumber(2)
  $core.String get errorMessage => $_getSZ(1);
  @$pb.TagNumber(2)
  set errorMessage($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasErrorMessage() => $_has(1);
  @$pb.TagNumber(2)
  void clearErrorMessage() => clearField(2);

  @$pb.TagNumber(3)
  $6.SigningInput get binance => $_getN(2);
  @$pb.TagNumber(3)
  set binance($6.SigningInput v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasBinance() => $_has(2);
  @$pb.TagNumber(3)
  void clearBinance() => clearField(3);
  @$pb.TagNumber(3)
  $6.SigningInput ensureBinance() => $_ensure(2);

  @$pb.TagNumber(4)
  $7.SigningInput get solana => $_getN(3);
  @$pb.TagNumber(4)
  set solana($7.SigningInput v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasSolana() => $_has(3);
  @$pb.TagNumber(4)
  void clearSolana() => clearField(4);
  @$pb.TagNumber(4)
  $7.SigningInput ensureSolana() => $_ensure(3);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
