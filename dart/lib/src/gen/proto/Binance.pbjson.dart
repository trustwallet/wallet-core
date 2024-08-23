//
//  Generated code. Do not modify.
//  source: Binance.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use transactionDescriptor instead')
const Transaction$json = {
  '1': 'Transaction',
  '2': [
    {'1': 'msgs', '3': 1, '4': 3, '5': 12, '10': 'msgs'},
    {'1': 'signatures', '3': 2, '4': 3, '5': 12, '10': 'signatures'},
    {'1': 'memo', '3': 3, '4': 1, '5': 9, '10': 'memo'},
    {'1': 'source', '3': 4, '4': 1, '5': 3, '10': 'source'},
    {'1': 'data', '3': 5, '4': 1, '5': 12, '10': 'data'},
  ],
};

/// Descriptor for `Transaction`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionDescriptor = $convert.base64Decode(
    'CgtUcmFuc2FjdGlvbhISCgRtc2dzGAEgAygMUgRtc2dzEh4KCnNpZ25hdHVyZXMYAiADKAxSCn'
    'NpZ25hdHVyZXMSEgoEbWVtbxgDIAEoCVIEbWVtbxIWCgZzb3VyY2UYBCABKANSBnNvdXJjZRIS'
    'CgRkYXRhGAUgASgMUgRkYXRh');

@$core.Deprecated('Use signatureDescriptor instead')
const Signature$json = {
  '1': 'Signature',
  '2': [
    {'1': 'pub_key', '3': 1, '4': 1, '5': 12, '10': 'pubKey'},
    {'1': 'signature', '3': 2, '4': 1, '5': 12, '10': 'signature'},
    {'1': 'account_number', '3': 3, '4': 1, '5': 3, '10': 'accountNumber'},
    {'1': 'sequence', '3': 4, '4': 1, '5': 3, '10': 'sequence'},
  ],
};

/// Descriptor for `Signature`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signatureDescriptor = $convert.base64Decode(
    'CglTaWduYXR1cmUSFwoHcHViX2tleRgBIAEoDFIGcHViS2V5EhwKCXNpZ25hdHVyZRgCIAEoDF'
    'IJc2lnbmF0dXJlEiUKDmFjY291bnRfbnVtYmVyGAMgASgDUg1hY2NvdW50TnVtYmVyEhoKCHNl'
    'cXVlbmNlGAQgASgDUghzZXF1ZW5jZQ==');

@$core.Deprecated('Use tradeOrderDescriptor instead')
const TradeOrder$json = {
  '1': 'TradeOrder',
  '2': [
    {'1': 'sender', '3': 1, '4': 1, '5': 12, '10': 'sender'},
    {'1': 'id', '3': 2, '4': 1, '5': 9, '10': 'id'},
    {'1': 'symbol', '3': 3, '4': 1, '5': 9, '10': 'symbol'},
    {'1': 'ordertype', '3': 4, '4': 1, '5': 3, '10': 'ordertype'},
    {'1': 'side', '3': 5, '4': 1, '5': 3, '10': 'side'},
    {'1': 'price', '3': 6, '4': 1, '5': 3, '10': 'price'},
    {'1': 'quantity', '3': 7, '4': 1, '5': 3, '10': 'quantity'},
    {'1': 'timeinforce', '3': 8, '4': 1, '5': 3, '10': 'timeinforce'},
  ],
};

/// Descriptor for `TradeOrder`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tradeOrderDescriptor = $convert.base64Decode(
    'CgpUcmFkZU9yZGVyEhYKBnNlbmRlchgBIAEoDFIGc2VuZGVyEg4KAmlkGAIgASgJUgJpZBIWCg'
    'ZzeW1ib2wYAyABKAlSBnN5bWJvbBIcCglvcmRlcnR5cGUYBCABKANSCW9yZGVydHlwZRISCgRz'
    'aWRlGAUgASgDUgRzaWRlEhQKBXByaWNlGAYgASgDUgVwcmljZRIaCghxdWFudGl0eRgHIAEoA1'
    'IIcXVhbnRpdHkSIAoLdGltZWluZm9yY2UYCCABKANSC3RpbWVpbmZvcmNl');

@$core.Deprecated('Use cancelTradeOrderDescriptor instead')
const CancelTradeOrder$json = {
  '1': 'CancelTradeOrder',
  '2': [
    {'1': 'sender', '3': 1, '4': 1, '5': 12, '10': 'sender'},
    {'1': 'symbol', '3': 2, '4': 1, '5': 9, '10': 'symbol'},
    {'1': 'refid', '3': 3, '4': 1, '5': 9, '10': 'refid'},
  ],
};

/// Descriptor for `CancelTradeOrder`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List cancelTradeOrderDescriptor = $convert.base64Decode(
    'ChBDYW5jZWxUcmFkZU9yZGVyEhYKBnNlbmRlchgBIAEoDFIGc2VuZGVyEhYKBnN5bWJvbBgCIA'
    'EoCVIGc3ltYm9sEhQKBXJlZmlkGAMgASgJUgVyZWZpZA==');

