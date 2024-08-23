//
//  Generated code. Do not modify.
//  source: Everscale.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

/// Message option
class MessageBehavior extends $pb.ProtobufEnum {
  static const MessageBehavior SimpleTransfer = MessageBehavior._(0, _omitEnumNames ? '' : 'SimpleTransfer');
  static const MessageBehavior SendAllBalance = MessageBehavior._(1, _omitEnumNames ? '' : 'SendAllBalance');

  static const $core.List<MessageBehavior> values = <MessageBehavior> [
    SimpleTransfer,
    SendAllBalance,
  ];

  static final $core.Map<$core.int, MessageBehavior> _byValue = $pb.ProtobufEnum.initByValue(values);
  static MessageBehavior? valueOf($core.int value) => _byValue[value];

  const MessageBehavior._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
