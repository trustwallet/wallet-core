//
//  Generated code. Do not modify.
//  source: Tron.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use transferContractDescriptor instead')
const TransferContract$json = {
  '1': 'TransferContract',
  '2': [
    {'1': 'owner_address', '3': 1, '4': 1, '5': 9, '10': 'ownerAddress'},
    {'1': 'to_address', '3': 2, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'amount', '3': 3, '4': 1, '5': 3, '10': 'amount'},
  ],
};

/// Descriptor for `TransferContract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transferContractDescriptor = $convert.base64Decode(
    'ChBUcmFuc2ZlckNvbnRyYWN0EiMKDW93bmVyX2FkZHJlc3MYASABKAlSDG93bmVyQWRkcmVzcx'
    'IdCgp0b19hZGRyZXNzGAIgASgJUgl0b0FkZHJlc3MSFgoGYW1vdW50GAMgASgDUgZhbW91bnQ=');

@$core.Deprecated('Use transferAssetContractDescriptor instead')
const TransferAssetContract$json = {
  '1': 'TransferAssetContract',
  '2': [
    {'1': 'asset_name', '3': 1, '4': 1, '5': 9, '10': 'assetName'},
    {'1': 'owner_address', '3': 2, '4': 1, '5': 9, '10': 'ownerAddress'},
    {'1': 'to_address', '3': 3, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'amount', '3': 4, '4': 1, '5': 3, '10': 'amount'},
  ],
};

/// Descriptor for `TransferAssetContract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transferAssetContractDescriptor = $convert.base64Decode(
    'ChVUcmFuc2ZlckFzc2V0Q29udHJhY3QSHQoKYXNzZXRfbmFtZRgBIAEoCVIJYXNzZXROYW1lEi'
    'MKDW93bmVyX2FkZHJlc3MYAiABKAlSDG93bmVyQWRkcmVzcxIdCgp0b19hZGRyZXNzGAMgASgJ'
    'Ugl0b0FkZHJlc3MSFgoGYW1vdW50GAQgASgDUgZhbW91bnQ=');

@$core.Deprecated('Use transferTRC20ContractDescriptor instead')
const TransferTRC20Contract$json = {
  '1': 'TransferTRC20Contract',
  '2': [
    {'1': 'contract_address', '3': 1, '4': 1, '5': 9, '10': 'contractAddress'},
    {'1': 'owner_address', '3': 2, '4': 1, '5': 9, '10': 'ownerAddress'},
    {'1': 'to_address', '3': 3, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'amount', '3': 4, '4': 1, '5': 12, '10': 'amount'},
  ],
};

/// Descriptor for `TransferTRC20Contract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transferTRC20ContractDescriptor = $convert.base64Decode(
    'ChVUcmFuc2ZlclRSQzIwQ29udHJhY3QSKQoQY29udHJhY3RfYWRkcmVzcxgBIAEoCVIPY29udH'
    'JhY3RBZGRyZXNzEiMKDW93bmVyX2FkZHJlc3MYAiABKAlSDG93bmVyQWRkcmVzcxIdCgp0b19h'
    'ZGRyZXNzGAMgASgJUgl0b0FkZHJlc3MSFgoGYW1vdW50GAQgASgMUgZhbW91bnQ=');

@$core.Deprecated('Use freezeBalanceContractDescriptor instead')
const FreezeBalanceContract$json = {
  '1': 'FreezeBalanceContract',
  '2': [
    {'1': 'owner_address', '3': 1, '4': 1, '5': 9, '10': 'ownerAddress'},
    {'1': 'frozen_balance', '3': 2, '4': 1, '5': 3, '10': 'frozenBalance'},
    {'1': 'frozen_duration', '3': 3, '4': 1, '5': 3, '10': 'frozenDuration'},
    {'1': 'resource', '3': 10, '4': 1, '5': 9, '10': 'resource'},
    {'1': 'receiver_address', '3': 15, '4': 1, '5': 9, '10': 'receiverAddress'},
  ],
};