@$core.Deprecated('Use sendOrderDescriptor instead')
const SendOrder$json = {
  '1': 'SendOrder',
  '2': [
    {'1': 'inputs', '3': 1, '4': 3, '5': 11, '6': '.TW.Binance.Proto.SendOrder.Input', '10': 'inputs'},
    {'1': 'outputs', '3': 2, '4': 3, '5': 11, '6': '.TW.Binance.Proto.SendOrder.Output', '10': 'outputs'},
  ],
  '3': [SendOrder_Token$json, SendOrder_Input$json, SendOrder_Output$json],
};

@$core.Deprecated('Use sendOrderDescriptor instead')
const SendOrder_Token$json = {
  '1': 'Token',
  '2': [
    {'1': 'denom', '3': 1, '4': 1, '5': 9, '10': 'denom'},
    {'1': 'amount', '3': 2, '4': 1, '5': 3, '10': 'amount'},
  ],
};

@$core.Deprecated('Use sendOrderDescriptor instead')
const SendOrder_Input$json = {
  '1': 'Input',
  '2': [
    {'1': 'address', '3': 1, '4': 1, '5': 12, '10': 'address'},
    {'1': 'coins', '3': 2, '4': 3, '5': 11, '6': '.TW.Binance.Proto.SendOrder.Token', '10': 'coins'},
  ],
};

@$core.Deprecated('Use sendOrderDescriptor instead')
const SendOrder_Output$json = {
  '1': 'Output',
  '2': [
    {'1': 'address', '3': 1, '4': 1, '5': 12, '10': 'address'},
    {'1': 'coins', '3': 2, '4': 3, '5': 11, '6': '.TW.Binance.Proto.SendOrder.Token', '10': 'coins'},
  ],
};

/// Descriptor for `SendOrder`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List sendOrderDescriptor = $convert.base64Decode(
    'CglTZW5kT3JkZXISOQoGaW5wdXRzGAEgAygLMiEuVFcuQmluYW5jZS5Qcm90by5TZW5kT3JkZX'
    'IuSW5wdXRSBmlucHV0cxI8CgdvdXRwdXRzGAIgAygLMiIuVFcuQmluYW5jZS5Qcm90by5TZW5k'
    'T3JkZXIuT3V0cHV0UgdvdXRwdXRzGjUKBVRva2VuEhQKBWRlbm9tGAEgASgJUgVkZW5vbRIWCg'
    'ZhbW91bnQYAiABKANSBmFtb3VudBpaCgVJbnB1dBIYCgdhZGRyZXNzGAEgASgMUgdhZGRyZXNz'
    'EjcKBWNvaW5zGAIgAygLMiEuVFcuQmluYW5jZS5Qcm90by5TZW5kT3JkZXIuVG9rZW5SBWNvaW'
    '5zGlsKBk91dHB1dBIYCgdhZGRyZXNzGAEgASgMUgdhZGRyZXNzEjcKBWNvaW5zGAIgAygLMiEu'
    'VFcuQmluYW5jZS5Qcm90by5TZW5kT3JkZXIuVG9rZW5SBWNvaW5z');

@$core.Deprecated('Use tokenIssueOrderDescriptor instead')
const TokenIssueOrder$json = {
  '1': 'TokenIssueOrder',
  '2': [
    {'1': 'from', '3': 1, '4': 1, '5': 12, '10': 'from'},
    {'1': 'name', '3': 2, '4': 1, '5': 9, '10': 'name'},
    {'1': 'symbol', '3': 3, '4': 1, '5': 9, '10': 'symbol'},
    {'1': 'total_supply', '3': 4, '4': 1, '5': 3, '10': 'totalSupply'},
    {'1': 'mintable', '3': 5, '4': 1, '5': 8, '10': 'mintable'},
  ],
};

/// Descriptor for `TokenIssueOrder`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tokenIssueOrderDescriptor = $convert.base64Decode(
    'Cg9Ub2tlbklzc3VlT3JkZXISEgoEZnJvbRgBIAEoDFIEZnJvbRISCgRuYW1lGAIgASgJUgRuYW'
    '1lEhYKBnN5bWJvbBgDIAEoCVIGc3ltYm9sEiEKDHRvdGFsX3N1cHBseRgEIAEoA1ILdG90YWxT'
    'dXBwbHkSGgoIbWludGFibGUYBSABKAhSCG1pbnRhYmxl');

@$core.Deprecated('Use tokenMintOrderDescriptor instead')
const TokenMintOrder$json = {
  '1': 'TokenMintOrder',
  '2': [
    {'1': 'from', '3': 1, '4': 1, '5': 12, '10': 'from'},
    {'1': 'symbol', '3': 2, '4': 1, '5': 9, '10': 'symbol'},
    {'1': 'amount', '3': 3, '4': 1, '5': 3, '10': 'amount'},
  ],
};

/// Descriptor for `TokenMintOrder`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tokenMintOrderDescriptor = $convert.base64Decode(
    'Cg5Ub2tlbk1pbnRPcmRlchISCgRmcm9tGAEgASgMUgRmcm9tEhYKBnN5bWJvbBgCIAEoCVIGc3'
    'ltYm9sEhYKBmFtb3VudBgDIAEoA1IGYW1vdW50');

