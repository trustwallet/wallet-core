//
//  Generated code. Do not modify.
//  source: Sui.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use objectRefDescriptor instead')
const ObjectRef$json = {
  '1': 'ObjectRef',
  '2': [
    {'1': 'object_id', '3': 1, '4': 1, '5': 9, '10': 'objectId'},
    {'1': 'version', '3': 2, '4': 1, '5': 4, '10': 'version'},
    {'1': 'object_digest', '3': 3, '4': 1, '5': 9, '10': 'objectDigest'},
  ],
};

/// Descriptor for `ObjectRef`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List objectRefDescriptor = $convert.base64Decode(
    'CglPYmplY3RSZWYSGwoJb2JqZWN0X2lkGAEgASgJUghvYmplY3RJZBIYCgd2ZXJzaW9uGAIgAS'
    'gEUgd2ZXJzaW9uEiMKDW9iamVjdF9kaWdlc3QYAyABKAlSDG9iamVjdERpZ2VzdA==');

@$core.Deprecated('Use amountDescriptor instead')
const Amount$json = {
  '1': 'Amount',
  '2': [
    {'1': 'amount', '3': 1, '4': 1, '5': 4, '10': 'amount'},
  ],
};

/// Descriptor for `Amount`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List amountDescriptor = $convert.base64Decode(
    'CgZBbW91bnQSFgoGYW1vdW50GAEgASgEUgZhbW91bnQ=');

@$core.Deprecated('Use signDirectDescriptor instead')
const SignDirect$json = {
  '1': 'SignDirect',
  '2': [
    {'1': 'unsigned_tx_msg', '3': 1, '4': 1, '5': 9, '10': 'unsignedTxMsg'},
  ],
};

/// Descriptor for `SignDirect`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signDirectDescriptor = $convert.base64Decode(
    'CgpTaWduRGlyZWN0EiYKD3Vuc2lnbmVkX3R4X21zZxgBIAEoCVINdW5zaWduZWRUeE1zZw==');

@$core.Deprecated('Use payDescriptor instead')
const Pay$json = {
  '1': 'Pay',
  '2': [
    {'1': 'input_coins', '3': 1, '4': 3, '5': 11, '6': '.TW.Sui.Proto.ObjectRef', '10': 'inputCoins'},
    {'1': 'recipients', '3': 2, '4': 3, '5': 9, '10': 'recipients'},
    {'1': 'amounts', '3': 3, '4': 3, '5': 4, '10': 'amounts'},
    {'1': 'gas', '3': 4, '4': 1, '5': 11, '6': '.TW.Sui.Proto.ObjectRef', '10': 'gas'},
  ],
};

/// Descriptor for `Pay`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List payDescriptor = $convert.base64Decode(
    'CgNQYXkSOAoLaW5wdXRfY29pbnMYASADKAsyFy5UVy5TdWkuUHJvdG8uT2JqZWN0UmVmUgppbn'
    'B1dENvaW5zEh4KCnJlY2lwaWVudHMYAiADKAlSCnJlY2lwaWVudHMSGAoHYW1vdW50cxgDIAMo'
    'BFIHYW1vdW50cxIpCgNnYXMYBCABKAsyFy5UVy5TdWkuUHJvdG8uT2JqZWN0UmVmUgNnYXM=');

@$core.Deprecated('Use paySuiDescriptor instead')
const PaySui$json = {
  '1': 'PaySui',
  '2': [
    {'1': 'input_coins', '3': 1, '4': 3, '5': 11, '6': '.TW.Sui.Proto.ObjectRef', '10': 'inputCoins'},
    {'1': 'recipients', '3': 2, '4': 3, '5': 9, '10': 'recipients'},
    {'1': 'amounts', '3': 3, '4': 3, '5': 4, '10': 'amounts'},
  ],
};

/// Descriptor for `PaySui`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List paySuiDescriptor = $convert.base64Decode(
    'CgZQYXlTdWkSOAoLaW5wdXRfY29pbnMYASADKAsyFy5UVy5TdWkuUHJvdG8uT2JqZWN0UmVmUg'
    'ppbnB1dENvaW5zEh4KCnJlY2lwaWVudHMYAiADKAlSCnJlY2lwaWVudHMSGAoHYW1vdW50cxgD'
    'IAMoBFIHYW1vdW50cw==');

