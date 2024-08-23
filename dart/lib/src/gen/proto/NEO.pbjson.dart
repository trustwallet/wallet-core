//
//  Generated code. Do not modify.
//  source: NEO.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use transactionInputDescriptor instead')
const TransactionInput$json = {
  '1': 'TransactionInput',
  '2': [
    {'1': 'prev_hash', '3': 1, '4': 1, '5': 12, '10': 'prevHash'},
    {'1': 'prev_index', '3': 2, '4': 1, '5': 7, '10': 'prevIndex'},
    {'1': 'value', '3': 3, '4': 1, '5': 3, '10': 'value'},
    {'1': 'asset_id', '3': 4, '4': 1, '5': 9, '10': 'assetId'},
  ],
};

/// Descriptor for `TransactionInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionInputDescriptor = $convert.base64Decode(
    'ChBUcmFuc2FjdGlvbklucHV0EhsKCXByZXZfaGFzaBgBIAEoDFIIcHJldkhhc2gSHQoKcHJldl'
    '9pbmRleBgCIAEoB1IJcHJldkluZGV4EhQKBXZhbHVlGAMgASgDUgV2YWx1ZRIZCghhc3NldF9p'
    'ZBgEIAEoCVIHYXNzZXRJZA==');

@$core.Deprecated('Use outputAddressDescriptor instead')
const OutputAddress$json = {
  '1': 'OutputAddress',
  '2': [
    {'1': 'amount', '3': 1, '4': 1, '5': 18, '10': 'amount'},
    {'1': 'to_address', '3': 2, '4': 1, '5': 9, '10': 'toAddress'},
  ],
};

/// Descriptor for `OutputAddress`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List outputAddressDescriptor = $convert.base64Decode(
    'Cg1PdXRwdXRBZGRyZXNzEhYKBmFtb3VudBgBIAEoElIGYW1vdW50Eh0KCnRvX2FkZHJlc3MYAi'
    'ABKAlSCXRvQWRkcmVzcw==');

@$core.Deprecated('Use transactionOutputDescriptor instead')
const TransactionOutput$json = {
  '1': 'TransactionOutput',
  '2': [
    {'1': 'asset_id', '3': 1, '4': 1, '5': 9, '10': 'assetId'},
    {'1': 'amount', '3': 2, '4': 1, '5': 18, '10': 'amount'},
    {'1': 'to_address', '3': 3, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'change_address', '3': 4, '4': 1, '5': 9, '10': 'changeAddress'},
    {'1': 'extra_outputs', '3': 5, '4': 3, '5': 11, '6': '.TW.NEO.Proto.OutputAddress', '10': 'extraOutputs'},
  ],
};

/// Descriptor for `TransactionOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionOutputDescriptor = $convert.base64Decode(
    'ChFUcmFuc2FjdGlvbk91dHB1dBIZCghhc3NldF9pZBgBIAEoCVIHYXNzZXRJZBIWCgZhbW91bn'
    'QYAiABKBJSBmFtb3VudBIdCgp0b19hZGRyZXNzGAMgASgJUgl0b0FkZHJlc3MSJQoOY2hhbmdl'
    'X2FkZHJlc3MYBCABKAlSDWNoYW5nZUFkZHJlc3MSQAoNZXh0cmFfb3V0cHV0cxgFIAMoCzIbLl'
    'RXLk5FTy5Qcm90by5PdXRwdXRBZGRyZXNzUgxleHRyYU91dHB1dHM=');

@$core.Deprecated('Use transactionDescriptor instead')
const Transaction$json = {
  '1': 'Transaction',
  '2': [
    {'1': 'nep5_transfer', '3': 1, '4': 1, '5': 11, '6': '.TW.NEO.Proto.Transaction.Nep5Transfer', '9': 0, '10': 'nep5Transfer'},
    {'1': 'invocation_generic', '3': 2, '4': 1, '5': 11, '6': '.TW.NEO.Proto.Transaction.InvocationGeneric', '9': 0, '10': 'invocationGeneric'},
  ],
  '3': [Transaction_Nep5Transfer$json, Transaction_InvocationGeneric$json],
  '8': [
    {'1': 'transaction_oneof'},
  ],
};

@$core.Deprecated('Use transactionDescriptor instead')
const Transaction_Nep5Transfer$json = {
  '1': 'Nep5Transfer',
  '2': [
    {'1': 'asset_id', '3': 1, '4': 1, '5': 9, '10': 'assetId'},
    {'1': 'from', '3': 2, '4': 1, '5': 9, '10': 'from'},
    {'1': 'to', '3': 3, '4': 1, '5': 9, '10': 'to'},
    {'1': 'amount', '3': 4, '4': 1, '5': 12, '10': 'amount'},
    {'1': 'script_with_ret', '3': 5, '4': 1, '5': 8, '10': 'scriptWithRet'},
  ],
};

