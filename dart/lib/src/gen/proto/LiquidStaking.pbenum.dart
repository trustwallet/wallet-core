//
//  Generated code. Do not modify.
//  source: LiquidStaking.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

/// Enum for supported coins for liquid staking
class Coin extends $pb.ProtobufEnum {
  static const Coin MATIC = Coin._(0, _omitEnumNames ? '' : 'MATIC');
  static const Coin ATOM = Coin._(1, _omitEnumNames ? '' : 'ATOM');
  static const Coin BNB = Coin._(2, _omitEnumNames ? '' : 'BNB');
  static const Coin APT = Coin._(3, _omitEnumNames ? '' : 'APT');
  static const Coin ETH = Coin._(4, _omitEnumNames ? '' : 'ETH');

  static const $core.List<Coin> values = <Coin> [
    MATIC,
    ATOM,
    BNB,
    APT,
    ETH,
  ];

  static final $core.Map<$core.int, Coin> _byValue = $pb.ProtobufEnum.initByValue(values);
  static Coin? valueOf($core.int value) => _byValue[value];

  const Coin._($core.int v, $core.String n) : super(v, n);
}

/// Enum for supported target blockchains for liquid staking
class Blockchain extends $pb.ProtobufEnum {
  static const Blockchain ETHEREUM = Blockchain._(0, _omitEnumNames ? '' : 'ETHEREUM');
  static const Blockchain POLYGON = Blockchain._(1, _omitEnumNames ? '' : 'POLYGON');
  static const Blockchain STRIDE = Blockchain._(2, _omitEnumNames ? '' : 'STRIDE');
  static const Blockchain BNB_BSC = Blockchain._(3, _omitEnumNames ? '' : 'BNB_BSC');
  static const Blockchain APTOS = Blockchain._(4, _omitEnumNames ? '' : 'APTOS');

  static const $core.List<Blockchain> values = <Blockchain> [
    ETHEREUM,
    POLYGON,
    STRIDE,
    BNB_BSC,
    APTOS,
  ];

  static final $core.Map<$core.int, Blockchain> _byValue = $pb.ProtobufEnum.initByValue(values);
  static Blockchain? valueOf($core.int value) => _byValue[value];

  const Blockchain._($core.int v, $core.String n) : super(v, n);
}

/// Enum for supported liquid staking protocols
class Protocol extends $pb.ProtobufEnum {
  static const Protocol Strader = Protocol._(0, _omitEnumNames ? '' : 'Strader');
  static const Protocol Stride = Protocol._(1, _omitEnumNames ? '' : 'Stride');
  static const Protocol Tortuga = Protocol._(2, _omitEnumNames ? '' : 'Tortuga');
  static const Protocol Lido = Protocol._(3, _omitEnumNames ? '' : 'Lido');

  static const $core.List<Protocol> values = <Protocol> [
    Strader,
    Stride,
    Tortuga,
    Lido,
  ];

  static final $core.Map<$core.int, Protocol> _byValue = $pb.ProtobufEnum.initByValue(values);
  static Protocol? valueOf($core.int value) => _byValue[value];

  const Protocol._($core.int v, $core.String n) : super(v, n);
}

/// Enum for status codes to indicate the result of an operation
class StatusCode extends $pb.ProtobufEnum {
  static const StatusCode OK = StatusCode._(0, _omitEnumNames ? '' : 'OK');
  static const StatusCode ERROR_ACTION_NOT_SET = StatusCode._(1, _omitEnumNames ? '' : 'ERROR_ACTION_NOT_SET');
  static const StatusCode ERROR_TARGETED_BLOCKCHAIN_NOT_SUPPORTED_BY_PROTOCOL = StatusCode._(2, _omitEnumNames ? '' : 'ERROR_TARGETED_BLOCKCHAIN_NOT_SUPPORTED_BY_PROTOCOL');
  static const StatusCode ERROR_SMART_CONTRACT_ADDRESS_NOT_SET = StatusCode._(3, _omitEnumNames ? '' : 'ERROR_SMART_CONTRACT_ADDRESS_NOT_SET');
  static const StatusCode ERROR_INPUT_PROTO_DESERIALIZATION = StatusCode._(4, _omitEnumNames ? '' : 'ERROR_INPUT_PROTO_DESERIALIZATION');
  static const StatusCode ERROR_OPERATION_NOT_SUPPORTED_BY_PROTOCOL = StatusCode._(5, _omitEnumNames ? '' : 'ERROR_OPERATION_NOT_SUPPORTED_BY_PROTOCOL');

  static const $core.List<StatusCode> values = <StatusCode> [
    OK,
    ERROR_ACTION_NOT_SET,
    ERROR_TARGETED_BLOCKCHAIN_NOT_SUPPORTED_BY_PROTOCOL,
    ERROR_SMART_CONTRACT_ADDRESS_NOT_SET,
    ERROR_INPUT_PROTO_DESERIALIZATION,
    ERROR_OPERATION_NOT_SUPPORTED_BY_PROTOCOL,
  ];

  static final $core.Map<$core.int, StatusCode> _byValue = $pb.ProtobufEnum.initByValue(values);
  static StatusCode? valueOf($core.int value) => _byValue[value];

  const StatusCode._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
