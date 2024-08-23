//
//  Generated code. Do not modify.
//  source: Aion.proto
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
    {'1': 'nonce', '3': 1, '4': 1, '5': 12, '10': 'nonce'},
    {'1': 'gas_price', '3': 2, '4': 1, '5': 12, '10': 'gasPrice'},
    {'1': 'gas_limit', '3': 3, '4': 1, '5': 12, '10': 'gasLimit'},
    {'1': 'to_address', '3': 4, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'amount', '3': 5, '4': 1, '5': 12, '10': 'amount'},
    {'1': 'payload', '3': 6, '4': 1, '5': 12, '10': 'payload'},
    {'1': 'private_key', '3': 7, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'timestamp', '3': 8, '4': 1, '5': 4, '10': 'timestamp'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSFAoFbm9uY2UYASABKAxSBW5vbmNlEhsKCWdhc19wcmljZRgCIAEoDF'
    'IIZ2FzUHJpY2USGwoJZ2FzX2xpbWl0GAMgASgMUghnYXNMaW1pdBIdCgp0b19hZGRyZXNzGAQg'
    'ASgJUgl0b0FkZHJlc3MSFgoGYW1vdW50GAUgASgMUgZhbW91bnQSGAoHcGF5bG9hZBgGIAEoDF'
    'IHcGF5bG9hZBIfCgtwcml2YXRlX2tleRgHIAEoDFIKcHJpdmF0ZUtleRIcCgl0aW1lc3RhbXAY'
    'CCABKARSCXRpbWVzdGFtcA==');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'signature', '3': 2, '4': 1, '5': 12, '10': 'signature'},
    {'1': 'error', '3': 3, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 4, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhgKB2VuY29kZWQYASABKAxSB2VuY29kZWQSHAoJc2lnbmF0dXJlGA'
    'IgASgMUglzaWduYXR1cmUSMwoFZXJyb3IYAyABKA4yHS5UVy5Db21tb24uUHJvdG8uU2lnbmlu'
    'Z0Vycm9yUgVlcnJvchIjCg1lcnJvcl9tZXNzYWdlGAQgASgJUgxlcnJvck1lc3NhZ2U=');

