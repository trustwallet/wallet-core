//
//  Generated code. Do not modify.
//  source: NULS.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use transactionCoinFromDescriptor instead')
const TransactionCoinFrom$json = {
  '1': 'TransactionCoinFrom',
  '2': [
    {'1': 'from_address', '3': 1, '4': 1, '5': 9, '10': 'fromAddress'},
    {'1': 'assets_chainid', '3': 2, '4': 1, '5': 13, '10': 'assetsChainid'},
    {'1': 'assets_id', '3': 3, '4': 1, '5': 13, '10': 'assetsId'},
    {'1': 'id_amount', '3': 4, '4': 1, '5': 12, '10': 'idAmount'},
    {'1': 'nonce', '3': 5, '4': 1, '5': 12, '10': 'nonce'},
    {'1': 'locked', '3': 6, '4': 1, '5': 13, '10': 'locked'},
  ],
};

/// Descriptor for `TransactionCoinFrom`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionCoinFromDescriptor = $convert.base64Decode(
    'ChNUcmFuc2FjdGlvbkNvaW5Gcm9tEiEKDGZyb21fYWRkcmVzcxgBIAEoCVILZnJvbUFkZHJlc3'
    'MSJQoOYXNzZXRzX2NoYWluaWQYAiABKA1SDWFzc2V0c0NoYWluaWQSGwoJYXNzZXRzX2lkGAMg'
    'ASgNUghhc3NldHNJZBIbCglpZF9hbW91bnQYBCABKAxSCGlkQW1vdW50EhQKBW5vbmNlGAUgAS'
    'gMUgVub25jZRIWCgZsb2NrZWQYBiABKA1SBmxvY2tlZA==');

@$core.Deprecated('Use transactionCoinToDescriptor instead')
const TransactionCoinTo$json = {
  '1': 'TransactionCoinTo',
  '2': [
    {'1': 'to_address', '3': 1, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'assets_chainid', '3': 2, '4': 1, '5': 13, '10': 'assetsChainid'},
    {'1': 'assets_id', '3': 3, '4': 1, '5': 13, '10': 'assetsId'},
    {'1': 'id_amount', '3': 4, '4': 1, '5': 12, '10': 'idAmount'},
    {'1': 'lock_time', '3': 5, '4': 1, '5': 13, '10': 'lockTime'},
  ],
};

/// Descriptor for `TransactionCoinTo`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionCoinToDescriptor = $convert.base64Decode(
    'ChFUcmFuc2FjdGlvbkNvaW5UbxIdCgp0b19hZGRyZXNzGAEgASgJUgl0b0FkZHJlc3MSJQoOYX'
    'NzZXRzX2NoYWluaWQYAiABKA1SDWFzc2V0c0NoYWluaWQSGwoJYXNzZXRzX2lkGAMgASgNUghh'
    'c3NldHNJZBIbCglpZF9hbW91bnQYBCABKAxSCGlkQW1vdW50EhsKCWxvY2tfdGltZRgFIAEoDV'
    'IIbG9ja1RpbWU=');

@$core.Deprecated('Use signatureDescriptor instead')
const Signature$json = {
  '1': 'Signature',
  '2': [
    {'1': 'pkey_len', '3': 1, '4': 1, '5': 13, '10': 'pkeyLen'},
    {'1': 'public_key', '3': 2, '4': 1, '5': 12, '10': 'publicKey'},
    {'1': 'sig_len', '3': 3, '4': 1, '5': 13, '10': 'sigLen'},
    {'1': 'signature', '3': 4, '4': 1, '5': 12, '10': 'signature'},
  ],
};

/// Descriptor for `Signature`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signatureDescriptor = $convert.base64Decode(
    'CglTaWduYXR1cmUSGQoIcGtleV9sZW4YASABKA1SB3BrZXlMZW4SHQoKcHVibGljX2tleRgCIA'
    'EoDFIJcHVibGljS2V5EhcKB3NpZ19sZW4YAyABKA1SBnNpZ0xlbhIcCglzaWduYXR1cmUYBCAB'
    'KAxSCXNpZ25hdHVyZQ==');

@$core.Deprecated('Use transactionDescriptor instead')
const Transaction$json = {
  '1': 'Transaction',
  '2': [
    {'1': 'type', '3': 1, '4': 1, '5': 13, '10': 'type'},
    {'1': 'timestamp', '3': 2, '4': 1, '5': 13, '10': 'timestamp'},
    {'1': 'remark', '3': 3, '4': 1, '5': 9, '10': 'remark'},
    {'1': 'tx_data', '3': 4, '4': 1, '5': 12, '10': 'txData'},
    {'1': 'input', '3': 5, '4': 3, '5': 11, '6': '.TW.NULS.Proto.TransactionCoinFrom', '10': 'input'},
    {'1': 'output', '3': 6, '4': 3, '5': 11, '6': '.TW.NULS.Proto.TransactionCoinTo', '10': 'output'},
    {'1': 'tx_sigs', '3': 7, '4': 1, '5': 11, '6': '.TW.NULS.Proto.Signature', '10': 'txSigs'},
    {'1': 'hash', '3': 8, '4': 1, '5': 13, '10': 'hash'},
  ],
};