@$core.Deprecated('Use transactionDescriptor instead')
const Transaction_InvocationGeneric$json = {
  '1': 'InvocationGeneric',
  '2': [
    {'1': 'gas', '3': 1, '4': 1, '5': 4, '10': 'gas'},
    {'1': 'script', '3': 2, '4': 1, '5': 12, '10': 'script'},
  ],
};

/// Descriptor for `Transaction`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionDescriptor = $convert.base64Decode(
    'CgtUcmFuc2FjdGlvbhJNCg1uZXA1X3RyYW5zZmVyGAEgASgLMiYuVFcuTkVPLlByb3RvLlRyYW'
    '5zYWN0aW9uLk5lcDVUcmFuc2ZlckgAUgxuZXA1VHJhbnNmZXISXAoSaW52b2NhdGlvbl9nZW5l'
    'cmljGAIgASgLMisuVFcuTkVPLlByb3RvLlRyYW5zYWN0aW9uLkludm9jYXRpb25HZW5lcmljSA'
    'BSEWludm9jYXRpb25HZW5lcmljGo0BCgxOZXA1VHJhbnNmZXISGQoIYXNzZXRfaWQYASABKAlS'
    'B2Fzc2V0SWQSEgoEZnJvbRgCIAEoCVIEZnJvbRIOCgJ0bxgDIAEoCVICdG8SFgoGYW1vdW50GA'
    'QgASgMUgZhbW91bnQSJgoPc2NyaXB0X3dpdGhfcmV0GAUgASgIUg1zY3JpcHRXaXRoUmV0Gj0K'
    'EUludm9jYXRpb25HZW5lcmljEhAKA2dhcxgBIAEoBFIDZ2FzEhYKBnNjcmlwdBgCIAEoDFIGc2'
    'NyaXB0QhMKEXRyYW5zYWN0aW9uX29uZW9m');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'inputs', '3': 1, '4': 3, '5': 11, '6': '.TW.NEO.Proto.TransactionInput', '10': 'inputs'},
    {'1': 'outputs', '3': 2, '4': 3, '5': 11, '6': '.TW.NEO.Proto.TransactionOutput', '10': 'outputs'},
    {'1': 'private_key', '3': 3, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'fee', '3': 4, '4': 1, '5': 3, '10': 'fee'},
    {'1': 'gas_asset_id', '3': 5, '4': 1, '5': 9, '10': 'gasAssetId'},
    {'1': 'gas_change_address', '3': 6, '4': 1, '5': 9, '10': 'gasChangeAddress'},
    {'1': 'plan', '3': 7, '4': 1, '5': 11, '6': '.TW.NEO.Proto.TransactionPlan', '10': 'plan'},
    {'1': 'transaction', '3': 8, '4': 1, '5': 11, '6': '.TW.NEO.Proto.Transaction', '10': 'transaction'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSNgoGaW5wdXRzGAEgAygLMh4uVFcuTkVPLlByb3RvLlRyYW5zYWN0aW'
    '9uSW5wdXRSBmlucHV0cxI5CgdvdXRwdXRzGAIgAygLMh8uVFcuTkVPLlByb3RvLlRyYW5zYWN0'
    'aW9uT3V0cHV0UgdvdXRwdXRzEh8KC3ByaXZhdGVfa2V5GAMgASgMUgpwcml2YXRlS2V5EhAKA2'
    'ZlZRgEIAEoA1IDZmVlEiAKDGdhc19hc3NldF9pZBgFIAEoCVIKZ2FzQXNzZXRJZBIsChJnYXNf'
    'Y2hhbmdlX2FkZHJlc3MYBiABKAlSEGdhc0NoYW5nZUFkZHJlc3MSMQoEcGxhbhgHIAEoCzIdLl'
    'RXLk5FTy5Qcm90by5UcmFuc2FjdGlvblBsYW5SBHBsYW4SOwoLdHJhbnNhY3Rpb24YCCABKAsy'
    'GS5UVy5ORU8uUHJvdG8uVHJhbnNhY3Rpb25SC3RyYW5zYWN0aW9u');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'error', '3': 2, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 3, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhgKB2VuY29kZWQYASABKAxSB2VuY29kZWQSMwoFZXJyb3IYAiABKA'
    '4yHS5UVy5Db21tb24uUHJvdG8uU2lnbmluZ0Vycm9yUgVlcnJvchIjCg1lcnJvcl9tZXNzYWdl'
    'GAMgASgJUgxlcnJvck1lc3NhZ2U=');

