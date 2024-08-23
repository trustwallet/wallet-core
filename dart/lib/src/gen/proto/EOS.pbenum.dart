//
//  Generated code. Do not modify.
//  source: EOS.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

class KeyType extends $pb.ProtobufEnum {
  static const KeyType LEGACY = KeyType._(0, _omitEnumNames ? '' : 'LEGACY');
  static const KeyType MODERNK1 = KeyType._(1, _omitEnumNames ? '' : 'MODERNK1');
  static const KeyType MODERNR1 = KeyType._(2, _omitEnumNames ? '' : 'MODERNR1');

  static const $core.List<KeyType> values = <KeyType> [
    LEGACY,
    MODERNK1,
    MODERNR1,
  ];

  static final $core.Map<$core.int, KeyType> _byValue = $pb.ProtobufEnum.initByValue(values);
  static KeyType? valueOf($core.int value) => _byValue[value];

  const KeyType._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
