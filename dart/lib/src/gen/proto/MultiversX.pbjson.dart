//
//  Generated code. Do not modify.
//  source: MultiversX.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use genericActionDescriptor instead')
const GenericAction$json = {
  '1': 'GenericAction',
  '2': [
    {'1': 'accounts', '3': 1, '4': 1, '5': 11, '6': '.TW.MultiversX.Proto.Accounts', '10': 'accounts'},
    {'1': 'value', '3': 2, '4': 1, '5': 9, '10': 'value'},
    {'1': 'data', '3': 3, '4': 1, '5': 9, '10': 'data'},
    {'1': 'version', '3': 4, '4': 1, '5': 13, '10': 'version'},
    {'1': 'options', '3': 5, '4': 1, '5': 13, '10': 'options'},
  ],
};

/// Descriptor for `GenericAction`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List genericActionDescriptor = $convert.base64Decode(
    'Cg1HZW5lcmljQWN0aW9uEjkKCGFjY291bnRzGAEgASgLMh0uVFcuTXVsdGl2ZXJzWC5Qcm90by'
    '5BY2NvdW50c1IIYWNjb3VudHMSFAoFdmFsdWUYAiABKAlSBXZhbHVlEhIKBGRhdGEYAyABKAlS'
    'BGRhdGESGAoHdmVyc2lvbhgEIAEoDVIHdmVyc2lvbhIYCgdvcHRpb25zGAUgASgNUgdvcHRpb2'
    '5z');

@$core.Deprecated('Use eGLDTransferDescriptor instead')
const EGLDTransfer$json = {
  '1': 'EGLDTransfer',
  '2': [
    {'1': 'accounts', '3': 1, '4': 1, '5': 11, '6': '.TW.MultiversX.Proto.Accounts', '10': 'accounts'},
    {'1': 'amount', '3': 2, '4': 1, '5': 9, '10': 'amount'},
    {'1': 'data', '3': 3, '4': 1, '5': 9, '10': 'data'},
    {'1': 'version', '3': 4, '4': 1, '5': 13, '10': 'version'},
  ],
};

/// Descriptor for `EGLDTransfer`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List eGLDTransferDescriptor = $convert.base64Decode(
    'CgxFR0xEVHJhbnNmZXISOQoIYWNjb3VudHMYASABKAsyHS5UVy5NdWx0aXZlcnNYLlByb3RvLk'
    'FjY291bnRzUghhY2NvdW50cxIWCgZhbW91bnQYAiABKAlSBmFtb3VudBISCgRkYXRhGAMgASgJ'
    'UgRkYXRhEhgKB3ZlcnNpb24YBCABKA1SB3ZlcnNpb24=');

@$core.Deprecated('Use eSDTTransferDescriptor instead')
const ESDTTransfer$json = {
  '1': 'ESDTTransfer',
  '2': [
    {'1': 'accounts', '3': 1, '4': 1, '5': 11, '6': '.TW.MultiversX.Proto.Accounts', '10': 'accounts'},
    {'1': 'token_identifier', '3': 2, '4': 1, '5': 9, '10': 'tokenIdentifier'},
    {'1': 'amount', '3': 3, '4': 1, '5': 9, '10': 'amount'},
    {'1': 'version', '3': 4, '4': 1, '5': 13, '10': 'version'},
  ],
};

/// Descriptor for `ESDTTransfer`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List eSDTTransferDescriptor = $convert.base64Decode(
    'CgxFU0RUVHJhbnNmZXISOQoIYWNjb3VudHMYASABKAsyHS5UVy5NdWx0aXZlcnNYLlByb3RvLk'
    'FjY291bnRzUghhY2NvdW50cxIpChB0b2tlbl9pZGVudGlmaWVyGAIgASgJUg90b2tlbklkZW50'
    'aWZpZXISFgoGYW1vdW50GAMgASgJUgZhbW91bnQSGAoHdmVyc2lvbhgEIAEoDVIHdmVyc2lvbg'
    '==');

@$core.Deprecated('Use eSDTNFTTransferDescriptor instead')
const ESDTNFTTransfer$json = {
  '1': 'ESDTNFTTransfer',
  '2': [
    {'1': 'accounts', '3': 1, '4': 1, '5': 11, '6': '.TW.MultiversX.Proto.Accounts', '10': 'accounts'},
    {'1': 'token_collection', '3': 2, '4': 1, '5': 9, '10': 'tokenCollection'},
    {'1': 'token_nonce', '3': 3, '4': 1, '5': 4, '10': 'tokenNonce'},
    {'1': 'amount', '3': 4, '4': 1, '5': 9, '10': 'amount'},
    {'1': 'version', '3': 5, '4': 1, '5': 13, '10': 'version'},
  ],
};

/// Descriptor for `ESDTNFTTransfer`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List eSDTNFTTransferDescriptor = $convert.base64Decode(
    'Cg9FU0RUTkZUVHJhbnNmZXISOQoIYWNjb3VudHMYASABKAsyHS5UVy5NdWx0aXZlcnNYLlByb3'
    'RvLkFjY291bnRzUghhY2NvdW50cxIpChB0b2tlbl9jb2xsZWN0aW9uGAIgASgJUg90b2tlbkNv'
    'bGxlY3Rpb24SHwoLdG9rZW5fbm9uY2UYAyABKARSCnRva2VuTm9uY2USFgoGYW1vdW50GAQgAS'
    'gJUgZhbW91bnQSGAoHdmVyc2lvbhgFIAEoDVIHdmVyc2lvbg==');

