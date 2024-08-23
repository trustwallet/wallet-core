//
//  Generated code. Do not modify.
//  source: FIO.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use publicAddressDescriptor instead')
const PublicAddress$json = {
  '1': 'PublicAddress',
  '2': [
    {'1': 'coin_symbol', '3': 1, '4': 1, '5': 9, '10': 'coinSymbol'},
    {'1': 'address', '3': 2, '4': 1, '5': 9, '10': 'address'},
  ],
};

/// Descriptor for `PublicAddress`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List publicAddressDescriptor = $convert.base64Decode(
    'Cg1QdWJsaWNBZGRyZXNzEh8KC2NvaW5fc3ltYm9sGAEgASgJUgpjb2luU3ltYm9sEhgKB2FkZH'
    'Jlc3MYAiABKAlSB2FkZHJlc3M=');

@$core.Deprecated('Use newFundsContentDescriptor instead')
const NewFundsContent$json = {
  '1': 'NewFundsContent',
  '2': [
    {'1': 'payee_public_address', '3': 1, '4': 1, '5': 9, '10': 'payeePublicAddress'},
    {'1': 'amount', '3': 2, '4': 1, '5': 9, '10': 'amount'},
    {'1': 'coin_symbol', '3': 3, '4': 1, '5': 9, '10': 'coinSymbol'},
    {'1': 'memo', '3': 4, '4': 1, '5': 9, '10': 'memo'},
    {'1': 'hash', '3': 5, '4': 1, '5': 9, '10': 'hash'},
    {'1': 'offline_url', '3': 6, '4': 1, '5': 9, '10': 'offlineUrl'},
  ],
};

/// Descriptor for `NewFundsContent`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List newFundsContentDescriptor = $convert.base64Decode(
    'Cg9OZXdGdW5kc0NvbnRlbnQSMAoUcGF5ZWVfcHVibGljX2FkZHJlc3MYASABKAlSEnBheWVlUH'
    'VibGljQWRkcmVzcxIWCgZhbW91bnQYAiABKAlSBmFtb3VudBIfCgtjb2luX3N5bWJvbBgDIAEo'
    'CVIKY29pblN5bWJvbBISCgRtZW1vGAQgASgJUgRtZW1vEhIKBGhhc2gYBSABKAlSBGhhc2gSHw'
    'oLb2ZmbGluZV91cmwYBiABKAlSCm9mZmxpbmVVcmw=');

@$core.Deprecated('Use actionDescriptor instead')
const Action$json = {
  '1': 'Action',
  '2': [
    {'1': 'register_fio_address_message', '3': 1, '4': 1, '5': 11, '6': '.TW.FIO.Proto.Action.RegisterFioAddress', '9': 0, '10': 'registerFioAddressMessage'},
    {'1': 'add_pub_address_message', '3': 2, '4': 1, '5': 11, '6': '.TW.FIO.Proto.Action.AddPubAddress', '9': 0, '10': 'addPubAddressMessage'},
    {'1': 'transfer_message', '3': 3, '4': 1, '5': 11, '6': '.TW.FIO.Proto.Action.Transfer', '9': 0, '10': 'transferMessage'},
    {'1': 'renew_fio_address_message', '3': 4, '4': 1, '5': 11, '6': '.TW.FIO.Proto.Action.RenewFioAddress', '9': 0, '10': 'renewFioAddressMessage'},
    {'1': 'new_funds_request_message', '3': 5, '4': 1, '5': 11, '6': '.TW.FIO.Proto.Action.NewFundsRequest', '9': 0, '10': 'newFundsRequestMessage'},
    {'1': 'remove_pub_address_message', '3': 6, '4': 1, '5': 11, '6': '.TW.FIO.Proto.Action.RemovePubAddress', '9': 0, '10': 'removePubAddressMessage'},
    {'1': 'remove_all_pub_addresses_message', '3': 7, '4': 1, '5': 11, '6': '.TW.FIO.Proto.Action.RemoveAllPubAddress', '9': 0, '10': 'removeAllPubAddressesMessage'},
    {'1': 'add_bundled_transactions_message', '3': 8, '4': 1, '5': 11, '6': '.TW.FIO.Proto.Action.AddBundledTransactions', '9': 0, '10': 'addBundledTransactionsMessage'},
  ],
  '3': [Action_RegisterFioAddress$json, Action_AddPubAddress$json, Action_RemovePubAddress$json, Action_RemoveAllPubAddress$json, Action_Transfer$json, Action_RenewFioAddress$json, Action_NewFundsRequest$json, Action_AddBundledTransactions$json],
  '8': [
    {'1': 'message_oneof'},
  ],
};

