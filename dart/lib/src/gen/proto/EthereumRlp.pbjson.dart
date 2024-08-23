//
//  Generated code. Do not modify.
//  source: EthereumRlp.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use rlpListDescriptor instead')
const RlpList$json = {
  '1': 'RlpList',
  '2': [
    {'1': 'items', '3': 1, '4': 3, '5': 11, '6': '.TW.EthereumRlp.Proto.RlpItem', '10': 'items'},
  ],
};

/// Descriptor for `RlpList`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List rlpListDescriptor = $convert.base64Decode(
    'CgdSbHBMaXN0EjMKBWl0ZW1zGAEgAygLMh0uVFcuRXRoZXJldW1SbHAuUHJvdG8uUmxwSXRlbV'
    'IFaXRlbXM=');

@$core.Deprecated('Use rlpItemDescriptor instead')
const RlpItem$json = {
  '1': 'RlpItem',
  '2': [
    {'1': 'string_item', '3': 1, '4': 1, '5': 9, '9': 0, '10': 'stringItem'},
    {'1': 'number_u64', '3': 2, '4': 1, '5': 4, '9': 0, '10': 'numberU64'},
    {'1': 'number_u256', '3': 3, '4': 1, '5': 12, '9': 0, '10': 'numberU256'},
    {'1': 'address', '3': 4, '4': 1, '5': 9, '9': 0, '10': 'address'},
    {'1': 'data', '3': 5, '4': 1, '5': 12, '9': 0, '10': 'data'},
    {'1': 'list', '3': 6, '4': 1, '5': 11, '6': '.TW.EthereumRlp.Proto.RlpList', '9': 0, '10': 'list'},
    {'1': 'raw_encoded', '3': 7, '4': 1, '5': 12, '9': 0, '10': 'rawEncoded'},
  ],
  '8': [
    {'1': 'item'},
  ],
};

/// Descriptor for `RlpItem`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List rlpItemDescriptor = $convert.base64Decode(
    'CgdSbHBJdGVtEiEKC3N0cmluZ19pdGVtGAEgASgJSABSCnN0cmluZ0l0ZW0SHwoKbnVtYmVyX3'
    'U2NBgCIAEoBEgAUgludW1iZXJVNjQSIQoLbnVtYmVyX3UyNTYYAyABKAxIAFIKbnVtYmVyVTI1'
    'NhIaCgdhZGRyZXNzGAQgASgJSABSB2FkZHJlc3MSFAoEZGF0YRgFIAEoDEgAUgRkYXRhEjMKBG'
    'xpc3QYBiABKAsyHS5UVy5FdGhlcmV1bVJscC5Qcm90by5SbHBMaXN0SABSBGxpc3QSIQoLcmF3'
    'X2VuY29kZWQYByABKAxIAFIKcmF3RW5jb2RlZEIGCgRpdGVt');

@$core.Deprecated('Use encodingInputDescriptor instead')
const EncodingInput$json = {
  '1': 'EncodingInput',
  '2': [
    {'1': 'item', '3': 1, '4': 1, '5': 11, '6': '.TW.EthereumRlp.Proto.RlpItem', '10': 'item'},
  ],
};

/// Descriptor for `EncodingInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List encodingInputDescriptor = $convert.base64Decode(
    'Cg1FbmNvZGluZ0lucHV0EjEKBGl0ZW0YASABKAsyHS5UVy5FdGhlcmV1bVJscC5Qcm90by5SbH'
    'BJdGVtUgRpdGVt');

@$core.Deprecated('Use encodingOutputDescriptor instead')
const EncodingOutput$json = {
  '1': 'EncodingOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'error', '3': 2, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 3, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `EncodingOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List encodingOutputDescriptor = $convert.base64Decode(
    'Cg5FbmNvZGluZ091dHB1dBIYCgdlbmNvZGVkGAEgASgMUgdlbmNvZGVkEjMKBWVycm9yGAIgAS'
    'gOMh0uVFcuQ29tbW9uLlByb3RvLlNpZ25pbmdFcnJvclIFZXJyb3ISIwoNZXJyb3JfbWVzc2Fn'
    'ZRgDIAEoCVIMZXJyb3JNZXNzYWdl');

