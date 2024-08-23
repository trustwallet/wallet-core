//
//  Generated code. Do not modify.
//  source: Harmony.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'chain_id', '3': 1, '4': 1, '5': 12, '10': 'chainId'},
    {'1': 'private_key', '3': 2, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'transaction_message', '3': 3, '4': 1, '5': 11, '6': '.TW.Harmony.Proto.TransactionMessage', '9': 0, '10': 'transactionMessage'},
    {'1': 'staking_message', '3': 4, '4': 1, '5': 11, '6': '.TW.Harmony.Proto.StakingMessage', '9': 0, '10': 'stakingMessage'},
  ],
  '8': [
    {'1': 'message_oneof'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSGQoIY2hhaW5faWQYASABKAxSB2NoYWluSWQSHwoLcHJpdmF0ZV9rZX'
    'kYAiABKAxSCnByaXZhdGVLZXkSVwoTdHJhbnNhY3Rpb25fbWVzc2FnZRgDIAEoCzIkLlRXLkhh'
    'cm1vbnkuUHJvdG8uVHJhbnNhY3Rpb25NZXNzYWdlSABSEnRyYW5zYWN0aW9uTWVzc2FnZRJLCg'
    '9zdGFraW5nX21lc3NhZ2UYBCABKAsyIC5UVy5IYXJtb255LlByb3RvLlN0YWtpbmdNZXNzYWdl'
    'SABSDnN0YWtpbmdNZXNzYWdlQg8KDW1lc3NhZ2Vfb25lb2Y=');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'v', '3': 2, '4': 1, '5': 12, '10': 'v'},
    {'1': 'r', '3': 3, '4': 1, '5': 12, '10': 'r'},
    {'1': 's', '3': 4, '4': 1, '5': 12, '10': 's'},
    {'1': 'error', '3': 5, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 6, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhgKB2VuY29kZWQYASABKAxSB2VuY29kZWQSDAoBdhgCIAEoDFIBdh'
    'IMCgFyGAMgASgMUgFyEgwKAXMYBCABKAxSAXMSMwoFZXJyb3IYBSABKA4yHS5UVy5Db21tb24u'
    'UHJvdG8uU2lnbmluZ0Vycm9yUgVlcnJvchIjCg1lcnJvcl9tZXNzYWdlGAYgASgJUgxlcnJvck'
    '1lc3NhZ2U=');

@$core.Deprecated('Use transactionMessageDescriptor instead')
const TransactionMessage$json = {
  '1': 'TransactionMessage',
  '2': [
    {'1': 'nonce', '3': 1, '4': 1, '5': 12, '10': 'nonce'},
    {'1': 'gas_price', '3': 2, '4': 1, '5': 12, '10': 'gasPrice'},
    {'1': 'gas_limit', '3': 3, '4': 1, '5': 12, '10': 'gasLimit'},
    {'1': 'to_address', '3': 4, '4': 1, '5': 9, '10': 'toAddress'},
    {'1': 'amount', '3': 5, '4': 1, '5': 12, '10': 'amount'},
    {'1': 'payload', '3': 6, '4': 1, '5': 12, '10': 'payload'},
    {'1': 'from_shard_id', '3': 7, '4': 1, '5': 12, '10': 'fromShardId'},
    {'1': 'to_shard_id', '3': 8, '4': 1, '5': 12, '10': 'toShardId'},
  ],
};

/// Descriptor for `TransactionMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionMessageDescriptor = $convert.base64Decode(
    'ChJUcmFuc2FjdGlvbk1lc3NhZ2USFAoFbm9uY2UYASABKAxSBW5vbmNlEhsKCWdhc19wcmljZR'
    'gCIAEoDFIIZ2FzUHJpY2USGwoJZ2FzX2xpbWl0GAMgASgMUghnYXNMaW1pdBIdCgp0b19hZGRy'
    'ZXNzGAQgASgJUgl0b0FkZHJlc3MSFgoGYW1vdW50GAUgASgMUgZhbW91bnQSGAoHcGF5bG9hZB'
    'gGIAEoDFIHcGF5bG9hZBIiCg1mcm9tX3NoYXJkX2lkGAcgASgMUgtmcm9tU2hhcmRJZBIeCgt0'
    'b19zaGFyZF9pZBgIIAEoDFIJdG9TaGFyZElk');

