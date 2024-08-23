//
//  Generated code. Do not modify.
//  source: EthereumAbi.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use abiErrorDescriptor instead')
const AbiError$json = {
  '1': 'AbiError',
  '2': [
    {'1': 'OK', '2': 0},
    {'1': 'Error_internal', '2': 1},
    {'1': 'Error_abi_mismatch', '2': 2},
    {'1': 'Error_invalid_abi', '2': 3},
    {'1': 'Error_invalid_param_type', '2': 4},
    {'1': 'Error_invalid_address_value', '2': 5},
    {'1': 'Error_invalid_uint_value', '2': 6},
    {'1': 'Error_missing_param_type', '2': 7},
    {'1': 'Error_missing_param_value', '2': 8},
    {'1': 'Error_decoding_data', '2': 9},
    {'1': 'Error_empty_type', '2': 10},
  ],
};

/// Descriptor for `AbiError`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List abiErrorDescriptor = $convert.base64Decode(
    'CghBYmlFcnJvchIGCgJPSxAAEhIKDkVycm9yX2ludGVybmFsEAESFgoSRXJyb3JfYWJpX21pc2'
    '1hdGNoEAISFQoRRXJyb3JfaW52YWxpZF9hYmkQAxIcChhFcnJvcl9pbnZhbGlkX3BhcmFtX3R5'
    'cGUQBBIfChtFcnJvcl9pbnZhbGlkX2FkZHJlc3NfdmFsdWUQBRIcChhFcnJvcl9pbnZhbGlkX3'
    'VpbnRfdmFsdWUQBhIcChhFcnJvcl9taXNzaW5nX3BhcmFtX3R5cGUQBxIdChlFcnJvcl9taXNz'
    'aW5nX3BhcmFtX3ZhbHVlEAgSFwoTRXJyb3JfZGVjb2RpbmdfZGF0YRAJEhQKEEVycm9yX2VtcH'
    'R5X3R5cGUQCg==');

@$core.Deprecated('Use boolTypeDescriptor instead')
const BoolType$json = {
  '1': 'BoolType',
};

/// Descriptor for `BoolType`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List boolTypeDescriptor = $convert.base64Decode(
    'CghCb29sVHlwZQ==');

@$core.Deprecated('Use numberNTypeDescriptor instead')
const NumberNType$json = {
  '1': 'NumberNType',
  '2': [
    {'1': 'bits', '3': 1, '4': 1, '5': 13, '10': 'bits'},
  ],
};

/// Descriptor for `NumberNType`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List numberNTypeDescriptor = $convert.base64Decode(
    'CgtOdW1iZXJOVHlwZRISCgRiaXRzGAEgASgNUgRiaXRz');

@$core.Deprecated('Use stringTypeDescriptor instead')
const StringType$json = {
  '1': 'StringType',
};

/// Descriptor for `StringType`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List stringTypeDescriptor = $convert.base64Decode(
    'CgpTdHJpbmdUeXBl');

@$core.Deprecated('Use addressTypeDescriptor instead')
const AddressType$json = {
  '1': 'AddressType',
};

/// Descriptor for `AddressType`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List addressTypeDescriptor = $convert.base64Decode(
    'CgtBZGRyZXNzVHlwZQ==');

@$core.Deprecated('Use arrayTypeDescriptor instead')
const ArrayType$json = {
  '1': 'ArrayType',
  '2': [
    {'1': 'element_type', '3': 1, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.ParamType', '10': 'elementType'},
  ],
};

/// Descriptor for `ArrayType`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List arrayTypeDescriptor = $convert.base64Decode(
    'CglBcnJheVR5cGUSQgoMZWxlbWVudF90eXBlGAEgASgLMh8uVFcuRXRoZXJldW1BYmkuUHJvdG'
    '8uUGFyYW1UeXBlUgtlbGVtZW50VHlwZQ==');

@$core.Deprecated('Use fixedArrayTypeDescriptor instead')
const FixedArrayType$json = {
  '1': 'FixedArrayType',
  '2': [
    {'1': 'size', '3': 1, '4': 1, '5': 4, '10': 'size'},
    {'1': 'element_type', '3': 2, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.ParamType', '10': 'elementType'},
  ],
};

/// Descriptor for `FixedArrayType`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List fixedArrayTypeDescriptor = $convert.base64Decode(
    'Cg5GaXhlZEFycmF5VHlwZRISCgRzaXplGAEgASgEUgRzaXplEkIKDGVsZW1lbnRfdHlwZRgCIA'
    'EoCzIfLlRXLkV0aGVyZXVtQWJpLlByb3RvLlBhcmFtVHlwZVILZWxlbWVudFR5cGU=');