@$core.Deprecated('Use actionDescriptor instead')
const Action_RegisterFioAddress$json = {
  '1': 'RegisterFioAddress',
  '2': [
    {'1': 'fio_address', '3': 1, '4': 1, '5': 9, '10': 'fioAddress'},
    {'1': 'owner_fio_public_key', '3': 2, '4': 1, '5': 9, '10': 'ownerFioPublicKey'},
    {'1': 'fee', '3': 3, '4': 1, '5': 4, '10': 'fee'},
  ],
};

@$core.Deprecated('Use actionDescriptor instead')
const Action_AddPubAddress$json = {
  '1': 'AddPubAddress',
  '2': [
    {'1': 'fio_address', '3': 1, '4': 1, '5': 9, '10': 'fioAddress'},
    {'1': 'public_addresses', '3': 2, '4': 3, '5': 11, '6': '.TW.FIO.Proto.PublicAddress', '10': 'publicAddresses'},
    {'1': 'fee', '3': 3, '4': 1, '5': 4, '10': 'fee'},
  ],
};

@$core.Deprecated('Use actionDescriptor instead')
const Action_RemovePubAddress$json = {
  '1': 'RemovePubAddress',
  '2': [
    {'1': 'fio_address', '3': 1, '4': 1, '5': 9, '10': 'fioAddress'},
    {'1': 'public_addresses', '3': 2, '4': 3, '5': 11, '6': '.TW.FIO.Proto.PublicAddress', '10': 'publicAddresses'},
    {'1': 'fee', '3': 3, '4': 1, '5': 4, '10': 'fee'},
  ],
};

@$core.Deprecated('Use actionDescriptor instead')
const Action_RemoveAllPubAddress$json = {
  '1': 'RemoveAllPubAddress',
  '2': [
    {'1': 'fio_address', '3': 1, '4': 1, '5': 9, '10': 'fioAddress'},
    {'1': 'fee', '3': 3, '4': 1, '5': 4, '10': 'fee'},
  ],
};

@$core.Deprecated('Use actionDescriptor instead')
const Action_Transfer$json = {
  '1': 'Transfer',
  '2': [
    {'1': 'payee_public_key', '3': 1, '4': 1, '5': 9, '10': 'payeePublicKey'},
    {'1': 'amount', '3': 2, '4': 1, '5': 4, '10': 'amount'},
    {'1': 'fee', '3': 3, '4': 1, '5': 4, '10': 'fee'},
  ],
};

@$core.Deprecated('Use actionDescriptor instead')
const Action_RenewFioAddress$json = {
  '1': 'RenewFioAddress',
  '2': [
    {'1': 'fio_address', '3': 1, '4': 1, '5': 9, '10': 'fioAddress'},
    {'1': 'owner_fio_public_key', '3': 2, '4': 1, '5': 9, '10': 'ownerFioPublicKey'},
    {'1': 'fee', '3': 3, '4': 1, '5': 4, '10': 'fee'},
  ],
};

