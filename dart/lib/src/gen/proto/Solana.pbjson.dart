//
//  Generated code. Do not modify.
//  source: Solana.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use encodingDescriptor instead')
const Encoding$json = {
  '1': 'Encoding',
  '2': [
    {'1': 'Base58', '2': 0},
    {'1': 'Base64', '2': 1},
  ],
};

/// Descriptor for `Encoding`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List encodingDescriptor = $convert.base64Decode(
    'CghFbmNvZGluZxIKCgZCYXNlNTgQABIKCgZCYXNlNjQQAQ==');

@$core.Deprecated('Use transferDescriptor instead')
const Transfer$json = {
  '1': 'Transfer',
  '2': [
    {'1': 'recipient', '3': 1, '4': 1, '5': 9, '10': 'recipient'},
    {'1': 'value', '3': 2, '4': 1, '5': 4, '10': 'value'},
    {'1': 'memo', '3': 3, '4': 1, '5': 9, '10': 'memo'},
    {'1': 'references', '3': 4, '4': 3, '5': 9, '10': 'references'},
  ],
};

/// Descriptor for `Transfer`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transferDescriptor = $convert.base64Decode(
    'CghUcmFuc2ZlchIcCglyZWNpcGllbnQYASABKAlSCXJlY2lwaWVudBIUCgV2YWx1ZRgCIAEoBF'
    'IFdmFsdWUSEgoEbWVtbxgDIAEoCVIEbWVtbxIeCgpyZWZlcmVuY2VzGAQgAygJUgpyZWZlcmVu'
    'Y2Vz');

@$core.Deprecated('Use delegateStakeDescriptor instead')
const DelegateStake$json = {
  '1': 'DelegateStake',
  '2': [
    {'1': 'validator_pubkey', '3': 1, '4': 1, '5': 9, '10': 'validatorPubkey'},
    {'1': 'value', '3': 2, '4': 1, '5': 4, '10': 'value'},
    {'1': 'stake_account', '3': 3, '4': 1, '5': 9, '10': 'stakeAccount'},
  ],
};

/// Descriptor for `DelegateStake`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List delegateStakeDescriptor = $convert.base64Decode(
    'Cg1EZWxlZ2F0ZVN0YWtlEikKEHZhbGlkYXRvcl9wdWJrZXkYASABKAlSD3ZhbGlkYXRvclB1Ym'
    'tleRIUCgV2YWx1ZRgCIAEoBFIFdmFsdWUSIwoNc3Rha2VfYWNjb3VudBgDIAEoCVIMc3Rha2VB'
    'Y2NvdW50');

@$core.Deprecated('Use deactivateStakeDescriptor instead')
const DeactivateStake$json = {
  '1': 'DeactivateStake',
  '2': [
    {'1': 'stake_account', '3': 1, '4': 1, '5': 9, '10': 'stakeAccount'},
  ],
};

/// Descriptor for `DeactivateStake`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List deactivateStakeDescriptor = $convert.base64Decode(
    'Cg9EZWFjdGl2YXRlU3Rha2USIwoNc3Rha2VfYWNjb3VudBgBIAEoCVIMc3Rha2VBY2NvdW50');

@$core.Deprecated('Use deactivateAllStakeDescriptor instead')
const DeactivateAllStake$json = {
  '1': 'DeactivateAllStake',
  '2': [
    {'1': 'stake_accounts', '3': 1, '4': 3, '5': 9, '10': 'stakeAccounts'},
  ],
};

/// Descriptor for `DeactivateAllStake`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List deactivateAllStakeDescriptor = $convert.base64Decode(
    'ChJEZWFjdGl2YXRlQWxsU3Rha2USJQoOc3Rha2VfYWNjb3VudHMYASADKAlSDXN0YWtlQWNjb3'
    'VudHM=');

@$core.Deprecated('Use withdrawStakeDescriptor instead')
const WithdrawStake$json = {
  '1': 'WithdrawStake',
  '2': [
    {'1': 'stake_account', '3': 1, '4': 1, '5': 9, '10': 'stakeAccount'},
    {'1': 'value', '3': 2, '4': 1, '5': 4, '10': 'value'},
  ],
};

/// Descriptor for `WithdrawStake`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List withdrawStakeDescriptor = $convert.base64Decode(
    'Cg1XaXRoZHJhd1N0YWtlEiMKDXN0YWtlX2FjY291bnQYASABKAlSDHN0YWtlQWNjb3VudBIUCg'
    'V2YWx1ZRgCIAEoBFIFdmFsdWU=');

@$core.Deprecated('Use stakeAccountValueDescriptor instead')
const StakeAccountValue$json = {
  '1': 'StakeAccountValue',
  '2': [
    {'1': 'stake_account', '3': 1, '4': 1, '5': 9, '10': 'stakeAccount'},
    {'1': 'value', '3': 2, '4': 1, '5': 4, '10': 'value'},
  ],
};

