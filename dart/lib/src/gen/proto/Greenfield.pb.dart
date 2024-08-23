//
//  Generated code. Do not modify.
//  source: Greenfield.proto
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
import 'Greenfield.pbenum.dart';

export 'Greenfield.pbenum.dart';

/// A denomination and an amount
class Amount extends $pb.GeneratedMessage {
  factory Amount({
    $core.String? denom,
    $core.String? amount,
  }) {
    final $result = create();
    if (denom != null) {
      $result.denom = denom;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    return $result;
  }
  Amount._() : super();
  factory Amount.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Amount.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Amount', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Greenfield.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'denom')
    ..aOS(2, _omitFieldNames ? '' : 'amount')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Amount clone() => Amount()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Amount copyWith(void Function(Amount) updates) => super.copyWith((message) => updates(message as Amount)) as Amount;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Amount create() => Amount._();
  Amount createEmptyInstance() => create();
  static $pb.PbList<Amount> createRepeated() => $pb.PbList<Amount>();
  @$core.pragma('dart2js:noInline')
  static Amount getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Amount>(create);
  static Amount? _defaultInstance;

  /// name of the denomination
  @$pb.TagNumber(1)
  $core.String get denom => $_getSZ(0);
  @$pb.TagNumber(1)
  set denom($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDenom() => $_has(0);
  @$pb.TagNumber(1)
  void clearDenom() => clearField(1);

  /// amount, number as string
  @$pb.TagNumber(2)
  $core.String get amount => $_getSZ(1);
  @$pb.TagNumber(2)
  set amount($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAmount() => $_has(1);
  @$pb.TagNumber(2)
  void clearAmount() => clearField(2);
}

/// Fee incl. gas
class Fee extends $pb.GeneratedMessage {
  factory Fee({
    $core.Iterable<Amount>? amounts,
    $fixnum.Int64? gas,
  }) {
    final $result = create();
    if (amounts != null) {
      $result.amounts.addAll(amounts);
    }
    if (gas != null) {
      $result.gas = gas;
    }
    return $result;
  }
  Fee._() : super();
  factory Fee.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Fee.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Fee', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Greenfield.Proto'), createEmptyInstance: create)
    ..pc<Amount>(1, _omitFieldNames ? '' : 'amounts', $pb.PbFieldType.PM, subBuilder: Amount.create)
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'gas', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Fee clone() => Fee()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Fee copyWith(void Function(Fee) updates) => super.copyWith((message) => updates(message as Fee)) as Fee;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Fee create() => Fee._();
  Fee createEmptyInstance() => create();
  static $pb.PbList<Fee> createRepeated() => $pb.PbList<Fee>();
  @$core.pragma('dart2js:noInline')
  static Fee getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Fee>(create);
  static Fee? _defaultInstance;

  /// Fee amount(s)
  @$pb.TagNumber(1)
  $core.List<Amount> get amounts => $_getList(0);

  /// Gas price
  @$pb.TagNumber(2)
  $fixnum.Int64 get gas => $_getI64(1);
  @$pb.TagNumber(2)
  set gas($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasGas() => $_has(1);
  @$pb.TagNumber(2)
  void clearGas() => clearField(2);
}

/// cosmos-sdk/MsgSend
class Message_Send extends $pb.GeneratedMessage {
  factory Message_Send({
    $core.String? fromAddress,
    $core.String? toAddress,
    $core.Iterable<Amount>? amounts,
    $core.String? typePrefix,
  }) {
    final $result = create();
    if (fromAddress != null) {
      $result.fromAddress = fromAddress;
    }
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (amounts != null) {
      $result.amounts.addAll(amounts);
    }
    if (typePrefix != null) {
      $result.typePrefix = typePrefix;
    }
    return $result;
  }
  Message_Send._() : super();
  factory Message_Send.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_Send.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.Send', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Greenfield.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'fromAddress')
    ..aOS(2, _omitFieldNames ? '' : 'toAddress')
    ..pc<Amount>(3, _omitFieldNames ? '' : 'amounts', $pb.PbFieldType.PM, subBuilder: Amount.create)
    ..aOS(4, _omitFieldNames ? '' : 'typePrefix')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_Send clone() => Message_Send()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_Send copyWith(void Function(Message_Send) updates) => super.copyWith((message) => updates(message as Message_Send)) as Message_Send;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_Send create() => Message_Send._();
  Message_Send createEmptyInstance() => create();
  static $pb.PbList<Message_Send> createRepeated() => $pb.PbList<Message_Send>();
  @$core.pragma('dart2js:noInline')
  static Message_Send getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_Send>(create);
  static Message_Send? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get fromAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set fromAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFromAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearFromAddress() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get toAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set toAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasToAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearToAddress() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<Amount> get amounts => $_getList(2);

  /// Optional. Default `cosmos.bank.v1beta1.MsgSend`.
  @$pb.TagNumber(4)
  $core.String get typePrefix => $_getSZ(3);
  @$pb.TagNumber(4)
  set typePrefix($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasTypePrefix() => $_has(3);
  @$pb.TagNumber(4)
  void clearTypePrefix() => clearField(4);
}

/// greenfield/MsgTransferOut
/// Used to transfer BNB Greenfield to BSC blockchain.
class Message_BridgeTransferOut extends $pb.GeneratedMessage {
  factory Message_BridgeTransferOut({
    $core.String? fromAddress,
    $core.String? toAddress,
    Amount? amount,
    $core.String? typePrefix,
  }) {
    final $result = create();
    if (fromAddress != null) {
      $result.fromAddress = fromAddress;
    }
    if (toAddress != null) {
      $result.toAddress = toAddress;
    }
    if (amount != null) {
      $result.amount = amount;
    }
    if (typePrefix != null) {
      $result.typePrefix = typePrefix;
    }
    return $result;
  }
  Message_BridgeTransferOut._() : super();
  factory Message_BridgeTransferOut.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message_BridgeTransferOut.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message.BridgeTransferOut', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Greenfield.Proto'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'fromAddress')
    ..aOS(2, _omitFieldNames ? '' : 'toAddress')
    ..aOM<Amount>(3, _omitFieldNames ? '' : 'amount', subBuilder: Amount.create)
    ..aOS(4, _omitFieldNames ? '' : 'typePrefix')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message_BridgeTransferOut clone() => Message_BridgeTransferOut()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message_BridgeTransferOut copyWith(void Function(Message_BridgeTransferOut) updates) => super.copyWith((message) => updates(message as Message_BridgeTransferOut)) as Message_BridgeTransferOut;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message_BridgeTransferOut create() => Message_BridgeTransferOut._();
  Message_BridgeTransferOut createEmptyInstance() => create();
  static $pb.PbList<Message_BridgeTransferOut> createRepeated() => $pb.PbList<Message_BridgeTransferOut>();
  @$core.pragma('dart2js:noInline')
  static Message_BridgeTransferOut getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message_BridgeTransferOut>(create);
  static Message_BridgeTransferOut? _defaultInstance;

  /// In most cases, `from_address` and `to_address` are equal.
  @$pb.TagNumber(1)
  $core.String get fromAddress => $_getSZ(0);
  @$pb.TagNumber(1)
  set fromAddress($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFromAddress() => $_has(0);
  @$pb.TagNumber(1)
  void clearFromAddress() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get toAddress => $_getSZ(1);
  @$pb.TagNumber(2)
  set toAddress($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasToAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearToAddress() => clearField(2);

  @$pb.TagNumber(3)
  Amount get amount => $_getN(2);
  @$pb.TagNumber(3)
  set amount(Amount v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasAmount() => $_has(2);
  @$pb.TagNumber(3)
  void clearAmount() => clearField(3);
  @$pb.TagNumber(3)
  Amount ensureAmount() => $_ensure(2);

  /// Optional. Default `greenfield.bridge.MsgTransferOut`.
  @$pb.TagNumber(4)
  $core.String get typePrefix => $_getSZ(3);
  @$pb.TagNumber(4)
  set typePrefix($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasTypePrefix() => $_has(3);
  @$pb.TagNumber(4)
  void clearTypePrefix() => clearField(4);
}

enum Message_MessageOneof {
  sendCoinsMessage, 
  bridgeTransferOut, 
  notSet
}

/// A transaction payload message
class Message extends $pb.GeneratedMessage {
  factory Message({
    Message_Send? sendCoinsMessage,
    Message_BridgeTransferOut? bridgeTransferOut,
  }) {
    final $result = create();
    if (sendCoinsMessage != null) {
      $result.sendCoinsMessage = sendCoinsMessage;
    }
    if (bridgeTransferOut != null) {
      $result.bridgeTransferOut = bridgeTransferOut;
    }
    return $result;
  }
  Message._() : super();
  factory Message.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Message.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, Message_MessageOneof> _Message_MessageOneofByTag = {
    1 : Message_MessageOneof.sendCoinsMessage,
    2 : Message_MessageOneof.bridgeTransferOut,
    0 : Message_MessageOneof.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Message', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Greenfield.Proto'), createEmptyInstance: create)
    ..oo(0, [1, 2])
    ..aOM<Message_Send>(1, _omitFieldNames ? '' : 'sendCoinsMessage', subBuilder: Message_Send.create)
    ..aOM<Message_BridgeTransferOut>(2, _omitFieldNames ? '' : 'bridgeTransferOut', subBuilder: Message_BridgeTransferOut.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Message clone() => Message()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Message copyWith(void Function(Message) updates) => super.copyWith((message) => updates(message as Message)) as Message;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Message create() => Message._();
  Message createEmptyInstance() => create();
  static $pb.PbList<Message> createRepeated() => $pb.PbList<Message>();
  @$core.pragma('dart2js:noInline')
  static Message getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Message>(create);
  static Message? _defaultInstance;

  Message_MessageOneof whichMessageOneof() => _Message_MessageOneofByTag[$_whichOneof(0)]!;
  void clearMessageOneof() => clearField($_whichOneof(0));

  @$pb.TagNumber(1)
  Message_Send get sendCoinsMessage => $_getN(0);
  @$pb.TagNumber(1)
  set sendCoinsMessage(Message_Send v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasSendCoinsMessage() => $_has(0);
  @$pb.TagNumber(1)
  void clearSendCoinsMessage() => clearField(1);
  @$pb.TagNumber(1)
  Message_Send ensureSendCoinsMessage() => $_ensure(0);

  @$pb.TagNumber(2)
  Message_BridgeTransferOut get bridgeTransferOut => $_getN(1);
  @$pb.TagNumber(2)
  set bridgeTransferOut(Message_BridgeTransferOut v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasBridgeTransferOut() => $_has(1);
  @$pb.TagNumber(2)
  void clearBridgeTransferOut() => clearField(2);
  @$pb.TagNumber(2)
  Message_BridgeTransferOut ensureBridgeTransferOut() => $_ensure(1);
}

/// Input data necessary to create a signed transaction.
class SigningInput extends $pb.GeneratedMessage {
  factory SigningInput({
    EncodingMode? encodingMode,
    SigningMode? signingMode,
    $fixnum.Int64? accountNumber,
    $core.String? ethChainId,
    $core.String? cosmosChainId,
    Fee? fee,
    $core.String? memo,
    $fixnum.Int64? sequence,
    $core.List<$core.int>? privateKey,
    $core.Iterable<Message>? messages,
    BroadcastMode? mode,
    $core.List<$core.int>? publicKey,
  }) {
    final $result = create();
    if (encodingMode != null) {
      $result.encodingMode = encodingMode;
    }
    if (signingMode != null) {
      $result.signingMode = signingMode;
    }
    if (accountNumber != null) {
      $result.accountNumber = accountNumber;
    }
    if (ethChainId != null) {
      $result.ethChainId = ethChainId;
    }
    if (cosmosChainId != null) {
      $result.cosmosChainId = cosmosChainId;
    }
    if (fee != null) {
      $result.fee = fee;
    }
    if (memo != null) {
      $result.memo = memo;
    }
    if (sequence != null) {
      $result.sequence = sequence;
    }
    if (privateKey != null) {
      $result.privateKey = privateKey;
    }
    if (messages != null) {
      $result.messages.addAll(messages);
    }
    if (mode != null) {
      $result.mode = mode;
    }
    if (publicKey != null) {
      $result.publicKey = publicKey;
    }
    return $result;
  }
  SigningInput._() : super();
  factory SigningInput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningInput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningInput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Greenfield.Proto'), createEmptyInstance: create)
    ..e<EncodingMode>(1, _omitFieldNames ? '' : 'encodingMode', $pb.PbFieldType.OE, defaultOrMaker: EncodingMode.Protobuf, valueOf: EncodingMode.valueOf, enumValues: EncodingMode.values)
    ..e<SigningMode>(2, _omitFieldNames ? '' : 'signingMode', $pb.PbFieldType.OE, defaultOrMaker: SigningMode.Eip712, valueOf: SigningMode.valueOf, enumValues: SigningMode.values)
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'accountNumber', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(4, _omitFieldNames ? '' : 'ethChainId')
    ..aOS(5, _omitFieldNames ? '' : 'cosmosChainId')
    ..aOM<Fee>(6, _omitFieldNames ? '' : 'fee', subBuilder: Fee.create)
    ..aOS(7, _omitFieldNames ? '' : 'memo')
    ..a<$fixnum.Int64>(8, _omitFieldNames ? '' : 'sequence', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.List<$core.int>>(9, _omitFieldNames ? '' : 'privateKey', $pb.PbFieldType.OY)
    ..pc<Message>(10, _omitFieldNames ? '' : 'messages', $pb.PbFieldType.PM, subBuilder: Message.create)
    ..e<BroadcastMode>(11, _omitFieldNames ? '' : 'mode', $pb.PbFieldType.OE, defaultOrMaker: BroadcastMode.SYNC, valueOf: BroadcastMode.valueOf, enumValues: BroadcastMode.values)
    ..a<$core.List<$core.int>>(12, _omitFieldNames ? '' : 'publicKey', $pb.PbFieldType.OY)
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

  /// An encoding mode.
  @$pb.TagNumber(1)
  EncodingMode get encodingMode => $_getN(0);
  @$pb.TagNumber(1)
  set encodingMode(EncodingMode v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasEncodingMode() => $_has(0);
  @$pb.TagNumber(1)
  void clearEncodingMode() => clearField(1);

  /// A signing mode.
  @$pb.TagNumber(2)
  SigningMode get signingMode => $_getN(1);
  @$pb.TagNumber(2)
  set signingMode(SigningMode v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasSigningMode() => $_has(1);
  @$pb.TagNumber(2)
  void clearSigningMode() => clearField(2);

  /// Source account number
  @$pb.TagNumber(3)
  $fixnum.Int64 get accountNumber => $_getI64(2);
  @$pb.TagNumber(3)
  set accountNumber($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasAccountNumber() => $_has(2);
  @$pb.TagNumber(3)
  void clearAccountNumber() => clearField(3);

  /// ETH Chain ID (string).
  /// Must be set if `signing_mode` is Eip712.
  @$pb.TagNumber(4)
  $core.String get ethChainId => $_getSZ(3);
  @$pb.TagNumber(4)
  set ethChainId($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasEthChainId() => $_has(3);
  @$pb.TagNumber(4)
  void clearEthChainId() => clearField(4);

  /// Cosmos Chain ID (string)
  @$pb.TagNumber(5)
  $core.String get cosmosChainId => $_getSZ(4);
  @$pb.TagNumber(5)
  set cosmosChainId($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasCosmosChainId() => $_has(4);
  @$pb.TagNumber(5)
  void clearCosmosChainId() => clearField(5);

  /// Transaction fee
  @$pb.TagNumber(6)
  Fee get fee => $_getN(5);
  @$pb.TagNumber(6)
  set fee(Fee v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasFee() => $_has(5);
  @$pb.TagNumber(6)
  void clearFee() => clearField(6);
  @$pb.TagNumber(6)
  Fee ensureFee() => $_ensure(5);

  /// Optional memo
  @$pb.TagNumber(7)
  $core.String get memo => $_getSZ(6);
  @$pb.TagNumber(7)
  set memo($core.String v) { $_setString(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasMemo() => $_has(6);
  @$pb.TagNumber(7)
  void clearMemo() => clearField(7);

  /// Sequence number (account specific)
  @$pb.TagNumber(8)
  $fixnum.Int64 get sequence => $_getI64(7);
  @$pb.TagNumber(8)
  set sequence($fixnum.Int64 v) { $_setInt64(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasSequence() => $_has(7);
  @$pb.TagNumber(8)
  void clearSequence() => clearField(8);

  /// The secret private key used for signing (32 bytes).
  @$pb.TagNumber(9)
  $core.List<$core.int> get privateKey => $_getN(8);
  @$pb.TagNumber(9)
  set privateKey($core.List<$core.int> v) { $_setBytes(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasPrivateKey() => $_has(8);
  @$pb.TagNumber(9)
  void clearPrivateKey() => clearField(9);

  /// Message payloads.
  @$pb.TagNumber(10)
  $core.List<Message> get messages => $_getList(9);

  /// Broadcast mode (included in output, relevant when broadcasting)
  @$pb.TagNumber(11)
  BroadcastMode get mode => $_getN(10);
  @$pb.TagNumber(11)
  set mode(BroadcastMode v) { setField(11, v); }
  @$pb.TagNumber(11)
  $core.bool hasMode() => $_has(10);
  @$pb.TagNumber(11)
  void clearMode() => clearField(11);

  @$pb.TagNumber(12)
  $core.List<$core.int> get publicKey => $_getN(11);
  @$pb.TagNumber(12)
  set publicKey($core.List<$core.int> v) { $_setBytes(11, v); }
  @$pb.TagNumber(12)
  $core.bool hasPublicKey() => $_has(11);
  @$pb.TagNumber(12)
  void clearPublicKey() => clearField(12);
}

/// Result containing the signed and encoded transaction.
class SigningOutput extends $pb.GeneratedMessage {
  factory SigningOutput({
    $core.List<$core.int>? signature,
    $core.String? serialized,
    $core.String? signatureJson,
    $core.String? errorMessage,
    $0.SigningError? error,
  }) {
    final $result = create();
    if (signature != null) {
      $result.signature = signature;
    }
    if (serialized != null) {
      $result.serialized = serialized;
    }
    if (signatureJson != null) {
      $result.signatureJson = signatureJson;
    }
    if (errorMessage != null) {
      $result.errorMessage = errorMessage;
    }
    if (error != null) {
      $result.error = error;
    }
    return $result;
  }
  SigningOutput._() : super();
  factory SigningOutput.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SigningOutput.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SigningOutput', package: const $pb.PackageName(_omitMessageNames ? '' : 'TW.Greenfield.Proto'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'signature', $pb.PbFieldType.OY)
    ..aOS(2, _omitFieldNames ? '' : 'serialized')
    ..aOS(3, _omitFieldNames ? '' : 'signatureJson')
    ..aOS(4, _omitFieldNames ? '' : 'errorMessage')
    ..e<$0.SigningError>(5, _omitFieldNames ? '' : 'error', $pb.PbFieldType.OE, defaultOrMaker: $0.SigningError.OK, valueOf: $0.SigningError.valueOf, enumValues: $0.SigningError.values)
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

  /// Signature
  @$pb.TagNumber(1)
  $core.List<$core.int> get signature => $_getN(0);
  @$pb.TagNumber(1)
  set signature($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSignature() => $_has(0);
  @$pb.TagNumber(1)
  void clearSignature() => clearField(1);

  /// Signed transaction containing protobuf encoded, Base64-encoded form (Stargate case),
  /// wrapped in a ready-to-broadcast json.
  @$pb.TagNumber(2)
  $core.String get serialized => $_getSZ(1);
  @$pb.TagNumber(2)
  set serialized($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasSerialized() => $_has(1);
  @$pb.TagNumber(2)
  void clearSerialized() => clearField(2);

  /// signatures array json string
  @$pb.TagNumber(3)
  $core.String get signatureJson => $_getSZ(2);
  @$pb.TagNumber(3)
  set signatureJson($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSignatureJson() => $_has(2);
  @$pb.TagNumber(3)
  void clearSignatureJson() => clearField(3);

  /// error description
  @$pb.TagNumber(4)
  $core.String get errorMessage => $_getSZ(3);
  @$pb.TagNumber(4)
  set errorMessage($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasErrorMessage() => $_has(3);
  @$pb.TagNumber(4)
  void clearErrorMessage() => clearField(4);

  @$pb.TagNumber(5)
  $0.SigningError get error => $_getN(4);
  @$pb.TagNumber(5)
  set error($0.SigningError v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasError() => $_has(4);
  @$pb.TagNumber(5)
  void clearError() => clearField(5);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
