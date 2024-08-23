//
//  Generated code. Do not modify.
//  source: Everscale.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use messageBehaviorDescriptor instead')
const MessageBehavior$json = {
  '1': 'MessageBehavior',
  '2': [
    {'1': 'SimpleTransfer', '2': 0},
    {'1': 'SendAllBalance', '2': 1},
  ],
};

/// Descriptor for `MessageBehavior`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List messageBehaviorDescriptor = $convert.base64Decode(
    'Cg9NZXNzYWdlQmVoYXZpb3ISEgoOU2ltcGxlVHJhbnNmZXIQABISCg5TZW5kQWxsQmFsYW5jZR'
    'AB');

@$core.Deprecated('Use transferDescriptor instead')
const Transfer$json = {
  '1': 'Transfer',
  '2': [
    {'1': 'bounce', '3': 1, '4': 1, '5': 8, '10': 'bounce'},
    {'1': 'behavior', '3': 2, '4': 1, '5': 14, '6': '.TW.Everscale.Proto.MessageBehavior', '10': 'behavior'},
    {'1': 'amount', '3': 3, '4': 1, '5': 4, '10': 'amount'},
    {'1': 'expired_at', '3': 4, '4': 1, '5': 13, '10': 'expiredAt'},
    {'1': 'to', '3': 5, '4': 1, '5': 9, '10': 'to'},
    {'1': 'encoded_contract_data', '3': 6, '4': 1, '5': 9, '9': 0, '10': 'encodedContractData'},
  ],
  '8': [
    {'1': 'account_state_oneof'},
  ],
};

/// Descriptor for `Transfer`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transferDescriptor = $convert.base64Decode(
    'CghUcmFuc2ZlchIWCgZib3VuY2UYASABKAhSBmJvdW5jZRI/CghiZWhhdmlvchgCIAEoDjIjLl'
    'RXLkV2ZXJzY2FsZS5Qcm90by5NZXNzYWdlQmVoYXZpb3JSCGJlaGF2aW9yEhYKBmFtb3VudBgD'
    'IAEoBFIGYW1vdW50Eh0KCmV4cGlyZWRfYXQYBCABKA1SCWV4cGlyZWRBdBIOCgJ0bxgFIAEoCV'
    'ICdG8SNAoVZW5jb2RlZF9jb250cmFjdF9kYXRhGAYgASgJSABSE2VuY29kZWRDb250cmFjdERh'
    'dGFCFQoTYWNjb3VudF9zdGF0ZV9vbmVvZg==');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'transfer', '3': 1, '4': 1, '5': 11, '6': '.TW.Everscale.Proto.Transfer', '9': 0, '10': 'transfer'},
    {'1': 'private_key', '3': 2, '4': 1, '5': 12, '10': 'privateKey'},
  ],
  '8': [
    {'1': 'action_oneof'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSOgoIdHJhbnNmZXIYASABKAsyHC5UVy5FdmVyc2NhbGUuUHJvdG8uVH'
    'JhbnNmZXJIAFIIdHJhbnNmZXISHwoLcHJpdmF0ZV9rZXkYAiABKAxSCnByaXZhdGVLZXlCDgoM'
    'YWN0aW9uX29uZW9m');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 9, '10': 'encoded'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhgKB2VuY29kZWQYASABKAlSB2VuY29kZWQ=');

