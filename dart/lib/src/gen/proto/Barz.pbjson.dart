//
//  Generated code. Do not modify.
//  source: Barz.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use facetCutActionDescriptor instead')
const FacetCutAction$json = {
  '1': 'FacetCutAction',
  '2': [
    {'1': 'ADD', '2': 0},
    {'1': 'REPLACE', '2': 1},
    {'1': 'REMOVE', '2': 2},
  ],
};

/// Descriptor for `FacetCutAction`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List facetCutActionDescriptor = $convert.base64Decode(
    'Cg5GYWNldEN1dEFjdGlvbhIHCgNBREQQABILCgdSRVBMQUNFEAESCgoGUkVNT1ZFEAI=');

@$core.Deprecated('Use contractAddressInputDescriptor instead')
const ContractAddressInput$json = {
  '1': 'ContractAddressInput',
  '2': [
    {'1': 'entry_point', '3': 1, '4': 1, '5': 9, '10': 'entryPoint'},
    {'1': 'factory', '3': 2, '4': 1, '5': 9, '10': 'factory'},
    {'1': 'account_facet', '3': 3, '4': 1, '5': 9, '10': 'accountFacet'},
    {'1': 'verification_facet', '3': 4, '4': 1, '5': 9, '10': 'verificationFacet'},
    {'1': 'facet_registry', '3': 5, '4': 1, '5': 9, '10': 'facetRegistry'},
    {'1': 'default_fallback', '3': 6, '4': 1, '5': 9, '10': 'defaultFallback'},
    {'1': 'bytecode', '3': 7, '4': 1, '5': 9, '10': 'bytecode'},
    {'1': 'public_key', '3': 8, '4': 1, '5': 9, '10': 'publicKey'},
    {'1': 'salt', '3': 9, '4': 1, '5': 13, '10': 'salt'},
  ],
};

/// Descriptor for `ContractAddressInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List contractAddressInputDescriptor = $convert.base64Decode(
    'ChRDb250cmFjdEFkZHJlc3NJbnB1dBIfCgtlbnRyeV9wb2ludBgBIAEoCVIKZW50cnlQb2ludB'
    'IYCgdmYWN0b3J5GAIgASgJUgdmYWN0b3J5EiMKDWFjY291bnRfZmFjZXQYAyABKAlSDGFjY291'
    'bnRGYWNldBItChJ2ZXJpZmljYXRpb25fZmFjZXQYBCABKAlSEXZlcmlmaWNhdGlvbkZhY2V0Ei'
    'UKDmZhY2V0X3JlZ2lzdHJ5GAUgASgJUg1mYWNldFJlZ2lzdHJ5EikKEGRlZmF1bHRfZmFsbGJh'
    'Y2sYBiABKAlSD2RlZmF1bHRGYWxsYmFjaxIaCghieXRlY29kZRgHIAEoCVIIYnl0ZWNvZGUSHQ'
    'oKcHVibGljX2tleRgIIAEoCVIJcHVibGljS2V5EhIKBHNhbHQYCSABKA1SBHNhbHQ=');

@$core.Deprecated('Use facetCutDescriptor instead')
const FacetCut$json = {
  '1': 'FacetCut',
  '2': [
    {'1': 'facet_address', '3': 1, '4': 1, '5': 9, '10': 'facetAddress'},
    {'1': 'action', '3': 2, '4': 1, '5': 14, '6': '.TW.Barz.Proto.FacetCutAction', '10': 'action'},
    {'1': 'function_selectors', '3': 3, '4': 3, '5': 12, '10': 'functionSelectors'},
  ],
};

/// Descriptor for `FacetCut`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List facetCutDescriptor = $convert.base64Decode(
    'CghGYWNldEN1dBIjCg1mYWNldF9hZGRyZXNzGAEgASgJUgxmYWNldEFkZHJlc3MSNQoGYWN0aW'
    '9uGAIgASgOMh0uVFcuQmFyei5Qcm90by5GYWNldEN1dEFjdGlvblIGYWN0aW9uEi0KEmZ1bmN0'
    'aW9uX3NlbGVjdG9ycxgDIAMoDFIRZnVuY3Rpb25TZWxlY3RvcnM=');

@$core.Deprecated('Use diamondCutInputDescriptor instead')
const DiamondCutInput$json = {
  '1': 'DiamondCutInput',
  '2': [
    {'1': 'facet_cuts', '3': 1, '4': 3, '5': 11, '6': '.TW.Barz.Proto.FacetCut', '10': 'facetCuts'},
    {'1': 'init_address', '3': 2, '4': 1, '5': 9, '10': 'initAddress'},
    {'1': 'init_data', '3': 3, '4': 1, '5': 12, '10': 'initData'},
  ],
};

/// Descriptor for `DiamondCutInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List diamondCutInputDescriptor = $convert.base64Decode(
    'Cg9EaWFtb25kQ3V0SW5wdXQSNgoKZmFjZXRfY3V0cxgBIAMoCzIXLlRXLkJhcnouUHJvdG8uRm'
    'FjZXRDdXRSCWZhY2V0Q3V0cxIhCgxpbml0X2FkZHJlc3MYAiABKAlSC2luaXRBZGRyZXNzEhsK'
    'CWluaXRfZGF0YRgDIAEoDFIIaW5pdERhdGE=');