@$core.Deprecated('Use accountsDescriptor instead')
const Accounts$json = {
  '1': 'Accounts',
  '2': [
    {'1': 'sender_nonce', '3': 1, '4': 1, '5': 4, '10': 'senderNonce'},
    {'1': 'sender', '3': 2, '4': 1, '5': 9, '10': 'sender'},
    {'1': 'sender_username', '3': 3, '4': 1, '5': 9, '10': 'senderUsername'},
    {'1': 'receiver', '3': 4, '4': 1, '5': 9, '10': 'receiver'},
    {'1': 'receiver_username', '3': 5, '4': 1, '5': 9, '10': 'receiverUsername'},
    {'1': 'guardian', '3': 6, '4': 1, '5': 9, '10': 'guardian'},
  ],
};

/// Descriptor for `Accounts`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List accountsDescriptor = $convert.base64Decode(
    'CghBY2NvdW50cxIhCgxzZW5kZXJfbm9uY2UYASABKARSC3NlbmRlck5vbmNlEhYKBnNlbmRlch'
    'gCIAEoCVIGc2VuZGVyEicKD3NlbmRlcl91c2VybmFtZRgDIAEoCVIOc2VuZGVyVXNlcm5hbWUS'
    'GgoIcmVjZWl2ZXIYBCABKAlSCHJlY2VpdmVyEisKEXJlY2VpdmVyX3VzZXJuYW1lGAUgASgJUh'
    'ByZWNlaXZlclVzZXJuYW1lEhoKCGd1YXJkaWFuGAYgASgJUghndWFyZGlhbg==');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'private_key', '3': 1, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'chain_id', '3': 2, '4': 1, '5': 9, '10': 'chainId'},
    {'1': 'gas_price', '3': 3, '4': 1, '5': 4, '10': 'gasPrice'},
    {'1': 'gas_limit', '3': 4, '4': 1, '5': 4, '10': 'gasLimit'},
    {'1': 'generic_action', '3': 5, '4': 1, '5': 11, '6': '.TW.MultiversX.Proto.GenericAction', '9': 0, '10': 'genericAction'},
    {'1': 'egld_transfer', '3': 6, '4': 1, '5': 11, '6': '.TW.MultiversX.Proto.EGLDTransfer', '9': 0, '10': 'egldTransfer'},
    {'1': 'esdt_transfer', '3': 7, '4': 1, '5': 11, '6': '.TW.MultiversX.Proto.ESDTTransfer', '9': 0, '10': 'esdtTransfer'},
    {'1': 'esdtnft_transfer', '3': 8, '4': 1, '5': 11, '6': '.TW.MultiversX.Proto.ESDTNFTTransfer', '9': 0, '10': 'esdtnftTransfer'},
  ],
  '8': [
    {'1': 'message_oneof'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSHwoLcHJpdmF0ZV9rZXkYASABKAxSCnByaXZhdGVLZXkSGQoIY2hhaW'
    '5faWQYAiABKAlSB2NoYWluSWQSGwoJZ2FzX3ByaWNlGAMgASgEUghnYXNQcmljZRIbCglnYXNf'
    'bGltaXQYBCABKARSCGdhc0xpbWl0EksKDmdlbmVyaWNfYWN0aW9uGAUgASgLMiIuVFcuTXVsdG'
    'l2ZXJzWC5Qcm90by5HZW5lcmljQWN0aW9uSABSDWdlbmVyaWNBY3Rpb24SSAoNZWdsZF90cmFu'
    'c2ZlchgGIAEoCzIhLlRXLk11bHRpdmVyc1guUHJvdG8uRUdMRFRyYW5zZmVySABSDGVnbGRUcm'
    'Fuc2ZlchJICg1lc2R0X3RyYW5zZmVyGAcgASgLMiEuVFcuTXVsdGl2ZXJzWC5Qcm90by5FU0RU'
    'VHJhbnNmZXJIAFIMZXNkdFRyYW5zZmVyElEKEGVzZHRuZnRfdHJhbnNmZXIYCCABKAsyJC5UVy'
    '5NdWx0aXZlcnNYLlByb3RvLkVTRFRORlRUcmFuc2ZlckgAUg9lc2R0bmZ0VHJhbnNmZXJCDwoN'
    'bWVzc2FnZV9vbmVvZg==');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 9, '10': 'encoded'},
    {'1': 'signature', '3': 2, '4': 1, '5': 9, '10': 'signature'},
    {'1': 'error', '3': 3, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 4, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhgKB2VuY29kZWQYASABKAlSB2VuY29kZWQSHAoJc2lnbmF0dXJlGA'
    'IgASgJUglzaWduYXR1cmUSMwoFZXJyb3IYAyABKA4yHS5UVy5Db21tb24uUHJvdG8uU2lnbmlu'
    'Z0Vycm9yUgVlcnJvchIjCg1lcnJvcl9tZXNzYWdlGAQgASgJUgxlcnJvck1lc3NhZ2U=');