/// Descriptor for `Transaction`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionDescriptor = $convert.base64Decode(
    'CgtUcmFuc2FjdGlvbhISCgR0eXBlGAEgASgNUgR0eXBlEhwKCXRpbWVzdGFtcBgCIAEoDVIJdG'
    'ltZXN0YW1wEhYKBnJlbWFyaxgDIAEoCVIGcmVtYXJrEhcKB3R4X2RhdGEYBCABKAxSBnR4RGF0'
    'YRI4CgVpbnB1dBgFIAMoCzIiLlRXLk5VTFMuUHJvdG8uVHJhbnNhY3Rpb25Db2luRnJvbVIFaW'
    '5wdXQSOAoGb3V0cHV0GAYgAygLMiAuVFcuTlVMUy5Qcm90by5UcmFuc2FjdGlvbkNvaW5Ub1IG'
    'b3V0cHV0EjEKB3R4X3NpZ3MYByABKAsyGC5UVy5OVUxTLlByb3RvLlNpZ25hdHVyZVIGdHhTaW'
    'dzEhIKBGhhc2gYCCABKA1SBGhhc2g=');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'private_key', '3': 1, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'from', '3': 2, '4': 1, '5': 9, '10': 'from'},
    {'1': 'to', '3': 3, '4': 1, '5': 9, '10': 'to'},
    {'1': 'amount', '3': 4, '4': 1, '5': 12, '10': 'amount'},
    {'1': 'chain_id', '3': 5, '4': 1, '5': 13, '10': 'chainId'},
    {'1': 'idassets_id', '3': 6, '4': 1, '5': 13, '10': 'idassetsId'},
    {'1': 'nonce', '3': 7, '4': 1, '5': 12, '10': 'nonce'},
    {'1': 'remark', '3': 8, '4': 1, '5': 9, '10': 'remark'},
    {'1': 'balance', '3': 9, '4': 1, '5': 12, '10': 'balance'},
    {'1': 'timestamp', '3': 10, '4': 1, '5': 13, '10': 'timestamp'},
    {'1': 'fee_payer', '3': 11, '4': 1, '5': 9, '10': 'feePayer'},
    {'1': 'fee_payer_nonce', '3': 12, '4': 1, '5': 12, '10': 'feePayerNonce'},
    {'1': 'fee_payer_private_key', '3': 13, '4': 1, '5': 12, '10': 'feePayerPrivateKey'},
    {'1': 'fee_payer_balance', '3': 14, '4': 1, '5': 12, '10': 'feePayerBalance'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSHwoLcHJpdmF0ZV9rZXkYASABKAxSCnByaXZhdGVLZXkSEgoEZnJvbR'
    'gCIAEoCVIEZnJvbRIOCgJ0bxgDIAEoCVICdG8SFgoGYW1vdW50GAQgASgMUgZhbW91bnQSGQoI'
    'Y2hhaW5faWQYBSABKA1SB2NoYWluSWQSHwoLaWRhc3NldHNfaWQYBiABKA1SCmlkYXNzZXRzSW'
    'QSFAoFbm9uY2UYByABKAxSBW5vbmNlEhYKBnJlbWFyaxgIIAEoCVIGcmVtYXJrEhgKB2JhbGFu'
    'Y2UYCSABKAxSB2JhbGFuY2USHAoJdGltZXN0YW1wGAogASgNUgl0aW1lc3RhbXASGwoJZmVlX3'
    'BheWVyGAsgASgJUghmZWVQYXllchImCg9mZWVfcGF5ZXJfbm9uY2UYDCABKAxSDWZlZVBheWVy'
    'Tm9uY2USMQoVZmVlX3BheWVyX3ByaXZhdGVfa2V5GA0gASgMUhJmZWVQYXllclByaXZhdGVLZX'
    'kSKgoRZmVlX3BheWVyX2JhbGFuY2UYDiABKAxSD2ZlZVBheWVyQmFsYW5jZQ==');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'error', '3': 2, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 3, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhgKB2VuY29kZWQYASABKAxSB2VuY29kZWQSMwoFZXJyb3IYAiABKA'
    '4yHS5UVy5Db21tb24uUHJvdG8uU2lnbmluZ0Vycm9yUgVlcnJvchIjCg1lcnJvcl9tZXNzYWdl'
    'GAMgASgJUgxlcnJvck1lc3NhZ2U=');