@$core.Deprecated('Use payAllSuiDescriptor instead')
const PayAllSui$json = {
  '1': 'PayAllSui',
  '2': [
    {'1': 'input_coins', '3': 1, '4': 3, '5': 11, '6': '.TW.Sui.Proto.ObjectRef', '10': 'inputCoins'},
    {'1': 'recipient', '3': 2, '4': 1, '5': 9, '10': 'recipient'},
  ],
};

/// Descriptor for `PayAllSui`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List payAllSuiDescriptor = $convert.base64Decode(
    'CglQYXlBbGxTdWkSOAoLaW5wdXRfY29pbnMYASADKAsyFy5UVy5TdWkuUHJvdG8uT2JqZWN0Um'
    'VmUgppbnB1dENvaW5zEhwKCXJlY2lwaWVudBgCIAEoCVIJcmVjaXBpZW50');

@$core.Deprecated('Use requestAddStakeDescriptor instead')
const RequestAddStake$json = {
  '1': 'RequestAddStake',
  '2': [
    {'1': 'coins', '3': 1, '4': 3, '5': 11, '6': '.TW.Sui.Proto.ObjectRef', '10': 'coins'},
    {'1': 'amount', '3': 2, '4': 1, '5': 11, '6': '.TW.Sui.Proto.Amount', '10': 'amount'},
    {'1': 'validator', '3': 3, '4': 1, '5': 9, '10': 'validator'},
    {'1': 'gas', '3': 4, '4': 1, '5': 11, '6': '.TW.Sui.Proto.ObjectRef', '10': 'gas'},
  ],
};

/// Descriptor for `RequestAddStake`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List requestAddStakeDescriptor = $convert.base64Decode(
    'Cg9SZXF1ZXN0QWRkU3Rha2USLQoFY29pbnMYASADKAsyFy5UVy5TdWkuUHJvdG8uT2JqZWN0Um'
    'VmUgVjb2lucxIsCgZhbW91bnQYAiABKAsyFC5UVy5TdWkuUHJvdG8uQW1vdW50UgZhbW91bnQS'
    'HAoJdmFsaWRhdG9yGAMgASgJUgl2YWxpZGF0b3ISKQoDZ2FzGAQgASgLMhcuVFcuU3VpLlByb3'
    'RvLk9iamVjdFJlZlIDZ2Fz');

@$core.Deprecated('Use requestWithdrawStakeDescriptor instead')
const RequestWithdrawStake$json = {
  '1': 'RequestWithdrawStake',
  '2': [
    {'1': 'staked_sui', '3': 1, '4': 1, '5': 11, '6': '.TW.Sui.Proto.ObjectRef', '10': 'stakedSui'},
    {'1': 'gas', '3': 2, '4': 1, '5': 11, '6': '.TW.Sui.Proto.ObjectRef', '10': 'gas'},
  ],
};

/// Descriptor for `RequestWithdrawStake`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List requestWithdrawStakeDescriptor = $convert.base64Decode(
    'ChRSZXF1ZXN0V2l0aGRyYXdTdGFrZRI2CgpzdGFrZWRfc3VpGAEgASgLMhcuVFcuU3VpLlByb3'
    'RvLk9iamVjdFJlZlIJc3Rha2VkU3VpEikKA2dhcxgCIAEoCzIXLlRXLlN1aS5Qcm90by5PYmpl'
    'Y3RSZWZSA2dhcw==');

@$core.Deprecated('Use transferObjectDescriptor instead')
const TransferObject$json = {
  '1': 'TransferObject',
  '2': [
    {'1': 'object', '3': 1, '4': 1, '5': 11, '6': '.TW.Sui.Proto.ObjectRef', '10': 'object'},
    {'1': 'recipient', '3': 2, '4': 1, '5': 9, '10': 'recipient'},
    {'1': 'gas', '3': 3, '4': 1, '5': 11, '6': '.TW.Sui.Proto.ObjectRef', '10': 'gas'},
  ],
};

