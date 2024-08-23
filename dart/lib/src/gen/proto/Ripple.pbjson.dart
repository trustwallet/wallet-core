//
//  Generated code. Do not modify.
//  source: Ripple.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use currencyAmountDescriptor instead')
const CurrencyAmount$json = {
  '1': 'CurrencyAmount',
  '2': [
    {'1': 'currency', '3': 1, '4': 1, '5': 9, '10': 'currency'},
    {'1': 'value', '3': 2, '4': 1, '5': 9, '10': 'value'},
    {'1': 'issuer', '3': 3, '4': 1, '5': 9, '10': 'issuer'},
  ],
};

/// Descriptor for `CurrencyAmount`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List currencyAmountDescriptor = $convert.base64Decode(
    'Cg5DdXJyZW5jeUFtb3VudBIaCghjdXJyZW5jeRgBIAEoCVIIY3VycmVuY3kSFAoFdmFsdWUYAi'
    'ABKAlSBXZhbHVlEhYKBmlzc3VlchgDIAEoCVIGaXNzdWVy');

@$core.Deprecated('Use operationTrustSetDescriptor instead')
const OperationTrustSet$json = {
  '1': 'OperationTrustSet',
  '2': [
    {'1': 'limit_amount', '3': 1, '4': 1, '5': 11, '6': '.TW.Ripple.Proto.CurrencyAmount', '10': 'limitAmount'},
  ],
};

/// Descriptor for `OperationTrustSet`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List operationTrustSetDescriptor = $convert.base64Decode(
    'ChFPcGVyYXRpb25UcnVzdFNldBJCCgxsaW1pdF9hbW91bnQYASABKAsyHy5UVy5SaXBwbGUuUH'
    'JvdG8uQ3VycmVuY3lBbW91bnRSC2xpbWl0QW1vdW50');

@$core.Deprecated('Use operationPaymentDescriptor instead')
const OperationPayment$json = {
  '1': 'OperationPayment',
  '2': [
    {'1': 'amount', '3': 1, '4': 1, '5': 3, '9': 0, '10': 'amount'},
    {'1': 'currency_amount', '3': 2, '4': 1, '5': 11, '6': '.TW.Ripple.Proto.CurrencyAmount', '9': 0, '10': 'currencyAmount'},
    {'1': 'destination', '3': 3, '4': 1, '5': 9, '10': 'destination'},
    {'1': 'destination_tag', '3': 4, '4': 1, '5': 3, '10': 'destinationTag'},
  ],
  '8': [
    {'1': 'amount_oneof'},
  ],
};

/// Descriptor for `OperationPayment`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List operationPaymentDescriptor = $convert.base64Decode(
    'ChBPcGVyYXRpb25QYXltZW50EhgKBmFtb3VudBgBIAEoA0gAUgZhbW91bnQSSgoPY3VycmVuY3'
    'lfYW1vdW50GAIgASgLMh8uVFcuUmlwcGxlLlByb3RvLkN1cnJlbmN5QW1vdW50SABSDmN1cnJl'
    'bmN5QW1vdW50EiAKC2Rlc3RpbmF0aW9uGAMgASgJUgtkZXN0aW5hdGlvbhInCg9kZXN0aW5hdG'
    'lvbl90YWcYBCABKANSDmRlc3RpbmF0aW9uVGFnQg4KDGFtb3VudF9vbmVvZg==');

@$core.Deprecated('Use operationEscrowCreateDescriptor instead')
const OperationEscrowCreate$json = {
  '1': 'OperationEscrowCreate',
  '2': [
    {'1': 'amount', '3': 1, '4': 1, '5': 3, '10': 'amount'},
    {'1': 'destination', '3': 2, '4': 1, '5': 9, '10': 'destination'},
    {'1': 'destination_tag', '3': 3, '4': 1, '5': 3, '10': 'destinationTag'},
    {'1': 'cancel_after', '3': 4, '4': 1, '5': 3, '10': 'cancelAfter'},
    {'1': 'finish_after', '3': 5, '4': 1, '5': 3, '10': 'finishAfter'},
    {'1': 'condition', '3': 6, '4': 1, '5': 9, '10': 'condition'},
  ],
};

