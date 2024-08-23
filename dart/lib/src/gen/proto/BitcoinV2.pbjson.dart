//
//  Generated code. Do not modify.
//  source: BitcoinV2.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use inputSelectorDescriptor instead')
const InputSelector$json = {
  '1': 'InputSelector',
  '2': [
    {'1': 'SelectAscending', '2': 0},
    {'1': 'SelectInOrder', '2': 1},
    {'1': 'SelectDescending', '2': 2},
    {'1': 'UseAll', '2': 10},
  ],
};

/// Descriptor for `InputSelector`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List inputSelectorDescriptor = $convert.base64Decode(
    'Cg1JbnB1dFNlbGVjdG9yEhMKD1NlbGVjdEFzY2VuZGluZxAAEhEKDVNlbGVjdEluT3JkZXIQAR'
    'IUChBTZWxlY3REZXNjZW5kaW5nEAISCgoGVXNlQWxsEAo=');

@$core.Deprecated('Use transactionVersionDescriptor instead')
const TransactionVersion$json = {
  '1': 'TransactionVersion',
  '2': [
    {'1': 'UseDefault', '2': 0},
    {'1': 'V1', '2': 1},
    {'1': 'V2', '2': 2},
  ],
};

/// Descriptor for `TransactionVersion`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List transactionVersionDescriptor = $convert.base64Decode(
    'ChJUcmFuc2FjdGlvblZlcnNpb24SDgoKVXNlRGVmYXVsdBAAEgYKAlYxEAESBgoCVjIQAg==');

@$core.Deprecated('Use publicKeyOrHashDescriptor instead')
const PublicKeyOrHash$json = {
  '1': 'PublicKeyOrHash',
  '2': [
    {'1': 'pubkey', '3': 1, '4': 1, '5': 12, '9': 0, '10': 'pubkey'},
    {'1': 'hash', '3': 2, '4': 1, '5': 12, '9': 0, '10': 'hash'},
  ],
  '8': [
    {'1': 'variant'},
  ],
};

/// Descriptor for `PublicKeyOrHash`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List publicKeyOrHashDescriptor = $convert.base64Decode(
    'Cg9QdWJsaWNLZXlPckhhc2gSGAoGcHVia2V5GAEgASgMSABSBnB1YmtleRIUCgRoYXNoGAIgAS'
    'gMSABSBGhhc2hCCQoHdmFyaWFudA==');

@$core.Deprecated('Use outPointDescriptor instead')
const OutPoint$json = {
  '1': 'OutPoint',
  '2': [
    {'1': 'hash', '3': 1, '4': 1, '5': 12, '10': 'hash'},
    {'1': 'vout', '3': 2, '4': 1, '5': 13, '10': 'vout'},
  ],
};

/// Descriptor for `OutPoint`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List outPointDescriptor = $convert.base64Decode(
    'CghPdXRQb2ludBISCgRoYXNoGAEgASgMUgRoYXNoEhIKBHZvdXQYAiABKA1SBHZvdXQ=');

