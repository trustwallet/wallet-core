//
//  Generated code. Do not modify.
//  source: BitcoinV2.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

class InputSelector extends $pb.ProtobufEnum {
  static const InputSelector SelectAscending = InputSelector._(0, _omitEnumNames ? '' : 'SelectAscending');
  static const InputSelector SelectInOrder = InputSelector._(1, _omitEnumNames ? '' : 'SelectInOrder');
  static const InputSelector SelectDescending = InputSelector._(2, _omitEnumNames ? '' : 'SelectDescending');
  static const InputSelector UseAll = InputSelector._(10, _omitEnumNames ? '' : 'UseAll');

  static const $core.List<InputSelector> values = <InputSelector> [
    SelectAscending,
    SelectInOrder,
    SelectDescending,
    UseAll,
  ];

  static final $core.Map<$core.int, InputSelector> _byValue = $pb.ProtobufEnum.initByValue(values);
  static InputSelector? valueOf($core.int value) => _byValue[value];

  const InputSelector._($core.int v, $core.String n) : super(v, n);
}

class TransactionVersion extends $pb.ProtobufEnum {
  static const TransactionVersion UseDefault = TransactionVersion._(0, _omitEnumNames ? '' : 'UseDefault');
  static const TransactionVersion V1 = TransactionVersion._(1, _omitEnumNames ? '' : 'V1');
  static const TransactionVersion V2 = TransactionVersion._(2, _omitEnumNames ? '' : 'V2');

  static const $core.List<TransactionVersion> values = <TransactionVersion> [
    UseDefault,
    V1,
    V2,
  ];

  static final $core.Map<$core.int, TransactionVersion> _byValue = $pb.ProtobufEnum.initByValue(values);
  static TransactionVersion? valueOf($core.int value) => _byValue[value];

  const TransactionVersion._($core.int v, $core.String n) : super(v, n);
}

class PreSigningOutput_SigningMethod extends $pb.ProtobufEnum {
  static const PreSigningOutput_SigningMethod Legacy = PreSigningOutput_SigningMethod._(0, _omitEnumNames ? '' : 'Legacy');
  static const PreSigningOutput_SigningMethod Segwit = PreSigningOutput_SigningMethod._(1, _omitEnumNames ? '' : 'Segwit');
  static const PreSigningOutput_SigningMethod Taproot = PreSigningOutput_SigningMethod._(2, _omitEnumNames ? '' : 'Taproot');

  static const $core.List<PreSigningOutput_SigningMethod> values = <PreSigningOutput_SigningMethod> [
    Legacy,
    Segwit,
    Taproot,
  ];

  static final $core.Map<$core.int, PreSigningOutput_SigningMethod> _byValue = $pb.ProtobufEnum.initByValue(values);
  static PreSigningOutput_SigningMethod? valueOf($core.int value) => _byValue[value];

  const PreSigningOutput_SigningMethod._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
