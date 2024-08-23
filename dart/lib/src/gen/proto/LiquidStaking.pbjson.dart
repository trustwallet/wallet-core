//
//  Generated code. Do not modify.
//  source: LiquidStaking.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use coinDescriptor instead')
const Coin$json = {
  '1': 'Coin',
  '2': [
    {'1': 'MATIC', '2': 0},
    {'1': 'ATOM', '2': 1},
    {'1': 'BNB', '2': 2},
    {'1': 'APT', '2': 3},
    {'1': 'ETH', '2': 4},
  ],
};

/// Descriptor for `Coin`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List coinDescriptor = $convert.base64Decode(
    'CgRDb2luEgkKBU1BVElDEAASCAoEQVRPTRABEgcKA0JOQhACEgcKA0FQVBADEgcKA0VUSBAE');

@$core.Deprecated('Use blockchainDescriptor instead')
const Blockchain$json = {
  '1': 'Blockchain',
  '2': [
    {'1': 'ETHEREUM', '2': 0},
    {'1': 'POLYGON', '2': 1},
    {'1': 'STRIDE', '2': 2},
    {'1': 'BNB_BSC', '2': 3},
    {'1': 'APTOS', '2': 4},
  ],
};

/// Descriptor for `Blockchain`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List blockchainDescriptor = $convert.base64Decode(
    'CgpCbG9ja2NoYWluEgwKCEVUSEVSRVVNEAASCwoHUE9MWUdPThABEgoKBlNUUklERRACEgsKB0'
    'JOQl9CU0MQAxIJCgVBUFRPUxAE');

@$core.Deprecated('Use protocolDescriptor instead')
const Protocol$json = {
  '1': 'Protocol',
  '2': [
    {'1': 'Strader', '2': 0},
    {'1': 'Stride', '2': 1},
    {'1': 'Tortuga', '2': 2},
    {'1': 'Lido', '2': 3},
  ],
};

/// Descriptor for `Protocol`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List protocolDescriptor = $convert.base64Decode(
    'CghQcm90b2NvbBILCgdTdHJhZGVyEAASCgoGU3RyaWRlEAESCwoHVG9ydHVnYRACEggKBExpZG'
    '8QAw==');

@$core.Deprecated('Use statusCodeDescriptor instead')
const StatusCode$json = {
  '1': 'StatusCode',
  '2': [
    {'1': 'OK', '2': 0},
    {'1': 'ERROR_ACTION_NOT_SET', '2': 1},
    {'1': 'ERROR_TARGETED_BLOCKCHAIN_NOT_SUPPORTED_BY_PROTOCOL', '2': 2},
    {'1': 'ERROR_SMART_CONTRACT_ADDRESS_NOT_SET', '2': 3},
    {'1': 'ERROR_INPUT_PROTO_DESERIALIZATION', '2': 4},
    {'1': 'ERROR_OPERATION_NOT_SUPPORTED_BY_PROTOCOL', '2': 5},
  ],
};

/// Descriptor for `StatusCode`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List statusCodeDescriptor = $convert.base64Decode(
    'CgpTdGF0dXNDb2RlEgYKAk9LEAASGAoURVJST1JfQUNUSU9OX05PVF9TRVQQARI3CjNFUlJPUl'
    '9UQVJHRVRFRF9CTE9DS0NIQUlOX05PVF9TVVBQT1JURURfQllfUFJPVE9DT0wQAhIoCiRFUlJP'
    'Ul9TTUFSVF9DT05UUkFDVF9BRERSRVNTX05PVF9TRVQQAxIlCiFFUlJPUl9JTlBVVF9QUk9UT1'
    '9ERVNFUklBTElaQVRJT04QBBItCilFUlJPUl9PUEVSQVRJT05fTk9UX1NVUFBPUlRFRF9CWV9Q'
    'Uk9UT0NPTBAF');

@$core.Deprecated('Use statusDescriptor instead')
const Status$json = {
  '1': 'Status',
  '2': [
    {'1': 'code', '3': 1, '4': 1, '5': 14, '6': '.TW.LiquidStaking.Proto.StatusCode', '10': 'code'},
    {'1': 'message', '3': 2, '4': 1, '5': 9, '10': 'message'},
  ],
};

/// Descriptor for `Status`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List statusDescriptor = $convert.base64Decode(
    'CgZTdGF0dXMSNgoEY29kZRgBIAEoDjIiLlRXLkxpcXVpZFN0YWtpbmcuUHJvdG8uU3RhdHVzQ2'
    '9kZVIEY29kZRIYCgdtZXNzYWdlGAIgASgJUgdtZXNzYWdl');

