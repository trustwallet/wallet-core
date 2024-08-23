//
//  Generated code. Do not modify.
//  source: Aptos.proto
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
    {'1': 'to', '3': 1, '4': 1, '5': 9, '10': 'to'},
    {'1': 'amount', '3': 2, '4': 1, '5': 4, '10': 'amount'},
  ],
};

/// Descriptor for `TransferMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transferMessageDescriptor = $convert.base64Decode(
    'Cg9UcmFuc2Zlck1lc3NhZ2USDgoCdG8YASABKAlSAnRvEhYKBmFtb3VudBgCIAEoBFIGYW1vdW'
    '50');

@$core.Deprecated('Use structTagDescriptor instead')
const StructTag$json = {
  '1': 'StructTag',
  '2': [
    {'1': 'account_address', '3': 1, '4': 1, '5': 9, '10': 'accountAddress'},
    {'1': 'module', '3': 2, '4': 1, '5': 9, '10': 'module'},
    {'1': 'name', '3': 3, '4': 1, '5': 9, '10': 'name'},
  ],
};

/// Descriptor for `StructTag`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List structTagDescriptor = $convert.base64Decode(
    'CglTdHJ1Y3RUYWcSJwoPYWNjb3VudF9hZGRyZXNzGAEgASgJUg5hY2NvdW50QWRkcmVzcxIWCg'
    'Ztb2R1bGUYAiABKAlSBm1vZHVsZRISCgRuYW1lGAMgASgJUgRuYW1l');

@$core.Deprecated('Use tokenTransferMessageDescriptor instead')
const TokenTransferMessage$json = {
  '1': 'TokenTransferMessage',
  '2': [
    {'1': 'to', '3': 1, '4': 1, '5': 9, '10': 'to'},
    {'1': 'amount', '3': 2, '4': 1, '5': 4, '10': 'amount'},
    {'1': 'function', '3': 3, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.StructTag', '10': 'function'},
  ],
};

/// Descriptor for `TokenTransferMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tokenTransferMessageDescriptor = $convert.base64Decode(
    'ChRUb2tlblRyYW5zZmVyTWVzc2FnZRIOCgJ0bxgBIAEoCVICdG8SFgoGYW1vdW50GAIgASgEUg'
    'ZhbW91bnQSNQoIZnVuY3Rpb24YAyABKAsyGS5UVy5BcHRvcy5Qcm90by5TdHJ1Y3RUYWdSCGZ1'
    'bmN0aW9u');

@$core.Deprecated('Use tokenTransferCoinsMessageDescriptor instead')
const TokenTransferCoinsMessage$json = {
  '1': 'TokenTransferCoinsMessage',
  '2': [
    {'1': 'to', '3': 1, '4': 1, '5': 9, '10': 'to'},
    {'1': 'amount', '3': 2, '4': 1, '5': 4, '10': 'amount'},
    {'1': 'function', '3': 3, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.StructTag', '10': 'function'},
  ],
};

/// Descriptor for `TokenTransferCoinsMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tokenTransferCoinsMessageDescriptor = $convert.base64Decode(
    'ChlUb2tlblRyYW5zZmVyQ29pbnNNZXNzYWdlEg4KAnRvGAEgASgJUgJ0bxIWCgZhbW91bnQYAi'
    'ABKARSBmFtb3VudBI1CghmdW5jdGlvbhgDIAEoCzIZLlRXLkFwdG9zLlByb3RvLlN0cnVjdFRh'
    'Z1IIZnVuY3Rpb24=');

@$core.Deprecated('Use managedTokensRegisterMessageDescriptor instead')
const ManagedTokensRegisterMessage$json = {
  '1': 'ManagedTokensRegisterMessage',
  '2': [
    {'1': 'function', '3': 1, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.StructTag', '10': 'function'},
  ],
};

/// Descriptor for `ManagedTokensRegisterMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List managedTokensRegisterMessageDescriptor = $convert.base64Decode(
    'ChxNYW5hZ2VkVG9rZW5zUmVnaXN0ZXJNZXNzYWdlEjUKCGZ1bmN0aW9uGAEgASgLMhkuVFcuQX'
    'B0b3MuUHJvdG8uU3RydWN0VGFnUghmdW5jdGlvbg==');

