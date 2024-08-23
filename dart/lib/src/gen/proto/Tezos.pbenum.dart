//
//  Generated code. Do not modify.
//  source: Tezos.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

/// Operation types
class Operation_OperationKind extends $pb.ProtobufEnum {
  static const Operation_OperationKind ENDORSEMENT = Operation_OperationKind._(0, _omitEnumNames ? '' : 'ENDORSEMENT');
  static const Operation_OperationKind REVEAL = Operation_OperationKind._(107, _omitEnumNames ? '' : 'REVEAL');
  static const Operation_OperationKind TRANSACTION = Operation_OperationKind._(108, _omitEnumNames ? '' : 'TRANSACTION');
  static const Operation_OperationKind DELEGATION = Operation_OperationKind._(110, _omitEnumNames ? '' : 'DELEGATION');

  static const $core.List<Operation_OperationKind> values = <Operation_OperationKind> [
    ENDORSEMENT,
    REVEAL,
    TRANSACTION,
    DELEGATION,
  ];

  static final $core.Map<$core.int, Operation_OperationKind> _byValue = $pb.ProtobufEnum.initByValue(values);
  static Operation_OperationKind? valueOf($core.int value) => _byValue[value];

  const Operation_OperationKind._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