/// Descriptor for `FreezeBalanceContract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List freezeBalanceContractDescriptor = $convert.base64Decode(
    'ChVGcmVlemVCYWxhbmNlQ29udHJhY3QSIwoNb3duZXJfYWRkcmVzcxgBIAEoCVIMb3duZXJBZG'
    'RyZXNzEiUKDmZyb3plbl9iYWxhbmNlGAIgASgDUg1mcm96ZW5CYWxhbmNlEicKD2Zyb3plbl9k'
    'dXJhdGlvbhgDIAEoA1IOZnJvemVuRHVyYXRpb24SGgoIcmVzb3VyY2UYCiABKAlSCHJlc291cm'
    'NlEikKEHJlY2VpdmVyX2FkZHJlc3MYDyABKAlSD3JlY2VpdmVyQWRkcmVzcw==');

@$core.Deprecated('Use freezeBalanceV2ContractDescriptor instead')
const FreezeBalanceV2Contract$json = {
  '1': 'FreezeBalanceV2Contract',
  '2': [
    {'1': 'owner_address', '3': 1, '4': 1, '5': 9, '10': 'ownerAddress'},
    {'1': 'frozen_balance', '3': 2, '4': 1, '5': 3, '10': 'frozenBalance'},
    {'1': 'resource', '3': 3, '4': 1, '5': 9, '10': 'resource'},
  ],
};

/// Descriptor for `FreezeBalanceV2Contract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List freezeBalanceV2ContractDescriptor = $convert.base64Decode(
    'ChdGcmVlemVCYWxhbmNlVjJDb250cmFjdBIjCg1vd25lcl9hZGRyZXNzGAEgASgJUgxvd25lck'
    'FkZHJlc3MSJQoOZnJvemVuX2JhbGFuY2UYAiABKANSDWZyb3plbkJhbGFuY2USGgoIcmVzb3Vy'
    'Y2UYAyABKAlSCHJlc291cmNl');

@$core.Deprecated('Use unfreezeBalanceV2ContractDescriptor instead')
const UnfreezeBalanceV2Contract$json = {
  '1': 'UnfreezeBalanceV2Contract',
  '2': [
    {'1': 'owner_address', '3': 1, '4': 1, '5': 9, '10': 'ownerAddress'},
    {'1': 'unfreeze_balance', '3': 2, '4': 1, '5': 3, '10': 'unfreezeBalance'},
    {'1': 'resource', '3': 3, '4': 1, '5': 9, '10': 'resource'},
  ],
};

/// Descriptor for `UnfreezeBalanceV2Contract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List unfreezeBalanceV2ContractDescriptor = $convert.base64Decode(
    'ChlVbmZyZWV6ZUJhbGFuY2VWMkNvbnRyYWN0EiMKDW93bmVyX2FkZHJlc3MYASABKAlSDG93bm'
    'VyQWRkcmVzcxIpChB1bmZyZWV6ZV9iYWxhbmNlGAIgASgDUg91bmZyZWV6ZUJhbGFuY2USGgoI'
    'cmVzb3VyY2UYAyABKAlSCHJlc291cmNl');

@$core.Deprecated('Use withdrawExpireUnfreezeContractDescriptor instead')
const WithdrawExpireUnfreezeContract$json = {
  '1': 'WithdrawExpireUnfreezeContract',
  '2': [
    {'1': 'owner_address', '3': 1, '4': 1, '5': 9, '10': 'ownerAddress'},
  ],
};

/// Descriptor for `WithdrawExpireUnfreezeContract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List withdrawExpireUnfreezeContractDescriptor = $convert.base64Decode(
    'Ch5XaXRoZHJhd0V4cGlyZVVuZnJlZXplQ29udHJhY3QSIwoNb3duZXJfYWRkcmVzcxgBIAEoCV'
    'IMb3duZXJBZGRyZXNz');

@$core.Deprecated('Use delegateResourceContractDescriptor instead')
const DelegateResourceContract$json = {
  '1': 'DelegateResourceContract',
  '2': [
    {'1': 'owner_address', '3': 1, '4': 1, '5': 9, '10': 'ownerAddress'},
    {'1': 'resource', '3': 2, '4': 1, '5': 9, '10': 'resource'},
    {'1': 'balance', '3': 3, '4': 1, '5': 3, '10': 'balance'},
    {'1': 'receiver_address', '3': 4, '4': 1, '5': 9, '10': 'receiverAddress'},
    {'1': 'lock', '3': 5, '4': 1, '5': 8, '10': 'lock'},
  ],
};