/// Descriptor for `OperationEscrowCreate`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List operationEscrowCreateDescriptor = $convert.base64Decode(
    'ChVPcGVyYXRpb25Fc2Nyb3dDcmVhdGUSFgoGYW1vdW50GAEgASgDUgZhbW91bnQSIAoLZGVzdG'
    'luYXRpb24YAiABKAlSC2Rlc3RpbmF0aW9uEicKD2Rlc3RpbmF0aW9uX3RhZxgDIAEoA1IOZGVz'
    'dGluYXRpb25UYWcSIQoMY2FuY2VsX2FmdGVyGAQgASgDUgtjYW5jZWxBZnRlchIhCgxmaW5pc2'
    'hfYWZ0ZXIYBSABKANSC2ZpbmlzaEFmdGVyEhwKCWNvbmRpdGlvbhgGIAEoCVIJY29uZGl0aW9u');

@$core.Deprecated('Use operationEscrowCancelDescriptor instead')
const OperationEscrowCancel$json = {
  '1': 'OperationEscrowCancel',
  '2': [
    {'1': 'owner', '3': 1, '4': 1, '5': 9, '10': 'owner'},
    {'1': 'offer_sequence', '3': 2, '4': 1, '5': 5, '10': 'offerSequence'},
  ],
};

/// Descriptor for `OperationEscrowCancel`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List operationEscrowCancelDescriptor = $convert.base64Decode(
    'ChVPcGVyYXRpb25Fc2Nyb3dDYW5jZWwSFAoFb3duZXIYASABKAlSBW93bmVyEiUKDm9mZmVyX3'
    'NlcXVlbmNlGAIgASgFUg1vZmZlclNlcXVlbmNl');

@$core.Deprecated('Use operationEscrowFinishDescriptor instead')
const OperationEscrowFinish$json = {
  '1': 'OperationEscrowFinish',
  '2': [
    {'1': 'owner', '3': 1, '4': 1, '5': 9, '10': 'owner'},
    {'1': 'offer_sequence', '3': 2, '4': 1, '5': 5, '10': 'offerSequence'},
    {'1': 'condition', '3': 3, '4': 1, '5': 9, '10': 'condition'},
    {'1': 'fulfillment', '3': 4, '4': 1, '5': 9, '10': 'fulfillment'},
  ],
};

/// Descriptor for `OperationEscrowFinish`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List operationEscrowFinishDescriptor = $convert.base64Decode(
    'ChVPcGVyYXRpb25Fc2Nyb3dGaW5pc2gSFAoFb3duZXIYASABKAlSBW93bmVyEiUKDm9mZmVyX3'
    'NlcXVlbmNlGAIgASgFUg1vZmZlclNlcXVlbmNlEhwKCWNvbmRpdGlvbhgDIAEoCVIJY29uZGl0'
    'aW9uEiAKC2Z1bGZpbGxtZW50GAQgASgJUgtmdWxmaWxsbWVudA==');

@$core.Deprecated('Use operationNFTokenBurnDescriptor instead')
const OperationNFTokenBurn$json = {
  '1': 'OperationNFTokenBurn',
  '2': [
    {'1': 'nftoken_id', '3': 1, '4': 1, '5': 12, '10': 'nftokenId'},
  ],
};

/// Descriptor for `OperationNFTokenBurn`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List operationNFTokenBurnDescriptor = $convert.base64Decode(
    'ChRPcGVyYXRpb25ORlRva2VuQnVybhIdCgpuZnRva2VuX2lkGAEgASgMUgluZnRva2VuSWQ=');

@$core.Deprecated('Use operationNFTokenCreateOfferDescriptor instead')
const OperationNFTokenCreateOffer$json = {
  '1': 'OperationNFTokenCreateOffer',
  '2': [
    {'1': 'nftoken_id', '3': 1, '4': 1, '5': 12, '10': 'nftokenId'},
    {'1': 'destination', '3': 2, '4': 1, '5': 9, '10': 'destination'},
  ],
};

