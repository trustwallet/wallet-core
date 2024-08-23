//
//  Generated code. Do not modify.
//  source: FIO.proto
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

/// A public blockchain address, such as {"BTC", "bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v"}
class PublicAddress extends $pb.GeneratedMessage {
  factory PublicAddress({
    $core.String? coinSymbol,
    $core.String? address,
  }) {
    final $result = create();
    if (coinSymbol != null) {
      $result.coinSymbol = coinSymbol;
    }
    if (address != null) {
      $result.address = address;
    }
    return $result;
  }
  PublicAddress._() : super();
  factory PublicAddress.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PublicAddress.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'PublicAddress', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.FIO.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'coinSymbol')
    ..aOS(2, _omitFieldNames ? '' : 'address')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  PublicAddress clone() => PublicAddress()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  PublicAddress copyWith(void Function(PublicAddress) updates) => super.copyWith((message) => updates(message as PublicAddress)) as PublicAddress;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static PublicAddress create() => PublicAddress._();
  PublicAddress createEmptyInstance() => create();
  static $pb.PbList<PublicAddress> createRepeated() => $pb.PbList<PublicAddress>();
  @$core.pragma('dart2js:noInline')
  static PublicAddress getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PublicAddress>(create);
  static PublicAddress? _defaultInstance;

  /// Coin symbol for the address (a.k.a. tokenCode)
  @$pb.TagNumber(1)
  $core.String get coinSymbol => $_getSZ(0);
  @$pb.TagNumber(1)
  set coinSymbol($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasCoinSymbol() => $_has(0);
  @$pb.TagNumber(1)
  void clearCoinSymbol() => clearField(1);

  /// The address
  @$pb.TagNumber(2)
  $core.String get address => $_getSZ(1);
  @$pb.TagNumber(2)
  set address($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearAddress() => clearField(2);
}

/// Payload content for New Funds requests
class NewFundsContent extends $pb.GeneratedMessage {
  factory NewFundsContent({
    $core.String? payeePublicAddress,
    $core.String? amount,
    $core.String? coinSymbol,
    $core.String? memo,
    $core.String? hash,
    $core.String? offlineUrl,
  }) {
    final $result = create();
    if (payeePublicAddress != null) {
      $result.payeePublicAddress = payeePublicAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (coinSymbol != null) {
      $result.coinSymbol = coinSymbol;
    }
    if (memo != null) {
      $result.memo = memo;
    }
    if (hash != null) {
      $result.hash = hash;
    }
    if (offlineUrl != null) {
      $result.offlineUrl = offlineUrl;
    }
    return $result;
  }
  NewFundsContent._() : super();
  factory NewFundsContent.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory NewFundsContent.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'NewFundsContent', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.FIO.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'payeePublicAddress')
    ..aOS(2, _omitFieldNames ? '' : 'amount')
    ..aOS(3, _omitFieldNames ? '' : 'coinSymbol')
    ..aOS(4, _omitFieldNames ? '' : 'memo')
    ..aOS(5, _omitFieldNames ? '' : 'hash')
    ..aOS(6, _omitFieldNames ? '' : 'offlineUrl')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  NewFundsContent clone() => NewFundsContent()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  NewFundsContent copyWith(void Function(NewFundsContent) updates) => super.copyWith((message) => updates(message as NewFundsContent)) as NewFundsContent;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static NewFundsContent create() => NewFundsContent._();
  NewFundsContent createEmptyInstance() => create();
  static $pb.PbList<NewFundsContent> createRepeated() => $pb.PbList<NewFundsContent>();
  @$core.pragma('dart2js:noInline')
  static NewFundsContent getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<NewFundsContent>(create);
  static NewFundsContent? _defaultInstance;

  /// Public addressed of the payee, on the mentioned blockchain.
  @$pb.TagNumber(1)
  $core.String get payeePublicAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set payeePublicAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPayeePublicAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearPayeePublicAddress() => clearField(1);

  /// Amount requested (as string)
  @$pb.TagNumber(2)
  $core.String get amount => $_getSZ(1);
  @$pb.TagNumber(2)
  set amount($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);

  /// Coin symbol of the amount requested (a.k.a. tokenCode)
  @$pb.TagNumber(3)
  $core.String get coinSymbol => $_getSZ(2);
  @$pb.TagNumber(3)
  set coinSymbol($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasCoinSymbol() => $_has(2);
  @$pb.TagNumber(3)
  void clearCoinSymbol() => clearField(3);

  /// Memo free text.  Optional, may be empty.
  @$pb.TagNumber(4)
  $core.String get memo => $_getSZ(3);
  @$pb.TagNumber(4)
  set memo($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasMemo() => $_has(3);
  @$pb.TagNumber(4)
  void clearMemo() => clearField(4);

  /// Hash.  Optional, may be empty.
  @$pb.TagNumber(5)
  $core.String get hash => $_getSZ(4);
  @$pb.TagNumber(5)
  set hash($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasHash() => $_has(4);
  @$pb.TagNumber(5)
  void clearHash() => clearField(5);

  /// Attached offline URL.  Optional, may be empty.
  @$pb.TagNumber(6)
  $core.String get offlineUrl => $_getSZ(5);
  @$pb.TagNumber(6)
  set offlineUrl($core.String v) { $_setString(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasOfflineUrl() => $_has(5);
  @$pb.TagNumber(6)
  void clearOfflineUrl() => clearField(6);
}

/// Action for registering a FIO name; register_fio_address
class Action_RegisterFioAddress extends $pb.GeneratedMessage {
  factory Action_RegisterFioAddress({
    $core.String? fioAddress,
    $core.String? ownerFioPublicKey,
    $fixnum.Int64? fee,
  }) {
    final $result = create();
    if (fioAddress != null) {
      $result.fioAddress = fioAddress;
    }
    if (ownerFioPublicKey != null) {
      $result.ownerFioPublicKey = ownerFioPublicKey;
    }
    if (fee != null) {
      $result.fee = fee;
    }
    return $result;
  }
  Action_RegisterFioAddress._() : super();
  factory Action_RegisterFioAddress.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Action_RegisterFioAddress.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Action.RegisterFioAddress', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.FIO.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'fioAddress')
    ..aOS(2, _omitFieldNames ? '' : 'ownerFioPublicKey')
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'fee', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Action_RegisterFioAddress clone() => Action_RegisterFioAddress()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Action_RegisterFioAddress copyWith(void Function(Action_RegisterFioAddress) updates) => super.copyWith((message) => updates(message as Action_RegisterFioAddress)) as Action_RegisterFioAddress;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Action_RegisterFioAddress create() => Action_RegisterFioAddress._();
  Action_RegisterFioAddress createEmptyInstance() => create();
  static $pb.PbList<Action_RegisterFioAddress> createRepeated() => $pb.PbList<Action_RegisterFioAddress>();
  @$core.pragma('dart2js:noInline')
  static Action_RegisterFioAddress getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Action_RegisterFioAddress>(create);
  static Action_RegisterFioAddress? _defaultInstance;

  /// The FIO name to be registered. Ex.: "alice@trust"
  @$pb.TagNumber(1)
  $core.String get fioAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set fioAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFioAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearFioAddress() => clearField(1);

  /// FIO address of the owner. Ex.: "FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf"
  @$pb.TagNumber(2)
  $core.String get ownerFioPublicKey => $_getSZ(1);
  @$pb.TagNumber(2)
  set ownerFioPublicKey($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasOwnerFioPublicKey() => $_has(1);
  @$pb.TagNumber(2)
  void clearOwnerFioPublicKey() => clearField(2);

  /// Max fee to spend, can be obtained using get_fee API.
  @$pb.TagNumber(3)
  $fixnum.Int64 get fee => $_getI64(2);
  @$pb.TagNumber(3)
  set fee($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasFee() => $_has(2);
  @$pb.TagNumber(3)
  void clearFee() => clearField(3);
}

/// Acion for adding public chain addresses to a FIO name; add_pub_address
/// Note: actor is not needed, computed from private key
class Action_AddPubAddress extends $pb.GeneratedMessage {
  factory Action_AddPubAddress({
    $core.String? fioAddress,
    $core.Iterable<PublicAddress>? publicAddresses,
    $fixnum.Int64? fee,
  }) {
    final $result = create();
    if (fioAddress != null) {
      $result.fioAddress = fioAddress;
    }
    if (publicAddresses != null) {
      $result.publicAddresses.addAll(publicAddresses);
    }
    if (fee != null) {
      $result.fee = fee;
    }
    return $result;
  }
  Action_AddPubAddress._() : super();
  factory Action_AddPubAddress.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Action_AddPubAddress.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Action.AddPubAddress', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.FIO.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'fioAddress')
    ..pc<PublicAddress>(2, _omitFieldNames ? '' : 'publicAddresses', $pb.PbFieldType.PM, subBuilder: PublicAddress.create)
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'fee', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Action_AddPubAddress clone() => Action_AddPubAddress()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Action_AddPubAddress copyWith(void Function(Action_AddPubAddress) updates) => super.copyWith((message) => updates(message as Action_AddPubAddress)) as Action_AddPubAddress;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Action_AddPubAddress create() => Action_AddPubAddress._();
  Action_AddPubAddress createEmptyInstance() => create();
  static $pb.PbList<Action_AddPubAddress> createRepeated() => $pb.PbList<Action_AddPubAddress>();
  @$core.pragma('dart2js:noInline')
  static Action_AddPubAddress getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Action_AddPubAddress>(create);
  static Action_AddPubAddress? _defaultInstance;

  /// The FIO name already registered to the owner. Ex.: "alice@trust"
  @$pb.TagNumber(1)
  $core.String get fioAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set fioAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFioAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearFioAddress() => clearField(1);

  /// List of public addresses to be registered, ex. {{"BTC", "bc1qv...7v"},{"BNB", "bnb1ts3...9s"}}
  @$pb.TagNumber(2)
  $core.List<PublicAddress> get publicAddresses => $_getList(1);

  /// Max fee to spend, can be obtained using get_fee API.
  @$pb.TagNumber(3)
  $fixnum.Int64 get fee => $_getI64(2);
  @$pb.TagNumber(3)
  set fee($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasFee() => $_has(2);
  @$pb.TagNumber(3)
  void clearFee() => clearField(3);
}

/// Action for removing public chain addresses from a FIO name; remove_pub_address
/// Note: actor is not needed, computed from private key
class Action_RemovePubAddress extends $pb.GeneratedMessage {
  factory Action_RemovePubAddress({
    $core.String? fioAddress,
    $core.Iterable<PublicAddress>? publicAddresses,
    $fixnum.Int64? fee,
  }) {
    final $result = create();
    if (fioAddress != null) {
      $result.fioAddress = fioAddress;
    }
    if (publicAddresses != null) {
      $result.publicAddresses.addAll(publicAddresses);
    }
    if (fee != null) {
      $result.fee = fee;
    }
    return $result;
  }
  Action_RemovePubAddress._() : super();
  factory Action_RemovePubAddress.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Action_RemovePubAddress.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Action.RemovePubAddress', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.FIO.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'fioAddress')
    ..pc<PublicAddress>(2, _omitFieldNames ? '' : 'publicAddresses', $pb.PbFieldType.PM, subBuilder: PublicAddress.create)
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'fee', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Action_RemovePubAddress clone() => Action_RemovePubAddress()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Action_RemovePubAddress copyWith(void Function(Action_RemovePubAddress) updates) => super.copyWith((message) => updates(message as Action_RemovePubAddress)) as Action_RemovePubAddress;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Action_RemovePubAddress create() => Action_RemovePubAddress._();
  Action_RemovePubAddress createEmptyInstance() => create();
  static $pb.PbList<Action_RemovePubAddress> createRepeated() => $pb.PbList<Action_RemovePubAddress>();
  @$core.pragma('dart2js:noInline')
  static Action_RemovePubAddress getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Action_RemovePubAddress>(create);
  static Action_RemovePubAddress? _defaultInstance;

  /// The FIO name already registered to the owner. Ex.: "alice@trust"
  @$pb.TagNumber(1)
  $core.String get fioAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set fioAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFioAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearFioAddress() => clearField(1);

  /// List of public addresses to be unregistered, ex. {{"BTC", "bc1qv...7v"},{"BNB", "bnb1ts3...9s"}}
  @$pb.TagNumber(2)
  $core.List<PublicAddress> get publicAddresses => $_getList(1);

  /// Max fee to spend, can be obtained using get_fee API.
  @$pb.TagNumber(3)
  $fixnum.Int64 get fee => $_getI64(2);
  @$pb.TagNumber(3)
  set fee($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasFee() => $_has(2);
  @$pb.TagNumber(3)
  void clearFee() => clearField(3);
}

/// Action for removing public chain addresses from a FIO name; remove_pub_address
/// Note: actor is not needed, computed from private key
class Action_RemoveAllPubAddress extends $pb.GeneratedMessage {
  factory Action_RemoveAllPubAddress({
    $core.String? fioAddress,
    $fixnum.Int64? fee,
  }) {
    final $result = create();
    if (fioAddress != null) {
      $result.fioAddress = fioAddress;
    }
    if (fee != null) {
      $result.fee = fee;
    }
    return $result;
  }
  Action_RemoveAllPubAddress._() : super();
  factory Action_RemoveAllPubAddress.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Action_RemoveAllPubAddress.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Action.RemoveAllPubAddress', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.FIO.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'fioAddress')
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'fee', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Action_RemoveAllPubAddress clone() => Action_RemoveAllPubAddress()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Action_RemoveAllPubAddress copyWith(void Function(Action_RemoveAllPubAddress) updates) => super.copyWith((message) => updates(message as Action_RemoveAllPubAddress)) as Action_RemoveAllPubAddress;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Action_RemoveAllPubAddress create() => Action_RemoveAllPubAddress._();
  Action_RemoveAllPubAddress createEmptyInstance() => create();
  static $pb.PbList<Action_RemoveAllPubAddress> createRepeated() => $pb.PbList<Action_RemoveAllPubAddress>();
  @$core.pragma('dart2js:noInline')
  static Action_RemoveAllPubAddress getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Action_RemoveAllPubAddress>(create);
  static Action_RemoveAllPubAddress? _defaultInstance;

  /// The FIO name already registered to the owner. Ex.: "alice@trust"
  @$pb.TagNumber(1)
  $core.String get fioAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set fioAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFioAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearFioAddress() => clearField(1);

  /// Max fee to spend, can be obtained using get_fee API.
  @$pb.TagNumber(3)
  $fixnum.Int64 get fee => $_getI64(1);
  @$pb.TagNumber(3)
  set fee($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(3)
  $core.bool hasFee() => $_has(1);
  @$pb.TagNumber(3)
  void clearFee() => clearField(3);
}

/// Action for transferring FIO coins; transfer_tokens_pub_key
/// Note: actor is not needed, computed from private key
class Action_Transfer extends $pb.GeneratedMessage {
  factory Action_Transfer({
    $core.String? payeePublicKey,
    $fixnum.Int64? amount,
    $fixnum.Int64? fee,
  }) {
    final $result = create();
    if (payeePublicKey != null) {
      $result.payeePublicKey = payeePublicKey;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (fee != null) {
      $result.fee = fee;
    }
    return $result;
  }
  Action_Transfer._() : super();
  factory Action_Transfer.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Action_Transfer.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Action.Transfer', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.FIO.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'payeePublicKey')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'fee', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Action_Transfer clone() => Action_Transfer()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Action_Transfer copyWith(void Function(Action_Transfer) updates) => super.copyWith((message) => updates(message as Action_Transfer)) as Action_Transfer;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Action_Transfer create() => Action_Transfer._();
  Action_Transfer createEmptyInstance() => create();
  static $pb.PbList<Action_Transfer> createRepeated() => $pb.PbList<Action_Transfer>();
  @$core.pragma('dart2js:noInline')
  static Action_Transfer getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Action_Transfer>(create);
  static Action_Transfer? _defaultInstance;

  /// FIO address of the payee. Ex.: "FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf"
  @$pb.TagNumber(1)
  $core.String get payeePublicKey => $_getSZ(0);
  @$pb.TagNumber(1)
  set payeePublicKey($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPayeePublicKey() => $_has(0);
  @$pb.TagNumber(1)
  void clearPayeePublicKey() => clearField(1);

  /// Amount of FIO coins to be transferred.
  @$pb.TagNumber(2)
  $fixnum.Int64 get amount => $_getI64(1);
  @$pb.TagNumber(2)
  set amount($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);

  /// Max fee to spend, can be obtained using get_fee API.
  @$pb.TagNumber(3)
  $fixnum.Int64 get fee => $_getI64(2);
  @$pb.TagNumber(3)
  set fee($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasFee() => $_has(2);
  @$pb.TagNumber(3)
  void clearFee() => clearField(3);
}

/// Action for renewing a FIO name; renew_fio_address
/// Note: actor is not needed, computed from private key
class Action_RenewFioAddress extends $pb.GeneratedMessage {
  factory Action_RenewFioAddress({
    $core.String? fioAddress,
    $core.String? ownerFioPublicKey,
    $fixnum.Int64? fee,
  }) {
    final $result = create();
    if (fioAddress != null) {
      $result.fioAddress = fioAddress;
    }
    if (ownerFioPublicKey != null) {
      $result.ownerFioPublicKey = ownerFioPublicKey;
    }
    if (fee != null) {
      $result.fee = fee;
    }
    return $result;
  }
  Action_RenewFioAddress._() : super();
  factory Action_RenewFioAddress.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Action_RenewFioAddress.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Action.RenewFioAddress', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.FIO.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'fioAddress')
    ..aOS(2, _omitFieldNames ? '' : 'ownerFioPublicKey')
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'fee', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Action_RenewFioAddress clone() => Action_RenewFioAddress()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Action_RenewFioAddress copyWith(void Function(Action_RenewFioAddress) updates) => super.copyWith((message) => updates(message as Action_RenewFioAddress)) as Action_RenewFioAddress;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Action_RenewFioAddress create() => Action_RenewFioAddress._();
  Action_RenewFioAddress createEmptyInstance() => create();
  static $pb.PbList<Action_RenewFioAddress> createRepeated() => $pb.PbList<Action_RenewFioAddress>();
  @$core.pragma('dart2js:noInline')
  static Action_RenewFioAddress getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Action_RenewFioAddress>(create);
  static Action_RenewFioAddress? _defaultInstance;

  /// The FIO name to be renewed. Ex.: "alice@trust"
  @$pb.TagNumber(1)
  $core.String get fioAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set fioAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFioAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearFioAddress() => clearField(1);

  /// FIO address of the owner. Ex.: "FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf"
  @$pb.TagNumber(2)
  $core.String get ownerFioPublicKey => $_getSZ(1);
  @$pb.TagNumber(2)
  set ownerFioPublicKey($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasOwnerFioPublicKey() => $_has(1);
  @$pb.TagNumber(2)
  void clearOwnerFioPublicKey() => clearField(2);

  /// Max fee to spend, can be obtained using get_fee API.
  @$pb.TagNumber(3)
  $fixnum.Int64 get fee => $_getI64(2);
  @$pb.TagNumber(3)
  set fee($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasFee() => $_has(2);
  @$pb.TagNumber(3)
  void clearFee() => clearField(3);
}

/// Action for creating a new payment request; new_funds_request
/// Note: actor is not needed, computed from private key
class Action_NewFundsRequest extends $pb.GeneratedMessage {
  factory Action_NewFundsRequest({
    $core.String? payerFioName,
    $core.String? payerFioAddress,
    $core.String? payeeFioName,
    NewFundsContent? content,
    $fixnum.Int64? fee,
  }) {
    final $result = create();
    if (payerFioName != null) {
      $result.payerFioName = payerFioName;
    }
    if (payerFioAddress != null) {
      $result.payerFioAddress = payerFioAddress;
    }
    if (payeeFioName != null) {
      $result.payeeFioName = payeeFioName;
    }
    if (content != null) {
      $result.content = content;
    }
    if (fee != null) {
      $result.fee = fee;
    }
    return $result;
  }
  Action_NewFundsRequest._() : super();
  factory Action_NewFundsRequest.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Action_NewFundsRequest.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Action.NewFundsRequest', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.FIO.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'payerFioName')
    ..aOS(2, _omitFieldNames ? '' : 'payerFioAddress')
    ..aOS(3, _omitFieldNames ? '' : 'payeeFioName')
    ..aOM<NewFundsContent>(4, _omitFieldNames ? '' : 'content', subBuilder: NewFundsContent.create)
    ..a<$fixnum.Int64>(5, _omitFieldNames ? '' : 'fee', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Action_NewFundsRequest clone() => Action_NewFundsRequest()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Action_NewFundsRequest copyWith(void Function(Action_NewFundsRequest) updates) => super.copyWith((message) => updates(message as Action_NewFundsRequest)) as Action_NewFundsRequest;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Action_NewFundsRequest create() => Action_NewFundsRequest._();
  Action_NewFundsRequest createEmptyInstance() => create();
  static $pb.PbList<Action_NewFundsRequest> createRepeated() => $pb.PbList<Action_NewFundsRequest>();
  @$core.pragma('dart2js:noInline')
  static Action_NewFundsRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Action_NewFundsRequest>(create);
  static Action_NewFundsRequest? _defaultInstance;

  /// The FIO name of the requested payer. Ex.: "alice@trust"
  @$pb.TagNumber(1)
  $core.String get payerFioName => $_getSZ(0);
  @$pb.TagNumber(1)
  set payerFioName($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPayerFioName() => $_has(0);
  @$pb.TagNumber(1)
  void clearPayerFioName() => clearField(1);

  /// The FIO address (not name) of the payer, owner of payee_fio_name.  Ex.: "FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf"
  @$pb.TagNumber(2)
  $core.String get payerFioAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set payerFioAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPayerFioAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearPayerFioAddress() => clearField(2);

  /// Own FIO name.  Ex.: "bob@trust"
  @$pb.TagNumber(3)
  $core.String get payeeFioName => $_getSZ(2);
  @$pb.TagNumber(3)
  set payeeFioName($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasPayeeFioName() => $_has(2);
  @$pb.TagNumber(3)
  void clearPayeeFioName() => clearField(3);

  /// Payload content of the request
  @$pb.TagNumber(4)
  NewFundsContent get content => $_getN(3);
  @$pb.TagNumber(4)
  set content(NewFundsContent v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasContent() => $_has(3);
  @$pb.TagNumber(4)
  void clearContent() => clearField(4);
  @$pb.TagNumber(4)
  NewFundsContent ensureContent() => $_ensure(3);

  /// Max fee to spend, can be obtained using get_fee API.
  @$pb.TagNumber(5)
  $fixnum.Int64 get fee => $_getI64(4);
  @$pb.TagNumber(5)
  set fee($fixnum.Int64 v) { $_setInt64(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasFee() => $_has(4);
  @$pb.TagNumber(5)
  void clearFee() => clearField(5);
}

/// Action for adding `100 * bundle_sets` bundled transactions to the supplied FIO Handle. When bundles are purchased one or more sets of bundled transactions are added to the existing count.
class Action_AddBundledTransactions extends $pb.GeneratedMessage {
  factory Action_AddBundledTransactions({
    $core.String? fioAddress,
    $fixnum.Int64? bundleSets,
    $fixnum.Int64? fee,
  }) {
    final $result = create();
    if (fioAddress != null) {
      $result.fioAddress = fioAddress;
    }
    if (bundleSets != null) {
      $result.bundleSets = bundleSets;
    }
    if (fee != null) {
      $result.fee = fee;
    }
    return $result;
  }
  Action_AddBundledTransactions._() : super();
  factory Action_AddBundledTransactions.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Action_AddBundledTransactions.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Action.AddBundledTransactions', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.FIO.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'fioAddress')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'bundleSets', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'fee', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Action_AddBundledTransactions clone() => Action_AddBundledTransactions()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Action_AddBundledTransactions copyWith(void Function(Action_AddBundledTransactions) updates) => super.copyWith((message) => updates(message as Action_AddBundledTransactions)) as Action_AddBundledTransactions;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Action_AddBundledTransactions create() => Action_AddBundledTransactions._();
  Action_AddBundledTransactions createEmptyInstance() => create();
  static $pb.PbList<Action_AddBundledTransactions> createRepeated() => $pb.PbList<Action_AddBundledTransactions>();
  @$core.pragma('dart2js:noInline')
  static Action_AddBundledTransactions getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Action_AddBundledTransactions>(create);
  static Action_AddBundledTransactions? _defaultInstance;

  /// The FIO name already registered to the owner. Ex.: "alice@trust"
  @$pb.TagNumber(1)
  $core.String get fioAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set fioAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFioAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearFioAddress() => clearField(1);

  /// Number of bundled sets. One set is 100 bundled transactions.
  @$pb.TagNumber(2)
  $fixnum.Int64 get bundleSets => $_getI64(1);
  @$pb.TagNumber(2)
  set bundleSets($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasBundleSets() => $_has(1);
  @$pb.TagNumber(2)
  void clearBundleSets() => clearField(2);

  /// Max fee to spend, can be obtained using get_fee API.
  @$pb.TagNumber(3)
  $fixnum.Int64 get fee => $_getI64(2);
  @$pb.TagNumber(3)
  set fee($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasFee() => $_has(2);
  @$pb.TagNumber(3)
  void clearFee() => clearField(3);
}

enum Action_MessageOneof {
  registerFioAddressMessage, 
  addPubAddressMessage, 
  transferMessage, 
  renewFioAddressMessage, 
  newFundsRequestMessage, 
  removePubAddressMessage, 
  removeAllPubAddressesMessage, 
  addBundledTransactionsMessage, 
  notSet
}

/// Different Actions
class Action extends $pb.GeneratedMessage {
  factory Action({
    Action_RegisterFioAddress? registerFioAddressMessage,
    Action_AddPubAddress? addPubAddressMessage,
    Action_Transfer? transferMessage,
    Action_RenewFioAddress? renewFioAddressMessage,
    Action_NewFundsRequest? newFundsRequestMessage,
    Action_RemovePubAddress? removePubAddressMessage,
    Action_RemoveAllPubAddress? removeAllPubAddressesMessage,
    Action_AddBundledTransactions? addBundledTransactionsMessage,
  }) {
    final $result = create();
    if (registerFioAddressMessage != null) {
      $result.registerFioAddressMessage = registerFioAddressMessage;
    }
    if (addPubAddressMessage != null) {
      $result.addPubAddressMessage = addPubAddressMessage;
    }
    if (transferMessage != null) {
      $result.transferMessage = transferMessage;
    }
    if (renewFioAddressMessage != null) {
      $result.renewFioAddressMessage = renewFioAddressMessage;
    }
    if (newFundsRequestMessage != null) {
      $result.newFundsRequestMessage = newFundsRequestMessage;
    }
    if (removePubAddressMessage != null) {
      $result.removePubAddressMessage = removePubAddressMessage;
    }
    if (removeAllPubAddressesMessage != null) {
      $result.removeAllPubAddressesMessage = removeAllPubAddressesMessage;
    }
    if (addBundledTransactionsMessage != null) {
      $result.addBundledTransactionsMessage = addBundledTransactionsMessage;
    }
    return $result;
  }
  Action._() : super();
  factory Action.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Action.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Action_MessageOneof> _Action_MessageOneofByTag = {
    1 : Action_MessageOneof.registerFioAddressMessage,
    2 : Action_MessageOneof.addPubAddressMessage,
    3 : Action_MessageOneof.transferMessage,
    4 : Action_MessageOneof.renewFioAddressMessage,
    5 : Action_MessageOneof.newFundsRequestMessage,
    6 : Action_MessageOneof.removePubAddressMessage,
    7 : Action_MessageOneof.removeAllPubAddressesMessage,
    8 : Action_MessageOneof.addBundledTransactionsMessage,
    0 : Action_MessageOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Action', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.FIO.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2, 3, 4, 5, 6, 7, 8])
    ..aOM<Action_RegisterFioAddress>(1, _omitFieldNames ? '' : 'registerFioAddressMessage', subBuilder: Action_RegisterFioAddress.create)
    ..aOM<Action_AddPubAddress>(2, _omitFieldNames ? '' : 'addPubAddressMessage', subBuilder: Action_AddPubAddress.create)
    ..aOM<Action_Transfer>(3, _omitFieldNames ? '' : 'transferMessage', subBuilder: Action_Transfer.create)
    ..aOM<Action_RenewFioAddress>(4, _omitFieldNames ? '' : 'renewFioAddressMessage', subBuilder: Action_RenewFioAddress.create)
    ..aOM<Action_NewFundsRequest>(5, _omitFieldNames ? '' : 'newFundsRequestMessage', subBuilder: Action_NewFundsRequest.create)
    ..aOM<Action_RemovePubAddress>(6, _omitFieldNames ? '' : 'removePubAddressMessage', subBuilder: Action_RemovePubAddress.create)
    ..aOM<Action_RemoveAllPubAddress>(7, _omitFieldNames ? '' : 'removeAllPubAddressesMessage', subBuilder: Action_RemoveAllPubAddress.create)
    ..aOM<Action_AddBundledTransactions>(8, _omitFieldNames ? '' : 'addBundledTransactionsMessage', subBuilder: Action_AddBundledTransactions.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Action clone() => Action()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Action copyWith(void Function(Action) updates) => super.copyWith((message) => updates(message as Action)) as Action;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Action create() => Action._();
  Action createEmptyInstance() => create();
  static $pb.PbList<Action> createRepeated() => $pb.PbList<Action>();
  @$core.pragma('dart2js:noInline')
  static Action getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Action>(create);
  static Action? _defaultInstance;

  Action_MessageOneof whichMessageOneof() => _Action_MessageOneofByTag[$_whichOneof(0)]!;
  void clearMessageOneof() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  Action_RegisterFioAddress get registerFioAddressMessage => $_getN(0);
  @$pb.TagNumber(1)
  set registerFioAddressMessage(Action_RegisterFioAddress v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasRegisterFioAddressMessage() => $_has(0);
  @$pb.TagNumber(1)
  void clearRegisterFioAddressMessage() => clearField(1);
  @$pb.TagNumber(1)
  Action_RegisterFioAddress ensureRegisterFioAddressMessage() => $_ensure(0);

  @$pb.TagNumber(2)
  Action_AddPubAddress get addPubAddressMessage => $_getN(1);
  @$pb.TagNumber(2)
  set addPubAddressMessage(Action_AddPubAddress v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasAddPubAddressMessage() => $_has(1);
  @$pb.TagNumber(2)
  void clearAddPubAddressMessage() => clearField(2);
  @$pb.TagNumber(2)
  Action_AddPubAddress ensureAddPubAddressMessage() => $_ensure(1);

  @$pb.TagNumber(3)
  Action_Transfer get transferMessage => $_getN(2);
  @$pb.TagNumber(3)
  set transferMessage(Action_Transfer v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasTransferMessage() => $_has(2);
  @$pb.TagNumber(3)
  void clearTransferMessage() => clearField(3);
  @$pb.TagNumber(3)
  Action_Transfer ensureTransferMessage() => $_ensure(2);

  @$pb.TagNumber(4)
  Action_RenewFioAddress get renewFioAddressMessage => $_getN(3);
  @$pb.TagNumber(4)
  set renewFioAddressMessage(Action_RenewFioAddress v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasRenewFioAddressMessage() => $_has(3);
  @$pb.TagNumber(4)
  void clearRenewFioAddressMessage() => clearField(4);
  @$pb.TagNumber(4)
  Action_RenewFioAddress ensureRenewFioAddressMessage() => $_ensure(3);

  @$pb.TagNumber(5)
  Action_NewFundsRequest get newFundsRequestMessage => $_getN(4);
  @$pb.TagNumber(5)
  set newFundsRequestMessage(Action_NewFundsRequest v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasNewFundsRequestMessage() => $_has(4);
  @$pb.TagNumber(5)
  void clearNewFundsRequestMessage() => clearField(5);
  @$pb.TagNumber(5)
  Action_NewFundsRequest ensureNewFundsRequestMessage() => $_ensure(4);

  @$pb.TagNumber(6)
  Action_RemovePubAddress get removePubAddressMessage => $_getN(5);
  @$pb.TagNumber(6)
  set removePubAddressMessage(Action_RemovePubAddress v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasRemovePubAddressMessage() => $_has(5);
  @$pb.TagNumber(6)
  void clearRemovePubAddressMessage() => clearField(6);
  @$pb.TagNumber(6)
  Action_RemovePubAddress ensureRemovePubAddressMessage() => $_ensure(5);

  @$pb.TagNumber(7)
  Action_RemoveAllPubAddress get removeAllPubAddressesMessage => $_getN(6);
  @$pb.TagNumber(7)
  set removeAllPubAddressesMessage(Action_RemoveAllPubAddress v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasRemoveAllPubAddressesMessage() => $_has(6);
  @$pb.TagNumber(7)
  void clearRemoveAllPubAddressesMessage() => clearField(7);
  @$pb.TagNumber(7)
  Action_RemoveAllPubAddress ensureRemoveAllPubAddressesMessage() => $_ensure(6);

  @$pb.TagNumber(8)
  Action_AddBundledTransactions get addBundledTransactionsMessage => $_getN(7);
  @$pb.TagNumber(8)
  set addBundledTransactionsMessage(Action_AddBundledTransactions v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasAddBundledTransactionsMessage() => $_has(7);
  @$pb.TagNumber(8)
  void clearAddBundledTransactionsMessage() => clearField(8);
  @$pb.TagNumber(8)
  Action_AddBundledTransactions ensureAddBundledTransactionsMessage() => $_ensure(7);
}

/// Represents current parameters of the FIO blockchain
class ChainParams extends $pb.GeneratedMessage {
  factory ChainParams({
    $core.List<$core.int>? chainId,
    $fixnum.Int64? headBlockNumber,
    $fixnum.Int64? refBlockPrefix,
  }) {
    final $result = create();
    if (chainId != null) {
      $result.chainId = chainId;
    }
    if (headBlockNumber != null) {
      $result.headBlockNumber = headBlockNumber;
    }
    if (refBlockPrefix != null) {
      $result.refBlockPrefix = refBlockPrefix;
    }
    return $result;
  }
  ChainParams._() : super();
  factory ChainParams.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ChainParams.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'ChainParams', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.FIO.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'chainId', $pb.PbFieldType.OY)
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'headBlockNumber', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'refBlockPrefix', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ChainParams clone() => ChainParams()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ChainParams copyWith(void Function(ChainParams) updates) => super.copyWith((message) => updates(message as ChainParams)) as ChainParams;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static ChainParams create() => ChainParams._();
  ChainParams createEmptyInstance() => create();
  static $pb.PbList<ChainParams> createRepeated() => $pb.PbList<ChainParams>();
  @$core.pragma('dart2js:noInline')
  static ChainParams getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ChainParams>(create);
  static ChainParams? _defaultInstance;

  /// Constant chainId (32 bytes), obtained from get_info API
  @$pb.TagNumber(1)
  $core.List<$core.int> get chainId => $_getN(0);
  @$pb.TagNumber(1)
  set chainId($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasChainId() => $_has(0);
  @$pb.TagNumber(1)
  void clearChainId() => clearField(1);

  /// The last block number, obtained from get_info API
  @$pb.TagNumber(2)
  $fixnum.Int64 get headBlockNumber => $_getI64(1);
  @$pb.TagNumber(2)
  set headBlockNumber($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasHeadBlockNumber() => $_has(1);
  @$pb.TagNumber(2)
  void clearHeadBlockNumber() => clearField(2);

  /// Block prefix of last block, obtained from get_block API
  @$pb.TagNumber(3)
  $fixnum.Int64 get refBlockPrefix => $_getI64(2);
  @$pb.TagNumber(3)
  set refBlockPrefix($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasRefBlockPrefix() => $_has(2);
  @$pb.TagNumber(3)
  void clearRefBlockPrefix() => clearField(3);
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.int? expiry,
    ChainParams? chainParams,
    $core.List<$core.int>? privateKey,
    $core.String? tpid,
    Action? action,
    $core.String? ownerPublicKey,
  }) {
    final $result = create();
    if (expiry != null) {
      $result.expiry = expiry;
    }
    if (chainParams != null) {
      $result.chainParams = chainParams;
    }
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (tpid != null) {
      $result.tpid = tpid;
    }
    if (action != null) {
      $result.action = action;
    }
    if (ownerPublicKey != null) {
      $result.ownerPublicKey = ownerPublicKey;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.FIO.Proto'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'expiry', $pb.PbFieldType.OU3)
    ..aOM<ChainParams>(2, _omitFieldNames ? '' : 'chainParams', subBuilder: ChainParams.create)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..aOS(4, _omitFieldNames ? '' : 'tpid')
    ..aOM<Action>(5, _omitFieldNames ? '' : 'action', subBuilder: Action.create)
    ..aOS(6, _omitFieldNames ? '' : 'ownerPublicKey')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SigningInput clone() => SigningInput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SigningInput copyWith(void Function(SigningInput) updates) => super.copyWith((message) => updates(message as SigningInput)) as SigningInput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SigningInput create() => SigningInput._();
  SigningInput createEmptyInstance() => create();
  static $pb.PbList<SigningInput> createRepeated() => $pb.PbList<SigningInput>();
  @$core.pragma('dart2js:noInline')
  static SigningInput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SigningInput>(create);
  static SigningInput? _defaultInstance;

  /// Expiry for this message, in unix time. Can be 0, then it is taken from current time with default expiry
  @$pb.TagNumber(1)
  $core.int get expiry => $_getIZ(0);
  @$pb.TagNumber(1)
  set expiry($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasExpiry() => $_has(0);
  @$pb.TagNumber(1)
  void clearExpiry() => clearField(1);

  /// Current parameters of the FIO blockchain
  @$pb.TagNumber(2)
  ChainParams get chainParams => $_getN(1);
  @$pb.TagNumber(2)
  set chainParams(ChainParams v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasChainParams() => $_has(1);
  @$pb.TagNumber(2)
  void clearChainParams() => clearField(2);
  @$pb.TagNumber(2)
  ChainParams ensureChainParams() => $_ensure(1);

  /// The secret private key matching the address, used for signing (32 bytes).
  @$pb.TagNumber(3)
  $core.List<$core.int> get privateKey => $_getN(2);
  @$pb.TagNumber(3)
  set privateKey($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasPrivateKey() => $_has(2);
  @$pb.TagNumber(3)
  void clearPrivateKey() => clearField(3);

  /// The FIO name of the originating wallet (project-wide constant)
  @$pb.TagNumber(4)
  $core.String get tpid => $_getSZ(3);
  @$pb.TagNumber(4)
  set tpid($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasTpid() => $_has(3);
  @$pb.TagNumber(4)
  void clearTpid() => clearField(4);

  /// Context-specific action data
  @$pb.TagNumber(5)
  Action get action => $_getN(4);
  @$pb.TagNumber(5)
  set action(Action v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasAction() => $_has(4);
  @$pb.TagNumber(5)
  void clearAction() => clearField(5);
  @$pb.TagNumber(5)
  Action ensureAction() => $_ensure(4);

  /// FIO address of the owner. Ex.: "FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf"
  @$pb.TagNumber(6)
  $core.String get ownerPublicKey => $_getSZ(5);
  @$pb.TagNumber(6)
  set ownerPublicKey($core.String v) { $_setString(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasOwnerPublicKey() => $_has(5);
  @$pb.TagNumber(6)
  void clearOwnerPublicKey() => clearField(6);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.String? json,
    $0.SigningError? error,
    $core.String? errorMessage,
    $core.String? actionName,
  }) {
    final $result = create();
    if (json != null) {
      $result.json = json;
    }
    if (error != null) {
      $result.error = error;
    }
    if (errorMessage != null) {
      $result.errorMessage = errorMessage;
    }
    if (actionName != null) {
      $result.actionName = actionName;
    }
    return $result;
  }
  SigningOutput._() : super();
  factory SigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.FIO.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'json')
    ..e<$0.SigningError>(2, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(3, _omitFieldNames ? '' : 'errorMessage')
    ..aOS(4, _omitFieldNames ? '' : 'actionName')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SigningOutput clone() => SigningOutput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SigningOutput copyWith(void Function(SigningOutput) updates) => super.copyWith((message) => updates(message as SigningOutput)) as SigningOutput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SigningOutput create() => SigningOutput._();
  SigningOutput createEmptyInstance() => create();
  static $pb.PbList<SigningOutput> createRepeated() => $pb.PbList<SigningOutput>();
  @$core.pragma('dart2js:noInline')
  static SigningOutput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SigningOutput>(create);
  static SigningOutput? _defaultInstance;

  /// Signed transaction in JSON
  @$pb.TagNumber(1)
  $core.String get json => $_getSZ(0);
  @$pb.TagNumber(1)
  set json($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasJson() => $_has(0);
  @$pb.TagNumber(1)
  void clearJson() => clearField(1);

  /// Optional error
  @$pb.TagNumber(2)
  $0.SigningError get error => $_getN(1);
  @$pb.TagNumber(2)
  set error($0.SigningError v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasError() => $_has(1);
  @$pb.TagNumber(2)
  void clearError() => clearField(2);

  /// error code description
  @$pb.TagNumber(3)
  $core.String get errorMessage => $_getSZ(2);
  @$pb.TagNumber(3)
  set errorMessage($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasErrorMessage() => $_has(2);
  @$pb.TagNumber(3)
  void clearErrorMessage() => clearField(3);

  /// Performed action name, ex. "addaddress", "remaddress", "trnsfiopubky" etc.
  @$pb.TagNumber(4)
  $core.String get actionName => $_getSZ(3);
  @$pb.TagNumber(4)
  set actionName($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasActionName() => $_has(3);
  @$pb.TagNumber(4)
  void clearActionName() => clearField(4);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
