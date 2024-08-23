//
//  Generated code. Do not modify.
//  source: Greenfield.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use broadcastModeDescriptor instead')
const BroadcastMode$json = {
  '1': 'BroadcastMode',
  '2': [
    {'1': 'SYNC', '2': 0},
    {'1': 'ASYNC', '2': 1},
  ],
};

/// Descriptor for `BroadcastMode`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List broadcastModeDescriptor = $convert.base64Decode(
    'Cg1Ccm9hZGNhc3RNb2RlEggKBFNZTkMQABIJCgVBU1lOQxAB');

@$core.Deprecated('Use encodingModeDescriptor instead')
const EncodingMode$json = {
  '1': 'EncodingMode',
  '2': [
    {'1': 'Protobuf', '2': 0},
  ],
};

/// Descriptor for `EncodingMode`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List encodingModeDescriptor = $convert.base64Decode(
    'CgxFbmNvZGluZ01vZGUSDAoIUHJvdG9idWYQAA==');

@$core.Deprecated('Use signingModeDescriptor instead')
const SigningMode$json = {
  '1': 'SigningMode',
  '2': [
    {'1': 'Eip712', '2': 0},
  ],
};

/// Descriptor for `SigningMode`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List signingModeDescriptor = $convert.base64Decode(
    'CgtTaWduaW5nTW9kZRIKCgZFaXA3MTIQAA==');

@$core.Deprecated('Use amountDescriptor instead')
const Amount$json = {
  '1': 'Amount',
  '2': [
    {'1': 'denom', '3': 1, '4': 1, '5': 9, '10': 'denom'},
    {'1': 'amount', '3': 2, '4': 1, '5': 9, '10': 'amount'},
  ],
};

/// Descriptor for `Amount`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List amountDescriptor = $convert.base64Decode(
    'CgZBbW91bnQSFAoFZGVub20YASABKAlSBWRlbm9tEhYKBmFtb3VudBgCIAEoCVIGYW1vdW50');

@$core.Deprecated('Use feeDescriptor instead')
const Fee$json = {
  '1': 'Fee',
  '2': [
    {'1': 'amounts', '3': 1, '4': 3, '5': 11, '6': '.TW.Greenfield.Proto.Amount', '10': 'amounts'},
    {'1': 'gas', '3': 2, '4': 1, '5': 4, '10': 'gas'},
  ],
};

/// Descriptor for `Fee`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List feeDescriptor = $convert.base64Decode(
    'CgNGZWUSNQoHYW1vdW50cxgBIAMoCzIbLlRXLkdyZWVuZmllbGQuUHJvdG8uQW1vdW50UgdhbW'
    '91bnRzEhAKA2dhcxgCIAEoBFIDZ2Fz');

@$core.Deprecated('Use messageDescriptor instead')
const Message$json = {
  '1': 'Message',
  '2': [
    {'1': 'send_coins_message', '3': 1, '4': 1, '5': 11, '6': '.TW.Greenfield.Proto.Message.Send', '9': 0, '10': 'sendCoinsMessage'},
    {'1': 'bridge_transfer_out', '3': 2, '4': 1, '5': 11, '6': '.TW.Greenfield.Proto.Message.BridgeTransferOut', '9': 0, '10': 'bridgeTransferOut'},
  ],
  '3': [Message_Send$json, Message_BridgeTransferOut$json],
  '8': [
    {'1': 'message_oneof'},
  ],
};

@$core.Deprecated('Use messageDescriptor instead')
const Message_Send$json = {
  '1': 'Send',
  '2': [
    {'1': 'from_address', '3': 1, '4': 1, '5': 9, '10': 'fromAddress'},
    {'1': 'to_address', '3': 2, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'amounts', '3': 3, '4': 3, '5': 11, '6': '.TW.Greenfield.Proto.Amount', '10': 'amounts'},
    {'1': 'type_prefix', '3': 4, '4': 1, '5': 9, '10': 'typePrefix'},
  ],
};

@$core.Deprecated('Use messageDescriptor instead')
const Message_BridgeTransferOut$json = {
  '1': 'BridgeTransferOut',
  '2': [
    {'1': 'from_address', '3': 1, '4': 1, '5': 9, '10': 'fromAddress'},
    {'1': 'to_address', '3': 2, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'amount', '3': 3, '4': 1, '5': 11, '6': '.TW.Greenfield.Proto.Amount', '10': 'amount'},
    {'1': 'type_prefix', '3': 4, '4': 1, '5': 9, '10': 'typePrefix'},
  ],
};