/// Descriptor for `StakeAccountValue`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List stakeAccountValueDescriptor = $convert.base64Decode(
    'ChFTdGFrZUFjY291bnRWYWx1ZRIjCg1zdGFrZV9hY2NvdW50GAEgASgJUgxzdGFrZUFjY291bn'
    'QSFAoFdmFsdWUYAiABKARSBXZhbHVl');

@$core.Deprecated('Use withdrawAllStakeDescriptor instead')
const WithdrawAllStake$json = {
  '1': 'WithdrawAllStake',
  '2': [
    {'1': 'stake_accounts', '3': 1, '4': 3, '5': 11, '6': '.TW.Solana.Proto.StakeAccountValue', '10': 'stakeAccounts'},
  ],
};

/// Descriptor for `WithdrawAllStake`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List withdrawAllStakeDescriptor = $convert.base64Decode(
    'ChBXaXRoZHJhd0FsbFN0YWtlEkkKDnN0YWtlX2FjY291bnRzGAEgAygLMiIuVFcuU29sYW5hLl'
    'Byb3RvLlN0YWtlQWNjb3VudFZhbHVlUg1zdGFrZUFjY291bnRz');

@$core.Deprecated('Use createTokenAccountDescriptor instead')
const CreateTokenAccount$json = {
  '1': 'CreateTokenAccount',
  '2': [
    {'1': 'main_address', '3': 1, '4': 1, '5': 9, '10': 'mainAddress'},
    {'1': 'token_mint_address', '3': 2, '4': 1, '5': 9, '10': 'tokenMintAddress'},
    {'1': 'token_address', '3': 3, '4': 1, '5': 9, '10': 'tokenAddress'},
  ],
};

/// Descriptor for `CreateTokenAccount`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List createTokenAccountDescriptor = $convert.base64Decode(
    'ChJDcmVhdGVUb2tlbkFjY291bnQSIQoMbWFpbl9hZGRyZXNzGAEgASgJUgttYWluQWRkcmVzcx'
    'IsChJ0b2tlbl9taW50X2FkZHJlc3MYAiABKAlSEHRva2VuTWludEFkZHJlc3MSIwoNdG9rZW5f'
    'YWRkcmVzcxgDIAEoCVIMdG9rZW5BZGRyZXNz');

@$core.Deprecated('Use tokenTransferDescriptor instead')
const TokenTransfer$json = {
  '1': 'TokenTransfer',
  '2': [
    {'1': 'token_mint_address', '3': 1, '4': 1, '5': 9, '10': 'tokenMintAddress'},
    {'1': 'sender_token_address', '3': 2, '4': 1, '5': 9, '10': 'senderTokenAddress'},
    {'1': 'recipient_token_address', '3': 3, '4': 1, '5': 9, '10': 'recipientTokenAddress'},
    {'1': 'amount', '3': 4, '4': 1, '5': 4, '10': 'amount'},
    {'1': 'decimals', '3': 5, '4': 1, '5': 13, '10': 'decimals'},
    {'1': 'memo', '3': 6, '4': 1, '5': 9, '10': 'memo'},
    {'1': 'references', '3': 7, '4': 3, '5': 9, '10': 'references'},
  ],
};

/// Descriptor for `TokenTransfer`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tokenTransferDescriptor = $convert.base64Decode(
    'Cg1Ub2tlblRyYW5zZmVyEiwKEnRva2VuX21pbnRfYWRkcmVzcxgBIAEoCVIQdG9rZW5NaW50QW'
    'RkcmVzcxIwChRzZW5kZXJfdG9rZW5fYWRkcmVzcxgCIAEoCVISc2VuZGVyVG9rZW5BZGRyZXNz'
    'EjYKF3JlY2lwaWVudF90b2tlbl9hZGRyZXNzGAMgASgJUhVyZWNpcGllbnRUb2tlbkFkZHJlc3'
    'MSFgoGYW1vdW50GAQgASgEUgZhbW91bnQSGgoIZGVjaW1hbHMYBSABKA1SCGRlY2ltYWxzEhIK'
    'BG1lbW8YBiABKAlSBG1lbW8SHgoKcmVmZXJlbmNlcxgHIAMoCVIKcmVmZXJlbmNlcw==');