@$core.Deprecated('Use transactionOutputPlanDescriptor instead')
const TransactionOutputPlan$json = {
  '1': 'TransactionOutputPlan',
  '2': [
    {'1': 'amount', '3': 1, '4': 1, '5': 3, '10': 'amount'},
    {'1': 'available_amount', '3': 2, '4': 1, '5': 3, '10': 'availableAmount'},
    {'1': 'change', '3': 3, '4': 1, '5': 3, '10': 'change'},
    {'1': 'asset_id', '3': 4, '4': 1, '5': 9, '10': 'assetId'},
    {'1': 'to_address', '3': 5, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'change_address', '3': 6, '4': 1, '5': 9, '10': 'changeAddress'},
    {'1': 'extra_outputs', '3': 7, '4': 3, '5': 11, '6': '.TW.NEO.Proto.OutputAddress', '10': 'extraOutputs'},
  ],
};

/// Descriptor for `TransactionOutputPlan`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionOutputPlanDescriptor = $convert.base64Decode(
    'ChVUcmFuc2FjdGlvbk91dHB1dFBsYW4SFgoGYW1vdW50GAEgASgDUgZhbW91bnQSKQoQYXZhaW'
    'xhYmxlX2Ftb3VudBgCIAEoA1IPYXZhaWxhYmxlQW1vdW50EhYKBmNoYW5nZRgDIAEoA1IGY2hh'
    'bmdlEhkKCGFzc2V0X2lkGAQgASgJUgdhc3NldElkEh0KCnRvX2FkZHJlc3MYBSABKAlSCXRvQW'
    'RkcmVzcxIlCg5jaGFuZ2VfYWRkcmVzcxgGIAEoCVINY2hhbmdlQWRkcmVzcxJACg1leHRyYV9v'
    'dXRwdXRzGAcgAygLMhsuVFcuTkVPLlByb3RvLk91dHB1dEFkZHJlc3NSDGV4dHJhT3V0cHV0cw'
    '==');

@$core.Deprecated('Use transactionAttributePlanDescriptor instead')
const TransactionAttributePlan$json = {
  '1': 'TransactionAttributePlan',
  '2': [
    {'1': 'usage', '3': 1, '4': 1, '5': 5, '10': 'usage'},
    {'1': 'data', '3': 2, '4': 1, '5': 12, '10': 'data'},
  ],
};

/// Descriptor for `TransactionAttributePlan`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionAttributePlanDescriptor = $convert.base64Decode(
    'ChhUcmFuc2FjdGlvbkF0dHJpYnV0ZVBsYW4SFAoFdXNhZ2UYASABKAVSBXVzYWdlEhIKBGRhdG'
    'EYAiABKAxSBGRhdGE=');

@$core.Deprecated('Use transactionPlanDescriptor instead')
const TransactionPlan$json = {
  '1': 'TransactionPlan',
  '2': [
    {'1': 'outputs', '3': 1, '4': 3, '5': 11, '6': '.TW.NEO.Proto.TransactionOutputPlan', '10': 'outputs'},
    {'1': 'inputs', '3': 2, '4': 3, '5': 11, '6': '.TW.NEO.Proto.TransactionInput', '10': 'inputs'},
    {'1': 'fee', '3': 3, '4': 1, '5': 3, '10': 'fee'},
    {'1': 'error', '3': 4, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'attributes', '3': 5, '4': 3, '5': 11, '6': '.TW.NEO.Proto.TransactionAttributePlan', '10': 'attributes'},
  ],
};

/// Descriptor for `TransactionPlan`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionPlanDescriptor = $convert.base64Decode(
    'Cg9UcmFuc2FjdGlvblBsYW4SPQoHb3V0cHV0cxgBIAMoCzIjLlRXLk5FTy5Qcm90by5UcmFuc2'
    'FjdGlvbk91dHB1dFBsYW5SB291dHB1dHMSNgoGaW5wdXRzGAIgAygLMh4uVFcuTkVPLlByb3Rv'
    'LlRyYW5zYWN0aW9uSW5wdXRSBmlucHV0cxIQCgNmZWUYAyABKANSA2ZlZRIzCgVlcnJvchgEIA'
    'EoDjIdLlRXLkNvbW1vbi5Qcm90by5TaWduaW5nRXJyb3JSBWVycm9yEkYKCmF0dHJpYnV0ZXMY'
    'BSADKAsyJi5UVy5ORU8uUHJvdG8uVHJhbnNhY3Rpb25BdHRyaWJ1dGVQbGFuUgphdHRyaWJ1dG'
    'Vz');

