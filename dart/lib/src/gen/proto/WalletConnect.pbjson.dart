//
//  Generated code. Do not modify.
//  source: WalletConnect.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use protocolDescriptor instead')
const Protocol$json = {
  '1': 'Protocol',
  '2': [
    {'1': 'V2', '2': 0},
  ],
};

/// Descriptor for `Protocol`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List protocolDescriptor = $convert.base64Decode(
    'CghQcm90b2NvbBIGCgJWMhAA');

@$core.Deprecated('Use methodDescriptor instead')
const Method$json = {
  '1': 'Method',
  '2': [
    {'1': 'Unknown', '2': 0},
    {'1': 'CosmosSignAmino', '2': 1},
    {'1': 'SolanaSignTransaction', '2': 2},
  ],
};

/// Descriptor for `Method`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List methodDescriptor = $convert.base64Decode(
    'CgZNZXRob2QSCwoHVW5rbm93bhAAEhMKD0Nvc21vc1NpZ25BbWlubxABEhkKFVNvbGFuYVNpZ2'
    '5UcmFuc2FjdGlvbhAC');

@$core.Deprecated('Use parseRequestInputDescriptor instead')
const ParseRequestInput$json = {
  '1': 'ParseRequestInput',
  '2': [
    {'1': 'protocol', '3': 1, '4': 1, '5': 14, '6': '.TW.WalletConnect.Proto.Protocol', '10': 'protocol'},
    {'1': 'method', '3': 2, '4': 1, '5': 14, '6': '.TW.WalletConnect.Proto.Method', '10': 'method'},
    {'1': 'payload', '3': 3, '4': 1, '5': 9, '10': 'payload'},
  ],
};

/// Descriptor for `ParseRequestInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List parseRequestInputDescriptor = $convert.base64Decode(
    'ChFQYXJzZVJlcXVlc3RJbnB1dBI8Cghwcm90b2NvbBgBIAEoDjIgLlRXLldhbGxldENvbm5lY3'
    'QuUHJvdG8uUHJvdG9jb2xSCHByb3RvY29sEjYKBm1ldGhvZBgCIAEoDjIeLlRXLldhbGxldENv'
    'bm5lY3QuUHJvdG8uTWV0aG9kUgZtZXRob2QSGAoHcGF5bG9hZBgDIAEoCVIHcGF5bG9hZA==');

@$core.Deprecated('Use parseRequestOutputDescriptor instead')
const ParseRequestOutput$json = {
  '1': 'ParseRequestOutput',
  '2': [
    {'1': 'error', '3': 1, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 2, '4': 1, '5': 9, '10': 'errorMessage'},
    {'1': 'binance', '3': 3, '4': 1, '5': 11, '6': '.TW.Binance.Proto.SigningInput', '9': 0, '10': 'binance'},
    {'1': 'solana', '3': 4, '4': 1, '5': 11, '6': '.TW.Solana.Proto.SigningInput', '9': 0, '10': 'solana'},
  ],
  '8': [
    {'1': 'signing_input_oneof'},
  ],
};

/// Descriptor for `ParseRequestOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List parseRequestOutputDescriptor = $convert.base64Decode(
    'ChJQYXJzZVJlcXVlc3RPdXRwdXQSMwoFZXJyb3IYASABKA4yHS5UVy5Db21tb24uUHJvdG8uU2'
    'lnbmluZ0Vycm9yUgVlcnJvchIjCg1lcnJvcl9tZXNzYWdlGAIgASgJUgxlcnJvck1lc3NhZ2US'
    'OgoHYmluYW5jZRgDIAEoCzIeLlRXLkJpbmFuY2UuUHJvdG8uU2lnbmluZ0lucHV0SABSB2Jpbm'
    'FuY2USNwoGc29sYW5hGAQgASgLMh0uVFcuU29sYW5hLlByb3RvLlNpZ25pbmdJbnB1dEgAUgZz'
    'b2xhbmFCFQoTc2lnbmluZ19pbnB1dF9vbmVvZg==');

