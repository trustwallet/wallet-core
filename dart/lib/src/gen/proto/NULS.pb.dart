//
//  Generated code. Do not modify.
//  source: NULS.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

import 'Common.pbenum.dart' as $0;

/// Transaction from address
class TransactionCoinFrom extends $pb.GeneratedMessage {
  factory TransactionCoinFrom({
    $core.String? fromAddress,
    $core.int? assetsChainid,
    $core.int? assetsId,
    $core.List<$core.int>? idAmount,
    $core.List<$core.int>? nonce,
    $core.int? locked,
  }) {
    final $result = create();
    if (fromAddress != null) {
      $result.fromAddress = fromAddress;
    }
    if (assetsChainid != null) {
      $result.assetsChainid = assetsChainid;
    }
    if (assetsId != null) {
      $result.assetsId = assetsId;
    }
    if (idAmount != null) {
      $result.idAmount = idAmount;
    }
    if (nonce != null) {
      $result.nonce = nonce;
    }
    if (locked != null) {
      $result.locked = locked;
    }
    return $result;
  }
  TransactionCoinFrom._() : super();
  factory TransactionCoinFrom.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TransactionCoinFrom.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TransactionCoinFrom', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NULS.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'fromAddress')
    ..a<$core.int>(2, _omitFieldNames ? '' : 'assetsChainid', $pb.PbFieldType.OU3)
    ..a<$core.int>(3, _omitFieldNames ? '' : 'assetsId', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'idAmount', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(5, _omitFieldNames ? '' : 'nonce', $pb.PbFieldType.OY)
    ..a<$core.int>(6, _omitFieldNames ? '' : 'locked', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TransactionCoinFrom clone() => TransactionCoinFrom()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TransactionCoinFrom copyWith(void Function(TransactionCoinFrom) updates) => super.copyWith((message) => updates(message as TransactionCoinFrom)) as TransactionCoinFrom;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TransactionCoinFrom create() => TransactionCoinFrom._();
  TransactionCoinFrom createEmptyInstance() => create();
  static $pb.PbList<TransactionCoinFrom> createRepeated() => $pb.PbList<TransactionCoinFrom>();
  @$core.pragma('dart2js:noInline')
  static TransactionCoinFrom getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TransactionCoinFrom>(create);
  static TransactionCoinFrom? _defaultInstance;

  /// Source address
  @$pb.TagNumber(1)
  $core.String get fromAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set fromAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFromAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearFromAddress() => clearField(1);

  /// Chain ID
  @$pb.TagNumber(2)
  $core.int get assetsChainid => $_getIZ(1);
  @$pb.TagNumber(2)
  set assetsChainid($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAssetsChainid() => $_has(1);
  @$pb.TagNumber(2)
  void clearAssetsChainid() => clearField(2);

  /// ID of the asset
  @$pb.TagNumber(3)
  $core.int get assetsId => $_getIZ(2);
  @$pb.TagNumber(3)
  set assetsId($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAssetsId() => $_has(2);
  @$pb.TagNumber(3)
  void clearAssetsId() => clearField(3);

  /// transaction out amount (256-bit number)
  @$pb.TagNumber(4)
  $core.List<$core.int> get idAmount => $_getN(3);
  @$pb.TagNumber(4)
  set idAmount($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasIdAmount() => $_has(3);
  @$pb.TagNumber(4)
  void clearIdAmount() => clearField(4);

  /// Nonce, 8 bytes
  @$pb.TagNumber(5)
  $core.List<$core.int> get nonce => $_getN(4);
  @$pb.TagNumber(5)
  set nonce($core.List<$core.int> v) { $_setBytes(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasNonce() => $_has(4);
  @$pb.TagNumber(5)
  void clearNonce() => clearField(5);

  /// lock status: 1 locked; 0 unlocked
  @$pb.TagNumber(6)
  $core.int get locked => $_getIZ(5);
  @$pb.TagNumber(6)
  set locked($core.int v) { $_setUnsignedInt32(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasLocked() => $_has(5);
  @$pb.TagNumber(6)
  void clearLocked() => clearField(6);
}

/// Transaction to a destination
class TransactionCoinTo extends $pb.GeneratedMessage {
  factory TransactionCoinTo({
    $core.String? toAddress,
    $core.int? assetsChainid,
    $core.int? assetsId,
    $core.List<$core.int>? idAmount,
    $core.int? lockTime,
  }) {
    final $result = create();
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (assetsChainid != null) {
      $result.assetsChainid = assetsChainid;
    }
    if (assetsId != null) {
      $result.assetsId = assetsId;
    }
    if (idAmount != null) {
      $result.idAmount = idAmount;
    }
    if (lockTime != null) {
      $result.lockTime = lockTime;
    }
    return $result;
  }
  TransactionCoinTo._() : super();
  factory TransactionCoinTo.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TransactionCoinTo.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TransactionCoinTo', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NULS.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'toAddress')
    ..a<$core.int>(2, _omitFieldNames ? '' : 'assetsChainid', $pb.PbFieldType.OU3)
    ..a<$core.int>(3, _omitFieldNames ? '' : 'assetsId', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'idAmount', $pb.PbFieldType.OY)
    ..a<$core.int>(5, _omitFieldNames ? '' : 'lockTime', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TransactionCoinTo clone() => TransactionCoinTo()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TransactionCoinTo copyWith(void Function(TransactionCoinTo) updates) => super.copyWith((message) => updates(message as TransactionCoinTo)) as TransactionCoinTo;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TransactionCoinTo create() => TransactionCoinTo._();
  TransactionCoinTo createEmptyInstance() => create();
  static $pb.PbList<TransactionCoinTo> createRepeated() => $pb.PbList<TransactionCoinTo>();
  @$core.pragma('dart2js:noInline')
  static TransactionCoinTo getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TransactionCoinTo>(create);
  static TransactionCoinTo? _defaultInstance;

  /// destination address
  @$pb.TagNumber(1)
  $core.String get toAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set toAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasToAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearToAddress() => clearField(1);

  /// Chain ID
  @$pb.TagNumber(2)
  $core.int get assetsChainid => $_getIZ(1);
  @$pb.TagNumber(2)
  set assetsChainid($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAssetsChainid() => $_has(1);
  @$pb.TagNumber(2)
  void clearAssetsChainid() => clearField(2);

  /// ID of the asset
  @$pb.TagNumber(3)
  $core.int get assetsId => $_getIZ(2);
  @$pb.TagNumber(3)
  set assetsId($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAssetsId() => $_has(2);
  @$pb.TagNumber(3)
  void clearAssetsId() => clearField(3);

  /// transaction amount (uint256, serialized big endian)
  @$pb.TagNumber(4)
  $core.List<$core.int> get idAmount => $_getN(3);
  @$pb.TagNumber(4)
  set idAmount($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasIdAmount() => $_has(3);
  @$pb.TagNumber(4)
  void clearIdAmount() => clearField(4);

  /// lock time
  @$pb.TagNumber(5)
  $core.int get lockTime => $_getIZ(4);
  @$pb.TagNumber(5)
  set lockTime($core.int v) { $_setUnsignedInt32(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasLockTime() => $_has(4);
  @$pb.TagNumber(5)
  void clearLockTime() => clearField(5);
}

/// A signature
class Signature extends $pb.GeneratedMessage {
  factory Signature({
    $core.int? pkeyLen,
    $core.List<$core.int>? publicKey,
    $core.int? sigLen,
    $core.List<$core.int>? signature,
  }) {
    final $result = create();
    if (pkeyLen != null) {
      $result.pkeyLen = pkeyLen;
    }
    if (publicKey != null) {
      $result.publicKey = publicKey;
    }
    if (sigLen != null) {
      $result.sigLen = sigLen;
    }
    if (signature != null) {
      $result.signature = signature;
    }
    return $result;
  }
  Signature._() : super();
  factory Signature.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Signature.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Signature', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NULS.Proto'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'pkeyLen', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'publicKey', $pb.PbFieldType.OY)
    ..a<$core.int>(3, _omitFieldNames ? '' : 'sigLen', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'signature', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Signature clone() => Signature()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Signature copyWith(void Function(Signature) updates) => super.copyWith((message) => updates(message as Signature)) as Signature;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Signature create() => Signature._();
  Signature createEmptyInstance() => create();
  static $pb.PbList<Signature> createRepeated() => $pb.PbList<Signature>();
  @$core.pragma('dart2js:noInline')
  static Signature getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Signature>(create);
  static Signature? _defaultInstance;

  /// Length of public key data
  @$pb.TagNumber(1)
  $core.int get pkeyLen => $_getIZ(0);
  @$pb.TagNumber(1)
  set pkeyLen($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPkeyLen() => $_has(0);
  @$pb.TagNumber(1)
  void clearPkeyLen() => clearField(1);

  /// The public key
  @$pb.TagNumber(2)
  $core.List<$core.int> get publicKey => $_getN(1);
  @$pb.TagNumber(2)
  set publicKey($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPublicKey() => $_has(1);
  @$pb.TagNumber(2)
  void clearPublicKey() => clearField(2);

  /// The length of the signature
  @$pb.TagNumber(3)
  $core.int get sigLen => $_getIZ(2);
  @$pb.TagNumber(3)
  set sigLen($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSigLen() => $_has(2);
  @$pb.TagNumber(3)
  void clearSigLen() => clearField(3);

  /// The signature data
  @$pb.TagNumber(4)
  $core.List<$core.int> get signature => $_getN(3);
  @$pb.TagNumber(4)
  set signature($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasSignature() => $_has(3);
  @$pb.TagNumber(4)
  void clearSignature() => clearField(4);
}

/// A transaction
class Transaction extends $pb.GeneratedMessage {
  factory Transaction({
    $core.int? type,
    $core.int? timestamp,
    $core.String? remark,
    $core.List<$core.int>? txData,
    $core.Iterable<TransactionCoinFrom>? input,
    $core.Iterable<TransactionCoinTo>? output,
    Signature? txSigs,
    $core.int? hash,
  }) {
    final $result = create();
    if (type != null) {
      $result.type = type;
    }
    if (timestamp != null) {
      $result.timestamp = timestamp;
    }
    if (remark != null) {
      $result.remark = remark;
    }
    if (txData != null) {
      $result.txData = txData;
    }
    if (input != null) {
      $result.input.addAll(input);
    }
    if (output != null) {
      $result.output.addAll(output);
    }
    if (txSigs != null) {
      $result.txSigs = txSigs;
    }
    if (hash != null) {
      $result.hash = hash;
    }
    return $result;
  }
  Transaction._() : super();
  factory Transaction.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transaction.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Transaction', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NULS.Proto'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'type', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'timestamp', $pb.PbFieldType.OU3)
    ..aOS(3, _omitFieldNames ? '' : 'remark')
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'txData', $pb.PbFieldType.OY)
    ..pc<TransactionCoinFrom>(5, _omitFieldNames ? '' : 'input', $pb.PbFieldType.PM, subBuilder: TransactionCoinFrom.create)
    ..pc<TransactionCoinTo>(6, _omitFieldNames ? '' : 'output', $pb.PbFieldType.PM, subBuilder: TransactionCoinTo.create)
    ..aOM<Signature>(7, _omitFieldNames ? '' : 'txSigs', subBuilder: Signature.create)
    ..a<$core.int>(8, _omitFieldNames ? '' : 'hash', $pb.PbFieldType.OU3)
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

  /// transaction type
  @$pb.TagNumber(1)
  $core.int get type => $_getIZ(0);
  @$pb.TagNumber(1)
  set type($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasType() => $_has(0);
  @$pb.TagNumber(1)
  void clearType() => clearField(1);

  /// Timestamp of the transaction
  @$pb.TagNumber(2)
  $core.int get timestamp => $_getIZ(1);
  @$pb.TagNumber(2)
  set timestamp($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTimestamp() => $_has(1);
  @$pb.TagNumber(2)
  void clearTimestamp() => clearField(2);

  /// Optional string remark
  @$pb.TagNumber(3)
  $core.String get remark => $_getSZ(2);
  @$pb.TagNumber(3)
  set remark($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasRemark() => $_has(2);
  @$pb.TagNumber(3)
  void clearRemark() => clearField(3);

  /// The raw data
  @$pb.TagNumber(4)
  $core.List<$core.int> get txData => $_getN(3);
  @$pb.TagNumber(4)
  set txData($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasTxData() => $_has(3);
  @$pb.TagNumber(4)
  void clearTxData() => clearField(4);

  /// CoinFrom
  @$pb.TagNumber(5)
  $core.List<TransactionCoinFrom> get input => $_getList(4);

  /// CoinTo
  @$pb.TagNumber(6)
  $core.List<TransactionCoinTo> get output => $_getList(5);

  /// Signature
  @$pb.TagNumber(7)
  Signature get txSigs => $_getN(6);
  @$pb.TagNumber(7)
  set txSigs(Signature v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasTxSigs() => $_has(6);
  @$pb.TagNumber(7)
  void clearTxSigs() => clearField(7);
  @$pb.TagNumber(7)
  Signature ensureTxSigs() => $_ensure(6);

  /// Tx hash
  @$pb.TagNumber(8)
  $core.int get hash => $_getIZ(7);
  @$pb.TagNumber(8)
  set hash($core.int v) { $_setUnsignedInt32(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasHash() => $_has(7);
  @$pb.TagNumber(8)
  void clearHash() => clearField(8);
}

/// Input data necessary to create a signed order.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    $core.List<$core.int>? privateKey,
    $core.String? from,
    $core.String? to,
    $core.List<$core.int>? amount,
    $core.int? chainId,
    $core.int? idassetsId,
    $core.List<$core.int>? nonce,
    $core.String? remark,
    $core.List<$core.int>? balance,
    $core.int? timestamp,
    $core.String? feePayer,
    $core.List<$core.int>? feePayerNonce,
    $core.List<$core.int>? feePayerPrivateKey,
    $core.List<$core.int>? feePayerBalance,
  }) {
    final $result = create();
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (from != null) {
      $result.from = from;
    }
    if (to != null) {
      $result.to = to;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (chainId != null) {
      $result.chainId = chainId;
    }
    if (idassetsId != null) {
      $result.idassetsId = idassetsId;
    }
    if (nonce != null) {
      $result.nonce = nonce;
    }
    if (remark != null) {
      $result.remark = remark;
    }
    if (balance != null) {
      $result.balance = balance;
    }
    if (timestamp != null) {
      $result.timestamp = timestamp;
    }
    if (feePayer != null) {
      $result.feePayer = feePayer;
    }
    if (feePayerNonce != null) {
      $result.feePayerNonce = feePayerNonce;
    }
    if (feePayerPrivateKey != null) {
      $result.feePayerPrivateKey = feePayerPrivateKey;
    }
    if (feePayerBalance != null) {
      $result.feePayerBalance = feePayerBalance;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NULS.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..aOS(2, _omitFieldNames ? '' : 'from')
    ..aOS(3, _omitFieldNames ? '' : 'to')
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'amount', $pb.PbFieldType.OY)
    ..a<$core.int>(5, _omitFieldNames ? '' : 'chainId', $pb.PbFieldType.OU3)
    ..a<$core.int>(6, _omitFieldNames ? '' : 'idassetsId', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(7, _omitFieldNames ? '' : 'nonce', $pb.PbFieldType.OY)
    ..aOS(8, _omitFieldNames ? '' : 'remark')
    ..a<$core.List<$core.int>>(9, _omitFieldNames ? '' : 'balance', $pb.PbFieldType.OY)
    ..a<$core.int>(10, _omitFieldNames ? '' : 'timestamp', $pb.PbFieldType.OU3)
    ..aOS(11, _omitFieldNames ? '' : 'feePayer')
    ..a<$core.List<$core.int>>(12, _omitFieldNames ? '' : 'feePayerNonce', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(13, _omitFieldNames ? '' : 'feePayerPrivateKey', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(14, _omitFieldNames ? '' : 'feePayerBalance', $pb.PbFieldType.OY)
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

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(1)
  $core.List<$core.int> get privateKey => $_getN(0);
  @$pb.TagNumber(1)
  set privateKey($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPrivateKey() => $_has(0);
  @$pb.TagNumber(1)
  void clearPrivateKey() => clearField(1);

  /// Source address
  @$pb.TagNumber(2)
  $core.String get from => $_getSZ(1);
  @$pb.TagNumber(2)
  set from($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasFrom() => $_has(1);
  @$pb.TagNumber(2)
  void clearFrom() => clearField(2);

  /// Destination address
  @$pb.TagNumber(3)
  $core.String get to => $_getSZ(2);
  @$pb.TagNumber(3)
  set to($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTo() => $_has(2);
  @$pb.TagNumber(3)
  void clearTo() => clearField(3);

  /// Transfer amount (uint256, serialized big endian)
  @$pb.TagNumber(4)
  $core.List<$core.int> get amount => $_getN(3);
  @$pb.TagNumber(4)
  set amount($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasAmount() => $_has(3);
  @$pb.TagNumber(4)
  void clearAmount() => clearField(4);

  /// Chain ID
  @$pb.TagNumber(5)
  $core.int get chainId => $_getIZ(4);
  @$pb.TagNumber(5)
  set chainId($core.int v) { $_setUnsignedInt32(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasChainId() => $_has(4);
  @$pb.TagNumber(5)
  void clearChainId() => clearField(5);

  /// Asset ID
  @$pb.TagNumber(6)
  $core.int get idassetsId => $_getIZ(5);
  @$pb.TagNumber(6)
  set idassetsId($core.int v) { $_setUnsignedInt32(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasIdassetsId() => $_has(5);
  @$pb.TagNumber(6)
  void clearIdassetsId() => clearField(6);

  /// The last 8 bytes of latest transaction hash
  @$pb.TagNumber(7)
  $core.List<$core.int> get nonce => $_getN(6);
  @$pb.TagNumber(7)
  set nonce($core.List<$core.int> v) { $_setBytes(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasNonce() => $_has(6);
  @$pb.TagNumber(7)
  void clearNonce() => clearField(7);

  /// Optional memo remark
  @$pb.TagNumber(8)
  $core.String get remark => $_getSZ(7);
  @$pb.TagNumber(8)
  set remark($core.String v) { $_setString(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasRemark() => $_has(7);
  @$pb.TagNumber(8)
  void clearRemark() => clearField(8);

  /// Account balance
  @$pb.TagNumber(9)
  $core.List<$core.int> get balance => $_getN(8);
  @$pb.TagNumber(9)
  set balance($core.List<$core.int> v) { $_setBytes(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasBalance() => $_has(8);
  @$pb.TagNumber(9)
  void clearBalance() => clearField(9);

  /// time, accurate to the second
  @$pb.TagNumber(10)
  $core.int get timestamp => $_getIZ(9);
  @$pb.TagNumber(10)
  set timestamp($core.int v) { $_setUnsignedInt32(9, v); }
  @$pb.TagNumber(10)
  $core.bool hasTimestamp() => $_has(9);
  @$pb.TagNumber(10)
  void clearTimestamp() => clearField(10);

  /// external address paying fee, required for token transfer, optional for NULS transfer, depending on if an external fee payer is provided. If provided, it will be the fee paying address.
  @$pb.TagNumber(11)
  $core.String get feePayer => $_getSZ(10);
  @$pb.TagNumber(11)
  set feePayer($core.String v) { $_setString(10, v); }
  @$pb.TagNumber(11)
  $core.bool hasFeePayer() => $_has(10);
  @$pb.TagNumber(11)
  void clearFeePayer() => clearField(11);

  /// fee payer address nonce, required for token transfer, optional for NULS transfer, depending on if fee_payer is provided.
  @$pb.TagNumber(12)
  $core.List<$core.int> get feePayerNonce => $_getN(11);
  @$pb.TagNumber(12)
  set feePayerNonce($core.List<$core.int> v) { $_setBytes(11, v); }
  @$pb.TagNumber(12)
  $core.bool hasFeePayerNonce() => $_has(11);
  @$pb.TagNumber(12)
  void clearFeePayerNonce() => clearField(12);

  /// fee payer address private key, required for token transfer, optional for NULS transfer, depending on if fee_payer is provided.
  @$pb.TagNumber(13)
  $core.List<$core.int> get feePayerPrivateKey => $_getN(12);
  @$pb.TagNumber(13)
  set feePayerPrivateKey($core.List<$core.int> v) { $_setBytes(12, v); }
  @$pb.TagNumber(13)
  $core.bool hasFeePayerPrivateKey() => $_has(12);
  @$pb.TagNumber(13)
  void clearFeePayerPrivateKey() => clearField(13);

  /// fee payer NULS balance, it is required for token transfer. optional for NULS transfer, depending on if fee_payer is provided.
  @$pb.TagNumber(14)
  $core.List<$core.int> get feePayerBalance => $_getN(13);
  @$pb.TagNumber(14)
  set feePayerBalance($core.List<$core.int> v) { $_setBytes(13, v); }
  @$pb.TagNumber(14)
  $core.bool hasFeePayerBalance() => $_has(13);
  @$pb.TagNumber(14)
  void clearFeePayerBalance() => clearField(14);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
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
  SigningOutput._() : super();
  factory SigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.NULS.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'encoded', $pb.PbFieldType.OY)
    ..e<$0.SigningError>(2, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
    ..aOS(3, _omitFieldNames ? '' : 'errorMessage')
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

  /// Encoded transaction
  @$pb.TagNumber(1)
  $core.List<$core.int> get encoded => $_getN(0);
  @$pb.TagNumber(1)
  set encoded($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasEncoded() => $_has(0);
  @$pb.TagNumber(1)
  void clearEncoded() => clearField(1);

  /// error code, 0 is ok, other codes will be treated as errors
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
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