@$core.Deprecated('Use assetDescriptor instead')
const Asset$json = {
  '1': 'Asset',
  '2': [
    {'1': 'staking_token', '3': 1, '4': 1, '5': 14, '6': '.TW.LiquidStaking.Proto.Coin', '10': 'stakingToken'},
    {'1': 'liquid_token', '3': 2, '4': 1, '5': 9, '10': 'liquidToken'},
    {'1': 'denom', '3': 3, '4': 1, '5': 9, '10': 'denom'},
    {'1': 'from_address', '3': 4, '4': 1, '5': 9, '10': 'fromAddress'},
  ],
};

/// Descriptor for `Asset`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List assetDescriptor = $convert.base64Decode(
    'CgVBc3NldBJBCg1zdGFraW5nX3Rva2VuGAEgASgOMhwuVFcuTGlxdWlkU3Rha2luZy5Qcm90by'
    '5Db2luUgxzdGFraW5nVG9rZW4SIQoMbGlxdWlkX3Rva2VuGAIgASgJUgtsaXF1aWRUb2tlbhIU'
    'CgVkZW5vbRgDIAEoCVIFZGVub20SIQoMZnJvbV9hZGRyZXNzGAQgASgJUgtmcm9tQWRkcmVzcw'
    '==');

@$core.Deprecated('Use stakeDescriptor instead')
const Stake$json = {
  '1': 'Stake',
  '2': [
    {'1': 'asset', '3': 1, '4': 1, '5': 11, '6': '.TW.LiquidStaking.Proto.Asset', '10': 'asset'},
    {'1': 'amount', '3': 2, '4': 1, '5': 9, '10': 'amount'},
  ],
};

/// Descriptor for `Stake`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List stakeDescriptor = $convert.base64Decode(
    'CgVTdGFrZRIzCgVhc3NldBgBIAEoCzIdLlRXLkxpcXVpZFN0YWtpbmcuUHJvdG8uQXNzZXRSBW'
    'Fzc2V0EhYKBmFtb3VudBgCIAEoCVIGYW1vdW50');

@$core.Deprecated('Use unstakeDescriptor instead')
const Unstake$json = {
  '1': 'Unstake',
  '2': [
    {'1': 'asset', '3': 1, '4': 1, '5': 11, '6': '.TW.LiquidStaking.Proto.Asset', '10': 'asset'},
    {'1': 'amount', '3': 2, '4': 1, '5': 9, '10': 'amount'},
    {'1': 'receiver_address', '3': 3, '4': 1, '5': 9, '10': 'receiverAddress'},
    {'1': 'receiver_chain_id', '3': 4, '4': 1, '5': 9, '10': 'receiverChainId'},
  ],
};

/// Descriptor for `Unstake`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List unstakeDescriptor = $convert.base64Decode(
    'CgdVbnN0YWtlEjMKBWFzc2V0GAEgASgLMh0uVFcuTGlxdWlkU3Rha2luZy5Qcm90by5Bc3NldF'
    'IFYXNzZXQSFgoGYW1vdW50GAIgASgJUgZhbW91bnQSKQoQcmVjZWl2ZXJfYWRkcmVzcxgDIAEo'
    'CVIPcmVjZWl2ZXJBZGRyZXNzEioKEXJlY2VpdmVyX2NoYWluX2lkGAQgASgJUg9yZWNlaXZlck'
    'NoYWluSWQ=');

@$core.Deprecated('Use withdrawDescriptor instead')
const Withdraw$json = {
  '1': 'Withdraw',
  '2': [
    {'1': 'asset', '3': 1, '4': 1, '5': 11, '6': '.TW.LiquidStaking.Proto.Asset', '10': 'asset'},
    {'1': 'amount', '3': 2, '4': 1, '5': 9, '10': 'amount'},
    {'1': 'idx', '3': 3, '4': 1, '5': 9, '10': 'idx'},
  ],
};

/// Descriptor for `Withdraw`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List withdrawDescriptor = $convert.base64Decode(
    'CghXaXRoZHJhdxIzCgVhc3NldBgBIAEoCzIdLlRXLkxpcXVpZFN0YWtpbmcuUHJvdG8uQXNzZX'
    'RSBWFzc2V0EhYKBmFtb3VudBgCIAEoCVIGYW1vdW50EhAKA2lkeBgDIAEoCVIDaWR4');