@$core.Deprecated('Use tokenBurnOrderDescriptor instead')
const TokenBurnOrder$json = {
  '1': 'TokenBurnOrder',
  '2': [
    {'1': 'from', '3': 1, '4': 1, '5': 12, '10': 'from'},
    {'1': 'symbol', '3': 2, '4': 1, '5': 9, '10': 'symbol'},
    {'1': 'amount', '3': 3, '4': 1, '5': 3, '10': 'amount'},
  ],
};

/// Descriptor for `TokenBurnOrder`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tokenBurnOrderDescriptor = $convert.base64Decode(
    'Cg5Ub2tlbkJ1cm5PcmRlchISCgRmcm9tGAEgASgMUgRmcm9tEhYKBnN5bWJvbBgCIAEoCVIGc3'
    'ltYm9sEhYKBmFtb3VudBgDIAEoA1IGYW1vdW50');

@$core.Deprecated('Use tokenFreezeOrderDescriptor instead')
const TokenFreezeOrder$json = {
  '1': 'TokenFreezeOrder',
  '2': [
    {'1': 'from', '3': 1, '4': 1, '5': 12, '10': 'from'},
    {'1': 'symbol', '3': 2, '4': 1, '5': 9, '10': 'symbol'},
    {'1': 'amount', '3': 3, '4': 1, '5': 3, '10': 'amount'},
  ],
};

/// Descriptor for `TokenFreezeOrder`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tokenFreezeOrderDescriptor = $convert.base64Decode(
    'ChBUb2tlbkZyZWV6ZU9yZGVyEhIKBGZyb20YASABKAxSBGZyb20SFgoGc3ltYm9sGAIgASgJUg'
    'ZzeW1ib2wSFgoGYW1vdW50GAMgASgDUgZhbW91bnQ=');

@$core.Deprecated('Use tokenUnfreezeOrderDescriptor instead')
const TokenUnfreezeOrder$json = {
  '1': 'TokenUnfreezeOrder',
  '2': [
    {'1': 'from', '3': 1, '4': 1, '5': 12, '10': 'from'},
    {'1': 'symbol', '3': 2, '4': 1, '5': 9, '10': 'symbol'},
    {'1': 'amount', '3': 3, '4': 1, '5': 3, '10': 'amount'},
  ],
};

/// Descriptor for `TokenUnfreezeOrder`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tokenUnfreezeOrderDescriptor = $convert.base64Decode(
    'ChJUb2tlblVuZnJlZXplT3JkZXISEgoEZnJvbRgBIAEoDFIEZnJvbRIWCgZzeW1ib2wYAiABKA'
    'lSBnN5bWJvbBIWCgZhbW91bnQYAyABKANSBmFtb3VudA==');

@$core.Deprecated('Use hTLTOrderDescriptor instead')
const HTLTOrder$json = {
  '1': 'HTLTOrder',
  '2': [
    {'1': 'from', '3': 1, '4': 1, '5': 12, '10': 'from'},
    {'1': 'to', '3': 2, '4': 1, '5': 12, '10': 'to'},
    {'1': 'recipient_other_chain', '3': 3, '4': 1, '5': 9, '10': 'recipientOtherChain'},
    {'1': 'sender_other_chain', '3': 4, '4': 1, '5': 9, '10': 'senderOtherChain'},
    {'1': 'random_number_hash', '3': 5, '4': 1, '5': 12, '10': 'randomNumberHash'},
    {'1': 'timestamp', '3': 6, '4': 1, '5': 3, '10': 'timestamp'},
    {'1': 'amount', '3': 7, '4': 3, '5': 11, '6': '.TW.Binance.Proto.SendOrder.Token', '10': 'amount'},
    {'1': 'expected_income', '3': 8, '4': 1, '5': 9, '10': 'expectedIncome'},
    {'1': 'height_span', '3': 9, '4': 1, '5': 3, '10': 'heightSpan'},
    {'1': 'cross_chain', '3': 10, '4': 1, '5': 8, '10': 'crossChain'},
  ],
};

/// Descriptor for `HTLTOrder`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List hTLTOrderDescriptor = $convert.base64Decode(
    'CglIVExUT3JkZXISEgoEZnJvbRgBIAEoDFIEZnJvbRIOCgJ0bxgCIAEoDFICdG8SMgoVcmVjaX'
    'BpZW50X290aGVyX2NoYWluGAMgASgJUhNyZWNpcGllbnRPdGhlckNoYWluEiwKEnNlbmRlcl9v'
    'dGhlcl9jaGFpbhgEIAEoCVIQc2VuZGVyT3RoZXJDaGFpbhIsChJyYW5kb21fbnVtYmVyX2hhc2'
    'gYBSABKAxSEHJhbmRvbU51bWJlckhhc2gSHAoJdGltZXN0YW1wGAYgASgDUgl0aW1lc3RhbXAS'
    'OQoGYW1vdW50GAcgAygLMiEuVFcuQmluYW5jZS5Qcm90by5TZW5kT3JkZXIuVG9rZW5SBmFtb3'
    'VudBInCg9leHBlY3RlZF9pbmNvbWUYCCABKAlSDmV4cGVjdGVkSW5jb21lEh8KC2hlaWdodF9z'
    'cGFuGAkgASgDUgpoZWlnaHRTcGFuEh8KC2Nyb3NzX2NoYWluGAogASgIUgpjcm9zc0NoYWlu');