@$core.Deprecated('Use createAndTransferTokenDescriptor instead')
const CreateAndTransferToken$json = {
  '1': 'CreateAndTransferToken',
  '2': [
    {'1': 'recipient_main_address', '3': 1, '4': 1, '5': 9, '10': 'recipientMainAddress'},
    {'1': 'token_mint_address', '3': 2, '4': 1, '5': 9, '10': 'tokenMintAddress'},
    {'1': 'recipient_token_address', '3': 3, '4': 1, '5': 9, '10': 'recipientTokenAddress'},
    {'1': 'sender_token_address', '3': 4, '4': 1, '5': 9, '10': 'senderTokenAddress'},
    {'1': 'amount', '3': 5, '4': 1, '5': 4, '10': 'amount'},
    {'1': 'decimals', '3': 6, '4': 1, '5': 13, '10': 'decimals'},
    {'1': 'memo', '3': 7, '4': 1, '5': 9, '10': 'memo'},
    {'1': 'references', '3': 8, '4': 3, '5': 9, '10': 'references'},
  ],
};

/// Descriptor for `CreateAndTransferToken`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List createAndTransferTokenDescriptor = $convert.base64Decode(
    'ChZDcmVhdGVBbmRUcmFuc2ZlclRva2VuEjQKFnJlY2lwaWVudF9tYWluX2FkZHJlc3MYASABKA'
    'lSFHJlY2lwaWVudE1haW5BZGRyZXNzEiwKEnRva2VuX21pbnRfYWRkcmVzcxgCIAEoCVIQdG9r'
    'ZW5NaW50QWRkcmVzcxI2ChdyZWNpcGllbnRfdG9rZW5fYWRkcmVzcxgDIAEoCVIVcmVjaXBpZW'
    '50VG9rZW5BZGRyZXNzEjAKFHNlbmRlcl90b2tlbl9hZGRyZXNzGAQgASgJUhJzZW5kZXJUb2tl'
    'bkFkZHJlc3MSFgoGYW1vdW50GAUgASgEUgZhbW91bnQSGgoIZGVjaW1hbHMYBiABKA1SCGRlY2'
    'ltYWxzEhIKBG1lbW8YByABKAlSBG1lbW8SHgoKcmVmZXJlbmNlcxgIIAMoCVIKcmVmZXJlbmNl'
    'cw==');

@$core.Deprecated('Use createNonceAccountDescriptor instead')
const CreateNonceAccount$json = {
  '1': 'CreateNonceAccount',
  '2': [
    {'1': 'nonce_account', '3': 1, '4': 1, '5': 9, '10': 'nonceAccount'},
    {'1': 'rent', '3': 2, '4': 1, '5': 4, '10': 'rent'},
    {'1': 'nonce_account_private_key', '3': 3, '4': 1, '5': 12, '10': 'nonceAccountPrivateKey'},
  ],
};

/// Descriptor for `CreateNonceAccount`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List createNonceAccountDescriptor = $convert.base64Decode(
    'ChJDcmVhdGVOb25jZUFjY291bnQSIwoNbm9uY2VfYWNjb3VudBgBIAEoCVIMbm9uY2VBY2NvdW'
    '50EhIKBHJlbnQYAiABKARSBHJlbnQSOQoZbm9uY2VfYWNjb3VudF9wcml2YXRlX2tleRgDIAEo'
    'DFIWbm9uY2VBY2NvdW50UHJpdmF0ZUtleQ==');

@$core.Deprecated('Use withdrawNonceAccountDescriptor instead')
const WithdrawNonceAccount$json = {
  '1': 'WithdrawNonceAccount',
  '2': [
    {'1': 'nonce_account', '3': 1, '4': 1, '5': 9, '10': 'nonceAccount'},
    {'1': 'recipient', '3': 2, '4': 1, '5': 9, '10': 'recipient'},
    {'1': 'value', '3': 3, '4': 1, '5': 4, '10': 'value'},
  ],
};

/// Descriptor for `WithdrawNonceAccount`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List withdrawNonceAccountDescriptor = $convert.base64Decode(
    'ChRXaXRoZHJhd05vbmNlQWNjb3VudBIjCg1ub25jZV9hY2NvdW50GAEgASgJUgxub25jZUFjY2'
    '91bnQSHAoJcmVjaXBpZW50GAIgASgJUglyZWNpcGllbnQSFAoFdmFsdWUYAyABKARSBXZhbHVl');

@$core.Deprecated('Use advanceNonceAccountDescriptor instead')
const AdvanceNonceAccount$json = {
  '1': 'AdvanceNonceAccount',
  '2': [
    {'1': 'nonce_account', '3': 1, '4': 1, '5': 9, '10': 'nonceAccount'},
  ],
};

/// Descriptor for `AdvanceNonceAccount`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List advanceNonceAccountDescriptor = $convert.base64Decode(
    'ChNBZHZhbmNlTm9uY2VBY2NvdW50EiMKDW5vbmNlX2FjY291bnQYASABKAlSDG5vbmNlQWNjb3'
    'VudA==');

