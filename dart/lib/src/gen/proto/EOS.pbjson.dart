//
//  Generated code. Do not modify.
//  source: EOS.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use keyTypeDescriptor instead')
const KeyType$json = {
  '1': 'KeyType',
  '2': [
    {'1': 'LEGACY', '2': 0},
    {'1': 'MODERNK1', '2': 1},
    {'1': 'MODERNR1', '2': 2},
  ],
};

/// Descriptor for `KeyType`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List keyTypeDescriptor = $convert.base64Decode(
    'CgdLZXlUeXBlEgoKBkxFR0FDWRAAEgwKCE1PREVSTksxEAESDAoITU9ERVJOUjEQAg==');

@$core.Deprecated('Use assetDescriptor instead')
const Asset$json = {
  '1': 'Asset',
  '2': [
    {'1': 'amount', '3': 1, '4': 1, '5': 3, '10': 'amount'},
    {'1': 'decimals', '3': 2, '4': 1, '5': 13, '10': 'decimals'},
    {'1': 'symbol', '3': 3, '4': 1, '5': 9, '10': 'symbol'},
  ],
};

/// Descriptor for `Asset`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List assetDescriptor = $convert.base64Decode(
    'CgVBc3NldBIWCgZhbW91bnQYASABKANSBmFtb3VudBIaCghkZWNpbWFscxgCIAEoDVIIZGVjaW'
    '1hbHMSFgoGc3ltYm9sGAMgASgJUgZzeW1ib2w=');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'chain_id', '3': 1, '4': 1, '5': 12, '10': 'chainId'},
    {'1': 'reference_block_id', '3': 2, '4': 1, '5': 12, '10': 'referenceBlockId'},
    {'1': 'reference_block_time', '3': 3, '4': 1, '5': 15, '10': 'referenceBlockTime'},
    {'1': 'currency', '3': 4, '4': 1, '5': 9, '10': 'currency'},
    {'1': 'sender', '3': 5, '4': 1, '5': 9, '10': 'sender'},
    {'1': 'recipient', '3': 6, '4': 1, '5': 9, '10': 'recipient'},
    {'1': 'memo', '3': 7, '4': 1, '5': 9, '10': 'memo'},
    {'1': 'asset', '3': 8, '4': 1, '5': 11, '6': '.TW.EOS.Proto.Asset', '10': 'asset'},
    {'1': 'private_key', '3': 9, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'private_key_type', '3': 10, '4': 1, '5': 14, '6': '.TW.EOS.Proto.KeyType', '10': 'privateKeyType'},
    {'1': 'expiration', '3': 11, '4': 1, '5': 15, '10': 'expiration'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSGQoIY2hhaW5faWQYASABKAxSB2NoYWluSWQSLAoScmVmZXJlbmNlX2'
    'Jsb2NrX2lkGAIgASgMUhByZWZlcmVuY2VCbG9ja0lkEjAKFHJlZmVyZW5jZV9ibG9ja190aW1l'
    'GAMgASgPUhJyZWZlcmVuY2VCbG9ja1RpbWUSGgoIY3VycmVuY3kYBCABKAlSCGN1cnJlbmN5Eh'
    'YKBnNlbmRlchgFIAEoCVIGc2VuZGVyEhwKCXJlY2lwaWVudBgGIAEoCVIJcmVjaXBpZW50EhIK'
    'BG1lbW8YByABKAlSBG1lbW8SKQoFYXNzZXQYCCABKAsyEy5UVy5FT1MuUHJvdG8uQXNzZXRSBW'
    'Fzc2V0Eh8KC3ByaXZhdGVfa2V5GAkgASgMUgpwcml2YXRlS2V5Ej8KEHByaXZhdGVfa2V5X3R5'
    'cGUYCiABKA4yFS5UVy5FT1MuUHJvdG8uS2V5VHlwZVIOcHJpdmF0ZUtleVR5cGUSHgoKZXhwaX'
    'JhdGlvbhgLIAEoD1IKZXhwaXJhdGlvbg==');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'json_encoded', '3': 1, '4': 1, '5': 9, '10': 'jsonEncoded'},
    {'1': 'error', '3': 2, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 3, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EiEKDGpzb25fZW5jb2RlZBgBIAEoCVILanNvbkVuY29kZWQSMwoFZX'
    'Jyb3IYAiABKA4yHS5UVy5Db21tb24uUHJvdG8uU2lnbmluZ0Vycm9yUgVlcnJvchIjCg1lcnJv'
    'cl9tZXNzYWdlGAMgASgJUgxlcnJvck1lc3NhZ2U=');

