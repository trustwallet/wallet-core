//
//  Generated code. Do not modify.
//  source: Bitcoin.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

class TransactionVariant extends $pb.ProtobufEnum {
  static const TransactionVariant P2PKH = TransactionVariant._(0, _omitEnumNames ? '' : 'P2PKH');
  static const TransactionVariant P2WPKH = TransactionVariant._(1, _omitEnumNames ? '' : 'P2WPKH');
  static const TransactionVariant P2TRKEYPATH = TransactionVariant._(2, _omitEnumNames ? '' : 'P2TRKEYPATH');
  static const TransactionVariant BRC20TRANSFER = TransactionVariant._(3, _omitEnumNames ? '' : 'BRC20TRANSFER');
  static const TransactionVariant NFTINSCRIPTION = TransactionVariant._(4, _omitEnumNames ? '' : 'NFTINSCRIPTION');

  static const $core.List<TransactionVariant> values = <TransactionVariant> [
    P2PKH,
    P2WPKH,
    P2TRKEYPATH,
    BRC20TRANSFER,
    NFTINSCRIPTION,
  ];

  static final $core.Map<$core.int, TransactionVariant> _byValue = $pb.ProtobufEnum.initByValue(values);
  static TransactionVariant? valueOf($core.int value) => _byValue[value];

  const TransactionVariant._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