@$core.Deprecated('Use depositHTLTOrderDescriptor instead')
const DepositHTLTOrder$json = {
  '1': 'DepositHTLTOrder',
  '2': [
    {'1': 'from', '3': 1, '4': 1, '5': 12, '10': 'from'},
    {'1': 'amount', '3': 2, '4': 3, '5': 11, '6': '.TW.Binance.Proto.SendOrder.Token', '10': 'amount'},
    {'1': 'swap_id', '3': 3, '4': 1, '5': 12, '10': 'swapId'},
  ],
};

/// Descriptor for `DepositHTLTOrder`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List depositHTLTOrderDescriptor = $convert.base64Decode(
    'ChBEZXBvc2l0SFRMVE9yZGVyEhIKBGZyb20YASABKAxSBGZyb20SOQoGYW1vdW50GAIgAygLMi'
    'EuVFcuQmluYW5jZS5Qcm90by5TZW5kT3JkZXIuVG9rZW5SBmFtb3VudBIXCgdzd2FwX2lkGAMg'
    'ASgMUgZzd2FwSWQ=');

@$core.Deprecated('Use claimHTLOrderDescriptor instead')
const ClaimHTLOrder$json = {
  '1': 'ClaimHTLOrder',
  '2': [
    {'1': 'from', '3': 1, '4': 1, '5': 12, '10': 'from'},
    {'1': 'swap_id', '3': 2, '4': 1, '5': 12, '10': 'swapId'},
    {'1': 'random_number', '3': 3, '4': 1, '5': 12, '10': 'randomNumber'},
  ],
};

/// Descriptor for `ClaimHTLOrder`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List claimHTLOrderDescriptor = $convert.base64Decode(
    'Cg1DbGFpbUhUTE9yZGVyEhIKBGZyb20YASABKAxSBGZyb20SFwoHc3dhcF9pZBgCIAEoDFIGc3'
    'dhcElkEiMKDXJhbmRvbV9udW1iZXIYAyABKAxSDHJhbmRvbU51bWJlcg==');

@$core.Deprecated('Use refundHTLTOrderDescriptor instead')
const RefundHTLTOrder$json = {
  '1': 'RefundHTLTOrder',
  '2': [
    {'1': 'from', '3': 1, '4': 1, '5': 12, '10': 'from'},
    {'1': 'swap_id', '3': 2, '4': 1, '5': 12, '10': 'swapId'},
  ],
};

/// Descriptor for `RefundHTLTOrder`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List refundHTLTOrderDescriptor = $convert.base64Decode(
    'Cg9SZWZ1bmRIVExUT3JkZXISEgoEZnJvbRgBIAEoDFIEZnJvbRIXCgdzd2FwX2lkGAIgASgMUg'
    'Zzd2FwSWQ=');

@$core.Deprecated('Use transferOutDescriptor instead')
const TransferOut$json = {
  '1': 'TransferOut',
  '2': [
    {'1': 'from', '3': 1, '4': 1, '5': 12, '10': 'from'},
    {'1': 'to', '3': 2, '4': 1, '5': 12, '10': 'to'},
    {'1': 'amount', '3': 3, '4': 1, '5': 11, '6': '.TW.Binance.Proto.SendOrder.Token', '10': 'amount'},
    {'1': 'expire_time', '3': 4, '4': 1, '5': 3, '10': 'expireTime'},
  ],
};

/// Descriptor for `TransferOut`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transferOutDescriptor = $convert.base64Decode(
    'CgtUcmFuc2Zlck91dBISCgRmcm9tGAEgASgMUgRmcm9tEg4KAnRvGAIgASgMUgJ0bxI5CgZhbW'
    '91bnQYAyABKAsyIS5UVy5CaW5hbmNlLlByb3RvLlNlbmRPcmRlci5Ub2tlblIGYW1vdW50Eh8K'
    'C2V4cGlyZV90aW1lGAQgASgDUgpleHBpcmVUaW1l');

@$core.Deprecated('Use sideChainDelegateDescriptor instead')
const SideChainDelegate$json = {
  '1': 'SideChainDelegate',
  '2': [
    {'1': 'delegator_addr', '3': 1, '4': 1, '5': 12, '10': 'delegatorAddr'},
    {'1': 'validator_addr', '3': 2, '4': 1, '5': 12, '10': 'validatorAddr'},
    {'1': 'delegation', '3': 3, '4': 1, '5': 11, '6': '.TW.Binance.Proto.SendOrder.Token', '10': 'delegation'},
    {'1': 'chain_id', '3': 4, '4': 1, '5': 9, '10': 'chainId'},
  ],
};

