//
//  Generated code. Do not modify.
//  source: Filecoin.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

/// Defines the type of `from` address derivation.
class DerivationType extends $pb.ProtobufEnum {
  static const DerivationType SECP256K1 = DerivationType._(0, _omitEnumNames ? '' : 'SECP256K1');
  static const DerivationType DELEGATED = DerivationType._(1, _omitEnumNames ? '' : 'DELEGATED');

  static const $core.List<DerivationType> values = <DerivationType> [
    SECP256K1,
    DELEGATED,
  ];

  static final $core.Map<$core.int, DerivationType> _byValue = $pb.ProtobufEnum.initByValue(values);
  static DerivationType? valueOf($core.int value) => _byValue[value];

  const DerivationType._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
