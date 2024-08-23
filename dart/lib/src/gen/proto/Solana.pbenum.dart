//
//  Generated code. Do not modify.
//  source: Solana.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

class Encoding extends $pb.ProtobufEnum {
  static const Encoding Base58 = Encoding._(0, _omitEnumNames ? '' : 'Base58');
  static const Encoding Base64 = Encoding._(1, _omitEnumNames ? '' : 'Base64');

  static const $core.List<Encoding> values = <Encoding> [
    Base58,
    Base64,
  ];

  static final $core.Map<$core.int, Encoding> _byValue = $pb.ProtobufEnum.initByValue(values);
  static Encoding? valueOf($core.int value) => _byValue[value];

  const Encoding._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
