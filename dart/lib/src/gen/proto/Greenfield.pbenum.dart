//
//  Generated code. Do not modify.
//  source: Greenfield.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

/// Transaction broadcast mode
class BroadcastMode extends $pb.ProtobufEnum {
  static const BroadcastMode SYNC = BroadcastMode._(0, _omitEnumNames ? '' : 'SYNC');
  static const BroadcastMode ASYNC = BroadcastMode._(1, _omitEnumNames ? '' : 'ASYNC');

  static const $core.List<BroadcastMode> values = <BroadcastMode> [
    SYNC,
    ASYNC,
  ];

  static final $core.Map<$core.int, BroadcastMode> _byValue = $pb.ProtobufEnum.initByValue(values);
  static BroadcastMode? valueOf($core.int value) => _byValue[value];

  const BroadcastMode._($core.int v, $core.String n) : super(v, n);
}

/// Options for transaction encoding.
/// Consider adding Json mode.
class EncodingMode extends $pb.ProtobufEnum {
  static const EncodingMode Protobuf = EncodingMode._(0, _omitEnumNames ? '' : 'Protobuf');

  static const $core.List<EncodingMode> values = <EncodingMode> [
    Protobuf,
  ];

  static final $core.Map<$core.int, EncodingMode> _byValue = $pb.ProtobufEnum.initByValue(values);
  static EncodingMode? valueOf($core.int value) => _byValue[value];

  const EncodingMode._($core.int v, $core.String n) : super(v, n);
}

/// Options for transaction signing.
/// Consider adding Direct mode when it is supported.
class SigningMode extends $pb.ProtobufEnum {
  static const SigningMode Eip712 = SigningMode._(0, _omitEnumNames ? '' : 'Eip712');

  static const $core.List<SigningMode> values = <SigningMode> [
    Eip712,
  ];

  static final $core.Map<$core.int, SigningMode> _byValue = $pb.ProtobufEnum.initByValue(values);
  static SigningMode? valueOf($core.int value) => _byValue[value];

  const SigningMode._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
