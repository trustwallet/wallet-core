//
//  Generated code. Do not modify.
//  source: Aeternity.proto
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
    {'1': 'from_address', '3': 1, '4': 1, '5': 9, '10': 'fromAddress'},
    {'1': 'to_address', '3': 2, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'amount', '3': 3, '4': 1, '5': 12, '10': 'amount'},
    {'1': 'fee', '3': 4, '4': 1, '5': 12, '10': 'fee'},
    {'1': 'payload', '3': 5, '4': 1, '5': 9, '10': 'payload'},
    {'1': 'ttl', '3': 6, '4': 1, '5': 4, '10': 'ttl'},
    {'1': 'nonce', '3': 7, '4': 1, '5': 4, '10': 'nonce'},
    {'1': 'private_key', '3': 8, '4': 1, '5': 12, '10': 'privateKey'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSIQoMZnJvbV9hZGRyZXNzGAEgASgJUgtmcm9tQWRkcmVzcxIdCgp0b1'
    '9hZGRyZXNzGAIgASgJUgl0b0FkZHJlc3MSFgoGYW1vdW50GAMgASgMUgZhbW91bnQSEAoDZmVl'
    'GAQgASgMUgNmZWUSGAoHcGF5bG9hZBgFIAEoCVIHcGF5bG9hZBIQCgN0dGwYBiABKARSA3R0bB'
    'IUCgVub25jZRgHIAEoBFIFbm9uY2USHwoLcHJpdmF0ZV9rZXkYCCABKAxSCnByaXZhdGVLZXk=');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 9, '10': 'encoded'},
    {'1': 'signature', '3': 2, '4': 1, '5': 9, '10': 'signature'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhgKB2VuY29kZWQYASABKAlSB2VuY29kZWQSHAoJc2lnbmF0dXJlGA'
    'IgASgJUglzaWduYXR1cmU=');

