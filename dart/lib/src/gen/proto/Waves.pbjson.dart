//
//  Generated code. Do not modify.
//  source: Waves.proto
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
    {'1': 'amount', '3': 1, '4': 1, '5': 3, '10': 'amount'},
    {'1': 'asset', '3': 2, '4': 1, '5': 9, '10': 'asset'},
    {'1': 'fee', '3': 3, '4': 1, '5': 3, '10': 'fee'},
    {'1': 'fee_asset', '3': 4, '4': 1, '5': 9, '10': 'feeAsset'},
    {'1': 'to', '3': 5, '4': 1, '5': 9, '10': 'to'},
    {'1': 'attachment', '3': 6, '4': 1, '5': 12, '10': 'attachment'},
  ],
};

/// Descriptor for `TransferMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transferMessageDescriptor = $convert.base64Decode(
    'Cg9UcmFuc2Zlck1lc3NhZ2USFgoGYW1vdW50GAEgASgDUgZhbW91bnQSFAoFYXNzZXQYAiABKA'
    'lSBWFzc2V0EhAKA2ZlZRgDIAEoA1IDZmVlEhsKCWZlZV9hc3NldBgEIAEoCVIIZmVlQXNzZXQS'
    'DgoCdG8YBSABKAlSAnRvEh4KCmF0dGFjaG1lbnQYBiABKAxSCmF0dGFjaG1lbnQ=');

@$core.Deprecated('Use leaseMessageDescriptor instead')
const LeaseMessage$json = {
  '1': 'LeaseMessage',
  '2': [
    {'1': 'amount', '3': 1, '4': 1, '5': 3, '10': 'amount'},
    {'1': 'to', '3': 2, '4': 1, '5': 9, '10': 'to'},
    {'1': 'fee', '3': 3, '4': 1, '5': 3, '10': 'fee'},
  ],
};

/// Descriptor for `LeaseMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List leaseMessageDescriptor = $convert.base64Decode(
    'CgxMZWFzZU1lc3NhZ2USFgoGYW1vdW50GAEgASgDUgZhbW91bnQSDgoCdG8YAiABKAlSAnRvEh'
    'AKA2ZlZRgDIAEoA1IDZmVl');

@$core.Deprecated('Use cancelLeaseMessageDescriptor instead')
const CancelLeaseMessage$json = {
  '1': 'CancelLeaseMessage',
  '2': [
    {'1': 'lease_id', '3': 1, '4': 1, '5': 9, '10': 'leaseId'},
    {'1': 'fee', '3': 2, '4': 1, '5': 3, '10': 'fee'},
  ],
};

/// Descriptor for `CancelLeaseMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List cancelLeaseMessageDescriptor = $convert.base64Decode(
    'ChJDYW5jZWxMZWFzZU1lc3NhZ2USGQoIbGVhc2VfaWQYASABKAlSB2xlYXNlSWQSEAoDZmVlGA'
    'IgASgDUgNmZWU=');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'timestamp', '3': 1, '4': 1, '5': 3, '10': 'timestamp'},
    {'1': 'private_key', '3': 2, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'transfer_message', '3': 3, '4': 1, '5': 11, '6': '.TW.Waves.Proto.TransferMessage', '9': 0, '10': 'transferMessage'},
    {'1': 'lease_message', '3': 4, '4': 1, '5': 11, '6': '.TW.Waves.Proto.LeaseMessage', '9': 0, '10': 'leaseMessage'},
    {'1': 'cancel_lease_message', '3': 5, '4': 1, '5': 11, '6': '.TW.Waves.Proto.CancelLeaseMessage', '9': 0, '10': 'cancelLeaseMessage'},
  ],
  '8': [
    {'1': 'message_oneof'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSHAoJdGltZXN0YW1wGAEgASgDUgl0aW1lc3RhbXASHwoLcHJpdmF0ZV'
    '9rZXkYAiABKAxSCnByaXZhdGVLZXkSTAoQdHJhbnNmZXJfbWVzc2FnZRgDIAEoCzIfLlRXLldh'
    'dmVzLlByb3RvLlRyYW5zZmVyTWVzc2FnZUgAUg90cmFuc2Zlck1lc3NhZ2USQwoNbGVhc2VfbW'
    'Vzc2FnZRgEIAEoCzIcLlRXLldhdmVzLlByb3RvLkxlYXNlTWVzc2FnZUgAUgxsZWFzZU1lc3Nh'
    'Z2USVgoUY2FuY2VsX2xlYXNlX21lc3NhZ2UYBSABKAsyIi5UVy5XYXZlcy5Qcm90by5DYW5jZW'
    'xMZWFzZU1lc3NhZ2VIAFISY2FuY2VsTGVhc2VNZXNzYWdlQg8KDW1lc3NhZ2Vfb25lb2Y=');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'signature', '3': 1, '4': 1, '5': 12, '10': 'signature'},
    {'1': 'json', '3': 2, '4': 1, '5': 9, '10': 'json'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhwKCXNpZ25hdHVyZRgBIAEoDFIJc2lnbmF0dXJlEhIKBGpzb24YAi'
    'ABKAlSBGpzb24=');

