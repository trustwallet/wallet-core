//
//  Generated code. Do not modify.
//  source: Barz.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

/// FacetCutAction represents the action to be performed for a FacetCut
class FacetCutAction extends $pb.ProtobufEnum {
  static const FacetCutAction ADD = FacetCutAction._(0, _omitEnumNames ? '' : 'ADD');
  static const FacetCutAction REPLACE = FacetCutAction._(1, _omitEnumNames ? '' : 'REPLACE');
  static const FacetCutAction REMOVE = FacetCutAction._(2, _omitEnumNames ? '' : 'REMOVE');

  static const $core.List<FacetCutAction> values = <FacetCutAction> [
    ADD,
    REPLACE,
    REMOVE,
  ];

  static final $core.Map<$core.int, FacetCutAction> _byValue = $pb.ProtobufEnum.initByValue(values);
  static FacetCutAction? valueOf($core.int value) => _byValue[value];

  const FacetCutAction._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