@$core.Deprecated('Use actionDescriptor instead')
const Action_NewFundsRequest$json = {
  '1': 'NewFundsRequest',
  '2': [
    {'1': 'payer_fio_name', '3': 1, '4': 1, '5': 9, '10': 'payerFioName'},
    {'1': 'payer_fio_address', '3': 2, '4': 1, '5': 9, '10': 'payerFioAddress'},
    {'1': 'payee_fio_name', '3': 3, '4': 1, '5': 9, '10': 'payeeFioName'},
    {'1': 'content', '3': 4, '4': 1, '5': 11, '6': '.TW.FIO.Proto.NewFundsContent', '10': 'content'},
    {'1': 'fee', '3': 5, '4': 1, '5': 4, '10': 'fee'},
  ],
};

@$core.Deprecated('Use actionDescriptor instead')
const Action_AddBundledTransactions$json = {
  '1': 'AddBundledTransactions',
  '2': [
    {'1': 'fio_address', '3': 1, '4': 1, '5': 9, '10': 'fioAddress'},
    {'1': 'bundle_sets', '3': 2, '4': 1, '5': 4, '10': 'bundleSets'},
    {'1': 'fee', '3': 3, '4': 1, '5': 4, '10': 'fee'},
  ],
};

/// Descriptor for `Action`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List actionDescriptor = $convert.base64Decode(
    'CgZBY3Rpb24SagoccmVnaXN0ZXJfZmlvX2FkZHJlc3NfbWVzc2FnZRgBIAEoCzInLlRXLkZJTy'
    '5Qcm90by5BY3Rpb24uUmVnaXN0ZXJGaW9BZGRyZXNzSABSGXJlZ2lzdGVyRmlvQWRkcmVzc01l'
    'c3NhZ2USWwoXYWRkX3B1Yl9hZGRyZXNzX21lc3NhZ2UYAiABKAsyIi5UVy5GSU8uUHJvdG8uQW'
    'N0aW9uLkFkZFB1YkFkZHJlc3NIAFIUYWRkUHViQWRkcmVzc01lc3NhZ2USSgoQdHJhbnNmZXJf'
    'bWVzc2FnZRgDIAEoCzIdLlRXLkZJTy5Qcm90by5BY3Rpb24uVHJhbnNmZXJIAFIPdHJhbnNmZX'
    'JNZXNzYWdlEmEKGXJlbmV3X2Zpb19hZGRyZXNzX21lc3NhZ2UYBCABKAsyJC5UVy5GSU8uUHJv'
    'dG8uQWN0aW9uLlJlbmV3RmlvQWRkcmVzc0gAUhZyZW5ld0Zpb0FkZHJlc3NNZXNzYWdlEmEKGW'
    '5ld19mdW5kc19yZXF1ZXN0X21lc3NhZ2UYBSABKAsyJC5UVy5GSU8uUHJvdG8uQWN0aW9uLk5l'
    'd0Z1bmRzUmVxdWVzdEgAUhZuZXdGdW5kc1JlcXVlc3RNZXNzYWdlEmQKGnJlbW92ZV9wdWJfYW'
    'RkcmVzc19tZXNzYWdlGAYgASgLMiUuVFcuRklPLlByb3RvLkFjdGlvbi5SZW1vdmVQdWJBZGRy'
    'ZXNzSABSF3JlbW92ZVB1YkFkZHJlc3NNZXNzYWdlEnIKIHJlbW92ZV9hbGxfcHViX2FkZHJlc3'
    'Nlc19tZXNzYWdlGAcgASgLMiguVFcuRklPLlByb3RvLkFjdGlvbi5SZW1vdmVBbGxQdWJBZGRy'
    'ZXNzSABSHHJlbW92ZUFsbFB1YkFkZHJlc3Nlc01lc3NhZ2USdgogYWRkX2J1bmRsZWRfdHJhbn'
    'NhY3Rpb25zX21lc3NhZ2UYCCABKAsyKy5UVy5GSU8uUHJvdG8uQWN0aW9uLkFkZEJ1bmRsZWRU'
    'cmFuc2FjdGlvbnNIAFIdYWRkQnVuZGxlZFRyYW5zYWN0aW9uc01lc3NhZ2UaeAoSUmVnaXN0ZX'
    'JGaW9BZGRyZXNzEh8KC2Zpb19hZGRyZXNzGAEgASgJUgpmaW9BZGRyZXNzEi8KFG93bmVyX2Zp'
    'b19wdWJsaWNfa2V5GAIgASgJUhFvd25lckZpb1B1YmxpY0tleRIQCgNmZWUYAyABKARSA2ZlZR'
    'qKAQoNQWRkUHViQWRkcmVzcxIfCgtmaW9fYWRkcmVzcxgBIAEoCVIKZmlvQWRkcmVzcxJGChBw'
    'dWJsaWNfYWRkcmVzc2VzGAIgAygLMhsuVFcuRklPLlByb3RvLlB1YmxpY0FkZHJlc3NSD3B1Ym'
    'xpY0FkZHJlc3NlcxIQCgNmZWUYAyABKARSA2ZlZRqNAQoQUmVtb3ZlUHViQWRkcmVzcxIfCgtm'
    'aW9fYWRkcmVzcxgBIAEoCVIKZmlvQWRkcmVzcxJGChBwdWJsaWNfYWRkcmVzc2VzGAIgAygLMh'
    'suVFcuRklPLlByb3RvLlB1YmxpY0FkZHJlc3NSD3B1YmxpY0FkZHJlc3NlcxIQCgNmZWUYAyAB'
    'KARSA2ZlZRpIChNSZW1vdmVBbGxQdWJBZGRyZXNzEh8KC2Zpb19hZGRyZXNzGAEgASgJUgpmaW'
    '9BZGRyZXNzEhAKA2ZlZRgDIAEoBFIDZmVlGl4KCFRyYW5zZmVyEigKEHBheWVlX3B1YmxpY19r'
    'ZXkYASABKAlSDnBheWVlUHVibGljS2V5EhYKBmFtb3VudBgCIAEoBFIGYW1vdW50EhAKA2ZlZR'
    'gDIAEoBFIDZmVlGnUKD1JlbmV3RmlvQWRkcmVzcxIfCgtmaW9fYWRkcmVzcxgBIAEoCVIKZmlv'
    'QWRkcmVzcxIvChRvd25lcl9maW9fcHVibGljX2tleRgCIAEoCVIRb3duZXJGaW9QdWJsaWNLZX'
    'kSEAoDZmVlGAMgASgEUgNmZWUa1AEKD05ld0Z1bmRzUmVxdWVzdBIkCg5wYXllcl9maW9fbmFt'
    'ZRgBIAEoCVIMcGF5ZXJGaW9OYW1lEioKEXBheWVyX2Zpb19hZGRyZXNzGAIgASgJUg9wYXllck'
    'Zpb0FkZHJlc3MSJAoOcGF5ZWVfZmlvX25hbWUYAyABKAlSDHBheWVlRmlvTmFtZRI3Cgdjb250'
    'ZW50GAQgASgLMh0uVFcuRklPLlByb3RvLk5ld0Z1bmRzQ29udGVudFIHY29udGVudBIQCgNmZW'
    'UYBSABKARSA2ZlZRpsChZBZGRCdW5kbGVkVHJhbnNhY3Rpb25zEh8KC2Zpb19hZGRyZXNzGAEg'
    'ASgJUgpmaW9BZGRyZXNzEh8KC2J1bmRsZV9zZXRzGAIgASgEUgpidW5kbGVTZXRzEhAKA2ZlZR'
    'gDIAEoBFIDZmVlQg8KDW1lc3NhZ2Vfb25lb2Y=');

