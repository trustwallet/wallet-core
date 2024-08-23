//
//  Generated code. Do not modify.
//  source: VeChain.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use clauseDescriptor instead')
const Clause$json = {
  '1': 'Clause',
  '2': [
    {'1': 'to', '3': 1, '4': 1, '5': 9, '10': 'to'},
    {'1': 'value', '3': 2, '4': 1, '5': 12, '10': 'value'},
    {'1': 'data', '3': 3, '4': 1, '5': 12, '10': 'data'},
  ],
};

/// Descriptor for `Clause`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List clauseDescriptor = $convert.base64Decode(
    'CgZDbGF1c2USDgoCdG8YASABKAlSAnRvEhQKBXZhbHVlGAIgASgMUgV2YWx1ZRISCgRkYXRhGA'
    'MgASgMUgRkYXRh');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'chain_tag', '3': 1, '4': 1, '5': 13, '10': 'chainTag'},
    {'1': 'block_ref', '3': 2, '4': 1, '5': 4, '10': 'blockRef'},
    {'1': 'expiration', '3': 3, '4': 1, '5': 13, '10': 'expiration'},
    {'1': 'clauses', '3': 4, '4': 3, '5': 11, '6': '.TW.VeChain.Proto.Clause', '10': 'clauses'},
    {'1': 'gas_price_coef', '3': 5, '4': 1, '5': 13, '10': 'gasPriceCoef'},
    {'1': 'gas', '3': 6, '4': 1, '5': 4, '10': 'gas'},
    {'1': 'depends_on', '3': 7, '4': 1, '5': 12, '10': 'dependsOn'},
    {'1': 'nonce', '3': 8, '4': 1, '5': 4, '10': 'nonce'},
    {'1': 'private_key', '3': 9, '4': 1, '5': 12, '10': 'privateKey'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSGwoJY2hhaW5fdGFnGAEgASgNUghjaGFpblRhZxIbCglibG9ja19yZW'
    'YYAiABKARSCGJsb2NrUmVmEh4KCmV4cGlyYXRpb24YAyABKA1SCmV4cGlyYXRpb24SMgoHY2xh'
    'dXNlcxgEIAMoCzIYLlRXLlZlQ2hhaW4uUHJvdG8uQ2xhdXNlUgdjbGF1c2VzEiQKDmdhc19wcm'
    'ljZV9jb2VmGAUgASgNUgxnYXNQcmljZUNvZWYSEAoDZ2FzGAYgASgEUgNnYXMSHQoKZGVwZW5k'
    'c19vbhgHIAEoDFIJZGVwZW5kc09uEhQKBW5vbmNlGAggASgEUgVub25jZRIfCgtwcml2YXRlX2'
    'tleRgJIAEoDFIKcHJpdmF0ZUtleQ==');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'signature', '3': 2, '4': 1, '5': 12, '10': 'signature'},
    {'1': 'error', '3': 3, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 4, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhgKB2VuY29kZWQYASABKAxSB2VuY29kZWQSHAoJc2lnbmF0dXJlGA'
    'IgASgMUglzaWduYXR1cmUSMwoFZXJyb3IYAyABKA4yHS5UVy5Db21tb24uUHJvdG8uU2lnbmlu'
    'Z0Vycm9yUgVlcnJvchIjCg1lcnJvcl9tZXNzYWdlGAQgASgJUgxlcnJvck1lc3NhZ2U=');

