//
//  Generated code. Do not modify.
//  source: Nano.proto
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
    {'1': 'parent_block', '3': 2, '4': 1, '5': 12, '10': 'parentBlock'},
    {'1': 'link_block', '3': 3, '4': 1, '5': 12, '9': 0, '10': 'linkBlock'},
    {'1': 'link_recipient', '3': 4, '4': 1, '5': 9, '9': 0, '10': 'linkRecipient'},
    {'1': 'representative', '3': 5, '4': 1, '5': 9, '10': 'representative'},
    {'1': 'balance', '3': 6, '4': 1, '5': 9, '10': 'balance'},
    {'1': 'work', '3': 7, '4': 1, '5': 9, '10': 'work'},
    {'1': 'public_key', '3': 8, '4': 1, '5': 12, '10': 'publicKey'},
  ],
  '8': [
    {'1': 'link_oneof'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSHwoLcHJpdmF0ZV9rZXkYASABKAxSCnByaXZhdGVLZXkSIQoMcGFyZW'
    '50X2Jsb2NrGAIgASgMUgtwYXJlbnRCbG9jaxIfCgpsaW5rX2Jsb2NrGAMgASgMSABSCWxpbmtC'
    'bG9jaxInCg5saW5rX3JlY2lwaWVudBgEIAEoCUgAUg1saW5rUmVjaXBpZW50EiYKDnJlcHJlc2'
    'VudGF0aXZlGAUgASgJUg5yZXByZXNlbnRhdGl2ZRIYCgdiYWxhbmNlGAYgASgJUgdiYWxhbmNl'
    'EhIKBHdvcmsYByABKAlSBHdvcmsSHQoKcHVibGljX2tleRgIIAEoDFIJcHVibGljS2V5QgwKCm'
    'xpbmtfb25lb2Y=');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'signature', '3': 1, '4': 1, '5': 12, '10': 'signature'},
    {'1': 'block_hash', '3': 2, '4': 1, '5': 12, '10': 'blockHash'},
    {'1': 'json', '3': 3, '4': 1, '5': 9, '10': 'json'},
    {'1': 'error', '3': 4, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 5, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhwKCXNpZ25hdHVyZRgBIAEoDFIJc2lnbmF0dXJlEh0KCmJsb2NrX2'
    'hhc2gYAiABKAxSCWJsb2NrSGFzaBISCgRqc29uGAMgASgJUgRqc29uEjMKBWVycm9yGAQgASgO'
    'Mh0uVFcuQ29tbW9uLlByb3RvLlNpZ25pbmdFcnJvclIFZXJyb3ISIwoNZXJyb3JfbWVzc2FnZR'
    'gFIAEoCVIMZXJyb3JNZXNzYWdl');

