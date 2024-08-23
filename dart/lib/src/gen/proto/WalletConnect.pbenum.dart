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

/// The transaction protocol may differ from version to version.
class Protocol extends $pb.ProtobufEnum {
  static const Protocol V2 = Protocol._(0, _omitEnumNames ? '' : 'V2');

  static const $core.List<Protocol> values = <Protocol> [
    V2,
  ];

  static final $core.Map<$core.int, Protocol> _byValue = $pb.ProtobufEnum.initByValue(values);
  static Protocol? valueOf($core.int value) => _byValue[value];

  const Protocol._($core.int v, $core.String n) : super(v, n);
}

/// WalletConnect request method.
class Method extends $pb.ProtobufEnum {
  static const Method Unknown = Method._(0, _omitEnumNames ? '' : 'Unknown');
  static const Method CosmosSignAmino = Method._(1, _omitEnumNames ? '' : 'CosmosSignAmino');
  static const Method SolanaSignTransaction = Method._(2, _omitEnumNames ? '' : 'SolanaSignTransaction');

  static const $core.List<Method> values = <Method> [
    Unknown,
    CosmosSignAmino,
    SolanaSignTransaction,
  ];

  static final $core.Map<$core.int, Method> _byValue = $pb.ProtobufEnum.initByValue(values);
  static Method? valueOf($core.int value) => _byValue[value];

  const Method._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