@$core.Deprecated('Use pubkeySignatureDescriptor instead')
const PubkeySignature$json = {
  '1': 'PubkeySignature',
  '2': [
    {'1': 'pubkey', '3': 1, '4': 1, '5': 9, '10': 'pubkey'},
    {'1': 'signature', '3': 2, '4': 1, '5': 9, '10': 'signature'},
  ],
};

/// Descriptor for `PubkeySignature`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List pubkeySignatureDescriptor = $convert.base64Decode(
    'Cg9QdWJrZXlTaWduYXR1cmUSFgoGcHVia2V5GAEgASgJUgZwdWJrZXkSHAoJc2lnbmF0dXJlGA'
    'IgASgJUglzaWduYXR1cmU=');

@$core.Deprecated('Use rawMessageDescriptor instead')
const RawMessage$json = {
  '1': 'RawMessage',
  '2': [
    {'1': 'signatures', '3': 1, '4': 3, '5': 11, '6': '.TW.Solana.Proto.PubkeySignature', '10': 'signatures'},
    {'1': 'legacy', '3': 2, '4': 1, '5': 11, '6': '.TW.Solana.Proto.RawMessage.MessageLegacy', '9': 0, '10': 'legacy'},
    {'1': 'v0', '3': 3, '4': 1, '5': 11, '6': '.TW.Solana.Proto.RawMessage.MessageV0', '9': 0, '10': 'v0'},
  ],
  '3': [RawMessage_MessageHeader$json, RawMessage_Instruction$json, RawMessage_MessageAddressTableLookup$json, RawMessage_MessageLegacy$json, RawMessage_MessageV0$json],
  '8': [
    {'1': 'message'},
  ],
};

@$core.Deprecated('Use rawMessageDescriptor instead')
const RawMessage_MessageHeader$json = {
  '1': 'MessageHeader',
  '2': [
    {'1': 'num_required_signatures', '3': 1, '4': 1, '5': 13, '10': 'numRequiredSignatures'},
    {'1': 'num_readonly_signed_accounts', '3': 2, '4': 1, '5': 13, '10': 'numReadonlySignedAccounts'},
    {'1': 'num_readonly_unsigned_accounts', '3': 3, '4': 1, '5': 13, '10': 'numReadonlyUnsignedAccounts'},
  ],
};

@$core.Deprecated('Use rawMessageDescriptor instead')
const RawMessage_Instruction$json = {
  '1': 'Instruction',
  '2': [
    {'1': 'program_id', '3': 1, '4': 1, '5': 13, '10': 'programId'},
    {
      '1': 'accounts',
      '3': 2,
      '4': 3,
      '5': 13,
      '8': {'2': true},
      '10': 'accounts',
    },
    {'1': 'program_data', '3': 3, '4': 1, '5': 12, '10': 'programData'},
  ],
};

@$core.Deprecated('Use rawMessageDescriptor instead')
const RawMessage_MessageAddressTableLookup$json = {
  '1': 'MessageAddressTableLookup',
  '2': [
    {'1': 'account_key', '3': 1, '4': 1, '5': 9, '10': 'accountKey'},
    {
      '1': 'writable_indexes',
      '3': 2,
      '4': 3,
      '5': 13,
      '8': {'2': true},
      '10': 'writableIndexes',
    },
    {
      '1': 'readonly_indexes',
      '3': 3,
      '4': 3,
      '5': 13,
      '8': {'2': true},
      '10': 'readonlyIndexes',
    },
  ],
};

@$core.Deprecated('Use rawMessageDescriptor instead')
const RawMessage_MessageLegacy$json = {
  '1': 'MessageLegacy',
  '2': [
    {'1': 'header', '3': 1, '4': 1, '5': 11, '6': '.TW.Solana.Proto.RawMessage.MessageHeader', '10': 'header'},
    {'1': 'account_keys', '3': 2, '4': 3, '5': 9, '10': 'accountKeys'},
    {'1': 'recent_blockhash', '3': 3, '4': 1, '5': 9, '10': 'recentBlockhash'},
    {'1': 'instructions', '3': 4, '4': 3, '5': 11, '6': '.TW.Solana.Proto.RawMessage.Instruction', '10': 'instructions'},
  ],
};

@$core.Deprecated('Use rawMessageDescriptor instead')
const RawMessage_MessageV0$json = {
  '1': 'MessageV0',
  '2': [
    {'1': 'header', '3': 1, '4': 1, '5': 11, '6': '.TW.Solana.Proto.RawMessage.MessageHeader', '10': 'header'},
    {'1': 'account_keys', '3': 2, '4': 3, '5': 9, '10': 'accountKeys'},
    {'1': 'recent_blockhash', '3': 3, '4': 1, '5': 9, '10': 'recentBlockhash'},
    {'1': 'instructions', '3': 4, '4': 3, '5': 11, '6': '.TW.Solana.Proto.RawMessage.Instruction', '10': 'instructions'},
    {'1': 'address_table_lookups', '3': 5, '4': 3, '5': 11, '6': '.TW.Solana.Proto.RawMessage.MessageAddressTableLookup', '10': 'addressTableLookups'},
  ],
};