@$core.Deprecated('Use chainParamsDescriptor instead')
const ChainParams$json = {
  '1': 'ChainParams',
  '2': [
    {'1': 'chain_id', '3': 1, '4': 1, '5': 12, '10': 'chainId'},
    {'1': 'head_block_number', '3': 2, '4': 1, '5': 4, '10': 'headBlockNumber'},
    {'1': 'ref_block_prefix', '3': 3, '4': 1, '5': 4, '10': 'refBlockPrefix'},
  ],
};

/// Descriptor for `ChainParams`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List chainParamsDescriptor = $convert.base64Decode(
    'CgtDaGFpblBhcmFtcxIZCghjaGFpbl9pZBgBIAEoDFIHY2hhaW5JZBIqChFoZWFkX2Jsb2NrX2'
    '51bWJlchgCIAEoBFIPaGVhZEJsb2NrTnVtYmVyEigKEHJlZl9ibG9ja19wcmVmaXgYAyABKARS'
    'DnJlZkJsb2NrUHJlZml4');

@$core.Deprecated('Use signingInputDescriptor instead')
const SigningInput$json = {
  '1': 'SigningInput',
  '2': [
    {'1': 'expiry', '3': 1, '4': 1, '5': 13, '10': 'expiry'},
    {'1': 'chain_params', '3': 2, '4': 1, '5': 11, '6': '.TW.FIO.Proto.ChainParams', '10': 'chainParams'},
    {'1': 'private_key', '3': 3, '4': 1, '5': 12, '10': 'privateKey'},
    {'1': 'tpid', '3': 4, '4': 1, '5': 9, '10': 'tpid'},
    {'1': 'action', '3': 5, '4': 1, '5': 11, '6': '.TW.FIO.Proto.Action', '10': 'action'},
    {'1': 'owner_public_key', '3': 6, '4': 1, '5': 9, '10': 'ownerPublicKey'},
  ],
};