/// Descriptor for `TransferObject`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transferObjectDescriptor = $convert.base64Decode(
    'Cg5UcmFuc2Zlck9iamVjdBIvCgZvYmplY3QYASABKAsyFy5UVy5TdWkuUHJvdG8uT2JqZWN0Um'
    'VmUgZvYmplY3QSHAoJcmVjaXBpZW50GAIgASgJUglyZWNpcGllbnQSKQoDZ2FzGAMgASgLMhcu'
    'VFcuU3VpLlByb3RvLk9iamVjdFJlZlIDZ2Fz');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'private_key', '3': 1, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'signer', '3': 2, '4': 1, '5': 9, '10': 'signer'},
    {'1': 'sign_direct_message', '3': 3, '4': 1, '5': 11, '6': '.TW.Sui.Proto.SignDirect', '9': 0, '10': 'signDirectMessage'},
    {'1': 'pay', '3': 4, '4': 1, '5': 11, '6': '.TW.Sui.Proto.Pay', '9': 0, '10': 'pay'},
    {'1': 'pay_sui', '3': 5, '4': 1, '5': 11, '6': '.TW.Sui.Proto.PaySui', '9': 0, '10': 'paySui'},
    {'1': 'pay_all_sui', '3': 6, '4': 1, '5': 11, '6': '.TW.Sui.Proto.PayAllSui', '9': 0, '10': 'payAllSui'},
    {'1': 'request_add_stake', '3': 7, '4': 1, '5': 11, '6': '.TW.Sui.Proto.RequestAddStake', '9': 0, '10': 'requestAddStake'},
    {'1': 'request_withdraw_stake', '3': 8, '4': 1, '5': 11, '6': '.TW.Sui.Proto.RequestWithdrawStake', '9': 0, '10': 'requestWithdrawStake'},
    {'1': 'transfer_object', '3': 9, '4': 1, '5': 11, '6': '.TW.Sui.Proto.TransferObject', '9': 0, '10': 'transferObject'},
    {'1': 'gas_budget', '3': 12, '4': 1, '5': 4, '10': 'gasBudget'},
    {'1': 'reference_gas_price', '3': 13, '4': 1, '5': 4, '10': 'referenceGasPrice'},
  ],
  '8': [
    {'1': 'transaction_payload'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSHwoLcHJpdmF0ZV9rZXkYASABKAxSCnByaXZhdGVLZXkSFgoGc2lnbm'
    'VyGAIgASgJUgZzaWduZXISSgoTc2lnbl9kaXJlY3RfbWVzc2FnZRgDIAEoCzIYLlRXLlN1aS5Q'
    'cm90by5TaWduRGlyZWN0SABSEXNpZ25EaXJlY3RNZXNzYWdlEiUKA3BheRgEIAEoCzIRLlRXLl'
    'N1aS5Qcm90by5QYXlIAFIDcGF5Ei8KB3BheV9zdWkYBSABKAsyFC5UVy5TdWkuUHJvdG8uUGF5'
    'U3VpSABSBnBheVN1aRI5CgtwYXlfYWxsX3N1aRgGIAEoCzIXLlRXLlN1aS5Qcm90by5QYXlBbG'
    'xTdWlIAFIJcGF5QWxsU3VpEksKEXJlcXVlc3RfYWRkX3N0YWtlGAcgASgLMh0uVFcuU3VpLlBy'
    'b3RvLlJlcXVlc3RBZGRTdGFrZUgAUg9yZXF1ZXN0QWRkU3Rha2USWgoWcmVxdWVzdF93aXRoZH'
    'Jhd19zdGFrZRgIIAEoCzIiLlRXLlN1aS5Qcm90by5SZXF1ZXN0V2l0aGRyYXdTdGFrZUgAUhRy'
    'ZXF1ZXN0V2l0aGRyYXdTdGFrZRJHCg90cmFuc2Zlcl9vYmplY3QYCSABKAsyHC5UVy5TdWkuUH'
    'JvdG8uVHJhbnNmZXJPYmplY3RIAFIOdHJhbnNmZXJPYmplY3QSHQoKZ2FzX2J1ZGdldBgMIAEo'
    'BFIJZ2FzQnVkZ2V0Ei4KE3JlZmVyZW5jZV9nYXNfcHJpY2UYDSABKARSEXJlZmVyZW5jZUdhc1'
    'ByaWNlQhUKE3RyYW5zYWN0aW9uX3BheWxvYWQ=');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'unsigned_tx', '3': 1, '4': 1, '5': 9, '10': 'unsignedTx'},
    {'1': 'signature', '3': 2, '4': 1, '5': 9, '10': 'signature'},
    {'1': 'error', '3': 3, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 4, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0Eh8KC3Vuc2lnbmVkX3R4GAEgASgJUgp1bnNpZ25lZFR4EhwKCXNpZ2'
    '5hdHVyZRgCIAEoCVIJc2lnbmF0dXJlEjMKBWVycm9yGAMgASgOMh0uVFcuQ29tbW9uLlByb3Rv'
    'LlNpZ25pbmdFcnJvclIFZXJyb3ISIwoNZXJyb3JfbWVzc2FnZRgEIAEoCVIMZXJyb3JNZXNzYW'
    'dl');