/// Descriptor for `RawMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List rawMessageDescriptor = $convert.base64Decode(
    'CgpSYXdNZXNzYWdlEkAKCnNpZ25hdHVyZXMYASADKAsyIC5UVy5Tb2xhbmEuUHJvdG8uUHVia2'
    'V5U2lnbmF0dXJlUgpzaWduYXR1cmVzEkMKBmxlZ2FjeRgCIAEoCzIpLlRXLlNvbGFuYS5Qcm90'
    'by5SYXdNZXNzYWdlLk1lc3NhZ2VMZWdhY3lIAFIGbGVnYWN5EjcKAnYwGAMgASgLMiUuVFcuU2'
    '9sYW5hLlByb3RvLlJhd01lc3NhZ2UuTWVzc2FnZVYwSABSAnYwGs0BCg1NZXNzYWdlSGVhZGVy'
    'EjYKF251bV9yZXF1aXJlZF9zaWduYXR1cmVzGAEgASgNUhVudW1SZXF1aXJlZFNpZ25hdHVyZX'
    'MSPwocbnVtX3JlYWRvbmx5X3NpZ25lZF9hY2NvdW50cxgCIAEoDVIZbnVtUmVhZG9ubHlTaWdu'
    'ZWRBY2NvdW50cxJDCh5udW1fcmVhZG9ubHlfdW5zaWduZWRfYWNjb3VudHMYAyABKA1SG251bV'
    'JlYWRvbmx5VW5zaWduZWRBY2NvdW50cxpvCgtJbnN0cnVjdGlvbhIdCgpwcm9ncmFtX2lkGAEg'
    'ASgNUglwcm9ncmFtSWQSHgoIYWNjb3VudHMYAiADKA1CAhABUghhY2NvdW50cxIhCgxwcm9ncm'
    'FtX2RhdGEYAyABKAxSC3Byb2dyYW1EYXRhGpoBChlNZXNzYWdlQWRkcmVzc1RhYmxlTG9va3Vw'
    'Eh8KC2FjY291bnRfa2V5GAEgASgJUgphY2NvdW50S2V5Ei0KEHdyaXRhYmxlX2luZGV4ZXMYAi'
    'ADKA1CAhABUg93cml0YWJsZUluZGV4ZXMSLQoQcmVhZG9ubHlfaW5kZXhlcxgDIAMoDUICEAFS'
    'D3JlYWRvbmx5SW5kZXhlcxrtAQoNTWVzc2FnZUxlZ2FjeRJBCgZoZWFkZXIYASABKAsyKS5UVy'
    '5Tb2xhbmEuUHJvdG8uUmF3TWVzc2FnZS5NZXNzYWdlSGVhZGVyUgZoZWFkZXISIQoMYWNjb3Vu'
    'dF9rZXlzGAIgAygJUgthY2NvdW50S2V5cxIpChByZWNlbnRfYmxvY2toYXNoGAMgASgJUg9yZW'
    'NlbnRCbG9ja2hhc2gSSwoMaW5zdHJ1Y3Rpb25zGAQgAygLMicuVFcuU29sYW5hLlByb3RvLlJh'
    'd01lc3NhZ2UuSW5zdHJ1Y3Rpb25SDGluc3RydWN0aW9ucxrUAgoJTWVzc2FnZVYwEkEKBmhlYW'
    'RlchgBIAEoCzIpLlRXLlNvbGFuYS5Qcm90by5SYXdNZXNzYWdlLk1lc3NhZ2VIZWFkZXJSBmhl'
    'YWRlchIhCgxhY2NvdW50X2tleXMYAiADKAlSC2FjY291bnRLZXlzEikKEHJlY2VudF9ibG9ja2'
    'hhc2gYAyABKAlSD3JlY2VudEJsb2NraGFzaBJLCgxpbnN0cnVjdGlvbnMYBCADKAsyJy5UVy5T'
    'b2xhbmEuUHJvdG8uUmF3TWVzc2FnZS5JbnN0cnVjdGlvblIMaW5zdHJ1Y3Rpb25zEmkKFWFkZH'
    'Jlc3NfdGFibGVfbG9va3VwcxgFIAMoCzI1LlRXLlNvbGFuYS5Qcm90by5SYXdNZXNzYWdlLk1l'
    'c3NhZ2VBZGRyZXNzVGFibGVMb29rdXBSE2FkZHJlc3NUYWJsZUxvb2t1cHNCCQoHbWVzc2FnZQ'
    '==');

