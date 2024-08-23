//
//  Generated code. Do not modify.
//  source: IOST.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use algorithmDescriptor instead')
const Algorithm$json = {
  '1': 'Algorithm',
  '2': [
    {'1': 'UNKNOWN', '2': 0},
    {'1': 'SECP256K1', '2': 1},
    {'1': 'ED25519', '2': 2},
  ],
};

/// Descriptor for `Algorithm`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List algorithmDescriptor = $convert.base64Decode(
    'CglBbGdvcml0aG0SCwoHVU5LTk9XThAAEg0KCVNFQ1AyNTZLMRABEgsKB0VEMjU1MTkQAg==');

@$core.Deprecated('Use actionDescriptor instead')
const Action$json = {
  '1': 'Action',
  '2': [
    {'1': 'contract', '3': 1, '4': 1, '5': 9, '10': 'contract'},
    {'1': 'action_name', '3': 2, '4': 1, '5': 9, '10': 'actionName'},
    {'1': 'data', '3': 3, '4': 1, '5': 9, '10': 'data'},
  ],
};

/// Descriptor for `Action`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List actionDescriptor = $convert.base64Decode(
    'CgZBY3Rpb24SGgoIY29udHJhY3QYASABKAlSCGNvbnRyYWN0Eh8KC2FjdGlvbl9uYW1lGAIgAS'
    'gJUgphY3Rpb25OYW1lEhIKBGRhdGEYAyABKAlSBGRhdGE=');

@$core.Deprecated('Use amountLimitDescriptor instead')
const AmountLimit$json = {
  '1': 'AmountLimit',
  '2': [
    {'1': 'token', '3': 1, '4': 1, '5': 9, '10': 'token'},
    {'1': 'value', '3': 2, '4': 1, '5': 9, '10': 'value'},
  ],
};

/// Descriptor for `AmountLimit`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List amountLimitDescriptor = $convert.base64Decode(
    'CgtBbW91bnRMaW1pdBIUCgV0b2tlbhgBIAEoCVIFdG9rZW4SFAoFdmFsdWUYAiABKAlSBXZhbH'
    'Vl');

@$core.Deprecated('Use signatureDescriptor instead')
const Signature$json = {
  '1': 'Signature',
  '2': [
    {'1': 'algorithm', '3': 1, '4': 1, '5': 14, '6': '.TW.IOST.Proto.Algorithm', '10': 'algorithm'},
    {'1': 'signature', '3': 2, '4': 1, '5': 12, '10': 'signature'},
    {'1': 'public_key', '3': 3, '4': 1, '5': 12, '10': 'publicKey'},
  ],
};

/// Descriptor for `Signature`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signatureDescriptor = $convert.base64Decode(
    'CglTaWduYXR1cmUSNgoJYWxnb3JpdGhtGAEgASgOMhguVFcuSU9TVC5Qcm90by5BbGdvcml0aG'
    '1SCWFsZ29yaXRobRIcCglzaWduYXR1cmUYAiABKAxSCXNpZ25hdHVyZRIdCgpwdWJsaWNfa2V5'
    'GAMgASgMUglwdWJsaWNLZXk=');

@$core.Deprecated('Use transactionDescriptor instead')
const Transaction$json = {
  '1': 'Transaction',
  '2': [
    {'1': 'time', '3': 1, '4': 1, '5': 3, '10': 'time'},
    {'1': 'expiration', '3': 2, '4': 1, '5': 3, '10': 'expiration'},
    {'1': 'gas_ratio', '3': 3, '4': 1, '5': 1, '10': 'gasRatio'},
    {'1': 'gas_limit', '3': 4, '4': 1, '5': 1, '10': 'gasLimit'},
    {'1': 'delay', '3': 5, '4': 1, '5': 3, '10': 'delay'},
    {'1': 'chain_id', '3': 6, '4': 1, '5': 13, '10': 'chainId'},
    {'1': 'actions', '3': 7, '4': 3, '5': 11, '6': '.TW.IOST.Proto.Action', '10': 'actions'},
    {'1': 'amount_limit', '3': 8, '4': 3, '5': 11, '6': '.TW.IOST.Proto.AmountLimit', '10': 'amountLimit'},
    {'1': 'signers', '3': 9, '4': 3, '5': 9, '10': 'signers'},
    {'1': 'signatures', '3': 10, '4': 3, '5': 11, '6': '.TW.IOST.Proto.Signature', '10': 'signatures'},
    {'1': 'publisher', '3': 11, '4': 1, '5': 9, '10': 'publisher'},
    {'1': 'publisher_sigs', '3': 12, '4': 3, '5': 11, '6': '.TW.IOST.Proto.Signature', '10': 'publisherSigs'},
  ],
};