/// Descriptor for `SideChainDelegate`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List sideChainDelegateDescriptor = $convert.base64Decode(
    'ChFTaWRlQ2hhaW5EZWxlZ2F0ZRIlCg5kZWxlZ2F0b3JfYWRkchgBIAEoDFINZGVsZWdhdG9yQW'
    'RkchIlCg52YWxpZGF0b3JfYWRkchgCIAEoDFINdmFsaWRhdG9yQWRkchJBCgpkZWxlZ2F0aW9u'
    'GAMgASgLMiEuVFcuQmluYW5jZS5Qcm90by5TZW5kT3JkZXIuVG9rZW5SCmRlbGVnYXRpb24SGQ'
    'oIY2hhaW5faWQYBCABKAlSB2NoYWluSWQ=');

@$core.Deprecated('Use sideChainRedelegateDescriptor instead')
const SideChainRedelegate$json = {
  '1': 'SideChainRedelegate',
  '2': [
    {'1': 'delegator_addr', '3': 1, '4': 1, '5': 12, '10': 'delegatorAddr'},
    {'1': 'validator_src_addr', '3': 2, '4': 1, '5': 12, '10': 'validatorSrcAddr'},
    {'1': 'validator_dst_addr', '3': 3, '4': 1, '5': 12, '10': 'validatorDstAddr'},
    {'1': 'amount', '3': 4, '4': 1, '5': 11, '6': '.TW.Binance.Proto.SendOrder.Token', '10': 'amount'},
    {'1': 'chain_id', '3': 5, '4': 1, '5': 9, '10': 'chainId'},
  ],
};

/// Descriptor for `SideChainRedelegate`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List sideChainRedelegateDescriptor = $convert.base64Decode(
    'ChNTaWRlQ2hhaW5SZWRlbGVnYXRlEiUKDmRlbGVnYXRvcl9hZGRyGAEgASgMUg1kZWxlZ2F0b3'
    'JBZGRyEiwKEnZhbGlkYXRvcl9zcmNfYWRkchgCIAEoDFIQdmFsaWRhdG9yU3JjQWRkchIsChJ2'
    'YWxpZGF0b3JfZHN0X2FkZHIYAyABKAxSEHZhbGlkYXRvckRzdEFkZHISOQoGYW1vdW50GAQgAS'
    'gLMiEuVFcuQmluYW5jZS5Qcm90by5TZW5kT3JkZXIuVG9rZW5SBmFtb3VudBIZCghjaGFpbl9p'
    'ZBgFIAEoCVIHY2hhaW5JZA==');

@$core.Deprecated('Use sideChainUndelegateDescriptor instead')
const SideChainUndelegate$json = {
  '1': 'SideChainUndelegate',
  '2': [
    {'1': 'delegator_addr', '3': 1, '4': 1, '5': 12, '10': 'delegatorAddr'},
    {'1': 'validator_addr', '3': 2, '4': 1, '5': 12, '10': 'validatorAddr'},
    {'1': 'amount', '3': 3, '4': 1, '5': 11, '6': '.TW.Binance.Proto.SendOrder.Token', '10': 'amount'},
    {'1': 'chain_id', '3': 4, '4': 1, '5': 9, '10': 'chainId'},
  ],
};

/// Descriptor for `SideChainUndelegate`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List sideChainUndelegateDescriptor = $convert.base64Decode(
    'ChNTaWRlQ2hhaW5VbmRlbGVnYXRlEiUKDmRlbGVnYXRvcl9hZGRyGAEgASgMUg1kZWxlZ2F0b3'
    'JBZGRyEiUKDnZhbGlkYXRvcl9hZGRyGAIgASgMUg12YWxpZGF0b3JBZGRyEjkKBmFtb3VudBgD'
    'IAEoCzIhLlRXLkJpbmFuY2UuUHJvdG8uU2VuZE9yZGVyLlRva2VuUgZhbW91bnQSGQoIY2hhaW'
    '5faWQYBCABKAlSB2NoYWluSWQ=');

@$core.Deprecated('Use sideChainStakeMigrationDescriptor instead')
const SideChainStakeMigration$json = {
  '1': 'SideChainStakeMigration',
  '2': [
    {'1': 'validator_src_addr', '3': 1, '4': 1, '5': 12, '10': 'validatorSrcAddr'},
    {'1': 'validator_dst_addr', '3': 2, '4': 1, '5': 12, '10': 'validatorDstAddr'},
    {'1': 'delegator_addr', '3': 3, '4': 1, '5': 12, '10': 'delegatorAddr'},
    {'1': 'refund_addr', '3': 4, '4': 1, '5': 12, '10': 'refundAddr'},
    {'1': 'amount', '3': 5, '4': 1, '5': 11, '6': '.TW.Binance.Proto.SendOrder.Token', '10': 'amount'},
  ],
};

/// Descriptor for `SideChainStakeMigration`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List sideChainStakeMigrationDescriptor = $convert.base64Decode(
    'ChdTaWRlQ2hhaW5TdGFrZU1pZ3JhdGlvbhIsChJ2YWxpZGF0b3Jfc3JjX2FkZHIYASABKAxSEH'
    'ZhbGlkYXRvclNyY0FkZHISLAoSdmFsaWRhdG9yX2RzdF9hZGRyGAIgASgMUhB2YWxpZGF0b3JE'
    'c3RBZGRyEiUKDmRlbGVnYXRvcl9hZGRyGAMgASgMUg1kZWxlZ2F0b3JBZGRyEh8KC3JlZnVuZF'
    '9hZGRyGAQgASgMUgpyZWZ1bmRBZGRyEjkKBmFtb3VudBgFIAEoCzIhLlRXLkJpbmFuY2UuUHJv'
    'dG8uU2VuZE9yZGVyLlRva2VuUgZhbW91bnQ=');

