//
//  Generated code. Do not modify.
//  source: NEAR.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use publicKeyDescriptor instead')
const PublicKey$json = {
  '1': 'PublicKey',
  '2': [
    {'1': 'key_type', '3': 1, '4': 1, '5': 13, '10': 'keyType'},
    {'1': 'data', '3': 2, '4': 1, '5': 12, '10': 'data'},
  ],
};

/// Descriptor for `PublicKey`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List publicKeyDescriptor = $convert.base64Decode(
    'CglQdWJsaWNLZXkSGQoIa2V5X3R5cGUYASABKA1SB2tleVR5cGUSEgoEZGF0YRgCIAEoDFIEZG'
    'F0YQ==');

@$core.Deprecated('Use functionCallPermissionDescriptor instead')
const FunctionCallPermission$json = {
  '1': 'FunctionCallPermission',
  '2': [
    {'1': 'allowance', '3': 1, '4': 1, '5': 12, '10': 'allowance'},
    {'1': 'receiver_id', '3': 2, '4': 1, '5': 9, '10': 'receiverId'},
    {'1': 'method_names', '3': 3, '4': 3, '5': 9, '10': 'methodNames'},
  ],
};

/// Descriptor for `FunctionCallPermission`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List functionCallPermissionDescriptor = $convert.base64Decode(
    'ChZGdW5jdGlvbkNhbGxQZXJtaXNzaW9uEhwKCWFsbG93YW5jZRgBIAEoDFIJYWxsb3dhbmNlEh'
    '8KC3JlY2VpdmVyX2lkGAIgASgJUgpyZWNlaXZlcklkEiEKDG1ldGhvZF9uYW1lcxgDIAMoCVIL'
    'bWV0aG9kTmFtZXM=');

@$core.Deprecated('Use fullAccessPermissionDescriptor instead')
const FullAccessPermission$json = {
  '1': 'FullAccessPermission',
};

/// Descriptor for `FullAccessPermission`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List fullAccessPermissionDescriptor = $convert.base64Decode(
    'ChRGdWxsQWNjZXNzUGVybWlzc2lvbg==');

@$core.Deprecated('Use accessKeyDescriptor instead')
const AccessKey$json = {
  '1': 'AccessKey',
  '2': [
    {'1': 'nonce', '3': 1, '4': 1, '5': 4, '10': 'nonce'},
    {'1': 'function_call', '3': 2, '4': 1, '5': 11, '6': '.TW.NEAR.Proto.FunctionCallPermission', '9': 0, '10': 'functionCall'},
    {'1': 'full_access', '3': 3, '4': 1, '5': 11, '6': '.TW.NEAR.Proto.FullAccessPermission', '9': 0, '10': 'fullAccess'},
  ],
  '8': [
    {'1': 'permission'},
  ],
};

/// Descriptor for `AccessKey`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List accessKeyDescriptor = $convert.base64Decode(
    'CglBY2Nlc3NLZXkSFAoFbm9uY2UYASABKARSBW5vbmNlEkwKDWZ1bmN0aW9uX2NhbGwYAiABKA'
    'syJS5UVy5ORUFSLlByb3RvLkZ1bmN0aW9uQ2FsbFBlcm1pc3Npb25IAFIMZnVuY3Rpb25DYWxs'
    'EkYKC2Z1bGxfYWNjZXNzGAMgASgLMiMuVFcuTkVBUi5Qcm90by5GdWxsQWNjZXNzUGVybWlzc2'
    'lvbkgAUgpmdWxsQWNjZXNzQgwKCnBlcm1pc3Npb24=');

@$core.Deprecated('Use createAccountDescriptor instead')
const CreateAccount$json = {
  '1': 'CreateAccount',
};

/// Descriptor for `CreateAccount`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List createAccountDescriptor = $convert.base64Decode(
    'Cg1DcmVhdGVBY2NvdW50');

@$core.Deprecated('Use deployContractDescriptor instead')
const DeployContract$json = {
  '1': 'DeployContract',
  '2': [
    {'1': 'code', '3': 1, '4': 1, '5': 12, '10': 'code'},
  ],
};

