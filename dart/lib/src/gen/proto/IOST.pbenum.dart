//
//  Generated code. Do not modify.
//  source: IOST.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

/// The enumeration defines the signature algorithm.
class Algorithm extends $pb.ProtobufEnum {
  static const Algorithm UNKNOWN = Algorithm._(0, _omitEnumNames ? '' : 'UNKNOWN');
  static const Algorithm SECP256K1 = Algorithm._(1, _omitEnumNames ? '' : 'SECP256K1');
  static const Algorithm ED25519 = Algorithm._(2, _omitEnumNames ? '' : 'ED25519');

  static const $core.List<Algorithm> values = <Algorithm> [
    UNKNOWN,
    SECP256K1,
    ED25519,
  ];

  static final $core.Map<$core.int, Algorithm> _byValue = $pb.ProtobufEnum.initByValue(values);
  static Algorithm? valueOf($core.int value) => _byValue[value];

  const Algorithm._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