/// Descriptor for `DelegateResourceContract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List delegateResourceContractDescriptor = $convert.base64Decode(
    'ChhEZWxlZ2F0ZVJlc291cmNlQ29udHJhY3QSIwoNb3duZXJfYWRkcmVzcxgBIAEoCVIMb3duZX'
    'JBZGRyZXNzEhoKCHJlc291cmNlGAIgASgJUghyZXNvdXJjZRIYCgdiYWxhbmNlGAMgASgDUgdi'
    'YWxhbmNlEikKEHJlY2VpdmVyX2FkZHJlc3MYBCABKAlSD3JlY2VpdmVyQWRkcmVzcxISCgRsb2'
    'NrGAUgASgIUgRsb2Nr');

@$core.Deprecated('Use unDelegateResourceContractDescriptor instead')
const UnDelegateResourceContract$json = {
  '1': 'UnDelegateResourceContract',
  '2': [
    {'1': 'owner_address', '3': 1, '4': 1, '5': 9, '10': 'ownerAddress'},
    {'1': 'resource', '3': 2, '4': 1, '5': 9, '10': 'resource'},
    {'1': 'balance', '3': 3, '4': 1, '5': 3, '10': 'balance'},
    {'1': 'receiver_address', '3': 4, '4': 1, '5': 9, '10': 'receiverAddress'},
  ],
};

/// Descriptor for `UnDelegateResourceContract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List unDelegateResourceContractDescriptor = $convert.base64Decode(
    'ChpVbkRlbGVnYXRlUmVzb3VyY2VDb250cmFjdBIjCg1vd25lcl9hZGRyZXNzGAEgASgJUgxvd2'
    '5lckFkZHJlc3MSGgoIcmVzb3VyY2UYAiABKAlSCHJlc291cmNlEhgKB2JhbGFuY2UYAyABKANS'
    'B2JhbGFuY2USKQoQcmVjZWl2ZXJfYWRkcmVzcxgEIAEoCVIPcmVjZWl2ZXJBZGRyZXNz');

@$core.Deprecated('Use unfreezeBalanceContractDescriptor instead')
const UnfreezeBalanceContract$json = {
  '1': 'UnfreezeBalanceContract',
  '2': [
    {'1': 'owner_address', '3': 1, '4': 1, '5': 9, '10': 'ownerAddress'},
    {'1': 'resource', '3': 10, '4': 1, '5': 9, '10': 'resource'},
    {'1': 'receiver_address', '3': 15, '4': 1, '5': 9, '10': 'receiverAddress'},
  ],
};

/// Descriptor for `UnfreezeBalanceContract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List unfreezeBalanceContractDescriptor = $convert.base64Decode(
    'ChdVbmZyZWV6ZUJhbGFuY2VDb250cmFjdBIjCg1vd25lcl9hZGRyZXNzGAEgASgJUgxvd25lck'
    'FkZHJlc3MSGgoIcmVzb3VyY2UYCiABKAlSCHJlc291cmNlEikKEHJlY2VpdmVyX2FkZHJlc3MY'
    'DyABKAlSD3JlY2VpdmVyQWRkcmVzcw==');

@$core.Deprecated('Use unfreezeAssetContractDescriptor instead')
const UnfreezeAssetContract$json = {
  '1': 'UnfreezeAssetContract',
  '2': [
    {'1': 'owner_address', '3': 1, '4': 1, '5': 9, '10': 'ownerAddress'},
  ],
};

/// Descriptor for `UnfreezeAssetContract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List unfreezeAssetContractDescriptor = $convert.base64Decode(
    'ChVVbmZyZWV6ZUFzc2V0Q29udHJhY3QSIwoNb3duZXJfYWRkcmVzcxgBIAEoCVIMb3duZXJBZG'
    'RyZXNz');

@$core.Deprecated('Use voteAssetContractDescriptor instead')
const VoteAssetContract$json = {
  '1': 'VoteAssetContract',
  '2': [
    {'1': 'owner_address', '3': 1, '4': 1, '5': 9, '10': 'ownerAddress'},
    {'1': 'vote_address', '3': 2, '4': 3, '5': 9, '10': 'voteAddress'},
    {'1': 'support', '3': 3, '4': 1, '5': 8, '10': 'support'},
    {'1': 'count', '3': 5, '4': 1, '5': 5, '10': 'count'},
  ],
};

