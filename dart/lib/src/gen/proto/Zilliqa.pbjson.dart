//
//  Generated code. Do not modify.
//  source: Zilliqa.proto
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
    {'1': 'transfer', '3': 1, '4': 1, '5': 11, '6': '.TW.Zilliqa.Proto.Transaction.Transfer', '9': 0, '10': 'transfer'},
    {'1': 'raw_transaction', '3': 2, '4': 1, '5': 11, '6': '.TW.Zilliqa.Proto.Transaction.Raw', '9': 0, '10': 'rawTransaction'},
  ],
  '3': [Transaction_Transfer$json, Transaction_Raw$json],
  '8': [
    {'1': 'message_oneof'},
  ],
};

@$core.Deprecated('Use transactionDescriptor instead')
const Transaction_Transfer$json = {
  '1': 'Transfer',
  '2': [
    {'1': 'amount', '3': 1, '4': 1, '5': 12, '10': 'amount'},
  ],
};

@$core.Deprecated('Use transactionDescriptor instead')
const Transaction_Raw$json = {
  '1': 'Raw',
  '2': [
    {'1': 'amount', '3': 1, '4': 1, '5': 12, '10': 'amount'},
    {'1': 'code', '3': 2, '4': 1, '5': 12, '10': 'code'},
    {'1': 'data', '3': 3, '4': 1, '5': 12, '10': 'data'},
  ],
};

/// Descriptor for `Transaction`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionDescriptor = $convert.base64Decode(
    'CgtUcmFuc2FjdGlvbhJECgh0cmFuc2ZlchgBIAEoCzImLlRXLlppbGxpcWEuUHJvdG8uVHJhbn'
    'NhY3Rpb24uVHJhbnNmZXJIAFIIdHJhbnNmZXISTAoPcmF3X3RyYW5zYWN0aW9uGAIgASgLMiEu'
    'VFcuWmlsbGlxYS5Qcm90by5UcmFuc2FjdGlvbi5SYXdIAFIOcmF3VHJhbnNhY3Rpb24aIgoIVH'
    'JhbnNmZXISFgoGYW1vdW50GAEgASgMUgZhbW91bnQaRQoDUmF3EhYKBmFtb3VudBgBIAEoDFIG'
    'YW1vdW50EhIKBGNvZGUYAiABKAxSBGNvZGUSEgoEZGF0YRgDIAEoDFIEZGF0YUIPCg1tZXNzYW'
    'dlX29uZW9m');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'version', '3': 1, '4': 1, '5': 13, '10': 'version'},
    {'1': 'nonce', '3': 2, '4': 1, '5': 4, '10': 'nonce'},
    {'1': 'to', '3': 3, '4': 1, '5': 9, '10': 'to'},
    {'1': 'gas_price', '3': 4, '4': 1, '5': 12, '10': 'gasPrice'},
    {'1': 'gas_limit', '3': 5, '4': 1, '5': 4, '10': 'gasLimit'},
    {'1': 'private_key', '3': 6, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'transaction', '3': 7, '4': 1, '5': 11, '6': '.TW.Zilliqa.Proto.Transaction', '10': 'transaction'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSGAoHdmVyc2lvbhgBIAEoDVIHdmVyc2lvbhIUCgVub25jZRgCIAEoBF'
    'IFbm9uY2USDgoCdG8YAyABKAlSAnRvEhsKCWdhc19wcmljZRgEIAEoDFIIZ2FzUHJpY2USGwoJ'
    'Z2FzX2xpbWl0GAUgASgEUghnYXNMaW1pdBIfCgtwcml2YXRlX2tleRgGIAEoDFIKcHJpdmF0ZU'
    'tleRI/Cgt0cmFuc2FjdGlvbhgHIAEoCzIdLlRXLlppbGxpcWEuUHJvdG8uVHJhbnNhY3Rpb25S'
    'C3RyYW5zYWN0aW9u');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'signature', '3': 1, '4': 1, '5': 12, '10': 'signature'},
    {'1': 'json', '3': 2, '4': 1, '5': 9, '10': 'json'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhwKCXNpZ25hdHVyZRgBIAEoDFIJc2lnbmF0dXJlEhIKBGpzb24YAi'
    'ABKAlSBGpzb24=');

