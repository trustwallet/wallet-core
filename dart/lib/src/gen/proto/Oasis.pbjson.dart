//
//  Generated code. Do not modify.
//  source: Oasis.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use transferMessageDescriptor instead')
const TransferMessage$json = {
  '1': 'TransferMessage',
  '2': [
    {'1': 'to', '3': 1, '4': 1, '5': 9, '10': 'to'},
    {'1': 'gas_price', '3': 2, '4': 1, '5': 4, '10': 'gasPrice'},
    {'1': 'gas_amount', '3': 3, '4': 1, '5': 9, '10': 'gasAmount'},
    {'1': 'amount', '3': 4, '4': 1, '5': 9, '10': 'amount'},
    {'1': 'nonce', '3': 5, '4': 1, '5': 4, '10': 'nonce'},
    {'1': 'context', '3': 6, '4': 1, '5': 9, '10': 'context'},
  ],
};

/// Descriptor for `TransferMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transferMessageDescriptor = $convert.base64Decode(
    'Cg9UcmFuc2Zlck1lc3NhZ2USDgoCdG8YASABKAlSAnRvEhsKCWdhc19wcmljZRgCIAEoBFIIZ2'
    'FzUHJpY2USHQoKZ2FzX2Ftb3VudBgDIAEoCVIJZ2FzQW1vdW50EhYKBmFtb3VudBgEIAEoCVIG'
    'YW1vdW50EhQKBW5vbmNlGAUgASgEUgVub25jZRIYCgdjb250ZXh0GAYgASgJUgdjb250ZXh0');

@$core.Deprecated('Use escrowMessageDescriptor instead')
const EscrowMessage$json = {
  '1': 'EscrowMessage',
  '2': [
    {'1': 'gas_price', '3': 1, '4': 1, '5': 4, '10': 'gasPrice'},
    {'1': 'gas_amount', '3': 2, '4': 1, '5': 9, '10': 'gasAmount'},
    {'1': 'nonce', '3': 3, '4': 1, '5': 4, '10': 'nonce'},
    {'1': 'account', '3': 4, '4': 1, '5': 9, '10': 'account'},
    {'1': 'amount', '3': 5, '4': 1, '5': 9, '10': 'amount'},
    {'1': 'context', '3': 6, '4': 1, '5': 9, '10': 'context'},
  ],
};

/// Descriptor for `EscrowMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List escrowMessageDescriptor = $convert.base64Decode(
    'Cg1Fc2Nyb3dNZXNzYWdlEhsKCWdhc19wcmljZRgBIAEoBFIIZ2FzUHJpY2USHQoKZ2FzX2Ftb3'
    'VudBgCIAEoCVIJZ2FzQW1vdW50EhQKBW5vbmNlGAMgASgEUgVub25jZRIYCgdhY2NvdW50GAQg'
    'ASgJUgdhY2NvdW50EhYKBmFtb3VudBgFIAEoCVIGYW1vdW50EhgKB2NvbnRleHQYBiABKAlSB2'
    'NvbnRleHQ=');

@$core.Deprecated('Use reclaimEscrowMessageDescriptor instead')
const ReclaimEscrowMessage$json = {
  '1': 'ReclaimEscrowMessage',
  '2': [
    {'1': 'gas_price', '3': 1, '4': 1, '5': 4, '10': 'gasPrice'},
    {'1': 'gas_amount', '3': 2, '4': 1, '5': 9, '10': 'gasAmount'},
    {'1': 'nonce', '3': 3, '4': 1, '5': 4, '10': 'nonce'},
    {'1': 'account', '3': 4, '4': 1, '5': 9, '10': 'account'},
    {'1': 'shares', '3': 5, '4': 1, '5': 9, '10': 'shares'},
    {'1': 'context', '3': 6, '4': 1, '5': 9, '10': 'context'},
  ],
};

/// Descriptor for `ReclaimEscrowMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List reclaimEscrowMessageDescriptor = $convert.base64Decode(
    'ChRSZWNsYWltRXNjcm93TWVzc2FnZRIbCglnYXNfcHJpY2UYASABKARSCGdhc1ByaWNlEh0KCm'
    'dhc19hbW91bnQYAiABKAlSCWdhc0Ftb3VudBIUCgVub25jZRgDIAEoBFIFbm9uY2USGAoHYWNj'
    'b3VudBgEIAEoCVIHYWNjb3VudBIWCgZzaGFyZXMYBSABKAlSBnNoYXJlcxIYCgdjb250ZXh0GA'
    'YgASgJUgdjb250ZXh0');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'private_key', '3': 1, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'transfer', '3': 2, '4': 1, '5': 11, '6': '.TW.Oasis.Proto.TransferMessage', '9': 0, '10': 'transfer'},
    {'1': 'escrow', '3': 3, '4': 1, '5': 11, '6': '.TW.Oasis.Proto.EscrowMessage', '9': 0, '10': 'escrow'},
    {'1': 'reclaimEscrow', '3': 4, '4': 1, '5': 11, '6': '.TW.Oasis.Proto.ReclaimEscrowMessage', '9': 0, '10': 'reclaimEscrow'},
  ],
  '8': [
    {'1': 'message'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSHwoLcHJpdmF0ZV9rZXkYASABKAxSCnByaXZhdGVLZXkSPQoIdHJhbn'
    'NmZXIYAiABKAsyHy5UVy5PYXNpcy5Qcm90by5UcmFuc2Zlck1lc3NhZ2VIAFIIdHJhbnNmZXIS'
    'NwoGZXNjcm93GAMgASgLMh0uVFcuT2FzaXMuUHJvdG8uRXNjcm93TWVzc2FnZUgAUgZlc2Nyb3'
    'cSTAoNcmVjbGFpbUVzY3JvdxgEIAEoCzIkLlRXLk9hc2lzLlByb3RvLlJlY2xhaW1Fc2Nyb3dN'
    'ZXNzYWdlSABSDXJlY2xhaW1Fc2Nyb3dCCQoHbWVzc2FnZQ==');

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

