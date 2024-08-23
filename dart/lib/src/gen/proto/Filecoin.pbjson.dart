//
//  Generated code. Do not modify.
//  source: Filecoin.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use derivationTypeDescriptor instead')
const DerivationType$json = {
  '1': 'DerivationType',
  '2': [
    {'1': 'SECP256K1', '2': 0},
    {'1': 'DELEGATED', '2': 1},
  ],
};

/// Descriptor for `DerivationType`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List derivationTypeDescriptor = $convert.base64Decode(
    'Cg5EZXJpdmF0aW9uVHlwZRINCglTRUNQMjU2SzEQABINCglERUxFR0FURUQQAQ==');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'private_key', '3': 1, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'to', '3': 2, '4': 1, '5': 9, '10': 'to'},
    {'1': 'nonce', '3': 3, '4': 1, '5': 4, '10': 'nonce'},
    {'1': 'value', '3': 4, '4': 1, '5': 12, '10': 'value'},
    {'1': 'gas_limit', '3': 5, '4': 1, '5': 3, '10': 'gasLimit'},
    {'1': 'gas_fee_cap', '3': 6, '4': 1, '5': 12, '10': 'gasFeeCap'},
    {'1': 'gas_premium', '3': 7, '4': 1, '5': 12, '10': 'gasPremium'},
    {'1': 'params', '3': 8, '4': 1, '5': 12, '10': 'params'},
    {'1': 'derivation', '3': 9, '4': 1, '5': 14, '6': '.TW.Filecoin.Proto.DerivationType', '10': 'derivation'},
    {'1': 'public_key', '3': 10, '4': 1, '5': 12, '10': 'publicKey'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSHwoLcHJpdmF0ZV9rZXkYASABKAxSCnByaXZhdGVLZXkSDgoCdG8YAi'
    'ABKAlSAnRvEhQKBW5vbmNlGAMgASgEUgVub25jZRIUCgV2YWx1ZRgEIAEoDFIFdmFsdWUSGwoJ'
    'Z2FzX2xpbWl0GAUgASgDUghnYXNMaW1pdBIeCgtnYXNfZmVlX2NhcBgGIAEoDFIJZ2FzRmVlQ2'
    'FwEh8KC2dhc19wcmVtaXVtGAcgASgMUgpnYXNQcmVtaXVtEhYKBnBhcmFtcxgIIAEoDFIGcGFy'
    'YW1zEkEKCmRlcml2YXRpb24YCSABKA4yIS5UVy5GaWxlY29pbi5Qcm90by5EZXJpdmF0aW9uVH'
    'lwZVIKZGVyaXZhdGlvbhIdCgpwdWJsaWNfa2V5GAogASgMUglwdWJsaWNLZXk=');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'json', '3': 1, '4': 1, '5': 9, '10': 'json'},
    {'1': 'error', '3': 2, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 3, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhIKBGpzb24YASABKAlSBGpzb24SMwoFZXJyb3IYAiABKA4yHS5UVy'
    '5Db21tb24uUHJvdG8uU2lnbmluZ0Vycm9yUgVlcnJvchIjCg1lcnJvcl9tZXNzYWdlGAMgASgJ'
    'UgxlcnJvck1lc3NhZ2U=');