/// Descriptor for `Transaction`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionDescriptor = $convert.base64Decode(
    'CgtUcmFuc2FjdGlvbhISCgR0aW1lGAEgASgDUgR0aW1lEh4KCmV4cGlyYXRpb24YAiABKANSCm'
    'V4cGlyYXRpb24SGwoJZ2FzX3JhdGlvGAMgASgBUghnYXNSYXRpbxIbCglnYXNfbGltaXQYBCAB'
    'KAFSCGdhc0xpbWl0EhQKBWRlbGF5GAUgASgDUgVkZWxheRIZCghjaGFpbl9pZBgGIAEoDVIHY2'
    'hhaW5JZBIvCgdhY3Rpb25zGAcgAygLMhUuVFcuSU9TVC5Qcm90by5BY3Rpb25SB2FjdGlvbnMS'
    'PQoMYW1vdW50X2xpbWl0GAggAygLMhouVFcuSU9TVC5Qcm90by5BbW91bnRMaW1pdFILYW1vdW'
    '50TGltaXQSGAoHc2lnbmVycxgJIAMoCVIHc2lnbmVycxI4CgpzaWduYXR1cmVzGAogAygLMhgu'
    'VFcuSU9TVC5Qcm90by5TaWduYXR1cmVSCnNpZ25hdHVyZXMSHAoJcHVibGlzaGVyGAsgASgJUg'
    'lwdWJsaXNoZXISPwoOcHVibGlzaGVyX3NpZ3MYDCADKAsyGC5UVy5JT1NULlByb3RvLlNpZ25h'
    'dHVyZVINcHVibGlzaGVyU2lncw==');

@$core.Deprecated('Use accountInfoDescriptor instead')
const AccountInfo$json = {
  '1': 'AccountInfo',
  '2': [
    {'1': 'name', '3': 1, '4': 1, '5': 9, '10': 'name'},
    {'1': 'active_key', '3': 2, '4': 1, '5': 12, '10': 'activeKey'},
    {'1': 'owner_key', '3': 3, '4': 1, '5': 12, '10': 'ownerKey'},
  ],
};

/// Descriptor for `AccountInfo`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List accountInfoDescriptor = $convert.base64Decode(
    'CgtBY2NvdW50SW5mbxISCgRuYW1lGAEgASgJUgRuYW1lEh0KCmFjdGl2ZV9rZXkYAiABKAxSCW'
    'FjdGl2ZUtleRIbCglvd25lcl9rZXkYAyABKAxSCG93bmVyS2V5');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'account', '3': 1, '4': 1, '5': 11, '6': '.TW.IOST.Proto.AccountInfo', '10': 'account'},
    {'1': 'transaction_template', '3': 2, '4': 1, '5': 11, '6': '.TW.IOST.Proto.Transaction', '10': 'transactionTemplate'},
    {'1': 'transfer_destination', '3': 3, '4': 1, '5': 9, '10': 'transferDestination'},
    {'1': 'transfer_amount', '3': 4, '4': 1, '5': 9, '10': 'transferAmount'},
    {'1': 'transfer_memo', '3': 5, '4': 1, '5': 9, '10': 'transferMemo'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSNAoHYWNjb3VudBgBIAEoCzIaLlRXLklPU1QuUHJvdG8uQWNjb3VudE'
    'luZm9SB2FjY291bnQSTQoUdHJhbnNhY3Rpb25fdGVtcGxhdGUYAiABKAsyGi5UVy5JT1NULlBy'
    'b3RvLlRyYW5zYWN0aW9uUhN0cmFuc2FjdGlvblRlbXBsYXRlEjEKFHRyYW5zZmVyX2Rlc3Rpbm'
    'F0aW9uGAMgASgJUhN0cmFuc2ZlckRlc3RpbmF0aW9uEicKD3RyYW5zZmVyX2Ftb3VudBgEIAEo'
    'CVIOdHJhbnNmZXJBbW91bnQSIwoNdHJhbnNmZXJfbWVtbxgFIAEoCVIMdHJhbnNmZXJNZW1v');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'transaction', '3': 1, '4': 1, '5': 11, '6': '.TW.IOST.Proto.Transaction', '10': 'transaction'},
    {'1': 'encoded', '3': 2, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'error', '3': 3, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 4, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EjwKC3RyYW5zYWN0aW9uGAEgASgLMhouVFcuSU9TVC5Qcm90by5Ucm'
    'Fuc2FjdGlvblILdHJhbnNhY3Rpb24SGAoHZW5jb2RlZBgCIAEoDFIHZW5jb2RlZBIzCgVlcnJv'
    'chgDIAEoDjIdLlRXLkNvbW1vbi5Qcm90by5TaWduaW5nRXJyb3JSBWVycm9yEiMKDWVycm9yX2'
    '1lc3NhZ2UYBCABKAlSDGVycm9yTWVzc2FnZQ==');

