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

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'BitcoinV2.pbenum.dart';
import 'Common.pbenum.dart' as $0;

export 'BitcoinV2.pbenum.dart';

enum PublicKeyOrHash_Variant {
  pubkey, 
  hash, 
  notSet
}

/// Either a public key or public key hash.
class PublicKeyOrHash extends $pb.GeneratedMessage {
  factory PublicKeyOrHash({
    $core.List<$core.int>? pubkey,
    $core.List<$core.int>? hash,
  }) {
    final $result = create();
    if (pubkey != null) {
      $result.pubkey = pubkey;
    }
    if (hash != null) {
      $result.hash = hash;
    }
    return $result;
  }
  PublicKeyOrHash._() : super();
  factory PublicKeyOrHash.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PublicKeyOrHash.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, PublicKeyOrHash_Variant> _PublicKeyOrHash_VariantByTag = {
    1 : PublicKeyOrHash_Variant.pubkey,
    2 : PublicKeyOrHash_Variant.hash,
    0 : PublicKeyOrHash_Variant.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'PublicKeyOrHash', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2])
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'pubkey', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'hash', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  PublicKeyOrHash clone() => PublicKeyOrHash()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  PublicKeyOrHash copyWith(void Function(PublicKeyOrHash) updates) => super.copyWith((message) => updates(message as PublicKeyOrHash)) as PublicKeyOrHash;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static PublicKeyOrHash create() => PublicKeyOrHash._();
  PublicKeyOrHash createEmptyInstance() => create();
  static $pb.PbList<PublicKeyOrHash> createRepeated() => $pb.PbList<PublicKeyOrHash>();
  @$core.pragma('dart2js:noInline')
  static PublicKeyOrHash getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PublicKeyOrHash>(create);
  static PublicKeyOrHash? _defaultInstance;

  PublicKeyOrHash_Variant whichVariant() => _PublicKeyOrHash_VariantByTag[$_whichOneof(0)]!;
  void clearVariant() => clearField($_whichOneof(0));

  /// Public key bytes.
  @$pb.TagNumber(1)
  $core.List<$core.int> get pubkey => $_getN(0);
  @$pb.TagNumber(1)
  set pubkey($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPubkey() => $_has(0);
  @$pb.TagNumber(1)
  void clearPubkey() => clearField(1);

  /// Public key hash.
  @$pb.TagNumber(2)
  $core.List<$core.int> get hash => $_getN(1);
  @$pb.TagNumber(2)
  set hash($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasHash() => $_has(1);
  @$pb.TagNumber(2)
  void clearHash() => clearField(2);
}

/// Bitcoin transaction out-point reference.
class OutPoint extends $pb.GeneratedMessage {
  factory OutPoint({
    $core.List<$core.int>? hash,
    $core.int? vout,
  }) {
    final $result = create();
    if (hash != null) {
      $result.hash = hash;
    }
    if (vout != null) {
      $result.vout = vout;
    }
    return $result;
  }
  OutPoint._() : super();
  factory OutPoint.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory OutPoint.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'OutPoint', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'hash', $pb.PbFieldType.OY)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'vout', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  OutPoint clone() => OutPoint()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  OutPoint copyWith(void Function(OutPoint) updates) => super.copyWith((message) => updates(message as OutPoint)) as OutPoint;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static OutPoint create() => OutPoint._();
  OutPoint createEmptyInstance() => create();
  static $pb.PbList<OutPoint> createRepeated() => $pb.PbList<OutPoint>();
  @$core.pragma('dart2js:noInline')
  static OutPoint getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<OutPoint>(create);
  static OutPoint? _defaultInstance;

  /// The hash of the referenced transaction (network byte order, usually needs to be reversed).
  /// The referenced transaction ID in REVERSED order.
  @$pb.TagNumber(1)
  $core.List<$core.int> get hash => $_getN(0);
  @$pb.TagNumber(1)
  set hash($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasHash() => $_has(0);
  @$pb.TagNumber(1)
  void clearHash() => clearField(1);

  /// The position in the previous transactions output that this input references.
  @$pb.TagNumber(2)
  $core.int get vout => $_getIZ(1);
  @$pb.TagNumber(2)
  set vout($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasVout() => $_has(1);
  @$pb.TagNumber(2)
  void clearVout() => clearField(2);
}

/// Optional sequence number, used for timelocks, replace-by-fee, etc.
class Input_Sequence extends $pb.GeneratedMessage {
  factory Input_Sequence({
    $core.int? sequence,
  }) {
    final $result = create();
    if (sequence != null) {
      $result.sequence = sequence;
    }
    return $result;
  }
  Input_Sequence._() : super();
  factory Input_Sequence.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Input_Sequence.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Input.Sequence', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'sequence', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Input_Sequence clone() => Input_Sequence()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Input_Sequence copyWith(void Function(Input_Sequence) updates) => super.copyWith((message) => updates(message as Input_Sequence)) as Input_Sequence;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Input_Sequence create() => Input_Sequence._();
  Input_Sequence createEmptyInstance() => create();
  static $pb.PbList<Input_Sequence> createRepeated() => $pb.PbList<Input_Sequence>();
  @$core.pragma('dart2js:noInline')
  static Input_Sequence getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Input_Sequence>(create);
  static Input_Sequence? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get sequence => $_getIZ(0);
  @$pb.TagNumber(1)
  set sequence($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSequence() => $_has(0);
  @$pb.TagNumber(1)
  void clearSequence() => clearField(1);
}

enum Input_InputBuilder_Variant {
  p2pk, 
  p2pkh, 
  p2wpkh, 
  p2trKeyPath, 
  brc20Inscribe, 
  notSet
}

class Input_InputBuilder extends $pb.GeneratedMessage {
  factory Input_InputBuilder({
    $core.List<$core.int>? p2pk,
    PublicKeyOrHash? p2pkh,
    PublicKeyOrHash? p2wpkh,
    $core.List<$core.int>? p2trKeyPath,
    Input_InputBrc20Inscription? brc20Inscribe,
  }) {
    final $result = create();
    if (p2pk != null) {
      $result.p2pk = p2pk;
    }
    if (p2pkh != null) {
      $result.p2pkh = p2pkh;
    }
    if (p2wpkh != null) {
      $result.p2wpkh = p2wpkh;
    }
    if (p2trKeyPath != null) {
      $result.p2trKeyPath = p2trKeyPath;
    }
    if (brc20Inscribe != null) {
      $result.brc20Inscribe = brc20Inscribe;
    }
    return $result;
  }
  Input_InputBuilder._() : super();
  factory Input_InputBuilder.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Input_InputBuilder.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Input_InputBuilder_Variant> _Input_InputBuilder_VariantByTag = {
    2 : Input_InputBuilder_Variant.p2pk,
    3 : Input_InputBuilder_Variant.p2pkh,
    5 : Input_InputBuilder_Variant.p2wpkh,
    7 : Input_InputBuilder_Variant.p2trKeyPath,
    9 : Input_InputBuilder_Variant.brc20Inscribe,
    0 : Input_InputBuilder_Variant.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Input.InputBuilder', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..oo(0, [2, 3, 5, 7, 9])
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'p2pk', $pb.PbFieldType.OY)
    ..aOM<PublicKeyOrHash>(3, _omitFieldNames ? '' : 'p2pkh', subBuilder: PublicKeyOrHash.create)
    ..aOM<PublicKeyOrHash>(5, _omitFieldNames ? '' : 'p2wpkh', subBuilder: PublicKeyOrHash.create)
    ..a<$core.List<$core.int>>(7, _omitFieldNames ? '' : 'p2trKeyPath', $pb.PbFieldType.OY)
    ..aOM<Input_InputBrc20Inscription>(9, _omitFieldNames ? '' : 'brc20Inscribe', subBuilder: Input_InputBrc20Inscription.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Input_InputBuilder clone() => Input_InputBuilder()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Input_InputBuilder copyWith(void Function(Input_InputBuilder) updates) => super.copyWith((message) => updates(message as Input_InputBuilder)) as Input_InputBuilder;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Input_InputBuilder create() => Input_InputBuilder._();
  Input_InputBuilder createEmptyInstance() => create();
  static $pb.PbList<Input_InputBuilder> createRepeated() => $pb.PbList<Input_InputBuilder>();
  @$core.pragma('dart2js:noInline')
  static Input_InputBuilder getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Input_InputBuilder>(create);
  static Input_InputBuilder? _defaultInstance;

  Input_InputBuilder_Variant whichVariant() => _Input_InputBuilder_VariantByTag[$_whichOneof(0)]!;
  void clearVariant() => clearField($_whichOneof(0));

  /// Pay-to-Public-Key, specify the public key.
  @$pb.TagNumber(2)
  $core.List<$core.int> get p2pk => $_getN(0);
  @$pb.TagNumber(2)
  set p2pk($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(2)
  $core.bool hasP2pk() => $_has(0);
  @$pb.TagNumber(2)
  void clearP2pk() => clearField(2);

  /// Pay-to-Public-Key-Hash, specify the public key.
  @$pb.TagNumber(3)
  PublicKeyOrHash get p2pkh => $_getN(1);
  @$pb.TagNumber(3)
  set p2pkh(PublicKeyOrHash v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasP2pkh() => $_has(1);
  @$pb.TagNumber(3)
  void clearP2pkh() => clearField(3);
  @$pb.TagNumber(3)
  PublicKeyOrHash ensureP2pkh() => $_ensure(1);

  /// Pay-to-Public-Key-Hash, specify the public key.
  @$pb.TagNumber(5)
  PublicKeyOrHash get p2wpkh => $_getN(2);
  @$pb.TagNumber(5)
  set p2wpkh(PublicKeyOrHash v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasP2wpkh() => $_has(2);
  @$pb.TagNumber(5)
  void clearP2wpkh() => clearField(5);
  @$pb.TagNumber(5)
  PublicKeyOrHash ensureP2wpkh() => $_ensure(2);

  /// Pay-to-Taproot-key-path (balance transfers), specify the public key.
  @$pb.TagNumber(7)
  $core.List<$core.int> get p2trKeyPath => $_getN(3);
  @$pb.TagNumber(7)
  set p2trKeyPath($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(7)
  $core.bool hasP2trKeyPath() => $_has(3);
  @$pb.TagNumber(7)
  void clearP2trKeyPath() => clearField(7);

  /// Create a BRC20 inscription.
  @$pb.TagNumber(9)
  Input_InputBrc20Inscription get brc20Inscribe => $_getN(4);
  @$pb.TagNumber(9)
  set brc20Inscribe(Input_InputBrc20Inscription v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasBrc20Inscribe() => $_has(4);
  @$pb.TagNumber(9)
  void clearBrc20Inscribe() => clearField(9);
  @$pb.TagNumber(9)
  Input_InputBrc20Inscription ensureBrc20Inscribe() => $_ensure(4);
}

class Input_InputTaprootScriptPath extends $pb.GeneratedMessage {
  factory Input_InputTaprootScriptPath({
    $core.List<$core.int>? payload,
    $core.List<$core.int>? controlBlock,
  }) {
    final $result = create();
    if (payload != null) {
      $result.payload = payload;
    }
    if (controlBlock != null) {
      $result.controlBlock = controlBlock;
    }
    return $result;
  }
  Input_InputTaprootScriptPath._() : super();
  factory Input_InputTaprootScriptPath.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Input_InputTaprootScriptPath.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Input.InputTaprootScriptPath', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'payload', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'controlBlock', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Input_InputTaprootScriptPath clone() => Input_InputTaprootScriptPath()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Input_InputTaprootScriptPath copyWith(void Function(Input_InputTaprootScriptPath) updates) => super.copyWith((message) => updates(message as Input_InputTaprootScriptPath)) as Input_InputTaprootScriptPath;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Input_InputTaprootScriptPath create() => Input_InputTaprootScriptPath._();
  Input_InputTaprootScriptPath createEmptyInstance() => create();
  static $pb.PbList<Input_InputTaprootScriptPath> createRepeated() => $pb.PbList<Input_InputTaprootScriptPath>();
  @$core.pragma('dart2js:noInline')
  static Input_InputTaprootScriptPath getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Input_InputTaprootScriptPath>(create);
  static Input_InputTaprootScriptPath? _defaultInstance;

  /// The payload of the Taproot transaction.
  @$pb.TagNumber(2)
  $core.List<$core.int> get payload => $_getN(0);
  @$pb.TagNumber(2)
  set payload($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(2)
  $core.bool hasPayload() => $_has(0);
  @$pb.TagNumber(2)
  void clearPayload() => clearField(2);

  /// The control block of the Taproot transaction required for claiming.
  @$pb.TagNumber(3)
  $core.List<$core.int> get controlBlock => $_getN(1);
  @$pb.TagNumber(3)
  set controlBlock($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(3)
  $core.bool hasControlBlock() => $_has(1);
  @$pb.TagNumber(3)
  void clearControlBlock() => clearField(3);
}

class Input_InputBrc20Inscription extends $pb.GeneratedMessage {
  factory Input_InputBrc20Inscription({
    $core.List<$core.int>? inscribeTo,
    $core.String? ticker,
    $core.String? transferAmount,
  }) {
    final $result = create();
    if (inscribeTo != null) {
      $result.inscribeTo = inscribeTo;
    }
    if (ticker != null) {
      $result.ticker = ticker;
    }
    if (transferAmount != null) {
      $result.transferAmount = transferAmount;
    }
    return $result;
  }
  Input_InputBrc20Inscription._() : super();
  factory Input_InputBrc20Inscription.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Input_InputBrc20Inscription.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Input.InputBrc20Inscription', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'inscribeTo', $pb.PbFieldType.OY)
    ..aOS(3, _omitFieldNames ? '' : 'ticker')
    ..aOS(4, _omitFieldNames ? '' : 'transferAmount')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Input_InputBrc20Inscription clone() => Input_InputBrc20Inscription()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Input_InputBrc20Inscription copyWith(void Function(Input_InputBrc20Inscription) updates) => super.copyWith((message) => updates(message as Input_InputBrc20Inscription)) as Input_InputBrc20Inscription;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Input_InputBrc20Inscription create() => Input_InputBrc20Inscription._();
  Input_InputBrc20Inscription createEmptyInstance() => create();
  static $pb.PbList<Input_InputBrc20Inscription> createRepeated() => $pb.PbList<Input_InputBrc20Inscription>();
  @$core.pragma('dart2js:noInline')
  static Input_InputBrc20Inscription getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Input_InputBrc20Inscription>(create);
  static Input_InputBrc20Inscription? _defaultInstance;

  /// The recipient of the inscription, usually the sender.
  @$pb.TagNumber(2)
  $core.List<$core.int> get inscribeTo => $_getN(0);
  @$pb.TagNumber(2)
  set inscribeTo($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(2)
  $core.bool hasInscribeTo() => $_has(0);
  @$pb.TagNumber(2)
  void clearInscribeTo() => clearField(2);

  /// The ticker of the BRC20 inscription.
  @$pb.TagNumber(3)
  $core.String get ticker => $_getSZ(1);
  @$pb.TagNumber(3)
  set ticker($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(3)
  $core.bool hasTicker() => $_has(1);
  @$pb.TagNumber(3)
  void clearTicker() => clearField(3);

  /// The BRC20 token transfer amount.
  @$pb.TagNumber(4)
  $core.String get transferAmount => $_getSZ(2);
  @$pb.TagNumber(4)
  set transferAmount($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(4)
  $core.bool hasTransferAmount() => $_has(2);
  @$pb.TagNumber(4)
  void clearTransferAmount() => clearField(4);
}

enum Input_ClaimingScript {
  scriptBuilder, 
  scriptData, 
  receiverAddress, 
  notSet
}

class Input extends $pb.GeneratedMessage {
  factory Input({
    OutPoint? outPoint,
    $fixnum.Int64? value,
    $core.int? sighashType,
    Input_Sequence? sequence,
    Input_InputBuilder? scriptBuilder,
    $core.List<$core.int>? scriptData,
    $core.String? receiverAddress,
  }) {
    final $result = create();
    if (outPoint != null) {
      $result.outPoint = outPoint;
    }
    if (value != null) {
      $result.value = value;
    }
    if (sighashType != null) {
      $result.sighashType = sighashType;
    }
    if (sequence != null) {
      $result.sequence = sequence;
    }
    if (scriptBuilder != null) {
      $result.scriptBuilder = scriptBuilder;
    }
    if (scriptData != null) {
      $result.scriptData = scriptData;
    }
    if (receiverAddress != null) {
      $result.receiverAddress = receiverAddress;
    }
    return $result;
  }
  Input._() : super();
  factory Input.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Input.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Input_ClaimingScript> _Input_ClaimingScriptByTag = {
    5 : Input_ClaimingScript.scriptBuilder,
    6 : Input_ClaimingScript.scriptData,
    7 : Input_ClaimingScript.receiverAddress,
    0 : Input_ClaimingScript.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Input', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..oo(0, [5, 6, 7])
    ..aOM<OutPoint>(1, _omitFieldNames ? '' : 'outPoint', subBuilder: OutPoint.create)
    ..aInt64(2, _omitFieldNames ? '' : 'value')
    ..a<$core.int>(3, _omitFieldNames ? '' : 'sighashType', $pb.PbFieldType.OU3)
    ..aOM<Input_Sequence>(4, _omitFieldNames ? '' : 'sequence', subBuilder: Input_Sequence.create)
    ..aOM<Input_InputBuilder>(5, _omitFieldNames ? '' : 'scriptBuilder', subBuilder: Input_InputBuilder.create)
    ..a<$core.List<$core.int>>(6, _omitFieldNames ? '' : 'scriptData', $pb.PbFieldType.OY)
    ..aOS(7, _omitFieldNames ? '' : 'receiverAddress')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Input clone() => Input()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Input copyWith(void Function(Input) updates) => super.copyWith((message) => updates(message as Input)) as Input;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Input create() => Input._();
  Input createEmptyInstance() => create();
  static $pb.PbList<Input> createRepeated() => $pb.PbList<Input>();
  @$core.pragma('dart2js:noInline')
  static Input getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Input>(create);
  static Input? _defaultInstance;

  Input_ClaimingScript whichClaimingScript() => _Input_ClaimingScriptByTag[$_whichOneof(0)]!;
  void clearClaimingScript() => clearField($_whichOneof(0));

  /// Reference to the previous transaction's output.
  @$pb.TagNumber(1)
  OutPoint get outPoint => $_getN(0);
  @$pb.TagNumber(1)
  set outPoint(OutPoint v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasOutPoint() => $_has(0);
  @$pb.TagNumber(1)
  void clearOutPoint() => clearField(1);
  @$pb.TagNumber(1)
  OutPoint ensureOutPoint() => $_ensure(0);

  /// The amount of satoshis of this input.
  @$pb.TagNumber(2)
  $fixnum.Int64 get value => $_getI64(1);
  @$pb.TagNumber(2)
  set value($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValue() => $_has(1);
  @$pb.TagNumber(2)
  void clearValue() => clearField(2);

  /// The sighash type, normally `All`.
  /// See `TWBitcoinSigHashType` enum.
  @$pb.TagNumber(3)
  $core.int get sighashType => $_getIZ(2);
  @$pb.TagNumber(3)
  set sighashType($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSighashType() => $_has(2);
  @$pb.TagNumber(3)
  void clearSighashType() => clearField(3);

  /// Optional sequence number, used for timelocks, replace-by-fee, etc.
  /// Leave empty to use a default 4294967295 (0xFFFFFFFF) value.
  @$pb.TagNumber(4)
  Input_Sequence get sequence => $_getN(3);
  @$pb.TagNumber(4)
  set sequence(Input_Sequence v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasSequence() => $_has(3);
  @$pb.TagNumber(4)
  void clearSequence() => clearField(4);
  @$pb.TagNumber(4)
  Input_Sequence ensureSequence() => $_ensure(3);

  /// Construct claiming script with a builder pattern.
  @$pb.TagNumber(5)
  Input_InputBuilder get scriptBuilder => $_getN(4);
  @$pb.TagNumber(5)
  set scriptBuilder(Input_InputBuilder v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasScriptBuilder() => $_has(4);
  @$pb.TagNumber(5)
  void clearScriptBuilder() => clearField(5);
  @$pb.TagNumber(5)
  Input_InputBuilder ensureScriptBuilder() => $_ensure(4);

  /// Spending script pubkey data.
  /// Use this variant if the UTXO claiming script is known already, otherwise use `InputBuilder`.
  /// Please note that the signing method (eg "legacy" or "segwit") will be determined by parsing the script data as:
  /// - P2PK, P2PKH - legacy signing method;
  /// - P2WPKH - segwit signing method.
  @$pb.TagNumber(6)
  $core.List<$core.int> get scriptData => $_getN(5);
  @$pb.TagNumber(6)
  set scriptData($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasScriptData() => $_has(5);
  @$pb.TagNumber(6)
  void clearScriptData() => clearField(6);

  /// Derive a spending script pubkey from a receiver address.
  /// E.g "bc1" segwit address will be P2WPKH claiming script.
  /// TODO consider deprecating this because we can't determine if the script pubkey is P2PK or P2PKH actually.
  @$pb.TagNumber(7)
  $core.String get receiverAddress => $_getSZ(6);
  @$pb.TagNumber(7)
  set receiverAddress($core.String v) { $_setString(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasReceiverAddress() => $_has(6);
  @$pb.TagNumber(7)
  void clearReceiverAddress() => clearField(7);
}

enum Output_OutputBuilder_Variant {
  p2sh, 
  p2pk, 
  p2pkh, 
  p2wsh, 
  p2wpkh, 
  p2trKeyPath, 
  p2trScriptPath, 
  p2trDangerousAssumeTweaked, 
  brc20Inscribe, 
  opReturn, 
  notSet
}

class Output_OutputBuilder extends $pb.GeneratedMessage {
  factory Output_OutputBuilder({
    Output_RedeemScriptOrHash? p2sh,
    $core.List<$core.int>? p2pk,
    PublicKeyOrHash? p2pkh,
    Output_RedeemScriptOrHash? p2wsh,
    PublicKeyOrHash? p2wpkh,
    $core.List<$core.int>? p2trKeyPath,
    Output_OutputTaprootScriptPath? p2trScriptPath,
    $core.List<$core.int>? p2trDangerousAssumeTweaked,
    Output_OutputBrc20Inscription? brc20Inscribe,
    $core.List<$core.int>? opReturn,
  }) {
    final $result = create();
    if (p2sh != null) {
      $result.p2sh = p2sh;
    }
    if (p2pk != null) {
      $result.p2pk = p2pk;
    }
    if (p2pkh != null) {
      $result.p2pkh = p2pkh;
    }
    if (p2wsh != null) {
      $result.p2wsh = p2wsh;
    }
    if (p2wpkh != null) {
      $result.p2wpkh = p2wpkh;
    }
    if (p2trKeyPath != null) {
      $result.p2trKeyPath = p2trKeyPath;
    }
    if (p2trScriptPath != null) {
      $result.p2trScriptPath = p2trScriptPath;
    }
    if (p2trDangerousAssumeTweaked != null) {
      $result.p2trDangerousAssumeTweaked = p2trDangerousAssumeTweaked;
    }
    if (brc20Inscribe != null) {
      $result.brc20Inscribe = brc20Inscribe;
    }
    if (opReturn != null) {
      $result.opReturn = opReturn;
    }
    return $result;
  }
  Output_OutputBuilder._() : super();
  factory Output_OutputBuilder.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Output_OutputBuilder.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Output_OutputBuilder_Variant> _Output_OutputBuilder_VariantByTag = {
    1 : Output_OutputBuilder_Variant.p2sh,
    2 : Output_OutputBuilder_Variant.p2pk,
    3 : Output_OutputBuilder_Variant.p2pkh,
    4 : Output_OutputBuilder_Variant.p2wsh,
    5 : Output_OutputBuilder_Variant.p2wpkh,
    6 : Output_OutputBuilder_Variant.p2trKeyPath,
    7 : Output_OutputBuilder_Variant.p2trScriptPath,
    8 : Output_OutputBuilder_Variant.p2trDangerousAssumeTweaked,
    9 : Output_OutputBuilder_Variant.brc20Inscribe,
    12 : Output_OutputBuilder_Variant.opReturn,
    0 : Output_OutputBuilder_Variant.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Output.OutputBuilder', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2, 3, 4, 5, 6, 7, 8, 9, 12])
    ..aOM<Output_RedeemScriptOrHash>(1, _omitFieldNames ? '' : 'p2sh', subBuilder: Output_RedeemScriptOrHash.create)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'p2pk', $pb.PbFieldType.OY)
    ..aOM<PublicKeyOrHash>(3, _omitFieldNames ? '' : 'p2pkh', subBuilder: PublicKeyOrHash.create)
    ..aOM<Output_RedeemScriptOrHash>(4, _omitFieldNames ? '' : 'p2wsh', subBuilder: Output_RedeemScriptOrHash.create)
    ..aOM<PublicKeyOrHash>(5, _omitFieldNames ? '' : 'p2wpkh', subBuilder: PublicKeyOrHash.create)
    ..a<$core.List<$core.int>>(6, _omitFieldNames ? '' : 'p2trKeyPath', $pb.PbFieldType.OY)
    ..aOM<Output_OutputTaprootScriptPath>(7, _omitFieldNames ? '' : 'p2trScriptPath', subBuilder: Output_OutputTaprootScriptPath.create)
    ..a<$core.List<$core.int>>(8, _omitFieldNames ? '' : 'p2trDangerousAssumeTweaked', $pb.PbFieldType.OY)
    ..aOM<Output_OutputBrc20Inscription>(9, _omitFieldNames ? '' : 'brc20Inscribe', subBuilder: Output_OutputBrc20Inscription.create)
    ..a<$core.List<$core.int>>(12, _omitFieldNames ? '' : 'opReturn', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Output_OutputBuilder clone() => Output_OutputBuilder()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Output_OutputBuilder copyWith(void Function(Output_OutputBuilder) updates) => super.copyWith((message) => updates(message as Output_OutputBuilder)) as Output_OutputBuilder;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Output_OutputBuilder create() => Output_OutputBuilder._();
  Output_OutputBuilder createEmptyInstance() => create();
  static $pb.PbList<Output_OutputBuilder> createRepeated() => $pb.PbList<Output_OutputBuilder>();
  @$core.pragma('dart2js:noInline')
  static Output_OutputBuilder getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Output_OutputBuilder>(create);
  static Output_OutputBuilder? _defaultInstance;

  Output_OutputBuilder_Variant whichVariant() => _Output_OutputBuilder_VariantByTag[$_whichOneof(0)]!;
  void clearVariant() => clearField($_whichOneof(0));

  /// Pay-to-Script-Hash, specify the redeem script or its hash.
  @$pb.TagNumber(1)
  Output_RedeemScriptOrHash get p2sh => $_getN(0);
  @$pb.TagNumber(1)
  set p2sh(Output_RedeemScriptOrHash v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasP2sh() => $_has(0);
  @$pb.TagNumber(1)
  void clearP2sh() => clearField(1);
  @$pb.TagNumber(1)
  Output_RedeemScriptOrHash ensureP2sh() => $_ensure(0);

  /// Pay-to-Public-Key, specify the public key.
  @$pb.TagNumber(2)
  $core.List<$core.int> get p2pk => $_getN(1);
  @$pb.TagNumber(2)
  set p2pk($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasP2pk() => $_has(1);
  @$pb.TagNumber(2)
  void clearP2pk() => clearField(2);

  /// Pay-to-Public-Key-Hash, specify the public key or its hash.
  @$pb.TagNumber(3)
  PublicKeyOrHash get p2pkh => $_getN(2);
  @$pb.TagNumber(3)
  set p2pkh(PublicKeyOrHash v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasP2pkh() => $_has(2);
  @$pb.TagNumber(3)
  void clearP2pkh() => clearField(3);
  @$pb.TagNumber(3)
  PublicKeyOrHash ensureP2pkh() => $_ensure(2);

  /// Pay-to-Witness-Script-Hash, specify the redeem script or its hash.
  @$pb.TagNumber(4)
  Output_RedeemScriptOrHash get p2wsh => $_getN(3);
  @$pb.TagNumber(4)
  set p2wsh(Output_RedeemScriptOrHash v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasP2wsh() => $_has(3);
  @$pb.TagNumber(4)
  void clearP2wsh() => clearField(4);
  @$pb.TagNumber(4)
  Output_RedeemScriptOrHash ensureP2wsh() => $_ensure(3);

  /// Pay-to-Public-Key-Hash, specify the public key or its hash.
  @$pb.TagNumber(5)
  PublicKeyOrHash get p2wpkh => $_getN(4);
  @$pb.TagNumber(5)
  set p2wpkh(PublicKeyOrHash v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasP2wpkh() => $_has(4);
  @$pb.TagNumber(5)
  void clearP2wpkh() => clearField(5);
  @$pb.TagNumber(5)
  PublicKeyOrHash ensureP2wpkh() => $_ensure(4);

  /// Pay-to-Taproot-key-path (balance transfers), specify the public key.
  @$pb.TagNumber(6)
  $core.List<$core.int> get p2trKeyPath => $_getN(5);
  @$pb.TagNumber(6)
  set p2trKeyPath($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasP2trKeyPath() => $_has(5);
  @$pb.TagNumber(6)
  void clearP2trKeyPath() => clearField(6);

  /// Pay-to-Taproot-script-path (complex transfers)
  @$pb.TagNumber(7)
  Output_OutputTaprootScriptPath get p2trScriptPath => $_getN(6);
  @$pb.TagNumber(7)
  set p2trScriptPath(Output_OutputTaprootScriptPath v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasP2trScriptPath() => $_has(6);
  @$pb.TagNumber(7)
  void clearP2trScriptPath() => clearField(7);
  @$pb.TagNumber(7)
  Output_OutputTaprootScriptPath ensureP2trScriptPath() => $_ensure(6);

  @$pb.TagNumber(8)
  $core.List<$core.int> get p2trDangerousAssumeTweaked => $_getN(7);
  @$pb.TagNumber(8)
  set p2trDangerousAssumeTweaked($core.List<$core.int> v) { $_setBytes(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasP2trDangerousAssumeTweaked() => $_has(7);
  @$pb.TagNumber(8)
  void clearP2trDangerousAssumeTweaked() => clearField(8);

  @$pb.TagNumber(9)
  Output_OutputBrc20Inscription get brc20Inscribe => $_getN(8);
  @$pb.TagNumber(9)
  set brc20Inscribe(Output_OutputBrc20Inscription v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasBrc20Inscribe() => $_has(8);
  @$pb.TagNumber(9)
  void clearBrc20Inscribe() => clearField(9);
  @$pb.TagNumber(9)
  Output_OutputBrc20Inscription ensureBrc20Inscribe() => $_ensure(8);

  /// OP_RETURN output. In most cases, with a zero-amount.
  @$pb.TagNumber(12)
  $core.List<$core.int> get opReturn => $_getN(9);
  @$pb.TagNumber(12)
  set opReturn($core.List<$core.int> v) { $_setBytes(9, v); }
  @$pb.TagNumber(12)
  $core.bool hasOpReturn() => $_has(9);
  @$pb.TagNumber(12)
  void clearOpReturn() => clearField(12);
}

enum Output_RedeemScriptOrHash_Variant {
  redeemScript, 
  hash, 
  notSet
}

/// Either a redeem script or its hash.
class Output_RedeemScriptOrHash extends $pb.GeneratedMessage {
  factory Output_RedeemScriptOrHash({
    $core.List<$core.int>? redeemScript,
    $core.List<$core.int>? hash,
  }) {
    final $result = create();
    if (redeemScript != null) {
      $result.redeemScript = redeemScript;
    }
    if (hash != null) {
      $result.hash = hash;
    }
    return $result;
  }
  Output_RedeemScriptOrHash._() : super();
  factory Output_RedeemScriptOrHash.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Output_RedeemScriptOrHash.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Output_RedeemScriptOrHash_Variant> _Output_RedeemScriptOrHash_VariantByTag = {
    1 : Output_RedeemScriptOrHash_Variant.redeemScript,
    2 : Output_RedeemScriptOrHash_Variant.hash,
    0 : Output_RedeemScriptOrHash_Variant.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Output.RedeemScriptOrHash', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2])
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'redeemScript', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'hash', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Output_RedeemScriptOrHash clone() => Output_RedeemScriptOrHash()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Output_RedeemScriptOrHash copyWith(void Function(Output_RedeemScriptOrHash) updates) => super.copyWith((message) => updates(message as Output_RedeemScriptOrHash)) as Output_RedeemScriptOrHash;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Output_RedeemScriptOrHash create() => Output_RedeemScriptOrHash._();
  Output_RedeemScriptOrHash createEmptyInstance() => create();
  static $pb.PbList<Output_RedeemScriptOrHash> createRepeated() => $pb.PbList<Output_RedeemScriptOrHash>();
  @$core.pragma('dart2js:noInline')
  static Output_RedeemScriptOrHash getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Output_RedeemScriptOrHash>(create);
  static Output_RedeemScriptOrHash? _defaultInstance;

  Output_RedeemScriptOrHash_Variant whichVariant() => _Output_RedeemScriptOrHash_VariantByTag[$_whichOneof(0)]!;
  void clearVariant() => clearField($_whichOneof(0));

  /// Redeem script bytes.
  @$pb.TagNumber(1)
  $core.List<$core.int> get redeemScript => $_getN(0);
  @$pb.TagNumber(1)
  set redeemScript($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasRedeemScript() => $_has(0);
  @$pb.TagNumber(1)
  void clearRedeemScript() => clearField(1);

  /// Public key hash.
  @$pb.TagNumber(2)
  $core.List<$core.int> get hash => $_getN(1);
  @$pb.TagNumber(2)
  set hash($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasHash() => $_has(1);
  @$pb.TagNumber(2)
  void clearHash() => clearField(2);
}

class Output_OutputTaprootScriptPath extends $pb.GeneratedMessage {
  factory Output_OutputTaprootScriptPath({
    $core.List<$core.int>? internalKey,
    $core.List<$core.int>? merkleRoot,
  }) {
    final $result = create();
    if (internalKey != null) {
      $result.internalKey = internalKey;
    }
    if (merkleRoot != null) {
      $result.merkleRoot = merkleRoot;
    }
    return $result;
  }
  Output_OutputTaprootScriptPath._() : super();
  factory Output_OutputTaprootScriptPath.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Output_OutputTaprootScriptPath.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Output.OutputTaprootScriptPath', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'internalKey', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'merkleRoot', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Output_OutputTaprootScriptPath clone() => Output_OutputTaprootScriptPath()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Output_OutputTaprootScriptPath copyWith(void Function(Output_OutputTaprootScriptPath) updates) => super.copyWith((message) => updates(message as Output_OutputTaprootScriptPath)) as Output_OutputTaprootScriptPath;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Output_OutputTaprootScriptPath create() => Output_OutputTaprootScriptPath._();
  Output_OutputTaprootScriptPath createEmptyInstance() => create();
  static $pb.PbList<Output_OutputTaprootScriptPath> createRepeated() => $pb.PbList<Output_OutputTaprootScriptPath>();
  @$core.pragma('dart2js:noInline')
  static Output_OutputTaprootScriptPath getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Output_OutputTaprootScriptPath>(create);
  static Output_OutputTaprootScriptPath? _defaultInstance;

  /// The internal key, usually the public key of the recipient.
  @$pb.TagNumber(1)
  $core.List<$core.int> get internalKey => $_getN(0);
  @$pb.TagNumber(1)
  set internalKey($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasInternalKey() => $_has(0);
  @$pb.TagNumber(1)
  void clearInternalKey() => clearField(1);

  /// The merkle root of the Taproot script(s), required to compute the sighash.
  @$pb.TagNumber(2)
  $core.List<$core.int> get merkleRoot => $_getN(1);
  @$pb.TagNumber(2)
  set merkleRoot($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasMerkleRoot() => $_has(1);
  @$pb.TagNumber(2)
  void clearMerkleRoot() => clearField(2);
}

class Output_OutputBrc20Inscription extends $pb.GeneratedMessage {
  factory Output_OutputBrc20Inscription({
    $core.List<$core.int>? inscribeTo,
    $core.String? ticker,
    $core.String? transferAmount,
  }) {
    final $result = create();
    if (inscribeTo != null) {
      $result.inscribeTo = inscribeTo;
    }
    if (ticker != null) {
      $result.ticker = ticker;
    }
    if (transferAmount != null) {
      $result.transferAmount = transferAmount;
    }
    return $result;
  }
  Output_OutputBrc20Inscription._() : super();
  factory Output_OutputBrc20Inscription.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Output_OutputBrc20Inscription.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Output.OutputBrc20Inscription', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'inscribeTo', $pb.PbFieldType.OY)
    ..aOS(2, _omitFieldNames ? '' : 'ticker')
    ..aOS(3, _omitFieldNames ? '' : 'transferAmount')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Output_OutputBrc20Inscription clone() => Output_OutputBrc20Inscription()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Output_OutputBrc20Inscription copyWith(void Function(Output_OutputBrc20Inscription) updates) => super.copyWith((message) => updates(message as Output_OutputBrc20Inscription)) as Output_OutputBrc20Inscription;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Output_OutputBrc20Inscription create() => Output_OutputBrc20Inscription._();
  Output_OutputBrc20Inscription createEmptyInstance() => create();
  static $pb.PbList<Output_OutputBrc20Inscription> createRepeated() => $pb.PbList<Output_OutputBrc20Inscription>();
  @$core.pragma('dart2js:noInline')
  static Output_OutputBrc20Inscription getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Output_OutputBrc20Inscription>(create);
  static Output_OutputBrc20Inscription? _defaultInstance;

  /// The recipient of the inscription, usually the sender.
  @$pb.TagNumber(1)
  $core.List<$core.int> get inscribeTo => $_getN(0);
  @$pb.TagNumber(1)
  set inscribeTo($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasInscribeTo() => $_has(0);
  @$pb.TagNumber(1)
  void clearInscribeTo() => clearField(1);

  /// The ticker of the BRC20 inscription.
  @$pb.TagNumber(2)
  $core.String get ticker => $_getSZ(1);
  @$pb.TagNumber(2)
  set ticker($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTicker() => $_has(1);
  @$pb.TagNumber(2)
  void clearTicker() => clearField(2);

  /// The BRC20 token transfer amount.
  @$pb.TagNumber(3)
  $core.String get transferAmount => $_getSZ(2);
  @$pb.TagNumber(3)
  set transferAmount($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTransferAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearTransferAmount() => clearField(3);
}

enum Output_ToRecipient {
  builder, 
  customScriptPubkey, 
  toAddress, 
  notSet
}

class Output extends $pb.GeneratedMessage {
  factory Output({
    $fixnum.Int64? value,
    Output_OutputBuilder? builder,
    $core.List<$core.int>? customScriptPubkey,
    $core.String? toAddress,
  }) {
    final $result = create();
    if (value != null) {
      $result.value = value;
    }
    if (builder != null) {
      $result.builder = builder;
    }
    if (customScriptPubkey != null) {
      $result.customScriptPubkey = customScriptPubkey;
    }
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    return $result;
  }
  Output._() : super();
  factory Output.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Output.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Output_ToRecipient> _Output_ToRecipientByTag = {
    2 : Output_ToRecipient.builder,
    3 : Output_ToRecipient.customScriptPubkey,
    4 : Output_ToRecipient.toAddress,
    0 : Output_ToRecipient.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Output', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..oo(0, [2, 3, 4])
    ..aInt64(1, _omitFieldNames ? '' : 'value')
    ..aOM<Output_OutputBuilder>(2, _omitFieldNames ? '' : 'builder', subBuilder: Output_OutputBuilder.create)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'customScriptPubkey', $pb.PbFieldType.OY)
    ..aOS(4, _omitFieldNames ? '' : 'toAddress')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Output clone() => Output()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Output copyWith(void Function(Output) updates) => super.copyWith((message) => updates(message as Output)) as Output;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Output create() => Output._();
  Output createEmptyInstance() => create();
  static $pb.PbList<Output> createRepeated() => $pb.PbList<Output>();
  @$core.pragma('dart2js:noInline')
  static Output getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Output>(create);
  static Output? _defaultInstance;

  Output_ToRecipient whichToRecipient() => _Output_ToRecipientByTag[$_whichOneof(0)]!;
  void clearToRecipient() => clearField($_whichOneof(0));

  /// The amount of satoshis to send.
  @$pb.TagNumber(1)
  $fixnum.Int64 get value => $_getI64(0);
  @$pb.TagNumber(1)
  set value($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasValue() => $_has(0);
  @$pb.TagNumber(1)
  void clearValue() => clearField(1);

  /// Construct output with builder pattern.
  @$pb.TagNumber(2)
  Output_OutputBuilder get builder => $_getN(1);
  @$pb.TagNumber(2)
  set builder(Output_OutputBuilder v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasBuilder() => $_has(1);
  @$pb.TagNumber(2)
  void clearBuilder() => clearField(2);
  @$pb.TagNumber(2)
  Output_OutputBuilder ensureBuilder() => $_ensure(1);

  /// Construct output by providing the scriptPubkey directly.
  @$pb.TagNumber(3)
  $core.List<$core.int> get customScriptPubkey => $_getN(2);
  @$pb.TagNumber(3)
  set customScriptPubkey($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasCustomScriptPubkey() => $_has(2);
  @$pb.TagNumber(3)
  void clearCustomScriptPubkey() => clearField(3);

  /// Derive the expected output from the provided address.
  @$pb.TagNumber(4)
  $core.String get toAddress => $_getSZ(3);
  @$pb.TagNumber(4)
  set toAddress($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasToAddress() => $_has(3);
  @$pb.TagNumber(4)
  void clearToAddress() => clearField(4);
}

class ChainInfo extends $pb.GeneratedMessage {
  factory ChainInfo({
    $core.int? p2pkhPrefix,
    $core.int? p2shPrefix,
  }) {
    final $result = create();
    if (p2pkhPrefix != null) {
      $result.p2pkhPrefix = p2pkhPrefix;
    }
    if (p2shPrefix != null) {
      $result.p2shPrefix = p2shPrefix;
    }
    return $result;
  }
  ChainInfo._() : super();
  factory ChainInfo.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ChainInfo.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'ChainInfo', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'p2pkhPrefix', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'p2shPrefix', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ChainInfo clone() => ChainInfo()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ChainInfo copyWith(void Function(ChainInfo) updates) => super.copyWith((message) => updates(message as ChainInfo)) as ChainInfo;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static ChainInfo create() => ChainInfo._();
  ChainInfo createEmptyInstance() => create();
  static $pb.PbList<ChainInfo> createRepeated() => $pb.PbList<ChainInfo>();
  @$core.pragma('dart2js:noInline')
  static ChainInfo getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ChainInfo>(create);
  static ChainInfo? _defaultInstance;

  /// P2PKH prefix for this chain.
  @$pb.TagNumber(1)
  $core.int get p2pkhPrefix => $_getIZ(0);
  @$pb.TagNumber(1)
  set p2pkhPrefix($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasP2pkhPrefix() => $_has(0);
  @$pb.TagNumber(1)
  void clearP2pkhPrefix() => clearField(1);

  /// P2SH prefix for this coin type
  @$pb.TagNumber(2)
  $core.int get p2shPrefix => $_getIZ(1);
  @$pb.TagNumber(2)
  set p2shPrefix($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasP2shPrefix() => $_has(1);
  @$pb.TagNumber(2)
  void clearP2shPrefix() => clearField(2);
}

enum SigningInput_DustPolicy {
  fixedDustThreshold, 
  notSet
}

class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    TransactionVersion? version,
    $core.Iterable<$core.List<$core.int>>? privateKeys,
    $core.Iterable<$core.List<$core.int>>? publicKeys,
    $core.int? lockTime,
    $core.Iterable<Input>? inputs,
    $core.Iterable<Output>? outputs,
    InputSelector? inputSelector,
    $fixnum.Int64? feePerVb,
    Output? changeOutput,
    Output? maxAmountOutput,
    ChainInfo? chainInfo,
    $fixnum.Int64? fixedDustThreshold,
    $core.bool? dangerousUseFixedSchnorrRng,
  }) {
    final $result = create();
    if (version != null) {
      $result.version = version;
    }
    if (privateKeys != null) {
      $result.privateKeys.addAll(privateKeys);
    }
    if (publicKeys != null) {
      $result.publicKeys.addAll(publicKeys);
    }
    if (lockTime != null) {
      $result.lockTime = lockTime;
    }
    if (inputs != null) {
      $result.inputs.addAll(inputs);
    }
    if (outputs != null) {
      $result.outputs.addAll(outputs);
    }
    if (inputSelector != null) {
      $result.inputSelector = inputSelector;
    }
    if (feePerVb != null) {
      $result.feePerVb = feePerVb;
    }
    if (changeOutput != null) {
      $result.changeOutput = changeOutput;
    }
    if (maxAmountOutput != null) {
      $result.maxAmountOutput = maxAmountOutput;
    }
    if (chainInfo != null) {
      $result.chainInfo = chainInfo;
    }
    if (fixedDustThreshold != null) {
      $result.fixedDustThreshold = fixedDustThreshold;
    }
    if (dangerousUseFixedSchnorrRng != null) {
      $result.dangerousUseFixedSchnorrRng = dangerousUseFixedSchnorrRng;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, SigningInput_DustPolicy> _SigningInput_DustPolicyByTag = {
    14 : SigningInput_DustPolicy.fixedDustThreshold,
    0 : SigningInput_DustPolicy.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..oo(0, [14])
    ..e<TransactionVersion>(1, _omitFieldNames ? '' : 'version', $pb.PbFieldType.OE, defaultOrMaker: TransactionVersion.UseDefault, valueOf: TransactionVersion.valueOf, enumValues: TransactionVersion.values)
    ..p<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'privateKeys', $pb.PbFieldType.PY)
    ..p<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'publicKeys', $pb.PbFieldType.PY)
    ..a<$core.int>(4, _omitFieldNames ? '' : 'lockTime', $pb.PbFieldType.OU3)
    ..pc<Input>(5, _omitFieldNames ? '' : 'inputs', $pb.PbFieldType.PM, subBuilder: Input.create)
    ..pc<Output>(6, _omitFieldNames ? '' : 'outputs', $pb.PbFieldType.PM, subBuilder: Output.create)
    ..e<InputSelector>(7, _omitFieldNames ? '' : 'inputSelector', $pb.PbFieldType.OE, defaultOrMaker: InputSelector.SelectAscending, valueOf: InputSelector.valueOf, enumValues: InputSelector.values)
    ..aInt64(8, _omitFieldNames ? '' : 'feePerVb')
    ..aOM<Output>(9, _omitFieldNames ? '' : 'changeOutput', subBuilder: Output.create)
    ..aOM<Output>(10, _omitFieldNames ? '' : 'maxAmountOutput', subBuilder: Output.create)
    ..aOM<ChainInfo>(13, _omitFieldNames ? '' : 'chainInfo', subBuilder: ChainInfo.create)
    ..aInt64(14, _omitFieldNames ? '' : 'fixedDustThreshold')
    ..aOB(20, _omitFieldNames ? '' : 'dangerousUseFixedSchnorrRng')
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

  SigningInput_DustPolicy whichDustPolicy() => _SigningInput_DustPolicyByTag[$_whichOneof(0)]!;
  void clearDustPolicy() => clearField($_whichOneof(0));

  /// Transaction version.
  @$pb.TagNumber(1)
  TransactionVersion get version => $_getN(0);
  @$pb.TagNumber(1)
  set version(TransactionVersion v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasVersion() => $_has(0);
  @$pb.TagNumber(1)
  void clearVersion() => clearField(1);

  /// User private keys.
  /// Only required if the `sign` method is called.
  @$pb.TagNumber(2)
  $core.List<$core.List<$core.int>> get privateKeys => $_getList(1);

  /// User public keys.
  /// Only required if the `plan`, `preImageHash` methods are called.
  @$pb.TagNumber(3)
  $core.List<$core.List<$core.int>> get publicKeys => $_getList(2);

  /// (optional) Block height or timestamp indicating at what point transactions can be included in a block.
  /// Zero by default.
  @$pb.TagNumber(4)
  $core.int get lockTime => $_getIZ(3);
  @$pb.TagNumber(4)
  set lockTime($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasLockTime() => $_has(3);
  @$pb.TagNumber(4)
  void clearLockTime() => clearField(4);

  /// The inputs to spend.
  @$pb.TagNumber(5)
  $core.List<Input> get inputs => $_getList(4);

  /// The output of the transaction. Note that the change output is specified
  /// in the `change_output` field.
  @$pb.TagNumber(6)
  $core.List<Output> get outputs => $_getList(5);

  /// How the inputs should be selected.
  @$pb.TagNumber(7)
  InputSelector get inputSelector => $_getN(6);
  @$pb.TagNumber(7)
  set inputSelector(InputSelector v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasInputSelector() => $_has(6);
  @$pb.TagNumber(7)
  void clearInputSelector() => clearField(7);

  /// The amount of satoshis per vbyte ("satVb"), used for fee calculation.
  /// Can be satoshis per byte ("satB") **ONLY** when transaction does not contain segwit UTXOs.
  @$pb.TagNumber(8)
  $fixnum.Int64 get feePerVb => $_getI64(7);
  @$pb.TagNumber(8)
  set feePerVb($fixnum.Int64 v) { $_setInt64(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasFeePerVb() => $_has(7);
  @$pb.TagNumber(8)
  void clearFeePerVb() => clearField(8);

  /// (optional) The change output to be added (return to sender) at the end of the outputs list.
  /// The `Output.value` will be overwritten, leave default.
  /// Note there can be no change output if the change amount is less than dust threshold.
  /// Leave empty to explicitly disable change output creation.
  @$pb.TagNumber(9)
  Output get changeOutput => $_getN(8);
  @$pb.TagNumber(9)
  set changeOutput(Output v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasChangeOutput() => $_has(8);
  @$pb.TagNumber(9)
  void clearChangeOutput() => clearField(9);
  @$pb.TagNumber(9)
  Output ensureChangeOutput() => $_ensure(8);

  /// The only output with a max available amount to be send.
  /// If set, `SigningInput.outputs` and `SigningInput.change` will be ignored.
  /// The `Output.value` will be overwritten, leave default.
  @$pb.TagNumber(10)
  Output get maxAmountOutput => $_getN(9);
  @$pb.TagNumber(10)
  set maxAmountOutput(Output v) { setField(10, v); }
  @$pb.TagNumber(10)
  $core.bool hasMaxAmountOutput() => $_has(9);
  @$pb.TagNumber(10)
  void clearMaxAmountOutput() => clearField(10);
  @$pb.TagNumber(10)
  Output ensureMaxAmountOutput() => $_ensure(9);

  /// Chain info includes p2pkh, p2sh address prefixes.
  /// The parameter needs to be set if an input/output has a receiver address pattern.
  @$pb.TagNumber(13)
  ChainInfo get chainInfo => $_getN(10);
  @$pb.TagNumber(13)
  set chainInfo(ChainInfo v) { setField(13, v); }
  @$pb.TagNumber(13)
  $core.bool hasChainInfo() => $_has(10);
  @$pb.TagNumber(13)
  void clearChainInfo() => clearField(13);
  @$pb.TagNumber(13)
  ChainInfo ensureChainInfo() => $_ensure(10);

  /// Use a constant "Dust" threshold.
  @$pb.TagNumber(14)
  $fixnum.Int64 get fixedDustThreshold => $_getI64(11);
  @$pb.TagNumber(14)
  set fixedDustThreshold($fixnum.Int64 v) { $_setInt64(11, v); }
  @$pb.TagNumber(14)
  $core.bool hasFixedDustThreshold() => $_has(11);
  @$pb.TagNumber(14)
  void clearFixedDustThreshold() => clearField(14);

  /// Whether disable auxiliary random data when signing.
  /// Use for testing **ONLY**.
  @$pb.TagNumber(20)
  $core.bool get dangerousUseFixedSchnorrRng => $_getBF(12);
  @$pb.TagNumber(20)
  set dangerousUseFixedSchnorrRng($core.bool v) { $_setBool(12, v); }
  @$pb.TagNumber(20)
  $core.bool hasDangerousUseFixedSchnorrRng() => $_has(12);
  @$pb.TagNumber(20)
  void clearDangerousUseFixedSchnorrRng() => clearField(20);
}

class Transaction_TransactionInput extends $pb.GeneratedMessage {
  factory Transaction_TransactionInput({
    OutPoint? outPoint,
    $core.int? sequence,
    $core.List<$core.int>? scriptSig,
    $core.Iterable<$core.List<$core.int>>? witnessItems,
  }) {
    final $result = create();
    if (outPoint != null) {
      $result.outPoint = outPoint;
    }
    if (sequence != null) {
      $result.sequence = sequence;
    }
    if (scriptSig != null) {
      $result.scriptSig = scriptSig;
    }
    if (witnessItems != null) {
      $result.witnessItems.addAll(witnessItems);
    }
    return $result;
  }
  Transaction_TransactionInput._() : super();
  factory Transaction_TransactionInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction_TransactionInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction.TransactionInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..aOM<OutPoint>(1, _omitFieldNames ? '' : 'outPoint', subBuilder: OutPoint.create)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'sequence', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'scriptSig', $pb.PbFieldType.OY)
    ..p<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'witnessItems', $pb.PbFieldType.PY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transaction_TransactionInput clone() => Transaction_TransactionInput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transaction_TransactionInput copyWith(void Function(Transaction_TransactionInput) updates) => super.copyWith((message) => updates(message as Transaction_TransactionInput)) as Transaction_TransactionInput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Transaction_TransactionInput create() => Transaction_TransactionInput._();
  Transaction_TransactionInput createEmptyInstance() => create();
  static $pb.PbList<Transaction_TransactionInput> createRepeated() => $pb.PbList<Transaction_TransactionInput>();
  @$core.pragma('dart2js:noInline')
  static Transaction_TransactionInput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transaction_TransactionInput>(create);
  static Transaction_TransactionInput? _defaultInstance;

  /// Reference to the previous transaction's output.
  @$pb.TagNumber(1)
  OutPoint get outPoint => $_getN(0);
  @$pb.TagNumber(1)
  set outPoint(OutPoint v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasOutPoint() => $_has(0);
  @$pb.TagNumber(1)
  void clearOutPoint() => clearField(1);
  @$pb.TagNumber(1)
  OutPoint ensureOutPoint() => $_ensure(0);

  /// The sequence number, used for timelocks, replace-by-fee, etc.
  @$pb.TagNumber(2)
  $core.int get sequence => $_getIZ(1);
  @$pb.TagNumber(2)
  set sequence($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasSequence() => $_has(1);
  @$pb.TagNumber(2)
  void clearSequence() => clearField(2);

  /// The script for claiming the input (non-Segwit/non-Taproot).
  @$pb.TagNumber(3)
  $core.List<$core.int> get scriptSig => $_getN(2);
  @$pb.TagNumber(3)
  set scriptSig($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasScriptSig() => $_has(2);
  @$pb.TagNumber(3)
  void clearScriptSig() => clearField(3);

  /// The script for claiming the input (Segit/Taproot).
  @$pb.TagNumber(4)
  $core.List<$core.List<$core.int>> get witnessItems => $_getList(3);
}

class Transaction_TransactionOutput extends $pb.GeneratedMessage {
  factory Transaction_TransactionOutput({
    $core.List<$core.int>? scriptPubkey,
    $fixnum.Int64? value,
  }) {
    final $result = create();
    if (scriptPubkey != null) {
      $result.scriptPubkey = scriptPubkey;
    }
    if (value != null) {
      $result.value = value;
    }
    return $result;
  }
  Transaction_TransactionOutput._() : super();
  factory Transaction_TransactionOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction_TransactionOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction.TransactionOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'scriptPubkey', $pb.PbFieldType.OY)
    ..aInt64(2, _omitFieldNames ? '' : 'value')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transaction_TransactionOutput clone() => Transaction_TransactionOutput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transaction_TransactionOutput copyWith(void Function(Transaction_TransactionOutput) updates) => super.copyWith((message) => updates(message as Transaction_TransactionOutput)) as Transaction_TransactionOutput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Transaction_TransactionOutput create() => Transaction_TransactionOutput._();
  Transaction_TransactionOutput createEmptyInstance() => create();
  static $pb.PbList<Transaction_TransactionOutput> createRepeated() => $pb.PbList<Transaction_TransactionOutput>();
  @$core.pragma('dart2js:noInline')
  static Transaction_TransactionOutput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transaction_TransactionOutput>(create);
  static Transaction_TransactionOutput? _defaultInstance;

  /// The condition for claiming the output.
  @$pb.TagNumber(1)
  $core.List<$core.int> get scriptPubkey => $_getN(0);
  @$pb.TagNumber(1)
  set scriptPubkey($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasScriptPubkey() => $_has(0);
  @$pb.TagNumber(1)
  void clearScriptPubkey() => clearField(1);

  /// The amount of satoshis to spend.
  @$pb.TagNumber(2)
  $fixnum.Int64 get value => $_getI64(1);
  @$pb.TagNumber(2)
  set value($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValue() => $_has(1);
  @$pb.TagNumber(2)
  void clearValue() => clearField(2);
}

class Transaction extends $pb.GeneratedMessage {
  factory Transaction({
    $core.int? version,
    $core.int? lockTime,
    $core.Iterable<Transaction_TransactionInput>? inputs,
    $core.Iterable<Transaction_TransactionOutput>? outputs,
  }) {
    final $result = create();
    if (version != null) {
      $result.version = version;
    }
    if (lockTime != null) {
      $result.lockTime = lockTime;
    }
    if (inputs != null) {
      $result.inputs.addAll(inputs);
    }
    if (outputs != null) {
      $result.outputs.addAll(outputs);
    }
    return $result;
  }
  Transaction._() : super();
  factory Transaction.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'version', $pb.PbFieldType.O3)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'lockTime', $pb.PbFieldType.OU3)
    ..pc<Transaction_TransactionInput>(3, _omitFieldNames ? '' : 'inputs', $pb.PbFieldType.PM, subBuilder: Transaction_TransactionInput.create)
    ..pc<Transaction_TransactionOutput>(4, _omitFieldNames ? '' : 'outputs', $pb.PbFieldType.PM, subBuilder: Transaction_TransactionOutput.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transaction clone() => Transaction()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transaction copyWith(void Function(Transaction) updates) => super.copyWith((message) => updates(message as Transaction)) as Transaction;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Transaction create() => Transaction._();
  Transaction createEmptyInstance() => create();
  static $pb.PbList<Transaction> createRepeated() => $pb.PbList<Transaction>();
  @$core.pragma('dart2js:noInline')
  static Transaction getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transaction>(create);
  static Transaction? _defaultInstance;

  /// The protocol version, is currently expected to be 1 or 2 (BIP68).
  @$pb.TagNumber(1)
  $core.int get version => $_getIZ(0);
  @$pb.TagNumber(1)
  set version($core.int v) { $_setSignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasVersion() => $_has(0);
  @$pb.TagNumber(1)
  void clearVersion() => clearField(1);

  /// Block height or timestamp indicating at what point transactions can be included in a block.
  /// Zero by default.
  @$pb.TagNumber(2)
  $core.int get lockTime => $_getIZ(1);
  @$pb.TagNumber(2)
  set lockTime($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasLockTime() => $_has(1);
  @$pb.TagNumber(2)
  void clearLockTime() => clearField(2);

  /// The transaction inputs.
  @$pb.TagNumber(3)
  $core.List<Transaction_TransactionInput> get inputs => $_getList(2);

  /// The transaction outputs.
  @$pb.TagNumber(4)
  $core.List<Transaction_TransactionOutput> get outputs => $_getList(3);
}

class TransactionPlan extends $pb.GeneratedMessage {
  factory TransactionPlan({
    $0.SigningError? error,
    $core.String? errorMessage,
    $core.Iterable<Input>? inputs,
    $core.Iterable<Output>? outputs,
    $fixnum.Int64? availableAmount,
    $fixnum.Int64? sendAmount,
    $fixnum.Int64? vsizeEstimate,
    $fixnum.Int64? feeEstimate,
    $fixnum.Int64? change,
  }) {
    final $result = create();
    if (error != null) {
      $result.error = error;
    }
    if (errorMessage != null) {
      $result.errorMessage = errorMessage;
    }
    if (inputs != null) {
      $result.inputs.addAll(inputs);
    }
    if (outputs != null) {
      $result.outputs.addAll(outputs);
    }
    if (availableAmount != null) {
      $result.availableAmount = availableAmount;
    }
    if (sendAmount != null) {
      $result.sendAmount = sendAmount;
    }
    if (vsizeEstimate != null) {
      $result.vsizeEstimate = vsizeEstimate;
    }
    if (feeEstimate != null) {
      $result.feeEstimate = feeEstimate;
    }
    if (change != null) {
      $result.change = change;
    }
    return $result;
  }
  TransactionPlan._() : super();
  factory TransactionPlan.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TransactionPlan.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TransactionPlan', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..e<$0.SigningError>(1, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(2, _omitFieldNames ? '' : 'errorMessage')
    ..pc<Input>(3, _omitFieldNames ? '' : 'inputs', $pb.PbFieldType.PM, subBuilder: Input.create)
    ..pc<Output>(4, _omitFieldNames ? '' : 'outputs', $pb.PbFieldType.PM, subBuilder: Output.create)
    ..aInt64(5, _omitFieldNames ? '' : 'availableAmount')
    ..aInt64(6, _omitFieldNames ? '' : 'sendAmount')
    ..a<$fixnum.Int64>(7, _omitFieldNames ? '' : 'vsizeEstimate', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aInt64(8, _omitFieldNames ? '' : 'feeEstimate')
    ..aInt64(9, _omitFieldNames ? '' : 'change')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TransactionPlan clone() => TransactionPlan()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TransactionPlan copyWith(void Function(TransactionPlan) updates) => super.copyWith((message) => updates(message as TransactionPlan)) as TransactionPlan;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TransactionPlan create() => TransactionPlan._();
  TransactionPlan createEmptyInstance() => create();
  static $pb.PbList<TransactionPlan> createRepeated() => $pb.PbList<TransactionPlan>();
  @$core.pragma('dart2js:noInline')
  static TransactionPlan getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TransactionPlan>(create);
  static TransactionPlan? _defaultInstance;

  /// A possible error, `OK` if none.
  @$pb.TagNumber(1)
  $0.SigningError get error => $_getN(0);
  @$pb.TagNumber(1)
  set error($0.SigningError v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasError() => $_has(0);
  @$pb.TagNumber(1)
  void clearError() => clearField(1);

  /// / Error description.
  @$pb.TagNumber(2)
  $core.String get errorMessage => $_getSZ(1);
  @$pb.TagNumber(2)
  set errorMessage($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasErrorMessage() => $_has(1);
  @$pb.TagNumber(2)
  void clearErrorMessage() => clearField(2);

  /// Selected unspent transaction outputs (subset of all input UTXOs).
  @$pb.TagNumber(3)
  $core.List<Input> get inputs => $_getList(2);

  /// Transaction outputs including a change output if applied.
  @$pb.TagNumber(4)
  $core.List<Output> get outputs => $_getList(3);

  /// Maximum available amount in all the transaction input UTXOs.
  /// That is an amount that will be spent by this transaction.
  @$pb.TagNumber(5)
  $fixnum.Int64 get availableAmount => $_getI64(4);
  @$pb.TagNumber(5)
  set availableAmount($fixnum.Int64 v) { $_setInt64(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasAvailableAmount() => $_has(4);
  @$pb.TagNumber(5)
  void clearAvailableAmount() => clearField(5);

  /// Total sending amount in all the transaction outputs.
  /// That is an amount that will be sent (including change output if applied).
  @$pb.TagNumber(6)
  $fixnum.Int64 get sendAmount => $_getI64(5);
  @$pb.TagNumber(6)
  set sendAmount($fixnum.Int64 v) { $_setInt64(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasSendAmount() => $_has(5);
  @$pb.TagNumber(6)
  void clearSendAmount() => clearField(6);

  /// The estimated `vsize` in `vbytes`.
  /// It is used to compare how much blockweight needs to be allocated to confirm a transaction.
  /// For non-segwit transactions, `vsize` = `size`.
  @$pb.TagNumber(7)
  $fixnum.Int64 get vsizeEstimate => $_getI64(6);
  @$pb.TagNumber(7)
  set vsizeEstimate($fixnum.Int64 v) { $_setInt64(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasVsizeEstimate() => $_has(6);
  @$pb.TagNumber(7)
  void clearVsizeEstimate() => clearField(7);

  /// The estimated fees of the transaction in satoshis.
  @$pb.TagNumber(8)
  $fixnum.Int64 get feeEstimate => $_getI64(7);
  @$pb.TagNumber(8)
  set feeEstimate($fixnum.Int64 v) { $_setInt64(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasFeeEstimate() => $_has(7);
  @$pb.TagNumber(8)
  void clearFeeEstimate() => clearField(8);

  /// Remaining change.
  /// Zero if not applied.
  @$pb.TagNumber(9)
  $fixnum.Int64 get change => $_getI64(8);
  @$pb.TagNumber(9)
  set change($fixnum.Int64 v) { $_setInt64(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasChange() => $_has(8);
  @$pb.TagNumber(9)
  void clearChange() => clearField(9);
}

class PreSigningOutput_Sighash extends $pb.GeneratedMessage {
  factory PreSigningOutput_Sighash({
    $core.List<$core.int>? publicKey,
    $core.List<$core.int>? sighash,
    PreSigningOutput_SigningMethod? signingMethod,
    PreSigningOutput_TaprootTweak? tweak,
  }) {
    final $result = create();
    if (publicKey != null) {
      $result.publicKey = publicKey;
    }
    if (sighash != null) {
      $result.sighash = sighash;
    }
    if (signingMethod != null) {
      $result.signingMethod = signingMethod;
    }
    if (tweak != null) {
      $result.tweak = tweak;
    }
    return $result;
  }
  PreSigningOutput_Sighash._() : super();
  factory PreSigningOutput_Sighash.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PreSigningOutput_Sighash.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'PreSigningOutput.Sighash', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'publicKey', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'sighash', $pb.PbFieldType.OY)
    ..e<PreSigningOutput_SigningMethod>(3, _omitFieldNames ? '' : 'signingMethod', $pb.PbFieldType.OE, defaultOrMaker: PreSigningOutput_SigningMethod.Legacy, valueOf: PreSigningOutput_SigningMethod.valueOf, enumValues: PreSigningOutput_SigningMethod.values)
    ..aOM<PreSigningOutput_TaprootTweak>(4, _omitFieldNames ? '' : 'tweak', subBuilder: PreSigningOutput_TaprootTweak.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  PreSigningOutput_Sighash clone() => PreSigningOutput_Sighash()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  PreSigningOutput_Sighash copyWith(void Function(PreSigningOutput_Sighash) updates) => super.copyWith((message) => updates(message as PreSigningOutput_Sighash)) as PreSigningOutput_Sighash;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static PreSigningOutput_Sighash create() => PreSigningOutput_Sighash._();
  PreSigningOutput_Sighash createEmptyInstance() => create();
  static $pb.PbList<PreSigningOutput_Sighash> createRepeated() => $pb.PbList<PreSigningOutput_Sighash>();
  @$core.pragma('dart2js:noInline')
  static PreSigningOutput_Sighash getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PreSigningOutput_Sighash>(create);
  static PreSigningOutput_Sighash? _defaultInstance;

  /// Public key used for signing.
  /// Please note it can be tweaked in case of P2TR scriptPubkey.
  @$pb.TagNumber(1)
  $core.List<$core.int> get publicKey => $_getN(0);
  @$pb.TagNumber(1)
  set publicKey($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPublicKey() => $_has(0);
  @$pb.TagNumber(1)
  void clearPublicKey() => clearField(1);

  /// The sighash to be signed.
  @$pb.TagNumber(2)
  $core.List<$core.int> get sighash => $_getN(1);
  @$pb.TagNumber(2)
  set sighash($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasSighash() => $_has(1);
  @$pb.TagNumber(2)
  void clearSighash() => clearField(2);

  /// Signing method to be used to sign the sighash.
  @$pb.TagNumber(3)
  PreSigningOutput_SigningMethod get signingMethod => $_getN(2);
  @$pb.TagNumber(3)
  set signingMethod(PreSigningOutput_SigningMethod v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasSigningMethod() => $_has(2);
  @$pb.TagNumber(3)
  void clearSigningMethod() => clearField(3);

  /// Taproot tweak if `Taproot` signing method is used.
  /// Empty if there is no need to tweak the private to sign the sighash.
  @$pb.TagNumber(4)
  PreSigningOutput_TaprootTweak get tweak => $_getN(3);
  @$pb.TagNumber(4)
  set tweak(PreSigningOutput_TaprootTweak v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasTweak() => $_has(3);
  @$pb.TagNumber(4)
  void clearTweak() => clearField(4);
  @$pb.TagNumber(4)
  PreSigningOutput_TaprootTweak ensureTweak() => $_ensure(3);
}

class PreSigningOutput_TaprootTweak extends $pb.GeneratedMessage {
  factory PreSigningOutput_TaprootTweak({
    $core.List<$core.int>? merkleRoot,
  }) {
    final $result = create();
    if (merkleRoot != null) {
      $result.merkleRoot = merkleRoot;
    }
    return $result;
  }
  PreSigningOutput_TaprootTweak._() : super();
  factory PreSigningOutput_TaprootTweak.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PreSigningOutput_TaprootTweak.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'PreSigningOutput.TaprootTweak', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'merkleRoot', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  PreSigningOutput_TaprootTweak clone() => PreSigningOutput_TaprootTweak()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  PreSigningOutput_TaprootTweak copyWith(void Function(PreSigningOutput_TaprootTweak) updates) => super.copyWith((message) => updates(message as PreSigningOutput_TaprootTweak)) as PreSigningOutput_TaprootTweak;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static PreSigningOutput_TaprootTweak create() => PreSigningOutput_TaprootTweak._();
  PreSigningOutput_TaprootTweak createEmptyInstance() => create();
  static $pb.PbList<PreSigningOutput_TaprootTweak> createRepeated() => $pb.PbList<PreSigningOutput_TaprootTweak>();
  @$core.pragma('dart2js:noInline')
  static PreSigningOutput_TaprootTweak getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PreSigningOutput_TaprootTweak>(create);
  static PreSigningOutput_TaprootTweak? _defaultInstance;

  /// 32 bytes merkle root of the script tree.
  /// Empty if there are no scripts, and the private key should be tweaked without a merkle root.
  @$pb.TagNumber(1)
  $core.List<$core.int> get merkleRoot => $_getN(0);
  @$pb.TagNumber(1)
  set merkleRoot($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasMerkleRoot() => $_has(0);
  @$pb.TagNumber(1)
  void clearMerkleRoot() => clearField(1);
}

class PreSigningOutput extends $pb.GeneratedMessage {
  factory PreSigningOutput({
    $0.SigningError? error,
    $core.String? errorMessage,
    $core.Iterable<PreSigningOutput_Sighash>? sighashes,
  }) {
    final $result = create();
    if (error != null) {
      $result.error = error;
    }
    if (errorMessage != null) {
      $result.errorMessage = errorMessage;
    }
    if (sighashes != null) {
      $result.sighashes.addAll(sighashes);
    }
    return $result;
  }
  PreSigningOutput._() : super();
  factory PreSigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PreSigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'PreSigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..e<$0.SigningError>(1, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(2, _omitFieldNames ? '' : 'errorMessage')
    ..pc<PreSigningOutput_Sighash>(4, _omitFieldNames ? '' : 'sighashes', $pb.PbFieldType.PM, subBuilder: PreSigningOutput_Sighash.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  PreSigningOutput clone() => PreSigningOutput()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  PreSigningOutput copyWith(void Function(PreSigningOutput) updates) => super.copyWith((message) => updates(message as PreSigningOutput)) as PreSigningOutput;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static PreSigningOutput create() => PreSigningOutput._();
  PreSigningOutput createEmptyInstance() => create();
  static $pb.PbList<PreSigningOutput> createRepeated() => $pb.PbList<PreSigningOutput>();
  @$core.pragma('dart2js:noInline')
  static PreSigningOutput getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PreSigningOutput>(create);
  static PreSigningOutput? _defaultInstance;

  /// A possible error, `OK` if none.
  @$pb.TagNumber(1)
  $0.SigningError get error => $_getN(0);
  @$pb.TagNumber(1)
  set error($0.SigningError v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasError() => $_has(0);
  @$pb.TagNumber(1)
  void clearError() => clearField(1);

  /// Error description.
  @$pb.TagNumber(2)
  $core.String get errorMessage => $_getSZ(1);
  @$pb.TagNumber(2)
  set errorMessage($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasErrorMessage() => $_has(1);
  @$pb.TagNumber(2)
  void clearErrorMessage() => clearField(2);

  /// / The sighashes to be signed; ECDSA for legacy and Segwit, Schnorr for Taproot.
  @$pb.TagNumber(4)
  $core.List<PreSigningOutput_Sighash> get sighashes => $_getList(2);
}

class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $0.SigningError? error,
    $core.String? errorMessage,
    Transaction? transaction,
    $core.List<$core.int>? encoded,
    $core.List<$core.int>? txid,
    $fixnum.Int64? vsize,
    $fixnum.Int64? weight,
    $fixnum.Int64? fee,
  }) {
    final $result = create();
    if (error != null) {
      $result.error = error;
    }
    if (errorMessage != null) {
      $result.errorMessage = errorMessage;
    }
    if (transaction != null) {
      $result.transaction = transaction;
    }
    if (encoded != null) {
      $result.encoded = encoded;
    }
    if (txid != null) {
      $result.txid = txid;
    }
    if (vsize != null) {
      $result.vsize = vsize;
    }
    if (weight != null) {
      $result.weight = weight;
    }
    if (fee != null) {
      $result.fee = fee;
    }
    return $result;
  }
  SigningOutput._() : super();
  factory SigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.BitcoinV2.Proto'), createEmptyInstance: create)
    ..e<$0.SigningError>(1, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(2, _omitFieldNames ? '' : 'errorMessage')
    ..aOM<Transaction>(3, _omitFieldNames ? '' : 'transaction', subBuilder: Transaction.create)
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'encoded', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(5, _omitFieldNames ? '' : 'txid', $pb.PbFieldType.OY)
    ..a<$fixnum.Int64>(6, _omitFieldNames ? '' : 'vsize', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(7, _omitFieldNames ? '' : 'weight', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aInt64(8, _omitFieldNames ? '' : 'fee')
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

  /// A possible error, `OK` if none.
  @$pb.TagNumber(1)
  $0.SigningError get error => $_getN(0);
  @$pb.TagNumber(1)
  set error($0.SigningError v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasError() => $_has(0);
  @$pb.TagNumber(1)
  void clearError() => clearField(1);

  /// Error description.
  @$pb.TagNumber(2)
  $core.String get errorMessage => $_getSZ(1);
  @$pb.TagNumber(2)
  set errorMessage($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasErrorMessage() => $_has(1);
  @$pb.TagNumber(2)
  void clearErrorMessage() => clearField(2);

  /// Resulting transaction.
  @$pb.TagNumber(3)
  Transaction get transaction => $_getN(2);
  @$pb.TagNumber(3)
  set transaction(Transaction v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasTransaction() => $_has(2);
  @$pb.TagNumber(3)
  void clearTransaction() => clearField(3);
  @$pb.TagNumber(3)
  Transaction ensureTransaction() => $_ensure(2);

  /// The encoded transaction that submitted to the network.
  @$pb.TagNumber(4)
  $core.List<$core.int> get encoded => $_getN(3);
  @$pb.TagNumber(4)
  set encoded($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasEncoded() => $_has(3);
  @$pb.TagNumber(4)
  void clearEncoded() => clearField(4);

  /// The transaction ID (hash).
  @$pb.TagNumber(5)
  $core.List<$core.int> get txid => $_getN(4);
  @$pb.TagNumber(5)
  set txid($core.List<$core.int> v) { $_setBytes(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasTxid() => $_has(4);
  @$pb.TagNumber(5)
  void clearTxid() => clearField(5);

  /// The total `vsize` in `vbytes`.
  /// It is used to compare how much blockweight needs to be allocated to confirm a transaction.
  /// For non-segwit transactions, `vsize` = `size`.
  @$pb.TagNumber(6)
  $fixnum.Int64 get vsize => $_getI64(5);
  @$pb.TagNumber(6)
  set vsize($fixnum.Int64 v) { $_setInt64(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasVsize() => $_has(5);
  @$pb.TagNumber(6)
  void clearVsize() => clearField(6);

  /// Transaction weight is defined as Base transaction size * 3 + Total transaction size
  /// (ie. the same method as calculating Block weight from Base size and Total size).
  @$pb.TagNumber(7)
  $fixnum.Int64 get weight => $_getI64(6);
  @$pb.TagNumber(7)
  set weight($fixnum.Int64 v) { $_setInt64(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasWeight() => $_has(6);
  @$pb.TagNumber(7)
  void clearWeight() => clearField(7);

  /// The total and final fee of the transaction in satoshis.
  @$pb.TagNumber(8)
  $fixnum.Int64 get fee => $_getI64(7);
  @$pb.TagNumber(8)
  set fee($fixnum.Int64 v) { $_setInt64(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasFee() => $_has(7);
  @$pb.TagNumber(8)
  void clearFee() => clearField(8);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