@$core.Deprecated('Use stakingMessageDescriptor instead')
const StakingMessage$json = {
  '1': 'StakingMessage',
  '2': [
    {'1': 'create_validator_message', '3': 1, '4': 1, '5': 11, '6': '.TW.Harmony.Proto.DirectiveCreateValidator', '9': 0, '10': 'createValidatorMessage'},
    {'1': 'edit_validator_message', '3': 2, '4': 1, '5': 11, '6': '.TW.Harmony.Proto.DirectiveEditValidator', '9': 0, '10': 'editValidatorMessage'},
    {'1': 'delegate_message', '3': 3, '4': 1, '5': 11, '6': '.TW.Harmony.Proto.DirectiveDelegate', '9': 0, '10': 'delegateMessage'},
    {'1': 'undelegate_message', '3': 4, '4': 1, '5': 11, '6': '.TW.Harmony.Proto.DirectiveUndelegate', '9': 0, '10': 'undelegateMessage'},
    {'1': 'collect_rewards', '3': 5, '4': 1, '5': 11, '6': '.TW.Harmony.Proto.DirectiveCollectRewards', '9': 0, '10': 'collectRewards'},
    {'1': 'nonce', '3': 6, '4': 1, '5': 12, '10': 'nonce'},
    {'1': 'gas_price', '3': 7, '4': 1, '5': 12, '10': 'gasPrice'},
    {'1': 'gas_limit', '3': 8, '4': 1, '5': 12, '10': 'gasLimit'},
  ],
  '8': [
    {'1': 'stake_msg'},
  ],
};

/// Descriptor for `StakingMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List stakingMessageDescriptor = $convert.base64Decode(
    'Cg5TdGFraW5nTWVzc2FnZRJmChhjcmVhdGVfdmFsaWRhdG9yX21lc3NhZ2UYASABKAsyKi5UVy'
    '5IYXJtb255LlByb3RvLkRpcmVjdGl2ZUNyZWF0ZVZhbGlkYXRvckgAUhZjcmVhdGVWYWxpZGF0'
    'b3JNZXNzYWdlEmAKFmVkaXRfdmFsaWRhdG9yX21lc3NhZ2UYAiABKAsyKC5UVy5IYXJtb255Ll'
    'Byb3RvLkRpcmVjdGl2ZUVkaXRWYWxpZGF0b3JIAFIUZWRpdFZhbGlkYXRvck1lc3NhZ2USUAoQ'
    'ZGVsZWdhdGVfbWVzc2FnZRgDIAEoCzIjLlRXLkhhcm1vbnkuUHJvdG8uRGlyZWN0aXZlRGVsZW'
    'dhdGVIAFIPZGVsZWdhdGVNZXNzYWdlElYKEnVuZGVsZWdhdGVfbWVzc2FnZRgEIAEoCzIlLlRX'
    'Lkhhcm1vbnkuUHJvdG8uRGlyZWN0aXZlVW5kZWxlZ2F0ZUgAUhF1bmRlbGVnYXRlTWVzc2FnZR'
    'JUCg9jb2xsZWN0X3Jld2FyZHMYBSABKAsyKS5UVy5IYXJtb255LlByb3RvLkRpcmVjdGl2ZUNv'
    'bGxlY3RSZXdhcmRzSABSDmNvbGxlY3RSZXdhcmRzEhQKBW5vbmNlGAYgASgMUgVub25jZRIbCg'
    'lnYXNfcHJpY2UYByABKAxSCGdhc1ByaWNlEhsKCWdhc19saW1pdBgIIAEoDFIIZ2FzTGltaXRC'
    'CwoJc3Rha2VfbXNn');

