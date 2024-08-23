//
//  Generated code. Do not modify.
//  source: Common.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use signingErrorDescriptor instead')
const SigningError$json = {
  '1': 'SigningError',
  '2': [
    {'1': 'OK', '2': 0},
    {'1': 'Error_general', '2': 1},
    {'1': 'Error_internal', '2': 2},
    {'1': 'Error_low_balance', '2': 3},
    {'1': 'Error_zero_amount_requested', '2': 4},
    {'1': 'Error_missing_private_key', '2': 5},
    {'1': 'Error_invalid_private_key', '2': 15},
    {'1': 'Error_invalid_address', '2': 16},
    {'1': 'Error_invalid_utxo', '2': 17},
    {'1': 'Error_invalid_utxo_amount', '2': 18},
    {'1': 'Error_wrong_fee', '2': 6},
    {'1': 'Error_signing', '2': 7},
    {'1': 'Error_tx_too_big', '2': 8},
    {'1': 'Error_missing_input_utxos', '2': 9},
    {'1': 'Error_not_enough_utxos', '2': 10},
    {'1': 'Error_script_redeem', '2': 11},
    {'1': 'Error_script_output', '2': 12},
    {'1': 'Error_script_witness_program', '2': 13},
    {'1': 'Error_invalid_memo', '2': 14},
    {'1': 'Error_input_parse', '2': 19},
    {'1': 'Error_no_support_n2n', '2': 20},
    {'1': 'Error_signatures_count', '2': 21},
    {'1': 'Error_invalid_params', '2': 22},
    {'1': 'Error_invalid_requested_token_amount', '2': 23},
    {'1': 'Error_not_supported', '2': 24},
    {'1': 'Error_dust_amount_requested', '2': 25},
  ],
};

/// Descriptor for `SigningError`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List signingErrorDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nRXJyb3ISBgoCT0sQABIRCg1FcnJvcl9nZW5lcmFsEAESEgoORXJyb3JfaW50ZX'
    'JuYWwQAhIVChFFcnJvcl9sb3dfYmFsYW5jZRADEh8KG0Vycm9yX3plcm9fYW1vdW50X3JlcXVl'
    'c3RlZBAEEh0KGUVycm9yX21pc3NpbmdfcHJpdmF0ZV9rZXkQBRIdChlFcnJvcl9pbnZhbGlkX3'
    'ByaXZhdGVfa2V5EA8SGQoVRXJyb3JfaW52YWxpZF9hZGRyZXNzEBASFgoSRXJyb3JfaW52YWxp'
    'ZF91dHhvEBESHQoZRXJyb3JfaW52YWxpZF91dHhvX2Ftb3VudBASEhMKD0Vycm9yX3dyb25nX2'
    'ZlZRAGEhEKDUVycm9yX3NpZ25pbmcQBxIUChBFcnJvcl90eF90b29fYmlnEAgSHQoZRXJyb3Jf'
    'bWlzc2luZ19pbnB1dF91dHhvcxAJEhoKFkVycm9yX25vdF9lbm91Z2hfdXR4b3MQChIXChNFcn'
    'Jvcl9zY3JpcHRfcmVkZWVtEAsSFwoTRXJyb3Jfc2NyaXB0X291dHB1dBAMEiAKHEVycm9yX3Nj'
    'cmlwdF93aXRuZXNzX3Byb2dyYW0QDRIWChJFcnJvcl9pbnZhbGlkX21lbW8QDhIVChFFcnJvcl'
    '9pbnB1dF9wYXJzZRATEhgKFEVycm9yX25vX3N1cHBvcnRfbjJuEBQSGgoWRXJyb3Jfc2lnbmF0'
    'dXJlc19jb3VudBAVEhgKFEVycm9yX2ludmFsaWRfcGFyYW1zEBYSKAokRXJyb3JfaW52YWxpZF'
    '9yZXF1ZXN0ZWRfdG9rZW5fYW1vdW50EBcSFwoTRXJyb3Jfbm90X3N1cHBvcnRlZBAYEh8KG0Vy'
    'cm9yX2R1c3RfYW1vdW50X3JlcXVlc3RlZBAZ');