/// Descriptor for `VoteAssetContract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List voteAssetContractDescriptor = $convert.base64Decode(
    'ChFWb3RlQXNzZXRDb250cmFjdBIjCg1vd25lcl9hZGRyZXNzGAEgASgJUgxvd25lckFkZHJlc3'
    'MSIQoMdm90ZV9hZGRyZXNzGAIgAygJUgt2b3RlQWRkcmVzcxIYCgdzdXBwb3J0GAMgASgIUgdz'
    'dXBwb3J0EhQKBWNvdW50GAUgASgFUgVjb3VudA==');

@$core.Deprecated('Use voteWitnessContractDescriptor instead')
const VoteWitnessContract$json = {
  '1': 'VoteWitnessContract',
  '2': [
    {'1': 'owner_address', '3': 1, '4': 1, '5': 9, '10': 'ownerAddress'},
    {'1': 'votes', '3': 2, '4': 3, '5': 11, '6': '.TW.Tron.Proto.VoteWitnessContract.Vote', '10': 'votes'},
    {'1': 'support', '3': 3, '4': 1, '5': 8, '10': 'support'},
  ],
  '3': [VoteWitnessContract_Vote$json],
};

@$core.Deprecated('Use voteWitnessContractDescriptor instead')
const VoteWitnessContract_Vote$json = {
  '1': 'Vote',
  '2': [
    {'1': 'vote_address', '3': 1, '4': 1, '5': 9, '10': 'voteAddress'},
    {'1': 'vote_count', '3': 2, '4': 1, '5': 3, '10': 'voteCount'},
  ],
};

/// Descriptor for `VoteWitnessContract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List voteWitnessContractDescriptor = $convert.base64Decode(
    'ChNWb3RlV2l0bmVzc0NvbnRyYWN0EiMKDW93bmVyX2FkZHJlc3MYASABKAlSDG93bmVyQWRkcm'
    'VzcxI9CgV2b3RlcxgCIAMoCzInLlRXLlRyb24uUHJvdG8uVm90ZVdpdG5lc3NDb250cmFjdC5W'
    'b3RlUgV2b3RlcxIYCgdzdXBwb3J0GAMgASgIUgdzdXBwb3J0GkgKBFZvdGUSIQoMdm90ZV9hZG'
    'RyZXNzGAEgASgJUgt2b3RlQWRkcmVzcxIdCgp2b3RlX2NvdW50GAIgASgDUgl2b3RlQ291bnQ=');

@$core.Deprecated('Use withdrawBalanceContractDescriptor instead')
const WithdrawBalanceContract$json = {
  '1': 'WithdrawBalanceContract',
  '2': [
    {'1': 'owner_address', '3': 1, '4': 1, '5': 9, '10': 'ownerAddress'},
  ],
};

/// Descriptor for `WithdrawBalanceContract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List withdrawBalanceContractDescriptor = $convert.base64Decode(
    'ChdXaXRoZHJhd0JhbGFuY2VDb250cmFjdBIjCg1vd25lcl9hZGRyZXNzGAEgASgJUgxvd25lck'
    'FkZHJlc3M=');

@$core.Deprecated('Use triggerSmartContractDescriptor instead')
const TriggerSmartContract$json = {
  '1': 'TriggerSmartContract',
  '2': [
    {'1': 'owner_address', '3': 1, '4': 1, '5': 9, '10': 'ownerAddress'},
    {'1': 'contract_address', '3': 2, '4': 1, '5': 9, '10': 'contractAddress'},
    {'1': 'call_value', '3': 3, '4': 1, '5': 3, '10': 'callValue'},
    {'1': 'data', '3': 4, '4': 1, '5': 12, '10': 'data'},
    {'1': 'call_token_value', '3': 5, '4': 1, '5': 3, '10': 'callTokenValue'},
    {'1': 'token_id', '3': 6, '4': 1, '5': 3, '10': 'tokenId'},
  ],
};