/// Descriptor for `SigningInput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingInputDescriptor = $convert.base64Decode(
    'CgxTaWduaW5nSW5wdXQSFgoGZXhwaXJ5GAEgASgNUgZleHBpcnkSPAoMY2hhaW5fcGFyYW1zGA'
    'IgASgLMhkuVFcuRklPLlByb3RvLkNoYWluUGFyYW1zUgtjaGFpblBhcmFtcxIfCgtwcml2YXRl'
    'X2tleRgDIAEoDFIKcHJpdmF0ZUtleRISCgR0cGlkGAQgASgJUgR0cGlkEiwKBmFjdGlvbhgFIA'
    'EoCzIULlRXLkZJTy5Qcm90by5BY3Rpb25SBmFjdGlvbhIoChBvd25lcl9wdWJsaWNfa2V5GAYg'
    'ASgJUg5vd25lclB1YmxpY0tleQ==');

@$core.Deprecated('Use signingOutputDescriptor instead')
const SigningOutput$json = {
  '1': 'SigningOutput',
  '2': [
    {'1': 'json', '3': 1, '4': 1, '5': 9, '10': 'json'},
    {'1': 'error', '3': 2, '4': 1, '5': 14, '6': '.TW.Common.Proto.SigningError', '10': 'error'},
    {'1': 'error_message', '3': 3, '4': 1, '5': 9, '10': 'errorMessage'},
    {'1': 'action_name', '3': 4, '4': 1, '5': 9, '10': 'actionName'},
  ],
};

/// Descriptor for `SigningOutput`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signingOutputDescriptor = $convert.base64Decode(
    'Cg1TaWduaW5nT3V0cHV0EhIKBGpzb24YASABKAlSBGpzb24SMwoFZXJyb3IYAiABKA4yHS5UVy'
    '5Db21tb24uUHJvdG8uU2lnbmluZ0Vycm9yUgVlcnJvchIjCg1lcnJvcl9tZXNzYWdlGAMgASgJ'
    'UgxlcnJvck1lc3NhZ2USHwoLYWN0aW9uX25hbWUYBCABKAlSCmFjdGlvbk5hbWU=');