@$core.Deprecated('Use inputDescriptor instead')
const Input$json = {
  '1': 'Input',
  '2': [
    {'1': 'out_point', '3': 1, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.OutPoint', '10': 'outPoint'},
    {'1': 'value', '3': 2, '4': 1, '5': 3, '10': 'value'},
    {'1': 'sighash_type', '3': 3, '4': 1, '5': 13, '10': 'sighashType'},
    {'1': 'sequence', '3': 4, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.Input.Sequence', '10': 'sequence'},
    {'1': 'script_builder', '3': 5, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.Input.InputBuilder', '9': 0, '10': 'scriptBuilder'},
    {'1': 'script_data', '3': 6, '4': 1, '5': 12, '9': 0, '10': 'scriptData'},
    {'1': 'receiver_address', '3': 7, '4': 1, '5': 9, '9': 0, '10': 'receiverAddress'},
  ],
  '3': [Input_Sequence$json, Input_InputBuilder$json, Input_InputTaprootScriptPath$json, Input_InputBrc20Inscription$json],
  '8': [
    {'1': 'claiming_script'},
  ],
};

@$core.Deprecated('Use inputDescriptor instead')
const Input_Sequence$json = {
  '1': 'Sequence',
  '2': [
    {'1': 'sequence', '3': 1, '4': 1, '5': 13, '10': 'sequence'},
  ],
};

@$core.Deprecated('Use inputDescriptor instead')
const Input_InputBuilder$json = {
  '1': 'InputBuilder',
  '2': [
    {'1': 'p2pk', '3': 2, '4': 1, '5': 12, '9': 0, '10': 'p2pk'},
    {'1': 'p2pkh', '3': 3, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.PublicKeyOrHash', '9': 0, '10': 'p2pkh'},
    {'1': 'p2wpkh', '3': 5, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.PublicKeyOrHash', '9': 0, '10': 'p2wpkh'},
    {'1': 'p2tr_key_path', '3': 7, '4': 1, '5': 12, '9': 0, '10': 'p2trKeyPath'},
    {'1': 'brc20_inscribe', '3': 9, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.Input.InputBrc20Inscription', '9': 0, '10': 'brc20Inscribe'},
  ],
  '8': [
    {'1': 'variant'},
  ],
};

@$core.Deprecated('Use inputDescriptor instead')
const Input_InputTaprootScriptPath$json = {
  '1': 'InputTaprootScriptPath',
  '2': [
    {'1': 'payload', '3': 2, '4': 1, '5': 12, '10': 'payload'},
    {'1': 'control_block', '3': 3, '4': 1, '5': 12, '10': 'controlBlock'},
  ],
};

@$core.Deprecated('Use inputDescriptor instead')
const Input_InputBrc20Inscription$json = {
  '1': 'InputBrc20Inscription',
  '2': [
    {'1': 'inscribe_to', '3': 2, '4': 1, '5': 12, '10': 'inscribeTo'},
    {'1': 'ticker', '3': 3, '4': 1, '5': 9, '10': 'ticker'},
    {'1': 'transfer_amount', '3': 4, '4': 1, '5': 9, '10': 'transferAmount'},
  ],
};

/// Descriptor for `Input`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List inputDescriptor = $convert.base64Decode(
    'CgVJbnB1dBI5CglvdXRfcG9pbnQYASABKAsyHC5UVy5CaXRjb2luVjIuUHJvdG8uT3V0UG9pbn'
    'RSCG91dFBvaW50EhQKBXZhbHVlGAIgASgDUgV2YWx1ZRIhCgxzaWdoYXNoX3R5cGUYAyABKA1S'
    'C3NpZ2hhc2hUeXBlEj4KCHNlcXVlbmNlGAQgASgLMiIuVFcuQml0Y29pblYyLlByb3RvLklucH'
    'V0LlNlcXVlbmNlUghzZXF1ZW5jZRJPCg5zY3JpcHRfYnVpbGRlchgFIAEoCzImLlRXLkJpdGNv'
    'aW5WMi5Qcm90by5JbnB1dC5JbnB1dEJ1aWxkZXJIAFINc2NyaXB0QnVpbGRlchIhCgtzY3JpcH'
    'RfZGF0YRgGIAEoDEgAUgpzY3JpcHREYXRhEisKEHJlY2VpdmVyX2FkZHJlc3MYByABKAlIAFIP'
    'cmVjZWl2ZXJBZGRyZXNzGiYKCFNlcXVlbmNlEhoKCHNlcXVlbmNlGAEgASgNUghzZXF1ZW5jZR'
    'qrAgoMSW5wdXRCdWlsZGVyEhQKBHAycGsYAiABKAxIAFIEcDJwaxI7CgVwMnBraBgDIAEoCzIj'
    'LlRXLkJpdGNvaW5WMi5Qcm90by5QdWJsaWNLZXlPckhhc2hIAFIFcDJwa2gSPQoGcDJ3cGtoGA'
    'UgASgLMiMuVFcuQml0Y29pblYyLlByb3RvLlB1YmxpY0tleU9ySGFzaEgAUgZwMndwa2gSJAoN'
    'cDJ0cl9rZXlfcGF0aBgHIAEoDEgAUgtwMnRyS2V5UGF0aBJYCg5icmMyMF9pbnNjcmliZRgJIA'
    'EoCzIvLlRXLkJpdGNvaW5WMi5Qcm90by5JbnB1dC5JbnB1dEJyYzIwSW5zY3JpcHRpb25IAFIN'
    'YnJjMjBJbnNjcmliZUIJCgd2YXJpYW50GlcKFklucHV0VGFwcm9vdFNjcmlwdFBhdGgSGAoHcG'
    'F5bG9hZBgCIAEoDFIHcGF5bG9hZBIjCg1jb250cm9sX2Jsb2NrGAMgASgMUgxjb250cm9sQmxv'
    'Y2saeQoVSW5wdXRCcmMyMEluc2NyaXB0aW9uEh8KC2luc2NyaWJlX3RvGAIgASgMUgppbnNjcm'
    'liZVRvEhYKBnRpY2tlchgDIAEoCVIGdGlja2VyEicKD3RyYW5zZmVyX2Ftb3VudBgEIAEoCVIO'
    'dHJhbnNmZXJBbW91bnRCEQoPY2xhaW1pbmdfc2NyaXB0');

@$core.Deprecated('Use outputDescriptor instead')
const Output$json = {
  '1': 'Output',
  '2': [
    {'1': 'value', '3': 1, '4': 1, '5': 3, '10': 'value'},
    {'1': 'builder', '3': 2, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.Output.OutputBuilder', '9': 0, '10': 'builder'},
    {'1': 'custom_script_pubkey', '3': 3, '4': 1, '5': 12, '9': 0, '10': 'customScriptPubkey'},
    {'1': 'to_address', '3': 4, '4': 1, '5': 9, '9': 0, '10': 'toAddress'},
  ],
  '3': [Output_OutputBuilder$json, Output_RedeemScriptOrHash$json, Output_OutputTaprootScriptPath$json, Output_OutputBrc20Inscription$json],
  '8': [
    {'1': 'to_recipient'},
  ],
};

@$core.Deprecated('Use outputDescriptor instead')
const Output_OutputBuilder$json = {
  '1': 'OutputBuilder',
  '2': [
    {'1': 'p2sh', '3': 1, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.Output.RedeemScriptOrHash', '9': 0, '10': 'p2sh'},
    {'1': 'p2pk', '3': 2, '4': 1, '5': 12, '9': 0, '10': 'p2pk'},
    {'1': 'p2pkh', '3': 3, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.PublicKeyOrHash', '9': 0, '10': 'p2pkh'},
    {'1': 'p2wsh', '3': 4, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.Output.RedeemScriptOrHash', '9': 0, '10': 'p2wsh'},
    {'1': 'p2wpkh', '3': 5, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.PublicKeyOrHash', '9': 0, '10': 'p2wpkh'},
    {'1': 'p2tr_key_path', '3': 6, '4': 1, '5': 12, '9': 0, '10': 'p2trKeyPath'},
    {'1': 'p2tr_script_path', '3': 7, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.Output.OutputTaprootScriptPath', '9': 0, '10': 'p2trScriptPath'},
    {'1': 'p2tr_dangerous_assume_tweaked', '3': 8, '4': 1, '5': 12, '9': 0, '10': 'p2trDangerousAssumeTweaked'},
    {'1': 'brc20_inscribe', '3': 9, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.Output.OutputBrc20Inscription', '9': 0, '10': 'brc20Inscribe'},
    {'1': 'op_return', '3': 12, '4': 1, '5': 12, '9': 0, '10': 'opReturn'},
  ],
  '8': [
    {'1': 'variant'},
  ],
};

@$core.Deprecated('Use outputDescriptor instead')
const Output_RedeemScriptOrHash$json = {
  '1': 'RedeemScriptOrHash',
  '2': [
    {'1': 'redeem_script', '3': 1, '4': 1, '5': 12, '9': 0, '10': 'redeemScript'},
    {'1': 'hash', '3': 2, '4': 1, '5': 12, '9': 0, '10': 'hash'},
  ],
  '8': [
    {'1': 'variant'},
  ],
};

@$core.Deprecated('Use outputDescriptor instead')
const Output_OutputTaprootScriptPath$json = {
  '1': 'OutputTaprootScriptPath',
  '2': [
    {'1': 'internal_key', '3': 1, '4': 1, '5': 12, '10': 'internalKey'},
    {'1': 'merkle_root', '3': 2, '4': 1, '5': 12, '10': 'merkleRoot'},
  ],
};

@$core.Deprecated('Use outputDescriptor instead')
const Output_OutputBrc20Inscription$json = {
  '1': 'OutputBrc20Inscription',
  '2': [
    {'1': 'inscribe_to', '3': 1, '4': 1, '5': 12, '10': 'inscribeTo'},
    {'1': 'ticker', '3': 2, '4': 1, '5': 9, '10': 'ticker'},
    {'1': 'transfer_amount', '3': 3, '4': 1, '5': 9, '10': 'transferAmount'},
  ],
};

/// Descriptor for `Output`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List outputDescriptor = $convert.base64Decode(
    'CgZPdXRwdXQSFAoFdmFsdWUYASABKANSBXZhbHVlEkQKB2J1aWxkZXIYAiABKAsyKC5UVy5CaX'
    'Rjb2luVjIuUHJvdG8uT3V0cHV0Lk91dHB1dEJ1aWxkZXJIAFIHYnVpbGRlchIyChRjdXN0b21f'
    'c2NyaXB0X3B1YmtleRgDIAEoDEgAUhJjdXN0b21TY3JpcHRQdWJrZXkSHwoKdG9fYWRkcmVzcx'
    'gEIAEoCUgAUgl0b0FkZHJlc3Ma/gQKDU91dHB1dEJ1aWxkZXISQwoEcDJzaBgBIAEoCzItLlRX'
    'LkJpdGNvaW5WMi5Qcm90by5PdXRwdXQuUmVkZWVtU2NyaXB0T3JIYXNoSABSBHAyc2gSFAoEcD'
    'JwaxgCIAEoDEgAUgRwMnBrEjsKBXAycGtoGAMgASgLMiMuVFcuQml0Y29pblYyLlByb3RvLlB1'
    'YmxpY0tleU9ySGFzaEgAUgVwMnBraBJFCgVwMndzaBgEIAEoCzItLlRXLkJpdGNvaW5WMi5Qcm'
    '90by5PdXRwdXQuUmVkZWVtU2NyaXB0T3JIYXNoSABSBXAyd3NoEj0KBnAyd3BraBgFIAEoCzIj'
    'LlRXLkJpdGNvaW5WMi5Qcm90by5QdWJsaWNLZXlPckhhc2hIAFIGcDJ3cGtoEiQKDXAydHJfa2'
    'V5X3BhdGgYBiABKAxIAFILcDJ0cktleVBhdGgSXgoQcDJ0cl9zY3JpcHRfcGF0aBgHIAEoCzIy'
    'LlRXLkJpdGNvaW5WMi5Qcm90by5PdXRwdXQuT3V0cHV0VGFwcm9vdFNjcmlwdFBhdGhIAFIOcD'
    'J0clNjcmlwdFBhdGgSQwodcDJ0cl9kYW5nZXJvdXNfYXNzdW1lX3R3ZWFrZWQYCCABKAxIAFIa'
    'cDJ0ckRhbmdlcm91c0Fzc3VtZVR3ZWFrZWQSWgoOYnJjMjBfaW5zY3JpYmUYCSABKAsyMS5UVy'
    '5CaXRjb2luVjIuUHJvdG8uT3V0cHV0Lk91dHB1dEJyYzIwSW5zY3JpcHRpb25IAFINYnJjMjBJ'
    'bnNjcmliZRIdCglvcF9yZXR1cm4YDCABKAxIAFIIb3BSZXR1cm5CCQoHdmFyaWFudBpcChJSZW'
    'RlZW1TY3JpcHRPckhhc2gSJQoNcmVkZWVtX3NjcmlwdBgBIAEoDEgAUgxyZWRlZW1TY3JpcHQS'
    'FAoEaGFzaBgCIAEoDEgAUgRoYXNoQgkKB3ZhcmlhbnQaXQoXT3V0cHV0VGFwcm9vdFNjcmlwdF'
    'BhdGgSIQoMaW50ZXJuYWxfa2V5GAEgASgMUgtpbnRlcm5hbEtleRIfCgttZXJrbGVfcm9vdBgC'
    'IAEoDFIKbWVya2xlUm9vdBp6ChZPdXRwdXRCcmMyMEluc2NyaXB0aW9uEh8KC2luc2NyaWJlX3'
    'RvGAEgASgMUgppbnNjcmliZVRvEhYKBnRpY2tlchgCIAEoCVIGdGlja2VyEicKD3RyYW5zZmVy'
    'X2Ftb3VudBgDIAEoCVIOdHJhbnNmZXJBbW91bnRCDgoMdG9fcmVjaXBpZW50');

@$core.Deprecated('Use chainInfoDescriptor instead')
const ChainInfo$json = {
  '1': 'ChainInfo',
  '2': [
    {'1': 'p2pkh_prefix', '3': 1, '4': 1, '5': 13, '10': 'p2pkhPrefix'},
    {'1': 'p2sh_prefix', '3': 2, '4': 1, '5': 13, '10': 'p2shPrefix'},
  ],
};

/// Descriptor for `ChainInfo`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List chainInfoDescriptor = $convert.base64Decode(
    'CglDaGFpbkluZm8SIQoMcDJwa2hfcHJlZml4GAEgASgNUgtwMnBraFByZWZpeBIfCgtwMnNoX3'
    'ByZWZpeBgCIAEoDVIKcDJzaFByZWZpeA==');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'version', '3': 1, '4': 1, '5': 14, '6': '.TW.BitcoinV2.Proto.TransactionVersion', '10': 'version'},
    {'1': 'private_keys', '3': 2, '4': 3, '5': 12, '10': 'privateKeys'},
    {'1': 'public_keys', '3': 3, '4': 3, '5': 12, '10': 'publicKeys'},
    {'1': 'lock_time', '3': 4, '4': 1, '5': 13, '10': 'lockTime'},
    {'1': 'inputs', '3': 5, '4': 3, '5': 11, '6': '.TW.BitcoinV2.Proto.Input', '10': 'inputs'},
    {'1': 'outputs', '3': 6, '4': 3, '5': 11, '6': '.TW.BitcoinV2.Proto.Output', '10': 'outputs'},
    {'1': 'input_selector', '3': 7, '4': 1, '5': 14, '6': '.TW.BitcoinV2.Proto.InputSelector', '10': 'inputSelector'},
    {'1': 'fee_per_vb', '3': 8, '4': 1, '5': 3, '10': 'feePerVb'},
    {'1': 'change_output', '3': 9, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.Output', '10': 'changeOutput'},
    {'1': 'max_amount_output', '3': 10, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.Output', '10': 'maxAmountOutput'},
    {'1': 'chain_info', '3': 13, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.ChainInfo', '10': 'chainInfo'},
    {'1': 'fixed_dust_threshold', '3': 14, '4': 1, '5': 3, '9': 0, '10': 'fixedDustThreshold'},
    {'1': 'dangerous_use_fixed_schnorr_rng', '3': 20, '4': 1, '5': 8, '10': 'dangerousUseFixedSchnorrRng'},
  ],
  '8': [
    {'1': 'dust_policy'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSQAoHdmVyc2lvbhgBIAEoDjImLlRXLkJpdGNvaW5WMi5Qcm90by5Ucm'
    'Fuc2FjdGlvblZlcnNpb25SB3ZlcnNpb24SIQoMcHJpdmF0ZV9rZXlzGAIgAygMUgtwcml2YXRl'
    'S2V5cxIfCgtwdWJsaWNfa2V5cxgDIAMoDFIKcHVibGljS2V5cxIbCglsb2NrX3RpbWUYBCABKA'
    '1SCGxvY2tUaW1lEjEKBmlucHV0cxgFIAMoCzIZLlRXLkJpdGNvaW5WMi5Qcm90by5JbnB1dFIG'
    'aW5wdXRzEjQKB291dHB1dHMYBiADKAsyGi5UVy5CaXRjb2luVjIuUHJvdG8uT3V0cHV0UgdvdX'
    'RwdXRzEkgKDmlucHV0X3NlbGVjdG9yGAcgASgOMiEuVFcuQml0Y29pblYyLlByb3RvLklucHV0'
    'U2VsZWN0b3JSDWlucHV0U2VsZWN0b3ISHAoKZmVlX3Blcl92YhgIIAEoA1IIZmVlUGVyVmISPw'
    'oNY2hhbmdlX291dHB1dBgJIAEoCzIaLlRXLkJpdGNvaW5WMi5Qcm90by5PdXRwdXRSDGNoYW5n'
    'ZU91dHB1dBJGChFtYXhfYW1vdW50X291dHB1dBgKIAEoCzIaLlRXLkJpdGNvaW5WMi5Qcm90by'
    '5PdXRwdXRSD21heEFtb3VudE91dHB1dBI8CgpjaGFpbl9pbmZvGA0gASgLMh0uVFcuQml0Y29p'
    'blYyLlByb3RvLkNoYWluSW5mb1IJY2hhaW5JbmZvEjIKFGZpeGVkX2R1c3RfdGhyZXNob2xkGA'
    '4gASgDSABSEmZpeGVkRHVzdFRocmVzaG9sZBJECh9kYW5nZXJvdXNfdXNlX2ZpeGVkX3NjaG5v'
    'cnJfcm5nGBQgASgIUhtkYW5nZXJvdXNVc2VGaXhlZFNjaG5vcnJSbmdCDQoLZHVzdF9wb2xpY3'
    'k=');

@$core.Deprecated('Use transactionDescriptor instead')
const Transaction$json = {
  '1': 'Transaction',
  '2': [
    {'1': 'version', '3': 1, '4': 1, '5': 5, '10': 'version'},
    {'1': 'lock_time', '3': 2, '4': 1, '5': 13, '10': 'lockTime'},
    {'1': 'inputs', '3': 3, '4': 3, '5': 11, '6': '.TW.BitcoinV2.Proto.Transaction.TransactionInput', '10': 'inputs'},
    {'1': 'outputs', '3': 4, '4': 3, '5': 11, '6': '.TW.BitcoinV2.Proto.Transaction.TransactionOutput', '10': 'outputs'},
  ],
  '3': [Transaction_TransactionInput$json, Transaction_TransactionOutput$json],
};

@$core.Deprecated('Use transactionDescriptor instead')
const Transaction_TransactionInput$json = {
  '1': 'TransactionInput',
  '2': [
    {'1': 'out_point', '3': 1, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.OutPoint', '10': 'outPoint'},
    {'1': 'sequence', '3': 2, '4': 1, '5': 13, '10': 'sequence'},
    {'1': 'script_sig', '3': 3, '4': 1, '5': 12, '10': 'scriptSig'},
    {'1': 'witness_items', '3': 4, '4': 3, '5': 12, '10': 'witnessItems'},
  ],
};

@$core.Deprecated('Use transactionDescriptor instead')
const Transaction_TransactionOutput$json = {
  '1': 'TransactionOutput',
  '2': [
    {'1': 'script_pubkey', '3': 1, '4': 1, '5': 12, '10': 'scriptPubkey'},
    {'1': 'value', '3': 2, '4': 1, '5': 3, '10': 'value'},
  ],
};

/// Descriptor for `Transaction`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionDescriptor = $convert.base64Decode(
    'CgtUcmFuc2FjdGlvbhIYCgd2ZXJzaW9uGAEgASgFUgd2ZXJzaW9uEhsKCWxvY2tfdGltZRgCIA'
    'EoDVIIbG9ja1RpbWUSSAoGaW5wdXRzGAMgAygLMjAuVFcuQml0Y29pblYyLlByb3RvLlRyYW5z'
    'YWN0aW9uLlRyYW5zYWN0aW9uSW5wdXRSBmlucHV0cxJLCgdvdXRwdXRzGAQgAygLMjEuVFcuQm'
    'l0Y29pblYyLlByb3RvLlRyYW5zYWN0aW9uLlRyYW5zYWN0aW9uT3V0cHV0UgdvdXRwdXRzGq0B'
    'ChBUcmFuc2FjdGlvbklucHV0EjkKCW91dF9wb2ludBgBIAEoCzIcLlRXLkJpdGNvaW5WMi5Qcm'
    '90by5PdXRQb2ludFIIb3V0UG9pbnQSGgoIc2VxdWVuY2UYAiABKA1SCHNlcXVlbmNlEh0KCnNj'
    'cmlwdF9zaWcYAyABKAxSCXNjcmlwdFNpZxIjCg13aXRuZXNzX2l0ZW1zGAQgAygMUgx3aXRuZX'
    'NzSXRlbXMaTgoRVHJhbnNhY3Rpb25PdXRwdXQSIwoNc2NyaXB0X3B1YmtleRgBIAEoDFIMc2Ny'
    'aXB0UHVia2V5EhQKBXZhbHVlGAIgASgDUgV2YWx1ZQ==');

@$core.Deprecated('Use transactionPlanDescriptor instead')
const TransactionPlan$json = {
  '1': 'TransactionPlan',
  '2': [
    {'1': 'error', '3': 1, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 2, '4': 1, '5': 9, '10': 'errorMessage'},
    {'1': 'inputs', '3': 3, '4': 3, '5': 11, '6': '.TW.BitcoinV2.Proto.Input', '10': 'inputs'},
    {'1': 'outputs', '3': 4, '4': 3, '5': 11, '6': '.TW.BitcoinV2.Proto.Output', '10': 'outputs'},
    {'1': 'available_amount', '3': 5, '4': 1, '5': 3, '10': 'availableAmount'},
    {'1': 'send_amount', '3': 6, '4': 1, '5': 3, '10': 'sendAmount'},
    {'1': 'vsize_estimate', '3': 7, '4': 1, '5': 4, '10': 'vsizeEstimate'},
    {'1': 'fee_estimate', '3': 8, '4': 1, '5': 3, '10': 'feeEstimate'},
    {'1': 'change', '3': 9, '4': 1, '5': 3, '10': 'change'},
  ],
};

/// Descriptor for `TransactionPlan`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transactionPlanDescriptor = $convert.base64Decode(
    'Cg9UcmFuc2FjdGlvblBsYW4SMwoFZXJyb3IYASABKA4yHS5UVy5Db21tb24uUHJvdG8uU2lnbm'
    'luZ0Vycm9yUgVlcnJvchIjCg1lcnJvcl9tZXNzYWdlGAIgASgJUgxlcnJvck1lc3NhZ2USMQoG'
    'aW5wdXRzGAMgAygLMhkuVFcuQml0Y29pblYyLlByb3RvLklucHV0UgZpbnB1dHMSNAoHb3V0cH'
    'V0cxgEIAMoCzIaLlRXLkJpdGNvaW5WMi5Qcm90by5PdXRwdXRSB291dHB1dHMSKQoQYXZhaWxh'
    'YmxlX2Ftb3VudBgFIAEoA1IPYXZhaWxhYmxlQW1vdW50Eh8KC3NlbmRfYW1vdW50GAYgASgDUg'
    'pzZW5kQW1vdW50EiUKDnZzaXplX2VzdGltYXRlGAcgASgEUg12c2l6ZUVzdGltYXRlEiEKDGZl'
    'ZV9lc3RpbWF0ZRgIIAEoA1ILZmVlRXN0aW1hdGUSFgoGY2hhbmdlGAkgASgDUgZjaGFuZ2U=');

@$core.Deprecated('Use preSigningOutputDescriptor instead')
const PreSigningOutput$json = {
  '1': 'PreSigningOutput',
  '2': [
    {'1': 'error', '3': 1, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 2, '4': 1, '5': 9, '10': 'errorMessage'},
    {'1': 'sighashes', '3': 4, '4': 3, '5': 11, '6': '.TW.BitcoinV2.Proto.PreSigningOutput.Sighash', '10': 'sighashes'},
  ],
  '3': [PreSigningOutput_Sighash$json, PreSigningOutput_TaprootTweak$json],
  '4': [PreSigningOutput_SigningMethod$json],
};

@$core.Deprecated('Use preSigningOutputDescriptor instead')
const PreSigningOutput_Sighash$json = {
  '1': 'Sighash',
  '2': [
    {'1': 'public_key', '3': 1, '4': 1, '5': 12, '10': 'publicKey'},
    {'1': 'sighash', '3': 2, '4': 1, '5': 12, '10': 'sighash'},
    {'1': 'signing_method', '3': 3, '4': 1, '5': 14, '6': '.TW.BitcoinV2.Proto.PreSigningOutput.SigningMethod', '10': 'signingMethod'},
    {'1': 'tweak', '3': 4, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.PreSigningOutput.TaprootTweak', '10': 'tweak'},
  ],
};

@$core.Deprecated('Use preSigningOutputDescriptor instead')
const PreSigningOutput_TaprootTweak$json = {
  '1': 'TaprootTweak',
  '2': [
    {'1': 'merkle_root', '3': 1, '4': 1, '5': 12, '10': 'merkleRoot'},
  ],
};

@$core.Deprecated('Use preSigningOutputDescriptor instead')
const PreSigningOutput_SigningMethod$json = {
  '1': 'SigningMethod',
  '2': [
    {'1': 'Legacy', '2': 0},
    {'1': 'Segwit', '2': 1},
    {'1': 'Taproot', '2': 2},
  ],
};

/// Descriptor for `PreSigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List preSigningOutputDescriptor = $convert.base64Decode(
    'ChBQcmVTaWduaW5nT3V0cHV0EjMKBWVycm9yGAEgASgOMh0uVFcuQ29tbW9uLlByb3RvLlNpZ2'
    '5pbmdFcnJvclIFZXJyb3ISIwoNZXJyb3JfbWVzc2FnZRgCIAEoCVIMZXJyb3JNZXNzYWdlEkoK'
    'CXNpZ2hhc2hlcxgEIAMoCzIsLlRXLkJpdGNvaW5WMi5Qcm90by5QcmVTaWduaW5nT3V0cHV0Ll'
    'NpZ2hhc2hSCXNpZ2hhc2hlcxrmAQoHU2lnaGFzaBIdCgpwdWJsaWNfa2V5GAEgASgMUglwdWJs'
    'aWNLZXkSGAoHc2lnaGFzaBgCIAEoDFIHc2lnaGFzaBJZCg5zaWduaW5nX21ldGhvZBgDIAEoDj'
    'IyLlRXLkJpdGNvaW5WMi5Qcm90by5QcmVTaWduaW5nT3V0cHV0LlNpZ25pbmdNZXRob2RSDXNp'
    'Z25pbmdNZXRob2QSRwoFdHdlYWsYBCABKAsyMS5UVy5CaXRjb2luVjIuUHJvdG8uUHJlU2lnbm'
    'luZ091dHB1dC5UYXByb290VHdlYWtSBXR3ZWFrGi8KDFRhcHJvb3RUd2VhaxIfCgttZXJrbGVf'
    'cm9vdBgBIAEoDFIKbWVya2xlUm9vdCI0Cg1TaWduaW5nTWV0aG9kEgoKBkxlZ2FjeRAAEgoKBl'
    'NlZ3dpdBABEgsKB1RhcHJvb3QQAg==');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'error', '3': 1, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 2, '4': 1, '5': 9, '10': 'errorMessage'},
    {'1': 'transaction', '3': 3, '4': 1, '5': 11, '6': '.TW.BitcoinV2.Proto.Transaction', '10': 'transaction'},
    {'1': 'encoded', '3': 4, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'txid', '3': 5, '4': 1, '5': 12, '10': 'txid'},
    {'1': 'vsize', '3': 6, '4': 1, '5': 4, '10': 'vsize'},
    {'1': 'weight', '3': 7, '4': 1, '5': 4, '10': 'weight'},
    {'1': 'fee', '3': 8, '4': 1, '5': 3, '10': 'fee'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EjMKBWVycm9yGAEgASgOMh0uVFcuQ29tbW9uLlByb3RvLlNpZ25pbm'
    'dFcnJvclIFZXJyb3ISIwoNZXJyb3JfbWVzc2FnZRgCIAEoCVIMZXJyb3JNZXNzYWdlEkEKC3Ry'
    'YW5zYWN0aW9uGAMgASgLMh8uVFcuQml0Y29pblYyLlByb3RvLlRyYW5zYWN0aW9uUgt0cmFuc2'
    'FjdGlvbhIYCgdlbmNvZGVkGAQgASgMUgdlbmNvZGVkEhIKBHR4aWQYBSABKAxSBHR4aWQSFAoF'
    'dnNpemUYBiABKARSBXZzaXplEhYKBndlaWdodBgHIAEoBFIGd2VpZ2h0EhAKA2ZlZRgIIAEoA1'
    'IDZmVl');

