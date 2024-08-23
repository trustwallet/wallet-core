//
//  Generated code. Do not modify.
//  source: Icon.proto
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
    {'1': 'value', '3': 3, '4': 1, '5': 12, '10': 'value'},
    {'1': 'step_limit', '3': 4, '4': 1, '5': 12, '10': 'stepLimit'},
    {'1': 'timestamp', '3': 5, '4': 1, '5': 3, '10': 'timestamp'},
    {'1': 'nonce', '3': 6, '4': 1, '5': 12, '10': 'nonce'},
    {'1': 'network_id', '3': 7, '4': 1, '5': 12, '10': 'networkId'},
    {'1': 'private_key', '3': 8, '4': 1, '5': 12, '10': 'privateKey'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSIQoMZnJvbV9hZGRyZXNzGAEgASgJUgtmcm9tQWRkcmVzcxIdCgp0b1'
    '9hZGRyZXNzGAIgASgJUgl0b0FkZHJlc3MSFAoFdmFsdWUYAyABKAxSBXZhbHVlEh0KCnN0ZXBf'
    'bGltaXQYBCABKAxSCXN0ZXBMaW1pdBIcCgl0aW1lc3RhbXAYBSABKANSCXRpbWVzdGFtcBIUCg'
    'Vub25jZRgGIAEoDFIFbm9uY2USHQoKbmV0d29ya19pZBgHIAEoDFIJbmV0d29ya0lkEh8KC3By'
    'aXZhdGVfa2V5GAggASgMUgpwcml2YXRlS2V5');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 9, '10': 'encoded'},
    {'1': 'signature', '3': 2, '4': 1, '5': 12, '10': 'signature'},
    {'1': 'error_message', '3': 3, '4': 1, '5': 9, '10': 'errorMessage'},
    {'1': 'error', '3': 4, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhgKB2VuY29kZWQYASABKAlSB2VuY29kZWQSHAoJc2lnbmF0dXJlGA'
    'IgASgMUglzaWduYXR1cmUSIwoNZXJyb3JfbWVzc2FnZRgDIAEoCVIMZXJyb3JNZXNzYWdlEjMK'
    'BWVycm9yGAQgASgOMh0uVFcuQ29tbW9uLlByb3RvLlNpZ25pbmdFcnJvclIFZXJyb3I=');