/// Descriptor for `TriggerSmartContract`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List triggerSmartContractDescriptor = $convert.base64Decode(
    'ChRUcmlnZ2VyU21hcnRDb250cmFjdBIjCg1vd25lcl9hZGRyZXNzGAEgASgJUgxvd25lckFkZH'
    'Jlc3MSKQoQY29udHJhY3RfYWRkcmVzcxgCIAEoCVIPY29udHJhY3RBZGRyZXNzEh0KCmNhbGxf'
    'dmFsdWUYAyABKANSCWNhbGxWYWx1ZRISCgRkYXRhGAQgASgMUgRkYXRhEigKEGNhbGxfdG9rZW'
    '5fdmFsdWUYBSABKANSDmNhbGxUb2tlblZhbHVlEhkKCHRva2VuX2lkGAYgASgDUgd0b2tlbklk');

@$core.Deprecated('Use blockHeaderDescriptor instead')
const BlockHeader$json = {
  '1': 'BlockHeader',
  '2': [
    {'1': 'timestamp', '3': 1, '4': 1, '5': 3, '10': 'timestamp'},
    {'1': 'tx_trie_root', '3': 2, '4': 1, '5': 12, '10': 'txTrieRoot'},
    {'1': 'parent_hash', '3': 3, '4': 1, '5': 12, '10': 'parentHash'},
    {'1': 'number', '3': 7, '4': 1, '5': 3, '10': 'number'},
    {'1': 'witness_address', '3': 9, '4': 1, '5': 12, '10': 'witnessAddress'},
    {'1': 'version', '3': 10, '4': 1, '5': 5, '10': 'version'},
  ],
};

/// Descriptor for `BlockHeader`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List blockHeaderDescriptor = $convert.base64Decode(
    'CgtCbG9ja0hlYWRlchIcCgl0aW1lc3RhbXAYASABKANSCXRpbWVzdGFtcBIgCgx0eF90cmllX3'
    'Jvb3QYAiABKAxSCnR4VHJpZVJvb3QSHwoLcGFyZW50X2hhc2gYAyABKAxSCnBhcmVudEhhc2gS'
    'FgoGbnVtYmVyGAcgASgDUgZudW1iZXISJwoPd2l0bmVzc19hZGRyZXNzGAkgASgMUg53aXRuZX'
    'NzQWRkcmVzcxIYCgd2ZXJzaW9uGAogASgFUgd2ZXJzaW9u');

@$core.Deprecated('Use transactionDescriptor instead')
const Transaction$json = {
  '1': 'Transaction',
  '2': [
    {'1': 'timestamp', '3': 1, '4': 1, '5': 3, '10': 'timestamp'},
    {'1': 'expiration', '3': 2, '4': 1, '5': 3, '10': 'expiration'},
    {'1': 'block_header', '3': 3, '4': 1, '5': 11, '6': '.TW.Tron.Proto.BlockHeader', '10': 'blockHeader'},
    {'1': 'fee_limit', '3': 4, '4': 1, '5': 3, '10': 'feeLimit'},
    {'1': 'transfer', '3': 10, '4': 1, '5': 11, '6': '.TW.Tron.Proto.TransferContract', '9': 0, '10': 'transfer'},
    {'1': 'transfer_asset', '3': 11, '4': 1, '5': 11, '6': '.TW.Tron.Proto.TransferAssetContract', '9': 0, '10': 'transferAsset'},
    {'1': 'freeze_balance', '3': 12, '4': 1, '5': 11, '6': '.TW.Tron.Proto.FreezeBalanceContract', '9': 0, '10': 'freezeBalance'},
    {'1': 'unfreeze_balance', '3': 13, '4': 1, '5': 11, '6': '.TW.Tron.Proto.UnfreezeBalanceContract', '9': 0, '10': 'unfreezeBalance'},
    {'1': 'unfreeze_asset', '3': 14, '4': 1, '5': 11, '6': '.TW.Tron.Proto.UnfreezeAssetContract', '9': 0, '10': 'unfreezeAsset'},
    {'1': 'withdraw_balance', '3': 15, '4': 1, '5': 11, '6': '.TW.Tron.Proto.WithdrawBalanceContract', '9': 0, '10': 'withdrawBalance'},
    {'1': 'vote_asset', '3': 16, '4': 1, '5': 11, '6': '.TW.Tron.Proto.VoteAssetContract', '9': 0, '10': 'voteAsset'},
    {'1': 'vote_witness', '3': 17, '4': 1, '5': 11, '6': '.TW.Tron.Proto.VoteWitnessContract', '9': 0, '10': 'voteWitness'},
    {'1': 'trigger_smart_contract', '3': 18, '4': 1, '5': 11, '6': '.TW.Tron.Proto.TriggerSmartContract', '9': 0, '10': 'triggerSmartContract'},
    {'1': 'transfer_trc20_contract', '3': 19, '4': 1, '5': 11, '6': '.TW.Tron.Proto.TransferTRC20Contract', '9': 0, '10': 'transferTrc20Contract'},
    {'1': 'freeze_balance_v2', '3': 20, '4': 1, '5': 11, '6': '.TW.Tron.Proto.FreezeBalanceV2Contract', '9': 0, '10': 'freezeBalanceV2'},
    {'1': 'unfreeze_balance_v2', '3': 21, '4': 1, '5': 11, '6': '.TW.Tron.Proto.UnfreezeBalanceV2Contract', '9': 0, '10': 'unfreezeBalanceV2'},
    {'1': 'withdraw_expire_unfreeze', '3': 23, '4': 1, '5': 11, '6': '.TW.Tron.Proto.WithdrawExpireUnfreezeContract', '9': 0, '10': 'withdrawExpireUnfreeze'},
    {'1': 'delegate_resource', '3': 24, '4': 1, '5': 11, '6': '.TW.Tron.Proto.DelegateResourceContract', '9': 0, '10': 'delegateResource'},
    {'1': 'undelegate_resource', '3': 25, '4': 1, '5': 11, '6': '.TW.Tron.Proto.UnDelegateResourceContract', '9': 0, '10': 'undelegateResource'},
  ],
  '8': [
    {'1': 'contract_oneof'},
  ],
};

