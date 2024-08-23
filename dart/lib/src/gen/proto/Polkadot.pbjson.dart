//
//  Generated code. Do not modify.
//  source: Polkadot.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use rewardDestinationDescriptor instead')
const RewardDestination$json = {
  '1': 'RewardDestination',
  '2': [
    {'1': 'STAKED', '2': 0},
    {'1': 'STASH', '2': 1},
    {'1': 'CONTROLLER', '2': 2},
  ],
};

/// Descriptor for `RewardDestination`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List rewardDestinationDescriptor = $convert.base64Decode(
    'ChFSZXdhcmREZXN0aW5hdGlvbhIKCgZTVEFLRUQQABIJCgVTVEFTSBABEg4KCkNPTlRST0xMRV'
    'IQAg==');

@$core.Deprecated('Use eraDescriptor instead')
const Era$json = {
  '1': 'Era',
  '2': [
    {'1': 'block_number', '3': 1, '4': 1, '5': 4, '10': 'blockNumber'},
    {'1': 'period', '3': 2, '4': 1, '5': 4, '10': 'period'},
  ],
};

/// Descriptor for `Era`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List eraDescriptor = $convert.base64Decode(
    'CgNFcmESIQoMYmxvY2tfbnVtYmVyGAEgASgEUgtibG9ja051bWJlchIWCgZwZXJpb2QYAiABKA'
    'RSBnBlcmlvZA==');

@$core.Deprecated('Use customCallIndicesDescriptor instead')
const CustomCallIndices$json = {
  '1': 'CustomCallIndices',
  '2': [
    {'1': 'module_index', '3': 4, '4': 1, '5': 5, '10': 'moduleIndex'},
    {'1': 'method_index', '3': 5, '4': 1, '5': 5, '10': 'methodIndex'},
  ],
};

/// Descriptor for `CustomCallIndices`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List customCallIndicesDescriptor = $convert.base64Decode(
    'ChFDdXN0b21DYWxsSW5kaWNlcxIhCgxtb2R1bGVfaW5kZXgYBCABKAVSC21vZHVsZUluZGV4Ei'
    'EKDG1ldGhvZF9pbmRleBgFIAEoBVILbWV0aG9kSW5kZXg=');

@$core.Deprecated('Use callIndicesDescriptor instead')
const CallIndices$json = {
  '1': 'CallIndices',
  '2': [
    {'1': 'custom', '3': 1, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CustomCallIndices', '9': 0, '10': 'custom'},
  ],
  '8': [
    {'1': 'variant'},
  ],
};

/// Descriptor for `CallIndices`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List callIndicesDescriptor = $convert.base64Decode(
    'CgtDYWxsSW5kaWNlcxI+CgZjdXN0b20YASABKAsyJC5UVy5Qb2xrYWRvdC5Qcm90by5DdXN0b2'
    '1DYWxsSW5kaWNlc0gAUgZjdXN0b21CCQoHdmFyaWFudA==');