/// Descriptor for `OperationNFTokenCreateOffer`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List operationNFTokenCreateOfferDescriptor = $convert.base64Decode(
    'ChtPcGVyYXRpb25ORlRva2VuQ3JlYXRlT2ZmZXISHQoKbmZ0b2tlbl9pZBgBIAEoDFIJbmZ0b2'
    'tlbklkEiAKC2Rlc3RpbmF0aW9uGAIgASgJUgtkZXN0aW5hdGlvbg==');

@$core.Deprecated('Use operationNFTokenAcceptOfferDescriptor instead')
const OperationNFTokenAcceptOffer$json = {
  '1': 'OperationNFTokenAcceptOffer',
  '2': [
    {'1': 'sell_offer', '3': 1, '4': 1, '5': 12, '10': 'sellOffer'},
  ],
};

/// Descriptor for `OperationNFTokenAcceptOffer`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List operationNFTokenAcceptOfferDescriptor = $convert.base64Decode(
    'ChtPcGVyYXRpb25ORlRva2VuQWNjZXB0T2ZmZXISHQoKc2VsbF9vZmZlchgBIAEoDFIJc2VsbE'
    '9mZmVy');

@$core.Deprecated('Use operationNFTokenCancelOfferDescriptor instead')
const OperationNFTokenCancelOffer$json = {
  '1': 'OperationNFTokenCancelOffer',
  '2': [
    {'1': 'token_offers', '3': 1, '4': 3, '5': 12, '10': 'tokenOffers'},
  ],
};