@$core.Deprecated('Use timeLockOrderDescriptor instead')
const TimeLockOrder$json = {
  '1': 'TimeLockOrder',
  '2': [
    {'1': 'from_address', '3': 1, '4': 1, '5': 12, '10': 'fromAddress'},
    {'1': 'description', '3': 2, '4': 1, '5': 9, '10': 'description'},
    {'1': 'amount', '3': 3, '4': 3, '5': 11, '6': '.TW.Binance.Proto.SendOrder.Token', '10': 'amount'},
    {'1': 'lock_time', '3': 4, '4': 1, '5': 3, '10': 'lockTime'},
  ],
};

/// Descriptor for `TimeLockOrder`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List timeLockOrderDescriptor = $convert.base64Decode(
    'Cg1UaW1lTG9ja09yZGVyEiEKDGZyb21fYWRkcmVzcxgBIAEoDFILZnJvbUFkZHJlc3MSIAoLZG'
    'VzY3JpcHRpb24YAiABKAlSC2Rlc2NyaXB0aW9uEjkKBmFtb3VudBgDIAMoCzIhLlRXLkJpbmFu'
    'Y2UuUHJvdG8uU2VuZE9yZGVyLlRva2VuUgZhbW91bnQSGwoJbG9ja190aW1lGAQgASgDUghsb2'
    'NrVGltZQ==');

@$core.Deprecated('Use timeRelockOrderDescriptor instead')
const TimeRelockOrder$json = {
  '1': 'TimeRelockOrder',
  '2': [
    {'1': 'from_address', '3': 1, '4': 1, '5': 12, '10': 'fromAddress'},
    {'1': 'id', '3': 2, '4': 1, '5': 3, '10': 'id'},
    {'1': 'description', '3': 3, '4': 1, '5': 9, '10': 'description'},
    {'1': 'amount', '3': 4, '4': 3, '5': 11, '6': '.TW.Binance.Proto.SendOrder.Token', '10': 'amount'},
    {'1': 'lock_time', '3': 5, '4': 1, '5': 3, '10': 'lockTime'},
  ],
};

/// Descriptor for `TimeRelockOrder`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List timeRelockOrderDescriptor = $convert.base64Decode(
    'Cg9UaW1lUmVsb2NrT3JkZXISIQoMZnJvbV9hZGRyZXNzGAEgASgMUgtmcm9tQWRkcmVzcxIOCg'
    'JpZBgCIAEoA1ICaWQSIAoLZGVzY3JpcHRpb24YAyABKAlSC2Rlc2NyaXB0aW9uEjkKBmFtb3Vu'
    'dBgEIAMoCzIhLlRXLkJpbmFuY2UuUHJvdG8uU2VuZE9yZGVyLlRva2VuUgZhbW91bnQSGwoJbG'
    '9ja190aW1lGAUgASgDUghsb2NrVGltZQ==');

@$core.Deprecated('Use timeUnlockOrderDescriptor instead')
const TimeUnlockOrder$json = {
  '1': 'TimeUnlockOrder',
  '2': [
    {'1': 'from_address', '3': 1, '4': 1, '5': 12, '10': 'fromAddress'},
    {'1': 'id', '3': 2, '4': 1, '5': 3, '10': 'id'},
  ],
};

