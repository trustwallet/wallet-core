//
//  Generated code. Do not modify.
//  source: Bitcoin.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use transactionVariantDescriptor instead')
const TransactionVariant$json = {
  '1': 'TransactionVariant',
  '2': [
    {'1': 'P2PKH', '2': 0},
    {'1': 'P2WPKH', '2': 1},
    {'1': 'P2TRKEYPATH', '2': 2},
    {'1': 'BRC20TRANSFER', '2': 3},
    {'1': 'NFTINSCRIPTION', '2': 4},
  ],
};

/// Descriptor for `TransactionVariant`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List transactionVariantDescriptor = $convert.base64Decode(
    'ChJUcmFuc2FjdGlvblZhcmlhbnQSCQoFUDJQS0gQABIKCgZQMldQS0gQARIPCgtQMlRSS0VZUE'
    'FUSBACEhEKDUJSQzIwVFJBTlNGRVIQAxISCg5ORlRJTlNDUklQVElPThAE');

@$core.Deprecated('Use transactionDescriptor instead')
const Transaction$json = {
  '1': 'Transaction',
  '2': [
    {'1': 'version', '3': 1, '4': 1, '5': 17, '10': 'version'},
    {'1': 'lockTime', '3': 2, '4': 1, '5': 13, '10': 'lockTime'},
    {'1': 'inputs', '3': 3, '4': 3, '5': 11, '6': '.TW.Bitcoin.Proto.TransactionInput', '10': 'inputs'},
    {'1': 'outputs', '3': 4, '4': 3, '5': 11, '6': '.TW.Bitcoin.Proto.TransactionOutput', '10': 'outputs'},
  ],
};

/// Descriptor for `Transaction`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionDescriptor = $convert.base64Decode(
    'CgtUcmFuc2FjdGlvbhIYCgd2ZXJzaW9uGAEgASgRUgd2ZXJzaW9uEhoKCGxvY2tUaW1lGAIgAS'
    'gNUghsb2NrVGltZRI6CgZpbnB1dHMYAyADKAsyIi5UVy5CaXRjb2luLlByb3RvLlRyYW5zYWN0'
    'aW9uSW5wdXRSBmlucHV0cxI9CgdvdXRwdXRzGAQgAygLMiMuVFcuQml0Y29pbi5Qcm90by5Ucm'
    'Fuc2FjdGlvbk91dHB1dFIHb3V0cHV0cw==');

@$core.Deprecated('Use transactionInputDescriptor instead')
const TransactionInput$json = {
  '1': 'TransactionInput',
  '2': [
    {'1': 'previousOutput', '3': 1, '4': 1, '5': 11, '6': '.TW.Bitcoin.Proto.OutPoint', '10': 'previousOutput'},
    {'1': 'sequence', '3': 2, '4': 1, '5': 13, '10': 'sequence'},
    {'1': 'script', '3': 3, '4': 1, '5': 12, '10': 'script'},
  ],
};

/// Descriptor for `TransactionInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionInputDescriptor = $convert.base64Decode(
    'ChBUcmFuc2FjdGlvbklucHV0EkIKDnByZXZpb3VzT3V0cHV0GAEgASgLMhouVFcuQml0Y29pbi'
    '5Qcm90by5PdXRQb2ludFIOcHJldmlvdXNPdXRwdXQSGgoIc2VxdWVuY2UYAiABKA1SCHNlcXVl'
    'bmNlEhYKBnNjcmlwdBgDIAEoDFIGc2NyaXB0');

@$core.Deprecated('Use outPointDescriptor instead')
const OutPoint$json = {
  '1': 'OutPoint',
  '2': [
    {'1': 'hash', '3': 1, '4': 1, '5': 12, '10': 'hash'},
    {'1': 'index', '3': 2, '4': 1, '5': 13, '10': 'index'},
    {'1': 'sequence', '3': 3, '4': 1, '5': 13, '10': 'sequence'},
    {'1': 'tree', '3': 4, '4': 1, '5': 5, '10': 'tree'},
  ],
};

