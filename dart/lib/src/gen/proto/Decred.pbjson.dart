//
//  Generated code. Do not modify.
//  source: Decred.proto
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
    {'1': 'serializeType', '3': 1, '4': 1, '5': 13, '10': 'serializeType'},
    {'1': 'version', '3': 2, '4': 1, '5': 13, '10': 'version'},
    {'1': 'inputs', '3': 3, '4': 3, '5': 11, '6': '.TW.Decred.Proto.TransactionInput', '10': 'inputs'},
    {'1': 'outputs', '3': 4, '4': 3, '5': 11, '6': '.TW.Decred.Proto.TransactionOutput', '10': 'outputs'},
    {'1': 'lockTime', '3': 5, '4': 1, '5': 13, '10': 'lockTime'},
    {'1': 'expiry', '3': 6, '4': 1, '5': 13, '10': 'expiry'},
  ],
};

/// Descriptor for `Transaction`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionDescriptor = $convert.base64Decode(
    'CgtUcmFuc2FjdGlvbhIkCg1zZXJpYWxpemVUeXBlGAEgASgNUg1zZXJpYWxpemVUeXBlEhgKB3'
    'ZlcnNpb24YAiABKA1SB3ZlcnNpb24SOQoGaW5wdXRzGAMgAygLMiEuVFcuRGVjcmVkLlByb3Rv'
    'LlRyYW5zYWN0aW9uSW5wdXRSBmlucHV0cxI8CgdvdXRwdXRzGAQgAygLMiIuVFcuRGVjcmVkLl'
    'Byb3RvLlRyYW5zYWN0aW9uT3V0cHV0UgdvdXRwdXRzEhoKCGxvY2tUaW1lGAUgASgNUghsb2Nr'
    'VGltZRIWCgZleHBpcnkYBiABKA1SBmV4cGlyeQ==');

@$core.Deprecated('Use transactionInputDescriptor instead')
const TransactionInput$json = {
  '1': 'TransactionInput',
  '2': [
    {'1': 'previousOutput', '3': 1, '4': 1, '5': 11, '6': '.TW.Bitcoin.Proto.OutPoint', '10': 'previousOutput'},
    {'1': 'sequence', '3': 2, '4': 1, '5': 13, '10': 'sequence'},
    {'1': 'valueIn', '3': 3, '4': 1, '5': 3, '10': 'valueIn'},
    {'1': 'blockHeight', '3': 4, '4': 1, '5': 13, '10': 'blockHeight'},
    {'1': 'blockIndex', '3': 5, '4': 1, '5': 13, '10': 'blockIndex'},
    {'1': 'script', '3': 6, '4': 1, '5': 12, '10': 'script'},
  ],
};

/// Descriptor for `TransactionInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionInputDescriptor = $convert.base64Decode(
    'ChBUcmFuc2FjdGlvbklucHV0EkIKDnByZXZpb3VzT3V0cHV0GAEgASgLMhouVFcuQml0Y29pbi'
    '5Qcm90by5PdXRQb2ludFIOcHJldmlvdXNPdXRwdXQSGgoIc2VxdWVuY2UYAiABKA1SCHNlcXVl'
    'bmNlEhgKB3ZhbHVlSW4YAyABKANSB3ZhbHVlSW4SIAoLYmxvY2tIZWlnaHQYBCABKA1SC2Jsb2'
    'NrSGVpZ2h0Eh4KCmJsb2NrSW5kZXgYBSABKA1SCmJsb2NrSW5kZXgSFgoGc2NyaXB0GAYgASgM'
    'UgZzY3JpcHQ=');

@$core.Deprecated('Use transactionOutputDescriptor instead')
const TransactionOutput$json = {
  '1': 'TransactionOutput',
  '2': [
    {'1': 'value', '3': 1, '4': 1, '5': 3, '10': 'value'},
    {'1': 'version', '3': 2, '4': 1, '5': 13, '10': 'version'},
    {'1': 'script', '3': 3, '4': 1, '5': 12, '10': 'script'},
  ],
};

/// Descriptor for `TransactionOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionOutputDescriptor = $convert.base64Decode(
    'ChFUcmFuc2FjdGlvbk91dHB1dBIUCgV2YWx1ZRgBIAEoA1IFdmFsdWUSGAoHdmVyc2lvbhgCIA'
    'EoDVIHdmVyc2lvbhIWCgZzY3JpcHQYAyABKAxSBnNjcmlwdA==');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'transaction', '3': 1, '4': 1, '5': 11, '6': '.TW.Decred.Proto.Transaction', '10': 'transaction'},
    {'1': 'encoded', '3': 2, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'transaction_id', '3': 3, '4': 1, '5': 9, '10': 'transactionId'},
    {'1': 'error', '3': 4, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 5, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0Ej4KC3RyYW5zYWN0aW9uGAEgASgLMhwuVFcuRGVjcmVkLlByb3RvLl'
    'RyYW5zYWN0aW9uUgt0cmFuc2FjdGlvbhIYCgdlbmNvZGVkGAIgASgMUgdlbmNvZGVkEiUKDnRy'
    'YW5zYWN0aW9uX2lkGAMgASgJUg10cmFuc2FjdGlvbklkEjMKBWVycm9yGAQgASgOMh0uVFcuQ2'
    '9tbW9uLlByb3RvLlNpZ25pbmdFcnJvclIFZXJyb3ISIwoNZXJyb3JfbWVzc2FnZRgFIAEoCVIM'
    'ZXJyb3JNZXNzYWdl');

