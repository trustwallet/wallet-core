//
//  Generated code. Do not modify.
//  source: Algorand.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use transferDescriptor instead')
const Transfer$json = {
  '1': 'Transfer',
  '2': [
    {'1': 'to_address', '3': 1, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'amount', '3': 2, '4': 1, '5': 4, '10': 'amount'},
  ],
};

/// Descriptor for `Transfer`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transferDescriptor = $convert.base64Decode(
    'CghUcmFuc2ZlchIdCgp0b19hZGRyZXNzGAEgASgJUgl0b0FkZHJlc3MSFgoGYW1vdW50GAIgAS'
    'gEUgZhbW91bnQ=');

@$core.Deprecated('Use assetTransferDescriptor instead')
const AssetTransfer$json = {
  '1': 'AssetTransfer',
  '2': [
    {'1': 'to_address', '3': 1, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'amount', '3': 2, '4': 1, '5': 4, '10': 'amount'},
    {'1': 'asset_id', '3': 3, '4': 1, '5': 4, '10': 'assetId'},
  ],
};

/// Descriptor for `AssetTransfer`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List assetTransferDescriptor = $convert.base64Decode(
    'Cg1Bc3NldFRyYW5zZmVyEh0KCnRvX2FkZHJlc3MYASABKAlSCXRvQWRkcmVzcxIWCgZhbW91bn'
    'QYAiABKARSBmFtb3VudBIZCghhc3NldF9pZBgDIAEoBFIHYXNzZXRJZA==');

@$core.Deprecated('Use assetOptInDescriptor instead')
const AssetOptIn$json = {
  '1': 'AssetOptIn',
  '2': [
    {'1': 'asset_id', '3': 1, '4': 1, '5': 4, '10': 'assetId'},
  ],
};

/// Descriptor for `AssetOptIn`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List assetOptInDescriptor = $convert.base64Decode(
    'CgpBc3NldE9wdEluEhkKCGFzc2V0X2lkGAEgASgEUgdhc3NldElk');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'genesis_id', '3': 1, '4': 1, '5': 9, '10': 'genesisId'},
    {'1': 'genesis_hash', '3': 2, '4': 1, '5': 12, '10': 'genesisHash'},
    {'1': 'note', '3': 3, '4': 1, '5': 12, '10': 'note'},
    {'1': 'private_key', '3': 4, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'first_round', '3': 5, '4': 1, '5': 4, '10': 'firstRound'},
    {'1': 'last_round', '3': 6, '4': 1, '5': 4, '10': 'lastRound'},
    {'1': 'fee', '3': 7, '4': 1, '5': 4, '10': 'fee'},
    {'1': 'public_key', '3': 8, '4': 1, '5': 12, '10': 'publicKey'},
    {'1': 'transfer', '3': 10, '4': 1, '5': 11, '6': '.TW.Algorand.Proto.Transfer', '9': 0, '10': 'transfer'},
    {'1': 'asset_transfer', '3': 11, '4': 1, '5': 11, '6': '.TW.Algorand.Proto.AssetTransfer', '9': 0, '10': 'assetTransfer'},
    {'1': 'asset_opt_in', '3': 12, '4': 1, '5': 11, '6': '.TW.Algorand.Proto.AssetOptIn', '9': 0, '10': 'assetOptIn'},
  ],
  '8': [
    {'1': 'message_oneof'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSHQoKZ2VuZXNpc19pZBgBIAEoCVIJZ2VuZXNpc0lkEiEKDGdlbmVzaX'
    'NfaGFzaBgCIAEoDFILZ2VuZXNpc0hhc2gSEgoEbm90ZRgDIAEoDFIEbm90ZRIfCgtwcml2YXRl'
    'X2tleRgEIAEoDFIKcHJpdmF0ZUtleRIfCgtmaXJzdF9yb3VuZBgFIAEoBFIKZmlyc3RSb3VuZB'
    'IdCgpsYXN0X3JvdW5kGAYgASgEUglsYXN0Um91bmQSEAoDZmVlGAcgASgEUgNmZWUSHQoKcHVi'
    'bGljX2tleRgIIAEoDFIJcHVibGljS2V5EjkKCHRyYW5zZmVyGAogASgLMhsuVFcuQWxnb3Jhbm'
    'QuUHJvdG8uVHJhbnNmZXJIAFIIdHJhbnNmZXISSQoOYXNzZXRfdHJhbnNmZXIYCyABKAsyIC5U'
    'Vy5BbGdvcmFuZC5Qcm90by5Bc3NldFRyYW5zZmVySABSDWFzc2V0VHJhbnNmZXISQQoMYXNzZX'
    'Rfb3B0X2luGAwgASgLMh0uVFcuQWxnb3JhbmQuUHJvdG8uQXNzZXRPcHRJbkgAUgphc3NldE9w'
    'dEluQg8KDW1lc3NhZ2Vfb25lb2Y=');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'signature', '3': 2, '4': 1, '5': 9, '10': 'signature'},
    {'1': 'error', '3': 3, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 4, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhgKB2VuY29kZWQYASABKAxSB2VuY29kZWQSHAoJc2lnbmF0dXJlGA'
    'IgASgJUglzaWduYXR1cmUSMwoFZXJyb3IYAyABKA4yHS5UVy5Db21tb24uUHJvdG8uU2lnbmlu'
    'Z0Vycm9yUgVlcnJvchIjCg1lcnJvcl9tZXNzYWdlGAQgASgJUgxlcnJvck1lc3NhZ2U=');