/// Descriptor for `OutPoint`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List outPointDescriptor = $convert.base64Decode(
    'CghPdXRQb2ludBISCgRoYXNoGAEgASgMUgRoYXNoEhQKBWluZGV4GAIgASgNUgVpbmRleBIaCg'
    'hzZXF1ZW5jZRgDIAEoDVIIc2VxdWVuY2USEgoEdHJlZRgEIAEoBVIEdHJlZQ==');

@$core.Deprecated('Use transactionOutputDescriptor instead')
const TransactionOutput$json = {
  '1': 'TransactionOutput',
  '2': [
    {'1': 'value', '3': 1, '4': 1, '5': 3, '10': 'value'},
    {'1': 'script', '3': 2, '4': 1, '5': 12, '10': 'script'},
    {'1': 'spendingScript', '3': 5, '4': 1, '5': 12, '10': 'spendingScript'},
  ],
};

/// Descriptor for `TransactionOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionOutputDescriptor = $convert.base64Decode(
    'ChFUcmFuc2FjdGlvbk91dHB1dBIUCgV2YWx1ZRgBIAEoA1IFdmFsdWUSFgoGc2NyaXB0GAIgAS'
    'gMUgZzY3JpcHQSJgoOc3BlbmRpbmdTY3JpcHQYBSABKAxSDnNwZW5kaW5nU2NyaXB0');

@$core.Deprecated('Use unspentTransactionDescriptor instead')
const UnspentTransaction$json = {
  '1': 'UnspentTransaction',
  '2': [
    {'1': 'out_point', '3': 1, '4': 1, '5': 11, '6': '.TW.Bitcoin.Proto.OutPoint', '10': 'outPoint'},
    {'1': 'script', '3': 2, '4': 1, '5': 12, '10': 'script'},
    {'1': 'amount', '3': 3, '4': 1, '5': 3, '10': 'amount'},
    {'1': 'variant', '3': 4, '4': 1, '5': 14, '6': '.TW.Bitcoin.Proto.TransactionVariant', '10': 'variant'},
    {'1': 'spendingScript', '3': 5, '4': 1, '5': 12, '10': 'spendingScript'},
  ],
};

/// Descriptor for `UnspentTransaction`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List unspentTransactionDescriptor = $convert.base64Decode(
    'ChJVbnNwZW50VHJhbnNhY3Rpb24SNwoJb3V0X3BvaW50GAEgASgLMhouVFcuQml0Y29pbi5Qcm'
    '90by5PdXRQb2ludFIIb3V0UG9pbnQSFgoGc2NyaXB0GAIgASgMUgZzY3JpcHQSFgoGYW1vdW50'
    'GAMgASgDUgZhbW91bnQSPgoHdmFyaWFudBgEIAEoDjIkLlRXLkJpdGNvaW4uUHJvdG8uVHJhbn'
    'NhY3Rpb25WYXJpYW50Ugd2YXJpYW50EiYKDnNwZW5kaW5nU2NyaXB0GAUgASgMUg5zcGVuZGlu'
    'Z1NjcmlwdA==');

@$core.Deprecated('Use outputAddressDescriptor instead')
const OutputAddress$json = {
  '1': 'OutputAddress',
  '2': [
    {'1': 'to_address', '3': 1, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'amount', '3': 2, '4': 1, '5': 3, '10': 'amount'},
  ],
};

/// Descriptor for `OutputAddress`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List outputAddressDescriptor = $convert.base64Decode(
    'Cg1PdXRwdXRBZGRyZXNzEh0KCnRvX2FkZHJlc3MYASABKAlSCXRvQWRkcmVzcxIWCgZhbW91bn'
    'QYAiABKANSBmFtb3VudA==');

@$core.Deprecated('Use outputIndexDescriptor instead')
const OutputIndex$json = {
  '1': 'OutputIndex',
  '2': [
    {'1': 'index', '3': 1, '4': 1, '5': 13, '10': 'index'},
  ],
};

