//
//  Generated code. Do not modify.
//  source: Common.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

/// Error codes, used in multiple blockchains.
class SigningError extends $pb.ProtobufEnum {
  static const SigningError OK = SigningError._(0, _omitEnumNames ? '' : 'OK');
  static const SigningError Error_general = SigningError._(1, _omitEnumNames ? '' : 'Error_general');
  static const SigningError Error_internal = SigningError._(2, _omitEnumNames ? '' : 'Error_internal');
  static const SigningError Error_low_balance = SigningError._(3, _omitEnumNames ? '' : 'Error_low_balance');
  static const SigningError Error_zero_amount_requested = SigningError._(4, _omitEnumNames ? '' : 'Error_zero_amount_requested');
  static const SigningError Error_missing_private_key = SigningError._(5, _omitEnumNames ? '' : 'Error_missing_private_key');
  static const SigningError Error_invalid_private_key = SigningError._(15, _omitEnumNames ? '' : 'Error_invalid_private_key');
  static const SigningError Error_invalid_address = SigningError._(16, _omitEnumNames ? '' : 'Error_invalid_address');
  static const SigningError Error_invalid_utxo = SigningError._(17, _omitEnumNames ? '' : 'Error_invalid_utxo');
  static const SigningError Error_invalid_utxo_amount = SigningError._(18, _omitEnumNames ? '' : 'Error_invalid_utxo_amount');
  static const SigningError Error_wrong_fee = SigningError._(6, _omitEnumNames ? '' : 'Error_wrong_fee');
  static const SigningError Error_signing = SigningError._(7, _omitEnumNames ? '' : 'Error_signing');
  static const SigningError Error_tx_too_big = SigningError._(8, _omitEnumNames ? '' : 'Error_tx_too_big');
  static const SigningError Error_missing_input_utxos = SigningError._(9, _omitEnumNames ? '' : 'Error_missing_input_utxos');
  static const SigningError Error_not_enough_utxos = SigningError._(10, _omitEnumNames ? '' : 'Error_not_enough_utxos');
  static const SigningError Error_script_redeem = SigningError._(11, _omitEnumNames ? '' : 'Error_script_redeem');
  static const SigningError Error_script_output = SigningError._(12, _omitEnumNames ? '' : 'Error_script_output');
  static const SigningError Error_script_witness_program = SigningError._(13, _omitEnumNames ? '' : 'Error_script_witness_program');
  static const SigningError Error_invalid_memo = SigningError._(14, _omitEnumNames ? '' : 'Error_invalid_memo');
  static const SigningError Error_input_parse = SigningError._(19, _omitEnumNames ? '' : 'Error_input_parse');
  static const SigningError Error_no_support_n2n = SigningError._(20, _omitEnumNames ? '' : 'Error_no_support_n2n');
  static const SigningError Error_signatures_count = SigningError._(21, _omitEnumNames ? '' : 'Error_signatures_count');
  static const SigningError Error_invalid_params = SigningError._(22, _omitEnumNames ? '' : 'Error_invalid_params');
  static const SigningError Error_invalid_requested_token_amount = SigningError._(23, _omitEnumNames ? '' : 'Error_invalid_requested_token_amount');
  static const SigningError Error_not_supported = SigningError._(24, _omitEnumNames ? '' : 'Error_not_supported');
  static const SigningError Error_dust_amount_requested = SigningError._(25, _omitEnumNames ? '' : 'Error_dust_amount_requested');

  static const $core.List<SigningError> values = <SigningError> [
    OK,
    Error_general,
    Error_internal,
    Error_low_balance,
    Error_zero_amount_requested,
    Error_missing_private_key,
    Error_invalid_private_key,
    Error_invalid_address,
    Error_invalid_utxo,
    Error_invalid_utxo_amount,
    Error_wrong_fee,
    Error_signing,
    Error_tx_too_big,
    Error_missing_input_utxos,
    Error_not_enough_utxos,
    Error_script_redeem,
    Error_script_output,
    Error_script_witness_program,
    Error_invalid_memo,
    Error_input_parse,
    Error_no_support_n2n,
    Error_signatures_count,
    Error_invalid_params,
    Error_invalid_requested_token_amount,
    Error_not_supported,
    Error_dust_amount_requested,
  ];

  static final $core.Map<$core.int, SigningError> _byValue = $pb.ProtobufEnum.initByValue(values);
  static SigningError? valueOf($core.int value) => _byValue[value];

  const SigningError._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