/// Descriptor for `DeployContract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List deployContractDescriptor = $convert.base64Decode(
    'Cg5EZXBsb3lDb250cmFjdBISCgRjb2RlGAEgASgMUgRjb2Rl');

@$core.Deprecated('Use functionCallDescriptor instead')
const FunctionCall$json = {
  '1': 'FunctionCall',
  '2': [
    {'1': 'method_name', '3': 1, '4': 1, '5': 9, '10': 'methodName'},
    {'1': 'args', '3': 2, '4': 1, '5': 12, '10': 'args'},
    {'1': 'gas', '3': 3, '4': 1, '5': 4, '10': 'gas'},
    {'1': 'deposit', '3': 4, '4': 1, '5': 12, '10': 'deposit'},
  ],
};

/// Descriptor for `FunctionCall`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List functionCallDescriptor = $convert.base64Decode(
    'CgxGdW5jdGlvbkNhbGwSHwoLbWV0aG9kX25hbWUYASABKAlSCm1ldGhvZE5hbWUSEgoEYXJncx'
    'gCIAEoDFIEYXJncxIQCgNnYXMYAyABKARSA2dhcxIYCgdkZXBvc2l0GAQgASgMUgdkZXBvc2l0');

@$core.Deprecated('Use transferDescriptor instead')
const Transfer$json = {
  '1': 'Transfer',
  '2': [
    {'1': 'deposit', '3': 1, '4': 1, '5': 12, '10': 'deposit'},
  ],
};

/// Descriptor for `Transfer`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transferDescriptor = $convert.base64Decode(
    'CghUcmFuc2ZlchIYCgdkZXBvc2l0GAEgASgMUgdkZXBvc2l0');

@$core.Deprecated('Use stakeDescriptor instead')
const Stake$json = {
  '1': 'Stake',
  '2': [
    {'1': 'stake', '3': 1, '4': 1, '5': 12, '10': 'stake'},
    {'1': 'public_key', '3': 2, '4': 1, '5': 11, '6': '.TW.NEAR.Proto.PublicKey', '10': 'publicKey'},
  ],
};

/// Descriptor for `Stake`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List stakeDescriptor = $convert.base64Decode(
    'CgVTdGFrZRIUCgVzdGFrZRgBIAEoDFIFc3Rha2USNwoKcHVibGljX2tleRgCIAEoCzIYLlRXLk'
    '5FQVIuUHJvdG8uUHVibGljS2V5UglwdWJsaWNLZXk=');

@$core.Deprecated('Use addKeyDescriptor instead')
const AddKey$json = {
  '1': 'AddKey',
  '2': [
    {'1': 'public_key', '3': 1, '4': 1, '5': 11, '6': '.TW.NEAR.Proto.PublicKey', '10': 'publicKey'},
    {'1': 'access_key', '3': 2, '4': 1, '5': 11, '6': '.TW.NEAR.Proto.AccessKey', '10': 'accessKey'},
  ],
};

/// Descriptor for `AddKey`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List addKeyDescriptor = $convert.base64Decode(
    'CgZBZGRLZXkSNwoKcHVibGljX2tleRgBIAEoCzIYLlRXLk5FQVIuUHJvdG8uUHVibGljS2V5Ug'
    'lwdWJsaWNLZXkSNwoKYWNjZXNzX2tleRgCIAEoCzIYLlRXLk5FQVIuUHJvdG8uQWNjZXNzS2V5'
    'UglhY2Nlc3NLZXk=');

@$core.Deprecated('Use deleteKeyDescriptor instead')
const DeleteKey$json = {
  '1': 'DeleteKey',
  '2': [
    {'1': 'public_key', '3': 1, '4': 1, '5': 11, '6': '.TW.NEAR.Proto.PublicKey', '10': 'publicKey'},
  ],
};

/// Descriptor for `DeleteKey`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List deleteKeyDescriptor = $convert.base64Decode(
    'CglEZWxldGVLZXkSNwoKcHVibGljX2tleRgBIAEoCzIYLlRXLk5FQVIuUHJvdG8uUHVibGljS2'
    'V5UglwdWJsaWNLZXk=');