/// Descriptor for `OutputIndex`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List outputIndexDescriptor = $convert.base64Decode(
    'CgtPdXRwdXRJbmRleBIUCgVpbmRleBgBIAEoDVIFaW5kZXg=');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'hash_type', '3': 1, '4': 1, '5': 13, '10': 'hashType'},
    {'1': 'amount', '3': 2, '4': 1, '5': 3, '10': 'amount'},
    {'1': 'byte_fee', '3': 3, '4': 1, '5': 3, '10': 'byteFee'},
    {'1': 'to_address', '3': 4, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'change_address', '3': 5, '4': 1, '5': 9, '10': 'changeAddress'},
    {'1': 'private_key', '3': 6, '4': 3, '5': 12, '10': 'privateKey'},
    {'1': 'scripts', '3': 7, '4': 3, '5': 11, '6': '.TW.Bitcoin.Proto.SigningInput.ScriptsEntry', '10': 'scripts'},
    {'1': 'utxo', '3': 8, '4': 3, '5': 11, '6': '.TW.Bitcoin.Proto.UnspentTransaction', '10': 'utxo'},
    {'1': 'use_max_amount', '3': 9, '4': 1, '5': 8, '10': 'useMaxAmount'},
    {'1': 'coin_type', '3': 10, '4': 1, '5': 13, '10': 'coinType'},
    {'1': 'plan', '3': 11, '4': 1, '5': 11, '6': '.TW.Bitcoin.Proto.TransactionPlan', '10': 'plan'},
    {'1': 'lock_time', '3': 12, '4': 1, '5': 13, '10': 'lockTime'},
    {'1': 'output_op_return', '3': 13, '4': 1, '5': 12, '10': 'outputOpReturn'},
    {'1': 'output_op_return_index', '3': 26, '4': 1, '5': 11, '6': '.TW.Bitcoin.Proto.OutputIndex', '10': 'outputOpReturnIndex'},
    {'1': 'extra_outputs', '3': 14, '4': 3, '5': 11, '6': '.TW.Bitcoin.Proto.OutputAddress', '10': 'extraOutputs'},
    {'1': 'use_max_utxo', '3': 15, '4': 1, '5': 8, '10': 'useMaxUtxo'},
    {'1': 'disable_dust_filter', '3': 16, '4': 1, '5': 8, '10': 'disableDustFilter'},
    {'1': 'time', '3': 17, '4': 1, '5': 13, '10': 'time'},
    {'1': 'signing_v2', '3': 21, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.SigningInput', '10': 'signingV2'},
    {'1': 'fixed_dust_threshold', '3': 24, '4': 1, '5': 3, '9': 0, '10': 'fixedDustThreshold'},
  ],
  '3': [SigningInput_ScriptsEntry$json],
  '8': [
    {'1': 'dust_policy'},
  ],
};

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput_ScriptsEntry$json = {
  '1': 'ScriptsEntry',
  '2': [
    {'1': 'key', '3': 1, '4': 1, '5': 9, '10': 'key'},
    {'1': 'value', '3': 2, '4': 1, '5': 12, '10': 'value'},
  ],
  '7': {'7': true},
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSGwoJaGFzaF90eXBlGAEgASgNUghoYXNoVHlwZRIWCgZhbW91bnQYAi'
    'ABKANSBmFtb3VudBIZCghieXRlX2ZlZRgDIAEoA1IHYnl0ZUZlZRIdCgp0b19hZGRyZXNzGAQg'
    'ASgJUgl0b0FkZHJlc3MSJQoOY2hhbmdlX2FkZHJlc3MYBSABKAlSDWNoYW5nZUFkZHJlc3MSHw'
    'oLcHJpdmF0ZV9rZXkYBiADKAxSCnByaXZhdGVLZXkSRQoHc2NyaXB0cxgHIAMoCzIrLlRXLkJp'
    'dGNvaW4uUHJvdG8uU2lnbmluZ0lucHV0LlNjcmlwdHNFbnRyeVIHc2NyaXB0cxI4CgR1dHhvGA'
    'ggAygLMiQuVFcuQml0Y29pbi5Qcm90by5VbnNwZW50VHJhbnNhY3Rpb25SBHV0eG8SJAoOdXNl'
    'X21heF9hbW91bnQYCSABKAhSDHVzZU1heEFtb3VudBIbCgljb2luX3R5cGUYCiABKA1SCGNvaW'
    '5UeXBlEjUKBHBsYW4YCyABKAsyIS5UVy5CaXRjb2luLlByb3RvLlRyYW5zYWN0aW9uUGxhblIE'
    'cGxhbhIbCglsb2NrX3RpbWUYDCABKA1SCGxvY2tUaW1lEigKEG91dHB1dF9vcF9yZXR1cm4YDS'
    'ABKAxSDm91dHB1dE9wUmV0dXJuElIKFm91dHB1dF9vcF9yZXR1cm5faW5kZXgYGiABKAsyHS5U'
    'Vy5CaXRjb2luLlByb3RvLk91dHB1dEluZGV4UhNvdXRwdXRPcFJldHVybkluZGV4EkQKDWV4dH'
    'JhX291dHB1dHMYDiADKAsyHy5UVy5CaXRjb2luLlByb3RvLk91dHB1dEFkZHJlc3NSDGV4dHJh'
    'T3V0cHV0cxIgCgx1c2VfbWF4X3V0eG8YDyABKAhSCnVzZU1heFV0eG8SLgoTZGlzYWJsZV9kdX'
    'N0X2ZpbHRlchgQIAEoCFIRZGlzYWJsZUR1c3RGaWx0ZXISEgoEdGltZRgRIAEoDVIEdGltZRI/'
    'CgpzaWduaW5nX3YyGBUgASgLMiAuVFcuQml0Y29pblYyLlByb3RvLlNpZ25pbmdJbnB1dFIJc2'
    'lnbmluZ1YyEjIKFGZpeGVkX2R1c3RfdGhyZXNob2xkGBggASgDSABSEmZpeGVkRHVzdFRocmVz'
    'aG9sZBo6CgxTY3JpcHRzRW50cnkSEAoDa2V5GAEgASgJUgNrZXkSFAoFdmFsdWUYAiABKAxSBX'
    'ZhbHVlOgI4AUINCgtkdXN0X3BvbGljeQ==');