@$core.Deprecated('Use createAccountMessageDescriptor instead')
const CreateAccountMessage$json = {
  '1': 'CreateAccountMessage',
  '2': [
    {'1': 'auth_key', '3': 1, '4': 1, '5': 9, '10': 'authKey'},
  ],
};

/// Descriptor for `CreateAccountMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List createAccountMessageDescriptor = $convert.base64Decode(
    'ChRDcmVhdGVBY2NvdW50TWVzc2FnZRIZCghhdXRoX2tleRgBIAEoCVIHYXV0aEtleQ==');

@$core.Deprecated('Use offerNftMessageDescriptor instead')
const OfferNftMessage$json = {
  '1': 'OfferNftMessage',
  '2': [
    {'1': 'receiver', '3': 1, '4': 1, '5': 9, '10': 'receiver'},
    {'1': 'creator', '3': 2, '4': 1, '5': 9, '10': 'creator'},
    {'1': 'collectionName', '3': 3, '4': 1, '5': 9, '10': 'collectionName'},
    {'1': 'name', '3': 4, '4': 1, '5': 9, '10': 'name'},
    {'1': 'property_version', '3': 5, '4': 1, '5': 4, '10': 'propertyVersion'},
    {'1': 'amount', '3': 6, '4': 1, '5': 4, '10': 'amount'},
  ],
};

/// Descriptor for `OfferNftMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List offerNftMessageDescriptor = $convert.base64Decode(
    'Cg9PZmZlck5mdE1lc3NhZ2USGgoIcmVjZWl2ZXIYASABKAlSCHJlY2VpdmVyEhgKB2NyZWF0b3'
    'IYAiABKAlSB2NyZWF0b3ISJgoOY29sbGVjdGlvbk5hbWUYAyABKAlSDmNvbGxlY3Rpb25OYW1l'
    'EhIKBG5hbWUYBCABKAlSBG5hbWUSKQoQcHJvcGVydHlfdmVyc2lvbhgFIAEoBFIPcHJvcGVydH'
    'lWZXJzaW9uEhYKBmFtb3VudBgGIAEoBFIGYW1vdW50');

@$core.Deprecated('Use cancelOfferNftMessageDescriptor instead')
const CancelOfferNftMessage$json = {
  '1': 'CancelOfferNftMessage',
  '2': [
    {'1': 'receiver', '3': 1, '4': 1, '5': 9, '10': 'receiver'},
    {'1': 'creator', '3': 2, '4': 1, '5': 9, '10': 'creator'},
    {'1': 'collectionName', '3': 3, '4': 1, '5': 9, '10': 'collectionName'},
    {'1': 'name', '3': 4, '4': 1, '5': 9, '10': 'name'},
    {'1': 'property_version', '3': 5, '4': 1, '5': 4, '10': 'propertyVersion'},
  ],
};

/// Descriptor for `CancelOfferNftMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List cancelOfferNftMessageDescriptor = $convert.base64Decode(
    'ChVDYW5jZWxPZmZlck5mdE1lc3NhZ2USGgoIcmVjZWl2ZXIYASABKAlSCHJlY2VpdmVyEhgKB2'
    'NyZWF0b3IYAiABKAlSB2NyZWF0b3ISJgoOY29sbGVjdGlvbk5hbWUYAyABKAlSDmNvbGxlY3Rp'
    'b25OYW1lEhIKBG5hbWUYBCABKAlSBG5hbWUSKQoQcHJvcGVydHlfdmVyc2lvbhgFIAEoBFIPcH'
    'JvcGVydHlWZXJzaW9u');

@$core.Deprecated('Use claimNftMessageDescriptor instead')
const ClaimNftMessage$json = {
  '1': 'ClaimNftMessage',
  '2': [
    {'1': 'sender', '3': 1, '4': 1, '5': 9, '10': 'sender'},
    {'1': 'creator', '3': 2, '4': 1, '5': 9, '10': 'creator'},
    {'1': 'collectionName', '3': 3, '4': 1, '5': 9, '10': 'collectionName'},
    {'1': 'name', '3': 4, '4': 1, '5': 9, '10': 'name'},
    {'1': 'property_version', '3': 5, '4': 1, '5': 4, '10': 'propertyVersion'},
  ],
};

