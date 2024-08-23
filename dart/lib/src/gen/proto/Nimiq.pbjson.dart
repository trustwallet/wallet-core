//
//  Generated code. Do not modify.
//  source: Nimiq.proto
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
    {'1': 'private_key', '3': 1, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'destination', '3': 2, '4': 1, '5': 9, '10': 'destination'},
    {'1': 'value', '3': 3, '4': 1, '5': 4, '10': 'value'},
    {'1': 'fee', '3': 4, '4': 1, '5': 4, '10': 'fee'},
    {'1': 'validity_start_height', '3': 5, '4': 1, '5': 13, '10': 'validityStartHeight'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSHwoLcHJpdmF0ZV9rZXkYASABKAxSCnByaXZhdGVLZXkSIAoLZGVzdG'
    'luYXRpb24YAiABKAlSC2Rlc3RpbmF0aW9uEhQKBXZhbHVlGAMgASgEUgV2YWx1ZRIQCgNmZWUY'
    'BCABKARSA2ZlZRIyChV2YWxpZGl0eV9zdGFydF9oZWlnaHQYBSABKA1SE3ZhbGlkaXR5U3Rhcn'
    'RIZWlnaHQ=');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 12, '10': 'encoded'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhgKB2VuY29kZWQYASABKAxSB2VuY29kZWQ=');