@$core.Deprecated('Use byteArrayTypeDescriptor instead')
const ByteArrayType$json = {
  '1': 'ByteArrayType',
};

/// Descriptor for `ByteArrayType`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List byteArrayTypeDescriptor = $convert.base64Decode(
    'Cg1CeXRlQXJyYXlUeXBl');

@$core.Deprecated('Use byteArrayFixTypeDescriptor instead')
const ByteArrayFixType$json = {
  '1': 'ByteArrayFixType',
  '2': [
    {'1': 'size', '3': 1, '4': 1, '5': 4, '10': 'size'},
  ],
};

/// Descriptor for `ByteArrayFixType`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List byteArrayFixTypeDescriptor = $convert.base64Decode(
    'ChBCeXRlQXJyYXlGaXhUeXBlEhIKBHNpemUYASABKARSBHNpemU=');

@$core.Deprecated('Use tupleTypeDescriptor instead')
const TupleType$json = {
  '1': 'TupleType',
  '2': [
    {'1': 'params', '3': 1, '4': 3, '5': 11, '6': '.TW.EthereumAbi.Proto.Param', '10': 'params'},
  ],
};

/// Descriptor for `TupleType`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tupleTypeDescriptor = $convert.base64Decode(
    'CglUdXBsZVR5cGUSMwoGcGFyYW1zGAEgAygLMhsuVFcuRXRoZXJldW1BYmkuUHJvdG8uUGFyYW'
    '1SBnBhcmFtcw==');

@$core.Deprecated('Use paramDescriptor instead')
const Param$json = {
  '1': 'Param',
  '2': [
    {'1': 'name', '3': 1, '4': 1, '5': 9, '10': 'name'},
    {'1': 'param', '3': 2, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.ParamType', '10': 'param'},
  ],
};

/// Descriptor for `Param`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List paramDescriptor = $convert.base64Decode(
    'CgVQYXJhbRISCgRuYW1lGAEgASgJUgRuYW1lEjUKBXBhcmFtGAIgASgLMh8uVFcuRXRoZXJldW'
    '1BYmkuUHJvdG8uUGFyYW1UeXBlUgVwYXJhbQ==');