/// Descriptor for `ClaimNftMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List claimNftMessageDescriptor = $convert.base64Decode(
    'Cg9DbGFpbU5mdE1lc3NhZ2USFgoGc2VuZGVyGAEgASgJUgZzZW5kZXISGAoHY3JlYXRvchgCIA'
    'EoCVIHY3JlYXRvchImCg5jb2xsZWN0aW9uTmFtZRgDIAEoCVIOY29sbGVjdGlvbk5hbWUSEgoE'
    'bmFtZRgEIAEoCVIEbmFtZRIpChBwcm9wZXJ0eV92ZXJzaW9uGAUgASgEUg9wcm9wZXJ0eVZlcn'
    'Npb24=');

@$core.Deprecated('Use tortugaClaimDescriptor instead')
const TortugaClaim$json = {
  '1': 'TortugaClaim',
  '2': [
    {'1': 'idx', '3': 1, '4': 1, '5': 4, '10': 'idx'},
  ],
};

/// Descriptor for `TortugaClaim`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tortugaClaimDescriptor = $convert.base64Decode(
    'CgxUb3J0dWdhQ2xhaW0SEAoDaWR4GAEgASgEUgNpZHg=');

@$core.Deprecated('Use tortugaStakeDescriptor instead')
const TortugaStake$json = {
  '1': 'TortugaStake',
  '2': [
    {'1': 'amount', '3': 1, '4': 1, '5': 4, '10': 'amount'},
  ],
};

/// Descriptor for `TortugaStake`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tortugaStakeDescriptor = $convert.base64Decode(
    'CgxUb3J0dWdhU3Rha2USFgoGYW1vdW50GAEgASgEUgZhbW91bnQ=');

@$core.Deprecated('Use tortugaUnstakeDescriptor instead')
const TortugaUnstake$json = {
  '1': 'TortugaUnstake',
  '2': [
    {'1': 'amount', '3': 1, '4': 1, '5': 4, '10': 'amount'},
  ],
};

/// Descriptor for `TortugaUnstake`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tortugaUnstakeDescriptor = $convert.base64Decode(
    'Cg5Ub3J0dWdhVW5zdGFrZRIWCgZhbW91bnQYASABKARSBmFtb3VudA==');