@$core.Deprecated('Use deleteAccountDescriptor instead')
const DeleteAccount$json = {
  '1': 'DeleteAccount',
  '2': [
    {'1': 'beneficiary_id', '3': 1, '4': 1, '5': 9, '10': 'beneficiaryId'},
  ],
};

/// Descriptor for `DeleteAccount`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List deleteAccountDescriptor = $convert.base64Decode(
    'Cg1EZWxldGVBY2NvdW50EiUKDmJlbmVmaWNpYXJ5X2lkGAEgASgJUg1iZW5lZmljaWFyeUlk');

@$core.Deprecated('Use tokenTransferDescriptor instead')
const TokenTransfer$json = {
  '1': 'TokenTransfer',
  '2': [
    {'1': 'token_amount', '3': 1, '4': 1, '5': 9, '10': 'tokenAmount'},
    {'1': 'receiver_id', '3': 2, '4': 1, '5': 9, '10': 'receiverId'},
    {'1': 'gas', '3': 3, '4': 1, '5': 4, '10': 'gas'},
    {'1': 'deposit', '3': 4, '4': 1, '5': 12, '10': 'deposit'},
  ],
};

/// Descriptor for `TokenTransfer`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tokenTransferDescriptor = $convert.base64Decode(
    'Cg1Ub2tlblRyYW5zZmVyEiEKDHRva2VuX2Ftb3VudBgBIAEoCVILdG9rZW5BbW91bnQSHwoLcm'
    'VjZWl2ZXJfaWQYAiABKAlSCnJlY2VpdmVySWQSEAoDZ2FzGAMgASgEUgNnYXMSGAoHZGVwb3Np'
    'dBgEIAEoDFIHZGVwb3NpdA==');

@$core.Deprecated('Use actionDescriptor instead')
const Action$json = {
  '1': 'Action',
  '2': [
    {'1': 'create_account', '3': 1, '4': 1, '5': 11, '6': '.TW.NEAR.Proto.CreateAccount', '9': 0, '10': 'createAccount'},
    {'1': 'deploy_contract', '3': 2, '4': 1, '5': 11, '6': '.TW.NEAR.Proto.DeployContract', '9': 0, '10': 'deployContract'},
    {'1': 'function_call', '3': 3, '4': 1, '5': 11, '6': '.TW.NEAR.Proto.FunctionCall', '9': 0, '10': 'functionCall'},
    {'1': 'transfer', '3': 4, '4': 1, '5': 11, '6': '.TW.NEAR.Proto.Transfer', '9': 0, '10': 'transfer'},
    {'1': 'stake', '3': 5, '4': 1, '5': 11, '6': '.TW.NEAR.Proto.Stake', '9': 0, '10': 'stake'},
    {'1': 'add_key', '3': 6, '4': 1, '5': 11, '6': '.TW.NEAR.Proto.AddKey', '9': 0, '10': 'addKey'},
    {'1': 'delete_key', '3': 7, '4': 1, '5': 11, '6': '.TW.NEAR.Proto.DeleteKey', '9': 0, '10': 'deleteKey'},
    {'1': 'delete_account', '3': 8, '4': 1, '5': 11, '6': '.TW.NEAR.Proto.DeleteAccount', '9': 0, '10': 'deleteAccount'},
    {'1': 'token_transfer', '3': 13, '4': 1, '5': 11, '6': '.TW.NEAR.Proto.TokenTransfer', '9': 0, '10': 'tokenTransfer'},
  ],
  '8': [
    {'1': 'payload'},
  ],
};