@$core.Deprecated('Use descriptionDescriptor instead')
const Description$json = {
  '1': 'Description',
  '2': [
    {'1': 'name', '3': 1, '4': 1, '5': 9, '10': 'name'},
    {'1': 'identity', '3': 2, '4': 1, '5': 9, '10': 'identity'},
    {'1': 'website', '3': 3, '4': 1, '5': 9, '10': 'website'},
    {'1': 'security_contact', '3': 4, '4': 1, '5': 9, '10': 'securityContact'},
    {'1': 'details', '3': 5, '4': 1, '5': 9, '10': 'details'},
  ],
};

/// Descriptor for `Description`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List descriptionDescriptor = $convert.base64Decode(
    'CgtEZXNjcmlwdGlvbhISCgRuYW1lGAEgASgJUgRuYW1lEhoKCGlkZW50aXR5GAIgASgJUghpZG'
    'VudGl0eRIYCgd3ZWJzaXRlGAMgASgJUgd3ZWJzaXRlEikKEHNlY3VyaXR5X2NvbnRhY3QYBCAB'
    'KAlSD3NlY3VyaXR5Q29udGFjdBIYCgdkZXRhaWxzGAUgASgJUgdkZXRhaWxz');

@$core.Deprecated('Use decimalDescriptor instead')
const Decimal$json = {
  '1': 'Decimal',
  '2': [
    {'1': 'value', '3': 1, '4': 1, '5': 12, '10': 'value'},
    {'1': 'precision', '3': 2, '4': 1, '5': 12, '10': 'precision'},
  ],
};

/// Descriptor for `Decimal`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List decimalDescriptor = $convert.base64Decode(
    'CgdEZWNpbWFsEhQKBXZhbHVlGAEgASgMUgV2YWx1ZRIcCglwcmVjaXNpb24YAiABKAxSCXByZW'
    'Npc2lvbg==');

@$core.Deprecated('Use commissionRateDescriptor instead')
const CommissionRate$json = {
  '1': 'CommissionRate',
  '2': [
    {'1': 'rate', '3': 1, '4': 1, '5': 11, '6': '.TW.Harmony.Proto.Decimal', '10': 'rate'},
    {'1': 'max_rate', '3': 2, '4': 1, '5': 11, '6': '.TW.Harmony.Proto.Decimal', '10': 'maxRate'},
    {'1': 'max_change_rate', '3': 3, '4': 1, '5': 11, '6': '.TW.Harmony.Proto.Decimal', '10': 'maxChangeRate'},
  ],
};

/// Descriptor for `CommissionRate`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List commissionRateDescriptor = $convert.base64Decode(
    'Cg5Db21taXNzaW9uUmF0ZRItCgRyYXRlGAEgASgLMhkuVFcuSGFybW9ueS5Qcm90by5EZWNpbW'
    'FsUgRyYXRlEjQKCG1heF9yYXRlGAIgASgLMhkuVFcuSGFybW9ueS5Qcm90by5EZWNpbWFsUgdt'
    'YXhSYXRlEkEKD21heF9jaGFuZ2VfcmF0ZRgDIAEoCzIZLlRXLkhhcm1vbnkuUHJvdG8uRGVjaW'
    '1hbFINbWF4Q2hhbmdlUmF0ZQ==');

@$core.Deprecated('Use directiveCreateValidatorDescriptor instead')
const DirectiveCreateValidator$json = {
  '1': 'DirectiveCreateValidator',
  '2': [
    {'1': 'validator_address', '3': 1, '4': 1, '5': 9, '10': 'validatorAddress'},
    {'1': 'description', '3': 2, '4': 1, '5': 11, '6': '.TW.Harmony.Proto.Description', '10': 'description'},
    {'1': 'commission_rates', '3': 3, '4': 1, '5': 11, '6': '.TW.Harmony.Proto.CommissionRate', '10': 'commissionRates'},
    {'1': 'min_self_delegation', '3': 4, '4': 1, '5': 12, '10': 'minSelfDelegation'},
    {'1': 'max_total_delegation', '3': 5, '4': 1, '5': 12, '10': 'maxTotalDelegation'},
    {'1': 'slot_pub_keys', '3': 6, '4': 3, '5': 12, '10': 'slotPubKeys'},
    {'1': 'slot_key_sigs', '3': 7, '4': 3, '5': 12, '10': 'slotKeySigs'},
    {'1': 'amount', '3': 8, '4': 1, '5': 12, '10': 'amount'},
  ],
};