/// Descriptor for `TimeUnlockOrder`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List timeUnlockOrderDescriptor = $convert.base64Decode(
    'Cg9UaW1lVW5sb2NrT3JkZXISIQoMZnJvbV9hZGRyZXNzGAEgASgMUgtmcm9tQWRkcmVzcxIOCg'
    'JpZBgCIAEoA1ICaWQ=');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'chain_id', '3': 1, '4': 1, '5': 9, '10': 'chainId'},
    {'1': 'account_number', '3': 2, '4': 1, '5': 3, '10': 'accountNumber'},
    {'1': 'sequence', '3': 3, '4': 1, '5': 3, '10': 'sequence'},
    {'1': 'source', '3': 4, '4': 1, '5': 3, '10': 'source'},
    {'1': 'memo', '3': 5, '4': 1, '5': 9, '10': 'memo'},
    {'1': 'private_key', '3': 6, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'trade_order', '3': 8, '4': 1, '5': 11, '6': '.TW.Binance.Proto.TradeOrder', '9': 0, '10': 'tradeOrder'},
    {'1': 'cancel_trade_order', '3': 9, '4': 1, '5': 11, '6': '.TW.Binance.Proto.CancelTradeOrder', '9': 0, '10': 'cancelTradeOrder'},
    {'1': 'send_order', '3': 10, '4': 1, '5': 11, '6': '.TW.Binance.Proto.SendOrder', '9': 0, '10': 'sendOrder'},
    {'1': 'freeze_order', '3': 11, '4': 1, '5': 11, '6': '.TW.Binance.Proto.TokenFreezeOrder', '9': 0, '10': 'freezeOrder'},
    {'1': 'unfreeze_order', '3': 12, '4': 1, '5': 11, '6': '.TW.Binance.Proto.TokenUnfreezeOrder', '9': 0, '10': 'unfreezeOrder'},
    {'1': 'htlt_order', '3': 13, '4': 1, '5': 11, '6': '.TW.Binance.Proto.HTLTOrder', '9': 0, '10': 'htltOrder'},
    {'1': 'depositHTLT_order', '3': 14, '4': 1, '5': 11, '6': '.TW.Binance.Proto.DepositHTLTOrder', '9': 0, '10': 'depositHTLTOrder'},
    {'1': 'claimHTLT_order', '3': 15, '4': 1, '5': 11, '6': '.TW.Binance.Proto.ClaimHTLOrder', '9': 0, '10': 'claimHTLTOrder'},
    {'1': 'refundHTLT_order', '3': 16, '4': 1, '5': 11, '6': '.TW.Binance.Proto.RefundHTLTOrder', '9': 0, '10': 'refundHTLTOrder'},
    {'1': 'issue_order', '3': 17, '4': 1, '5': 11, '6': '.TW.Binance.Proto.TokenIssueOrder', '9': 0, '10': 'issueOrder'},
    {'1': 'mint_order', '3': 18, '4': 1, '5': 11, '6': '.TW.Binance.Proto.TokenMintOrder', '9': 0, '10': 'mintOrder'},
    {'1': 'burn_order', '3': 19, '4': 1, '5': 11, '6': '.TW.Binance.Proto.TokenBurnOrder', '9': 0, '10': 'burnOrder'},
    {'1': 'transfer_out_order', '3': 20, '4': 1, '5': 11, '6': '.TW.Binance.Proto.TransferOut', '9': 0, '10': 'transferOutOrder'},
    {'1': 'side_delegate_order', '3': 21, '4': 1, '5': 11, '6': '.TW.Binance.Proto.SideChainDelegate', '9': 0, '10': 'sideDelegateOrder'},
    {'1': 'side_redelegate_order', '3': 22, '4': 1, '5': 11, '6': '.TW.Binance.Proto.SideChainRedelegate', '9': 0, '10': 'sideRedelegateOrder'},
    {'1': 'side_undelegate_order', '3': 23, '4': 1, '5': 11, '6': '.TW.Binance.Proto.SideChainUndelegate', '9': 0, '10': 'sideUndelegateOrder'},
    {'1': 'time_lock_order', '3': 24, '4': 1, '5': 11, '6': '.TW.Binance.Proto.TimeLockOrder', '9': 0, '10': 'timeLockOrder'},
    {'1': 'time_relock_order', '3': 25, '4': 1, '5': 11, '6': '.TW.Binance.Proto.TimeRelockOrder', '9': 0, '10': 'timeRelockOrder'},
    {'1': 'time_unlock_order', '3': 26, '4': 1, '5': 11, '6': '.TW.Binance.Proto.TimeUnlockOrder', '9': 0, '10': 'timeUnlockOrder'},
    {'1': 'side_stake_migration_order', '3': 27, '4': 1, '5': 11, '6': '.TW.Binance.Proto.SideChainStakeMigration', '9': 0, '10': 'sideStakeMigrationOrder'},
  ],
  '8': [
    {'1': 'order_oneof'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSGQoIY2hhaW5faWQYASABKAlSB2NoYWluSWQSJQoOYWNjb3VudF9udW'
    '1iZXIYAiABKANSDWFjY291bnROdW1iZXISGgoIc2VxdWVuY2UYAyABKANSCHNlcXVlbmNlEhYK'
    'BnNvdXJjZRgEIAEoA1IGc291cmNlEhIKBG1lbW8YBSABKAlSBG1lbW8SHwoLcHJpdmF0ZV9rZX'
    'kYBiABKAxSCnByaXZhdGVLZXkSPwoLdHJhZGVfb3JkZXIYCCABKAsyHC5UVy5CaW5hbmNlLlBy'
    'b3RvLlRyYWRlT3JkZXJIAFIKdHJhZGVPcmRlchJSChJjYW5jZWxfdHJhZGVfb3JkZXIYCSABKA'
    'syIi5UVy5CaW5hbmNlLlByb3RvLkNhbmNlbFRyYWRlT3JkZXJIAFIQY2FuY2VsVHJhZGVPcmRl'
    'chI8CgpzZW5kX29yZGVyGAogASgLMhsuVFcuQmluYW5jZS5Qcm90by5TZW5kT3JkZXJIAFIJc2'
    'VuZE9yZGVyEkcKDGZyZWV6ZV9vcmRlchgLIAEoCzIiLlRXLkJpbmFuY2UuUHJvdG8uVG9rZW5G'
    'cmVlemVPcmRlckgAUgtmcmVlemVPcmRlchJNCg51bmZyZWV6ZV9vcmRlchgMIAEoCzIkLlRXLk'
    'JpbmFuY2UuUHJvdG8uVG9rZW5VbmZyZWV6ZU9yZGVySABSDXVuZnJlZXplT3JkZXISPAoKaHRs'
    'dF9vcmRlchgNIAEoCzIbLlRXLkJpbmFuY2UuUHJvdG8uSFRMVE9yZGVySABSCWh0bHRPcmRlch'
    'JRChFkZXBvc2l0SFRMVF9vcmRlchgOIAEoCzIiLlRXLkJpbmFuY2UuUHJvdG8uRGVwb3NpdEhU'
    'TFRPcmRlckgAUhBkZXBvc2l0SFRMVE9yZGVyEkoKD2NsYWltSFRMVF9vcmRlchgPIAEoCzIfLl'
    'RXLkJpbmFuY2UuUHJvdG8uQ2xhaW1IVExPcmRlckgAUg5jbGFpbUhUTFRPcmRlchJOChByZWZ1'
    'bmRIVExUX29yZGVyGBAgASgLMiEuVFcuQmluYW5jZS5Qcm90by5SZWZ1bmRIVExUT3JkZXJIAF'
    'IPcmVmdW5kSFRMVE9yZGVyEkQKC2lzc3VlX29yZGVyGBEgASgLMiEuVFcuQmluYW5jZS5Qcm90'
    'by5Ub2tlbklzc3VlT3JkZXJIAFIKaXNzdWVPcmRlchJBCgptaW50X29yZGVyGBIgASgLMiAuVF'
    'cuQmluYW5jZS5Qcm90by5Ub2tlbk1pbnRPcmRlckgAUgltaW50T3JkZXISQQoKYnVybl9vcmRl'
    'chgTIAEoCzIgLlRXLkJpbmFuY2UuUHJvdG8uVG9rZW5CdXJuT3JkZXJIAFIJYnVybk9yZGVyEk'
    '0KEnRyYW5zZmVyX291dF9vcmRlchgUIAEoCzIdLlRXLkJpbmFuY2UuUHJvdG8uVHJhbnNmZXJP'
    'dXRIAFIQdHJhbnNmZXJPdXRPcmRlchJVChNzaWRlX2RlbGVnYXRlX29yZGVyGBUgASgLMiMuVF'
    'cuQmluYW5jZS5Qcm90by5TaWRlQ2hhaW5EZWxlZ2F0ZUgAUhFzaWRlRGVsZWdhdGVPcmRlchJb'
    'ChVzaWRlX3JlZGVsZWdhdGVfb3JkZXIYFiABKAsyJS5UVy5CaW5hbmNlLlByb3RvLlNpZGVDaG'
    'FpblJlZGVsZWdhdGVIAFITc2lkZVJlZGVsZWdhdGVPcmRlchJbChVzaWRlX3VuZGVsZWdhdGVf'
    'b3JkZXIYFyABKAsyJS5UVy5CaW5hbmNlLlByb3RvLlNpZGVDaGFpblVuZGVsZWdhdGVIAFITc2'
    'lkZVVuZGVsZWdhdGVPcmRlchJJCg90aW1lX2xvY2tfb3JkZXIYGCABKAsyHy5UVy5CaW5hbmNl'
    'LlByb3RvLlRpbWVMb2NrT3JkZXJIAFINdGltZUxvY2tPcmRlchJPChF0aW1lX3JlbG9ja19vcm'
    'RlchgZIAEoCzIhLlRXLkJpbmFuY2UuUHJvdG8uVGltZVJlbG9ja09yZGVySABSD3RpbWVSZWxv'
    'Y2tPcmRlchJPChF0aW1lX3VubG9ja19vcmRlchgaIAEoCzIhLlRXLkJpbmFuY2UuUHJvdG8uVG'
    'ltZVVubG9ja09yZGVySABSD3RpbWVVbmxvY2tPcmRlchJoChpzaWRlX3N0YWtlX21pZ3JhdGlv'
    'bl9vcmRlchgbIAEoCzIpLlRXLkJpbmFuY2UuUHJvdG8uU2lkZUNoYWluU3Rha2VNaWdyYXRpb2'
    '5IAFIXc2lkZVN0YWtlTWlncmF0aW9uT3JkZXJCDQoLb3JkZXJfb25lb2Y=');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'error', '3': 2, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 3, '4': 1, '5': 9, '10': 'errorMessage'},
    {'1': 'signature', '3': 4, '4': 1, '5': 12, '10': 'signature'},
    {'1': 'signature_json', '3': 5, '4': 1, '5': 9, '10': 'signatureJson'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhgKB2VuY29kZWQYASABKAxSB2VuY29kZWQSMwoFZXJyb3IYAiABKA'
    '4yHS5UVy5Db21tb24uUHJvdG8uU2lnbmluZ0Vycm9yUgVlcnJvchIjCg1lcnJvcl9tZXNzYWdl'
    'GAMgASgJUgxlcnJvck1lc3NhZ2USHAoJc2lnbmF0dXJlGAQgASgMUglzaWduYXR1cmUSJQoOc2'
    'lnbmF0dXJlX2pzb24YBSABKAlSDXNpZ25hdHVyZUpzb24=');