/// Descriptor for `Message`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List messageDescriptor = $convert.base64Decode(
    'CgdNZXNzYWdlElEKEnNlbmRfY29pbnNfbWVzc2FnZRgBIAEoCzIhLlRXLkdyZWVuZmllbGQuUH'
    'JvdG8uTWVzc2FnZS5TZW5kSABSEHNlbmRDb2luc01lc3NhZ2USYAoTYnJpZGdlX3RyYW5zZmVy'
    'X291dBgCIAEoCzIuLlRXLkdyZWVuZmllbGQuUHJvdG8uTWVzc2FnZS5CcmlkZ2VUcmFuc2Zlck'
    '91dEgAUhFicmlkZ2VUcmFuc2Zlck91dBqgAQoEU2VuZBIhCgxmcm9tX2FkZHJlc3MYASABKAlS'
    'C2Zyb21BZGRyZXNzEh0KCnRvX2FkZHJlc3MYAiABKAlSCXRvQWRkcmVzcxI1CgdhbW91bnRzGA'
    'MgAygLMhsuVFcuR3JlZW5maWVsZC5Qcm90by5BbW91bnRSB2Ftb3VudHMSHwoLdHlwZV9wcmVm'
    'aXgYBCABKAlSCnR5cGVQcmVmaXgaqwEKEUJyaWRnZVRyYW5zZmVyT3V0EiEKDGZyb21fYWRkcm'
    'VzcxgBIAEoCVILZnJvbUFkZHJlc3MSHQoKdG9fYWRkcmVzcxgCIAEoCVIJdG9BZGRyZXNzEjMK'
    'BmFtb3VudBgDIAEoCzIbLlRXLkdyZWVuZmllbGQuUHJvdG8uQW1vdW50UgZhbW91bnQSHwoLdH'
    'lwZV9wcmVmaXgYBCABKAlSCnR5cGVQcmVmaXhCDwoNbWVzc2FnZV9vbmVvZg==');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'encoding_mode', '3': 1, '4': 1, '5': 14, '6': '.TW.Greenfield.Proto.EncodingMode', '10': 'encodingMode'},
    {'1': 'signing_mode', '3': 2, '4': 1, '5': 14, '6': '.TW.Greenfield.Proto.SigningMode', '10': 'signingMode'},
    {'1': 'account_number', '3': 3, '4': 1, '5': 4, '10': 'accountNumber'},
    {'1': 'eth_chain_id', '3': 4, '4': 1, '5': 9, '10': 'ethChainId'},
    {'1': 'cosmos_chain_id', '3': 5, '4': 1, '5': 9, '10': 'cosmosChainId'},
    {'1': 'fee', '3': 6, '4': 1, '5': 11, '6': '.TW.Greenfield.Proto.Fee', '10': 'fee'},
    {'1': 'memo', '3': 7, '4': 1, '5': 9, '10': 'memo'},
    {'1': 'sequence', '3': 8, '4': 1, '5': 4, '10': 'sequence'},
    {'1': 'private_key', '3': 9, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'messages', '3': 10, '4': 3, '5': 11, '6': '.TW.Greenfield.Proto.Message', '10': 'messages'},
    {'1': 'mode', '3': 11, '4': 1, '5': 14, '6': '.TW.Greenfield.Proto.BroadcastMode', '10': 'mode'},
    {'1': 'public_key', '3': 12, '4': 1, '5': 12, '10': 'publicKey'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSRgoNZW5jb2RpbmdfbW9kZRgBIAEoDjIhLlRXLkdyZWVuZmllbGQuUH'
    'JvdG8uRW5jb2RpbmdNb2RlUgxlbmNvZGluZ01vZGUSQwoMc2lnbmluZ19tb2RlGAIgASgOMiAu'
    'VFcuR3JlZW5maWVsZC5Qcm90by5TaWduaW5nTW9kZVILc2lnbmluZ01vZGUSJQoOYWNjb3VudF'
    '9udW1iZXIYAyABKARSDWFjY291bnROdW1iZXISIAoMZXRoX2NoYWluX2lkGAQgASgJUgpldGhD'
    'aGFpbklkEiYKD2Nvc21vc19jaGFpbl9pZBgFIAEoCVINY29zbW9zQ2hhaW5JZBIqCgNmZWUYBi'
    'ABKAsyGC5UVy5HcmVlbmZpZWxkLlByb3RvLkZlZVIDZmVlEhIKBG1lbW8YByABKAlSBG1lbW8S'
    'GgoIc2VxdWVuY2UYCCABKARSCHNlcXVlbmNlEh8KC3ByaXZhdGVfa2V5GAkgASgMUgpwcml2YX'
    'RlS2V5EjgKCG1lc3NhZ2VzGAogAygLMhwuVFcuR3JlZW5maWVsZC5Qcm90by5NZXNzYWdlUght'
    'ZXNzYWdlcxI2CgRtb2RlGAsgASgOMiIuVFcuR3JlZW5maWVsZC5Qcm90by5Ccm9hZGNhc3RNb2'
    'RlUgRtb2RlEh0KCnB1YmxpY19rZXkYDCABKAxSCXB1YmxpY0tleQ==');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'signature', '3': 1, '4': 1, '5': 12, '10': 'signature'},
    {'1': 'serialized', '3': 2, '4': 1, '5': 9, '10': 'serialized'},
    {'1': 'signature_json', '3': 3, '4': 1, '5': 9, '10': 'signatureJson'},
    {'1': 'error_message', '3': 4, '4': 1, '5': 9, '10': 'errorMessage'},
    {'1': 'error', '3': 5, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhwKCXNpZ25hdHVyZRgBIAEoDFIJc2lnbmF0dXJlEh4KCnNlcmlhbG'
    'l6ZWQYAiABKAlSCnNlcmlhbGl6ZWQSJQoOc2lnbmF0dXJlX2pzb24YAyABKAlSDXNpZ25hdHVy'
    'ZUpzb24SIwoNZXJyb3JfbWVzc2FnZRgEIAEoCVIMZXJyb3JNZXNzYWdlEjMKBWVycm9yGAUgAS'
    'gOMh0uVFcuQ29tbW9uLlByb3RvLlNpZ25pbmdFcnJvclIFZXJyb3I=');