/// Descriptor for `DirectiveCreateValidator`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List directiveCreateValidatorDescriptor = $convert.base64Decode(
    'ChhEaXJlY3RpdmVDcmVhdGVWYWxpZGF0b3ISKwoRdmFsaWRhdG9yX2FkZHJlc3MYASABKAlSEH'
    'ZhbGlkYXRvckFkZHJlc3MSPwoLZGVzY3JpcHRpb24YAiABKAsyHS5UVy5IYXJtb255LlByb3Rv'
    'LkRlc2NyaXB0aW9uUgtkZXNjcmlwdGlvbhJLChBjb21taXNzaW9uX3JhdGVzGAMgASgLMiAuVF'
    'cuSGFybW9ueS5Qcm90by5Db21taXNzaW9uUmF0ZVIPY29tbWlzc2lvblJhdGVzEi4KE21pbl9z'
    'ZWxmX2RlbGVnYXRpb24YBCABKAxSEW1pblNlbGZEZWxlZ2F0aW9uEjAKFG1heF90b3RhbF9kZW'
    'xlZ2F0aW9uGAUgASgMUhJtYXhUb3RhbERlbGVnYXRpb24SIgoNc2xvdF9wdWJfa2V5cxgGIAMo'
    'DFILc2xvdFB1YktleXMSIgoNc2xvdF9rZXlfc2lncxgHIAMoDFILc2xvdEtleVNpZ3MSFgoGYW'
    '1vdW50GAggASgMUgZhbW91bnQ=');

@$core.Deprecated('Use directiveEditValidatorDescriptor instead')
const DirectiveEditValidator$json = {
  '1': 'DirectiveEditValidator',
  '2': [
    {'1': 'validator_address', '3': 1, '4': 1, '5': 9, '10': 'validatorAddress'},
    {'1': 'description', '3': 2, '4': 1, '5': 11, '6': '.TW.Harmony.Proto.Description', '10': 'description'},
    {'1': 'commission_rate', '3': 3, '4': 1, '5': 11, '6': '.TW.Harmony.Proto.Decimal', '10': 'commissionRate'},
    {'1': 'min_self_delegation', '3': 4, '4': 1, '5': 12, '10': 'minSelfDelegation'},
    {'1': 'max_total_delegation', '3': 5, '4': 1, '5': 12, '10': 'maxTotalDelegation'},
    {'1': 'slot_key_to_remove', '3': 6, '4': 1, '5': 12, '10': 'slotKeyToRemove'},
    {'1': 'slot_key_to_add', '3': 7, '4': 1, '5': 12, '10': 'slotKeyToAdd'},
    {'1': 'slot_key_to_add_sig', '3': 8, '4': 1, '5': 12, '10': 'slotKeyToAddSig'},
    {'1': 'active', '3': 9, '4': 1, '5': 12, '10': 'active'},
  ],
};

