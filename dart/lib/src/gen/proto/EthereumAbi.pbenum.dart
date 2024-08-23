//
//  Generated code. Do not modify.
//  source: EthereumAbi.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

class AbiError extends $pb.ProtobufEnum {
  static const AbiError OK = AbiError._(0, _omitEnumNames ? '' : 'OK');
  static const AbiError Error_internal = AbiError._(1, _omitEnumNames ? '' : 'Error_internal');
  static const AbiError Error_abi_mismatch = AbiError._(2, _omitEnumNames ? '' : 'Error_abi_mismatch');
  static const AbiError Error_invalid_abi = AbiError._(3, _omitEnumNames ? '' : 'Error_invalid_abi');
  static const AbiError Error_invalid_param_type = AbiError._(4, _omitEnumNames ? '' : 'Error_invalid_param_type');
  static const AbiError Error_invalid_address_value = AbiError._(5, _omitEnumNames ? '' : 'Error_invalid_address_value');
  static const AbiError Error_invalid_uint_value = AbiError._(6, _omitEnumNames ? '' : 'Error_invalid_uint_value');
  static const AbiError Error_missing_param_type = AbiError._(7, _omitEnumNames ? '' : 'Error_missing_param_type');
  static const AbiError Error_missing_param_value = AbiError._(8, _omitEnumNames ? '' : 'Error_missing_param_value');
  static const AbiError Error_decoding_data = AbiError._(9, _omitEnumNames ? '' : 'Error_decoding_data');
  static const AbiError Error_empty_type = AbiError._(10, _omitEnumNames ? '' : 'Error_empty_type');

  static const $core.List<AbiError> values = <AbiError> [
    OK,
    Error_internal,
    Error_abi_mismatch,
    Error_invalid_abi,
    Error_invalid_param_type,
    Error_invalid_address_value,
    Error_invalid_uint_value,
    Error_missing_param_type,
    Error_missing_param_value,
    Error_decoding_data,
    Error_empty_type,
  ];

  static final $core.Map<$core.int, AbiError> _byValue = $pb.ProtobufEnum.initByValue(values);
  static AbiError? valueOf($core.int value) => _byValue[value];

  const AbiError._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