/// Descriptor for `Action`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List actionDescriptor = $convert.base64Decode(
    'CgZBY3Rpb24SRQoOY3JlYXRlX2FjY291bnQYASABKAsyHC5UVy5ORUFSLlByb3RvLkNyZWF0ZU'
    'FjY291bnRIAFINY3JlYXRlQWNjb3VudBJICg9kZXBsb3lfY29udHJhY3QYAiABKAsyHS5UVy5O'
    'RUFSLlByb3RvLkRlcGxveUNvbnRyYWN0SABSDmRlcGxveUNvbnRyYWN0EkIKDWZ1bmN0aW9uX2'
    'NhbGwYAyABKAsyGy5UVy5ORUFSLlByb3RvLkZ1bmN0aW9uQ2FsbEgAUgxmdW5jdGlvbkNhbGwS'
    'NQoIdHJhbnNmZXIYBCABKAsyFy5UVy5ORUFSLlByb3RvLlRyYW5zZmVySABSCHRyYW5zZmVyEi'
    'wKBXN0YWtlGAUgASgLMhQuVFcuTkVBUi5Qcm90by5TdGFrZUgAUgVzdGFrZRIwCgdhZGRfa2V5'
    'GAYgASgLMhUuVFcuTkVBUi5Qcm90by5BZGRLZXlIAFIGYWRkS2V5EjkKCmRlbGV0ZV9rZXkYBy'
    'ABKAsyGC5UVy5ORUFSLlByb3RvLkRlbGV0ZUtleUgAUglkZWxldGVLZXkSRQoOZGVsZXRlX2Fj'
    'Y291bnQYCCABKAsyHC5UVy5ORUFSLlByb3RvLkRlbGV0ZUFjY291bnRIAFINZGVsZXRlQWNjb3'
    'VudBJFCg50b2tlbl90cmFuc2ZlchgNIAEoCzIcLlRXLk5FQVIuUHJvdG8uVG9rZW5UcmFuc2Zl'
    'ckgAUg10b2tlblRyYW5zZmVyQgkKB3BheWxvYWQ=');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'signer_id', '3': 1, '4': 1, '5': 9, '10': 'signerId'},
    {'1': 'nonce', '3': 2, '4': 1, '5': 4, '10': 'nonce'},
    {'1': 'receiver_id', '3': 3, '4': 1, '5': 9, '10': 'receiverId'},
    {'1': 'block_hash', '3': 4, '4': 1, '5': 12, '10': 'blockHash'},
    {'1': 'actions', '3': 5, '4': 3, '5': 11, '6': '.TW.NEAR.Proto.Action', '10': 'actions'},
    {'1': 'private_key', '3': 6, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'public_key', '3': 7, '4': 1, '5': 12, '10': 'publicKey'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSGwoJc2lnbmVyX2lkGAEgASgJUghzaWduZXJJZBIUCgVub25jZRgCIA'
    'EoBFIFbm9uY2USHwoLcmVjZWl2ZXJfaWQYAyABKAlSCnJlY2VpdmVySWQSHQoKYmxvY2tfaGFz'
    'aBgEIAEoDFIJYmxvY2tIYXNoEi8KB2FjdGlvbnMYBSADKAsyFS5UVy5ORUFSLlByb3RvLkFjdG'
    'lvblIHYWN0aW9ucxIfCgtwcml2YXRlX2tleRgGIAEoDFIKcHJpdmF0ZUtleRIdCgpwdWJsaWNf'
    'a2V5GAcgASgMUglwdWJsaWNLZXk=');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'signed_transaction', '3': 1, '4': 1, '5': 12, '10': 'signedTransaction'},
    {'1': 'error', '3': 2, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 3, '4': 1, '5': 9, '10': 'errorMessage'},
    {'1': 'hash', '3': 4, '4': 1, '5': 12, '10': 'hash'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0Ei0KEnNpZ25lZF90cmFuc2FjdGlvbhgBIAEoDFIRc2lnbmVkVHJhbn'
    'NhY3Rpb24SMwoFZXJyb3IYAiABKA4yHS5UVy5Db21tb24uUHJvdG8uU2lnbmluZ0Vycm9yUgVl'
    'cnJvchIjCg1lcnJvcl9tZXNzYWdlGAMgASgJUgxlcnJvck1lc3NhZ2USEgoEaGFzaBgEIAEoDF'
    'IEaGFzaA==');

