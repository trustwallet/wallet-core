//
//  Generated code. Do not modify.
//  source: Nebulas.proto
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
    {'1': 'chain_id', '3': 2, '4': 1, '5': 12, '10': 'chainId'},
    {'1': 'nonce', '3': 3, '4': 1, '5': 12, '10': 'nonce'},
    {'1': 'gas_price', '3': 4, '4': 1, '5': 12, '10': 'gasPrice'},
    {'1': 'gas_limit', '3': 5, '4': 1, '5': 12, '10': 'gasLimit'},
    {'1': 'to_address', '3': 6, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'amount', '3': 7, '4': 1, '5': 12, '10': 'amount'},
    {'1': 'timestamp', '3': 8, '4': 1, '5': 12, '10': 'timestamp'},
    {'1': 'payload', '3': 9, '4': 1, '5': 9, '10': 'payload'},
    {'1': 'private_key', '3': 10, '4': 1, '5': 12, '10': 'privateKey'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSIQoMZnJvbV9hZGRyZXNzGAEgASgJUgtmcm9tQWRkcmVzcxIZCghjaG'
    'Fpbl9pZBgCIAEoDFIHY2hhaW5JZBIUCgVub25jZRgDIAEoDFIFbm9uY2USGwoJZ2FzX3ByaWNl'
    'GAQgASgMUghnYXNQcmljZRIbCglnYXNfbGltaXQYBSABKAxSCGdhc0xpbWl0Eh0KCnRvX2FkZH'
    'Jlc3MYBiABKAlSCXRvQWRkcmVzcxIWCgZhbW91bnQYByABKAxSBmFtb3VudBIcCgl0aW1lc3Rh'
    'bXAYCCABKAxSCXRpbWVzdGFtcBIYCgdwYXlsb2FkGAkgASgJUgdwYXlsb2FkEh8KC3ByaXZhdG'
    'Vfa2V5GAogASgMUgpwcml2YXRlS2V5');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'algorithm', '3': 1, '4': 1, '5': 13, '10': 'algorithm'},
    {'1': 'signature', '3': 2, '4': 1, '5': 12, '10': 'signature'},
    {'1': 'raw', '3': 3, '4': 1, '5': 9, '10': 'raw'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhwKCWFsZ29yaXRobRgBIAEoDVIJYWxnb3JpdGhtEhwKCXNpZ25hdH'
    'VyZRgCIAEoDFIJc2lnbmF0dXJlEhAKA3JhdxgDIAEoCVIDcmF3');

@$core.Deprecated('Use dataDescriptor instead')
const Data$json = {
  '1': 'Data',
  '2': [
    {'1': 'type', '3': 1, '4': 1, '5': 9, '10': 'type'},
    {'1': 'payload', '3': 2, '4': 1, '5': 12, '10': 'payload'},
  ],
};

/// Descriptor for `Data`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List dataDescriptor = $convert.base64Decode(
    'CgREYXRhEhIKBHR5cGUYASABKAlSBHR5cGUSGAoHcGF5bG9hZBgCIAEoDFIHcGF5bG9hZA==');

@$core.Deprecated('Use rawTransactionDescriptor instead')
const RawTransaction$json = {
  '1': 'RawTransaction',
  '2': [
    {'1': 'hash', '3': 1, '4': 1, '5': 12, '10': 'hash'},
    {'1': 'from', '3': 2, '4': 1, '5': 12, '10': 'from'},
    {'1': 'to', '3': 3, '4': 1, '5': 12, '10': 'to'},
    {'1': 'value', '3': 4, '4': 1, '5': 12, '10': 'value'},
    {'1': 'nonce', '3': 5, '4': 1, '5': 4, '10': 'nonce'},
    {'1': 'timestamp', '3': 6, '4': 1, '5': 3, '10': 'timestamp'},
    {'1': 'data', '3': 7, '4': 1, '5': 11, '6': '.TW.Nebulas.Proto.Data', '10': 'data'},
    {'1': 'chain_id', '3': 8, '4': 1, '5': 13, '10': 'chainId'},
    {'1': 'gas_price', '3': 9, '4': 1, '5': 12, '10': 'gasPrice'},
    {'1': 'gas_limit', '3': 10, '4': 1, '5': 12, '10': 'gasLimit'},
    {'1': 'alg', '3': 11, '4': 1, '5': 13, '10': 'alg'},
    {'1': 'sign', '3': 12, '4': 1, '5': 12, '10': 'sign'},
  ],
};

/// Descriptor for `RawTransaction`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List rawTransactionDescriptor = $convert.base64Decode(
    'Cg5SYXdUcmFuc2FjdGlvbhISCgRoYXNoGAEgASgMUgRoYXNoEhIKBGZyb20YAiABKAxSBGZyb2'
    '0SDgoCdG8YAyABKAxSAnRvEhQKBXZhbHVlGAQgASgMUgV2YWx1ZRIUCgVub25jZRgFIAEoBFIF'
    'bm9uY2USHAoJdGltZXN0YW1wGAYgASgDUgl0aW1lc3RhbXASKgoEZGF0YRgHIAEoCzIWLlRXLk'
    '5lYnVsYXMuUHJvdG8uRGF0YVIEZGF0YRIZCghjaGFpbl9pZBgIIAEoDVIHY2hhaW5JZBIbCgln'
    'YXNfcHJpY2UYCSABKAxSCGdhc1ByaWNlEhsKCWdhc19saW1pdBgKIAEoDFIIZ2FzTGltaXQSEA'
    'oDYWxnGAsgASgNUgNhbGcSEgoEc2lnbhgMIAEoDFIEc2lnbg==');