@$core.Deprecated('Use inputDescriptor instead')
const Input$json = {
  '1': 'Input',
  '2': [
    {'1': 'stake', '3': 1, '4': 1, '5': 11, '6': '.TW.LiquidStaking.Proto.Stake', '9': 0, '10': 'stake'},
    {'1': 'unstake', '3': 2, '4': 1, '5': 11, '6': '.TW.LiquidStaking.Proto.Unstake', '9': 0, '10': 'unstake'},
    {'1': 'withdraw', '3': 3, '4': 1, '5': 11, '6': '.TW.LiquidStaking.Proto.Withdraw', '9': 0, '10': 'withdraw'},
    {'1': 'smart_contract_address', '3': 4, '4': 1, '5': 9, '10': 'smartContractAddress'},
    {'1': 'protocol', '3': 5, '4': 1, '5': 14, '6': '.TW.LiquidStaking.Proto.Protocol', '10': 'protocol'},
    {'1': 'blockchain', '3': 6, '4': 1, '5': 14, '6': '.TW.LiquidStaking.Proto.Blockchain', '10': 'blockchain'},
  ],
  '8': [
    {'1': 'action'},
  ],
};

/// Descriptor for `Input`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List inputDescriptor = $convert.base64Decode(
    'CgVJbnB1dBI1CgVzdGFrZRgBIAEoCzIdLlRXLkxpcXVpZFN0YWtpbmcuUHJvdG8uU3Rha2VIAF'
    'IFc3Rha2USOwoHdW5zdGFrZRgCIAEoCzIfLlRXLkxpcXVpZFN0YWtpbmcuUHJvdG8uVW5zdGFr'
    'ZUgAUgd1bnN0YWtlEj4KCHdpdGhkcmF3GAMgASgLMiAuVFcuTGlxdWlkU3Rha2luZy5Qcm90by'
    '5XaXRoZHJhd0gAUgh3aXRoZHJhdxI0ChZzbWFydF9jb250cmFjdF9hZGRyZXNzGAQgASgJUhRz'
    'bWFydENvbnRyYWN0QWRkcmVzcxI8Cghwcm90b2NvbBgFIAEoDjIgLlRXLkxpcXVpZFN0YWtpbm'
    'cuUHJvdG8uUHJvdG9jb2xSCHByb3RvY29sEkIKCmJsb2NrY2hhaW4YBiABKA4yIi5UVy5MaXF1'
    'aWRTdGFraW5nLlByb3RvLkJsb2NrY2hhaW5SCmJsb2NrY2hhaW5CCAoGYWN0aW9u');

@$core.Deprecated('Use outputDescriptor instead')
const Output$json = {
  '1': 'Output',
  '2': [
    {'1': 'status', '3': 1, '4': 1, '5': 11, '6': '.TW.LiquidStaking.Proto.Status', '10': 'status'},
    {'1': 'ethereum', '3': 2, '4': 1, '5': 11, '6': '.TW.Ethereum.Proto.SigningInput', '9': 0, '10': 'ethereum'},
    {'1': 'cosmos', '3': 3, '4': 1, '5': 11, '6': '.TW.Cosmos.Proto.SigningInput', '9': 0, '10': 'cosmos'},
    {'1': 'aptos', '3': 4, '4': 1, '5': 11, '6': '.TW.Aptos.Proto.SigningInput', '9': 0, '10': 'aptos'},
  ],
  '8': [
    {'1': 'signing_input_oneof'},
  ],
};

/// Descriptor for `Output`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List outputDescriptor = $convert.base64Decode(
    'CgZPdXRwdXQSNgoGc3RhdHVzGAEgASgLMh4uVFcuTGlxdWlkU3Rha2luZy5Qcm90by5TdGF0dX'
    'NSBnN0YXR1cxI9CghldGhlcmV1bRgCIAEoCzIfLlRXLkV0aGVyZXVtLlByb3RvLlNpZ25pbmdJ'
    'bnB1dEgAUghldGhlcmV1bRI3CgZjb3Ntb3MYAyABKAsyHS5UVy5Db3Ntb3MuUHJvdG8uU2lnbm'
    'luZ0lucHV0SABSBmNvc21vcxI0CgVhcHRvcxgEIAEoCzIcLlRXLkFwdG9zLlByb3RvLlNpZ25p'
    'bmdJbnB1dEgAUgVhcHRvc0IVChNzaWduaW5nX2lucHV0X29uZW9m');