/// Descriptor for `DirectiveEditValidator`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List directiveEditValidatorDescriptor = $convert.base64Decode(
    'ChZEaXJlY3RpdmVFZGl0VmFsaWRhdG9yEisKEXZhbGlkYXRvcl9hZGRyZXNzGAEgASgJUhB2YW'
    'xpZGF0b3JBZGRyZXNzEj8KC2Rlc2NyaXB0aW9uGAIgASgLMh0uVFcuSGFybW9ueS5Qcm90by5E'
    'ZXNjcmlwdGlvblILZGVzY3JpcHRpb24SQgoPY29tbWlzc2lvbl9yYXRlGAMgASgLMhkuVFcuSG'
    'FybW9ueS5Qcm90by5EZWNpbWFsUg5jb21taXNzaW9uUmF0ZRIuChNtaW5fc2VsZl9kZWxlZ2F0'
    'aW9uGAQgASgMUhFtaW5TZWxmRGVsZWdhdGlvbhIwChRtYXhfdG90YWxfZGVsZWdhdGlvbhgFIA'
    'EoDFISbWF4VG90YWxEZWxlZ2F0aW9uEisKEnNsb3Rfa2V5X3RvX3JlbW92ZRgGIAEoDFIPc2xv'
    'dEtleVRvUmVtb3ZlEiUKD3Nsb3Rfa2V5X3RvX2FkZBgHIAEoDFIMc2xvdEtleVRvQWRkEiwKE3'
    'Nsb3Rfa2V5X3RvX2FkZF9zaWcYCCABKAxSD3Nsb3RLZXlUb0FkZFNpZxIWCgZhY3RpdmUYCSAB'
    'KAxSBmFjdGl2ZQ==');

@$core.Deprecated('Use directiveDelegateDescriptor instead')
const DirectiveDelegate$json = {
  '1': 'DirectiveDelegate',
  '2': [
    {'1': 'delegator_address', '3': 1, '4': 1, '5': 9, '10': 'delegatorAddress'},
    {'1': 'validator_address', '3': 2, '4': 1, '5': 9, '10': 'validatorAddress'},
    {'1': 'amount', '3': 3, '4': 1, '5': 12, '10': 'amount'},
  ],
};

/// Descriptor for `DirectiveDelegate`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List directiveDelegateDescriptor = $convert.base64Decode(
    'ChFEaXJlY3RpdmVEZWxlZ2F0ZRIrChFkZWxlZ2F0b3JfYWRkcmVzcxgBIAEoCVIQZGVsZWdhdG'
    '9yQWRkcmVzcxIrChF2YWxpZGF0b3JfYWRkcmVzcxgCIAEoCVIQdmFsaWRhdG9yQWRkcmVzcxIW'
    'CgZhbW91bnQYAyABKAxSBmFtb3VudA==');

@$core.Deprecated('Use directiveUndelegateDescriptor instead')
const DirectiveUndelegate$json = {
  '1': 'DirectiveUndelegate',
  '2': [
    {'1': 'delegator_address', '3': 1, '4': 1, '5': 9, '10': 'delegatorAddress'},
    {'1': 'validator_address', '3': 2, '4': 1, '5': 9, '10': 'validatorAddress'},
    {'1': 'amount', '3': 3, '4': 1, '5': 12, '10': 'amount'},
  ],
};

/// Descriptor for `DirectiveUndelegate`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List directiveUndelegateDescriptor = $convert.base64Decode(
    'ChNEaXJlY3RpdmVVbmRlbGVnYXRlEisKEWRlbGVnYXRvcl9hZGRyZXNzGAEgASgJUhBkZWxlZ2'
    'F0b3JBZGRyZXNzEisKEXZhbGlkYXRvcl9hZGRyZXNzGAIgASgJUhB2YWxpZGF0b3JBZGRyZXNz'
    'EhYKBmFtb3VudBgDIAEoDFIGYW1vdW50');

@$core.Deprecated('Use directiveCollectRewardsDescriptor instead')
const DirectiveCollectRewards$json = {
  '1': 'DirectiveCollectRewards',
  '2': [
    {'1': 'delegator_address', '3': 1, '4': 1, '5': 9, '10': 'delegatorAddress'},
  ],
};

/// Descriptor for `DirectiveCollectRewards`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List directiveCollectRewardsDescriptor = $convert.base64Decode(
    'ChdEaXJlY3RpdmVDb2xsZWN0UmV3YXJkcxIrChFkZWxlZ2F0b3JfYWRkcmVzcxgBIAEoCVIQZG'
    'VsZWdhdG9yQWRkcmVzcw==');

