//
//  Generated code. Do not modify.
//  source: Polkadot.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

/// Destination options for reward
class RewardDestination extends $pb.ProtobufEnum {
  static const RewardDestination STAKED = RewardDestination._(0, _omitEnumNames ? '' : 'STAKED');
  static const RewardDestination STASH = RewardDestination._(1, _omitEnumNames ? '' : 'STASH');
  static const RewardDestination CONTROLLER = RewardDestination._(2, _omitEnumNames ? '' : 'CONTROLLER');

  static const $core.List<RewardDestination> values = <RewardDestination> [
    STAKED,
    STASH,
    CONTROLLER,
  ];

  static final $core.Map<$core.int, RewardDestination> _byValue = $pb.ProtobufEnum.initByValue(values);
  static RewardDestination? valueOf($core.int value) => _byValue[value];

  const RewardDestination._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