/// Descriptor for `Transaction`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionDescriptor = $convert.base64Decode(
    'CgtUcmFuc2FjdGlvbhIcCgl0aW1lc3RhbXAYASABKANSCXRpbWVzdGFtcBIeCgpleHBpcmF0aW'
    '9uGAIgASgDUgpleHBpcmF0aW9uEj0KDGJsb2NrX2hlYWRlchgDIAEoCzIaLlRXLlRyb24uUHJv'
    'dG8uQmxvY2tIZWFkZXJSC2Jsb2NrSGVhZGVyEhsKCWZlZV9saW1pdBgEIAEoA1IIZmVlTGltaX'
    'QSPQoIdHJhbnNmZXIYCiABKAsyHy5UVy5Ucm9uLlByb3RvLlRyYW5zZmVyQ29udHJhY3RIAFII'
    'dHJhbnNmZXISTQoOdHJhbnNmZXJfYXNzZXQYCyABKAsyJC5UVy5Ucm9uLlByb3RvLlRyYW5zZm'
    'VyQXNzZXRDb250cmFjdEgAUg10cmFuc2ZlckFzc2V0Ek0KDmZyZWV6ZV9iYWxhbmNlGAwgASgL'
    'MiQuVFcuVHJvbi5Qcm90by5GcmVlemVCYWxhbmNlQ29udHJhY3RIAFINZnJlZXplQmFsYW5jZR'
    'JTChB1bmZyZWV6ZV9iYWxhbmNlGA0gASgLMiYuVFcuVHJvbi5Qcm90by5VbmZyZWV6ZUJhbGFu'
    'Y2VDb250cmFjdEgAUg91bmZyZWV6ZUJhbGFuY2USTQoOdW5mcmVlemVfYXNzZXQYDiABKAsyJC'
    '5UVy5Ucm9uLlByb3RvLlVuZnJlZXplQXNzZXRDb250cmFjdEgAUg11bmZyZWV6ZUFzc2V0ElMK'
    'EHdpdGhkcmF3X2JhbGFuY2UYDyABKAsyJi5UVy5Ucm9uLlByb3RvLldpdGhkcmF3QmFsYW5jZU'
    'NvbnRyYWN0SABSD3dpdGhkcmF3QmFsYW5jZRJBCgp2b3RlX2Fzc2V0GBAgASgLMiAuVFcuVHJv'
    'bi5Qcm90by5Wb3RlQXNzZXRDb250cmFjdEgAUgl2b3RlQXNzZXQSRwoMdm90ZV93aXRuZXNzGB'
    'EgASgLMiIuVFcuVHJvbi5Qcm90by5Wb3RlV2l0bmVzc0NvbnRyYWN0SABSC3ZvdGVXaXRuZXNz'
    'ElsKFnRyaWdnZXJfc21hcnRfY29udHJhY3QYEiABKAsyIy5UVy5Ucm9uLlByb3RvLlRyaWdnZX'
    'JTbWFydENvbnRyYWN0SABSFHRyaWdnZXJTbWFydENvbnRyYWN0El4KF3RyYW5zZmVyX3RyYzIw'
    'X2NvbnRyYWN0GBMgASgLMiQuVFcuVHJvbi5Qcm90by5UcmFuc2ZlclRSQzIwQ29udHJhY3RIAF'
    'IVdHJhbnNmZXJUcmMyMENvbnRyYWN0ElQKEWZyZWV6ZV9iYWxhbmNlX3YyGBQgASgLMiYuVFcu'
    'VHJvbi5Qcm90by5GcmVlemVCYWxhbmNlVjJDb250cmFjdEgAUg9mcmVlemVCYWxhbmNlVjISWg'
    'oTdW5mcmVlemVfYmFsYW5jZV92MhgVIAEoCzIoLlRXLlRyb24uUHJvdG8uVW5mcmVlemVCYWxh'
    'bmNlVjJDb250cmFjdEgAUhF1bmZyZWV6ZUJhbGFuY2VWMhJpChh3aXRoZHJhd19leHBpcmVfdW'
    '5mcmVlemUYFyABKAsyLS5UVy5Ucm9uLlByb3RvLldpdGhkcmF3RXhwaXJlVW5mcmVlemVDb250'
    'cmFjdEgAUhZ3aXRoZHJhd0V4cGlyZVVuZnJlZXplElYKEWRlbGVnYXRlX3Jlc291cmNlGBggAS'
    'gLMicuVFcuVHJvbi5Qcm90by5EZWxlZ2F0ZVJlc291cmNlQ29udHJhY3RIAFIQZGVsZWdhdGVS'
    'ZXNvdXJjZRJcChN1bmRlbGVnYXRlX3Jlc291cmNlGBkgASgLMikuVFcuVHJvbi5Qcm90by5Vbk'
    'RlbGVnYXRlUmVzb3VyY2VDb250cmFjdEgAUhJ1bmRlbGVnYXRlUmVzb3VyY2VCEAoOY29udHJh'
    'Y3Rfb25lb2Y=');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'transaction', '3': 1, '4': 1, '5': 11, '6': '.TW.Tron.Proto.Transaction', '10': 'transaction'},
    {'1': 'private_key', '3': 2, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'txId', '3': 3, '4': 1, '5': 9, '10': 'txId'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSPAoLdHJhbnNhY3Rpb24YASABKAsyGi5UVy5Ucm9uLlByb3RvLlRyYW'
    '5zYWN0aW9uUgt0cmFuc2FjdGlvbhIfCgtwcml2YXRlX2tleRgCIAEoDFIKcHJpdmF0ZUtleRIS'
    'CgR0eElkGAMgASgJUgR0eElk');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'id', '3': 1, '4': 1, '5': 12, '10': 'id'},
    {'1': 'signature', '3': 2, '4': 1, '5': 12, '10': 'signature'},
    {'1': 'ref_block_bytes', '3': 3, '4': 1, '5': 12, '10': 'refBlockBytes'},
    {'1': 'ref_block_hash', '3': 4, '4': 1, '5': 12, '10': 'refBlockHash'},
    {'1': 'json', '3': 5, '4': 1, '5': 9, '10': 'json'},
    {'1': 'error', '3': 6, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 7, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0Eg4KAmlkGAEgASgMUgJpZBIcCglzaWduYXR1cmUYAiABKAxSCXNpZ2'
    '5hdHVyZRImCg9yZWZfYmxvY2tfYnl0ZXMYAyABKAxSDXJlZkJsb2NrQnl0ZXMSJAoOcmVmX2Js'
    'b2NrX2hhc2gYBCABKAxSDHJlZkJsb2NrSGFzaBISCgRqc29uGAUgASgJUgRqc29uEjMKBWVycm'
    '9yGAYgASgOMh0uVFcuQ29tbW9uLlByb3RvLlNpZ25pbmdFcnJvclIFZXJyb3ISIwoNZXJyb3Jf'
    'bWVzc2FnZRgHIAEoCVIMZXJyb3JNZXNzYWdl');