/// Descriptor for `OperationNFTokenCancelOffer`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List operationNFTokenCancelOfferDescriptor = $convert.base64Decode(
    'ChtPcGVyYXRpb25ORlRva2VuQ2FuY2VsT2ZmZXISIQoMdG9rZW5fb2ZmZXJzGAEgAygMUgt0b2'
    'tlbk9mZmVycw==');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'fee', '3': 1, '4': 1, '5': 3, '10': 'fee'},
    {'1': 'sequence', '3': 2, '4': 1, '5': 5, '10': 'sequence'},
    {'1': 'last_ledger_sequence', '3': 3, '4': 1, '5': 5, '10': 'lastLedgerSequence'},
    {'1': 'account', '3': 4, '4': 1, '5': 9, '10': 'account'},
    {'1': 'flags', '3': 5, '4': 1, '5': 3, '10': 'flags'},
    {'1': 'private_key', '3': 6, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'op_trust_set', '3': 7, '4': 1, '5': 11, '6': '.TW.Ripple.Proto.OperationTrustSet', '9': 0, '10': 'opTrustSet'},
    {'1': 'op_payment', '3': 8, '4': 1, '5': 11, '6': '.TW.Ripple.Proto.OperationPayment', '9': 0, '10': 'opPayment'},
    {'1': 'op_nftoken_burn', '3': 9, '4': 1, '5': 11, '6': '.TW.Ripple.Proto.OperationNFTokenBurn', '9': 0, '10': 'opNftokenBurn'},
    {'1': 'op_nftoken_create_offer', '3': 10, '4': 1, '5': 11, '6': '.TW.Ripple.Proto.OperationNFTokenCreateOffer', '9': 0, '10': 'opNftokenCreateOffer'},
    {'1': 'op_nftoken_accept_offer', '3': 11, '4': 1, '5': 11, '6': '.TW.Ripple.Proto.OperationNFTokenAcceptOffer', '9': 0, '10': 'opNftokenAcceptOffer'},
    {'1': 'op_nftoken_cancel_offer', '3': 12, '4': 1, '5': 11, '6': '.TW.Ripple.Proto.OperationNFTokenCancelOffer', '9': 0, '10': 'opNftokenCancelOffer'},
    {'1': 'op_escrow_create', '3': 16, '4': 1, '5': 11, '6': '.TW.Ripple.Proto.OperationEscrowCreate', '9': 0, '10': 'opEscrowCreate'},
    {'1': 'op_escrow_cancel', '3': 17, '4': 1, '5': 11, '6': '.TW.Ripple.Proto.OperationEscrowCancel', '9': 0, '10': 'opEscrowCancel'},
    {'1': 'op_escrow_finish', '3': 18, '4': 1, '5': 11, '6': '.TW.Ripple.Proto.OperationEscrowFinish', '9': 0, '10': 'opEscrowFinish'},
    {'1': 'public_key', '3': 15, '4': 1, '5': 12, '10': 'publicKey'},
  ],
  '8': [
    {'1': 'operation_oneof'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSEAoDZmVlGAEgASgDUgNmZWUSGgoIc2VxdWVuY2UYAiABKAVSCHNlcX'
    'VlbmNlEjAKFGxhc3RfbGVkZ2VyX3NlcXVlbmNlGAMgASgFUhJsYXN0TGVkZ2VyU2VxdWVuY2US'
    'GAoHYWNjb3VudBgEIAEoCVIHYWNjb3VudBIUCgVmbGFncxgFIAEoA1IFZmxhZ3MSHwoLcHJpdm'
    'F0ZV9rZXkYBiABKAxSCnByaXZhdGVLZXkSRgoMb3BfdHJ1c3Rfc2V0GAcgASgLMiIuVFcuUmlw'
    'cGxlLlByb3RvLk9wZXJhdGlvblRydXN0U2V0SABSCm9wVHJ1c3RTZXQSQgoKb3BfcGF5bWVudB'
    'gIIAEoCzIhLlRXLlJpcHBsZS5Qcm90by5PcGVyYXRpb25QYXltZW50SABSCW9wUGF5bWVudBJP'
    'Cg9vcF9uZnRva2VuX2J1cm4YCSABKAsyJS5UVy5SaXBwbGUuUHJvdG8uT3BlcmF0aW9uTkZUb2'
    'tlbkJ1cm5IAFINb3BOZnRva2VuQnVybhJlChdvcF9uZnRva2VuX2NyZWF0ZV9vZmZlchgKIAEo'
    'CzIsLlRXLlJpcHBsZS5Qcm90by5PcGVyYXRpb25ORlRva2VuQ3JlYXRlT2ZmZXJIAFIUb3BOZn'
    'Rva2VuQ3JlYXRlT2ZmZXISZQoXb3BfbmZ0b2tlbl9hY2NlcHRfb2ZmZXIYCyABKAsyLC5UVy5S'
    'aXBwbGUuUHJvdG8uT3BlcmF0aW9uTkZUb2tlbkFjY2VwdE9mZmVySABSFG9wTmZ0b2tlbkFjY2'
    'VwdE9mZmVyEmUKF29wX25mdG9rZW5fY2FuY2VsX29mZmVyGAwgASgLMiwuVFcuUmlwcGxlLlBy'
    'b3RvLk9wZXJhdGlvbk5GVG9rZW5DYW5jZWxPZmZlckgAUhRvcE5mdG9rZW5DYW5jZWxPZmZlch'
    'JSChBvcF9lc2Nyb3dfY3JlYXRlGBAgASgLMiYuVFcuUmlwcGxlLlByb3RvLk9wZXJhdGlvbkVz'
    'Y3Jvd0NyZWF0ZUgAUg5vcEVzY3Jvd0NyZWF0ZRJSChBvcF9lc2Nyb3dfY2FuY2VsGBEgASgLMi'
    'YuVFcuUmlwcGxlLlByb3RvLk9wZXJhdGlvbkVzY3Jvd0NhbmNlbEgAUg5vcEVzY3Jvd0NhbmNl'
    'bBJSChBvcF9lc2Nyb3dfZmluaXNoGBIgASgLMiYuVFcuUmlwcGxlLlByb3RvLk9wZXJhdGlvbk'
    'VzY3Jvd0ZpbmlzaEgAUg5vcEVzY3Jvd0ZpbmlzaBIdCgpwdWJsaWNfa2V5GA8gASgMUglwdWJs'
    'aWNLZXlCEQoPb3BlcmF0aW9uX29uZW9m');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'encoded', '3': 1, '4': 1, '5': 12, '10': 'encoded'},
    {'1': 'error', '3': 2, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 3, '4': 1, '5': 9, '10': 'errorMessage'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhgKB2VuY29kZWQYASABKAxSB2VuY29kZWQSMwoFZXJyb3IYAiABKA'
    '4yHS5UVy5Db21tb24uUHJvdG8uU2lnbmluZ0Vycm9yUgVlcnJvchIjCg1lcnJvcl9tZXNzYWdl'
    'GAMgASgJUgxlcnJvck1lc3NhZ2U=');