@$core.Deprecated('Use transactionPlanDescriptor instead')
const TransactionPlan$json = {
  '1': 'TransactionPlan',
  '2': [
    {'1': 'amount', '3': 1, '4': 1, '5': 3, '10': 'amount'},
    {'1': 'available_amount', '3': 2, '4': 1, '5': 3, '10': 'availableAmount'},
    {'1': 'fee', '3': 3, '4': 1, '5': 3, '10': 'fee'},
    {'1': 'change', '3': 4, '4': 1, '5': 3, '10': 'change'},
    {'1': 'utxos', '3': 5, '4': 3, '5': 11, '6': '.TW.Bitcoin.Proto.UnspentTransaction', '10': 'utxos'},
    {'1': 'branch_id', '3': 6, '4': 1, '5': 12, '10': 'branchId'},
    {'1': 'error', '3': 7, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'output_op_return', '3': 8, '4': 1, '5': 12, '10': 'outputOpReturn'},
    {'1': 'output_op_return_index', '3': 14, '4': 1, '5': 11, '6': '.TW.Bitcoin.Proto.OutputIndex', '10': 'outputOpReturnIndex'},
    {'1': 'preblockhash', '3': 9, '4': 1, '5': 12, '10': 'preblockhash'},
    {'1': 'preblockheight', '3': 10, '4': 1, '5': 3, '10': 'preblockheight'},
    {'1': 'planning_result_v2', '3': 12, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.TransactionPlan', '10': 'planningResultV2'},
  ],
};

/// Descriptor for `TransactionPlan`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionPlanDescriptor = $convert.base64Decode(
    'Cg9UcmFuc2FjdGlvblBsYW4SFgoGYW1vdW50GAEgASgDUgZhbW91bnQSKQoQYXZhaWxhYmxlX2'
    'Ftb3VudBgCIAEoA1IPYXZhaWxhYmxlQW1vdW50EhAKA2ZlZRgDIAEoA1IDZmVlEhYKBmNoYW5n'
    'ZRgEIAEoA1IGY2hhbmdlEjoKBXV0eG9zGAUgAygLMiQuVFcuQml0Y29pbi5Qcm90by5VbnNwZW'
    '50VHJhbnNhY3Rpb25SBXV0eG9zEhsKCWJyYW5jaF9pZBgGIAEoDFIIYnJhbmNoSWQSMwoFZXJy'
    'b3IYByABKA4yHS5UVy5Db21tb24uUHJvdG8uU2lnbmluZ0Vycm9yUgVlcnJvchIoChBvdXRwdX'
    'Rfb3BfcmV0dXJuGAggASgMUg5vdXRwdXRPcFJldHVybhJSChZvdXRwdXRfb3BfcmV0dXJuX2lu'
    'ZGV4GA4gASgLMh0uVFcuQml0Y29pbi5Qcm90by5PdXRwdXRJbmRleFITb3V0cHV0T3BSZXR1cm'
    '5JbmRleBIiCgxwcmVibG9ja2hhc2gYCSABKAxSDHByZWJsb2NraGFzaBImCg5wcmVibG9ja2hl'
    'aWdodBgKIAEoA1IOcHJlYmxvY2toZWlnaHQSUQoScGxhbm5pbmdfcmVzdWx0X3YyGAwgASgLMi'
    'MuVFcuQml0Y29pblYyLlByb3RvLlRyYW5zYWN0aW9uUGxhblIQcGxhbm5pbmdSZXN1bHRWMg==');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'transaction', '3': 1, '4': 1, '5': 11, '6': '.TW.Bitcoin.Proto.Transaction', '10': 'transaction'},
    {'1': 'encoded', '3': 2, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'transaction_id', '3': 3, '4': 1, '5': 9, '10': 'transactionId'},
    {'1': 'error', '3': 4, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 5, '4': 1, '5': 9, '10': 'errorMessage'},
    {'1': 'signing_result_v2', '3': 7, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.SigningOutput', '10': 'signingResultV2'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0Ej8KC3RyYW5zYWN0aW9uGAEgASgLMh0uVFcuQml0Y29pbi5Qcm90by'
    '5UcmFuc2FjdGlvblILdHJhbnNhY3Rpb24SGAoHZW5jb2RlZBgCIAEoDFIHZW5jb2RlZBIlCg50'
    'cmFuc2FjdGlvbl9pZBgDIAEoCVINdHJhbnNhY3Rpb25JZBIzCgVlcnJvchgEIAEoDjIdLlRXLk'
    'NvbW1vbi5Qcm90by5TaWduaW5nRXJyb3JSBWVycm9yEiMKDWVycm9yX21lc3NhZ2UYBSABKAlS'
    'DGVycm9yTWVzc2FnZRJNChFzaWduaW5nX3Jlc3VsdF92MhgHIAEoCzIhLlRXLkJpdGNvaW5WMi'
    '5Qcm90by5TaWduaW5nT3V0cHV0Ug9zaWduaW5nUmVzdWx0VjI=');