@$core.Deprecated('Use decodingTransactionOutputDescriptor instead')
const DecodingTransactionOutput$json = {
  '1': 'DecodingTransactionOutput',
  '2': [
    {'1': 'transaction', '3': 1, '4': 1, '5': 11, '6': '.TW.Solana.Proto.RawMessage', '10': 'transaction'},
    {'1': 'error', '3': 2, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 3, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `DecodingTransactionOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List decodingTransactionOutputDescriptor = $convert.base64Decode(
    'ChlEZWNvZGluZ1RyYW5zYWN0aW9uT3V0cHV0Ej0KC3RyYW5zYWN0aW9uGAEgASgLMhsuVFcuU2'
    '9sYW5hLlByb3RvLlJhd01lc3NhZ2VSC3RyYW5zYWN0aW9uEjMKBWVycm9yGAIgASgOMh0uVFcu'
    'Q29tbW9uLlByb3RvLlNpZ25pbmdFcnJvclIFZXJyb3ISIwoNZXJyb3JfbWVzc2FnZRgDIAEoCV'
    'IMZXJyb3JNZXNzYWdl');

@$core.Deprecated('Use priorityFeePriceDescriptor instead')
const PriorityFeePrice$json = {
  '1': 'PriorityFeePrice',
  '2': [
    {'1': 'price', '3': 1, '4': 1, '5': 4, '10': 'price'},
  ],
};

/// Descriptor for `PriorityFeePrice`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List priorityFeePriceDescriptor = $convert.base64Decode(
    'ChBQcmlvcml0eUZlZVByaWNlEhQKBXByaWNlGAEgASgEUgVwcmljZQ==');

@$core.Deprecated('Use priorityFeeLimitDescriptor instead')
const PriorityFeeLimit$json = {
  '1': 'PriorityFeeLimit',
  '2': [
    {'1': 'limit', '3': 2, '4': 1, '5': 13, '10': 'limit'},
  ],
};

/// Descriptor for `PriorityFeeLimit`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List priorityFeeLimitDescriptor = $convert.base64Decode(
    'ChBQcmlvcml0eUZlZUxpbWl0EhQKBWxpbWl0GAIgASgNUgVsaW1pdA==');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'private_key', '3': 1, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'recent_blockhash', '3': 2, '4': 1, '5': 9, '10': 'recentBlockhash'},
    {'1': 'v0_msg', '3': 3, '4': 1, '5': 8, '10': 'v0Msg'},
    {'1': 'transfer_transaction', '3': 4, '4': 1, '5': 11, '6': '.TW.Solana.Proto.Transfer', '9': 0, '10': 'transferTransaction'},
    {'1': 'delegate_stake_transaction', '3': 5, '4': 1, '5': 11, '6': '.TW.Solana.Proto.DelegateStake', '9': 0, '10': 'delegateStakeTransaction'},
    {'1': 'deactivate_stake_transaction', '3': 6, '4': 1, '5': 11, '6': '.TW.Solana.Proto.DeactivateStake', '9': 0, '10': 'deactivateStakeTransaction'},
    {'1': 'deactivate_all_stake_transaction', '3': 7, '4': 1, '5': 11, '6': '.TW.Solana.Proto.DeactivateAllStake', '9': 0, '10': 'deactivateAllStakeTransaction'},
    {'1': 'withdraw_transaction', '3': 8, '4': 1, '5': 11, '6': '.TW.Solana.Proto.WithdrawStake', '9': 0, '10': 'withdrawTransaction'},
    {'1': 'withdraw_all_transaction', '3': 9, '4': 1, '5': 11, '6': '.TW.Solana.Proto.WithdrawAllStake', '9': 0, '10': 'withdrawAllTransaction'},
    {'1': 'create_token_account_transaction', '3': 10, '4': 1, '5': 11, '6': '.TW.Solana.Proto.CreateTokenAccount', '9': 0, '10': 'createTokenAccountTransaction'},
    {'1': 'token_transfer_transaction', '3': 11, '4': 1, '5': 11, '6': '.TW.Solana.Proto.TokenTransfer', '9': 0, '10': 'tokenTransferTransaction'},
    {'1': 'create_and_transfer_token_transaction', '3': 12, '4': 1, '5': 11, '6': '.TW.Solana.Proto.CreateAndTransferToken', '9': 0, '10': 'createAndTransferTokenTransaction'},
    {'1': 'create_nonce_account', '3': 13, '4': 1, '5': 11, '6': '.TW.Solana.Proto.CreateNonceAccount', '9': 0, '10': 'createNonceAccount'},
    {'1': 'withdraw_nonce_account', '3': 16, '4': 1, '5': 11, '6': '.TW.Solana.Proto.WithdrawNonceAccount', '9': 0, '10': 'withdrawNonceAccount'},
    {'1': 'advance_nonce_account', '3': 19, '4': 1, '5': 11, '6': '.TW.Solana.Proto.AdvanceNonceAccount', '9': 0, '10': 'advanceNonceAccount'},
    {'1': 'sender', '3': 14, '4': 1, '5': 9, '10': 'sender'},
    {'1': 'nonce_account', '3': 15, '4': 1, '5': 9, '10': 'nonceAccount'},
    {'1': 'fee_payer_private_key', '3': 17, '4': 1, '5': 12, '10': 'feePayerPrivateKey'},
    {'1': 'fee_payer', '3': 18, '4': 1, '5': 9, '10': 'feePayer'},
    {'1': 'raw_message', '3': 20, '4': 1, '5': 11, '6': '.TW.Solana.Proto.RawMessage', '10': 'rawMessage'},
    {'1': 'tx_encoding', '3': 21, '4': 1, '5': 14, '6': '.TW.Solana.Proto.Encoding', '10': 'txEncoding'},
    {'1': 'priority_fee_price', '3': 22, '4': 1, '5': 11, '6': '.TW.Solana.Proto.PriorityFeePrice', '10': 'priorityFeePrice'},
    {'1': 'priority_fee_limit', '3': 23, '4': 1, '5': 11, '6': '.TW.Solana.Proto.PriorityFeeLimit', '10': 'priorityFeeLimit'},
  ],
  '8': [
    {'1': 'transaction_type'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSHwoLcHJpdmF0ZV9rZXkYASABKAxSCnByaXZhdGVLZXkSKQoQcmVjZW'
    '50X2Jsb2NraGFzaBgCIAEoCVIPcmVjZW50QmxvY2toYXNoEhUKBnYwX21zZxgDIAEoCFIFdjBN'
    'c2cSTgoUdHJhbnNmZXJfdHJhbnNhY3Rpb24YBCABKAsyGS5UVy5Tb2xhbmEuUHJvdG8uVHJhbn'
    'NmZXJIAFITdHJhbnNmZXJUcmFuc2FjdGlvbhJeChpkZWxlZ2F0ZV9zdGFrZV90cmFuc2FjdGlv'
    'bhgFIAEoCzIeLlRXLlNvbGFuYS5Qcm90by5EZWxlZ2F0ZVN0YWtlSABSGGRlbGVnYXRlU3Rha2'
    'VUcmFuc2FjdGlvbhJkChxkZWFjdGl2YXRlX3N0YWtlX3RyYW5zYWN0aW9uGAYgASgLMiAuVFcu'
    'U29sYW5hLlByb3RvLkRlYWN0aXZhdGVTdGFrZUgAUhpkZWFjdGl2YXRlU3Rha2VUcmFuc2FjdG'
    'lvbhJuCiBkZWFjdGl2YXRlX2FsbF9zdGFrZV90cmFuc2FjdGlvbhgHIAEoCzIjLlRXLlNvbGFu'
    'YS5Qcm90by5EZWFjdGl2YXRlQWxsU3Rha2VIAFIdZGVhY3RpdmF0ZUFsbFN0YWtlVHJhbnNhY3'
    'Rpb24SUwoUd2l0aGRyYXdfdHJhbnNhY3Rpb24YCCABKAsyHi5UVy5Tb2xhbmEuUHJvdG8uV2l0'
    'aGRyYXdTdGFrZUgAUhN3aXRoZHJhd1RyYW5zYWN0aW9uEl0KGHdpdGhkcmF3X2FsbF90cmFuc2'
    'FjdGlvbhgJIAEoCzIhLlRXLlNvbGFuYS5Qcm90by5XaXRoZHJhd0FsbFN0YWtlSABSFndpdGhk'
    'cmF3QWxsVHJhbnNhY3Rpb24SbgogY3JlYXRlX3Rva2VuX2FjY291bnRfdHJhbnNhY3Rpb24YCi'
    'ABKAsyIy5UVy5Tb2xhbmEuUHJvdG8uQ3JlYXRlVG9rZW5BY2NvdW50SABSHWNyZWF0ZVRva2Vu'
    'QWNjb3VudFRyYW5zYWN0aW9uEl4KGnRva2VuX3RyYW5zZmVyX3RyYW5zYWN0aW9uGAsgASgLMh'
    '4uVFcuU29sYW5hLlByb3RvLlRva2VuVHJhbnNmZXJIAFIYdG9rZW5UcmFuc2ZlclRyYW5zYWN0'
    'aW9uEnsKJWNyZWF0ZV9hbmRfdHJhbnNmZXJfdG9rZW5fdHJhbnNhY3Rpb24YDCABKAsyJy5UVy'
    '5Tb2xhbmEuUHJvdG8uQ3JlYXRlQW5kVHJhbnNmZXJUb2tlbkgAUiFjcmVhdGVBbmRUcmFuc2Zl'
    'clRva2VuVHJhbnNhY3Rpb24SVwoUY3JlYXRlX25vbmNlX2FjY291bnQYDSABKAsyIy5UVy5Tb2'
    'xhbmEuUHJvdG8uQ3JlYXRlTm9uY2VBY2NvdW50SABSEmNyZWF0ZU5vbmNlQWNjb3VudBJdChZ3'
    'aXRoZHJhd19ub25jZV9hY2NvdW50GBAgASgLMiUuVFcuU29sYW5hLlByb3RvLldpdGhkcmF3Tm'
    '9uY2VBY2NvdW50SABSFHdpdGhkcmF3Tm9uY2VBY2NvdW50EloKFWFkdmFuY2Vfbm9uY2VfYWNj'
    'b3VudBgTIAEoCzIkLlRXLlNvbGFuYS5Qcm90by5BZHZhbmNlTm9uY2VBY2NvdW50SABSE2Fkdm'
    'FuY2VOb25jZUFjY291bnQSFgoGc2VuZGVyGA4gASgJUgZzZW5kZXISIwoNbm9uY2VfYWNjb3Vu'
    'dBgPIAEoCVIMbm9uY2VBY2NvdW50EjEKFWZlZV9wYXllcl9wcml2YXRlX2tleRgRIAEoDFISZm'
    'VlUGF5ZXJQcml2YXRlS2V5EhsKCWZlZV9wYXllchgSIAEoCVIIZmVlUGF5ZXISPAoLcmF3X21l'
    'c3NhZ2UYFCABKAsyGy5UVy5Tb2xhbmEuUHJvdG8uUmF3TWVzc2FnZVIKcmF3TWVzc2FnZRI6Cg'
    't0eF9lbmNvZGluZxgVIAEoDjIZLlRXLlNvbGFuYS5Qcm90by5FbmNvZGluZ1IKdHhFbmNvZGlu'
    'ZxJPChJwcmlvcml0eV9mZWVfcHJpY2UYFiABKAsyIS5UVy5Tb2xhbmEuUHJvdG8uUHJpb3JpdH'
    'lGZWVQcmljZVIQcHJpb3JpdHlGZWVQcmljZRJPChJwcmlvcml0eV9mZWVfbGltaXQYFyABKAsy'
    'IS5UVy5Tb2xhbmEuUHJvdG8uUHJpb3JpdHlGZWVMaW1pdFIQcHJpb3JpdHlGZWVMaW1pdEISCh'
    'B0cmFuc2FjdGlvbl90eXBl');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 9, '10': 'encoded'},
    {'1': 'error', '3': 2, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 3, '4': 1, '5': 9, '10': 'errorMessage'},
    {'1': 'unsigned_tx', '3': 4, '4': 1, '5': 9, '10': 'unsignedTx'},
    {'1': 'signatures', '3': 5, '4': 3, '5': 11, '6': '.TW.Solana.Proto.PubkeySignature', '10': 'signatures'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhgKB2VuY29kZWQYASABKAlSB2VuY29kZWQSMwoFZXJyb3IYAiABKA'
    '4yHS5UVy5Db21tb24uUHJvdG8uU2lnbmluZ0Vycm9yUgVlcnJvchIjCg1lcnJvcl9tZXNzYWdl'
    'GAMgASgJUgxlcnJvck1lc3NhZ2USHwoLdW5zaWduZWRfdHgYBCABKAlSCnVuc2lnbmVkVHgSQA'
    'oKc2lnbmF0dXJlcxgFIAMoCzIgLlRXLlNvbGFuYS5Qcm90by5QdWJrZXlTaWduYXR1cmVSCnNp'
    'Z25hdHVyZXM=');

@$core.Deprecated('Use preSigningOutputDescriptor instead')
const PreSigningOutput$json = {
  '1': 'PreSigningOutput',
  '2': [
    {'1': 'signers', '3': 1, '4': 3, '5': 12, '10': 'signers'},
    {'1': 'data', '3': 2, '4': 1, '5': 12, '10': 'data'},
    {'1': 'error', '3': 3, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 4, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `PreSigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List preSigningOutputDescriptor = $convert.base64Decode(
    'ChBQcmVTaWduaW5nT3V0cHV0EhgKB3NpZ25lcnMYASADKAxSB3NpZ25lcnMSEgoEZGF0YRgCIA'
    'EoDFIEZGF0YRIzCgVlcnJvchgDIAEoDjIdLlRXLkNvbW1vbi5Qcm90by5TaWduaW5nRXJyb3JS'
    'BWVycm9yEiMKDWVycm9yX21lc3NhZ2UYBCABKAlSDGVycm9yTWVzc2FnZQ==');