@$core.Deprecated('Use liquidStakingDescriptor instead')
const LiquidStaking$json = {
  '1': 'LiquidStaking',
  '2': [
    {'1': 'smart_contract_address', '3': 1, '4': 1, '5': 9, '10': 'smartContractAddress'},
    {'1': 'stake', '3': 2, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.TortugaStake', '9': 0, '10': 'stake'},
    {'1': 'unstake', '3': 3, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.TortugaUnstake', '9': 0, '10': 'unstake'},
    {'1': 'claim', '3': 4, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.TortugaClaim', '9': 0, '10': 'claim'},
  ],
  '8': [
    {'1': 'liquid_stake_transaction_payload'},
  ],
};

/// Descriptor for `LiquidStaking`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List liquidStakingDescriptor = $convert.base64Decode(
    'Cg1MaXF1aWRTdGFraW5nEjQKFnNtYXJ0X2NvbnRyYWN0X2FkZHJlc3MYASABKAlSFHNtYXJ0Q2'
    '9udHJhY3RBZGRyZXNzEjQKBXN0YWtlGAIgASgLMhwuVFcuQXB0b3MuUHJvdG8uVG9ydHVnYVN0'
    'YWtlSABSBXN0YWtlEjoKB3Vuc3Rha2UYAyABKAsyHi5UVy5BcHRvcy5Qcm90by5Ub3J0dWdhVW'
    '5zdGFrZUgAUgd1bnN0YWtlEjQKBWNsYWltGAQgASgLMhwuVFcuQXB0b3MuUHJvdG8uVG9ydHVn'
    'YUNsYWltSABSBWNsYWltQiIKIGxpcXVpZF9zdGFrZV90cmFuc2FjdGlvbl9wYXlsb2Fk');

@$core.Deprecated('Use nftMessageDescriptor instead')
const NftMessage$json = {
  '1': 'NftMessage',
  '2': [
    {'1': 'offer_nft', '3': 1, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.OfferNftMessage', '9': 0, '10': 'offerNft'},
    {'1': 'cancel_offer_nft', '3': 2, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.CancelOfferNftMessage', '9': 0, '10': 'cancelOfferNft'},
    {'1': 'claim_nft', '3': 3, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.ClaimNftMessage', '9': 0, '10': 'claimNft'},
  ],
  '8': [
    {'1': 'nft_transaction_payload'},
  ],
};

/// Descriptor for `NftMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List nftMessageDescriptor = $convert.base64Decode(
    'CgpOZnRNZXNzYWdlEj4KCW9mZmVyX25mdBgBIAEoCzIfLlRXLkFwdG9zLlByb3RvLk9mZmVyTm'
    'Z0TWVzc2FnZUgAUghvZmZlck5mdBJRChBjYW5jZWxfb2ZmZXJfbmZ0GAIgASgLMiUuVFcuQXB0'
    'b3MuUHJvdG8uQ2FuY2VsT2ZmZXJOZnRNZXNzYWdlSABSDmNhbmNlbE9mZmVyTmZ0Ej4KCWNsYW'
    'ltX25mdBgDIAEoCzIfLlRXLkFwdG9zLlByb3RvLkNsYWltTmZ0TWVzc2FnZUgAUghjbGFpbU5m'
    'dEIZChduZnRfdHJhbnNhY3Rpb25fcGF5bG9hZA==');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'sender', '3': 1, '4': 1, '5': 9, '10': 'sender'},
    {'1': 'sequence_number', '3': 2, '4': 1, '5': 3, '10': 'sequenceNumber'},
    {'1': 'max_gas_amount', '3': 3, '4': 1, '5': 4, '10': 'maxGasAmount'},
    {'1': 'gas_unit_price', '3': 4, '4': 1, '5': 4, '10': 'gasUnitPrice'},
    {'1': 'expiration_timestamp_secs', '3': 5, '4': 1, '5': 4, '10': 'expirationTimestampSecs'},
    {'1': 'chain_id', '3': 6, '4': 1, '5': 13, '10': 'chainId'},
    {'1': 'private_key', '3': 7, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'any_encoded', '3': 8, '4': 1, '5': 9, '10': 'anyEncoded'},
    {'1': 'transfer', '3': 9, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.TransferMessage', '9': 0, '10': 'transfer'},
    {'1': 'token_transfer', '3': 10, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.TokenTransferMessage', '9': 0, '10': 'tokenTransfer'},
    {'1': 'create_account', '3': 11, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.CreateAccountMessage', '9': 0, '10': 'createAccount'},
    {'1': 'nft_message', '3': 12, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.NftMessage', '9': 0, '10': 'nftMessage'},
    {'1': 'register_token', '3': 13, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.ManagedTokensRegisterMessage', '9': 0, '10': 'registerToken'},
    {'1': 'liquid_staking_message', '3': 14, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.LiquidStaking', '9': 0, '10': 'liquidStakingMessage'},
    {'1': 'token_transfer_coins', '3': 15, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.TokenTransferCoinsMessage', '9': 0, '10': 'tokenTransferCoins'},
  ],
  '8': [
    {'1': 'transaction_payload'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSFgoGc2VuZGVyGAEgASgJUgZzZW5kZXISJwoPc2VxdWVuY2VfbnVtYm'
    'VyGAIgASgDUg5zZXF1ZW5jZU51bWJlchIkCg5tYXhfZ2FzX2Ftb3VudBgDIAEoBFIMbWF4R2Fz'
    'QW1vdW50EiQKDmdhc191bml0X3ByaWNlGAQgASgEUgxnYXNVbml0UHJpY2USOgoZZXhwaXJhdG'
    'lvbl90aW1lc3RhbXBfc2VjcxgFIAEoBFIXZXhwaXJhdGlvblRpbWVzdGFtcFNlY3MSGQoIY2hh'
    'aW5faWQYBiABKA1SB2NoYWluSWQSHwoLcHJpdmF0ZV9rZXkYByABKAxSCnByaXZhdGVLZXkSHw'
    'oLYW55X2VuY29kZWQYCCABKAlSCmFueUVuY29kZWQSPQoIdHJhbnNmZXIYCSABKAsyHy5UVy5B'
    'cHRvcy5Qcm90by5UcmFuc2Zlck1lc3NhZ2VIAFIIdHJhbnNmZXISTQoOdG9rZW5fdHJhbnNmZX'
    'IYCiABKAsyJC5UVy5BcHRvcy5Qcm90by5Ub2tlblRyYW5zZmVyTWVzc2FnZUgAUg10b2tlblRy'
    'YW5zZmVyEk0KDmNyZWF0ZV9hY2NvdW50GAsgASgLMiQuVFcuQXB0b3MuUHJvdG8uQ3JlYXRlQW'
    'Njb3VudE1lc3NhZ2VIAFINY3JlYXRlQWNjb3VudBI9CgtuZnRfbWVzc2FnZRgMIAEoCzIaLlRX'
    'LkFwdG9zLlByb3RvLk5mdE1lc3NhZ2VIAFIKbmZ0TWVzc2FnZRJVCg5yZWdpc3Rlcl90b2tlbh'
    'gNIAEoCzIsLlRXLkFwdG9zLlByb3RvLk1hbmFnZWRUb2tlbnNSZWdpc3Rlck1lc3NhZ2VIAFIN'
    'cmVnaXN0ZXJUb2tlbhJVChZsaXF1aWRfc3Rha2luZ19tZXNzYWdlGA4gASgLMh0uVFcuQXB0b3'
    'MuUHJvdG8uTGlxdWlkU3Rha2luZ0gAUhRsaXF1aWRTdGFraW5nTWVzc2FnZRJdChR0b2tlbl90'
    'cmFuc2Zlcl9jb2lucxgPIAEoCzIpLlRXLkFwdG9zLlByb3RvLlRva2VuVHJhbnNmZXJDb2luc0'
    '1lc3NhZ2VIAFISdG9rZW5UcmFuc2ZlckNvaW5zQhUKE3RyYW5zYWN0aW9uX3BheWxvYWQ=');

@$core.Deprecated('Use transactionAuthenticatorDescriptor instead')
const TransactionAuthenticator$json = {
  '1': 'TransactionAuthenticator',
  '2': [
    {'1': 'signature', '3': 1, '4': 1, '5': 12, '10': 'signature'},
    {'1': 'public_key', '3': 2, '4': 1, '5': 12, '10': 'publicKey'},
  ],
};

/// Descriptor for `TransactionAuthenticator`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionAuthenticatorDescriptor = $convert.base64Decode(
    'ChhUcmFuc2FjdGlvbkF1dGhlbnRpY2F0b3ISHAoJc2lnbmF0dXJlGAEgASgMUglzaWduYXR1cm'
    'USHQoKcHVibGljX2tleRgCIAEoDFIJcHVibGljS2V5');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'raw_txn', '3': 1, '4': 1, '5': 12, '10': 'rawTxn'},
    {'1': 'authenticator', '3': 2, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.TransactionAuthenticator', '10': 'authenticator'},
    {'1': 'encoded', '3': 3, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'json', '3': 4, '4': 1, '5': 9, '10': 'json'},
    {'1': 'error', '3': 5, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 6, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhcKB3Jhd190eG4YASABKAxSBnJhd1R4bhJOCg1hdXRoZW50aWNhdG'
    '9yGAIgASgLMiguVFcuQXB0b3MuUHJvdG8uVHJhbnNhY3Rpb25BdXRoZW50aWNhdG9yUg1hdXRo'
    'ZW50aWNhdG9yEhgKB2VuY29kZWQYAyABKAxSB2VuY29kZWQSEgoEanNvbhgEIAEoCVIEanNvbh'
    'IzCgVlcnJvchgFIAEoDjIdLlRXLkNvbW1vbi5Qcm90by5TaWduaW5nRXJyb3JSBWVycm9yEiMK'
    'DWVycm9yX21lc3NhZ2UYBiABKAlSDGVycm9yTWVzc2FnZQ==');