@$core.Deprecated('Use hashPublicKeyDescriptor instead')
const HashPublicKey$json = {
  '1': 'HashPublicKey',
  '2': [
    {'1': 'data_hash', '3': 1, '4': 1, '5': 12, '10': 'dataHash'},
    {'1': 'public_key_hash', '3': 2, '4': 1, '5': 12, '10': 'publicKeyHash'},
  ],
};

/// Descriptor for `HashPublicKey`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List hashPublicKeyDescriptor = $convert.base64Decode(
    'Cg1IYXNoUHVibGljS2V5EhsKCWRhdGFfaGFzaBgBIAEoDFIIZGF0YUhhc2gSJgoPcHVibGljX2'
    'tleV9oYXNoGAIgASgMUg1wdWJsaWNLZXlIYXNo');

@$core.Deprecated('Use preSigningOutputDescriptor instead')
const PreSigningOutput$json = {
  '1': 'PreSigningOutput',
  '2': [
    {'1': 'hash_public_keys', '3': 1, '4': 3, '5': 11, '6': '.TW.Bitcoin.Proto.HashPublicKey', '10': 'hashPublicKeys'},
    {'1': 'error', '3': 2, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 3, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `PreSigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List preSigningOutputDescriptor = $convert.base64Decode(
    'ChBQcmVTaWduaW5nT3V0cHV0EkkKEGhhc2hfcHVibGljX2tleXMYASADKAsyHy5UVy5CaXRjb2'
    'luLlByb3RvLkhhc2hQdWJsaWNLZXlSDmhhc2hQdWJsaWNLZXlzEjMKBWVycm9yGAIgASgOMh0u'
    'VFcuQ29tbW9uLlByb3RvLlNpZ25pbmdFcnJvclIFZXJyb3ISIwoNZXJyb3JfbWVzc2FnZRgDIA'
    'EoCVIMZXJyb3JNZXNzYWdl');

