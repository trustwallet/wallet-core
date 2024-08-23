//
//  Generated code. Do not modify.
//  source: Ontology.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'contract', '3': 1, '4': 1, '5': 9, '10': 'contract'},
    {'1': 'method', '3': 2, '4': 1, '5': 9, '10': 'method'},
    {'1': 'owner_private_key', '3': 3, '4': 1, '5': 12, '10': 'ownerPrivateKey'},
    {'1': 'to_address', '3': 4, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'amount', '3': 5, '4': 1, '5': 4, '10': 'amount'},
    {'1': 'payer_private_key', '3': 6, '4': 1, '5': 12, '10': 'payerPrivateKey'},
    {'1': 'gas_price', '3': 7, '4': 1, '5': 4, '10': 'gasPrice'},
    {'1': 'gas_limit', '3': 8, '4': 1, '5': 4, '10': 'gasLimit'},
    {'1': 'query_address', '3': 9, '4': 1, '5': 9, '10': 'queryAddress'},
    {'1': 'nonce', '3': 10, '4': 1, '5': 13, '10': 'nonce'},
    {'1': 'owner_address', '3': 11, '4': 1, '5': 9, '10': 'ownerAddress'},
    {'1': 'payer_address', '3': 12, '4': 1, '5': 9, '10': 'payerAddress'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSGgoIY29udHJhY3QYASABKAlSCGNvbnRyYWN0EhYKBm1ldGhvZBgCIA'
    'EoCVIGbWV0aG9kEioKEW93bmVyX3ByaXZhdGVfa2V5GAMgASgMUg9vd25lclByaXZhdGVLZXkS'
    'HQoKdG9fYWRkcmVzcxgEIAEoCVIJdG9BZGRyZXNzEhYKBmFtb3VudBgFIAEoBFIGYW1vdW50Ei'
    'oKEXBheWVyX3ByaXZhdGVfa2V5GAYgASgMUg9wYXllclByaXZhdGVLZXkSGwoJZ2FzX3ByaWNl'
    'GAcgASgEUghnYXNQcmljZRIbCglnYXNfbGltaXQYCCABKARSCGdhc0xpbWl0EiMKDXF1ZXJ5X2'
    'FkZHJlc3MYCSABKAlSDHF1ZXJ5QWRkcmVzcxIUCgVub25jZRgKIAEoDVIFbm9uY2USIwoNb3du'
    'ZXJfYWRkcmVzcxgLIAEoCVIMb3duZXJBZGRyZXNzEiMKDXBheWVyX2FkZHJlc3MYDCABKAlSDH'
    'BheWVyQWRkcmVzcw==');

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