@$core.Deprecated('Use balanceDescriptor instead')
const Balance$json = {
  '1': 'Balance',
  '2': [
    {'1': 'transfer', '3': 1, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Balance.Transfer', '9': 0, '10': 'transfer'},
    {'1': 'batchTransfer', '3': 2, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Balance.BatchTransfer', '9': 0, '10': 'batchTransfer'},
    {'1': 'asset_transfer', '3': 3, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Balance.AssetTransfer', '9': 0, '10': 'assetTransfer'},
    {'1': 'batch_asset_transfer', '3': 4, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Balance.BatchAssetTransfer', '9': 0, '10': 'batchAssetTransfer'},
  ],
  '3': [Balance_Transfer$json, Balance_BatchTransfer$json, Balance_AssetTransfer$json, Balance_BatchAssetTransfer$json],
  '8': [
    {'1': 'message_oneof'},
  ],
};

@$core.Deprecated('Use balanceDescriptor instead')
const Balance_Transfer$json = {
  '1': 'Transfer',
  '2': [
    {'1': 'to_address', '3': 1, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'value', '3': 2, '4': 1, '5': 12, '10': 'value'},
    {'1': 'memo', '3': 3, '4': 1, '5': 9, '10': 'memo'},
    {'1': 'call_indices', '3': 4, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CallIndices', '10': 'callIndices'},
  ],
};

@$core.Deprecated('Use balanceDescriptor instead')
const Balance_BatchTransfer$json = {
  '1': 'BatchTransfer',
  '2': [
    {'1': 'call_indices', '3': 1, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CallIndices', '10': 'callIndices'},
    {'1': 'transfers', '3': 2, '4': 3, '5': 11, '6': '.TW.Polkadot.Proto.Balance.Transfer', '10': 'transfers'},
  ],
};

@$core.Deprecated('Use balanceDescriptor instead')
const Balance_AssetTransfer$json = {
  '1': 'AssetTransfer',
  '2': [
    {'1': 'call_indices', '3': 1, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CallIndices', '10': 'callIndices'},
    {'1': 'to_address', '3': 2, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'value', '3': 3, '4': 1, '5': 12, '10': 'value'},
    {'1': 'asset_id', '3': 4, '4': 1, '5': 13, '10': 'assetId'},
    {'1': 'fee_asset_id', '3': 5, '4': 1, '5': 13, '10': 'feeAssetId'},
  ],
};

@$core.Deprecated('Use balanceDescriptor instead')
const Balance_BatchAssetTransfer$json = {
  '1': 'BatchAssetTransfer',
  '2': [
    {'1': 'call_indices', '3': 1, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CallIndices', '10': 'callIndices'},
    {'1': 'fee_asset_id', '3': 2, '4': 1, '5': 13, '10': 'feeAssetId'},
    {'1': 'transfers', '3': 3, '4': 3, '5': 11, '6': '.TW.Polkadot.Proto.Balance.AssetTransfer', '10': 'transfers'},
  ],
};

/// Descriptor for `Balance`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List balanceDescriptor = $convert.base64Decode(
    'CgdCYWxhbmNlEkEKCHRyYW5zZmVyGAEgASgLMiMuVFcuUG9sa2Fkb3QuUHJvdG8uQmFsYW5jZS'
    '5UcmFuc2ZlckgAUgh0cmFuc2ZlchJQCg1iYXRjaFRyYW5zZmVyGAIgASgLMiguVFcuUG9sa2Fk'
    'b3QuUHJvdG8uQmFsYW5jZS5CYXRjaFRyYW5zZmVySABSDWJhdGNoVHJhbnNmZXISUQoOYXNzZX'
    'RfdHJhbnNmZXIYAyABKAsyKC5UVy5Qb2xrYWRvdC5Qcm90by5CYWxhbmNlLkFzc2V0VHJhbnNm'
    'ZXJIAFINYXNzZXRUcmFuc2ZlchJhChRiYXRjaF9hc3NldF90cmFuc2ZlchgEIAEoCzItLlRXLl'
    'BvbGthZG90LlByb3RvLkJhbGFuY2UuQmF0Y2hBc3NldFRyYW5zZmVySABSEmJhdGNoQXNzZXRU'
    'cmFuc2ZlchqWAQoIVHJhbnNmZXISHQoKdG9fYWRkcmVzcxgBIAEoCVIJdG9BZGRyZXNzEhQKBX'
    'ZhbHVlGAIgASgMUgV2YWx1ZRISCgRtZW1vGAMgASgJUgRtZW1vEkEKDGNhbGxfaW5kaWNlcxgE'
    'IAEoCzIeLlRXLlBvbGthZG90LlByb3RvLkNhbGxJbmRpY2VzUgtjYWxsSW5kaWNlcxqVAQoNQm'
    'F0Y2hUcmFuc2ZlchJBCgxjYWxsX2luZGljZXMYASABKAsyHi5UVy5Qb2xrYWRvdC5Qcm90by5D'
    'YWxsSW5kaWNlc1ILY2FsbEluZGljZXMSQQoJdHJhbnNmZXJzGAIgAygLMiMuVFcuUG9sa2Fkb3'
    'QuUHJvdG8uQmFsYW5jZS5UcmFuc2ZlclIJdHJhbnNmZXJzGsQBCg1Bc3NldFRyYW5zZmVyEkEK'
    'DGNhbGxfaW5kaWNlcxgBIAEoCzIeLlRXLlBvbGthZG90LlByb3RvLkNhbGxJbmRpY2VzUgtjYW'
    'xsSW5kaWNlcxIdCgp0b19hZGRyZXNzGAIgASgJUgl0b0FkZHJlc3MSFAoFdmFsdWUYAyABKAxS'
    'BXZhbHVlEhkKCGFzc2V0X2lkGAQgASgNUgdhc3NldElkEiAKDGZlZV9hc3NldF9pZBgFIAEoDV'
    'IKZmVlQXNzZXRJZBrBAQoSQmF0Y2hBc3NldFRyYW5zZmVyEkEKDGNhbGxfaW5kaWNlcxgBIAEo'
    'CzIeLlRXLlBvbGthZG90LlByb3RvLkNhbGxJbmRpY2VzUgtjYWxsSW5kaWNlcxIgCgxmZWVfYX'
    'NzZXRfaWQYAiABKA1SCmZlZUFzc2V0SWQSRgoJdHJhbnNmZXJzGAMgAygLMiguVFcuUG9sa2Fk'
    'b3QuUHJvdG8uQmFsYW5jZS5Bc3NldFRyYW5zZmVyUgl0cmFuc2ZlcnNCDwoNbWVzc2FnZV9vbm'
    'VvZg==');

@$core.Deprecated('Use stakingDescriptor instead')
const Staking$json = {
  '1': 'Staking',
  '2': [
    {'1': 'bond', '3': 1, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Staking.Bond', '9': 0, '10': 'bond'},
    {'1': 'bond_and_nominate', '3': 2, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Staking.BondAndNominate', '9': 0, '10': 'bondAndNominate'},
    {'1': 'bond_extra', '3': 3, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Staking.BondExtra', '9': 0, '10': 'bondExtra'},
    {'1': 'unbond', '3': 4, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Staking.Unbond', '9': 0, '10': 'unbond'},
    {'1': 'withdraw_unbonded', '3': 5, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Staking.WithdrawUnbonded', '9': 0, '10': 'withdrawUnbonded'},
    {'1': 'nominate', '3': 6, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Staking.Nominate', '9': 0, '10': 'nominate'},
    {'1': 'chill', '3': 7, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Staking.Chill', '9': 0, '10': 'chill'},
    {'1': 'chill_and_unbond', '3': 8, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Staking.ChillAndUnbond', '9': 0, '10': 'chillAndUnbond'},
    {'1': 'rebond', '3': 9, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Staking.Rebond', '9': 0, '10': 'rebond'},
  ],
  '3': [Staking_Bond$json, Staking_BondAndNominate$json, Staking_BondExtra$json, Staking_Unbond$json, Staking_Rebond$json, Staking_WithdrawUnbonded$json, Staking_Nominate$json, Staking_ChillAndUnbond$json, Staking_Chill$json],
  '8': [
    {'1': 'message_oneof'},
  ],
};

@$core.Deprecated('Use stakingDescriptor instead')
const Staking_Bond$json = {
  '1': 'Bond',
  '2': [
    {'1': 'controller', '3': 1, '4': 1, '5': 9, '10': 'controller'},
    {'1': 'value', '3': 2, '4': 1, '5': 12, '10': 'value'},
    {'1': 'reward_destination', '3': 3, '4': 1, '5': 14, '6': '.TW.Polkadot.Proto.RewardDestination', '10': 'rewardDestination'},
    {'1': 'call_indices', '3': 4, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CallIndices', '10': 'callIndices'},
  ],
};

@$core.Deprecated('Use stakingDescriptor instead')
const Staking_BondAndNominate$json = {
  '1': 'BondAndNominate',
  '2': [
    {'1': 'controller', '3': 1, '4': 1, '5': 9, '10': 'controller'},
    {'1': 'value', '3': 2, '4': 1, '5': 12, '10': 'value'},
    {'1': 'reward_destination', '3': 3, '4': 1, '5': 14, '6': '.TW.Polkadot.Proto.RewardDestination', '10': 'rewardDestination'},
    {'1': 'nominators', '3': 4, '4': 3, '5': 9, '10': 'nominators'},
    {'1': 'call_indices', '3': 5, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CallIndices', '10': 'callIndices'},
  ],
};

@$core.Deprecated('Use stakingDescriptor instead')
const Staking_BondExtra$json = {
  '1': 'BondExtra',
  '2': [
    {'1': 'value', '3': 1, '4': 1, '5': 12, '10': 'value'},
    {'1': 'call_indices', '3': 2, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CallIndices', '10': 'callIndices'},
  ],
};

@$core.Deprecated('Use stakingDescriptor instead')
const Staking_Unbond$json = {
  '1': 'Unbond',
  '2': [
    {'1': 'value', '3': 1, '4': 1, '5': 12, '10': 'value'},
    {'1': 'call_indices', '3': 2, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CallIndices', '10': 'callIndices'},
  ],
};

@$core.Deprecated('Use stakingDescriptor instead')
const Staking_Rebond$json = {
  '1': 'Rebond',
  '2': [
    {'1': 'value', '3': 1, '4': 1, '5': 12, '10': 'value'},
    {'1': 'call_indices', '3': 2, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CallIndices', '10': 'callIndices'},
  ],
};

@$core.Deprecated('Use stakingDescriptor instead')
const Staking_WithdrawUnbonded$json = {
  '1': 'WithdrawUnbonded',
  '2': [
    {'1': 'slashing_spans', '3': 1, '4': 1, '5': 5, '10': 'slashingSpans'},
    {'1': 'call_indices', '3': 2, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CallIndices', '10': 'callIndices'},
  ],
};

@$core.Deprecated('Use stakingDescriptor instead')
const Staking_Nominate$json = {
  '1': 'Nominate',
  '2': [
    {'1': 'nominators', '3': 1, '4': 3, '5': 9, '10': 'nominators'},
    {'1': 'call_indices', '3': 2, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CallIndices', '10': 'callIndices'},
  ],
};

@$core.Deprecated('Use stakingDescriptor instead')
const Staking_ChillAndUnbond$json = {
  '1': 'ChillAndUnbond',
  '2': [
    {'1': 'value', '3': 1, '4': 1, '5': 12, '10': 'value'},
    {'1': 'call_indices', '3': 2, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CallIndices', '10': 'callIndices'},
  ],
};

@$core.Deprecated('Use stakingDescriptor instead')
const Staking_Chill$json = {
  '1': 'Chill',
  '2': [
    {'1': 'call_indices', '3': 1, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CallIndices', '10': 'callIndices'},
  ],
};

/// Descriptor for `Staking`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List stakingDescriptor = $convert.base64Decode(
    'CgdTdGFraW5nEjUKBGJvbmQYASABKAsyHy5UVy5Qb2xrYWRvdC5Qcm90by5TdGFraW5nLkJvbm'
    'RIAFIEYm9uZBJYChFib25kX2FuZF9ub21pbmF0ZRgCIAEoCzIqLlRXLlBvbGthZG90LlByb3Rv'
    'LlN0YWtpbmcuQm9uZEFuZE5vbWluYXRlSABSD2JvbmRBbmROb21pbmF0ZRJFCgpib25kX2V4dH'
    'JhGAMgASgLMiQuVFcuUG9sa2Fkb3QuUHJvdG8uU3Rha2luZy5Cb25kRXh0cmFIAFIJYm9uZEV4'
    'dHJhEjsKBnVuYm9uZBgEIAEoCzIhLlRXLlBvbGthZG90LlByb3RvLlN0YWtpbmcuVW5ib25kSA'
    'BSBnVuYm9uZBJaChF3aXRoZHJhd191bmJvbmRlZBgFIAEoCzIrLlRXLlBvbGthZG90LlByb3Rv'
    'LlN0YWtpbmcuV2l0aGRyYXdVbmJvbmRlZEgAUhB3aXRoZHJhd1VuYm9uZGVkEkEKCG5vbWluYX'
    'RlGAYgASgLMiMuVFcuUG9sa2Fkb3QuUHJvdG8uU3Rha2luZy5Ob21pbmF0ZUgAUghub21pbmF0'
    'ZRI4CgVjaGlsbBgHIAEoCzIgLlRXLlBvbGthZG90LlByb3RvLlN0YWtpbmcuQ2hpbGxIAFIFY2'
    'hpbGwSVQoQY2hpbGxfYW5kX3VuYm9uZBgIIAEoCzIpLlRXLlBvbGthZG90LlByb3RvLlN0YWtp'
    'bmcuQ2hpbGxBbmRVbmJvbmRIAFIOY2hpbGxBbmRVbmJvbmQSOwoGcmVib25kGAkgASgLMiEuVF'
    'cuUG9sa2Fkb3QuUHJvdG8uU3Rha2luZy5SZWJvbmRIAFIGcmVib25kGtQBCgRCb25kEh4KCmNv'
    'bnRyb2xsZXIYASABKAlSCmNvbnRyb2xsZXISFAoFdmFsdWUYAiABKAxSBXZhbHVlElMKEnJld2'
    'FyZF9kZXN0aW5hdGlvbhgDIAEoDjIkLlRXLlBvbGthZG90LlByb3RvLlJld2FyZERlc3RpbmF0'
    'aW9uUhFyZXdhcmREZXN0aW5hdGlvbhJBCgxjYWxsX2luZGljZXMYBCABKAsyHi5UVy5Qb2xrYW'
    'RvdC5Qcm90by5DYWxsSW5kaWNlc1ILY2FsbEluZGljZXMa/wEKD0JvbmRBbmROb21pbmF0ZRIe'
    'Cgpjb250cm9sbGVyGAEgASgJUgpjb250cm9sbGVyEhQKBXZhbHVlGAIgASgMUgV2YWx1ZRJTCh'
    'JyZXdhcmRfZGVzdGluYXRpb24YAyABKA4yJC5UVy5Qb2xrYWRvdC5Qcm90by5SZXdhcmREZXN0'
    'aW5hdGlvblIRcmV3YXJkRGVzdGluYXRpb24SHgoKbm9taW5hdG9ycxgEIAMoCVIKbm9taW5hdG'
    '9ycxJBCgxjYWxsX2luZGljZXMYBSABKAsyHi5UVy5Qb2xrYWRvdC5Qcm90by5DYWxsSW5kaWNl'
    'c1ILY2FsbEluZGljZXMaZAoJQm9uZEV4dHJhEhQKBXZhbHVlGAEgASgMUgV2YWx1ZRJBCgxjYW'
    'xsX2luZGljZXMYAiABKAsyHi5UVy5Qb2xrYWRvdC5Qcm90by5DYWxsSW5kaWNlc1ILY2FsbElu'
    'ZGljZXMaYQoGVW5ib25kEhQKBXZhbHVlGAEgASgMUgV2YWx1ZRJBCgxjYWxsX2luZGljZXMYAi'
    'ABKAsyHi5UVy5Qb2xrYWRvdC5Qcm90by5DYWxsSW5kaWNlc1ILY2FsbEluZGljZXMaYQoGUmVi'
    'b25kEhQKBXZhbHVlGAEgASgMUgV2YWx1ZRJBCgxjYWxsX2luZGljZXMYAiABKAsyHi5UVy5Qb2'
    'xrYWRvdC5Qcm90by5DYWxsSW5kaWNlc1ILY2FsbEluZGljZXMafAoQV2l0aGRyYXdVbmJvbmRl'
    'ZBIlCg5zbGFzaGluZ19zcGFucxgBIAEoBVINc2xhc2hpbmdTcGFucxJBCgxjYWxsX2luZGljZX'
    'MYAiABKAsyHi5UVy5Qb2xrYWRvdC5Qcm90by5DYWxsSW5kaWNlc1ILY2FsbEluZGljZXMabQoI'
    'Tm9taW5hdGUSHgoKbm9taW5hdG9ycxgBIAMoCVIKbm9taW5hdG9ycxJBCgxjYWxsX2luZGljZX'
    'MYAiABKAsyHi5UVy5Qb2xrYWRvdC5Qcm90by5DYWxsSW5kaWNlc1ILY2FsbEluZGljZXMaaQoO'
    'Q2hpbGxBbmRVbmJvbmQSFAoFdmFsdWUYASABKAxSBXZhbHVlEkEKDGNhbGxfaW5kaWNlcxgCIA'
    'EoCzIeLlRXLlBvbGthZG90LlByb3RvLkNhbGxJbmRpY2VzUgtjYWxsSW5kaWNlcxpKCgVDaGls'
    'bBJBCgxjYWxsX2luZGljZXMYASABKAsyHi5UVy5Qb2xrYWRvdC5Qcm90by5DYWxsSW5kaWNlc1'
    'ILY2FsbEluZGljZXNCDwoNbWVzc2FnZV9vbmVvZg==');

@$core.Deprecated('Use identityDescriptor instead')
const Identity$json = {
  '1': 'Identity',
  '2': [
    {'1': 'join_identity_as_key', '3': 1, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Identity.JoinIdentityAsKey', '9': 0, '10': 'joinIdentityAsKey'},
    {'1': 'add_authorization', '3': 2, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Identity.AddAuthorization', '9': 0, '10': 'addAuthorization'},
  ],
  '3': [Identity_JoinIdentityAsKey$json, Identity_AddAuthorization$json],
  '8': [
    {'1': 'message_oneof'},
  ],
};

@$core.Deprecated('Use identityDescriptor instead')
const Identity_JoinIdentityAsKey$json = {
  '1': 'JoinIdentityAsKey',
  '2': [
    {'1': 'call_indices', '3': 1, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CallIndices', '10': 'callIndices'},
    {'1': 'auth_id', '3': 2, '4': 1, '5': 4, '10': 'authId'},
  ],
};

@$core.Deprecated('Use identityDescriptor instead')
const Identity_AddAuthorization$json = {
  '1': 'AddAuthorization',
  '2': [
    {'1': 'call_indices', '3': 1, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.CallIndices', '10': 'callIndices'},
    {'1': 'target', '3': 2, '4': 1, '5': 9, '10': 'target'},
    {'1': 'data', '3': 3, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Identity.AddAuthorization.AuthData', '10': 'data'},
    {'1': 'expiry', '3': 4, '4': 1, '5': 4, '10': 'expiry'},
  ],
  '3': [Identity_AddAuthorization_Data$json, Identity_AddAuthorization_AuthData$json],
};

@$core.Deprecated('Use identityDescriptor instead')
const Identity_AddAuthorization_Data$json = {
  '1': 'Data',
  '2': [
    {'1': 'data', '3': 1, '4': 1, '5': 12, '10': 'data'},
  ],
};

@$core.Deprecated('Use identityDescriptor instead')
const Identity_AddAuthorization_AuthData$json = {
  '1': 'AuthData',
  '2': [
    {'1': 'asset', '3': 1, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Identity.AddAuthorization.Data', '10': 'asset'},
    {'1': 'extrinsic', '3': 2, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Identity.AddAuthorization.Data', '10': 'extrinsic'},
    {'1': 'portfolio', '3': 3, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Identity.AddAuthorization.Data', '10': 'portfolio'},
  ],
};

/// Descriptor for `Identity`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List identityDescriptor = $convert.base64Decode(
    'CghJZGVudGl0eRJgChRqb2luX2lkZW50aXR5X2FzX2tleRgBIAEoCzItLlRXLlBvbGthZG90Ll'
    'Byb3RvLklkZW50aXR5LkpvaW5JZGVudGl0eUFzS2V5SABSEWpvaW5JZGVudGl0eUFzS2V5ElsK'
    'EWFkZF9hdXRob3JpemF0aW9uGAIgASgLMiwuVFcuUG9sa2Fkb3QuUHJvdG8uSWRlbnRpdHkuQW'
    'RkQXV0aG9yaXphdGlvbkgAUhBhZGRBdXRob3JpemF0aW9uGm8KEUpvaW5JZGVudGl0eUFzS2V5'
    'EkEKDGNhbGxfaW5kaWNlcxgBIAEoCzIeLlRXLlBvbGthZG90LlByb3RvLkNhbGxJbmRpY2VzUg'
    'tjYWxsSW5kaWNlcxIXCgdhdXRoX2lkGAIgASgEUgZhdXRoSWQa5AMKEEFkZEF1dGhvcml6YXRp'
    'b24SQQoMY2FsbF9pbmRpY2VzGAEgASgLMh4uVFcuUG9sa2Fkb3QuUHJvdG8uQ2FsbEluZGljZX'
    'NSC2NhbGxJbmRpY2VzEhYKBnRhcmdldBgCIAEoCVIGdGFyZ2V0EkkKBGRhdGEYAyABKAsyNS5U'
    'Vy5Qb2xrYWRvdC5Qcm90by5JZGVudGl0eS5BZGRBdXRob3JpemF0aW9uLkF1dGhEYXRhUgRkYX'
    'RhEhYKBmV4cGlyeRgEIAEoBFIGZXhwaXJ5GhoKBERhdGESEgoEZGF0YRgBIAEoDFIEZGF0YRr1'
    'AQoIQXV0aERhdGESRwoFYXNzZXQYASABKAsyMS5UVy5Qb2xrYWRvdC5Qcm90by5JZGVudGl0eS'
    '5BZGRBdXRob3JpemF0aW9uLkRhdGFSBWFzc2V0Ek8KCWV4dHJpbnNpYxgCIAEoCzIxLlRXLlBv'
    'bGthZG90LlByb3RvLklkZW50aXR5LkFkZEF1dGhvcml6YXRpb24uRGF0YVIJZXh0cmluc2ljEk'
    '8KCXBvcnRmb2xpbxgDIAEoCzIxLlRXLlBvbGthZG90LlByb3RvLklkZW50aXR5LkFkZEF1dGhv'
    'cml6YXRpb24uRGF0YVIJcG9ydGZvbGlvQg8KDW1lc3NhZ2Vfb25lb2Y=');

@$core.Deprecated('Use polymeshCallDescriptor instead')
const PolymeshCall$json = {
  '1': 'PolymeshCall',
  '2': [
    {'1': 'identity_call', '3': 2, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Identity', '9': 0, '10': 'identityCall'},
  ],
  '8': [
    {'1': 'message_oneof'},
  ],
};

/// Descriptor for `PolymeshCall`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List polymeshCallDescriptor = $convert.base64Decode(
    'CgxQb2x5bWVzaENhbGwSQgoNaWRlbnRpdHlfY2FsbBgCIAEoCzIbLlRXLlBvbGthZG90LlByb3'
    'RvLklkZW50aXR5SABSDGlkZW50aXR5Q2FsbEIPCg1tZXNzYWdlX29uZW9m');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'block_hash', '3': 1, '4': 1, '5': 12, '10': 'blockHash'},
    {'1': 'genesis_hash', '3': 2, '4': 1, '5': 12, '10': 'genesisHash'},
    {'1': 'nonce', '3': 3, '4': 1, '5': 4, '10': 'nonce'},
    {'1': 'spec_version', '3': 4, '4': 1, '5': 13, '10': 'specVersion'},
    {'1': 'transaction_version', '3': 5, '4': 1, '5': 13, '10': 'transactionVersion'},
    {'1': 'tip', '3': 6, '4': 1, '5': 12, '10': 'tip'},
    {'1': 'era', '3': 7, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Era', '10': 'era'},
    {'1': 'private_key', '3': 8, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'network', '3': 9, '4': 1, '5': 13, '10': 'network'},
    {'1': 'multi_address', '3': 10, '4': 1, '5': 8, '10': 'multiAddress'},
    {'1': 'balance_call', '3': 11, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Balance', '9': 0, '10': 'balanceCall'},
    {'1': 'staking_call', '3': 12, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.Staking', '9': 0, '10': 'stakingCall'},
    {'1': 'polymesh_call', '3': 13, '4': 1, '5': 11, '6': '.TW.Polkadot.Proto.PolymeshCall', '9': 0, '10': 'polymeshCall'},
  ],
  '8': [
    {'1': 'message_oneof'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSHQoKYmxvY2tfaGFzaBgBIAEoDFIJYmxvY2tIYXNoEiEKDGdlbmVzaX'
    'NfaGFzaBgCIAEoDFILZ2VuZXNpc0hhc2gSFAoFbm9uY2UYAyABKARSBW5vbmNlEiEKDHNwZWNf'
    'dmVyc2lvbhgEIAEoDVILc3BlY1ZlcnNpb24SLwoTdHJhbnNhY3Rpb25fdmVyc2lvbhgFIAEoDV'
    'ISdHJhbnNhY3Rpb25WZXJzaW9uEhAKA3RpcBgGIAEoDFIDdGlwEigKA2VyYRgHIAEoCzIWLlRX'
    'LlBvbGthZG90LlByb3RvLkVyYVIDZXJhEh8KC3ByaXZhdGVfa2V5GAggASgMUgpwcml2YXRlS2'
    'V5EhgKB25ldHdvcmsYCSABKA1SB25ldHdvcmsSIwoNbXVsdGlfYWRkcmVzcxgKIAEoCFIMbXVs'
    'dGlBZGRyZXNzEj8KDGJhbGFuY2VfY2FsbBgLIAEoCzIaLlRXLlBvbGthZG90LlByb3RvLkJhbG'
    'FuY2VIAFILYmFsYW5jZUNhbGwSPwoMc3Rha2luZ19jYWxsGAwgASgLMhouVFcuUG9sa2Fkb3Qu'
    'UHJvdG8uU3Rha2luZ0gAUgtzdGFraW5nQ2FsbBJGCg1wb2x5bWVzaF9jYWxsGA0gASgLMh8uVF'
    'cuUG9sa2Fkb3QuUHJvdG8uUG9seW1lc2hDYWxsSABSDHBvbHltZXNoQ2FsbEIPCg1tZXNzYWdl'
    'X29uZW9m');

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

