//
//  Generated code. Do not modify.
//  source: InternetComputer.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use transactionDescriptor instead')
const Transaction$json = {
  '1': 'Transaction',
  '2': [
    {'1': 'transfer', '3': 1, '4': 1, '5': 11, '6': '.TW.InternetComputer.Proto.Transaction.Transfer', '9': 0, '10': 'transfer'},
  ],
  '3': [Transaction_Transfer$json],
  '8': [
    {'1': 'transaction_oneof'},
  ],
};

@$core.Deprecated('Use transactionDescriptor instead')
const Transaction_Transfer$json = {
  '1': 'Transfer',
  '2': [
    {'1': 'to_account_identifier', '3': 1, '4': 1, '5': 9, '10': 'toAccountIdentifier'},
    {'1': 'amount', '3': 2, '4': 1, '5': 4, '10': 'amount'},
    {'1': 'memo', '3': 3, '4': 1, '5': 4, '10': 'memo'},
    {'1': 'current_timestamp_nanos', '3': 4, '4': 1, '5': 4, '10': 'currentTimestampNanos'},
    {'1': 'permitted_drift', '3': 5, '4': 1, '5': 4, '10': 'permittedDrift'},
  ],
};

/// Descriptor for `Transaction`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionDescriptor = $convert.base64Decode(
    'CgtUcmFuc2FjdGlvbhJNCgh0cmFuc2ZlchgBIAEoCzIvLlRXLkludGVybmV0Q29tcHV0ZXIuUH'
    'JvdG8uVHJhbnNhY3Rpb24uVHJhbnNmZXJIAFIIdHJhbnNmZXIaywEKCFRyYW5zZmVyEjIKFXRv'
    'X2FjY291bnRfaWRlbnRpZmllchgBIAEoCVITdG9BY2NvdW50SWRlbnRpZmllchIWCgZhbW91bn'
    'QYAiABKARSBmFtb3VudBISCgRtZW1vGAMgASgEUgRtZW1vEjYKF2N1cnJlbnRfdGltZXN0YW1w'
    'X25hbm9zGAQgASgEUhVjdXJyZW50VGltZXN0YW1wTmFub3MSJwoPcGVybWl0dGVkX2RyaWZ0GA'
    'UgASgEUg5wZXJtaXR0ZWREcmlmdEITChF0cmFuc2FjdGlvbl9vbmVvZg==');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'private_key', '3': 1, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'transaction', '3': 2, '4': 1, '5': 11, '6': '.TW.InternetComputer.Proto.Transaction', '10': 'transaction'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSHwoLcHJpdmF0ZV9rZXkYASABKAxSCnByaXZhdGVLZXkSSAoLdHJhbn'
    'NhY3Rpb24YAiABKAsyJi5UVy5JbnRlcm5ldENvbXB1dGVyLlByb3RvLlRyYW5zYWN0aW9uUgt0'
    'cmFuc2FjdGlvbg==');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'signed_transaction', '3': 1, '4': 1, '5': 12, '10': 'signedTransaction'},
    {'1': 'error', '3': 2, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 3, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0Ei0KEnNpZ25lZF90cmFuc2FjdGlvbhgBIAEoDFIRc2lnbmVkVHJhbn'
    'NhY3Rpb24SMwoFZXJyb3IYAiABKA4yHS5UVy5Db21tb24uUHJvdG8uU2lnbmluZ0Vycm9yUgVl'
    'cnJvchIjCg1lcnJvcl9tZXNzYWdlGAMgASgJUgxlcnJvck1lc3NhZ2U=');

