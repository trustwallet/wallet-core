//
//  Generated code. Do not modify.
//  source: EthereumRlp.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'Common.pbenum.dart' as $0;

/// List of elements.
class RlpList extends $pb.GeneratedMessage {
  factory RlpList({
    $core.Iterable<RlpItem>? items,
  }) {
    final $result = create();
    if (items != null) {
      $result.items.addAll(items);
    }
    return $result;
  }
  RlpList._() : super();
  factory RlpList.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RlpList.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'RlpList', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.EthereumRlp.Proto'), createEmptyInstance: create)
    ..pc<RlpItem>(1, _omitFieldNames ? '' : 'items', $pb.PbFieldType.PM, subBuilder: RlpItem.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  RlpList clone() => RlpList()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  RlpList copyWith(void Function(RlpList) updates) => super.copyWith((message) => updates(message as RlpList)) as RlpList;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static RlpList create() => RlpList._();
  RlpList createEmptyInstance() => create();
  static $pb.PbList<RlpList> createRepeated() => $pb.PbList<RlpList>();
  @$core.pragma('dart2js:noInline')
  static RlpList getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RlpList>(create);
  static RlpList? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<RlpItem> get items => $_getList(0);
}

enum RlpItem_Item {
  stringItem, 
  numberU64, 
  numberU256, 
  address, 
  data, 
  list, 
  rawEncoded, 
  notSet
}

/// RLP item.
class RlpItem extends $pb.GeneratedMessage {
  factory RlpItem({
    $core.String? stringItem,
    $fixnum.Int64? numberU64,
    $core.List<$core.int>? numberU256,
    $core.String? address,
    $core.List<$core.int>? data,
    RlpList? list,
    $core.List<$core.int>? rawEncoded,
  }) {
    final $result = create();
    if (stringItem != null) {
      $result.stringItem = stringItem;
    }
    if (numberU64 != null) {
      $result.numberU64 = numberU64;
    }
    if (numberU256 != null) {
      $result.numberU256 = numberU256;
    }
    if (address != null) {
      $result.address = address;
    }
    if (data != null) {
      $result.data = data;
    }
    if (list != null) {
      $result.list = list;
    }
    if (rawEncoded != null) {
      $result.rawEncoded = rawEncoded;
    }
    return $result;
  }
  RlpItem._() : super();
  factory RlpItem.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory RlpItem.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, RlpItem_Item> _RlpItem_ItemByTag = {
    1 : RlpItem_Item.stringItem,
    2 : RlpItem_Item.numberU64,
    3 : RlpItem_Item.numberU256,
    4 : RlpItem_Item.address,
    5 : RlpItem_Item.data,
    6 : RlpItem_Item.list,
    7 : RlpItem_Item.rawEncoded,
    0 : RlpItem_Item.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'RlpItem', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.EthereumRlp.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2, 3, 4, 5, 6, 7])
    ..aOS(1, _omitFieldNames ? '' : 'stringItem')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'numberU64', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'numberU256', $pb.PbFieldType.OY)
    ..aOS(4, _omitFieldNames ? '' : 'address')
    ..a<$core.List<$core.int>>(5, _omitFieldNames ? '' : 'data', $pb.PbFieldType.OY)
    ..aOM<RlpList>(6, _omitFieldNames ? '' : 'list', subBuilder: RlpList.create)
    ..a<$core.List<$core.int>>(7, _omitFieldNames ? '' : 'rawEncoded', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  RlpItem clone() => RlpItem()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  RlpItem copyWith(void Function(RlpItem) updates) => super.copyWith((message) => updates(message as RlpItem)) as RlpItem;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static RlpItem create() => RlpItem._();
  RlpItem createEmptyInstance() => create();
  static $pb.PbList<RlpItem> createRepeated() => $pb.PbList<RlpItem>();
  @$core.pragma('dart2js:noInline')
  static RlpItem getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<RlpItem>(create);
  static RlpItem? _defaultInstance;

  RlpItem_Item whichItem() => _RlpItem_ItemByTag[$_whichOneof(0)]!;
  void clearItem() => clearField($_whichOneof(0));

  /// A string to be encoded.
  @$pb.TagNumber(1)
  $core.String get stringItem => $_getSZ(0);
  @$pb.TagNumber(1)
  set stringItem($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasStringItem() => $_has(0);
  @$pb.TagNumber(1)
  void clearStringItem() => clearField(1);

  /// A U64 number to be encoded.
  @$pb.TagNumber(2)
  $fixnum.Int64 get numberU64 => $_getI64(1);
  @$pb.TagNumber(2)
  set numberU64($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasNumberU64() => $_has(1);
  @$pb.TagNumber(2)
  void clearNumberU64() => clearField(2);

  /// A U256 number to be encoded.
  @$pb.TagNumber(3)
  $core.List<$core.int> get numberU256 => $_getN(2);
  @$pb.TagNumber(3)
  set numberU256($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasNumberU256() => $_has(2);
  @$pb.TagNumber(3)
  void clearNumberU256() => clearField(3);

  /// An address to be encoded.
  @$pb.TagNumber(4)
  $core.String get address => $_getSZ(3);
  @$pb.TagNumber(4)
  set address($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasAddress() => $_has(3);
  @$pb.TagNumber(4)
  void clearAddress() => clearField(4);

  /// A data to be encoded.
  @$pb.TagNumber(5)
  $core.List<$core.int> get data => $_getN(4);
  @$pb.TagNumber(5)
  set data($core.List<$core.int> v) { $_setBytes(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasData() => $_has(4);
  @$pb.TagNumber(5)
  void clearData() => clearField(5);

  /// A list of items to be encoded.
  @$pb.TagNumber(6)
  RlpList get list => $_getN(5);
  @$pb.TagNumber(6)
  set list(RlpList v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasList() => $_has(5);
  @$pb.TagNumber(6)
  void clearList() => clearField(6);
  @$pb.TagNumber(6)
  RlpList ensureList() => $_ensure(5);

  /// An RLP encoded item to be appended as it is.
  @$pb.TagNumber(7)
  $core.List<$core.int> get rawEncoded => $_getN(6);
  @$pb.TagNumber(7)
  set rawEncoded($core.List<$core.int> v) { $_setBytes(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasRawEncoded() => $_has(6);
  @$pb.TagNumber(7)
  void clearRawEncoded() => clearField(7);
}

/// RLP encoding input.
class EncodingInput extends $pb.GeneratedMessage {
  factory EncodingInput({
    RlpItem? item,
  }) {
    final $result = create();
    if (item != null) {
      $result.item = item;
    }
    return $result;
  }
  EncodingInput._() : super();
  factory EncodingInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory EncodingInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'EncodingInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.EthereumRlp.Proto'), createEmptyInstance: create)
    ..aOM<RlpItem>(1, _omitFieldNames ? '' : 'item', subBuilder: RlpItem.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  EncodingInput clone() => EncodingInput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  EncodingInput copyWith(void Function(EncodingInput) updates) => super.copyWith((message) => updates(message as EncodingInput)) as EncodingInput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static EncodingInput create() => EncodingInput._();
  EncodingInput createEmptyInstance() => create();
  static $pb.PbList<EncodingInput> createRepeated() => $pb.PbList<EncodingInput>();
  @$core.pragma('dart2js:noInline')
  static EncodingInput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<EncodingInput>(create);
  static EncodingInput? _defaultInstance;

  /// An item or a list to encode.
  @$pb.TagNumber(1)
  RlpItem get item => $_getN(0);
  @$pb.TagNumber(1)
  set item(RlpItem v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasItem() => $_has(0);
  @$pb.TagNumber(1)
  void clearItem() => clearField(1);
  @$pb.TagNumber(1)
  RlpItem ensureItem() => $_ensure(0);
}

/// / RLP encoding output.
class EncodingOutput extends $pb.GeneratedMessage {
  factory EncodingOutput({
    $core.List<$core.int>? encoded,
    $0.SigningError? error,
    $core.String? errorMessage,
  }) {
    final $result = create();
    if (encoded != null) {
      $result.encoded = encoded;
    }
    if (error != null) {
      $result.error = error;
    }
    if (errorMessage != null) {
      $result.errorMessage = errorMessage;
    }
    return $result;
  }
  EncodingOutput._() : super();
  factory EncodingOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory EncodingOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'EncodingOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.EthereumRlp.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'encoded', $pb.PbFieldType.OY)
    ..e<$0.SigningError>(2, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(3, _omitFieldNames ? '' : 'errorMessage')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  EncodingOutput clone() => EncodingOutput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  EncodingOutput copyWith(void Function(EncodingOutput) updates) => super.copyWith((message) => updates(message as EncodingOutput)) as EncodingOutput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static EncodingOutput create() => EncodingOutput._();
  EncodingOutput createEmptyInstance() => create();
  static $pb.PbList<EncodingOutput> createRepeated() => $pb.PbList<EncodingOutput>();
  @$core.pragma('dart2js:noInline')
  static EncodingOutput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<EncodingOutput>(create);
  static EncodingOutput? _defaultInstance;

  /// An item RLP encoded.
  @$pb.TagNumber(1)
  $core.List<$core.int> get encoded => $_getN(0);
  @$pb.TagNumber(1)
  set encoded($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasEncoded() => $_has(0);
  @$pb.TagNumber(1)
  void clearEncoded() => clearField(1);

  /// Error code, 0 is ok, other codes will be treated as errors.
  @$pb.TagNumber(2)
  $0.SigningError get error => $_getN(1);
  @$pb.TagNumber(2)
  set error($0.SigningError v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasError() => $_has(1);
  @$pb.TagNumber(2)
  void clearError() => clearField(2);

  /// Error code description.
  @$pb.TagNumber(3)
  $core.String get errorMessage => $_getSZ(2);
  @$pb.TagNumber(3)
  set errorMessage($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasErrorMessage() => $_has(2);
  @$pb.TagNumber(3)
  void clearErrorMessage() => clearField(3);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