@$core.Deprecated('Use paramTypeDescriptor instead')
const ParamType$json = {
  '1': 'ParamType',
  '2': [
    {'1': 'boolean', '3': 1, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.BoolType', '9': 0, '10': 'boolean'},
    {'1': 'number_int', '3': 2, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.NumberNType', '9': 0, '10': 'numberInt'},
    {'1': 'number_uint', '3': 3, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.NumberNType', '9': 0, '10': 'numberUint'},
    {'1': 'string_param', '3': 7, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.StringType', '9': 0, '10': 'stringParam'},
    {'1': 'address', '3': 8, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.AddressType', '9': 0, '10': 'address'},
    {'1': 'byte_array', '3': 9, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.ByteArrayType', '9': 0, '10': 'byteArray'},
    {'1': 'byte_array_fix', '3': 10, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.ByteArrayFixType', '9': 0, '10': 'byteArrayFix'},
    {'1': 'array', '3': 14, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.ArrayType', '9': 0, '10': 'array'},
    {'1': 'fixed_array', '3': 15, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.FixedArrayType', '9': 0, '10': 'fixedArray'},
    {'1': 'tuple', '3': 19, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.TupleType', '9': 0, '10': 'tuple'},
  ],
  '8': [
    {'1': 'param'},
  ],
};

/// Descriptor for `ParamType`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List paramTypeDescriptor = $convert.base64Decode(
    'CglQYXJhbVR5cGUSOgoHYm9vbGVhbhgBIAEoCzIeLlRXLkV0aGVyZXVtQWJpLlByb3RvLkJvb2'
    'xUeXBlSABSB2Jvb2xlYW4SQgoKbnVtYmVyX2ludBgCIAEoCzIhLlRXLkV0aGVyZXVtQWJpLlBy'
    'b3RvLk51bWJlck5UeXBlSABSCW51bWJlckludBJECgtudW1iZXJfdWludBgDIAEoCzIhLlRXLk'
    'V0aGVyZXVtQWJpLlByb3RvLk51bWJlck5UeXBlSABSCm51bWJlclVpbnQSRQoMc3RyaW5nX3Bh'
    'cmFtGAcgASgLMiAuVFcuRXRoZXJldW1BYmkuUHJvdG8uU3RyaW5nVHlwZUgAUgtzdHJpbmdQYX'
    'JhbRI9CgdhZGRyZXNzGAggASgLMiEuVFcuRXRoZXJldW1BYmkuUHJvdG8uQWRkcmVzc1R5cGVI'
    'AFIHYWRkcmVzcxJECgpieXRlX2FycmF5GAkgASgLMiMuVFcuRXRoZXJldW1BYmkuUHJvdG8uQn'
    'l0ZUFycmF5VHlwZUgAUglieXRlQXJyYXkSTgoOYnl0ZV9hcnJheV9maXgYCiABKAsyJi5UVy5F'
    'dGhlcmV1bUFiaS5Qcm90by5CeXRlQXJyYXlGaXhUeXBlSABSDGJ5dGVBcnJheUZpeBI3CgVhcn'
    'JheRgOIAEoCzIfLlRXLkV0aGVyZXVtQWJpLlByb3RvLkFycmF5VHlwZUgAUgVhcnJheRJHCgtm'
    'aXhlZF9hcnJheRgPIAEoCzIkLlRXLkV0aGVyZXVtQWJpLlByb3RvLkZpeGVkQXJyYXlUeXBlSA'
    'BSCmZpeGVkQXJyYXkSNwoFdHVwbGUYEyABKAsyHy5UVy5FdGhlcmV1bUFiaS5Qcm90by5UdXBs'
    'ZVR5cGVIAFIFdHVwbGVCBwoFcGFyYW0=');

@$core.Deprecated('Use numberNParamDescriptor instead')
const NumberNParam$json = {
  '1': 'NumberNParam',
  '2': [
    {'1': 'bits', '3': 1, '4': 1, '5': 13, '10': 'bits'},
    {'1': 'value', '3': 2, '4': 1, '5': 12, '10': 'value'},
  ],
};

/// Descriptor for `NumberNParam`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List numberNParamDescriptor = $convert.base64Decode(
    'CgxOdW1iZXJOUGFyYW0SEgoEYml0cxgBIAEoDVIEYml0cxIUCgV2YWx1ZRgCIAEoDFIFdmFsdW'
    'U=');

@$core.Deprecated('Use arrayParamDescriptor instead')
const ArrayParam$json = {
  '1': 'ArrayParam',
  '2': [
    {'1': 'element_type', '3': 1, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.ParamType', '10': 'elementType'},
    {'1': 'elements', '3': 2, '4': 3, '5': 11, '6': '.TW.EthereumAbi.Proto.Token', '10': 'elements'},
  ],
};

/// Descriptor for `ArrayParam`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List arrayParamDescriptor = $convert.base64Decode(
    'CgpBcnJheVBhcmFtEkIKDGVsZW1lbnRfdHlwZRgBIAEoCzIfLlRXLkV0aGVyZXVtQWJpLlByb3'
    'RvLlBhcmFtVHlwZVILZWxlbWVudFR5cGUSNwoIZWxlbWVudHMYAiADKAsyGy5UVy5FdGhlcmV1'
    'bUFiaS5Qcm90by5Ub2tlblIIZWxlbWVudHM=');

@$core.Deprecated('Use tupleParamDescriptor instead')
const TupleParam$json = {
  '1': 'TupleParam',
  '2': [
    {'1': 'params', '3': 1, '4': 3, '5': 11, '6': '.TW.EthereumAbi.Proto.Token', '10': 'params'},
  ],
};

/// Descriptor for `TupleParam`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tupleParamDescriptor = $convert.base64Decode(
    'CgpUdXBsZVBhcmFtEjMKBnBhcmFtcxgBIAMoCzIbLlRXLkV0aGVyZXVtQWJpLlByb3RvLlRva2'
    'VuUgZwYXJhbXM=');

@$core.Deprecated('Use tokenDescriptor instead')
const Token$json = {
  '1': 'Token',
  '2': [
    {'1': 'name', '3': 1, '4': 1, '5': 9, '10': 'name'},
    {'1': 'boolean', '3': 2, '4': 1, '5': 8, '9': 0, '10': 'boolean'},
    {'1': 'number_int', '3': 3, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.NumberNParam', '9': 0, '10': 'numberInt'},
    {'1': 'number_uint', '3': 4, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.NumberNParam', '9': 0, '10': 'numberUint'},
    {'1': 'string_value', '3': 7, '4': 1, '5': 9, '9': 0, '10': 'stringValue'},
    {'1': 'address', '3': 8, '4': 1, '5': 9, '9': 0, '10': 'address'},
    {'1': 'byte_array', '3': 9, '4': 1, '5': 12, '9': 0, '10': 'byteArray'},
    {'1': 'byte_array_fix', '3': 10, '4': 1, '5': 12, '9': 0, '10': 'byteArrayFix'},
    {'1': 'array', '3': 14, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.ArrayParam', '9': 0, '10': 'array'},
    {'1': 'fixed_array', '3': 15, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.ArrayParam', '9': 0, '10': 'fixedArray'},
    {'1': 'tuple', '3': 19, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.TupleParam', '9': 0, '10': 'tuple'},
  ],
  '8': [
    {'1': 'token'},
  ],
};

/// Descriptor for `Token`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List tokenDescriptor = $convert.base64Decode(
    'CgVUb2tlbhISCgRuYW1lGAEgASgJUgRuYW1lEhoKB2Jvb2xlYW4YAiABKAhIAFIHYm9vbGVhbh'
    'JDCgpudW1iZXJfaW50GAMgASgLMiIuVFcuRXRoZXJldW1BYmkuUHJvdG8uTnVtYmVyTlBhcmFt'
    'SABSCW51bWJlckludBJFCgtudW1iZXJfdWludBgEIAEoCzIiLlRXLkV0aGVyZXVtQWJpLlByb3'
    'RvLk51bWJlck5QYXJhbUgAUgpudW1iZXJVaW50EiMKDHN0cmluZ192YWx1ZRgHIAEoCUgAUgtz'
    'dHJpbmdWYWx1ZRIaCgdhZGRyZXNzGAggASgJSABSB2FkZHJlc3MSHwoKYnl0ZV9hcnJheRgJIA'
    'EoDEgAUglieXRlQXJyYXkSJgoOYnl0ZV9hcnJheV9maXgYCiABKAxIAFIMYnl0ZUFycmF5Rml4'
    'EjgKBWFycmF5GA4gASgLMiAuVFcuRXRoZXJldW1BYmkuUHJvdG8uQXJyYXlQYXJhbUgAUgVhcn'
    'JheRJDCgtmaXhlZF9hcnJheRgPIAEoCzIgLlRXLkV0aGVyZXVtQWJpLlByb3RvLkFycmF5UGFy'
    'YW1IAFIKZml4ZWRBcnJheRI4CgV0dXBsZRgTIAEoCzIgLlRXLkV0aGVyZXVtQWJpLlByb3RvLl'
    'R1cGxlUGFyYW1IAFIFdHVwbGVCBwoFdG9rZW4=');

@$core.Deprecated('Use contractCallDecodingInputDescriptor instead')
const ContractCallDecodingInput$json = {
  '1': 'ContractCallDecodingInput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'smart_contract_abi_json', '3': 2, '4': 1, '5': 9, '10': 'smartContractAbiJson'},
  ],
};

/// Descriptor for `ContractCallDecodingInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List contractCallDecodingInputDescriptor = $convert.base64Decode(
    'ChlDb250cmFjdENhbGxEZWNvZGluZ0lucHV0EhgKB2VuY29kZWQYASABKAxSB2VuY29kZWQSNQ'
    'oXc21hcnRfY29udHJhY3RfYWJpX2pzb24YAiABKAlSFHNtYXJ0Q29udHJhY3RBYmlKc29u');

@$core.Deprecated('Use contractCallDecodingOutputDescriptor instead')
const ContractCallDecodingOutput$json = {
  '1': 'ContractCallDecodingOutput',
  '2': [
    {'1': 'decoded_json', '3': 1, '4': 1, '5': 9, '10': 'decodedJson'},
    {'1': 'tokens', '3': 2, '4': 3, '5': 11, '6': '.TW.EthereumAbi.Proto.Token', '10': 'tokens'},
    {'1': 'error', '3': 3, '4': 1, '5': 14, '6': '.TW.EthereumAbi.Proto.AbiError', '10': 'error'},
    {'1': 'error_message', '3': 4, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `ContractCallDecodingOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List contractCallDecodingOutputDescriptor = $convert.base64Decode(
    'ChpDb250cmFjdENhbGxEZWNvZGluZ091dHB1dBIhCgxkZWNvZGVkX2pzb24YASABKAlSC2RlY2'
    '9kZWRKc29uEjMKBnRva2VucxgCIAMoCzIbLlRXLkV0aGVyZXVtQWJpLlByb3RvLlRva2VuUgZ0'
    'b2tlbnMSNAoFZXJyb3IYAyABKA4yHi5UVy5FdGhlcmV1bUFiaS5Qcm90by5BYmlFcnJvclIFZX'
    'Jyb3ISIwoNZXJyb3JfbWVzc2FnZRgEIAEoCVIMZXJyb3JNZXNzYWdl');

@$core.Deprecated('Use abiParamsDescriptor instead')
const AbiParams$json = {
  '1': 'AbiParams',
  '2': [
    {'1': 'params', '3': 1, '4': 3, '5': 11, '6': '.TW.EthereumAbi.Proto.Param', '10': 'params'},
  ],
};

/// Descriptor for `AbiParams`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List abiParamsDescriptor = $convert.base64Decode(
    'CglBYmlQYXJhbXMSMwoGcGFyYW1zGAEgAygLMhsuVFcuRXRoZXJldW1BYmkuUHJvdG8uUGFyYW'
    '1SBnBhcmFtcw==');

@$core.Deprecated('Use paramsDecodingInputDescriptor instead')
const ParamsDecodingInput$json = {
  '1': 'ParamsDecodingInput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'abi_json', '3': 2, '4': 1, '5': 9, '9': 0, '10': 'abiJson'},
    {'1': 'abi_params', '3': 3, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.AbiParams', '9': 0, '10': 'abiParams'},
  ],
  '8': [
    {'1': 'abi'},
  ],
};

/// Descriptor for `ParamsDecodingInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List paramsDecodingInputDescriptor = $convert.base64Decode(
    'ChNQYXJhbXNEZWNvZGluZ0lucHV0EhgKB2VuY29kZWQYASABKAxSB2VuY29kZWQSGwoIYWJpX2'
    'pzb24YAiABKAlIAFIHYWJpSnNvbhJACgphYmlfcGFyYW1zGAMgASgLMh8uVFcuRXRoZXJldW1B'
    'YmkuUHJvdG8uQWJpUGFyYW1zSABSCWFiaVBhcmFtc0IFCgNhYmk=');

@$core.Deprecated('Use paramsDecodingOutputDescriptor instead')
const ParamsDecodingOutput$json = {
  '1': 'ParamsDecodingOutput',
  '2': [
    {'1': 'tokens', '3': 1, '4': 3, '5': 11, '6': '.TW.EthereumAbi.Proto.Token', '10': 'tokens'},
    {'1': 'error', '3': 2, '4': 1, '5': 14, '6': '.TW.EthereumAbi.Proto.AbiError', '10': 'error'},
    {'1': 'error_message', '3': 3, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `ParamsDecodingOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List paramsDecodingOutputDescriptor = $convert.base64Decode(
    'ChRQYXJhbXNEZWNvZGluZ091dHB1dBIzCgZ0b2tlbnMYASADKAsyGy5UVy5FdGhlcmV1bUFiaS'
    '5Qcm90by5Ub2tlblIGdG9rZW5zEjQKBWVycm9yGAIgASgOMh4uVFcuRXRoZXJldW1BYmkuUHJv'
    'dG8uQWJpRXJyb3JSBWVycm9yEiMKDWVycm9yX21lc3NhZ2UYAyABKAlSDGVycm9yTWVzc2FnZQ'
    '==');

@$core.Deprecated('Use valueDecodingInputDescriptor instead')
const ValueDecodingInput$json = {
  '1': 'ValueDecodingInput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'param_type', '3': 2, '4': 1, '5': 9, '10': 'paramType'},
  ],
};

/// Descriptor for `ValueDecodingInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List valueDecodingInputDescriptor = $convert.base64Decode(
    'ChJWYWx1ZURlY29kaW5nSW5wdXQSGAoHZW5jb2RlZBgBIAEoDFIHZW5jb2RlZBIdCgpwYXJhbV'
    '90eXBlGAIgASgJUglwYXJhbVR5cGU=');

@$core.Deprecated('Use valueDecodingOutputDescriptor instead')
const ValueDecodingOutput$json = {
  '1': 'ValueDecodingOutput',
  '2': [
    {'1': 'token', '3': 1, '4': 1, '5': 11, '6': '.TW.EthereumAbi.Proto.Token', '10': 'token'},
    {'1': 'param_str', '3': 2, '4': 1, '5': 9, '10': 'paramStr'},
    {'1': 'error', '3': 3, '4': 1, '5': 14, '6': '.TW.EthereumAbi.Proto.AbiError', '10': 'error'},
    {'1': 'error_message', '3': 4, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `ValueDecodingOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List valueDecodingOutputDescriptor = $convert.base64Decode(
    'ChNWYWx1ZURlY29kaW5nT3V0cHV0EjEKBXRva2VuGAEgASgLMhsuVFcuRXRoZXJldW1BYmkuUH'
    'JvdG8uVG9rZW5SBXRva2VuEhsKCXBhcmFtX3N0chgCIAEoCVIIcGFyYW1TdHISNAoFZXJyb3IY'
    'AyABKA4yHi5UVy5FdGhlcmV1bUFiaS5Qcm90by5BYmlFcnJvclIFZXJyb3ISIwoNZXJyb3JfbW'
    'Vzc2FnZRgEIAEoCVIMZXJyb3JNZXNzYWdl');

@$core.Deprecated('Use functionEncodingInputDescriptor instead')
const FunctionEncodingInput$json = {
  '1': 'FunctionEncodingInput',
  '2': [
    {'1': 'function_name', '3': 1, '4': 1, '5': 9, '10': 'functionName'},
    {'1': 'tokens', '3': 2, '4': 3, '5': 11, '6': '.TW.EthereumAbi.Proto.Token', '10': 'tokens'},
  ],
};

/// Descriptor for `FunctionEncodingInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List functionEncodingInputDescriptor = $convert.base64Decode(
    'ChVGdW5jdGlvbkVuY29kaW5nSW5wdXQSIwoNZnVuY3Rpb25fbmFtZRgBIAEoCVIMZnVuY3Rpb2'
    '5OYW1lEjMKBnRva2VucxgCIAMoCzIbLlRXLkV0aGVyZXVtQWJpLlByb3RvLlRva2VuUgZ0b2tl'
    'bnM=');

@$core.Deprecated('Use functionEncodingOutputDescriptor instead')
const FunctionEncodingOutput$json = {
  '1': 'FunctionEncodingOutput',
  '2': [
    {'1': 'function_type', '3': 1, '4': 1, '5': 9, '10': 'functionType'},
    {'1': 'encoded', '3': 2, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'error', '3': 3, '4': 1, '5': 14, '6': '.TW.EthereumAbi.Proto.AbiError', '10': 'error'},
    {'1': 'error_message', '3': 4, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `FunctionEncodingOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List functionEncodingOutputDescriptor = $convert.base64Decode(
    'ChZGdW5jdGlvbkVuY29kaW5nT3V0cHV0EiMKDWZ1bmN0aW9uX3R5cGUYASABKAlSDGZ1bmN0aW'
    '9uVHlwZRIYCgdlbmNvZGVkGAIgASgMUgdlbmNvZGVkEjQKBWVycm9yGAMgASgOMh4uVFcuRXRo'
    'ZXJldW1BYmkuUHJvdG8uQWJpRXJyb3JSBWVycm9yEiMKDWVycm9yX21lc3NhZ2UYBCABKAlSDG'
    'Vycm9yTWVzc2FnZQ==');

@$core.Deprecated('Use functionGetTypeInputDescriptor instead')
const FunctionGetTypeInput$json = {
  '1': 'FunctionGetTypeInput',
  '2': [
    {'1': 'function_name', '3': 1, '4': 1, '5': 9, '10': 'functionName'},
    {'1': 'inputs', '3': 2, '4': 3, '5': 11, '6': '.TW.EthereumAbi.Proto.Param', '10': 'inputs'},
  ],
};

/// Descriptor for `FunctionGetTypeInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List functionGetTypeInputDescriptor = $convert.base64Decode(
    'ChRGdW5jdGlvbkdldFR5cGVJbnB1dBIjCg1mdW5jdGlvbl9uYW1lGAEgASgJUgxmdW5jdGlvbk'
    '5hbWUSMwoGaW5wdXRzGAIgAygLMhsuVFcuRXRoZXJldW1BYmkuUHJvdG8uUGFyYW1SBmlucHV0'
    'cw==');

