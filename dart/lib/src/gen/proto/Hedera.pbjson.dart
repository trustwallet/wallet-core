//
//  Generated code. Do not modify.
//  source: Hedera.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use timestampDescriptor instead')
const Timestamp$json = {
  '1': 'Timestamp',
  '2': [
    {'1': 'seconds', '3': 1, '4': 1, '5': 3, '10': 'seconds'},
    {'1': 'nanos', '3': 2, '4': 1, '5': 5, '10': 'nanos'},
  ],
};

/// Descriptor for `Timestamp`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List timestampDescriptor = $convert.base64Decode(
    'CglUaW1lc3RhbXASGAoHc2Vjb25kcxgBIAEoA1IHc2Vjb25kcxIUCgVuYW5vcxgCIAEoBVIFbm'
    'Fub3M=');

@$core.Deprecated('Use transactionIDDescriptor instead')
const TransactionID$json = {
  '1': 'TransactionID',
  '2': [
    {'1': 'transactionValidStart', '3': 1, '4': 1, '5': 11, '6': '.TW.Hedera.Proto.Timestamp', '10': 'transactionValidStart'},
    {'1': 'accountID', '3': 2, '4': 1, '5': 9, '10': 'accountID'},
    {'1': 'scheduled', '3': 3, '4': 1, '5': 8, '10': 'scheduled'},
    {'1': 'nonce', '3': 4, '4': 1, '5': 5, '10': 'nonce'},
  ],
};

/// Descriptor for `TransactionID`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionIDDescriptor = $convert.base64Decode(
    'Cg1UcmFuc2FjdGlvbklEElAKFXRyYW5zYWN0aW9uVmFsaWRTdGFydBgBIAEoCzIaLlRXLkhlZG'
    'VyYS5Qcm90by5UaW1lc3RhbXBSFXRyYW5zYWN0aW9uVmFsaWRTdGFydBIcCglhY2NvdW50SUQY'
    'AiABKAlSCWFjY291bnRJRBIcCglzY2hlZHVsZWQYAyABKAhSCXNjaGVkdWxlZBIUCgVub25jZR'
    'gEIAEoBVIFbm9uY2U=');

@$core.Deprecated('Use transferMessageDescriptor instead')
const TransferMessage$json = {
  '1': 'TransferMessage',
  '2': [
    {'1': 'from', '3': 1, '4': 1, '5': 9, '10': 'from'},
    {'1': 'to', '3': 2, '4': 1, '5': 9, '10': 'to'},
    {'1': 'amount', '3': 3, '4': 1, '5': 18, '10': 'amount'},
  ],
};

/// Descriptor for `TransferMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transferMessageDescriptor = $convert.base64Decode(
    'Cg9UcmFuc2Zlck1lc3NhZ2USEgoEZnJvbRgBIAEoCVIEZnJvbRIOCgJ0bxgCIAEoCVICdG8SFg'
    'oGYW1vdW50GAMgASgSUgZhbW91bnQ=');

@$core.Deprecated('Use transactionBodyDescriptor instead')
const TransactionBody$json = {
  '1': 'TransactionBody',
  '2': [
    {'1': 'transactionID', '3': 1, '4': 1, '5': 11, '6': '.TW.Hedera.Proto.TransactionID', '10': 'transactionID'},
    {'1': 'nodeAccountID', '3': 2, '4': 1, '5': 9, '10': 'nodeAccountID'},
    {'1': 'transactionFee', '3': 3, '4': 1, '5': 4, '10': 'transactionFee'},
    {'1': 'transactionValidDuration', '3': 4, '4': 1, '5': 3, '10': 'transactionValidDuration'},
    {'1': 'memo', '3': 5, '4': 1, '5': 9, '10': 'memo'},
    {'1': 'transfer', '3': 6, '4': 1, '5': 11, '6': '.TW.Hedera.Proto.TransferMessage', '9': 0, '10': 'transfer'},
  ],
  '8': [
    {'1': 'data'},
  ],
};

/// Descriptor for `TransactionBody`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionBodyDescriptor = $convert.base64Decode(
    'Cg9UcmFuc2FjdGlvbkJvZHkSRAoNdHJhbnNhY3Rpb25JRBgBIAEoCzIeLlRXLkhlZGVyYS5Qcm'
    '90by5UcmFuc2FjdGlvbklEUg10cmFuc2FjdGlvbklEEiQKDW5vZGVBY2NvdW50SUQYAiABKAlS'
    'DW5vZGVBY2NvdW50SUQSJgoOdHJhbnNhY3Rpb25GZWUYAyABKARSDnRyYW5zYWN0aW9uRmVlEj'
    'oKGHRyYW5zYWN0aW9uVmFsaWREdXJhdGlvbhgEIAEoA1IYdHJhbnNhY3Rpb25WYWxpZER1cmF0'
    'aW9uEhIKBG1lbW8YBSABKAlSBG1lbW8SPgoIdHJhbnNmZXIYBiABKAsyIC5UVy5IZWRlcmEuUH'
    'JvdG8uVHJhbnNmZXJNZXNzYWdlSABSCHRyYW5zZmVyQgYKBGRhdGE=');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'private_key', '3': 1, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'body', '3': 2, '4': 1, '5': 11, '6': '.TW.Hedera.Proto.TransactionBody', '10': 'body'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSHwoLcHJpdmF0ZV9rZXkYASABKAxSCnByaXZhdGVLZXkSNAoEYm9keR'
    'gCIAEoCzIgLlRXLkhlZGVyYS5Qcm90by5UcmFuc2FjdGlvbkJvZHlSBGJvZHk=');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 12, '10': 'encoded'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhgKB2VuY29kZWQYASABKAxSB2VuY29kZWQ=');

